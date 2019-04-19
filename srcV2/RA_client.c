/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "RA.h"


void
resourceallocator_2(char *host)
{
	CLIENT *clnt;
	enum clnt_stat retval_1;
	reply result_1;
	rsrc_req  allocate_2_arg;


#ifndef	DEBUG
	clnt = clnt_create ("127.0.0.1", ResourceAllocator, RA, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror ("127.0.0.1");
		exit (1);
	}
#endif	/* DEBUG */
	while(1)
	{
		/*Request A random number of Private Resources 
		 * from the server
		 * in Range [0: MaxPrivateReources]
		 * @MaxPrivateReources:=10
		 */
		allocate_2_arg.req = rand()%10;;

		retval_1 = allocate_2(&allocate_2_arg, &result_1, clnt);
		if (retval_1 != RPC_SUCCESS) {
			clnt_perror (clnt, "call failed");
		}
		/*Check the result*/
		if (result_1.rep==allocate_2_arg.req*2){
			printf("I'm Blocked\n");
		}
		else printf("[Result:\t] %ld\n",result_1.rep);
		sleep(3); /*Delay between each Request*/
	}

#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}


int
main (int argc, char *argv[])
{
	resourceallocator_2 ("127.0.0.1");
exit (0);
}
