#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>



// C code to implement Priority Queue
// using Linked List

// Node
typedef struct proccess {
	float id;
    int remainingTime;
    int arrivingTime;
    char p;
    int executionTime;

	// Lower values indicate higher priority
	//int priority;

	struct proccess* next;

    //
    int completion;
    int waiting;
    int turnaround;

} Proccess;

typedef struct cpu {
    int id;
	float currentProccess;
    Proccess* queue; 
} CPU;

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

int getTotalRemainingTime(Proccess** head){
    Proccess* temp = *head;
    int total =0;
    while (temp != NULL){
        total += temp -> remainingTime;
        temp = temp -> next;
    }
    return total;
}


void push_waiting(Proccess** head, float id,int remainingTime, int arrivingTime, char p,int executionTime)
{
	Proccess* start = (*head);

	// Create new Node
	Proccess* temp = newProccess(id,remainingTime,arrivingTime,p, executionTime);

	// Special Case: The head of list has lesser
	// priority than new node. So insert new
	// node before head node and change head node.
	if (((*head)->arrivingTime > arrivingTime) || ((*head)->arrivingTime == arrivingTime && (*head)->remainingTime > remainingTime) ||((*head)->arrivingTime == arrivingTime && (*head)->remainingTime == remainingTime && (*head)-> id > id)) {

		// Insert New Node before head
		temp->next = *head;
		(*head) = temp;
	}
	else {

		// Traverse the list and find a
		// position to insert new node
		while (start->next != NULL &&
            ((start->next->arrivingTime < arrivingTime) || (start->next->arrivingTime == arrivingTime && start->next->remainingTime < remainingTime) ||(start->next->arrivingTime == arrivingTime && start->next->remainingTime == remainingTime && start->next-> id < id))){
			start = start->next;
		}

		// Either at the ends of the list
		// or at required position
		temp->next = start->next;
		start->next = temp;
	}
}

