# Os-Project
**Barber Shop Simulation**
This program simulates a barber shop with multiple barbers and chairs, and customers who come in and wait for a barber to become available.

**Design:-**
The program uses pthreads and semaphores to simulate a barber shop. When a customer arrives, they check to see if there are any available chairs in the waiting area. If there are, they take a seat and wait for a barber to become available. If there are no chairs available, they leave.
When a barber finishes cutting a customer's hair, they check to see if there are any customers waiting in the waiting area. If there are, they start cutting their hair. If there are no customers waiting, they go to sleep until a customer arrives.

**Partition**
This code is divided into three parts:

1.User Input and Initialization
2.Customer Thread Function
3.Barber Thread Function

**Credits**
This program was written by Aleem, Rahima, and Abdul Samad as part of a project for a course on operating systems.
