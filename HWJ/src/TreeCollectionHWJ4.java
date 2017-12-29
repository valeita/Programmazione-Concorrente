import java.util.Spliterator;
import java.util.function.Consumer;


public class TreeCollectionHWJ4<T> implements Spliterator<T> {

	private Node node;

	public TreeCollectionHWJ4(Node node) {
		
		this.node = node;
	}

	@Override
	public boolean tryAdvance(Consumer<? super T> action) {
		// TODO Auto-generated method stub
		return false;
	}

	@Override
	public Spliterator<T> trySplit() {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public long estimateSize() {
		// TODO Auto-generated method stub
		return 0;
	}

	@Override
	public int characteristics() {
		// TODO Auto-generated method stub
		return 0;
	}

	
	
	
}
