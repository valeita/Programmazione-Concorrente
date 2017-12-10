
#ifndef msg_h
#define msg_h


typedef struct msg {
    void* content;   						// generico contenuto del messaggio
    struct msg * (*msg_init)(void*);        // creazione msg
    void (*msg_destroy)(struct msg *);      // deallocazione msg
    struct msg * (*msg_copy)(struct msg *); // creazione/copia msg
} msg_t;


typedef struct blocco_msg {
    msg_t** messaggi;					// un insieme di messaggi
    int dimensione;						//il numero totale dei messaggi contenuti nel blocco
    int K;								//indice di inserimento
} blocco_msg_t;


//creare ed allocare un nuovo messaggio ospitante un generico
//contenuto content di tipo void*
msg_t* msg_init_string(void* content);

//deallocazione di un messaggio
void msg_destroy_string(msg_t* msg);

//copia di un messaggio
msg_t* msg_copy_string(msg_t* msg);

//inizializzazione di un blocco di messaggi
blocco_msg_t* inizializzazione_blocco(int numeroMessaggi);

//inserimento di un messaggio in un blocco di messaggi
void inserimento_messaggio_nel_blocco(blocco_msg_t* blocco_messaggi,msg_t* messaggio);

//deallocazione di un blocco di messaggi
void deallocazione_blocco(blocco_msg_t* blocco_messaggi);

#endif
