import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.Callable;
import java.util.concurrent.RecursiveTask;

public class TaskHWJ3 extends RecursiveTask<Integer>{

	
	private FakeProcessor processor;
	private Node node;
	
	

	public TaskHWJ3(Node node) {
		this.node = node;
		processor = new FakeProcessor(1500);
	}

	
	@Override
	protected Integer compute() {

		if(node==null) {return 0;}
		
		int sum = processor.onerousFunction(node.getValue());
		TaskHWJ3 taskDx = new TaskHWJ3(node.getDx());
		taskDx.fork();
		TaskHWJ3 taskSx = new TaskHWJ3(node.getSx());
		sum += taskSx.compute();
		sum += taskDx.join();
		
		
		return sum;

	}
}


