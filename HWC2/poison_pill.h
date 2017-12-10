/*
 * poison_pill.h
 *
 *  Created on: 05 dic 2017
 *      Author: valerio
 */
#include "msg.h"
#include <stdlib.h>

extern const msg_t POISON_PILL_MSG;
#define POISON_PILL ( (msg_t*)&POISON_PILL_MSG )

msg_t* msg_init_pill(void *);

msg_t* msg_copy_pill(msg_t *);

void msg_destroy_pill(msg_t *);


