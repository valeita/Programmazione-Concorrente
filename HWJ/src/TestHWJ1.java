import static org.junit.jupiter.api.Assertions.assertEquals;
import org.junit.Before;
import org.junit.Test;


public class TestHWJ1 {

	private TreeGenerator tree;
	private BinaryTreeAdderProcessingHWJ1 treeSerialProcessing;
	private BinaryTreeAdderProcessingHWJ1 treeConcurrentProcessing;
	
	
	
	@Before
	public void prepare_test_tree_generator() {
		
		tree = new TreeGenerator();											//set-up
		treeSerialProcessing = new BinaryTreeAdderProcessingHWJ1(1);
		treeConcurrentProcessing = new BinaryTreeAdderProcessingHWJ1(Runtime.getRuntime().availableProcessors());
	}
	
	
	
	//calcolo del numero di elementi di un albero bilanciato
	@Test
	public void test_tree_balanced_generator_number_elements() {
				
		Node radix = tree.generateBalancedTree(2);					//sollecitazione
		tree.calculateNumberNodes(radix);
		
		assertEquals(tree.calculateNumberNodes(radix),7);				//verifica
	}
	
	
	
	//calcolo della profondità di un albero bilanciato
	@Test
	public void test_tree_balanced_generator_depth() {
			
		Node radix = tree.generateBalancedTree(2);		//sollecitazione
		int depth = tree.calculateDepth(radix);			
		
		assertEquals(depth,2);							//verifica
		
	}
	
	
	
	//calcolo del numero di elementi di un albero sbilanciato
	@Test
	public void test_tree_unbalanced_generator_number_elements() {
				
		Node radix = tree.generateUnbalancedTree(2,1);					//sollecitazione											
		tree.calculateNumberNodes(radix);
		
		assertEquals(tree.calculateNumberNodes(radix),5);					//verifica
	}
	
	
	
	//calcolo della profondità di un albero sbilanciato
	@Test
	public void test_tree_unbalanced_generator_depth() {
			
		Node radix = tree.generateUnbalancedTree(2,1);		//sollecitazione
		int depth = tree.calculateDepth(radix);				
		
		assertEquals(depth,2);								//verifica
	}
	
	
	
	//test del metodo computeOneroussum eseguito in modo non concorrente
	@Test
	public void test_compute_onerous_sum_serial() {
		
		Node radix = tree.generateBalancedTree(2);					
		int sum = treeSerialProcessing.computeOnerousSum(radix);		//sollecitazione
		
		assertEquals(sum, 4);										//verifica
	}

	
	
	
	//test del metodo computeOneroussum eseguito in modo concorrente
		@Test
		public void test_compute_onerous_sum_concurrent() {
			
			Node radix = tree.generateBalancedTree(2);						
			int sum = treeConcurrentProcessing.computeOnerousSum(radix);		//sollecitazione
			
			assertEquals(sum, 4);											//verifica
		}
	
	
		
		
	public static void main(String[] args) {
		
		System.out.println("HWJ1 - BUFFER ILLIMITATO\n");
		TreeGenerator Balancedtree = new TreeGenerator();
		SpeedUpWatch watcher = new SpeedUpWatch(new BinaryTreeAdderProcessingHWJ1(1), new BinaryTreeAdderProcessingHWJ1(Runtime.getRuntime().availableProcessors()));

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
