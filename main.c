#include <stdio.h>
#include <stdlib.h>

/*
 * Process: Each Process has a name and has an execution time.
 *          We keep track of how long the Process still needs
 *          to run on the Processor. "remaining_time" is the
 *          remaining execution time of the Process.
 * Item: An item that is being enqueued to or dequeued from the queue
 *       It is a pointer to the Process
 */
typedef struct {
	char *name;
	int startTime;
	int remainingTime;
} Process, *Item;

/*
 *
 */
typedef struct Node {
	Item data;
	struct Node* next;
} Node;

/*
 *
 */
typedef struct {
    Node *first, *last;
} Queue;

/*
 *
 */
void init(Queue *q) {
    q->first = (Node *) malloc( sizeof( Node ) );
    q->last = (Node *) malloc( sizeof( Node ) );
    q->first = NULL;
    q->last = NULL;
}

/*
 *
 */
int isEmpty(Queue *q) {
	if (!(q->first) )
        return 1;
    return 0;
}

/*
 *
 */
Item dequeue(Queue *q) {
	if( isEmpty(q) )
    {
        printf("Empty Queue");
        return 0;
    }
    Item temp;
    temp = q->first->data;
    q->first = q->first->next;
    if(! (q->first) )
       q->last = NULL;
    return temp;
}

/*
 *
 */
void enqueue(Queue *q, Item val) {
	Node* n = (Node *) malloc( sizeof(Node) );
    n->data = val;
    n->next = NULL;
    if(q->first)
    {
        q->last->next = (Node *)malloc(sizeof(Node));
        q->last->next = n;
        q->last = n;
    }
    else
    {
        q->first = n;
        q->last = n;
    }
}

/*
 *
 */
void destroy(Queue *q) {
	Node* temp;
    while( q->first )
    {
        temp = q->first->next;
        free( q->first->data );
        q->first = temp;
    }
    q->first = NULL;
    q->last = NULL;
}

/*
 * RoundRobin Scheduling
 */
void RoundRobin(char* filename) {

    Queue q;
    init( &q );
    Item temp;
    int time = 0;
    int unitTime = 1;
    int i, j;

    FILE* fp = fopen(filename,"r");
    i = 0;

    while(!feof(fp)) {
        temp = (Item* ) malloc( sizeof( Item ) );
        temp->name = (char *) malloc( 2 );
        fscanf(fp, "%[^ ] %d %d\n", temp->name, &(temp->startTime), &(temp->remainingTime));
        enqueue(&q, temp);
        i++;
    }
    fclose(fp);

    j=0;
    while(!isEmpty(&q))
    {
       temp = dequeue(&q);
       while((temp->startTime > time) && j<i)
       {
           enqueue(&q, temp);
           temp = dequeue(&q);
           j++;
       }
       if(i == j)
       {
           printf("\nidle (%d-->%d) ", time, (time + unitTime));
           j = 0;
           enqueue(&q, temp);
       }
       else
       {
           temp->remainingTime--;
           printf("\n%s (%d-->%d) ", temp->name, time, (time + unitTime));

           if(temp->remainingTime < 1)
               printf("%s halts", temp->name);
           else
               enqueue(&q, temp);
       }
       time += unitTime;
    }
    printf("\nEOF\n\n");
    destroy(&q);
}

/*
 *
 */
int main(int argc, char**argv) {

	if(argc!=2) {
		printf("Usage: %s filename\n", argv[0]);
		exit(1);
	}

	RoundRobin(argv[1]);

	return 0;
}
