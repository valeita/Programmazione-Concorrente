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
void test1(){

	buffer_t* buffer_vuoto = buffer_init(2);
	msg_t* m_start = msg_init_string("zero");
	msg_t* m_end = put_bloccante(buffer_vuoto,m_start);
	CU_ASSERT_STRING_EQUAL(m_start->content,m_end->content);
	msg_destroy_string(m_start);
	buffer_destroy(buffer_vuoto);

}

//(P=1; C=0; N=1), produzione non bloccante di un solo messaggio in un buffer vuoto
void test2(){

	buffer_t* buffer_vuoto = buffer_init(2);
	msg_t* m_start = msg_init_string("zero");

	msg_t* m_end = put_non_bloccante(buffer_vuoto,m_start);
	CU_ASSERT_STRING_EQUAL(m_start->content,m_end->content);

	msg_destroy_string(m_start);
	buffer_destroy(buffer_vuoto);


}


//(P=0; C=1; N=1) Consumazione bloccante di un solo messaggio da un buffer pieno
void test3(){

	buffer_t* buffer_pieno = buffer_init(2);
	msg_t* m_start1 = msg_init_string("zero");
	msg_t* m_start2 = msg_init_string("uno");

	put_bloccante(buffer_pieno,m_start1);
	put_bloccante(buffer_pieno,m_start2);
	msg_t* m_consumed = get_bloccante(buffer_pieno);
	CU_ASSERT_STRING_EQUAL(m_consumed->content,m_start1->content);
	CU_ASSERT_STRING_NOT_EQUAL(m_consumed->content,m_start2->content);

	msg_destroy_string(m_start1);
	msg_destroy_string(m_start2);
	buffer_destroy(buffer_pieno);

}

//(P=0; C=1; N=1) Consumazione non bloccante di un solo messaggio da un buffer pieno
void test4(){

	buffer_t* buffer_pieno = buffer_init(2);
	msg_t* m_start1 = msg_init_string("zero");
	msg_t* m_start2 = msg_init_string("uno");

	put_non_bloccante(buffer_pieno,m_start1);
	put_non_bloccante(buffer_pieno,m_start2);
	msg_t* m_consumed = get_non_bloccante(buffer_pieno);
	CU_ASSERT_STRING_EQUAL(m_consumed->content,m_start1->content);
	CU_ASSERT_STRING_NOT_EQUAL(m_consumed->content,m_start2->content);

	msg_destroy_string(m_start1);
	msg_destroy_string(m_start2);
	buffer_destroy(buffer_pieno);

}



//(P=1; C=0; N=1) Produzione non bloccante in un buffer pieno
void test5(){

	buffer_t* buffer_pieno = buffer_init(2);
	msg_t* m_start1 = msg_init_string("zero");
	msg_t* m_start2 = msg_init_string("uno");
	msg_t* m_excessive = msg_init_string("due");

	put_non_bloccante(buffer_pieno,m_start1);
	put_non_bloccante(buffer_pieno,m_start2);
	msg_t* msg_error = put_non_bloccante(buffer_pieno,m_excessive);
	CU_ASSERT_PTR_NOT_NULL(m_start1);
	CU_ASSERT_PTR_NOT_NULL(m_start2);
	CU_ASSERT_PTR_NULL(msg_error);

	msg_destroy_string(m_start1);
	msg_destroy_string(m_start2);
	msg_destroy_string(m_excessive);
	buffer_destroy(buffer_pieno);

}

//(P=0; C=1; N=1) Consumazione non bloccante da un buffer vuoto
void test6(){

	buffer_t* buffer_vuoto = buffer_init(2);

	msg_t* msg_error = get_non_bloccante(buffer_vuoto);
	CU_ASSERT_PTR_NULL(msg_error);

	buffer_destroy(buffer_vuoto);

}


msg_t* get_bloccante_thread(void){

	return get_bloccante(buffer);

}

msg_t* get_non_bloccante_thread(void){

	return get_non_bloccante(buffer);

}

