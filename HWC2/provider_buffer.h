/*
 * provider_buffer.h
 *
 *  Created on: 06 dic 2017
 *      Author: valerio
 */

#include "buffer.h"
#include "poison_pill.h"

#ifndef PROVIDER_BUFFER_H_
#define PROVIDER_BUFFER_H_

//il buffer del provider
buffer_t* buffer_provider;

//inizializzazione del buffer del provider
void buffer_provider_init(int maxsize);

//deallocazione del buffer del provider
void buffer_provider_destroy();

//inserimento di un messaggio nel buffer_provider
msg_t* put_buffer_provider(msg_t* messaggio);

//prelevamento di un messaggio dal buffer_provider
msg_t* get_buffer_provider();


#endif /* PROVIDER_BUFFER_H_ */
