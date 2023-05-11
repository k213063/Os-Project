#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

sem_t customer_sem;
sem_t barber_sem;
sem_t mutex;

int waiting_customers = 0;

void *customer(void *arg);
void *barber(void *arg);

typedef struct {
    int id;
    int num_chairs;
} customer_info;

int main() {
    int num_chairs, num_customers, num_barbers;
    
    printf("Enter the number of chairs: ");
    scanf("%d", &num_chairs);

    printf("Enter the number of customers: ");
    scanf("%d", &num_customers);

    printf("Enter the number of barbers: ");
    scanf("%d", &num_barbers);

    pthread_t customer_threads[num_customers];
    pthread_t barber_threads[num_barbers];
    customer_info customer_data[num_customers];

    sem_init(&customer_sem, 0, 0);
    sem_init(&barber_sem, 0, 0);
    sem_init(&mutex, 0, 1);

    for (int i = 0; i < num_barbers; i++) {
        pthread_create(&barber_threads[i], NULL, barber, NULL);
    }

    for (int i = 0; i < num_customers; i++) {
        customer_data[i].id = i;
        customer_data[i].num_chairs = num_chairs;
        pthread_create(&customer_threads[i], NULL, customer, (void *)&customer_data[i]);
        sleep(1);
    }

    for (int i = 0; i < num_customers; i++) {
        pthread_join(customer_threads[i], NULL);
    }

    for (int i = 0; i < num_barbers; i++) {
        pthread_cancel(barber_threads[i]);
    }

    sem_destroy(&customer_sem);
    sem_destroy(&barber_sem);
    sem_destroy(&mutex);

    return 0;
}
void *customer(void *arg) {
    customer_info *data = (customer_info *)arg;
    int customer_id = data->id;
    int num_chairs = data->num_chairs;

    sem_wait(&mutex);
    if (waiting_customers < num_chairs) {
        waiting_customers++;
        printf("Customer %d is waiting. (Total waiting customers: %d)\n", customer_id, waiting_customers);
        sem_post(&customer_sem);
        sem_post(&mutex);

        sem_wait(&barber_sem);
        sem_wait(&mutex);
        waiting_customers--;
        printf("Customer %d is getting a haircut. (Total waiting customers: %d)\n", customer_id, waiting_customers);
        sem_post(&mutex);
    } else {
        printf("No chairs available. Customer %d leaves.\n", customer_id);
        sem_post(&mutex);
    }

    return NULL;
}
void *barber(void *arg){
    while(1){
     sem_wait(&customer_sem);
     sem_post(&barber_sem);
     printf("Barber is cutting hair");
     sleep(3);
     printf("Barber has finished cutting hair");
    }
    return null;
}
