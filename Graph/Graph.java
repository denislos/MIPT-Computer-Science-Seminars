
import java.util.*;

public class Graph <T extends Comparable<T>>
  {
    public Graph()
      {
        root = new Node<T>();
      }

    public Graph(Node<T> node)
      {
        root = node;
      }  

    public Node<T> getRoot()
      {
        return root;
      }  
    

    public Node<T> depthFirstSearch(T elem)
      {
        return root.depthFirstSearch(elem);
      }


    public Node<T> breadthFirstSearch(T elem)
      {
        return root.breadthFirstSearch(elem);
      }  

    


    private Node<T> root = null;
  }