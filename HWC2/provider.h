/*
 * provider.h
 *
 *  Created on: 06 dic 2017
 *      Author: valerio
 */
#include "msg.h"
#include "poison_pill.h"

#ifndef PROVIDER_H_
#define PROVIDER_H_

//invio della sequenza dimessaggi nel buffer + il messaggio POISON PILL
void invia_sequenza_messaggi(blocco_msg_t* blocco_messaggi);



#endif /* PROVIDER_H_ */
