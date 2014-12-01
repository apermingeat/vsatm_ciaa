/* Copyright 2014, Mariano Cerdeiro
 * Copyright 2014, Pablo Ridolfi
 * Copyright 2014, Juan Cecconi
 * Copyright 2014, Gustavo Muro
 *
 * This file is part of CIAA Firmware.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/** \brief VSATM-CIAA source file
 **
 ** This is an implementation of VSATM over the CIAA Firmware.
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup VSATM-CIAA
 ** @{ */
/** \addtogroup OS Tasks
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * AP         Alejandro Permingeat
 */

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 */

/*==================[inclusions]=============================================*/
#include "os.h"               /* <= operating system header */
#include "ciaaPOSIX_stdio.h"  /* <= device handler header */
#include "ciaaPOSIX_string.h" /* <= string header */
#include "ciaak.h"            /* <= ciaa kernel header */
#include "vsatm_ciaa.h"         /* <= own header */
#include "queue.h"

#include "inclinometro.h"
#include "magnetometro.h"
#include "sensorSenalRF.h"
#include "sensorFinCarrera.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/
/** \brief File descriptor for digital input ports
 *
 * Device path /dev/dio/in/0
 */
static int32_t fd_in;

/** \brief File descriptor for digital output ports
 *
 * Device path /dev/dio/out/0
 */
static int32_t fd_out;

/** \brief File descriptor of the USB uart
 *
 * Device path /dev/serial/uart/1
 */
static int32_t fd_uart1;

/** \brief File descriptor of the RS232 uart
 *
 * Device path /dev/serial/uart/2
 */
static int32_t fd_uart2;

/** \brief Sincronizador Task Counter
 *
 */
static uint32_t SincronizadorTask_Counter;

/** \brief Control General Task Counter
 *
 */
static uint32_t ControlGeneralTask_Counter;

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
/** \brief Main function
 *
 * This is the main entry point of the software.
 *
 * \returns 0
 *
 * \remarks This function never returns. Return value is only to avoid compiler
 *          warnings or errors.
 */
int main(void)
{
   /* Starts the operating system in the Application Mode 1 */
   /* This example has only one Application Mode */
   StartOS(AppMode1);

   /* StartOs shall never returns, but to avoid compiler warnings or errors
    * 0 is returned */
   return 0;
}

/** \brief Error Hook function
 *
 * This fucntion is called from the os if an os interface (API) returns an
 * error. Is for debugging proposes. If called this function triggers a
 * ShutdownOs which ends in a while(1).
 *
 * The values:
 *    OSErrorGetServiceId
 *    OSErrorGetParam1
 *    OSErrorGetParam2
 *    OSErrorGetParam3
 *    OSErrorGetRet
 *
 * will provide you the interface, the input parameters and the returned value.
 * For more details see the OSEK specification:
 * http://portal.osek-vdx.org/files/pdf/specs/os223.pdf
 *
 */
void ErrorHook(void)
{
   ciaaPOSIX_printf("ErrorHook was called\n");
   ciaaPOSIX_printf("Service: %d, P1: %d, P2: %d, P3: %d, RET: %d\n", OSErrorGetServiceId(), OSErrorGetParam1(), OSErrorGetParam2(), OSErrorGetParam3(), OSErrorGetRet());
   ShutdownOS(0);
}

/** \brief Initial task
 *
 * This task is started automatically in the application mode 1.
 */
