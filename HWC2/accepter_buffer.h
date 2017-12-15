/*
 * accepter_buffer.h
 *
 *  Created on: 06 dic 2017
 *      Author: valerio
 */


#include "buffer.h"
#include "poison_pill.h"

#ifndef accepter_BUFFER_H_
#define accepter_BUFFER_H_

//il buffer del accepter
buffer_t* buffer_accepter;

//inizializzazione del buffer del accepter
void buffer_accepter_init(int maxsize);

//deallocazione del buffer del accepter
void buffer_accepter_destroy();

//inserimento di un messaggio nel buffer_accepter
msg_t* put_buffer_accepter(msg_t* messaggio);

//prelevamento di un messaggio dal buffer_accepter
msg_t* get_buffer_accepter();


#endif /* accepter_BUFFER_H_ */
