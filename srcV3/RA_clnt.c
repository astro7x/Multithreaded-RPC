/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include <memory.h> /* for memset */
#include "RA.h"

/* Default timeout can be changed using clnt_control() */
static struct timeval TIMEOUT = { 25, 0 };

enum clnt_stat 
allocate_2(rsrc_req *argp, reply *clnt_res, CLIENT *clnt)
{
	return (clnt_call(clnt, allocate,
		(xdrproc_t) xdr_rsrc_req, (caddr_t) argp,
		(xdrproc_t) xdr_reply, (caddr_t) clnt_res,
		TIMEOUT));
}

enum clnt_stat 
release_2(rsrc_req *argp, reply *clnt_res, CLIENT *clnt)
{
	return (clnt_call(clnt, release,
		(xdrproc_t) xdr_rsrc_req, (caddr_t) argp,
		(xdrproc_t) xdr_reply, (caddr_t) clnt_res,
		TIMEOUT));
}
