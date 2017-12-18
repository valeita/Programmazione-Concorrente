#include "buffer.h"
#include "CUnit/Basic.h"

static buffer_t* buffer;



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



//(P=1; C=0; N=1), Produzione bloccante di un solo messaggio in un buffer vuoto
void test_produzione_bloccante_unico_messaggio_in_buffer_vuoto(){

	msg_t* messaggio_da_inserire = msg_init_string("zero");	//set-up

	msg_t* messaggio_inserito = put_bloccante(buffer,messaggio_da_inserire);		//sollecitazione

	CU_ASSERT_STRING_EQUAL(messaggio_da_inserire->content,messaggio_inserito->content);		//verifica
	CU_ASSERT_PTR_EQUAL(messaggio_inserito,messaggio_da_inserire);

	msg_destroy_string(messaggio_da_inserire);

}

//(P=1; C=0; N=1), produzione non bloccante di un solo messaggio in un buffer vuoto
void test_produzione_non_bloccante_unico_messaggio_in_buffer_vuoto(){

	msg_t* messaggio_da_inserire = msg_init_string("zero");		//set-up

	msg_t* messaggio_inserito = put_non_bloccante(buffer,messaggio_da_inserire);		//sollecitazione

	CU_ASSERT_STRING_EQUAL(messaggio_da_inserire->content,messaggio_inserito->content);		//verifica
	CU_ASSERT_PTR_EQUAL(messaggio_inserito,messaggio_da_inserire);

	msg_destroy_string(messaggio_da_inserire);


}


//(P=0; C=1; N=1) Consumazione bloccante di un solo messaggio da un buffer pieno
void consumazione_bloccante_unico_messaggio_in_buffer_pieno(){

	msg_t* messaggio_da_inserire_1 = msg_init_string("zero");								//set-up
	put_bloccante(buffer,messaggio_da_inserire_1);	//riempio il buffer

	msg_t* messaggio_consumato = get_bloccante(buffer);									//sollecitazione

	CU_ASSERT_STRING_EQUAL(messaggio_consumato->content,messaggio_da_inserire_1->content);
	CU_ASSERT_PTR_NOT_EQUAL(messaggio_consumato,messaggio_da_inserire_1);					//verifica

	msg_destroy_string(messaggio_da_inserire_1);
}

//(P=0; C=1; N=1) Consumazione non bloccante di un solo messaggio da un buffer pieno
void consumazione_non_bloccante_unico_messaggio_in_buffer_pieno(){

	msg_t* messaggio_da_inserire_1 = msg_init_string("zero");					//set-up
	put_non_bloccante(buffer,messaggio_da_inserire_1);	//riempio il buffer

	msg_t* messaggio_consumato = get_non_bloccante(buffer);					//sollecitazione

	CU_ASSERT_STRING_EQUAL(messaggio_consumato->content,messaggio_da_inserire_1->content);
	CU_ASSERT_PTR_NOT_EQUAL(messaggio_consumato,messaggio_da_inserire_1);		//verifica

	msg_destroy_string(messaggio_da_inserire_1);

}



//(P=1; C=0; N=1) Produzione non bloccante in un buffer pieno
void produzione_non_bloccante_buffer_pieno(){

	msg_t* messaggio_da_inserire_1 = msg_init_string("zero");
	msg_t* messaggio_in_eccesso = msg_init_string("tre");							//set-up
	put_non_bloccante(buffer,messaggio_da_inserire_1);	//riempio il buffer

	msg_t* messaggio_di_errore = put_non_bloccante(buffer,messaggio_in_eccesso);	//sollecitazione

	CU_ASSERT_PTR_NOT_NULL(messaggio_da_inserire_1);								//verifica
	CU_ASSERT_PTR_NULL(messaggio_di_errore);

	msg_destroy_string(messaggio_da_inserire_1);
	msg_destroy_string(messaggio_in_eccesso);

}












