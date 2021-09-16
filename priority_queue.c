#include "priority_queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

// Function to Create A New Node
Proccess* newProccess(float id,int remainingTime, int arrivingTime, char p,int executionTime)
{
	Proccess* temp = (Proccess*)malloc(sizeof(Proccess));
	temp->id = id;
    temp->arrivingTime = arrivingTime;
    temp->p = p;
	temp->remainingTime = remainingTime;
    temp->executionTime = executionTime;
	temp->next = NULL;

	return temp;
}

// Return the value at head
Proccess* peek(Proccess** head)
{
	return *head;
}

// Removes the element with the
// highest priority form the list
void pop(Proccess** head)
{
	Proccess* temp = *head;
	(*head) = (*head)->next;
	free(temp);
}

// Function to push according to priority
void push(Proccess** head, float id,int remainingTime, int arrivingTime, char p,int executionTime)
{
	Proccess* start = (*head);

	// Create new Node
	Proccess* temp = newProccess(id,remainingTime,arrivingTime,p, executionTime);

	// Special Case: The head of list has lesser
	// priority than new node. So insert new
	// node before head node and change head node.
	if (((*head)->remainingTime > remainingTime) || ((*head)->remainingTime == remainingTime &&  (*head)-> id > id)) {

		// Insert New Node before head
		temp->next = *head;
		(*head) = temp;
	}
	else {

		// Traverse the list and find a
		// position to insert new node
		while (start->next != NULL &&
			((start->next->remainingTime < remainingTime)|| (start->next->remainingTime == remainingTime &&  start->next-> id < id))) {
			start = start->next;
		}

		// Either at the ends of the list
		// or at required position
		temp->next = start->next;
		start->next = temp;
	}
}

// Function to check is list is empty
int isEmpty(Proccess** head)
{
	return (*head) == NULL;
}