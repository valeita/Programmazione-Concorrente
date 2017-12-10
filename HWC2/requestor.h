/*
 * requestor.h
 *
 *  Created on: 07 dic 2017
 *      Author: valerio
 */

#ifndef REQUESTOR_H_
#define REQUESTOR_H_

#include <stdlib.h>
#include "accepter_buffer.h"


//invio di una richiesta nella coda delle richieste
msg_t* submitRequest(msg_t* request);

#endif /* REQUESTOR_H_ */