//(P=1; C=0; N=1) Produzione bloccante in un buffer pieno
void produzione_bloccante_buffer_pieno(){

	pthread_t produttore,consumatore;
	msg_t* messaggio_da_inserire_1 = msg_init_string("prodotto_1");							//set-up
	msg_t* messaggio_da_inserire_2 = msg_init_string("prodotto_2");
	msg_t* messaggio_prodotto = NULL;
	msg_t* messaggio_consumato = NULL;
	put_bloccante(buffer,messaggio_da_inserire_1);	//riempio il buffer

	pthread_create(&produttore, NULL, &put_bloccante_thread, messaggio_da_inserire_2);		//sollecitazione
	sleep(5);		//violazione dell'assunzione di progresso finito, fatto solo
					//esclusivamente e consapevolmente per il testing del codice

	CU_ASSERT_STRING_EQUAL(messaggio_da_inserire_1->content,buffer->coda[0].content);
	CU_ASSERT_STRING_NOT_EQUAL(messaggio_da_inserire_2->content,buffer->coda[0].content);		//verifica
	pthread_create(&consumatore, NULL, &get_bloccante_thread, NULL);
	pthread_join(produttore,&messaggio_prodotto);
	pthread_join(consumatore,&messaggio_consumato);
	CU_ASSERT_STRING_EQUAL(messaggio_consumato->content,messaggio_da_inserire_1->content);
	CU_ASSERT_STRING_NOT_EQUAL(messaggio_consumato->content,messaggio_da_inserire_2->content);
	CU_ASSERT_STRING_EQUAL(buffer->coda[0].content,messaggio_prodotto->content);
	CU_ASSERT_PTR_EQUAL(messaggio_da_inserire_2,messaggio_prodotto);

	msg_destroy_string(messaggio_da_inserire_1);
	msg_destroy_string(messaggio_da_inserire_2);
	msg_destroy_string(messaggio_consumato);

}




//(P=0; C=1; N=1) Consumazione non bloccante da un buffer vuoto
void consumazione_non_bloccante_da_buffer_vuoto(){
																//set-up già impostato in partenza
	msg_t* messaggio_di_errore = get_non_bloccante(buffer);		//sollecitazione
	CU_ASSERT_PTR_NULL(messaggio_di_errore);						//verifica

}



void consumazione_bloccante_da_buffer_vuoto(){

	pthread_t produttore,consumatore;
		msg_t* messaggio_da_inserire = msg_init_string("prodotto");								//set-up
		msg_t* messaggio_prodotto = NULL;
		msg_t* messaggio_consumato = NULL;

		pthread_create(&consumatore, NULL, &get_bloccante_thread, NULL);		//sollecitazione
		sleep(5);		//violazione dell'assunzione di progresso finito, fatto solo				//sollecitazione
						//esclusivamente e consapevolmente per il testing del codice

		CU_ASSERT_EQUAL(buffer->T,0);															//verifica
		pthread_create(&produttore, NULL, &put_bloccante_thread, messaggio_da_inserire);
		pthread_join(produttore,&messaggio_prodotto);
		pthread_join(consumatore,&messaggio_consumato);
		CU_ASSERT_STRING_EQUAL(messaggio_consumato->content,messaggio_da_inserire->content);
		CU_ASSERT_PTR_NOT_EQUAL(messaggio_prodotto,messaggio_consumato);

		msg_destroy_string(messaggio_da_inserire);
		msg_destroy_string(messaggio_consumato);
}



//(P=1; C=1; N=1) Consumazione e produzione non bloccante concorrente di un messaggio da un buffer unitario; prima il consumatore
void consumazione_produzione_non_bloccante_unico_messaggio_buffer_unitario_prima_consumatore(){

	pthread_t consumatore_1, produttore_1;
	msg_t* messaggio_da_inserire = msg_init_string("prova");										//set-up
	msg_t* messaggio_consumato = NULL;

	pthread_create(&consumatore_1, NULL, &get_non_bloccante_thread, NULL);
	pthread_join(consumatore_1,&messaggio_consumato);
	pthread_create(&produttore_1, NULL, &put_non_bloccante_thread, messaggio_da_inserire);		//sollecitazione
	pthread_join(produttore_1,NULL);

	CU_ASSERT_PTR_NULL(messaggio_consumato);														//verifica

	msg_destroy_string(messaggio_da_inserire);

}