msg_t* put_bloccante_thread(msg_t* msg){

	return put_bloccante(buffer,msg);

}

msg_t* put_non_bloccante_thread(msg_t* msg){

	return put_non_bloccante(buffer,msg);
}



//(P=1; C=1; N=1) Consumazione e produzione bloccante concorrente di un messaggio da un buffer unitario; prima il consumatore
void test7(){
						//(il test è stato reaolizzato con un buffer vuoto)
	pthread_t c1, p1;
	buffer = buffer_init(1);
	msg_t* m1 = msg_init_string("zero");
	msg_t* r1 = NULL;

	CU_ASSERT_EQUAL(buffer->contatoreMessaggi,0);
	pthread_create(&c1, NULL, &get_bloccante_thread, NULL);
	pthread_create(&p1, NULL, &put_bloccante_thread, m1);
	pthread_join(c1,&r1);
	pthread_join(p1,NULL);
	CU_ASSERT_EQUAL(buffer->contatoreMessaggi,0);
	CU_ASSERT_STRING_EQUAL(m1->content,r1->content);

	msg_destroy_string(m1);
	buffer_destroy(buffer);
}







//(P=1; C=1; N=1) Consumazione e produzione non bloccante concorrente di un messaggio da un buffer unitario; prima il consumatore
void test8(){
					//(il test è stato reaolizzato con un buffer vuoto)

	pthread_t c1, p1;
	buffer = buffer_init(1);
	msg_t* m1 = msg_init_string("prova");
	msg_t* r1 = NULL;
	msg_t* r2 = NULL;

	CU_ASSERT_EQUAL(buffer->contatoreMessaggi,0);
	pthread_create(&c1, NULL, &get_non_bloccante_thread, NULL);
	pthread_create(&p1, NULL, &put_non_bloccante_thread, m1);
	pthread_join(c1,&r1);
	pthread_join(p1,&r2);
	CU_ASSERT_EQUAL(buffer->contatoreMessaggi,1);  		//ovviamente con una get_non_bloccante, ritorna senza aver prelevato.
	CU_ASSERT_STRING_EQUAL(m1->content,r2->content);		// il risultato sarà = 1, poiche il produttore non si bloccherà e inserirà nel buffer
	CU_ASSERT_PTR_NULL(r1);

	msg_destroy_string(m1);
	buffer_destroy(buffer);

}



//(P>1; C=0; N=1) Produzione bloccante e concorrente di molteplici messaggi in un buffer unitario vuoto
void test9(){
				//far intervenire altri produttori, farebbe si che essi siano sospesi in attesa passiva.

	pthread_t p1;
	buffer = buffer_init(1);
	msg_t* m = msg_init_string("prova");
	msg_t* r = NULL;

	CU_ASSERT_EQUAL(buffer->contatoreMessaggi,0);
	pthread_create(&p1, NULL, &put_bloccante_thread, m);
	pthread_join(p1,&r);
	CU_ASSERT_EQUAL(buffer->contatoreMessaggi,1);
	CU_ASSERT_STRING_EQUAL(m->content,r->content);

	msg_destroy_string(m);
	buffer_destroy(buffer);

}



//(P>1; C=0; N=1) Produzione non bloccante e concorrente di molteplici messaggi in un buffer unitario vuoto
void test10(){

	pthread_t p1,p2,p3;
	buffer = buffer_init(1);
	msg_t* m1 = msg_init_string("zero");
	msg_t* m2 = msg_init_string("uno");
	msg_t* m3 = msg_init_string("due");
	msg_t* r1=NULL;
	msg_t* r2=NULL;
	msg_t* r3=NULL;

	CU_ASSERT_EQUAL(buffer->contatoreMessaggi,0);
	pthread_create(&p1, NULL, &put_non_bloccante_thread, m1);
	pthread_create(&p2, NULL, &put_non_bloccante_thread, m2);
	pthread_create(&p3, NULL, &put_non_bloccante_thread, m3);
	pthread_join(p1,&r1);
	pthread_join(p2,&r2);
	pthread_join(p3,&r3);
	CU_ASSERT_PTR_NOT_NULL(r1);
	CU_ASSERT_PTR_NULL(r2);
	CU_ASSERT_PTR_NULL(r3);
	CU_ASSERT_EQUAL(buffer->contatoreMessaggi,1);

	msg_destroy_string(m1);
	msg_destroy_string(m2);
	msg_destroy_string(m3);
	buffer_destroy(buffer);

}

