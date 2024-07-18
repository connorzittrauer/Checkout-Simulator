#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Delcare Global Variables
int simulationTime;
int numExpressLines;
int numRegularLines;
int avgCustomerTimeDifferential;
int percentageExpressCustomers;
int avgExpressCustomerTime;
int minExpressCustomerTime;
int avgRegularCustomerTime;
int minRegularCustomerTime;



// #region
/*
 * QUEUE IMPLEMENTATION
 *
 */

// Define a node in the queue
typedef struct Node
{   
    int customerCount; // was 'data'
    struct Node *next;
} Node;

// Define the queue
typedef struct Queue
{
    Node *front;
    Node *rear;
    int size;
} Queue;

// Function to create a new queue
Queue *createQueue()
{
    Queue *q = (Queue *)malloc(sizeof(Queue));
    q->front = NULL;
    q->rear = NULL;
    q->size = 0;

    return q;
}

// Function to add an element to the queue
void enqueue(Queue *q, int data)
{
    Node *temp = (Node *)malloc(sizeof(Node));
    temp->customerCount = data;
    temp->next = NULL;

    //if the rear of the queue is empty, update the pointer to the front and rear to the temp variable
    if(q->rear == NULL){
        q->front = temp;
        q->rear = temp;

    } 
    // otherwise, set the pointer of the rear's next node to be equal to temp  
    else {
        q->rear->next = temp;
        q->rear = temp;
    }

    q->size++;
}

// Function to remove an element from the queue
int dequeue(Queue *q)
{
    if (q->front == NULL)
    {
        printf("Queue is empty.\n");
        return -1;
    }

    Node *temp = q->front;
    int data = temp->customerCount;
    q->front = q->front->next;

    if (q->front == NULL)
    {
        q->rear = NULL;
    }

    free(temp);
    q->size--;

    return data;
}

// Function to check if the queue is empty
int isEmpty(Queue *q)
{
    return q->front == NULL;
}

int getSize(Queue *q) {
    return q->size;
}

// Function to print the queue
void printQueue(Queue *q)
{
    Node *temp = q->front;
    while (temp != NULL)
    {
        printf("%d ", temp->customerCount);
        temp = temp->next;
    }
    printf("\n");
}

/*
 * END QUEUE IMPLEMENTATION
 *
 */
// #endregion

void userInterface()
{
    printf("Enter the length of simulation (in seconds): ");
    // ampersand so we can directly access the memory address of the variable
    scanf("%d", &simulationTime);

    // printf("Enter the number of open regular lines: ");
    // scanf("%d", &numRegularLines);

    // printf("Enter the number of open express lines: ");
    // scanf("%d", &numExpressLines);

    // printf("Enter the average time between customers entering checkout line (in seconds): ");
    // scanf("%d", &avgCustomerTimeDifferential);

    printf("Enter the percentage of qualified Express Customers: ");
    scanf("%d", &percentageExpressCustomers);

    // // EXPRESS CUSTOMER TIME
    // printf("Enter the average time needed for an express customer to checkout (in seconds): ");
    // scanf("%d", &avgExpressCustomerTime);
    // printf("Minimum time needed for an express customer to checkout (in seconds): ");
    // scanf("%d", &minExpressCustomerTime);

    // // REGULAR CUSTOMER TIME
    // printf("Enter the average time needed for a regular customer to checkout (in seconds): ");
    // scanf("%d", &avgRegularCustomerTime);
    // printf("Minimum time needed for a regular customer to checkout (in seconds): ");
    // scanf("%d", &minRegularCustomerTime);



    printf("\n");

    // Print out to make sure we're assigning variables properly for debugging
    // printf("Simulation length: %d\n", simulationTime);
    // printf("Number of regular lines: %d\n", numRegularLines);
    // printf("Number of express lines: %d\n", numExpressLines);
    // printf("Average time between customers entering checkout line: %d\n", avgCustomerTimeDifferential);
    // printf("Percentage of customers that qualify as express: %d\n", percentageExpressCustomers);
    // printf("Average time needed for an express customer to checkout %d\n", avgExpressCustomerTime);
    // printf("Minimum time needed for an express customer to checkout %d\n", minExpressCustomerTime);
    // printf("Average time needed for a regular customer to checkout %d\n", avgRegularCustomerTime);
    // printf("Minimum time needed for a regular customer to checkout %d\n", minRegularCustomerTime);
}

int pgrand(int x, int y)
{
    int i;
    long long total = 0; //  note:a long long is a 64 bit integer - wanted to be sure to have enough space.

    for (i = 0; i < 6; i++)
        // generate a pseudorandom number between 0 and 6*RAND_MAX
        total += rand();
    // Next two lintes scale the total to the range 0 to y-x
    total *= (y - x);
    total /= (((long long)RAND_MAX) * 6);
    return x + total; // finally add x and return to get a value between x and y
}

void simulation()
{   
    Queue* expressLane = createQueue();
    Queue* regularLane = createQueue();
    
    int expressCustomerCount = 0;
    int regularCustomerCount = 0;
    double avgExpressWaitTime = 0;
    double avgRegularWaitTime = 0;

    // start with time 0
    int START_TIME = 0;
    //random number to check if customer is express or now
    int expressCheckRandom;
    //psuedo-gaussian random number for wait times
    int guassianRandom;


    while (START_TIME < simulationTime)
    {   
        expressCheckRandom = rand() % 100 + 1;
      
        // Check whether the customer is express or not, if so enqueue them into the express lane 
        if (expressCheckRandom <= percentageExpressCustomers) {
            
            // the average wait time is the sum of guasianRandom divided by the simulation time.       
            guassianRandom = pgrand(0, 100);
            avgExpressWaitTime += guassianRandom;
            enqueue(expressLane, expressCustomerCount++);    

        }
        // otherwise, enqueue them into the regular lane
        else{
            guassianRandom = pgrand(0, 100);
            avgRegularWaitTime += 100;
            enqueue(regularLane, regularCustomerCount++);

        }

        START_TIME += 1;
    }

    printf("The simulation ran: %d times \n", START_TIME);
    printf("Number of customers processed through express lane: %d\n", getSize(expressLane));
    printf("The average express customer wait time was: %.2f seconds\n", avgExpressWaitTime/simulationTime);
    printf("Number of customers processed through regular lane: %d\n", getSize(regularLane));
    printf("The average regular customer wait time was: %.2f seconds\n", avgRegularWaitTime/simulationTime);

}


int main()
{
    // Seed the random number generator to get true random values
    srand(time(NULL));

    userInterface();

    simulation();

    return 0;
}