//(P=1; C=1; N=1) Consumazione e produzione bloccante concorrente di un messaggio in un buffer unitario; prima il produttore
void consumazione_produzione_bloccante_buffer_unitario_prima_produttore(){

	pthread_t consumatore_1, produttore_1;
	msg_t* messaggio_da_inserire = msg_init_string("prova");								//set-up
	msg_t* messaggio_consumato = NULL;
	msg_t* messaggio_prodotto = NULL;

	pthread_create(&produttore_1, NULL, &put_bloccante_thread, messaggio_da_inserire);
	pthread_create(&consumatore_1, NULL, &get_bloccante_thread, NULL);					//sollecitazione
	pthread_join(produttore_1,&messaggio_prodotto);
	pthread_join(consumatore_1,&messaggio_consumato);

	CU_ASSERT_STRING_EQUAL(messaggio_consumato->content,messaggio_prodotto->content);		//verifica

	msg_destroy_string(messaggio_consumato);
	msg_destroy_string(messaggio_da_inserire);
}



//(P>1; C=0; N=1) Produzione non bloccante e concorrente di molteplici messaggi in un buffer unitario vuoto
void produzione_non_bloccante_molteplici_messaggi_buffer_unitario_vuoto(){

	pthread_t produttore_1,produttore_2,produttore_3;
	msg_t* messaggio_da_inserire_1 = msg_init_string("zero");
	msg_t* messaggio_da_inserire_2 = msg_init_string("uno");
	msg_t* messaggio_da_inserire_3 = msg_init_string("due");										//set-up
	msg_t* messaggio_prodotto_1=NULL;
	msg_t* messaggio_prodotto_2=NULL;
	msg_t* messaggio_prodotto_3=NULL;
	int numeroPuntatoriNulli = 0;
	int numeroPuntatoriNonNulli = 0;

	pthread_create(&produttore_1, NULL, &put_non_bloccante_thread, messaggio_da_inserire_1);
	pthread_create(&produttore_2, NULL, &put_non_bloccante_thread, messaggio_da_inserire_2);		//sollecitazione
	pthread_create(&produttore_3, NULL, &put_non_bloccante_thread, messaggio_da_inserire_3);
	pthread_join(produttore_1,&messaggio_prodotto_1);
	pthread_join(produttore_2,&messaggio_prodotto_2);
	pthread_join(produttore_3,&messaggio_prodotto_3);
	if(messaggio_prodotto_1==NULL){numeroPuntatoriNulli++;}else{numeroPuntatoriNonNulli++;}
	if(messaggio_prodotto_2==NULL){numeroPuntatoriNulli++;}else{numeroPuntatoriNonNulli++;}
	if(messaggio_prodotto_3==NULL){numeroPuntatoriNulli++;}else{numeroPuntatoriNonNulli++;}

	CU_ASSERT_EQUAL(numeroPuntatoriNulli,2);														//verifica
	CU_ASSERT_EQUAL(numeroPuntatoriNonNulli,1);

	msg_destroy_string(messaggio_da_inserire_1);
	msg_destroy_string(messaggio_da_inserire_2);
	msg_destroy_string(messaggio_da_inserire_3);

}



//(P=0; C>1; N=1) Consumazione concorrente non bloccante di molteplici messaggi da un buffer unitario pieno
void consumazione_non_bloccante_messaggi_multipli_buffer_unitario_pieno(){

	pthread_t produttore;
	pthread_t consumatore_1,consumatore_2,consumatore_3;
	msg_t* messaggio_da_inserire = msg_init_string("zero");
	msg_t* messaggio_prodotto=NULL;
	msg_t*messaggio_consumato_1;
	msg_t*messaggio_consumato_2;																				//set-up
	msg_t*messaggio_consumato_3;
	int numeroPuntatoriNulli = 0;
	int numeroPuntatoriNonNulli = 0;
	pthread_create(&produttore, NULL, &put_non_bloccante_thread, messaggio_da_inserire);//riempio il buffer
	pthread_join(produttore,&messaggio_prodotto);

	pthread_create(&consumatore_1, NULL, &get_non_bloccante_thread, NULL);
	pthread_create(&consumatore_2, NULL, &get_non_bloccante_thread, NULL);									//sollecitazione
	pthread_create(&consumatore_3, NULL, &get_non_bloccante_thread, NULL);
	pthread_join(consumatore_1,&messaggio_consumato_1);
	pthread_join(consumatore_2,&messaggio_consumato_2);
	pthread_join(consumatore_3,&messaggio_consumato_3);
	if(messaggio_consumato_1==NULL){numeroPuntatoriNulli++;}else{numeroPuntatoriNonNulli++;}
	if(messaggio_consumato_2==NULL){numeroPuntatoriNulli++;}else{numeroPuntatoriNonNulli++;}
	if(messaggio_consumato_3==NULL){numeroPuntatoriNulli++;}else{numeroPuntatoriNonNulli++;}

	CU_ASSERT_EQUAL(numeroPuntatoriNulli,2);																	//verifica
	CU_ASSERT_EQUAL(numeroPuntatoriNonNulli,1);

	msg_destroy_string(messaggio_da_inserire);

}






