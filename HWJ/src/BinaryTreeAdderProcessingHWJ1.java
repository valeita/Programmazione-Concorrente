import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import java.util.Queue;
import java.util.concurrent.CancellationException;
import java.util.concurrent.CompletionService;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorCompletionService;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;



public class BinaryTreeAdderProcessingHWJ1 implements BinaryTreeAdder{

	
	private Queue<Node> buffer;
	private Lock lock;
	private int ncpu;
	
	
	
	
	public BinaryTreeAdderProcessingHWJ1(int ncpu){
		
		this.buffer = new LinkedList();
		this.lock = new ReentrantLock();
		this.ncpu = ncpu;
	}


	//esegue una somma onerosa sui valori dei nodi di un intero albero
	@Override
	public int computeOnerousSum(Node root) {
		
		buffer.add(root);	
		int sum = 0;
		ExecutorService pool = Executors.newFixedThreadPool(ncpu);
		CompletionService<Integer> service = new ExecutorCompletionService<Integer>(pool);
		List<Future<Integer>> futures = new ArrayList<Future<Integer>>(ncpu);
			  
		// sottomissione di tutti i task
		try { 
			
		     for (int i=0; i<ncpu; i++) {
		    	 
		       futures.add(service.submit(new TaskHWJ1(buffer,lock,1500)));
		     }
		     
		// raccolta dei risultati nell'ordine di completamento
		     for (int i = 0; i < ncpu; ++i) {
		    	 
		       try { 
		    	   		Integer value = service.take().get();
		       		if (value != null) {
		       			sum += value.intValue();
		       		}
		       	} 
		       	catch(ExecutionException e) {e.printStackTrace();}
		        catch(CancellationException e) {e.printStackTrace();}
		     	catch(InterruptedException e) { e.printStackTrace(); }
		        catch(Exception e) { e.printStackTrace(); }
		     }     
		}
		// prova a cancellare i rimanenti (eventualmente ancora in esecuzione)

		   finally {
		     for (Future<Integer> f : futures)
		       f.cancel(true);
		   }
		
		pool.shutdown();
		return sum;
	}

	
}
