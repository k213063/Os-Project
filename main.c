// Include necessary header files
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>

// Define global semaphores
sem_t customer_sem;     // Declare semaphore for customers
sem_t barber_sem;       // Declare semaphore for barbers
sem_t mutex;            // Declare mutex for synchronizing access to the shared queue

typedef struct {        // Define a struct for storing customer information
    int id;             // Customer ID
    int num_chairs;     // Number of chairs in the waiting room
} customer_info;

typedef struct {        // Define a struct for storing barber information
    int id;             // Barber ID
} barber_info;

int queue[100];         // Declare an array for the queue of customers
int front = -1;         // Initialize front of queue as -1
int rear = -1;          // Initialize rear of queue as -1

void enqueue(int id);   // Declare function for adding a customer to the queue
int dequeue();          // Declare function for removing a customer from the queue
bool is_queue_empty();  // Declare function for checking if the queue is empty
bool is_queue_full(int num_chairs);  // Declare function for checking if the queue is full

void *customer(void *arg);  // Declare function for simulating a customer
void *barber(void *arg);    // Declare function for simulating a barber

int main() {                    // Main function

    int num_chairs, num_customers, num_barbers;  // Declare variables for user input

    printf("Enter the number of chairs: ");       // Prompt user for number of chairs
    scanf("%d", &num_chairs);                     // Get user input

    printf("Enter the number of customers: ");    // Prompt user for number of customers
    scanf("%d", &num_customers);                  // Get user input

    printf("Enter the number of barbers: ");      // Prompt user for number of barbers
    scanf("%d", &num_barbers);                    // Get user input

    pthread_t customer_threads[num_customers];    // Declare array of customer threads
    pthread_t barber_threads[num_barbers];        // Declare array of barber threads
    customer_info customer_data[num_customers];   // Declare array of customer information structs
    barber_info barber_data[num_barbers];         // Declare array of barber information structs

    sem_init(&customer_sem, 0, 0);                // Initialize customer semaphore to 0
    sem_init(&barber_sem, 0, 0);                  // Initialize barber semaphore to 0
    sem_init(&mutex, 0, 1);                       // Initialize mutex to 1


   for (int i = 0; i < num_barbers; i++) { // Create threads for the barbers
    barber_data[i].id = i;
    pthread_create(&barber_threads[i], NULL, barber, (void *)&barber_data[i]); // Create a new barber thread and pass the barber data as an argument
}

for (int i = 0; i < num_customers; i++) { // Create threads for the customers
    customer_data[i].id = i;
    customer_data[i].num_chairs = num_chairs;
    pthread_create(&customer_threads[i], NULL, customer, (void *)&customer_data[i]); // Create a new customer thread and pass the customer data as an argument
    sleep(1); // Sleep for 1 second to simulate customers arriving at different times
}

for (int i = 0; i < num_customers; i++) { // Wait for all the customer threads to finish
    pthread_join(customer_threads[i], NULL);
}

for (int i = 0; i < num_barbers; i++) { // Cancel all the barber threads
    pthread_cancel(barber_threads[i]);
}

sem_destroy(&customer_sem); // Destroy the customer semaphore
sem_destroy(&barber_sem); // Destroy the barber semaphore
sem_destroy(&mutex); // Destroy the mutex semaphore
	
	
    return 0;
}

void *customer(void *arg) {
    customer_info *data = (customer_info *)arg;
    int customer_id = data->id;
    int num_chairs = data->num_chairs;

    sem_wait(&mutex);
    if (!is_queue_full(num_chairs)) {
        enqueue(customer_id);
        printf("Customer %d is waiting. (Total waiting customers: %d)\n", customer_id, rear - front);
        sem_post(&customer_sem);
        sem_post(&mutex);

        sem_wait(&barber_sem);
        sem_wait(&mutex);
        dequeue();
        printf("Customer %d is getting a haircut. (Total waiting customers: %d)\n", customer_id, rear - front);
        sem_post(&mutex);
    } else {
        printf("No chairs available. Customer %d leaves.\n", customer_id);
        sem_post(&mutex);
    }

    return NULL;
}

void *barber(void *arg) {
	// receiving a pointer to barber info as an argument which contains the data of the barber func
    barber_info *data = (barber_info *)arg;
	// extracts the babrber id and store it into the local variable 
    int barber_id = data->id;
    while (1) {
	    //customer  semaphore is initialized with 0
        sem_wait(&customer_sem); //the barber thread is blocked until a customer is avail
	    //when the custiomer is avaialbe semaphore is decremented and the barber thread will continue its execution
        sem_post(&barber_sem); // this will signal that the barber is now avaialble to serve the customer 
        printf("Barber %d is cutting hair.\n", barber_id);
        sleep(3); //hair cutting process 
        printf("Barber %d has finished cutting hair.\n", barber_id);
	    // and the function will go back to the loop statrrting and wait for the next custiomer 
	    
    }

    return NULL;
}

void enqueue(int id) {
    if (rear == -1) {
        front = rear = 0;
        queue[rear] = id;
    } else {
        rear++;
        queue[rear] = id;
    }
}

int dequeue() {
    if (is_queue_empty()) {
        return -1;
    }

    int data = queue[front];
    front++;

    if (front > rear) {
        front = rear = -1;
    }

    return data;
}

bool is_queue_empty() {
    return front == -1;
}

bool is_queue_full(int num_chairs) {
    return rear == num_chairs - 1;
}
