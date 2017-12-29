
public class NodeTree implements Node{

	private int id;
	private Node sx;
	private Node dx;
	

	public NodeTree(int id, Node sx, Node dx) {
		
		this.id = id;
		this.sx = sx;
		this.dx = dx;
	}

	@Override
	public Node getSx() {
		
		return sx;
	}

	@Override
	public Node getDx() {
		
		return dx;
	}

	@Override
	public int getValue() {
		
		return id;
	}

	
}
