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

