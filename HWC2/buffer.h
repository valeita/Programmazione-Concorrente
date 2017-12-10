#ifndef buffer_h
#define buffer_h

#include "msg.h"
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define BUFFER_ERROR  (msg_t *) NULL;

typedef struct buffer{

	msg_t* coda;
	int T; 					// indice usano per prelevare messaggi
	int D; 					// indice usato per depositare messaggi
	int size;				 //dimensioni del buffer
	pthread_mutex_t uso_D;	// mutex associato all'indice di produzione
	pthread_mutex_t uso_T;	// mutex associato all'indice di consumazione
	sem_t* piene;			//semaforo per gestire l'accesso al buffer. inizializzato a 0.
	sem_t* vuote;			//semaforo per gestire l'accesso al buffer. inizializzato a size.

}buffer_t;


//funzione che genera stringhe casuali, da assegnare ai semafori
//necessaria per chiamare semafori in modo diverso a runtime.
char *randstring(int length);


// inizializzazione del buffer
buffer_t* buffer_init(unsigned int maxsize);


// deallocazione di un buffer
void buffer_destroy(buffer_t* buffer);



/* operazioni sul buffer */
// inserimento bloccante: sospende se pieno, quindi
// effettua l’inserimento non appena si libera dello spazio
// restituisce il messaggio inserito; N.B.: msg!=null
msg_t* put_bloccante(buffer_t* buffer, msg_t* msg);






// inserimento non bloccante: restituisce BUFFER_ERROR se pieno,
// altrimenti effettua l’inserimento e restituisce il messaggio
// inserito; N.B.: msg!=null
msg_t* put_non_bloccante(buffer_t* buffer, msg_t* msg);







// estrazione bloccante: sospende se vuoto, quindi
// restituisce il valore estratto non appena disponibile
msg_t* get_bloccante(buffer_t* buffer);







// estrazione non bloccante: restituisce BUFFER_ERROR se vuoto
// ed il valore estratto in caso contrario
msg_t* get_non_bloccante(buffer_t* buffer);

#endif

