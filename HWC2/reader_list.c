/*
 * reader_list.c
 *
 *  Created on: 07 dic 2017
 *      Author: valerio
 */

#include "reader_list.h"



//inizializzazione della lista e del mutex che gestisce la mutua esclusione sulla lista
void list_init_concurrent(){

	pthread_mutex_init(&mutex_list,NULL);
	lista_nodi_reader = list_init();
}



//deallocazione della lista e del mutex che gestisce la mutua esclusione sulla lista
void list_destroy_concurrent(){

	list_destroy(lista_nodi_reader);
	pthread_mutex_destroy(&mutex_list);
}


////aggiunge un elemento alla lista in mutua esclusione
void add_element_concurrent(reader_msg* reader){

	pthread_mutex_lock(&mutex_list);
		addElement(lista_nodi_reader, reader);
	pthread_mutex_unlock(&mutex_list);

}

//rimuove un elemento dalla lista in mutua esclusione.
void remove_element_concurrent(void* element){

	pthread_mutex_lock(&mutex_list);
		removeElement(lista_nodi_reader, element);
	pthread_mutex_unlock(&mutex_list);
}


//calcola la dimensione della lista in mutua esclusione
int size_concurrent(){

	pthread_mutex_lock(&mutex_list);
		int dim = size(lista_nodi_reader);
	pthread_mutex_unlock(&mutex_list);
	return dim;
}

//creazione dell'iteratore in modo concorrente
iterator_t* iterator_init_concurrent(){

	pthread_mutex_lock(&mutex_list);
			iterator_t* it = iterator_init(lista_nodi_reader);
	pthread_mutex_unlock(&mutex_list);
	return it;

}
