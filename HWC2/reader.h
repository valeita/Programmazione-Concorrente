/*
 * reader.h
 *
 *  Created on: 06 dic 2017
 *      Author: valerio
 */



#ifndef READER_H_
#define READER_H_


#define DIM_BUFFER_READER    3
#include <stdlib.h>
#include "reader_buffer.h"


typedef struct reader{

	buffer_t* my_buffer_reader;		//il buffer personale associato al reader
	char* nome;						//nome del reader

}reader_msg;


//prelevamento dei messaggi dal suo buffer personale e rimozione dalla lista alla ricezione del poison pill
void avvia_reader_di_messaggi(reader_msg* reader);


//versione del metodo principale che al suo termine di esecuzione non dealloca la struttura reader
void avvia_reader_di_messaggi_per_testing(reader_msg* reader);


//inizializzazione della struttura dati del reader
reader_msg* inizializzazione_reader();


//metodo di prova per fare testing unitari.
void fai_qualcosa();


//deallocazione della struttura dati del reader
void dealloca_reader(reader_msg* reader);

//inizializzazione della struttura dati reader usata per fare testing,
//in questo caso, viene creato un buffer unitario, pieno in partenza
reader_msg* inizializzazione_reader_per_testing_reader_lento(char* nome_reader,msg_t* messaggio);

//avvio un reader lento
int avvia_reader_di_messaggi_per_testing_reader_lento(reader_msg* reader);


#endif /* READER_H_ */
