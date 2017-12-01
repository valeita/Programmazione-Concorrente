#include "buffer.h"


/* allocazione / deallocazione  buffer */
// creazione di un buffer vuoto di dim. max nota
buffer_t* buffer_init(unsigned int maxsize){

	buffer_t* new_buffer = malloc(sizeof(buffer_t));
	new_buffer->coda = malloc(sizeof(msg_t)*maxsize);

	new_buffer->size = maxsize;
	new_buffer->T = 0;
	new_buffer->D = 0;
	pthread_mutex_init(&(new_buffer->uso_T),NULL);
	pthread_mutex_init(&(new_buffer->uso_D),NULL);

	new_buffer->piene = sem_open("semaphore_piene", O_CREAT, 0644, 0);
	new_buffer->vuote = sem_open("semaphore_vuote", O_CREAT, 0644, maxsize);

	return new_buffer;
}

// deallocazione di un buffer
void buffer_destroy(buffer_t* buffer){

	pthread_mutex_destroy(&(buffer->uso_D));
	pthread_mutex_destroy(&(buffer->uso_T));
	sem_close(buffer->piene);
	sem_close(buffer->vuote);
	sem_unlink("semaphore_piene");
	sem_unlink("semaphore_vuote");

	free(buffer->coda);
	free(buffer);
}



/* operazioni sul buffer */
// inserimento bloccante: sospende se pieno, quindi
// effettua l’inserimento non appena si libera dello spazio
// restituisce il messaggio inserito; N.B.: msg!=null
msg_t* put_bloccante(buffer_t* buffer, msg_t* msg){

	sem_wait(buffer->vuote);
		pthread_mutex_lock(&buffer->uso_D);
			buffer->coda[buffer->D] = *msg;
			buffer->D = (buffer->D + 1) % (buffer->size);
		pthread_mutex_unlock(&buffer->uso_D);
	sem_post(buffer->piene);

	return msg;
}




// inserimento non bloccante: restituisce BUFFER_ERROR se pieno,
// altrimenti effettua l’inserimento e restituisce il messaggio
// inserito; N.B.: msg!=null
msg_t* put_non_bloccante(buffer_t* buffer, msg_t* msg){

	if (sem_trywait(buffer->vuote) == -1){ return BUFFER_ERROR};
		pthread_mutex_lock(&buffer->uso_D);
			buffer->coda[buffer->D] = *msg;
			buffer->D = (buffer->D + 1) % (buffer->size);
		pthread_mutex_unlock(&buffer->uso_D);
	sem_post(buffer->piene);

		return msg;
}



// estrazione bloccante: sospende se vuoto, quindi
// restituisce il valore estratto non appena disponibile
msg_t* get_bloccante(buffer_t* buffer){

	sem_wait(buffer->piene);
		pthread_mutex_lock(&buffer->uso_T);
			msg_t* m = msg_init_string(buffer->coda[buffer->T].content);
			buffer->T = (buffer->T + 1) % (buffer->size);
		pthread_mutex_unlock(&buffer->uso_T);
	sem_post(buffer->vuote);

	return m;
}




// estrazione non bloccante: restituisce BUFFER_ERROR se vuoto
// ed il valore estratto in caso contrario
msg_t* get_non_bloccante(buffer_t* buffer){

	if(sem_trywait(buffer->piene) == -1){ return BUFFER_ERROR;}
		pthread_mutex_lock(&buffer->uso_T);
			msg_t* m = msg_init_string(buffer->coda[buffer->T].content);
			buffer->T = (buffer->T + 1) % (buffer->size);
		pthread_mutex_unlock(&buffer->uso_T);
	sem_post(buffer->vuote);

	return m;
}