TASK(InitTask)
{
   /* init CIAA kernel and devices */
   ciaak_start();

   ciaaPOSIX_printf("Init Task...\n");
   /* open CIAA digital inputs */
   fd_in = ciaaPOSIX_open("/dev/dio/in/0", O_RDONLY);

   /* open CIAA digital outputs */
   fd_out = ciaaPOSIX_open("/dev/dio/out/0", O_RDWR);

   /* open UART connected to USB bridge (FT2232) */
   fd_uart1 = ciaaPOSIX_open("/dev/serial/uart/1", O_RDWR);

   /* open UART connected to RS232 connector */
   fd_uart2 = ciaaPOSIX_open("/dev/serial/uart/2", O_RDWR);

   /* change baud rate for uart usb */
   ciaaPOSIX_ioctl(fd_uart1, ciaaPOSIX_IOCTL_SET_BAUDRATE, (void *)ciaaBAUDRATE_115200);

   /* change FIFO TRIGGER LEVEL for uart usb */
   ciaaPOSIX_ioctl(fd_uart1, ciaaPOSIX_IOCTL_SET_FIFO_TRIGGER_LEVEL, (void *)ciaaFIFO_TRIGGER_LEVEL3);

   queueInit(&ListaEventos);

   /* activate example tasks */
   SincronizadorTask_Counter = 0;
   ControlGeneralTask_Counter = 0;
   SetRelAlarm(ActivateSincronizadorTask, 100, 100);

   SetRelAlarm(ActivateControlGeneralTask, 120, 20);

   /* Activates the SerialEchoTask task */
   ActivateTask(SerialEchoTask);

   /* end InitTask */
   TerminateTask();
}

/** \brief Serial Echo Task
 *
 * This tasks waits for input data from fd_uart1 and writes the received data
 * to fd_uart1 and fd_uart2. This taks alos blinkgs the output 5.
 *
 */
TASK(SerialEchoTask)
{
   int8_t buf[20];   /* buffer for uart operation              */
   uint8_t outputs;  /* to store outputs status                */
   int32_t ret;      /* return value variable for posix calls  */

   ciaaPOSIX_printf("SerialEchoTask...\n");
   /* send a message to the world :) */
   char message[] = "Hi! :)\nSerialEchoTask: Waiting for characters...\n";
   ciaaPOSIX_write(fd_uart1, message, ciaaPOSIX_strlen(message));

   while(1)
   {
      /* wait for any character ... */
      ret = ciaaPOSIX_read(fd_uart1, buf, 20);

      if(ret > 0)
      {
         /* ... and write them to the same device */
         ciaaPOSIX_write(fd_uart1, buf, ret);

         /* also write them to the other device */
         ciaaPOSIX_write(fd_uart2, buf, ret);
      }

      /* blink output 5 with each loop */
      ciaaPOSIX_read(fd_out, &outputs, 1);
      outputs ^= 0x20;
      ciaaPOSIX_write(fd_out, &outputs, 1);
   }
}

/** \brief Sincronizador Task
 *
 * This task is activated by the Alarm ActivateSincronizadorTask.
 * This task updates inclinometer, magnetometer, RF signal sensor and sensor fin de carrera.
 * Each of those updates routines puts values in ListaEventos queue.
 */
TASK(SincronizadorTask)
{
   /* Print Task info */
   SincronizadorTask_Counter++;
   ciaaPOSIX_printf("Sincronizador Task: %d\n", SincronizadorTask_Counter);

   /* update inclinometer values*/
   inclinometroUpdate();
   /* update magnetonometer values*/
   magnetometroUpdate();
   /* update RF signal sensor values*/
   sensorSenalRFUpdate();
   /* update  sensor fin de carrera values*/
   sensorFinCarreraUpdate();
   /* end PeriodicTask */
   TerminateTask();
}

/** \brief Control General Task
 *
 * This task is activated by the Alarm ActivateControlGeneralTask.
 * This task checks periodically the ListaEventos queue.
 * If this queue has an element, this task process it and feeds
 * a state chart (not implemented yet)
  */
TASK(ControlGeneralTask)
{
	queueElementT msg;

	ControlGeneralTask_Counter++;

	/*Checks the ListaEventos queue*/
   if (!(queueGet(&ListaEventos, &msg, NO_BLOCKING_QUEUE)))
   {
	   /*A new message is in ListaEventos queue*/
	   ciaaPOSIX_printf("ControlGeneral Task: %d - EventID: %d value0: %d value1: %d\n", ControlGeneralTask_Counter, msg.eventID, msg.data[0], msg.data[1]);
   }
   /* end PeriodicTask */
   TerminateTask();
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

