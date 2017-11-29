Il seguente file descrive le caratteristiche principali del progetto, focalizzandosi sugli aspetti fondamentali:

Tema principale del progetto: trovare la soluzione al problema classico produttore-consumatore con un buffer che può contenere messaggi multipli, acceduto da più thread concordantemente.

Il progetto si compone di un totale di 5 file:

Un file header msg.h, che descrive struttura dati e funzioni riguardante un messaggio.
Un file msg.c, che contiene le implementazioni delle funzioni contenute nel file header msg.h.

Un file header buffer.h, che descrive struttura dati e funzioni riguardante il buffer.
Un file buffer.c, che contiene le implementazioni delle funzioni contenute nel file header buffer.h.

Un file main, che conterrà esclusivamente codice inerente a CUnit per eseguire i test ritenuti da me necessari per verificare il corretto funzionamento delle primitive e del codice.

Per lo sviluppo della soluzione sono stati utilizzati i seguenti strumenti di sincronizzazione:

un semaforo binario (Mutex) uso_D che garantisce da parte di un thread produttore, la mutua esclusione sull'inserimento di un messaggio nel buffer rispetto ad altri produttori.

un semaforo binario (Mutex) uso_T che garantisce da parte di un thread consumatore, la mutua esclusione sul prelevamento di un messaggio nel buffer rispetto ad altri consumatori.

Un semaforo Piene, inizializzato a zero, poiché all'inizio del programma le celle del buffer sono tutte vuote. scopo di questo semaforo è sospendere in attesa passiva i produttori che vorrebbero produrre su un buffer pieno, e risvegliare un produttore nel momento in cui si è liberato un posto nel buffer.

Un semaforo Vuote, inizializzato alla grandezza del buffer, poiché all'inizio del programma le celle del buffer sono tutte vuote. scopo di questo semaforo è sospendere in attesa passiva i consumatori che vorrebbero consumare su un buffer vuoto, e risvegliare un consumatore nel momento in cui si è stato inserito un messaggio nel buffer.