//(P>1; C=0; N>1) Produzione bloccante e concorrente di molteplici messaggi in un buffer vuoto; il buffer non si riempe
void produzione_bloccante_molteplici_messaggi_buffer_vuoto_non_riempito(){

	pthread_t produttore1,produttore2,produttore3;
	msg_t* messaggio_da_inserire_1 = msg_init_string("zero");
	msg_t* messaggio_da_inserire_2 = msg_init_string("uno");									//set-up
	msg_t* messaggio_prodotto_1=NULL;
	msg_t* messaggio_prodotto_2=NULL;

	pthread_create(&produttore1, NULL, &put_bloccante_thread, messaggio_da_inserire_1);
	pthread_create(&produttore2, NULL, &put_bloccante_thread, messaggio_da_inserire_2);		//sollecitazione
	pthread_join(produttore1,&messaggio_prodotto_1);
	pthread_join(produttore2,&messaggio_prodotto_2);

	CU_ASSERT_EQUAL(buffer->D - buffer->T,2);												//verifica
	CU_ASSERT_NOT_EQUAL(buffer->D - buffer->T,buffer->size);

	msg_destroy_string(messaggio_da_inserire_1);
	msg_destroy_string(messaggio_da_inserire_2);

}



//(P>1; C=0; N>1) Produzione non bloccante e concorrente di molteplici messaggi in un buffer vuoto; il buffer si satura in corso
void produzione_non_bloccante_molteplici_messaggi_buffer_vuoto_saturato_in_corso(){

	pthread_t produttore_1,produttore_2,produttore_3,produttore_4;
	msg_t* messaggio_da_inserire_1 = msg_init_string("zero");
	msg_t* messaggio_da_inserire_2 = msg_init_string("uno");
	msg_t* messaggio_da_inserire_3 = msg_init_string("due");
	msg_t* messaggio_da_inserire_4 = msg_init_string("tre");
	int numeroPuntatoriNulli = 0;																//set-up
	int numeroPuntatoriNonNulli = 0;
	msg_t* messaggio_prodotto_1=NULL;
	msg_t* messaggio_prodotto_2=NULL;
	msg_t* messaggio_prodotto_3=NULL;
	msg_t* messaggio_prodotto_4=NULL;

	pthread_create(&produttore_1, NULL, &put_non_bloccante_thread, messaggio_da_inserire_1);
	pthread_create(&produttore_2, NULL, &put_non_bloccante_thread, messaggio_da_inserire_2);	   //sollecitazione
	pthread_create(&produttore_3, NULL, &put_non_bloccante_thread, messaggio_da_inserire_3);
	pthread_create(&produttore_4, NULL, &put_non_bloccante_thread, messaggio_da_inserire_4);
	pthread_join(produttore_1,&messaggio_prodotto_1);
	pthread_join(produttore_2,&messaggio_prodotto_2);
	pthread_join(produttore_3,&messaggio_prodotto_3);
	pthread_join(produttore_4,&messaggio_prodotto_4);
	if(messaggio_prodotto_1==NULL){numeroPuntatoriNulli++;}else{numeroPuntatoriNonNulli++;}
	if(messaggio_prodotto_2==NULL){numeroPuntatoriNulli++;}else{numeroPuntatoriNonNulli++;}
	if(messaggio_prodotto_3==NULL){numeroPuntatoriNulli++;}else{numeroPuntatoriNonNulli++;}
	if(messaggio_prodotto_4==NULL){numeroPuntatoriNulli++;}else{numeroPuntatoriNonNulli++;}

	CU_ASSERT_EQUAL(numeroPuntatoriNulli,1);													  //verifica
	CU_ASSERT_EQUAL(numeroPuntatoriNonNulli,3);

	msg_destroy_string(messaggio_da_inserire_1);
	msg_destroy_string(messaggio_da_inserire_2);
	msg_destroy_string(messaggio_da_inserire_3);
	msg_destroy_string(messaggio_da_inserire_4);
}




