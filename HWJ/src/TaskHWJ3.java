import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.Callable;
import java.util.concurrent.RecursiveTask;

public class TaskHWJ3 extends RecursiveTask<Integer>{

	
	private FakeProcessor processor;
	private int sum;
	private Node radix;
	
	

	public TaskHWJ3(Node radix) {
		this.radix = radix;
		this.sum = 0;
		processor = new FakeProcessor(1500);
	}


	@Override
	protected Integer compute() {
		
		final List<TaskHWJ3> tasks = new ArrayList<TaskHWJ3>();
		
		
		sum += radix.getValue();
				
		if(radix.getSx()!= null) {
			
			final TaskHWJ3 task = new TaskHWJ3(radix.getSx());
            task.fork();
            tasks.add(task);
		}
		if(radix.getDx()!= null) {
			
			final TaskHWJ3 task = new TaskHWJ3(radix.getDx());
            task.fork();
            tasks.add(task);
		}
		
		for (final TaskHWJ3 task : tasks)
	         this.sum += task.join();
	      
		return this.sum;
	}

}
