/*
 * test.c
 *
 *  Created on: 06 dic 2017
 *      Author: valerio
 */


#include "CUnit/Basic.h"
#include "provider_buffer.h"
#include "provider.h"
#include "accepter_buffer.h"
#include "requestor.h"
#include "accepter.h"
#include "reader.h"
#include "dispatcher.h"


//invio di una sequenza di messaggi da  parte del provider nel buffer_provider
void test_provider_invio_sequenza_messaggi(){

	blocco_msg_t* blocco = inizializzazione_blocco(2);
	msg_t* messaggio_1 = msg_init_string("prodotto_1");
	msg_t* messaggio_2 = msg_init_string("prodotto_2");									//set-up
	inserimento_messaggio_nel_blocco(blocco, messaggio_1);
	inserimento_messaggio_nel_blocco(blocco, messaggio_2);

	pthread_t provider;
	pthread_create(&provider,NULL,&invia_sequenza_messaggi,blocco);						//sollecitazione
	pthread_join(provider,NULL);


	CU_ASSERT_EQUAL(buffer_provider->D-buffer_provider->T,0);
	CU_ASSERT_STRING_EQUAL(buffer_provider->coda[0].content,messaggio_1->content);
	CU_ASSERT_STRING_EQUAL(buffer_provider->coda[1].content,messaggio_2->content);		//verifica
	CU_ASSERT_PTR_EQUAL(buffer_provider->coda[2].content,POISON_PILL->content);

	msg_destroy_string(messaggio_1);
	msg_destroy_string(messaggio_2);
	deallocazione_blocco(blocco);
}


//invio della richieste di reader nel buffer_accepter da parte di un requestor
void test_requestor_invio_messaggi_buffer_accepter(){

	pthread_t requestor_1, requestor_2;														//set-up
	msg_t* messaggio_da_inserire = msg_init_string("reader_1");
	msg_t* messaggio_prodotto_1;
	msg_t* messaggio_prodotto_2;

	pthread_create(&requestor_1,NULL,&submitRequest,messaggio_da_inserire);					//sollecitazione
	pthread_join(requestor_1,&messaggio_prodotto_1);

	pthread_create(&requestor_2,NULL,&submitRequest,POISON_PILL);
	pthread_join(requestor_2,&messaggio_prodotto_2);

	CU_ASSERT_STRING_EQUAL(buffer_accepter->coda[0].content,"reader_1");						//verifica
	CU_ASSERT_PTR_NOT_NULL(messaggio_prodotto_1);
	CU_ASSERT_STRING_EQUAL(messaggio_prodotto_1->content,"reader_1");

	CU_ASSERT_PTR_EQUAL(buffer_accepter->coda[1].content,POISON_PILL->content);
	CU_ASSERT_PTR_EQUAL(messaggio_prodotto_2->content,POISON_PILL->content);
	CU_ASSERT_PTR_EQUAL(messaggio_prodotto_2,POISON_PILL);


	msg_destroy_string(messaggio_prodotto_1);
}



//testing del prelevamento di una richiesta e un poison pill dalla coda delle richieste dell'accepter,
//e relativa creazione di reader rispettivamente con inserimento nella lista dei reader
void test_accepter_ricezione_richiesta_e_poison_pill(){

	pthread_t requestor_1,requestor_2, requestor_3, accepter;																			//set-up
	list_init_concurrent();
	msg_t* messaggio_da_inserire_1 = msg_init_string("reader_1");
	msg_t* messaggio_da_inserire_2 = msg_init_string("reader_2");
	msg_t* messaggio_prodotto_1 = NULL;

	pthread_create(&requestor_1,NULL,&submitRequest,messaggio_da_inserire_1);
	pthread_join(requestor_1,&messaggio_prodotto_1);																		//sollecitazione
	pthread_create(&requestor_2,NULL,&submitRequest,POISON_PILL); //inserisco le richieste nella coda
	pthread_join(requestor_2,NULL);
	pthread_create(&requestor_3,NULL,&submitRequest,messaggio_da_inserire_2); //richiesta inevasa
	pthread_join(requestor_3,NULL);

	pthread_create(&accepter,NULL,&accetta_richieste_per_testing_unitario,NULL);
	pthread_join(accepter,NULL);

	CU_ASSERT(!(isEmpty(lista_nodi_reader)));
	CU_ASSERT(size(lista_nodi_reader) == 1);
	CU_ASSERT_STRING_EQUAL(((reader_msg*)(lista_nodi_reader->head->payload))->nome,messaggio_da_inserire_1->content);		//verifica
	CU_ASSERT_PTR_NOT_NULL(((reader_msg*)(lista_nodi_reader->head->payload))->my_buffer_reader);

	dealloca_reader((reader_msg*)(lista_nodi_reader->head->payload));
	list_destroy_concurrent(lista_nodi_reader);
	msg_destroy_string(messaggio_da_inserire_1);
	msg_destroy_string(messaggio_da_inserire_2);
}




