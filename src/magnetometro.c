/*
 * magnetometro.c
 *
 *  Created on: 01/12/2014
 *      Author: alejandro
 */

#include "ciaaPOSIX_stdio.h"  /* <= device handler header */
#include "ciaaPOSIX_string.h" /* <= string header */
#include "queue.h"
#include "eventos.h"
#include "magnetometro.h"


void magnetometroUpdate()
{
	uint8_t	gradosParteEntera, gradosParteDecimal;
	queueElementT msg;
	//Aqui se debe consultar al magnetometro los grados de inclinación y dejarlos
	//en las variables gradosParteEntera, gradosParteDecimal
	gradosParteEntera = 87;
	gradosParteDecimal = 4;


	msg.eventID = EV_NUEVO_VAL_MAGNETOMETRO;
	msg.data[0] = gradosParteEntera;
	msg.data[1] = gradosParteDecimal;

	queuePut(&ListaEventos,msg,NO_BLOCKING_QUEUE);

}
