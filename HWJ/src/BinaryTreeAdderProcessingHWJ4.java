import java.util.Collection;
import java.util.Spliterator;
import java.util.stream.Stream;
import java.util.stream.StreamSupport;

public class BinaryTreeAdderProcessingHWJ4 implements BinaryTreeAdder{



	@Override
	public int computeOnerousSum(Node root) {
		
		Spliterator<Node> spliterator = new TreeCollectionHWJ4<Node>(root);
		Stream<Node> stream = StreamSupport.stream(spliterator, true);
		
		
		
		
		return 0;
	}

}
