/*
 * accepter.c
 *
 *  Created on: 06 dic 2017
 *      Author: valerio
 */

#include "accepter.h"



//l'accepter accetta le richieste dal buffer_Accepter,
//e per ogni richiesta creo un fde reader, e aggiunge la sua struttura dati nella lista
void accetta_richieste(){

	msg_t* messaggio;

	while(((messaggio = get_buffer_accepter())) != NULL){

		reader_msg* reader = inizializzazione_reader(messaggio->content);
		add_element_concurrent(reader);

		pthread_t reader_thread;
		pthread_create(&reader_thread,NULL,avvia_reader_di_messaggi,reader);
	}
}



//versione di accetta_richieste utilizzata per fare testing.
//crea un fde che non dealloca la struttura reader al termine della sua esecuzione
void accetta_richieste_per_testing(){

	msg_t* messaggio;

	while(((messaggio = get_buffer_accepter())) != NULL){

		reader_msg* reader = inizializzazione_reader(messaggio->content);
		add_element_concurrent(reader);

		pthread_t reader_thread;
		pthread_create(&reader_thread,NULL,avvia_reader_di_messaggi_per_testing,reader);
	}
}

// utilizzato per fare testing unitario dell'accepter
void accetta_richieste_per_testing_unitario(){

	msg_t* messaggio;

	while(((messaggio = get_buffer_accepter())) != NULL){

		reader_msg* reader = inizializzazione_reader(messaggio->content);
		add_element_concurrent(reader);

		pthread_t reader_thread;
		pthread_create(&reader_thread,NULL,&fai_qualcosa,NULL);
	}
}
