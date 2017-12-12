/*
 * main.c
 *
 *  Created on: 06 dic 2017
 *      Author: valerio
 */

#include "provider_buffer.h"
#include "provider.h"
#include "accepter_buffer.h"
#include "requestor.h"
#include "accepter.h"
#include "reader.h"
#include "dispatcher.h"



int main(){

	return test();
}


//questo metodo controlla se ci sono ancora reader esistenti, e gli invia una poison pill in tal caso.
void controlla_reader_ancora_esistenti(){

	pthread_mutex_lock(&mutex_list);
	iterator_t* iterator = iterator_init(lista_nodi_reader);

	while(hasNext(iterator)){

		reader_msg* reader_attuale = (reader_msg*)(next(iterator));
		put_bloccante_buffer_reader(reader_attuale->my_buffer_reader, POISON_PILL);
	}
	iterator_destroy(iterator);
	pthread_mutex_unlock(&mutex_list);

	while(size_concurrent()>0){}
}







// flusso di esecuzione main principale:
//Questo flusso ha la responsabilit`a di creare il sistema di flussi descritto, e di gestire attivamente il transitorio di chiusura finale
void flusso_main_principale(){

	buffer_provider_init(3);
	buffer_accepter_init(3);
	list_init_concurrent();
	blocco_msg_t* blocco = inizializzazione_blocco(2);
	msg_t* messaggio_1 = msg_init_string("prodotto_1");
	msg_t* messaggio_2 = msg_init_string("prodotto_2");
	inserimento_messaggio_nel_blocco(blocco, messaggio_1);
	inserimento_messaggio_nel_blocco(blocco, messaggio_2);
	msg_t* messaggio_da_inserire_1 = msg_init_string("reader_1");
	msg_t* messaggio_da_inserire_2 = msg_init_string("reader_2");


	// AVVIO PROVIDER \\

	pthread_t provider;
	pthread_create(&provider,NULL,&invia_sequenza_messaggi,blocco);

	// AVVIO ACCEPTER \\

	pthread_t accepter;
	pthread_create(&accepter,NULL,&accetta_richieste_per_testing,NULL);


	// INVIO RICHIESTE \\

	pthread_t requestor_1, requestor_2, requestor_poison;
	pthread_create(&requestor_1,NULL,&submitRequest,messaggio_da_inserire_1);
	pthread_create(&requestor_2,NULL,&submitRequest,messaggio_da_inserire_2);

	pthread_join(provider,NULL);

	pthread_create(&requestor_poison,NULL,&submitRequest,POISON_PILL);

	// AVVIO DISPATCHER \\

	pthread_t dispatcher;
	pthread_create(&dispatcher,NULL,&accetta_e_inoltra,NULL);


	pthread_join(accepter,NULL);
	pthread_join(requestor_1,NULL);
	pthread_join(requestor_2,NULL);
	pthread_join(requestor_poison,NULL);
	pthread_join(dispatcher,NULL);

	controlla_reader_ancora_esistenti();

	msg_destroy_string(messaggio_da_inserire_1);
	msg_destroy_string(messaggio_da_inserire_2);
	msg_destroy_string(messaggio_1);
	msg_destroy_string(messaggio_2);
	deallocazione_blocco(blocco);
	list_destroy_concurrent(lista_nodi_reader);
	buffer_destroy(buffer_accepter);
	buffer_destroy(buffer_provider);
}
