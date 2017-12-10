/*
 * requestor.c
 *
 *  Created on: 07 dic 2017
 *      Author: valerio
 */

#include "requestor.h"

//invio di una richiesta nella coda delle richieste
msg_t* submitRequest(msg_t* request){

	return put_buffer_accepter(request);
}