//test dispatcher con prelevamento dei messaggi dal buffer_provider e inoltro in broadcast a molteplici reader veloci
void test_dispatcher_prelevamento_e_inoltro_messaggi_molteplici_reader_veloci(){

	blocco_msg_t* blocco = inizializzazione_blocco(2);
	msg_t* messaggio_1 = msg_init_string("prodotto_1");
	msg_t* messaggio_2 = msg_init_string("prodotto_2");												//set-up
	inserimento_messaggio_nel_blocco(blocco, messaggio_1);
	inserimento_messaggio_nel_blocco(blocco, messaggio_2);
	invia_sequenza_messaggi(blocco);
	list_init_concurrent();
	reader_msg* reader_1 = inizializzazione_reader("reader_1");
	reader_msg* reader_2 = inizializzazione_reader("reader_2");
	add_element_concurrent(reader_1);
	add_element_concurrent(reader_2);

	pthread_t dispatcher;
	pthread_create(&dispatcher,NULL,&accetta_e_inoltra,NULL);											//sollecitazione
	pthread_join(dispatcher,NULL);

	CU_ASSERT_STRING_EQUAL(reader_1->my_buffer_reader->coda[0].content,messaggio_1->content);
	CU_ASSERT_STRING_EQUAL(reader_2->my_buffer_reader->coda[0].content,messaggio_1->content);
	CU_ASSERT_STRING_EQUAL(reader_1->my_buffer_reader->coda[1].content,messaggio_2->content);			//verifica
	CU_ASSERT_STRING_EQUAL(reader_2->my_buffer_reader->coda[1].content,messaggio_2->content);
	CU_ASSERT_PTR_EQUAL(reader_1->my_buffer_reader->coda[2].content,POISON_PILL->content);
	CU_ASSERT_PTR_EQUAL(reader_2->my_buffer_reader->coda[2].content,POISON_PILL->content);

	dealloca_reader(reader_1);
	dealloca_reader(reader_2);
	list_destroy_concurrent(lista_nodi_reader);
	msg_destroy_string(messaggio_1);
	msg_destroy_string(messaggio_2);
	deallocazione_blocco(blocco);
}











//testing della rimozione anticipata di un reader perchè considerato lento dal dispatcher
//qui non si può parlare di test unitario in senso stretto poichè si stanno testando dei flussi che interagiscono.
void test_dispatcher_prelevamento_e_inoltro_messaggi_molteplici_reader_di_cui_uno_lento(){

	msg_t* messaggio_da_inserire_nel_buffer = msg_init_string("prova");
	blocco_msg_t* blocco = inizializzazione_blocco(2);
	msg_t* messaggio_1 = msg_init_string("prodotto_1");
	msg_t* messaggio_2 = msg_init_string("prodotto_2");																	//set-up
	int* messaggi_letti;
	inserimento_messaggio_nel_blocco(blocco, messaggio_1);
	inserimento_messaggio_nel_blocco(blocco, messaggio_2);
	invia_sequenza_messaggi(blocco);
	list_init_concurrent();
	reader_msg* reader_messaggi_1 = inizializzazione_reader_per_testing_reader_lento("reader_1",messaggio_da_inserire_nel_buffer);
	reader_msg* reader_messaggi_2 = inizializzazione_reader("reader_2");
	add_element_concurrent(reader_messaggi_1);
	add_element_concurrent(reader_messaggi_2);

	pthread_t reader_1, reader_2, dispatcher;

	pthread_create(&reader_1,NULL,&avvia_reader_di_messaggi_per_testing_reader_lento,reader_messaggi_1);
	pthread_create(&reader_2,NULL,&avvia_reader_di_messaggi_per_testing,reader_messaggi_2);
	pthread_create(&dispatcher,NULL,&accetta_e_inoltra,NULL);																//sollecitazione
	pthread_join(dispatcher,NULL);
	pthread_join(reader_1,&messaggi_letti);
	pthread_join(reader_2,NULL);

	CU_ASSERT_STRING_EQUAL(reader_messaggi_2->my_buffer_reader->coda[0].content,messaggio_1->content);
	CU_ASSERT_STRING_EQUAL(reader_messaggi_2->my_buffer_reader->coda[1].content,messaggio_2->content);
	CU_ASSERT_PTR_EQUAL(reader_messaggi_2->my_buffer_reader->coda[2].content,POISON_PILL->content);						//verifica

	CU_ASSERT_EQUAL(messaggi_letti,2);		//ha letto il messaggio che gia c'era, e poi la poison pill inviata dal dispatcher
	CU_ASSERT_PTR_EQUAL(reader_messaggi_1->my_buffer_reader->coda[0].content,POISON_PILL->content);

	dealloca_reader(reader_messaggi_1);
	dealloca_reader(reader_messaggi_2);
	list_destroy_concurrent(lista_nodi_reader);
	msg_destroy_string(messaggio_1);
	msg_destroy_string(messaggio_2);
	msg_destroy_string(messaggio_da_inserire_nel_buffer);
	deallocazione_blocco(blocco);
}





