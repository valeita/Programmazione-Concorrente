import static org.junit.jupiter.api.Assertions.assertEquals;

import org.junit.Before;
import org.junit.Test;

public class testHWJ4 {

	
	
	private TreeGenerator tree;
	BinaryTreeAdderProcessingHWJ4 treeConcurrentProcessing;
	
	
	@Before
	public void prepare_test_tree_generator() {
		
		tree = new TreeGenerator();		
		treeConcurrentProcessing = new BinaryTreeAdderProcessingHWJ4();
	}
	
	
	//test della somma seriale eseguite sui valori dei nodi di un albero utilizzando il framework fork&join
	@Test
	public void test_compute_onerous_sum_serial_parallel_stream() {
		
		Node radix = tree.generateBalancedTree(2);
		int sum = treeConcurrentProcessing.computeOnerousSum(radix);				//sollecitazione
		
		assertEquals(sum, 4);												//verifica
	}
	
	
	
	public static void main(String[] args) {
		
		TreeGenerator Balancedtree = new TreeGenerator();	
		Node radix = Balancedtree.generateBalancedTree(2);
		BinaryTreeAdder b = new BinaryTreeAdderProcessingHWJ4();

		int sum = b.computeOnerousSum(radix);
		System.out.println(sum);
		/*
		System.out.println("HWJ4 - PARALLEL STREAM\n");

		TreeGenerator Balancedtree = new TreeGenerator();		
		SpeedUpWatch watcher = new SpeedUpWatch(new BinaryTreeAdderProcessingHWJ3(1), new BinaryTreeAdderProcessingHWJ4());

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
		*/
	}

}
