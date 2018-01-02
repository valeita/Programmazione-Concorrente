import java.util.Spliterator;
import java.util.Spliterators;
import java.util.function.Consumer;

import javax.swing.plaf.synth.SynthSpinnerUI;


public class TreeCollectionHWJ4<T> implements Spliterator<T> {

	private Node node;
	private Boolean sx;
	private Boolean dx;


	
	public TreeCollectionHWJ4(Node node) {
		
		this.node = node;
		this.sx = false;
		this.dx = false;
	}


	@Override
	public boolean tryAdvance(Consumer<? super T> action) {

		if(node == null) { return false; }
		else {
		action.accept((T) node);
		
		return false;
		}
	}


	@Override
	public Spliterator<T> trySplit() {

		if(sx == false) {
			sx = true;
			return new TreeCollectionHWJ4(node.getSx());
		}
		if(dx == false) {
			dx = true;
			return new TreeCollectionHWJ4(node.getDx());
		}
		return null;
	}


	@Override
	public long estimateSize() {

		if(node!= null) { return Long.MAX_VALUE; }
		else { return 0; }
	}


	@Override
	public int characteristics() {
		
		return CONCURRENT;
	}
}