//(P>1; C=0; N>1) Produzione bloccante e concorrente di molteplici messaggi in un buffer vuoto; il buffer non si riempe
void test11(){

	pthread_t p1,p2,p3;
	buffer = buffer_init(4);
	msg_t* m1 = msg_init_string("zero");
	msg_t* m2 = msg_init_string("uno");
	msg_t* m3 = msg_init_string("due");
	msg_t* r1=NULL;
	msg_t* r2=NULL;
	msg_t* r3=NULL;

	CU_ASSERT_EQUAL(buffer->contatoreMessaggi,0);
	pthread_create(&p1, NULL, &put_bloccante_thread, m1);
	pthread_create(&p2, NULL, &put_bloccante_thread, m2);
	pthread_create(&p3, NULL, &put_bloccante_thread, m3);
	pthread_join(p1,&r1);
	pthread_join(p2,&r2);
	pthread_join(p3,&r3);
	CU_ASSERT_EQUAL(buffer->contatoreMessaggi,3);
	CU_ASSERT_NOT_EQUAL(buffer->contatoreMessaggi,buffer->size);
	CU_ASSERT_STRING_EQUAL(m1->content,r1->content);
	CU_ASSERT_STRING_EQUAL(m2->content,r2->content);
	CU_ASSERT_STRING_EQUAL(m3->content,r3->content);

	msg_destroy_string(m1);
	msg_destroy_string(m2);
	msg_destroy_string(m3);
	buffer_destroy(buffer);

}


//(P>1; C=0; N>1) Produzione non bloccante e concorrente di molteplici messaggi in un buffer vuoto; il buffer non si riempe
void test12(){

	pthread_t p1,p2,p3;
	buffer = buffer_init(4);
	msg_t* m1 = msg_init_string("zero");
	msg_t* m2 = msg_init_string("uno");
	msg_t* m3 = msg_init_string("due");
	msg_t* r1=NULL;
	msg_t* r2=NULL;
	msg_t* r3=NULL;

	CU_ASSERT_EQUAL(buffer->contatoreMessaggi,0);
	pthread_create(&p1, NULL, &put_non_bloccante_thread, m1);
	pthread_create(&p2, NULL, &put_non_bloccante_thread, m2);
	pthread_create(&p3, NULL, &put_non_bloccante_thread, m3);
	pthread_join(p1,&r1);
	pthread_join(p2,&r2);
	pthread_join(p3,&r3);
	CU_ASSERT_EQUAL(buffer->contatoreMessaggi,3);
	CU_ASSERT_NOT_EQUAL(buffer->contatoreMessaggi,buffer->size);
	CU_ASSERT_STRING_EQUAL(m1->content,r1->content);
	CU_ASSERT_STRING_EQUAL(m2->content,r2->content);
	CU_ASSERT_STRING_EQUAL(m3->content,r3->content);

	msg_destroy_string(m1);
	msg_destroy_string(m2);
	msg_destroy_string(m3);
	buffer_destroy(buffer);

}


