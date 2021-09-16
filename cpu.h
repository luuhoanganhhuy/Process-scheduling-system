#include "priority_queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>


typedef struct cpu {
    int id;
	float currentProccess;
    Proccess* queue; 
} CPU;

int getTotalRemainingTime(Proccess** head);
