/*
 * accepter_buffer.c
 *
 *  Created on: 06 dic 2017
 *      Author: valerio
 */

#include "accepter_buffer.h"


//inizializzazione del buffer del accepter
void buffer_accepter_init(int maxsize){

	buffer_accepter = buffer_init(maxsize);
}

//deallocazione del buffer del accepter
void buffer_accepter_destroy(){

	buffer_destroy(buffer_accepter);
}

//inserimento di un messaggio nel buffer_accepter
msg_t* put_buffer_accepter(msg_t* messaggio){

	return put_bloccante(buffer_accepter, messaggio);
}

//prelevamento di un messaggio dal buffer_accepter
msg_t* get_buffer_accepter(){

	return get_bloccante(buffer_accepter);
}

