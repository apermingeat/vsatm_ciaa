/*
 * queue.h
 *
 *  Created on: 01/12/2014
 *      Author: alejandro
 */

#ifndef QUEUE_H_
#define QUEUE_H_

#define MAX_QUEUE_SIZE  100
#define MAX_ELEMENT_DATA  4

#define BLOCKING_QUEUE		0
#define NO_BLOCKING_QUEUE	1

struct queueElement_struct
{
	uint16_t eventID;
	uint16_t data[MAX_ELEMENT_DATA];
};

typedef struct queueElement_struct queueElementT;

struct queue_struct
{
	 queueElementT contents[MAX_QUEUE_SIZE];
	 uint16_t front;
	 uint16_t rear;
	 uint16_t count;
};

typedef struct queue_struct queue_t;


uint8_t queuePut(queue_t * queue, queueElementT msg, uint8_t blocking);
uint8_t queueGet(queue_t * queue, queueElementT *msg, uint8_t blocking);

#endif /* QUEUE_H_ */
