

public class GraphBuilder <T extends Comparable<T>>
  {


    public GraphBuilder(T elem)
      {
        parent = currentNode = root = new Node<T>(elem);
      }



    public Graph<T> build()
      {
        Graph<T> graph = new Graph<T>(root);

        return graph;
      }




    public GraphBuilder<T> append(T elem)
      {
        currentNode.append(elem);

        return this;
      }


    public GraphBuilder<T> toParent()
      {
        currentNode = parent;

        return this;
      }  


    public GraphBuilder<T> toChild(int number)
       {
         parent = currentNode;
         currentNode = currentNode.getChild(number);

         return this;
       }  




    
    private Node<T> root;
    private Node<T> currentNode;  
    private Node<T> parent;  
  }