import java.util.Spliterator;
import java.util.function.Consumer;


public class TreeCollectionHWJ4<T> implements Spliterator<T> {

	private Node node;

	
	public TreeCollectionHWJ4(Node node) {
		
		this.node = node;
	}

	
	
	@Override
	public boolean tryAdvance(Consumer<? super T> action) {
		
		if(node == null) return false;
		return true;
	}

	
	
	@Override
	public Spliterator<T> trySplit() {
		
		// E QUI COME SPLITTO?
		
		
	}

	@Override
	public long estimateSize() {
		
		return node.getValue();
	}

	
	
	@Override
	public int characteristics() {
		
		return CONCURRENT;
	}

	
	public splitTree() {
		
		
	}
	
	
}
