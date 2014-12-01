/*
 * queue.c
 *
 *  Created on: 01/12/2014
 *      Author: alejandro
 */

#include "ciaaPOSIX_stdio.h"  /* <= device handler header */
#include "ciaaPOSIX_string.h" /* <= string header */
#include "queue.h"

void queueInit(queue_t * queue)
{
	queue->front = 0;
	queue->rear = 0;
	queue->count = 0;
}

uint8_t queueIsEmpty(queue_t * queue)
{
	return (queue->count == 0);
}

uint8_t queueHasAvailableSpace(queue_t * queue)
{
	return (queue->count < MAX_QUEUE_SIZE);
}

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
	queue->rear++;
	if (queue->rear >= MAX_QUEUE_SIZE)
	{
		queue->rear = 0;
	}
	queue->contents[queue->rear] = msg;
	queue->count++;
	return(0);
}

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