//(P>1; C=0; N>1) Produzione bloccante e concorrente di molteplici messaggi in un buffer vuoto; il buffer si satura in corso
void test13(){

	pthread_t p1,p2,p3;
	buffer = buffer_init(3);
	msg_t* m1 = msg_init_string("zero");
	msg_t* m2 = msg_init_string("uno");
	msg_t* m3 = msg_init_string("due");
	msg_t* r1=NULL;
	msg_t* r2=NULL;
	msg_t* r3=NULL;

	CU_ASSERT_EQUAL(buffer->contatoreMessaggi,0);
	pthread_create(&p1, NULL, &put_bloccante_thread, m1);
	pthread_create(&p2, NULL, &put_bloccante_thread, m2);
	pthread_create(&p3, NULL, &put_bloccante_thread, m3);
	pthread_join(p1,&r1);
	pthread_join(p2,&r2);
	pthread_join(p3,&r3);
	CU_ASSERT_EQUAL(buffer->contatoreMessaggi,buffer->size);
	CU_ASSERT_STRING_EQUAL(m1->content,r1->content);
	CU_ASSERT_STRING_EQUAL(m2->content,r2->content);
	CU_ASSERT_STRING_EQUAL(m3->content,r3->content);

	msg_destroy_string(m1);
	msg_destroy_string(m2);
	msg_destroy_string(m3);
	buffer_destroy(buffer);

}


//(P>1; C=0; N>1) Produzione non bloccante e concorrente di molteplici messaggi in un buffer vuoto; il buffer si satura in corso
void test14(){

	pthread_t p1,p2,p3,p4,p5;
	buffer = buffer_init(3);
	msg_t* m1 = msg_init_string("zero");
	msg_t* m2 = msg_init_string("uno");
	msg_t* m3 = msg_init_string("due");
	msg_t* m4 = msg_init_string("tre");
	msg_t* m5 = msg_init_string("quattro");
	msg_t* r1=NULL;
	msg_t* r2=NULL;
	msg_t* r3=NULL;
	msg_t* r4=NULL;
	msg_t* r5=NULL;

	CU_ASSERT_EQUAL(buffer->contatoreMessaggi,0);
	pthread_create(&p1, NULL, &put_non_bloccante_thread, m1);
	pthread_create(&p2, NULL, &put_non_bloccante_thread, m2);
	pthread_create(&p3, NULL, &put_non_bloccante_thread, m3);
	pthread_create(&p4, NULL, &put_non_bloccante_thread, m4);
	pthread_create(&p5, NULL, &put_non_bloccante_thread, m5);
	pthread_join(p1,&r1);
	pthread_join(p2,&r2);
	pthread_join(p3,&r3);
	pthread_join(p4,&r4);
	pthread_join(p5,&r5);

	CU_ASSERT_EQUAL(buffer->contatoreMessaggi,buffer->size);
	CU_ASSERT_STRING_EQUAL(m1->content,r1->content);
	CU_ASSERT_STRING_EQUAL(m2->content,r2->content);
	CU_ASSERT_STRING_EQUAL(m3->content,r3->content);
	CU_ASSERT_PTR_NULL(r4);
	CU_ASSERT_PTR_NULL(r5);	//(p4 e p5 saranno ritornati senza aver inserito il messaggio nel buffer);


	msg_destroy_string(m1);
	msg_destroy_string(m2);
	msg_destroy_string(m3);
	msg_destroy_string(m4);
	msg_destroy_string(m5);
	buffer_destroy(buffer);

}

//(P>1; C>1; N=1) Consumazioni e produzioni bloccanti e concorrenti di molteplici messaggi in un buffer unitario.
void test15(){

	pthread_t p1,p2;
	pthread_t c3,c4;
	buffer = buffer_init(1);
	msg_t* m_p1 = msg_init_string("prodotto_1");
	msg_t* m_p2 = msg_init_string("prodotto_2");
	msg_t* r1=NULL;
	msg_t* r2=NULL;

	CU_ASSERT_EQUAL(buffer->contatoreMessaggi,0);
	pthread_create(&c3, NULL, &get_bloccante_thread, NULL);
	pthread_create(&p1, NULL, &put_bloccante_thread, m_p1);
	pthread_create(&p2, NULL, &put_bloccante_thread, m_p2);
	pthread_create(&c4, NULL, &get_bloccante_thread, NULL);


	pthread_join(c3,&r1);
	pthread_join(p1,NULL);
	pthread_join(p2,NULL);
	pthread_join(c4,&r2);

	CU_ASSERT_PTR_NOT_NULL(r1);
	CU_ASSERT_PTR_NOT_NULL(r2);
	CU_ASSERT_EQUAL(buffer->contatoreMessaggi,0);

	msg_destroy_string(m_p1);
	msg_destroy_string(m_p2);
	buffer_destroy(buffer);

}


