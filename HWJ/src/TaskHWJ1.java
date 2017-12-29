import java.util.Queue;
import java.util.concurrent.Callable;
import java.util.concurrent.locks.Lock;

public class TaskHWJ1 implements Callable<Integer>{

	private Queue<Node> buffer;
	private Lock lockBuffer;
	private FakeProcessor processor;
	
	
	

	public TaskHWJ1(Queue buffer, Lock lock, int max) {
		
		this.buffer = buffer;
		this.lockBuffer = lock;
		processor = new FakeProcessor(max);	
	}



	//prelevamento dal buffer di un nodo e inserimento dei suoi figli fino a che il buffer non è vuoto
	@Override
	public Integer call(){
		
		int sum = 0;
		try {
			
			while(true) {
				
				lockBuffer.lock();
				if(buffer.size() == 0) {
					
					lockBuffer.unlock(); 
					break;
				}
				else { 
					
					Node node = buffer.poll();
					if(node.getSx() != null) { buffer.add(node.getSx()); }
					if(node.getDx() != null) { buffer.add(node.getDx()); }

					lockBuffer.unlock();	
					sum += processor.onerousFunction(node.getValue());	
					
				}
			}
			return new Integer(sum);
		}
		catch(Exception e) {e.printStackTrace();}

		return new Integer(sum);
	}
}
