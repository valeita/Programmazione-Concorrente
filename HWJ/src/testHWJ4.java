
public class testHWJ4 {

	
	
	
	public static void main(String[] args) {
		
		System.out.println("HWJ4 - PARALLEL STREAM\n");

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
