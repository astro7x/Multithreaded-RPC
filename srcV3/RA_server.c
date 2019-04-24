/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "RA.h"

#define TICKET_LOCK_INITIALIZER { PTHREAD_COND_INITIALIZER, PTHREAD_MUTEX_INITIALIZER }


/*Semi-Anderson Queue Based Ticket Algorithm
 *without Atomic Operation Fetch and Increment
 */

typedef struct AQLock
{
	pthread_mutex_t lock;		//mutex lock
	pthread_cond_t cond;		//Cond wait
	unsigned int QNow;		//now serving
	unsigned int QNext; 		//next ticket to serve
} AQLock_ticket;

/*Global Shared resources handeled by the threads*/
pthread_mutex_t 	lock;				/*mutexLock for updating the private resources number*/
pthread_mutex_t		rsrc_lock;			/*lock to be used with con_wait after the resources have been allocated*/
pthread_cond_t		cond;
AQLock_ticket Qlock =	TICKET_LOCK_INITIALIZER;	/*Lock for Ticket based Queue*/
unsigned int 		rsrc_pvt=10;			/*Number of Private Resources*/
unsigned int 		init=0;				/*Flag to initialize the @lock once*/

void AQ_Lock(AQLock_ticket *ticket);
void AQ_Unlock(AQLock_ticket *ticket);



/*----------------------------------------------*/
/* Function: @resourceallocator_2 is Called
 *  by the thread @serv_request 	        */
/*----------------------------------------------*/

bool_t
allocate_2_svc(rsrc_req *argp, reply *result, struct svc_req *rqstp)
{
	bool_t retval;
	unsigned int work;
	/*Initialize all the locks*/
	if (!init) {
		pthread_mutex_init(&lock,NULL);
		pthread_mutex_init(&rsrc_lock,NULL);
		pthread_cond_init(&cond,NULL);
		init=1;
	}

	/*For each client request, a thread is to be started
	 * @pthread_create is called in the @RA_svc.c
	 */	
	
	/*Print the running thread and the num of requested resources*/
	printf("[START:\t] Thread id = %d, arg = %d\n",pthread_self(),argp->req);

	/*Anderson Queue Lock*/
	AQ_Lock(&Qlock);

	/* 
	 * >critical section
	 * [Allocation]: Update the resources number */
	pthread_mutex_lock(&lock);
	rsrc_pvt-=argp->req;
	pthread_mutex_unlock(&lock);
	printf("[DEBUG:\t] rsrc_pvt = %d \n",rsrc_pvt);


	pthread_mutex_lock(&rsrc_lock);
	/*Block: num_requestedResource < num_PrivateResources*/
	while (argp->req > rsrc_pvt) {
		pthread_cond_wait(&cond,&rsrc_lock);
	}
	/*Do some dummy work*/
	work=rand()%2;
	sleep(work); 
	result->rep = 2*(argp->req);
	/*WakeUp all other threads in the Queue*/
	pthread_cond_broadcast(&cond);
	pthread_mutex_unlock(&rsrc_lock);
s

	/* 
	 * >critical section
	 * [DeAllocation]: Update the resources number */
	pthread_mutex_lock(&lock);
	rsrc_pvt+=argp->req;
	pthread_mutex_unlock(&lock);
	printf("[DEBUG:\t] rsrc_pvt = %d \n",rsrc_pvt);
  	printf("[END  :\t] Thread id = %d is done %d \n",pthread_self(),result->rep);

	/*Anderson Queue unLock*/
	AQ_Unlock(&Qlock);

	return retval;
}

int
resourceallocator_2_freeresult (SVCXPRT *transp, xdrproc_t xdr_result, caddr_t result)
{
	xdr_free (xdr_result, result);

	return 1;
}



void AQ_Lock(AQLock_ticket *ticket)
{
    unsigned long Myticket;	
    pthread_mutex_lock(&ticket->lock);
    /*FetchAndIncrement next Ticket (Not Atomic!)*/
    Myticket = ticket->QNext++;
    while (Myticket != ticket->QNow)
    {   
	printf("\n waiting on queue %d \n",pthread_self());
        pthread_cond_wait(&ticket->cond, &ticket->lock);
    }
    pthread_mutex_unlock(&ticket->lock);
}


void AQ_Unlock(AQLock_ticket *ticket)
{
    pthread_mutex_lock(&ticket->lock);
    ticket->QNow++;
    pthread_cond_broadcast(&ticket->cond);
    pthread_mutex_unlock(&ticket->lock);
}

