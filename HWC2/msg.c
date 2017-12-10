#include "msg.h"
#include <stdlib.h>

//creare ed allocare un nuovo messaggio ospitante un generico
//contenuto content di tipo void*
msg_t* msg_init_string(void* content){

	if(content == NULL){return NULL;}
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


//inizializzazione di un blocco di messaggi
blocco_msg_t* inizializzazione_blocco(int numeroMessaggi){

	blocco_msg_t* blocco_messaggi = (blocco_msg_t*)malloc(sizeof(blocco_msg_t));

	blocco_messaggi->messaggi = (msg_t**)malloc(sizeof(msg_t*)*numeroMessaggi);
	blocco_messaggi->dimensione = numeroMessaggi;
	blocco_messaggi->K = 0;

	return blocco_messaggi;
}

//inserimento di un messaggio in un blocco di messaggi
void inserimento_messaggio_nel_blocco(blocco_msg_t* blocco_messaggi,msg_t* messaggio){

	blocco_messaggi->messaggi[blocco_messaggi->K] = messaggio;
	blocco_messaggi->K = blocco_messaggi->K + 1;
}

//deallocazione di un blocco di messaggi
void deallocazione_blocco(blocco_msg_t* blocco_messaggi){

	free(blocco_messaggi->messaggi);
	free(blocco_messaggi);
}
