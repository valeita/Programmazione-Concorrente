/*
 * msg.c
 *
 *  Created on: 20 nov 2017
 *      Author: valerio
 */

#include "msg.h"


//creare ed allocare un nuovo messaggio ospitante un generico
//contenuto content di tipo void*
msg_t* msg_init_string(void* content){

	    msg_t* new_msg = (msg_t*)malloc( sizeof(msg_t) );

	    //viene creata una copia "privata" della stringa
	    char* string = (char*)content;
	    char* new_content = (char*)malloc(strlen(string)+1); // +1 per \0 finale
	    strcpy(new_content, string);

	    new_msg->content = new_content;
	    new_msg->msg_init = msg_init_string;
	    new_msg->msg_destroy = msg_destroy_string;
	    new_msg->msg_copy = msg_copy_string;

	    return new_msg;
}


//deallocazione di un messaggio
void msg_destroy_string(msg_t* msg) {
    free(msg->content);
    free(msg);
}

//copia di un messaggio
msg_t* msg_copy_string(msg_t* msg) {
	return msg_init_string( msg->content );
}
