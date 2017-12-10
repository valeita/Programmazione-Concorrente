/*
 * provider.c
 *
 *  Created on: 06 dic 2017
 *      Author: valerio
 */

#include "provider.h"

//invio della sequenza dimessaggi nel buffer + il messaggio POISON PILL
void invia_sequenza_messaggi(blocco_msg_t* blocco_messaggi){

	for(int i=0; i<blocco_messaggi->dimensione; i++){

		put_buffer_provider(blocco_messaggi->messaggi[i]);
	}
	put_buffer_provider(POISON_PILL);
}
