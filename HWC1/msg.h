
#ifndef msg_h
#define msg_h


typedef struct msg {
    void* content;   						// generico contenuto del messaggio
    struct msg * (*msg_init)(void*);        // creazione msg
    void (*msg_destroy)(struct msg *);      // deallocazione msg
    struct msg * (*msg_copy)(struct msg *); // creazione/copia msg
} msg_t;



//creare ed allocare un nuovo messaggio ospitante un generico
//contenuto content di tipo void*
msg_t* msg_init_string(void* content);

//deallocazione di un messaggio
void msg_destroy_string(msg_t* msg);

//copia di un messaggio
msg_t* msg_copy_string(msg_t* msg);


#endif
