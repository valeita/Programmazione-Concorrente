/*
 * reader_buffer.h
 *
 *  Created on: 07 dic 2017
 *      Author: valerio
 */


#include "buffer.h"
#include "poison_pill.h"

#ifndef reader_BUFFER_H_
#define reader_BUFFER_H_

//inizializzazione del buffer del reader
buffer_t* buffer_reader_init(int maxsize);

//deallocazione del buffer del reader
void buffer_reader_destroy(buffer_t* buffer);

//inserimento bloccante di un messaggio nel buffer_reader
msg_t* put_bloccante_buffer_reader(buffer_t* buffer, msg_t* messaggio);

//inserimento non bloccante di un messaggio nel buffer_reader
msg_t* put_non_bloccante_buffer_reader(buffer_t* buffer, msg_t* messaggio);

//prelevamento di un messaggio dal buffer_reader
msg_t* get_buffer_reader(buffer_t* buffer);


#endif /* reader_BUFFER_H_ */

