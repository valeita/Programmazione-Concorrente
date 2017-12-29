import static org.junit.jupiter.api.Assertions.assertEquals;

import org.junit.Before;
import org.junit.Test;

public class TestHWJ3 {

	
	private TreeGenerator tree;
	BinaryTreeAdderProcessingHWJ3 treeSerialProcessing;
	BinaryTreeAdderProcessingHWJ3 treeConcurrentProcessing;
	
	
	@Before
	public void prepare_test_tree_generator() {
		
		tree = new TreeGenerator();		
		treeSerialProcessing = new BinaryTreeAdderProcessingHWJ3(1);
		treeConcurrentProcessing = new BinaryTreeAdderProcessingHWJ3(Runtime.getRuntime().availableProcessors());
	}
	
	//test della somma seriale eseguite sui valori dei nodi di un albero utilizzando il framework fork&join
	@Test
	public void test_compute_onerous_sum_serial_fork_join() {
		
		Node radix = tree.generateBalancedTree(2);
		int sum = treeSerialProcessing.computeOnerousSum(radix);				//sollecitazione
		
		assertEquals(sum, 4);												//verifica
	}
	
	
	//test della somma concorrente eseguite sui valori dei nodi di un albero utilizzando il framework fork&join
	@Test
	public void test_compute_onerous_sum_concurrent_fork_join(){
		
		Node radix = tree.generateBalancedTree(2);
		int sum = treeConcurrentProcessing.computeOnerousSum(radix);			//sollecitazione
		
		assertEquals(sum, 4);												//verifica
	}
	
	
	public static void main(String[] args) {
		
		System.out.println("HWJ3 - FRAMEWORK FORK & JOIN\n");

		TreeGenerator Balancedtree = new TreeGenerator();
		SpeedUpWatch watcher = new SpeedUpWatch(new BinaryTreeAdderProcessingHWJ3(1), new BinaryTreeAdderProcessingHWJ3(Runtime.getRuntime().availableProcessors()));

		System.out.println("speed-up albero bilanciato:\n");
		for(int i=7; i<13; i++) {
			
			Node radix = Balancedtree.generateBalancedTree(i);
			System.out.println("profondità " + i + " (" + Balancedtree.calculateNumberNodes(radix) + " nodi)   speed-up: "+ watcher.calculateSpeedUp(radix));
		}
	
		
		TreeGenerator UnbalancedTree = new TreeGenerator();
		System.out.println("\nspeed-up albero sbilanciato:\n");
		
		for(int i=7; i<13; i++) {
			
			Node radix = UnbalancedTree.generateUnbalancedTree(i, i-2);
			System.out.println("profondità " + (i) + " (" + UnbalancedTree.calculateNumberNodes(radix) + " nodi)   speed-up: "+ watcher.calculateSpeedUp(radix));
		}
	}

}
