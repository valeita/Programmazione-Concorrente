
public class SpeedUpWatch {

	
	private BinaryTreeAdder sumSerialBinaryTree;
	private BinaryTreeAdder sumConcurrentBinaryTree;
	
	
	

	public SpeedUpWatch(BinaryTreeAdder sumSerialBinaryTree, BinaryTreeAdder sumConcurrentBinaryTree) {
		
		this.sumSerialBinaryTree = sumSerialBinaryTree;
		this.sumConcurrentBinaryTree = sumConcurrentBinaryTree;
	}


	//alcune esecuzioni “a vuoto” di warm-up: in questo modo si `e sicuri che alcune ottimizzazioni
	//a tempo dinamico della JVM siano gi`a effettive all’atto delle misurazioni vere e proprie
	public void warmUpEsecutions(boolean serial_concurr, Node node){
		
		if(serial_concurr) {
			for(int i=0; i<5; i++) {
				sumSerialBinaryTree.computeOnerousSum(node);
			}
		}
		else {
			
			for(int i=0; i<5; i++) {
				sumConcurrentBinaryTree.computeOnerousSum(node);
			}
		}
	}
	
	
	//calcolo dello speed-up
	public float calculateSpeedUp(Node node) {
		
		float timeSerialExecution = 0, timeConcurrentExecution = 0;
		
		//esecuzioni di warm-up seriale
		warmUpEsecutions(true,node);
		
		
		//esecuzione seriale
	
		for(int i=0; i<5; i++) {
			
			long time_start_serial = System.currentTimeMillis();
			sumSerialBinaryTree.computeOnerousSum(node);
			long time_end_serial = System.currentTimeMillis();
			timeSerialExecution += time_end_serial-time_start_serial;
		}
		timeSerialExecution = timeSerialExecution/5;
		
		//esecuzioni di warm-up concorrente
		warmUpEsecutions(false,node);
		
		//esecuzioni concorrenti

		for(int i=0; i<5; i++) {
			
			long time_start_concurrent = System.currentTimeMillis();
			sumConcurrentBinaryTree.computeOnerousSum(node);
			long time_end_concurrent = System.currentTimeMillis();
			timeConcurrentExecution += time_end_concurrent-time_start_concurrent;
		}
		timeConcurrentExecution = timeConcurrentExecution/5;
		
		float speedUp = timeSerialExecution/timeConcurrentExecution;
		
		return speedUp;
		
	}
}
