#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>



// C code to implement priority queue using linked list 
// Contributed to this website https://www.geeksforgeeks.org/priority-queue-using-linked-list/

typedef struct proccess {
    
    // proccess attributes
	float id;
    int remainingTime;
    int arrivingTime;
    char p;
    int executionTime;

    //next proccess in the linked list
	struct proccess* next;

    //track value
    int completion;
    int waiting;
    int turnaround;

} Proccess;

// struct for proccessor 
typedef struct cpu {
     id;
	float currentProccess;
    Proccess* queue; 
} CPU;

// function to create a new proccess
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

// return the proccess with the highest priority
Proccess* peek(Proccess** head)
{
	return *head;
}

// Removes the proccess with the highest priority
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

	// Create new proccess
	Proccess* temp = newProccess(id,remainingTime,arrivingTime,p, executionTime);

	// Insert at head
	if (((*head)->remainingTime > remainingTime) || ((*head)->remainingTime == remainingTime &&  (*head)-> id > id)) {
		temp->next = *head;
		(*head) = temp;
	}
	else {
		// Traverse the queue to find insertion position
		while (start->next != NULL &&
			((start->next->remainingTime < remainingTime)|| (start->next->remainingTime == remainingTime &&  start->next-> id < id))) {
			start = start->next;
		}
		temp->next = start->next;
		start->next = temp;
	}
}

// Check if the queue is empty
int isEmpty(Proccess** head)
{
	return (*head) == NULL;
}

// Get the total remaining time of each cpu
int getTotalRemainingTime(Proccess** head){
    Proccess* temp = *head;
    int total =0;
    while (temp != NULL){
        total += temp -> remainingTime;
        temp = temp -> next;
    }
    return total;
}





int main(int argc, char **argv) {

    // Read the txt input according to the position of -p and -f
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
    int temp_arrival_time, temp_remaining_time;
    float temp_id, avg_overhead =0, max_overhead=0;
    char temp_p;
    int count=0,time= 0,num_processess=0,end;
    double total_turnaround= 0 ;


    // init value for cpu
    CPU* cpu[num_cpu];
    for (int i =0; i< num_cpu;i++){
        cpu[i] = (CPU*)malloc(sizeof(CPU));
        cpu[i] ->id = i;
        cpu[i] -> queue = NULL;
    }


    // read value from input by line
    while (fscanf(fp, "%d %f %d %c",&temp_arrival_time,&temp_id,&temp_remaining_time,&temp_p) == 4){

        //count time pass by
        for(; true; time++)
        {

            // assign new process to cpu
            if (temp_arrival_time == time)
            {

                //break the proccess into subprocess when it is parralelable
                if (temp_p == 'p'){
                    num_processess++;
                    int temp_remaining_time_sub = ceil((double)temp_remaining_time/num_cpu) +1;
                    for (int i=0; i < num_cpu; i++)
                    {  
                        if (i != 0)
                            temp_id += 0.1;                      
                        int min = 99999,assign_to;

                        // get the smallest remaining time to assign cpu
                        for (int i =0; i< num_cpu;i++)
                        {
                            if ((getTotalRemainingTime(&(cpu[i] -> queue)) < min) || ((getTotalRemainingTime(&(cpu[i] -> queue)) == min) && cpu[i] -> id < assign_to  ) )
                            {
                                min = getTotalRemainingTime(&(cpu[i] -> queue));
                                assign_to = cpu[i] -> id;
                            }   
                        }

                        // push the process into queue of each cpu
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
                    break;

                } else {

                    // assign normally if proccess is not parraleable
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
                    
                    num_processess++;             
                    break;
                }
                
            }


            // execute the process
            for (int i =0; i< num_cpu;i++){

                if (!isEmpty(&(cpu[i] -> queue))){

                    // print the running when there is change
                    if (cpu[i] -> currentProccess != peek(&(cpu[i] -> queue)) -> id){
                        if  (peek(&(cpu[i] -> queue)) -> p == 'p')
                            printf("%d,RUNNING,pid=%.1f,remaining_time=%d,cpu=%d\n",time, peek(&(cpu[i] -> queue)) -> id, peek(&(cpu[i] -> queue)) -> remainingTime, cpu[i]->id);
                        else
                            printf("%d,RUNNING,pid=%d,remaining_time=%d,cpu=%d\n",time, (int)peek(&(cpu[i] -> queue)) -> id, peek(&(cpu[i] -> queue)) -> remainingTime, cpu[i]->id);

                        cpu[i] -> currentProccess = peek(&(cpu[i] -> queue)) -> id;
                    }
                    
                    // minus 1 time of the smallest remaing time
                    peek(&(cpu[i] -> queue)) -> remainingTime --;


                    // check if the process is completed
                    if(peek(&(cpu[i] -> queue)) -> remainingTime == 0)
                    {
                        end=time+1;
                        peek(&(cpu[i] -> queue)) ->completion = end;
                        
                        // calculate turnaroud time
                        peek(&(cpu[i] -> queue)) ->turnaround = end - peek(&(cpu[i] -> queue)) -> arrivingTime;
                        
                        // calculate max overhead
                        if (max_overhead <  (float)(peek(&cpu[i] -> queue) ->turnaround)/peek(&(cpu[i] -> queue)) -> executionTime)
                            max_overhead = (float)(peek(&cpu[i] -> queue) ->turnaround)/peek(&(cpu[i] -> queue)) -> executionTime;
                        
                        //handle the case when proccess is compleeted when all of its sub proccesses completed
                        if  (peek(&(cpu[i] -> queue)) -> p == 'p'){
                            bool allFinish = true;
                            for (int j =0; j< num_cpu;j++){
                                if (j != i){
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

                        // pop the process out of queue when finished
                        pop(&(cpu[i] -> queue));
                    }
                }               
            }
		}
    }
    
    // execute process when all processes already in queue
    for(; count!=num_processess; time++){
            for (int i =0; i< num_cpu;i++){

                if (!isEmpty(&(cpu[i] -> queue))){
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
                        
                        if (max_overhead <  (float)(peek(&cpu[i] -> queue) ->turnaround)/peek(&(cpu[i] -> queue)) -> executionTime)
                            max_overhead = (float)(peek(&cpu[i] -> queue) ->turnaround)/peek(&(cpu[i] -> queue)) -> executionTime;


                        if  (peek(&(cpu[i] -> queue)) -> p == 'p'){
                            bool allFinish = true;
                            for (int j =0; j< num_cpu;j++){
                                if (j != i){
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

    //print the statsitc
    printf("Turnaround time %d\n",(int)ceil(total_turnaround/num_processess));
    printf("Time overhead %.2f %.2f\n",max_overhead,avg_overhead/num_processess);
    printf("Makespan %d",end);
    fclose(fp);

    
    return 0;


}
