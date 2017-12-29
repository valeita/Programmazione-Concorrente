
public class TreeGenerator{


	//genera un albero bilanciato
	public Node generateBalancedTree(int depth) {
		
		if(depth < 0) return null;
		return new NodeTree(depth, generateBalancedTree(depth-1), generateBalancedTree(depth-1));
	}
	
	//genera un albero sbilanciato									
	public Node generateUnbalancedTree(int depthSx, int depthDx) {
		
		if (depthSx<0) { return null;}
		return new NodeTree(depthSx, generateUnbalancedTree(depthSx-1, depthDx-1),generateUnbalancedTree(depthDx-1, depthSx-1));
	}

	//calcola il numero dei nodi di un albero
	public int calculateNumberNodes(Node node) {
		
		if(node==null) {return 0;}
		else {
			int numNode_1 = calculateNumberNodes(node.getSx());
			int numNode_2 = calculateNumberNodes(node.getDx());
			return numNode_1 + numNode_2 + 1;
		}
	}
	
	
	//calcola la profondità di un albero
	public int calculateDepth(Node node) {
		
		if (node == null) {return -1;}
			else { 
				int ps = calculateDepth(node.getSx());
				int pd = calculateDepth(node.getDx());
	      if (ps > pd)
		return 1 + ps;
	      else return 1 + pd;
	    }
	}
	
}
