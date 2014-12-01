/*
 * inclinometro.c
 *
 *  Created on: 01/12/2014
 *      Author: alejandro
 */

#include "ciaaPOSIX_stdio.h"  /* <= device handler header */
#include "ciaaPOSIX_string.h" /* <= string header */
#include "queue.h"
#include "eventos.h"
#include "inclinometro.h"


void inclinometroUpdate()
{
	uint8_t	gradosParteEntera, gradosParteDecimal;
	queueElementT msg;
	//Aqui se debe consultar al inclinometro los grados de inclinación y dejarlos
	//en las variables gradosParteEntera, gradosParteDecimal
	gradosParteEntera = 5;
	gradosParteDecimal = 13;


	msg.eventID = NUEVO_VAL_INCLINOMETRO;
	msg.data[0] = gradosParteEntera;
	msg.data[1] = gradosParteDecimal;

	queuePut(&ListaEventos,msg,NO_BLOCKING_QUEUE);

}
