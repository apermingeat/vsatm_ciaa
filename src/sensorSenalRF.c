/*
 * sensorSenalRF.c
 *
 *  Created on: 01/12/2014
 *      Author: alejandro
 */

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup VSATM-CIAA
 ** @{ */
/** \addtogroup Drivers
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

#include "ciaaPOSIX_stdio.h"  /* <= device handler header */
#include "ciaaPOSIX_string.h" /* <= string header */
#include "queue.h"
#include "eventos.h"
#include "sensorSenalRF.h"


void sensorSenalRFUpdate()
{
	uint8_t	atenuacionParteEntera, atenuacionParteDecimal;
	queueElementT msg;
	//Aqui se debe consultar al sensor de señal RF la atenuación y dejarlos
	//en las variables atenuacionParteEntera, atenuacionParteDecimal
	atenuacionParteEntera = -23;
	atenuacionParteDecimal = 2;


	msg.eventID = EV_NUEVO_VAL_SENSOR_RF;
	msg.data[0] = atenuacionParteEntera;
	msg.data[1] = atenuacionParteDecimal;

	queuePut(&ListaEventos,msg,NO_BLOCKING_QUEUE);

}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