//(P>1; C>1; N=1) Consumazioni e produzioni non bloccanti e concorrenti di molteplici messaggi in un buffer unitario
void test16(){

	pthread_t p1,p2;
	pthread_t c3,c4;
	buffer = buffer_init(1);
	msg_t* m_p1 = msg_init_string("prodotto_1");
	msg_t* m_p2 = msg_init_string("prodotto_2");
	msg_t* r1=NULL;
	msg_t* r2=NULL;
	msg_t* r3=NULL;
	msg_t* r4=NULL;

	CU_ASSERT_EQUAL(buffer->contatoreMessaggi,0);
	pthread_create(&c3, NULL, &get_non_bloccante_thread, NULL);	//ritornerà un errore perche buffer vuoto
	pthread_create(&p1, NULL, &put_non_bloccante_thread, m_p1);	//inserirà
	pthread_create(&p2, NULL, &put_non_bloccante_thread, m_p2);	//ritornerà un errore perche buffer pieno
	pthread_create(&c4, NULL, &get_non_bloccante_thread, NULL);	//preleverà


	pthread_join(c3,&r1);
	pthread_join(p1,&r3);
	pthread_join(p2,&r4);
	pthread_join(c4,&r2);

	CU_ASSERT_PTR_NULL(r1);
	CU_ASSERT_PTR_NULL(r4);
	CU_ASSERT_STRING_EQUAL(r3->content,r2->content);
	CU_ASSERT_EQUAL(buffer->contatoreMessaggi,0);

	msg_destroy_string(m_p1);
	msg_destroy_string(m_p2);
	buffer_destroy(buffer);

}



int init_suite1(){return 0;}
int clean_suite1(){return 0;}


