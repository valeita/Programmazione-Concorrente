import java.util.Spliterator;
import java.util.function.Consumer;

public class TaskHWJ4<T> implements Spliterator<T>{

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
