/*
 ============================================================================
 Name        : PC_project.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "buffer.h"
#include "msg.h"
#include "CUnit/Basic.h"
#include <string.h>
#include <semaphore.h>

static buffer_t* buffer;



//(P=1; C=0; N=1), Produzione bloccante di un solo messaggio in un buffer vuoto
void test_produzione_bloccante_unico_messaggio_in_buffer_vuoto(){

	msg_t* messaggio_da_inserire = msg_init_string("zero");
	msg_t* messaggio_inserito = put_bloccante(buffer,messaggio_da_inserire);
	CU_ASSERT_STRING_EQUAL(messaggio_da_inserire->content,messaggio_inserito->content);
	CU_ASSERT_PTR_EQUAL(messaggio_inserito,messaggio_da_inserire);

	msg_destroy_string(messaggio_da_inserire);

}

//(P=1; C=0; N=1), produzione non bloccante di un solo messaggio in un buffer vuoto
void test_produzione_non_bloccante_unico_messaggio_in_buffer_vuoto(){

	msg_t* messaggio_da_inserire = msg_init_string("zero");
	msg_t* messaggio_inserito = put_non_bloccante(buffer,messaggio_da_inserire);
	CU_ASSERT_STRING_EQUAL(messaggio_da_inserire->content,messaggio_inserito->content);
	CU_ASSERT_PTR_EQUAL(messaggio_inserito,messaggio_da_inserire);

	msg_destroy_string(messaggio_da_inserire);


}


//(P=0; C=1; N=1) Consumazione bloccante di un solo messaggio da un buffer pieno
void consumazione_bloccante_unico_messaggio_in_buffer_pieno(){

	msg_t* messaggio_da_inserire_1 = msg_init_string("zero");
	msg_t* messaggio_da_inserire_2 = msg_init_string("uno");
	msg_t* messaggio_da_inserire_3 = msg_init_string("due");

	put_bloccante(buffer,messaggio_da_inserire_1);
	put_bloccante(buffer,messaggio_da_inserire_2);
	put_bloccante(buffer,messaggio_da_inserire_3);

	msg_t* messaggio_consumato = get_bloccante(buffer);
	CU_ASSERT_STRING_EQUAL(messaggio_consumato->content,messaggio_da_inserire_1->content);

	msg_destroy_string(messaggio_da_inserire_1);
	msg_destroy_string(messaggio_da_inserire_2);
	msg_destroy_string(messaggio_da_inserire_3);

}

//(P=0; C=1; N=1) Consumazione non bloccante di un solo messaggio da un buffer pieno
void consumazione_non_bloccante_unico_messaggio_in_buffer_pieno(){

	msg_t* messaggio_da_inserire_1 = msg_init_string("zero");
	msg_t* messaggio_da_inserire_2 = msg_init_string("uno");
	msg_t* messaggio_da_inserire_3 = msg_init_string("due");

	put_non_bloccante(buffer,messaggio_da_inserire_1);
	put_non_bloccante(buffer,messaggio_da_inserire_2);
	put_non_bloccante(buffer,messaggio_da_inserire_3);
	msg_t* messaggio_consumato = get_non_bloccante(buffer);
	CU_ASSERT_STRING_EQUAL(messaggio_consumato->content,messaggio_da_inserire_1->content);

	msg_destroy_string(messaggio_da_inserire_1);
	msg_destroy_string(messaggio_da_inserire_2);

}



//(P=1; C=0; N=1) Produzione non bloccante in un buffer pieno
void produzione_bloccante_buffer_pieno(){

	msg_t* messaggio_da_inserire_1 = msg_init_string("zero");
	msg_t* messaggio_da_inserire_2 = msg_init_string("uno");
	msg_t* messaggio_da_inserire_3 = msg_init_string("due");
	msg_t* messaggio_in_eccesso = msg_init_string("tre");

	put_non_bloccante(buffer,messaggio_da_inserire_1);
	put_non_bloccante(buffer,messaggio_da_inserire_2);
	put_non_bloccante(buffer,messaggio_da_inserire_3);
	msg_t* messaggio_di_errore = put_non_bloccante(buffer,messaggio_in_eccesso);
	CU_ASSERT_PTR_NOT_NULL(messaggio_da_inserire_1);
	CU_ASSERT_PTR_NOT_NULL(messaggio_da_inserire_2);
	CU_ASSERT_PTR_NOT_NULL(messaggio_da_inserire_3);
	CU_ASSERT_PTR_NULL(messaggio_di_errore);

	msg_destroy_string(messaggio_da_inserire_1);
	msg_destroy_string(messaggio_da_inserire_2);
	msg_destroy_string(messaggio_da_inserire_3);
	msg_destroy_string(messaggio_in_eccesso);

}

//(P=0; C=1; N=1) Consumazione non bloccante da un buffer vuoto
void consumazione_non_bloccante_da_buffer_vuoto(){

	msg_t* messaggio_di_errore = get_non_bloccante(buffer);
	CU_ASSERT_PTR_NULL(messaggio_di_errore);

}


//CONSUMATORE USA E GETTA
msg_t* get_bloccante_thread(void){

	return get_bloccante(buffer);
}
msg_t* get_non_bloccante_thread(void){

	return get_non_bloccante(buffer);
}


//PRODUTTORE USA E GETTA
msg_t* put_bloccante_thread(msg_t* msg){

	return put_bloccante(buffer,msg);

}
msg_t* put_non_bloccante_thread(msg_t* msg){

	return put_non_bloccante(buffer,msg);
}


//(P=1; C=1; N=1) Consumazione e produzione non bloccante concorrente di un messaggio da un buffer unitario; prima il consumatore
void consumazione_produzione_non_bloccante_unico_messaggio_buffer_unitario_prima_consumatore(){

	pthread_t consumatore_1, produttore_1;
	msg_t* messaggio_da_inserire = msg_init_string("prova");
	msg_t* messaggio_consumato = NULL;

	pthread_create(&consumatore_1, NULL, &get_non_bloccante_thread, NULL);
	pthread_join(consumatore_1,&messaggio_consumato);
	pthread_create(&produttore_1, NULL, &put_non_bloccante_thread, messaggio_da_inserire);
	pthread_join(produttore_1,NULL);
	CU_ASSERT_PTR_NULL(messaggio_consumato);

	msg_destroy_string(messaggio_da_inserire);

}


//(P=1; C=1; N=1) Consumazione e produzione bloccante concorrente di un messaggio in un buffer unitario; prima il produttore
void consumazione_produzione_bloccante_buffer_unitario_prima_produttore(){

	pthread_t consumatore_1, produttore_1;
	msg_t* messaggio_da_inserire = msg_init_string("prova");
	msg_t* messaggio_consumato = NULL;
	msg_t* messaggio_prodotto = NULL;

	pthread_create(&produttore_1, NULL, &put_bloccante_thread, messaggio_da_inserire);
	pthread_create(&consumatore_1, NULL, &get_bloccante_thread, NULL);
	pthread_join(produttore_1,&messaggio_prodotto);
	pthread_join(consumatore_1,&messaggio_consumato);

	CU_ASSERT_STRING_EQUAL(messaggio_consumato->content,messaggio_prodotto->content);

	msg_destroy_string(messaggio_consumato);
	msg_destroy_string(messaggio_da_inserire);
}



//(P>1; C=0; N=1) Produzione non bloccante e concorrente di molteplici messaggi in un buffer unitario vuoto
void produzione_non_bloccante_molteplici_messaggi_buffer_unitario_vuoto(){

	pthread_t produttore_1,produttore_2,produttore_3;
	msg_t* messaggio_da_inserire_1 = msg_init_string("zero");
	msg_t* messaggio_da_inserire_2 = msg_init_string("uno");
	msg_t* messaggio_da_inserire_3 = msg_init_string("due");
	msg_t* messaggio_prodotto_1=NULL;
	msg_t* messaggio_prodotto_2=NULL;
	msg_t* messaggio_prodotto_3=NULL;
	int numeroPuntatoriNulli = 0;
	int numeroPuntatoriNonNulli = 0;

	pthread_create(&produttore_1, NULL, &put_non_bloccante_thread, messaggio_da_inserire_1);
	pthread_create(&produttore_2, NULL, &put_non_bloccante_thread, messaggio_da_inserire_2);
	pthread_create(&produttore_3, NULL, &put_non_bloccante_thread, messaggio_da_inserire_3);
	pthread_join(produttore_1,&messaggio_prodotto_1);
	pthread_join(produttore_2,&messaggio_prodotto_2);
	pthread_join(produttore_3,&messaggio_prodotto_3);

	if(messaggio_prodotto_1==NULL){numeroPuntatoriNulli++;}else{numeroPuntatoriNonNulli++;}
	if(messaggio_prodotto_2==NULL){numeroPuntatoriNulli++;}else{numeroPuntatoriNonNulli++;}
	if(messaggio_prodotto_3==NULL){numeroPuntatoriNulli++;}else{numeroPuntatoriNonNulli++;}

	CU_ASSERT_EQUAL(numeroPuntatoriNulli,2);
	CU_ASSERT_EQUAL(numeroPuntatoriNonNulli,1);

	msg_destroy_string(messaggio_da_inserire_1);
	msg_destroy_string(messaggio_da_inserire_2);
	msg_destroy_string(messaggio_da_inserire_3);

}



//(P=0; C>1; N=1) Consumazione concorrente di molteplici messaggi da un buffer unitario pieno
void consumazione_non_bloccante_messaggi_multipli_buffer_unitario_pieno(){
	
	pthread_t produttore;
	pthread_t consumatore_1,consumatore_2,consumatore_3;
	msg_t* messaggio_da_inserire = msg_init_string("zero");
	msg_t* messaggio_prodotto=NULL;
	msg_t*messaggio_consumato_1;
	msg_t*messaggio_consumato_2;
	msg_t*messaggio_consumato_3;
	int numeroPuntatoriNulli = 0;
	int numeroPuntatoriNonNulli = 0;

	pthread_create(&produttore, NULL, &put_non_bloccante_thread, messaggio_da_inserire);
	pthread_join(produttore,&messaggio_prodotto);

	pthread_create(&consumatore_1, NULL, &get_non_bloccante_thread, NULL);
	pthread_create(&consumatore_2, NULL, &get_non_bloccante_thread, NULL);
	pthread_create(&consumatore_3, NULL, &get_non_bloccante_thread, NULL);
	pthread_join(consumatore_1,&messaggio_consumato_1);
	pthread_join(consumatore_2,&messaggio_consumato_2);
	pthread_join(consumatore_3,&messaggio_consumato_3);

	if(messaggio_consumato_1==NULL){numeroPuntatoriNulli++;}else{numeroPuntatoriNonNulli++;}
	if(messaggio_consumato_2==NULL){numeroPuntatoriNulli++;}else{numeroPuntatoriNonNulli++;}
	if(messaggio_consumato_3==NULL){numeroPuntatoriNulli++;}else{numeroPuntatoriNonNulli++;}

	CU_ASSERT_EQUAL(numeroPuntatoriNulli,2);
	CU_ASSERT_EQUAL(numeroPuntatoriNonNulli,1);

	msg_destroy_string(messaggio_da_inserire);

}
		
		
		

//(P>1; C=0; N>1) Produzione bloccante e concorrente di molteplici messaggi in un buffer vuoto; il buffer non si riempe
void produzione_bloccante_molteplici_messaggi_buffer_vuoto_non_riempito(){

	pthread_t produttore1,produttore2,produttore3;
	msg_t* messaggio_da_inserire_1 = msg_init_string("zero");
	msg_t* messaggio_da_inserire_2 = msg_init_string("uno");
	msg_t* messaggio_prodotto_1=NULL;
	msg_t* messaggio_prodotto_2=NULL;

	pthread_create(&produttore1, NULL, &put_bloccante_thread, messaggio_da_inserire_1);
	pthread_create(&produttore2, NULL, &put_bloccante_thread, messaggio_da_inserire_2);
	pthread_join(produttore1,&messaggio_prodotto_1);
	pthread_join(produttore2,&messaggio_prodotto_2);
	CU_ASSERT_EQUAL(buffer->D - buffer->T,2);
	CU_ASSERT_NOT_EQUAL(buffer->D - buffer->T,buffer->size);

	msg_destroy_string(messaggio_da_inserire_1);
	msg_destroy_string(messaggio_da_inserire_2);

}



//(P>1; C=0; N>1) Produzione non bloccante e concorrente di molteplici messaggi in un buffer vuoto; il buffer si satura in corso
void produzione_non_bloccante_molteplici_messaggi_buffer_vuoto_saturato_in_corso(){

	pthread_t produttore_1,produttore_2,produttore_3,produttore_4,produttore_5;
	msg_t* messaggio_da_inserire_1 = msg_init_string("zero");
	msg_t* messaggio_da_inserire_2 = msg_init_string("uno");
	msg_t* messaggio_da_inserire_3 = msg_init_string("due");
	msg_t* messaggio_da_inserire_4 = msg_init_string("tre");
	msg_t* messaggio_da_inserire_5 = msg_init_string("quattro");
	int numeroPuntatoriNulli = 0;
	int numeroPuntatoriNonNulli = 0;
	msg_t* messaggio_prodotto_1=NULL;
	msg_t* messaggio_prodotto_2=NULL;
	msg_t* messaggio_prodotto_3=NULL;
	msg_t* messaggio_prodotto_4=NULL;
	msg_t* messaggio_prodotto_5=NULL;

	pthread_create(&produttore_1, NULL, &put_non_bloccante_thread, messaggio_da_inserire_1);
	pthread_create(&produttore_2, NULL, &put_non_bloccante_thread, messaggio_da_inserire_2);
	pthread_create(&produttore_3, NULL, &put_non_bloccante_thread, messaggio_da_inserire_3);
	pthread_create(&produttore_4, NULL, &put_non_bloccante_thread, messaggio_da_inserire_4);
	pthread_create(&produttore_5, NULL, &put_non_bloccante_thread, messaggio_da_inserire_5);
	pthread_join(produttore_1,&messaggio_prodotto_1);
	pthread_join(produttore_2,&messaggio_prodotto_2);
	pthread_join(produttore_3,&messaggio_prodotto_3);
	pthread_join(produttore_4,&messaggio_prodotto_4);
	pthread_join(produttore_5,&messaggio_prodotto_5);

	if(messaggio_prodotto_1==NULL){numeroPuntatoriNulli++;}else{numeroPuntatoriNonNulli++;}
	if(messaggio_prodotto_2==NULL){numeroPuntatoriNulli++;}else{numeroPuntatoriNonNulli++;}
	if(messaggio_prodotto_3==NULL){numeroPuntatoriNulli++;}else{numeroPuntatoriNonNulli++;}
	if(messaggio_prodotto_4==NULL){numeroPuntatoriNulli++;}else{numeroPuntatoriNonNulli++;}
	if(messaggio_prodotto_5==NULL){numeroPuntatoriNulli++;}else{numeroPuntatoriNonNulli++;}

	CU_ASSERT_EQUAL(numeroPuntatoriNulli,2);
	CU_ASSERT_EQUAL(numeroPuntatoriNonNulli,3);

	msg_destroy_string(messaggio_da_inserire_1);
	msg_destroy_string(messaggio_da_inserire_2);
	msg_destroy_string(messaggio_da_inserire_3);
	msg_destroy_string(messaggio_da_inserire_4);
	msg_destroy_string(messaggio_da_inserire_5);

}


//(P>1; C>1; N=1) Consumazioni e produzioni bloccanti e concorrenti di molteplici messaggi in un buffer unitario.
void consumazione_produzione_bloccante_molteplici_messaggi_buffer_unitario(){

	pthread_t produttore_1,produttore_2;
	pthread_t consumatore_3,consumatore_4;
	msg_t* messaggio_da_inserire_1 = msg_init_string("prodotto_1");
	msg_t* messaggio_da_inserire_2 = msg_init_string("prodotto_2");
	msg_t* messaggio_consumato_1=NULL;
	msg_t* messaggio_consumato_2=NULL;

	pthread_create(&consumatore_3, NULL, &get_bloccante_thread, NULL);
	pthread_create(&produttore_1, NULL, &put_bloccante_thread, messaggio_da_inserire_1);
	pthread_create(&produttore_2, NULL, &put_bloccante_thread, messaggio_da_inserire_2);
	pthread_create(&consumatore_4, NULL, &get_bloccante_thread, NULL);

	pthread_join(consumatore_3,&messaggio_consumato_1);
	pthread_join(produttore_1,NULL);
	pthread_join(produttore_2,NULL);
	pthread_join(consumatore_4,&messaggio_consumato_2);

	CU_ASSERT_PTR_NOT_NULL(messaggio_consumato_1);
	CU_ASSERT_PTR_NOT_NULL(messaggio_consumato_2);

	CU_ASSERT_STRING_EQUAL(messaggio_da_inserire_1->content,messaggio_consumato_1->content);
	CU_ASSERT_STRING_EQUAL(messaggio_da_inserire_2->content,messaggio_consumato_2->content);

	msg_destroy_string(messaggio_consumato_1);
	msg_destroy_string(messaggio_consumato_2);
	msg_destroy_string(messaggio_da_inserire_1);
	msg_destroy_string(messaggio_da_inserire_2);
}




int init_buffer_multiplo(){ buffer = buffer_init(3); return 0;}
int clean_buffer_multiplo(){buffer_destroy(buffer);return 0;}

int init_buffer_unitario(){ buffer = buffer_init(1); return 0;}
int clean_buffer_unitario(){buffer_destroy(buffer);return 0;}


int main(){


	CU_pSuite pSuite = NULL;

	   /* initialize the CUnit test registry */
	   if (CUE_SUCCESS != CU_initialize_registry())
	      return CU_get_error();

	   /* add a suite to the registry */
	   pSuite = CU_add_suite("Suite_1", init_buffer_multiplo, clean_buffer_multiplo);
	   CU_add_test(pSuite, "(P=1; C=0; N=1), Produzione bloccante di un solo messaggio in un buffer vuoto", test_produzione_bloccante_unico_messaggio_in_buffer_vuoto);

	   pSuite = CU_add_suite("Suite_2", init_buffer_multiplo, clean_buffer_multiplo);
	   CU_add_test(pSuite, "(P=1; C=0; N=1), produzione non bloccante di un solo messaggio in un buffer vuoto", test_produzione_non_bloccante_unico_messaggio_in_buffer_vuoto);

	   pSuite = CU_add_suite("Suite_3", init_buffer_multiplo,clean_buffer_multiplo);
	   CU_add_test(pSuite, "(P=0; C=1; N=1) Consumazione bloccante di un solo messaggio da un buffer pieno", consumazione_bloccante_unico_messaggio_in_buffer_pieno);

	   pSuite = CU_add_suite("Suite_4", init_buffer_multiplo, clean_buffer_multiplo);
	   CU_add_test(pSuite, "(P=0; C=1; N=1) Consumazione non bloccante di un solo messaggio da un buffer pieno", consumazione_non_bloccante_unico_messaggio_in_buffer_pieno);

	   pSuite = CU_add_suite("Suite_5", init_buffer_multiplo, clean_buffer_multiplo);
	   CU_add_test(pSuite, "(P=1; C=0; N=1) Produzione non bloccante in un buffer pieno", produzione_bloccante_buffer_pieno);

	   pSuite = CU_add_suite("Suite_6", init_buffer_multiplo, clean_buffer_multiplo);
	   CU_add_test(pSuite, "(P=0; C=1; N=1) Consumazione non bloccante da un buffer vuoto",consumazione_non_bloccante_da_buffer_vuoto);

	   pSuite = CU_add_suite("Suite_7", init_buffer_unitario, clean_buffer_unitario);
	   CU_add_test(pSuite, "(P=1; C=1; N=1) Consumazione e produzione non bloccante concorrente di un messaggio da un buffer unitario; prima il consumatore",consumazione_produzione_non_bloccante_unico_messaggio_buffer_unitario_prima_consumatore);

	   pSuite = CU_add_suite("Suite_8", init_buffer_unitario, clean_buffer_unitario);
	   CU_add_test(pSuite, "(P>1; C=0; N=1) Produzione non bloccante e concorrente di molteplici messaggi in un buffer unitario vuoto",produzione_non_bloccante_molteplici_messaggi_buffer_unitario_vuoto);

	   pSuite = CU_add_suite("Suite_9", init_buffer_multiplo, clean_buffer_multiplo);
	   CU_add_test(pSuite, "(P>1; C=0; N>1) Produzione bloccante e concorrente di molteplici messaggi in un buffer vuoto; il buffer non si riempe",produzione_bloccante_molteplici_messaggi_buffer_vuoto_non_riempito);

	   pSuite = CU_add_suite("Suite_10", init_buffer_multiplo, clean_buffer_multiplo);
	   CU_add_test(pSuite, "(P>1; C=0; N>1) Produzione non bloccante e concorrente di molteplici messaggi in un buffer vuoto; il buffer si satura in corso",produzione_non_bloccante_molteplici_messaggi_buffer_vuoto_saturato_in_corso);

	   pSuite = CU_add_suite("Suite_11", init_buffer_unitario, clean_buffer_unitario);
	   CU_add_test(pSuite, "(P>1; C>1; N=1) Consumazioni e produzioni bloccanti e concorrenti di molteplici messaggi in un buffer unitario",consumazione_produzione_bloccante_molteplici_messaggi_buffer_unitario);

	   pSuite = CU_add_suite("suite12", init_buffer_unitario, clean_buffer_unitario);
	   CU_add_test(pSuite, "(P=1; C=1; N=1) Consumazione e produzione bloccante concorrente di un messaggio in un buffer unitario; prima il produttore",consumazione_produzione_bloccante_buffer_unitario_prima_produttore);

	   pSuite = CU_add_suite("suite_13", init_buffer_unitario, clean_buffer_unitario);
	   CU_add_test(pSuite, "(P=0; C>1; N=1) Consumazione concorrente di molteplici messaggi da un buffer unitario pieno",consumazione_non_bloccante_messaggi_multipli_buffer_unitario_pieno);

	   CU_basic_set_mode(CU_BRM_VERBOSE);
	   CU_basic_run_tests();
	   CU_cleanup_registry();
	   return CU_get_error();
}
