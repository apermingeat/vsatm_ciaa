/*
 * sensorFinCarrera.c
 *
 *  Created on: 01/12/2014
 *      Author: alejandro
 */


#include "ciaaPOSIX_stdio.h"  /* <= device handler header */
#include "ciaaPOSIX_string.h" /* <= string header */
#include "queue.h"
#include "eventos.h"
#include "sensorFinCarrera.h"


void sensorFinCarreraUpdate()
{
	uint8_t	sensorFinCarrera;
	queueElementT msg;
	//Aqui se debe consultar al sensor de fin de carrera y dejar el valor
	//en la variable sensorFinCarrera
	sensorFinCarrera = 1;



	msg.eventID = EV_NUEVO_VAL_SENSOR_FIN_CARRERA;
	msg.data[0] = sensorFinCarrera;


	queuePut(&ListaEventos,msg,NO_BLOCKING_QUEUE);

}