/*
 * dispatcher.h
 *
 *  Created on: 06 dic 2017
 *      Author: valerio
 */

#ifndef DISPATCHER_H_
#define DISPATCHER_H_

#include <stdio.h>
#include "stdlib.h"
#include "msg.h"
#include "reader_list.h"


//funzione di supporto al dispatcher. nel casoin cui un un reader va troppo lento, il dispatcher lo elimina dalla lista
// e delega il compito a un fde di supporto di fare una put bloccante sul relativo buffer in attesa di depositare la poison pill.
// punto focale: si evitano rallentamenti da parte del dispatcher.
void elimina_reader(reader_msg* reader);


//il dispatcher avendo ricevuto la poison pill dal buffer_provider, la inoltra a tutti i suoi reader.
void propaga_poison_pill();


//poichè la lista dei readers è vuota, è compito del dispatcher consumare un messaggio letto
void sostituisci_readers(msg_t* messaggio);


//il dispatcher invia in broadcast un messaggio letto dal buffer provider a tutti i buffer dei reader presenti nella lista
//controlla inoltra che essa sia vuota, poichè in tal caso sarà compito suo quello di consumare il messaggio
void scandisci_lista_e_invia_messaggio_broadcast(msg_t* messaggio);


//il dispatcher accetta i messaggi e li inoltra in broadcast ai reader presenti in lista
void accetta_e_inoltra();


#endif /* DISPATCHER_H_ */