// testing del funzionamento di un reader che preleva dal suo buffer i messaggi e si autorimuove dalla lista.
void test_prelevamento_messaggi_e_rimozione_dalla_lista_reader(){

	pthread_t reader;
	msg_t* messaggio_da_inserire = msg_init_string("prodotto_1");
	reader_msg* reader_messaggi = inizializzazione_reader("reader_1");											//set-up
	add_element_concurrent(reader_messaggi);
	CU_ASSERT(size(lista_nodi_reader)==1);
	put_bloccante(reader_messaggi->my_buffer_reader, messaggio_da_inserire);
	put_bloccante(reader_messaggi->my_buffer_reader, POISON_PILL);

	pthread_create(&reader,NULL,&avvia_reader_di_messaggi_per_testing,reader_messaggi);											//sollecitazione
	pthread_join(reader,NULL);

	CU_ASSERT_EQUAL(reader_messaggi->my_buffer_reader->T,2);
	CU_ASSERT_STRING_EQUAL(reader_messaggi->my_buffer_reader->coda[0].content,messaggio_da_inserire->content);
	CU_ASSERT_PTR_EQUAL(reader_messaggi->my_buffer_reader->coda[1].content,POISON_PILL->content);
	CU_ASSERT(size(lista_nodi_reader)==0);

	dealloca_reader(reader_messaggi);
	msg_destroy_string(messaggio_da_inserire);
}





void test_main(){

	buffer_provider_init(3);
	buffer_accepter_init(3);
	list_init_concurrent();
	blocco_msg_t* blocco = inizializzazione_blocco(2);
	msg_t* messaggio_1 = msg_init_string("prodotto_1");
	msg_t* messaggio_2 = msg_init_string("prodotto_2");
	inserimento_messaggio_nel_blocco(blocco, messaggio_1);
	inserimento_messaggio_nel_blocco(blocco, messaggio_2);													//set-up
	msg_t* messaggio_da_inserire_1 = msg_init_string("reader_1");
	msg_t* messaggio_da_inserire_2 = msg_init_string("reader_2");


	// AVVIO PROVIDER \\

	pthread_t provider;
	pthread_create(&provider,NULL,&invia_sequenza_messaggi,blocco);

	// AVVIO ACCEPTER \\

	pthread_t accepter;
	pthread_create(&accepter,NULL,&accetta_richieste,NULL);


	// INVIO RICHIESTE \\

	pthread_t requestor_1, requestor_2, requestor_poison;
	pthread_create(&requestor_1,NULL,&submitRequest,messaggio_da_inserire_1);														//sollecitazione
	pthread_create(&requestor_2,NULL,&submitRequest,messaggio_da_inserire_2);

	pthread_join(provider,NULL);

	pthread_create(&requestor_poison,NULL,&submitRequest,POISON_PILL);

	pthread_join(requestor_1,NULL);
	pthread_join(requestor_2,NULL);
	pthread_join(requestor_poison,NULL);
	pthread_join(accepter,NULL);


	CU_ASSERT(size_concurrent()==2);
	CU_ASSERT_STRING_EQUAL(((reader_msg*)(lista_nodi_reader->head->payload))->nome,messaggio_da_inserire_1->content);				//pre-verifica
	CU_ASSERT_STRING_EQUAL(((reader_msg*)(lista_nodi_reader->tail->payload))->nome,messaggio_da_inserire_2->content);


	// AVVIO DISPATCHER \\

	pthread_t dispatcher;
	pthread_create(&dispatcher,NULL,&accetta_e_inoltra,NULL);
	pthread_join(dispatcher,NULL);

	sleep(5);		//violazione dell'assunzione di progresso finito fatto per scopi di testing

	CU_ASSERT_STRING_EQUAL(buffer_provider->coda[0].content,messaggio_1->content);
	CU_ASSERT_STRING_EQUAL(buffer_provider->coda[1].content,messaggio_2->content);
	CU_ASSERT_PTR_EQUAL(buffer_provider->coda[2].content,POISON_PILL->content);													//verifica


	CU_ASSERT_STRING_EQUAL(buffer_accepter->coda[0].content,messaggio_da_inserire_1->content);
	CU_ASSERT_STRING_EQUAL(buffer_accepter->coda[1].content,messaggio_da_inserire_2->content);
	CU_ASSERT_PTR_EQUAL(buffer_accepter->coda[2].content,POISON_PILL->content);

	CU_ASSERT(size_concurrent() == 0);


	msg_destroy_string(messaggio_da_inserire_1);
	msg_destroy_string(messaggio_da_inserire_2);
	msg_destroy_string(messaggio_1);
	msg_destroy_string(messaggio_2);
	deallocazione_blocco(blocco);
	list_destroy_concurrent(lista_nodi_reader);
	buffer_destroy(buffer_accepter);
	buffer_destroy(buffer_provider);
}





