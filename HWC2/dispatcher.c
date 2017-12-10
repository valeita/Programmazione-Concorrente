/*
 * dispatcher.c
 *
 *  Created on: 06 dic 2017
 *      Author: valerio
 */

#include "dispatcher.h"
#include "provider_buffer.h"


//funzione di supporto al dispatcher. nel casoin cui un un reader va troppo lento, il dispatcher lo elimina dalla lista
// e delega il compito a un fde di supporto di fare una put bloccante sul relativo buffer in attesa di depositare la poison pill.
// punto focale: si evitano rallentamenti da parte del dispatcher.
void elimina_reader(reader_msg* reader){

	put_bloccante_buffer_reader(reader->my_buffer_reader, POISON_PILL);
}


//il dispatcher avendo ricevuto la poison pill dal buffer_provider, la inoltra a tutti i suoi reader.
void propaga_poison_pill(){

	pthread_mutex_lock(&mutex_list);
	iterator_t* iterator = iterator_init(lista_nodi_reader);

	while(hasNext(iterator)){

		reader_msg* reader_attuale = (reader_msg*)(next(iterator));
		msg_t* messaggio_di_ritorno = put_non_bloccante_buffer_reader(reader_attuale->my_buffer_reader, POISON_PILL);

		if(messaggio_di_ritorno == NULL){	//richiesta inevasa

			removeElement(lista_nodi_reader, reader_attuale);
			pthread_t aiutante;
			pthread_create(&aiutante,NULL,&elimina_reader,reader_attuale);
		}
	}
	iterator_destroy(iterator);
	pthread_mutex_unlock(&mutex_list);
}


//poichè la lista dei readers è vuota, è compito del dispatcher consumare un messaggio letto
void sostituisci_readers(msg_t* messaggio){

	/*  FAI QUALCOSA CON QUESTO MESSAGGIO     */
}



//il dispatcher invia in broadcast un messaggio letto dal buffer provider a tutti i buffer dei reader presenti nella lista
//controlla inoltra che essa sia vuota, poichè in tal caso sarà compito suo quello di consumare il messaggio
void scandisci_lista_e_invia_messaggio_broadcast(msg_t* messaggio){

	pthread_mutex_lock(&mutex_list);
	iterator_t* iterator = iterator_init(lista_nodi_reader);

	while(hasNext(iterator)){

		reader_msg* reader_attuale = (reader_msg*)(next(iterator));
		msg_t* messaggio_di_ritorno = put_non_bloccante_buffer_reader(reader_attuale->my_buffer_reader, messaggio);

		if(messaggio_di_ritorno == NULL){	//richiesta inevasa

			removeElement(lista_nodi_reader, reader_attuale);
			pthread_t aiutante;
			pthread_create(&aiutante,NULL,&elimina_reader,reader_attuale);
		}
	}
	if(size(lista_nodi_reader)==0) {sostituisci_readers(messaggio);}

	iterator_destroy(iterator);
	pthread_mutex_unlock(&mutex_list);
}



//il dispatcher accetta i messaggi e li inoltra in broadcast ai reader presenti in lista
void accetta_e_inoltra(){

	msg_t* messaggio;

	while((messaggio = get_buffer_provider()) != NULL){

		scandisci_lista_e_invia_messaggio_broadcast(messaggio);
	}
	propaga_poison_pill();
}

