/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "RA.h"

bool_t
xdr_rsrc_req (XDR *xdrs, rsrc_req *objp)
{
	register int32_t *buf;

	 if (!xdr_int (xdrs, &objp->req))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_reply (XDR *xdrs, reply *objp)
{
	register int32_t *buf;

	 if (!xdr_int (xdrs, &objp->rep))
		 return FALSE;
	return TRUE;
}