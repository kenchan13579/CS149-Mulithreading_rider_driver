#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#define TRUE 1
#define FALSE 0
#define COMPANY_NAME "Chan's Cruise"
void *createRiders(void *);
void *createDrivers(void *);
int enqueue(int);
int dequeue(int);
int WaitQueueSize();
int isExist(int);
pthread_mutex_t mutex;
int numberOfRiders , numberOfDrivers, silmulation_time , L;
int *WaitQueue ,  head = -1 , tail = -1;
int total_rider_served = 0 ;
sem_t emptySlots;
sem_t filledSlots;
void *createRiders(void *id)
{
	int rider_id, x;
	rider_id = (int) id;
    
   // printf("%d\n",rider_id );
	for (;;)
	{
		sleep(rand() % 20);
      enqueue(rider_id);     
	}	
}

void  *createDrivers(void *id)
{
	int driver_id;
	driver_id = (int) id;
	
	for (;;) {
		dequeue(driver_id);
      }
	}
int dequeue(int id)
{
// Wait on the "filledSlots" semaphore.
    sem_wait(&filledSlots);

    // Acquire the mutex lock to protect the buffer.
    pthread_mutex_lock(&mutex);
     int completedrider ;
      completedrider= WaitQueue[head];
      if ( head==tail && WaitQueueSize()!=0 ) // 1 data
	{

     printf("Driver %d : arrived , Length of WaitQueue: %d\n", id, WaitQueueSize() );	
     
    // Critical region: Remove an item from the buffer.
   
     WaitQueue[head] = 0;
    	head=tail= -1;

    printf("Driver %d : Riding , Length of WaitQueue: %d\n", id, WaitQueueSize() );
    sleep(rand()%5);

    printf("%60s  %d: completed riding , Length of WaitQueue: %d\n","Rider", completedrider, WaitQueueSize() );
	total_rider_served++;
    //printf("head : %d , tail :%d\n", head,tail);
		
	}
	else if (WaitQueueSize()==0)
	{

	}
	else { // more than one data
		if ( head==L-1)  //  head=L-1  to head =0
		{
			printf("Driver %d : arrived , Length of WaitQueue: %d\n", id, WaitQueueSize() );	
     
    // Critical region: Remove an item from the buffer.
  
    WaitQueue[head]=0;
			head=0;
	printf("Driver %d : Riding , Length of WaitQueue: %d\n", id, WaitQueueSize() );		
    sleep(rand()%5);
    printf("%60s  %d: completed riding , Length of WaitQueue: %d\n","Rider", completedrider, WaitQueueSize() );
    total_rider_served++;
   // printf("head : %d , tail :%d\n", head,tail);
		}
		else {
			printf("Driver %d : arrived , Length of WaitQueue: %d\n", id, WaitQueueSize() );	
     
    // Critical region: Remove an item from the buffer.
    
    WaitQueue[head]=0;
			head++;
	printf("Driver %d : Riding , Length of WaitQueue: %d\n", id, WaitQueueSize() );	
    sleep(rand()%5);
    printf("%60s  %d: completed riding , Length of WaitQueue: %d\n","Rider", completedrider, WaitQueueSize() );
    total_rider_served++;
	//printf("head : %d , tail :%d\n", head,tail);	
		}
	}
   
    
    // Release the mutex lock. 
    pthread_mutex_unlock(&mutex);
    
    // Signal the "emptySlots" semaphore.
    sem_post(&emptySlots);  // signal

    return 0;
	
}
int enqueue(int id)
{
	 // Wait on the "emptySlots" semaphore.
    sem_wait(&emptySlots);

    // Acquire the mutex lock to protect the buffer.
    pthread_mutex_lock(&mutex);
   	if (isExist(id)==FALSE)
   	{
		    // Critical region: Insert an item into the buffer.
		    if ( WaitQueueSize()==0)
			{
				head=0;
				tail=0;
				WaitQueue[tail]=id;
				
		     printf("Rider %d : arrived , length of WaitQueue :%d \n", id, WaitQueueSize());
		    // printf("head : %d , tail :%d\n", head,tail);
			}
			
			else if (tail==L-1 && head!=0)
			{
				tail=0;
				WaitQueue[tail] = id;
				
		     printf("Rider %d : arrived , length of WaitQueue :%d \n", id, WaitQueueSize());
		   //  printf("head : %d , tail :%d\n", head,tail);
			}
			else if ( WaitQueueSize() < L)
			{
				tail++;
				WaitQueue[tail] = id;
				
		     printf("Rider %d : arrived , length of WaitQueue :%d \n", id, WaitQueueSize());
		    // printf("head : %d , tail :%d\n", head,tail);
			}
   }
    // Release the mutex lock.
    pthread_mutex_unlock(&mutex);
    
    // Signal the "filledSlots" semaphore.
    sem_post(&filledSlots);  // signal

    return 0;
	
}
int isExist(int id)
{
	int u;
	if ( WaitQueueSize()>0)
	{
		if ( tail >= head) 
		{
			for ( u = head; u <= tail ; u++)
			{
				if ( id == WaitQueue[u])
				{
					return TRUE;
				}
			}
		}
		else 
		{
			for ( u = head; u>=0  ; u--)
			{
				if ( id == WaitQueue[u])
				{
					return TRUE;
				}
			}
			for ( u = tail; u<L ; u++)
			{
				if ( id == WaitQueue[u])
				{
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}
int WaitQueueSize()
{
	int size;
	if ( tail==head && head!=-1)
	{
		return 1;
	}
	else if (head==-1)
	{
		return 0;
	}
	else if ( tail > head)
	{
		return (tail-head+1);
	}
	else {
		return (L-head+tail+1);
	}

}
int main(int argc, char *argv[]) {

	
	int   i ,j , k;
	int initial_simulatetime;
	printf("Enter number of riders(N), number of drivers (M), and the time(T) the system will simulate \n in the following format: N M T (BTW, L is set at 100 and N should be larger than 100.)\n");
	srand(silmulation_time);
	scanf("%d %d %d", &numberOfRiders, &numberOfDrivers , &silmulation_time);
	pthread_t riders_N[numberOfRiders];
	pthread_t drivers_M[numberOfDrivers];
	L = 0.8 *numberOfRiders;
	WaitQueue = (int *)malloc(L * sizeof(int));
	printf ( " Creating %d riders and  %d drivers with Waitqueue of %d in a simulated system of %d seconds ...\n\n", numberOfRiders, numberOfDrivers,L, silmulation_time);
	/// Initialize the the locks.
    printf("         Mutex init: %d\n", pthread_mutex_init(&mutex, NULL));
    printf("Empty semaphor init: %d\n", sem_init(&emptySlots, 0, L));
    printf(" Full semaphor init: %d\n", sem_init(&filledSlots, 0, 0));
    printf("\n");

	printf("Rider sleeptime in a range of 0 -20, Driver sleeptime in a range of 0-5");

	printf ("\n\n\nWelcome to %20s\n\n", COMPANY_NAME);

     srand(time(0));
     for ( i = 1 ; i <= numberOfRiders; i++)
	{
		pthread_create(&riders_N[i-1], NULL, createRiders, (void *) i);
	}
	for ( j = 1; j <= numberOfDrivers ;  j++)
	{
		int driverID ;
		driverID = j+ numberOfRiders;
		pthread_create(&drivers_M[j-1], NULL , createDrivers, (void *) driverID);
	}

	
	
    sleep(silmulation_time);
    printf("\nSystem time reaches 0 , now ends...\n\n");
    printf("**********			Statistic		************\n");
    printf("Total number of Riders served : %d \n", total_rider_served);
    printf("Total number of RIders remains in the wait queue: %d\n", WaitQueueSize());
    printf("The average number of Riders each Driver served: %d \n",(total_rider_served/numberOfDrivers));
    return 0;

	
}

