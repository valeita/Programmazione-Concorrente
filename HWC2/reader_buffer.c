/*
 * reader_buffer.c
 *
 *  Created on: 07 dic 2017
 *      Author: valerio
 */


#include "reader_buffer.h"


//inizializzazione del buffer del reader
buffer_t* buffer_reader_init(int maxsize){

	return buffer_init(maxsize);
}

//deallocazione del buffer del reader
void buffer_reader_destroy(buffer_t* buffer){

	buffer_destroy(buffer);
}

//inserimento bloccante di un messaggio nel buffer_reader
msg_t* put_bloccante_buffer_reader(buffer_t* buffer, msg_t* messaggio){

	return put_bloccante(buffer, messaggio);
}

//inserimento non bloccante di un messaggio nel buffer_reader
msg_t* put_non_bloccante_buffer_reader(buffer_t* buffer, msg_t* messaggio){

	return put_non_bloccante(buffer, messaggio);
}

//prelevamento di un messaggio dal buffer_reader
msg_t* get_buffer_reader(buffer_t* buffer){

	return get_bloccante(buffer);
}

