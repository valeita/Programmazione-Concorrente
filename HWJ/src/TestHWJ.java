
public class TestHWJ {

	public static void main(String[] args) {
	
		TreeGenerator Balancedtree;
		TreeGenerator UnbalancedTree;
		SpeedUpWatch watcher;
		
		System.out.println("HWJ1 - BUFFER ILLIMITATO\n");
		Balancedtree = new TreeGenerator();
		watcher = new SpeedUpWatch(new BinaryTreeAdderProcessingHWJ1(1), new BinaryTreeAdderProcessingHWJ1(Runtime.getRuntime().availableProcessors()));

		System.out.println("speed-up albero bilanciato:\n");
		for(int i=7; i<13; i++) {
			
			Node radix = Balancedtree.generateBalancedTree(i);
			System.out.println("profondità " + i + " (" + Balancedtree.calculateNumberNodes(radix) + " nodi)   speed-up: "+ watcher.calculateSpeedUp(radix));
		}
	
		
		UnbalancedTree = new TreeGenerator();
		System.out.println("\nspeed-up albero sbilanciato:\n");
		
		for(int i=7; i<13; i++) {
			
			Node radix = UnbalancedTree.generateUnbalancedTree(i, i-2);
			System.out.println("profondità " + (i) + " (" + UnbalancedTree.calculateNumberNodes(radix) + " nodi)   speed-up: "+ watcher.calculateSpeedUp(radix));
		}
		
		System.out.println("\n\n\n");
		
		System.out.println("HWJ3 - FRAMEWORK FORK & JOIN\n");

		Balancedtree = new TreeGenerator();
		watcher = new SpeedUpWatch(new BinaryTreeAdderProcessingHWJ3(1), new BinaryTreeAdderProcessingHWJ3(Runtime.getRuntime().availableProcessors()));

		System.out.println("speed-up albero bilanciato:\n");
		for(int i=7; i<13; i++) {
			
			Node radix = Balancedtree.generateBalancedTree(i);
			System.out.println("profondità " + i + " (" + Balancedtree.calculateNumberNodes(radix) + " nodi)   speed-up: "+ watcher.calculateSpeedUp(radix));
		}
	
		
		UnbalancedTree = new TreeGenerator();
		System.out.println("\nspeed-up albero sbilanciato:\n");
		
		for(int i=7; i<13; i++) {
			
			Node radix = UnbalancedTree.generateUnbalancedTree(i, i-2);
			System.out.println("profondità " + (i) + " (" + UnbalancedTree.calculateNumberNodes(radix) + " nodi)   speed-up: "+ watcher.calculateSpeedUp(radix));
		}
		
		System.out.println("\n\n\n");
		
		System.out.println("HWJ4 - PARALLEL STREAM\n");

		Balancedtree = new TreeGenerator();	
		watcher = new SpeedUpWatch(new BinaryTreeAdderProcessingHWJ3(1), new BinaryTreeAdderProcessingHWJ4());

		System.out.println("speed-up albero bilanciato:\n");
		for(int i=7; i<13; i++) {
			
			Node radix = Balancedtree.generateBalancedTree(i);
			System.out.println("profondità " + i + " (" + Balancedtree.calculateNumberNodes(radix) + " nodi)   speed-up: "+ watcher.calculateSpeedUp(radix));
		}
	
		
		UnbalancedTree = new TreeGenerator();
		System.out.println("\nspeed-up albero sbilanciato:\n");
		
		for(int i=7; i<13; i++) {
			
			Node radix = UnbalancedTree.generateUnbalancedTree(i, i-2);
			System.out.println("profondità " + (i) + " (" + UnbalancedTree.calculateNumberNodes(radix) + " nodi)   speed-up: "+ watcher.calculateSpeedUp(radix));
		}

	}

}