//(P>1; C>1; N=1) Consumazioni e produzioni bloccanti e concorrenti di molteplici messaggi in un buffer unitario.
void consumazione_produzione_bloccante_molteplici_messaggi_buffer_unitario(){

	pthread_t produttore_1,produttore_2,produttore_3;
	pthread_t consumatore_1,consumatore_2,consumatore_3;
	msg_t* messaggio_da_inserire_1 = msg_init_string("prodotto_1");
	msg_t* messaggio_da_inserire_2 = msg_init_string("prodotto_2");
	msg_t* messaggio_da_inserire_3 = msg_init_string("prodotto_3");								//set-up
	msg_t* messaggio_consumato_1=NULL;
	msg_t* messaggio_consumato_2=NULL;
	msg_t* messaggio_consumato_3=NULL;

	pthread_create(&consumatore_1, NULL, &get_bloccante_thread, NULL);
	pthread_create(&consumatore_2, NULL, &get_bloccante_thread, NULL);
	pthread_create(&consumatore_3, NULL, &get_bloccante_thread, NULL);
	pthread_create(&produttore_1, NULL, &put_bloccante_thread, messaggio_da_inserire_1);
	pthread_create(&produttore_2, NULL, &put_bloccante_thread, messaggio_da_inserire_2);
	pthread_create(&produttore_2, NULL, &put_bloccante_thread, messaggio_da_inserire_3);			//sollecitazione
	pthread_join(consumatore_1,&messaggio_consumato_1);
	pthread_join(consumatore_2,&messaggio_consumato_2);
	pthread_join(consumatore_3,&messaggio_consumato_3);
	pthread_join(produttore_1,NULL);
	pthread_join(produttore_2,NULL);
	pthread_join(produttore_3,NULL);

	CU_ASSERT_PTR_NOT_NULL(messaggio_consumato_1);												//verifica
	CU_ASSERT_PTR_NOT_NULL(messaggio_consumato_2);
	CU_ASSERT_PTR_NOT_NULL(messaggio_consumato_3);

	msg_destroy_string(messaggio_consumato_1);
	msg_destroy_string(messaggio_consumato_2);
	msg_destroy_string(messaggio_consumato_3);
	msg_destroy_string(messaggio_da_inserire_1);
	msg_destroy_string(messaggio_da_inserire_2);
	msg_destroy_string(messaggio_da_inserire_3);

}












int init_buffer_multiplo(){ buffer = buffer_init(3); return 0;}
int clean_buffer_multiplo(){buffer_destroy(buffer);return 0;}

int init_buffer_unitario(){ buffer = buffer_init(1); return 0;}
int clean_buffer_unitario(){buffer_destroy(buffer);return 0;}


