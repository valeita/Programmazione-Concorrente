/*
 * reader.c
 *
 *  Created on: 06 dic 2017
 *      Author: valerio
 */

#include "reader.h"



//prelevamento dei messaggi dal suo buffer personale e rimozione dalla lista alla ricezione del poison pill
void avvia_reader_di_messaggi(reader_msg* reader){

	msg_t* messaggio;

	while((messaggio = get_buffer_reader(reader->my_buffer_reader)) != NULL){  }

	remove_element_concurrent(reader);
	dealloca_reader(reader);
}



//versione del metodo principale che al suo termine di esecuzione non dealloca la struttura reader
void avvia_reader_di_messaggi_per_testing(reader_msg* reader){

	msg_t* messaggio;

	while((messaggio = get_buffer_reader(reader->my_buffer_reader)) != NULL){  }

	remove_element_concurrent(reader);
	//dealloca_reader(reader);				//la deallocazione del reader "dovrebbe" avvenire qui, tuttavia per eseguire i test, commento questa riga e dealloco nei test
}



//metodo di prova per fare testing unitari.
void fai_qualcosa(){    /*fa niente*/    }


//inizializzazione della struttura dati del reader
reader_msg* inizializzazione_reader(char* nome_reader){

	reader_msg* reader = (reader_msg*)malloc(sizeof(reader_msg));
	reader->my_buffer_reader = buffer_reader_init(DIM_BUFFER_READER);
	reader->nome = nome_reader;

	return reader;
}

//deallocazione della struttura dati del reader
void dealloca_reader(reader_msg* reader){

	buffer_destroy(reader->my_buffer_reader);
	free(reader);
}


//inizializzazione della struttura dati reader usata per fare testing,
//in questo caso, viene creato un buffer unitario, pieno in partenza
reader_msg* inizializzazione_reader_per_testing_reader_lento(char* nome_reader, msg_t* messaggio){

	reader_msg* reader = (reader_msg*)malloc(sizeof(reader_msg));
	reader->my_buffer_reader = buffer_reader_init(1);
	reader->nome = nome_reader;

	put_bloccante_buffer_reader(reader->my_buffer_reader, messaggio);

	return reader;
}


//avvio un reader lento
int avvia_reader_di_messaggi_per_testing_reader_lento(reader_msg* reader){

	int contatore_messaggi_letti = 0;
	msg_t* messaggio;
	sleep(3);

	while((messaggio = get_buffer_reader(reader->my_buffer_reader)) != NULL){
		sleep(3); 		//violazione dell'assunzione di progresso finito, fatto solo per scopo di testing
		contatore_messaggi_letti++;
	}
	contatore_messaggi_letti++;
	remove_element_concurrent(reader);
	//dealloca_reader(reader);
	return contatore_messaggi_letti;
}

