/*
 * queue.c
 *
 *  Created on: 01/12/2014
 *      Author: alejandro
 */

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup VSATM-CIAA
 ** @{ */
/** \addtogroup Queue
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

/** \brief System event queue
 *
 */
queue_t ListaEventos;


/** \brief Queue initialization
 *
 * This function initialize the internal structure of a given queue passed as parameter.
 *
 *\param queue to be initialized
 *
 * \returns void
 *
 */
void queueInit(queue_t * queue)
{
	queue->front = 0;
	queue->rear = 0;
	queue->count = 0;
}

/** \brief Queue empty checking
 *
 * This function checks if a given queue passed as parameter is empty.
 *
 * \param queue to be checked
 *
 * \returns 0 if queue is not empty / value different to 0 if the queue is empty
 *
 */
uint8_t queueIsEmpty(queue_t * queue)
{
	return (queue->count == 0);
}

/** \brief Queue space available checking
 *
 * This function checks if a given queue passed as parameter has available space to insert an element.
 *
 * \param queue to be checked
 *
 * \returns 0 if queue is full / value different to 0 if the queue has available space
 *
 */
uint8_t queueHasAvailableSpace(queue_t * queue)
{
	return (queue->count < MAX_QUEUE_SIZE);
}

/** \brief Put an element in the queue
 *
 * This function inserts an element in the rear of a given queue passed as parameter.
 *
 * \param queue a given queue where the element will be inserted.
 * \param msg an element to be inserted.
 * \param blocking determines if this function is or is not blocking. Possible values are:
 * 			BLOCKING_QUEUE, this function must return -1 if the queue is full /
 * 			NO_BLOCKING_QUEUE, if the queue is full this function must wait until the data could be queued
 *
 * \returns 0 the data was queued successfully / -1 data not queued due to full queue
 *
 */
uint8_t queuePut(queue_t * queue, queueElementT msg, uint8_t blocking)
{
	if(!queueHasAvailableSpace(queue))
	{
		if(blocking == BLOCKING_QUEUE)
		{
			//the queue is blocking and is not available space
			while (!queueHasAvailableSpace(queue))
			{

			}
		}
		else
		{
			return(-1);
		}
	}
	//insert msg in the queue
	queue->contents[queue->rear] = msg;
	queue->rear++;
	if (queue->rear >= MAX_QUEUE_SIZE)
	{
		queue->rear = 0;
	}
	queue->count++;
	return(0);
}

/** \brief Get an element from the queue
 *
 * This function gets an element from the front of a given queue passed as parameter.
 *
 * \param queue a given queue from where the element will be read.
 * \param msg pinter where the read element will be returned.
 * \param blocking determines if this function is or is not blocking. Possible values are:
 * 			BLOCKING_QUEUE, this function must return -1 if the queue is empty /
 * 			NO_BLOCKING_QUEUE, if the queue is empty this function must wait until the queue has a data.
 *
 * \returns 0 the data was gotten from queue successfully / -1 data was not gotten from the queue due to empty queue
 *
 */
uint8_t queueGet(queue_t * queue, queueElementT *msg, uint8_t blocking)
{
	if(queueIsEmpty(queue))
	{
		if(blocking == BLOCKING_QUEUE)
		{
			//the queue is empty and is blocking
			while (queueIsEmpty(queue))
			{

			}
		}
		else
		{
			return(-1);
		}
	}
	//get msg from the queue
	*msg = queue->contents[queue->front];
	queue->front++;
	if (queue->front >= MAX_QUEUE_SIZE)
	{
		queue->front = 0;
	}
	queue->count--;
	return(0);
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/