int main(int argc, char **argv) {

    // Need to redo the reading argv part (using the -f or -c to defind their following path)
    int num_cpu ;
    FILE *fp;
    for (int i = 0; i < argc; i++){
        if (!strcmp(argv[i],"-p")){
            num_cpu = atoi(argv[i+1]);
        }
        if (!strcmp(argv[i],"-f")){
            fp = fopen(argv[i+1], "r");
            if (fp == NULL){
                printf("Could not open file %s",argv[i+1]);
                return 1;
            }
        }
            

    }

    
    

    // init value of the linked list 
    int temp_arrival_time, temp_remaining_time,num_parallelable=0;
    float temp_id, avg_overhead =0, max_overhead=0;
    char temp_p;
    int count=0,time= 0,num_processess=0,end;
    double total_turnaround= 0 ;


    
    //printf("Number of cpu: %d\n", num_cpu);
    CPU* cpu[num_cpu];


    // init value for cpu
    for (int i =0; i< num_cpu;i++){
        cpu[i] = (CPU*)malloc(sizeof(CPU));
        cpu[i] ->id = i;
        cpu[i] -> queue = NULL;
    }
    Proccess*  waiting_process = NULL;
    while (fscanf(fp, "%d %f %d %c",&temp_arrival_time,&temp_id,&temp_remaining_time,&temp_p) == 4){
        if (waiting_process == NULL || isEmpty(&waiting_process)){
                    waiting_process = newProccess(temp_id,temp_remaining_time,temp_arrival_time,temp_p,temp_remaining_time);
        }else{
                push_waiting(&waiting_process,temp_id,temp_remaining_time,temp_arrival_time,temp_p,temp_remaining_time);
        }
    }
    // read all remaining value
    while (!isEmpty(&waiting_process)){
        //while (!isEmpty(&(cpu[0] -> queue)))
        for(; true; time++)
        {
            float temp_id = peek(&waiting_process) -> id;
            int temp_remaining_time = peek(&waiting_process) -> remainingTime;
            int temp_arrival_time = peek(&waiting_process) -> arrivingTime;
            char temp_p = peek(&waiting_process) -> p;
            // assign new process to cpu
            if (temp_arrival_time == time)
            {   
                if (temp_p == 'p'){
                    num_parallelable++;
                    num_processess++;
                    //printf("THis one is parallel: %f: temp_remaing: %d\n",temp_id,temp_remaining_time);
                    int temp_remaining_time_sub = ceil((double)temp_remaining_time/num_cpu) +1;
                    for (int i=0; i < num_cpu; i++)
                    {  
                        if (i != 0)
                            temp_id += 0.1;                      
                        int min = 99999,assign_to;

                        for (int i =0; i< num_cpu;i++)
                        {
                            if ((getTotalRemainingTime(&(cpu[i] -> queue)) < min) || ((getTotalRemainingTime(&(cpu[i] -> queue)) == min) && cpu[i] -> id < assign_to  ) )
                            {
                                min = getTotalRemainingTime(&(cpu[i] -> queue));
                                assign_to = cpu[i] -> id;
                            }   
                        }

                        if (cpu[assign_to] -> queue == NULL){
                            cpu[assign_to] -> currentProccess = 0;
                            Proccess*  pq = newProccess(temp_id,temp_remaining_time_sub,temp_arrival_time,temp_p,temp_remaining_time);
                            cpu[assign_to] -> queue = pq;
                        }
                        else{
                            cpu[assign_to] -> currentProccess = peek(&(cpu[assign_to] -> queue)) ->id ;
                            push(&(cpu[assign_to] -> queue), temp_id,temp_remaining_time_sub,temp_arrival_time,temp_p,temp_remaining_time);
                        }
                        
                    }
                    pop(&waiting_process);
                    break;

                } else {
                    int min = 99999,assign_to;

                    for (int i =0; i< num_cpu;i++)
                    {
                        if ((getTotalRemainingTime(&(cpu[i] -> queue)) < min) || ((getTotalRemainingTime(&(cpu[i] -> queue)) == min) && cpu[i] -> id < assign_to  ) ){
                            min = getTotalRemainingTime(&(cpu[i] -> queue));
                            assign_to = cpu[i] -> id;
                        }
                    }

                    if (cpu[assign_to] -> queue == NULL){
                        cpu[assign_to] -> currentProccess = 0;
                        Proccess*  pq = newProccess(temp_id,temp_remaining_time,temp_arrival_time,temp_p,temp_remaining_time);
                        cpu[assign_to] -> queue = pq;
                    
                    }
                    else{
                        cpu[assign_to] -> currentProccess = peek(&(cpu[assign_to] -> queue)) ->id ;
                        push(&(cpu[assign_to] -> queue), temp_id,temp_remaining_time,temp_arrival_time,temp_p,temp_remaining_time);
                    }
                    pop(&waiting_process);
                    
                    num_processess++;             
                    break;
                }
                
            }


            // execute the process
            for (int i =0; i< num_cpu;i++){

                if (!isEmpty(&(cpu[i] -> queue))){
                    //printf("prev id: %d , Lowest id: %d, time: %d\n",cpu[i] -> currentProccess,peek(&(cpu[i] -> queue)) -> id,time);
                    if (cpu[i] -> currentProccess != peek(&(cpu[i] -> queue)) -> id){
                        if  (peek(&(cpu[i] -> queue)) -> p == 'p')
                            printf("%d,RUNNING,pid=%.1f,remaining_time=%d,cpu=%d\n",time, peek(&(cpu[i] -> queue)) -> id, peek(&(cpu[i] -> queue)) -> remainingTime, cpu[i]->id);
                        else
                            printf("%d,RUNNING,pid=%d,remaining_time=%d,cpu=%d\n",time, (int)peek(&(cpu[i] -> queue)) -> id, peek(&(cpu[i] -> queue)) -> remainingTime, cpu[i]->id);

                        cpu[i] -> currentProccess = peek(&(cpu[i] -> queue)) -> id;
                    }
                    
                    peek(&(cpu[i] -> queue)) -> remainingTime --;

                    if(peek(&(cpu[i] -> queue)) -> remainingTime == 0)
                    {
                        end=time+1;
                        peek(&(cpu[i] -> queue)) ->completion = end;
                        peek(&(cpu[i] -> queue)) ->turnaround = end - peek(&(cpu[i] -> queue)) -> arrivingTime;
                        //printf("Turnarond time: %d\n",peek(&cpu[i] -> queue) ->turnaround);
                        //avg_overhead += (float)(peek(&cpu[i] -> queue) ->turnaround)/peek(&(cpu[i] -> queue)) -> executionTime;
                        if (max_overhead <  (float)(peek(&cpu[i] -> queue) ->turnaround)/peek(&(cpu[i] -> queue)) -> executionTime)
                            max_overhead = (float)(peek(&cpu[i] -> queue) ->turnaround)/peek(&(cpu[i] -> queue)) -> executionTime;

                        if  (peek(&(cpu[i] -> queue)) -> p == 'p'){
                            bool allFinish = true;
                            //printf("Id of this parrel: %.1f\n",peek(&(cpu[i] -> queue)) -> id);
                            for (int j =0; j< num_cpu;j++){
                                if (j != i){
                                    //printf("CPU to compare: %d\n",cpu[j]->id);
                                    Proccess* temp = cpu[j] -> queue;
                                    while (temp != NULL){
                                        if ((int)temp -> id ==  (int)peek(&(cpu[i] -> queue)) ->id){
                                            allFinish = false;
                                        }
                                        temp = temp -> next;                                        
                                    }
                                }                               
                            }
                            if (allFinish){
                                count++;
                                avg_overhead += (float)(peek(&cpu[i] -> queue) ->turnaround)/peek(&(cpu[i] -> queue)) -> executionTime;
                                total_turnaround += peek(&cpu[i] -> queue) ->turnaround;
                                printf("%d,FINISHED,pid=%d,proc_remaining=%d\n",time+1,(int)peek(&(cpu[i] -> queue)) ->id ,num_processess-count);
                            }
                        }else{
                            count++;
                            avg_overhead += (float)(peek(&cpu[i] -> queue) ->turnaround)/peek(&(cpu[i] -> queue)) -> executionTime;
                            total_turnaround += peek(&cpu[i] -> queue) ->turnaround;
                            printf("%d,FINISHED,pid=%d,proc_remaining=%d\n",time+1,(int)peek(&(cpu[i] -> queue)) ->id ,num_processess-count);
                        }
                        pop(&(cpu[i] -> queue));
                    }
                }               
            }
		}

    }
    // execute process when all processes already in queue
    //printf("Num proccesses %d\n",num_processess);

    /* Print all the elements in the linked list */
    for(; count!=num_processess; time++){
            for (int i =0; i< num_cpu;i++){

                if (!isEmpty(&(cpu[i] -> queue))){
                    printf("prev id: %d , Lowest id: %.1f, time: %d\n",cpu[i] -> currentProccess,peek(&(cpu[i] -> queue)) -> id,time);
                    if (cpu[i] -> currentProccess != peek(&(cpu[i] -> queue)) -> id){
                        if  (peek(&(cpu[i] -> queue)) -> p == 'p')
                            printf("%d,RUNNING,pid=%.1f,remaining_time=%d,cpu=%d\n",time, peek(&(cpu[i] -> queue)) -> id, peek(&(cpu[i] -> queue)) -> remainingTime, cpu[i]->id);
                        else
                            printf("%d,RUNNING,pid=%d,remaining_time=%d,cpu=%d\n",time, (int)peek(&(cpu[i] -> queue)) -> id, peek(&(cpu[i] -> queue)) -> remainingTime, cpu[i]->id);

                        cpu[i] -> currentProccess = peek(&(cpu[i] -> queue)) -> id;
                    }
                    
                    peek(&(cpu[i] -> queue)) -> remainingTime --;

                    if(peek(&(cpu[i] -> queue)) -> remainingTime == 0)
                    {
                        end=time+1;
                        peek(&(cpu[i] -> queue)) ->completion = end;
                        peek(&(cpu[i] -> queue)) ->turnaround = end - peek(&(cpu[i] -> queue)) -> arrivingTime;
                        //printf("Turnarond time: %d\n",peek(&cpu[i] -> queue) ->turnaround);
                        if (max_overhead <  (float)(peek(&cpu[i] -> queue) ->turnaround)/peek(&(cpu[i] -> queue)) -> executionTime)
                            max_overhead = (float)(peek(&cpu[i] -> queue) ->turnaround)/peek(&(cpu[i] -> queue)) -> executionTime;


                        if  (peek(&(cpu[i] -> queue)) -> p == 'p'){
                            bool allFinish = true;
                            //printf("Id of this parrel: %.1f\n",peek(&(cpu[i] -> queue)) -> id);
                            for (int j =0; j< num_cpu;j++){
                                if (j != i){
                                    //printf("CPU to compare: %d\n",cpu[j]->id);
                                    Proccess* temp = cpu[j] -> queue;
                                    while (temp != NULL){
                                        if ((int)temp -> id ==  (int)peek(&(cpu[i] -> queue)) ->id){
                                            allFinish = false;
                                        }
                                        temp = temp -> next;                                        
                                    }
                                }                               
                            }
                            if (allFinish){
                                count++;
                                avg_overhead += (float)(peek(&cpu[i] -> queue) ->turnaround)/peek(&(cpu[i] -> queue)) -> executionTime;
                                total_turnaround += peek(&cpu[i] -> queue) ->turnaround;
                                printf("%d,FINISHED,pid=%d,proc_remaining=%d\n",time+1,(int)peek(&(cpu[i] -> queue)) ->id ,num_processess-count);
                            }
                        }else{
                            count++;
                            avg_overhead += (float)(peek(&cpu[i] -> queue) ->turnaround)/peek(&(cpu[i] -> queue)) -> executionTime;
                            total_turnaround += peek(&cpu[i] -> queue) ->turnaround;
                            printf("%d,FINISHED,pid=%d,proc_remaining=%d\n",time+1,(int)peek(&(cpu[i] -> queue)) ->id ,num_processess-count);
                        }
                        pop(&(cpu[i] -> queue));
                    }
                }               
            }
    }
    printf("Turnaround time %d\n",(int)ceil(total_turnaround/num_processess));
    printf("Time overhead %.2f %.2f\n",max_overhead,avg_overhead/num_processess);
    printf("Makespan %d",end);
    fclose(fp);

    
    return 0;


}
