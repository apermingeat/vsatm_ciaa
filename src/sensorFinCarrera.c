/*
 * sensorFinCarrera.c
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
#include "sensorFinCarrera.h"

/** \brief Update Fin Carrera Vales
 *
 * This function updates the Fin Carrera values by reading the hardware.
 * The read value is queued into ListaEventos queue.
 *
 * \returns void
 *
 */
void sensorFinCarreraUpdate()
{
	uint8_t	sensorFinCarrera;
	queueElementT msg;
	//Aqui se debe consultar al sensor de fin de carrera y dejar el valor
	//en la variable sensorFinCarrera
	sensorFinCarrera = 1;

	/*prepare the message with updated values*/
	msg.eventID = EV_NUEVO_VAL_SENSOR_FIN_CARRERA;
	msg.data[0] = sensorFinCarrera;

	/*send a message to ListaEventos queue with updated values*/
	queuePut(&ListaEventos,msg,NO_BLOCKING_QUEUE);

}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
