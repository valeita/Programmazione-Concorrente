/*
 * provider_buffer.c
 *
 *  Created on: 06 dic 2017
 *      Author: valerio
 */

#include "provider_buffer.h"


//inizializzazione del buffer del provider
void buffer_provider_init(int maxsize){

	buffer_provider = buffer_init(maxsize);
}

//deallocazione del buffer del provider
void buffer_provider_destroy(){

	buffer_destroy(buffer_provider);
}

//inserimento di un messaggio nel buffer_provider
msg_t* put_buffer_provider(msg_t* messaggio){

	return put_bloccante(buffer_provider, messaggio);
}

//prelevamento di un messaggio dal buffer_provider
msg_t* get_buffer_provider(){

	return get_bloccante(buffer_provider);
}

