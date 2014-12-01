/*
 * inclinometro.c
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
#include "inclinometro.h"

/** \brief Update Inclinometer Values
 *
 * This function updates the Inclinometer values by reading the hardware.
 * The read value is queued into ListaEventos queue.
 *
 * \returns void
 *
 */

void inclinometroUpdate()
{
	uint8_t	gradosParteEntera, gradosParteDecimal;
	queueElementT msg;
	//Aqui se debe consultar al inclinometro los grados de inclinación y dejarlos
	//en las variables gradosParteEntera, gradosParteDecimal
	gradosParteEntera = 5;
	gradosParteDecimal = 13;

	/*prepare the message with updated values*/
	msg.eventID = EV_NUEVO_VAL_INCLINOMETRO;
	msg.data[0] = gradosParteEntera;
	msg.data[1] = gradosParteDecimal;

	/*send a message to ListaEventos queue with updated values*/
	queuePut(&ListaEventos,msg,NO_BLOCKING_QUEUE);

}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