int main(){


	CU_pSuite pSuite = NULL;


	if(CUE_SUCCESS != CU_initialize_registry())
	        return CU_get_error();





	  pSuite = CU_add_suite("Suite_1", init_suite1, clean_suite1);
	  if (NULL == pSuite) {
	      CU_cleanup_registry();
	      return CU_get_error();
	  }


	  if ((NULL == CU_add_test(pSuite, "(P=1; C=0; N=1), Produzione bloccante di un solo messaggio in un buffer vuoto", test1)) ||
	       (NULL == CU_add_test(pSuite, "(P=1; C=0; N=1), produzione non bloccante di un solo messaggio in un buffer vuoto", test2)))
	  {
	      CU_cleanup_registry();
	      return CU_get_error();
	  }




	  pSuite = CU_add_suite("Suite_2", init_suite1, clean_suite1);
	  if (NULL == pSuite) {
	  	   CU_cleanup_registry();
	  	   return CU_get_error();
	  }


	  if ((NULL == CU_add_test(pSuite, "(P=0; C=1; N=1) Consumazione bloccante di un solo messaggio da un buffer pieno", test3)) ||
	  	  	(NULL == CU_add_test(pSuite, "(P=0; C=1; N=1) Consumazione non bloccante di un solo messaggio da un buffer pieno", test4)))
	  {
	  	  	CU_cleanup_registry();
	  	  	return CU_get_error();
	  }




	  pSuite = CU_add_suite("Suite_3", init_suite1, clean_suite1);
	  if (NULL == pSuite) {
	  	   CU_cleanup_registry();
	  	   return CU_get_error();
	  }

	  if ((NULL == CU_add_test(pSuite, "(P=1; C=0; N=1) Produzione non bloccante in un buffer pieno", test5)) ||
	  		(NULL == CU_add_test(pSuite, "(P=0; C=1; N=1) Consumazione non bloccante da un buffer vuoto", test6)))
	  {

	  	     CU_cleanup_registry();
	  	     return CU_get_error();
	  }




	   pSuite = CU_add_suite("Suite_4", init_suite1, clean_suite1);
	   if (NULL == pSuite) {
		  	CU_cleanup_registry();
		  	return CU_get_error();
	   }


	   if ((NULL == CU_add_test(pSuite, "(P=1; C=1; N=1) Consumazione e produzione bloccante concorrente di un messaggio da un buffer unitario; prima il consumatore", test7)) ||
		  	(NULL == CU_add_test(pSuite, "(P=1; C=1; N=1) Consumazione e produzione non bloccante concorrente di un messaggio da un buffer unitario; prima il consumatore", test8)))
	   {
		  	CU_cleanup_registry();
		  	return CU_get_error();
	   }


	   pSuite = CU_add_suite("Suite_5", init_suite1, clean_suite1);
	   if (NULL == pSuite) {
		  	CU_cleanup_registry();
		  	return CU_get_error();
	   }


	   if ((NULL == CU_add_test(pSuite, "(P>1; C=0; N=1) Produzione bloccante e concorrente di molteplici messaggi in un buffer unitario vuoto", test9)) ||
		  	(NULL == CU_add_test(pSuite, "(P>1; C=0; N=1) Produzione non bloccante e concorrente di molteplici messaggi in un buffer unitario vuoto", test10)))
	   {
		  	CU_cleanup_registry();
		  	return CU_get_error();
	   }


	   pSuite = CU_add_suite("Suite_6", init_suite1, clean_suite1);
	   if (NULL == pSuite) {
		  	CU_cleanup_registry();
		  	return CU_get_error();
	   }


	   if ((NULL == CU_add_test(pSuite, "(P>1; C=0; N>1) Produzione bloccante e concorrente di molteplici messaggi in un buffer vuoto; il buffer non si riempe", test11)) ||
		  	(NULL == CU_add_test(pSuite, "(P>1; C=0; N>1) Produzione non bloccante e concorrente di molteplici messaggi in un buffer vuoto; il buffer non si riempe", test12)))
	   {
		  	CU_cleanup_registry();
		  	return CU_get_error();
	   }

	   pSuite = CU_add_suite("Suite_7", init_suite1, clean_suite1);
	   if (NULL == pSuite) {
		  	CU_cleanup_registry();
		  	return CU_get_error();
	   }


	   if ((NULL == CU_add_test(pSuite, "(P>1; C=0; N>1) Produzione bloccante e concorrente di molteplici messaggi in un buffer vuoto; il buffer si satura in corso", test13)) ||
		  	(NULL == CU_add_test(pSuite, "(P>1; C=0; N>1) Produzione non bloccante e concorrente di molteplici messaggi in un buffer vuoto; il buffer si satura in corso", test14)))
	   {
		  	CU_cleanup_registry();
		  	return CU_get_error();
	   }

	   pSuite = CU_add_suite("Suite_8", init_suite1, clean_suite1);
	   	   if (NULL == pSuite) {
	   		  	CU_cleanup_registry();
	   		  	return CU_get_error();
	   	   }


	   	   if ((NULL == CU_add_test(pSuite, "(P>1; C>1; N=1) Consumazioni e produzioni bloccanti e concorrenti di molteplici messaggi in un buffer unitario.", test15)) ||
	   		  	(NULL == CU_add_test(pSuite, "(P>1; C>1; N=1) Consumazioni e produzioni non bloccanti e concorrenti di molteplici messaggi in un buffer unitario.", test16)))
	   	   {
	   		  	CU_cleanup_registry();
	   		  	return CU_get_error();
	   	   }



	  CU_basic_set_mode(CU_BRM_VERBOSE);
	  CU_basic_run_tests();
	  CU_cleanup_registry();
	  return CU_get_error();



}
