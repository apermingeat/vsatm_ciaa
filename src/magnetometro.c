/*
 * magnetometro.c
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
#include "magnetometro.h"

/** \brief Update Magnetometer Values
 *
 * This function updates the Magnetometer values by reading the hardware.
 * The read value is queued into ListaEventos queue.
 *
 * \returns void
 *
 */
void magnetometroUpdate()
{
	uint8_t	gradosParteEntera, gradosParteDecimal;
	queueElementT msg;
	//Aqui se debe consultar al magnetometro los grados de inclinación y dejarlos
	//en las variables gradosParteEntera, gradosParteDecimal
	gradosParteEntera = 87;
	gradosParteDecimal = 4;

	/*prepare the message with updated values*/
	msg.eventID = EV_NUEVO_VAL_MAGNETOMETRO;
	msg.data[0] = gradosParteEntera;
	msg.data[1] = gradosParteDecimal;

	/*send a message to ListaEventos queue with updated values*/
	queuePut(&ListaEventos,msg,NO_BLOCKING_QUEUE);

}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
