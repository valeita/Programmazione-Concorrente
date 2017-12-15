/*
 * reader_list.h
 *
 *  Created on: 07 dic 2017
 *      Author: valerio
 */

#ifndef READER_LIST_H_
#define READER_LIST_H_




#include "list.h"
#include "reader.h"
#include <stdlib.h>


list_t* lista_nodi_reader;			//lista di nodi reader
pthread_mutex_t mutex_list;			//mutex che garantisce la mutua esclusione sulla lista


//inizializzazione della lista e del mutex che gestisce la mutua esclusione sulla lista
void list_init_concurrent();


//deallocazione della lista e del mutex che gestisce la mutua esclusione sulla lista
void list_destroy_concurrent();


////aggiunge un elemento alla lista in mutua esclusione
void add_element_concurrent(reader_msg* reader);


//rimuove un elemento dalla lista in mutua esclusione.
void remove_element_concurrent(void* element);

//calcola la dimensione della lista in mutua esclusione
int size_concurrent();

iterator_t* iterator_init_concurrent();
#endif /* READER_LIST_H_ */