int init_buffer_provider(){buffer_provider_init(3); return 0;}

int clean_buffer_provider(){buffer_destroy(buffer_provider); return 0;}


int init_buffer_accepter(){buffer_accepter_init(3); return 0;}

int clean_buffer_accepter(){buffer_destroy(buffer_accepter); return 0;}


int init_list(){list_init_concurrent();return 0;}

int clean_list(){list_destroy_concurrent(lista_nodi_reader);return 0;}


int init_nothing(){ return 0;}

int clean_nothing(){ return 0;}



int test(){


	CU_pSuite pSuite = NULL;

    if (CUE_SUCCESS != CU_initialize_registry())
	   return CU_get_error();

    pSuite = CU_add_suite("Suite_provider", init_buffer_provider, clean_buffer_provider);
    CU_add_test(pSuite, "invio di una sequenza di messaggi da  parte del provider nel buffer_provider", test_provider_invio_sequenza_messaggi);

    pSuite = CU_add_suite("Suite_accepter_1", init_buffer_accepter, clean_buffer_accepter);
    CU_add_test(pSuite, "invio della richieste di reader nel buffer_accepter da parte di un requestor",test_requestor_invio_messaggi_buffer_accepter);

    pSuite = CU_add_suite("Suite_accepter_2", init_buffer_accepter, clean_buffer_accepter);
    CU_add_test(pSuite, "testing di prelevamento di richiesta e poison pill dalla coda delle richieste dell'accepter,e creazione di reader con inserimento in lista",test_accepter_ricezione_richiesta_e_poison_pill);

    pSuite = CU_add_suite("Suite_dispatcher_1", init_buffer_provider, clean_buffer_provider);
    CU_add_test(pSuite, "test dispatcher con prelevamento dei messaggi dal buffer_provider e inoltro broadcast a molteplici reader veloci",test_dispatcher_prelevamento_e_inoltro_messaggi_molteplici_reader_veloci);

    pSuite = CU_add_suite("Suite_dispatcher_2",init_buffer_provider, clean_buffer_provider);
    CU_add_test(pSuite, "testing della rimozione anticipata di un reader perchè considerato lento dal dispatcher",test_dispatcher_prelevamento_e_inoltro_messaggi_molteplici_reader_di_cui_uno_lento);

    pSuite = CU_add_suite("Suite_reader_1", init_list, clean_list);
    CU_add_test(pSuite, "testing del funzionamento di un reader che preleva dal suo buffer i messaggi e si autorimuove dalla lista.",test_prelevamento_messaggi_e_rimozione_dalla_lista_reader);

    pSuite = CU_add_suite("Suite_main", init_nothing, clean_nothing);
    CU_add_test(pSuite, "test flusso principale main",test_main);


    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
