import java.util.Spliterator;
import java.util.Spliterators;
import java.util.function.Consumer;

import javax.swing.plaf.synth.SynthSpinnerUI;


public class TreeCollectionHWJ4<T> implements Spliterator<T> {

	private Node node;
	Boolean sx;
	Boolean dx;
	


	
	public TreeCollectionHWJ4(Node node) {
		
		this.node = node;
		this.sx = false;
		this.dx = false;
		

	}


	@Override
	public boolean tryAdvance(Consumer<? super T> action) {

		System.out.println("sono entrato in tryAdvance");
		if(node == null) { return false; }

		action.accept((T) node);
		node = node.getDx();

		
		return true;
	}


	@Override
	public Spliterator<T> trySplit() {
		
		System.out.println("sono entrato in trysplit");
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
		
		return 0;
	}


	@Override
	public int characteristics() {
		
		return CONCURRENT;
	}




}