int test(){

	CU_pSuite pSuite = NULL;

	   if (CUE_SUCCESS != CU_initialize_registry())
	      return CU_get_error();

	   pSuite = CU_add_suite("Suite_1", init_buffer_multiplo, clean_buffer_multiplo);
	   CU_add_test(pSuite, "(P=1; C=0; N=1), Produzione bloccante di un solo messaggio in un buffer vuoto", test_produzione_bloccante_unico_messaggio_in_buffer_vuoto);

	   pSuite = CU_add_suite("Suite_2", init_buffer_multiplo, clean_buffer_multiplo);
	   CU_add_test(pSuite, "(P=1; C=0; N=1), produzione non bloccante di un solo messaggio in un buffer vuoto", test_produzione_non_bloccante_unico_messaggio_in_buffer_vuoto);

	   pSuite = CU_add_suite("Suite_3", init_buffer_unitario,clean_buffer_unitario);
	   CU_add_test(pSuite, "(P=0; C=1; N=1) Consumazione bloccante di un solo messaggio da un buffer pieno", consumazione_bloccante_unico_messaggio_in_buffer_pieno);

	   pSuite = CU_add_suite("Suite_4", init_buffer_unitario, clean_buffer_unitario);
	   CU_add_test(pSuite, "(P=0; C=1; N=1) Consumazione non bloccante di un solo messaggio da un buffer pieno", consumazione_non_bloccante_unico_messaggio_in_buffer_pieno);

	   pSuite = CU_add_suite("Suite_5", init_buffer_unitario, clean_buffer_unitario);
	   CU_add_test(pSuite, "(P=1; C=0; N=1) Produzione non bloccante in un buffer pieno", produzione_non_bloccante_buffer_pieno);

	   pSuite = CU_add_suite("Suite_6", init_buffer_unitario, clean_buffer_unitario);
	   CU_add_test(pSuite, "(P=1; C=0; N=1) Produzione bloccante in un buffer pieno", produzione_bloccante_buffer_pieno);

	   pSuite = CU_add_suite("Suite_7", init_buffer_unitario, clean_buffer_unitario);
	   CU_add_test(pSuite, "(P=0; C=1; N=1) Consumazione non bloccante da un buffer vuoto",consumazione_non_bloccante_da_buffer_vuoto);

	   pSuite = CU_add_suite("Suite_8", init_buffer_multiplo, clean_buffer_multiplo);
	   CU_add_test(pSuite, "(P=0; C=1; N=1) Consumazione bloccante da un buffer vuoto",consumazione_bloccante_da_buffer_vuoto);

	   pSuite = CU_add_suite("Suite_9", init_buffer_unitario, clean_buffer_unitario);
	   CU_add_test(pSuite, "(P=1; C=1; N=1) Consumazione e produzione non bloccante concorrente di un messaggio da un buffer unitario; prima il consumatore",consumazione_produzione_non_bloccante_unico_messaggio_buffer_unitario_prima_consumatore);

	   pSuite = CU_add_suite("Suite_10", init_buffer_unitario, clean_buffer_unitario);
	   CU_add_test(pSuite, "(P>1; C=0; N=1) Produzione non bloccante e concorrente di molteplici messaggi in un buffer unitario vuoto",produzione_non_bloccante_molteplici_messaggi_buffer_unitario_vuoto);

	   pSuite = CU_add_suite("Suite_11", init_buffer_multiplo, clean_buffer_multiplo);
	   CU_add_test(pSuite, "(P>1; C=0; N>1) Produzione bloccante e concorrente di molteplici messaggi in un buffer vuoto; il buffer non si riempe",produzione_bloccante_molteplici_messaggi_buffer_vuoto_non_riempito);

	   pSuite = CU_add_suite("Suite_12", init_buffer_multiplo, clean_buffer_multiplo);
	   CU_add_test(pSuite, "(P>1; C=0; N>1) Produzione non bloccante e concorrente di molteplici messaggi in un buffer vuoto; il buffer si satura in corso",produzione_non_bloccante_molteplici_messaggi_buffer_vuoto_saturato_in_corso);

	   pSuite = CU_add_suite("Suite_13", init_buffer_unitario, clean_buffer_unitario);
	   CU_add_test(pSuite, "(P>1; C>1; N=1) Consumazioni e produzioni bloccanti e concorrenti di molteplici messaggi in un buffer unitario",consumazione_produzione_bloccante_molteplici_messaggi_buffer_unitario);

	   pSuite = CU_add_suite("suite14", init_buffer_unitario, clean_buffer_unitario);
	   CU_add_test(pSuite, "(P=1; C=1; N=1) Consumazione e produzione bloccante concorrente di un messaggio in un buffer unitario; prima il produttore",consumazione_produzione_bloccante_buffer_unitario_prima_produttore);

	   pSuite = CU_add_suite("suite_15", init_buffer_unitario, clean_buffer_unitario);
	   CU_add_test(pSuite, "//(P>1; C>1; N=1) Consumazioni e produzioni bloccanti e concorrenti di molteplici messaggi in un buffer unitario.",consumazione_produzione_bloccante_molteplici_messaggi_buffer_unitario);




	   CU_basic_set_mode(CU_BRM_VERBOSE);
	   CU_basic_run_tests();
	   CU_cleanup_registry();
	   return CU_get_error();
}


