Il seguente file descrive le caratteristiche principali del progetto, focalizzandosi sugli aspetti fondamentali:

Tema principale del progetto: trovare la soluzione al problema classico produttore-consumatore con un buffer che può contenere messaggi multipli, acceduto da più thread concordantemente.

Il progetto si compone di un totale di 5 file:

Un file header msg.h, che descrive struttura dati e funzioni riguardante un messaggio.
Un file msg.c, che contiene le implementazioni delle funzioni contenute nel file header msg.h.

Un file header buffer.h, che descrive struttura dati e funzioni riguardante il buffer.
Un file buffer.c, che contiene le implementazioni delle funzioni contenute nel file header buffer.h.

Un file test.c, che conterrà esclusivamente codice inerente a CUnit per eseguire i test ritenuti da me necessari per verificare il corretto funzionamento delle primitive e del codice.

Un file main.c, che conterrà esclusivamente una chiamata al metodo test() il cui scopo è testare tutto il mio codice. Appositamente ho preferito lasciare vuota la main del progetto e creare un file aggiuntivo test.c con un metodo generale da richiamare nella main, perché per quanto in questo contesto potevo testare il mio codice direttamente nella main, è "logicamente" errato farlo, in quanto nella main non deve esserci il codice di testing.

Per lo sviluppo della soluzione sono stati utilizzati i seguenti strumenti di sincronizzazione:

un semaforo binario (Mutex) uso_D che garantisce da parte di un thread produttore, la mutua esclusione sull'inserimento di un messaggio nel buffer rispetto ad altri produttori.

un semaforo binario (Mutex) uso_T che garantisce da parte di un thread consumatore, la mutua esclusione sul prelevamento di un messaggio nel buffer rispetto ad altri consumatori.

Un semaforo Piene, inizializzato a zero, poiché all'inizio del programma le celle del buffer sono tutte vuote. scopo di questo semaforo è sospendere in attesa passiva i produttori che vorrebbero produrre su un buffer pieno, e risvegliare un produttore nel momento in cui si è liberato un posto nel buffer.

Un semaforo Vuote, inizializzato alla grandezza del buffer, poiché all'inizio del programma le celle del buffer sono tutte vuote. scopo di questo semaforo è sospendere in attesa passiva i consumatori che vorrebbero consumare su un buffer vuoto, e risvegliare un consumatore nel momento in cui si è stato inserito un messaggio nel buffer.

La soluzione presenta due sleep(di 5 secondi ciascuna), che seppur violano l'assunzione di progresso finito, sono state utili per la verifica della sospesa passiva di un thread.

È utile spendere due parole per descrivere un problema che ho incontrato, e su come l'ho affrontato:
nella maggior parte dei test che ho svolto, non è stato fatto il controllo del numero dei messaggi presenti nel buffer, non verificando di fatto a volte in modo esplicito la capienza attuale del buffer.
Non è possibile inserire un contatore messaggi nel buffer, in quanto, anche se lo aggiornassi nella sezione critica delle put e delle get(bloccanti e non), provocherebbe interferenza tra produttori e consumatori che rispettivamente inseriscono e prelevano un messaggio, aggiornandolo concorrentemente.

La soluzione che ho adottato, è di omettere quindi il contatore e omettere determinate assert.
Ho valutato che da alcune Assert e da alcuni test, in modo implicito la natura del problema passa in secondo ruolo in quanto comunque, è verificato il corretto funzionamento.
Ho comunque utilizzato quando possibile nel relativo scenario, (buffer->D - buffer-T) per verificare il numero dei messaggi contenuti nel buffer.
Alternativamente, solo per l'aggiornamento di un contatore presente nella struct buffer_t, avrei potuto inserire nella rispettiva put/get, un mutex_lock uso_T/uso_D, per evitare che ci fosse interferenza sull'aggiornamento della variabile condivisa contatoreMessaggi. Questa è stata scartata a prescindere, poiché anche se leggermente, compromette il grado di parallelismo.

