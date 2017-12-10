/*
 * accepter.h
 *
 *  Created on: 06 dic 2017
 *      Author: valerio
 */

#ifndef ACCEPTER_H_
#define ACCEPTER_H_

#include "reader.h"
#include "accepter_buffer.h"
#include "reader_list.h"


//l'accepter accetta le richieste dal buffer_Accepter,
//e per ogni richiesta creo un fde reader, e aggiunge la sua struttura dati nella lista
void accetta_richieste();


//versione di accetta_richieste utilizzata per fare testing.
//crea un fde che non dealloca la struttura reader al termine della sua esecuzione
void accetta_richieste_per_testing(int boolean);


// utilizzato per fare testing unitario dell'accepter
void accetta_richieste_per_testing_unitario();


#endif /* ACCEPTER_H_ */
