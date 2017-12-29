import java.util.concurrent.ForkJoinPool;

public class BinaryTreeAdderProcessingHWJ3  implements BinaryTreeAdder{

	private int ncpu;
	
	
	public BinaryTreeAdderProcessingHWJ3(int ncpu) {
		this.ncpu = ncpu;
	}




	@Override
	public int computeOnerousSum(Node root) {
	
		int sum = 0;
		final ForkJoinPool pool = new ForkJoinPool(ncpu);
		
		sum += pool.invoke(new TaskHWJ3(root));
		
		pool.shutdown();
		return sum;
	}

}
