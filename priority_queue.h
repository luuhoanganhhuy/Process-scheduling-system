

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


Proccess* newProccess(float id,int remainingTime, int arrivingTime, char p,int executionTime);

Proccess* peek(Proccess** head);

void pop(Proccess** head);

void push(Proccess** head, float id,int remainingTime, int arrivingTime, char p,int executionTime);

int isEmpty(Proccess** head);
