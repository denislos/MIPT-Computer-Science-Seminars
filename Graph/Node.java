
import java.util.*;


public class Node <T extends Comparable<T>> implements Comparable<Node<T>>
  {
    public Node()
      {
        children = new ArrayList<Node<T>>();
      }

    public Node(T data)
      {
        children = new ArrayList<Node<T>>();

        elem = data;
      }  

    public T getData()
      {
        return elem;
      }  

    public void setData(T data)
      {
        elem = data;
      }  
    
    public boolean isDataEqual(T data)
      {
        return elem.compareTo(data) == 0;
      }

    public int compareTo(Node<T> node)
      {
        return elem.compareTo(node.elem);
      }

    public void append(T data)
      {
        Node<T> newNode = new Node<T>(data);
        children.add(newNode);
      }  

    public Node<T> getChild(int n)
      {
        return children.get(n);
      }  



    public Node<T> depthFirstSearch(T data)
      {
        if ( elem.compareTo(data) == 0)
          return this;
        else
          {
           Node<T> result;

            for ( Node<T> node : children )
              if ( (result = node.depthFirstSearch(data)) != null )
                return result;  

            return null;  
          } 
      }


    public Node<T> breadthFirstSearch(T data)
      {
        LinkedList<Node<T>> queue = new LinkedList<Node<T>>();
        queue.add(this);       
        
        Node< T> node = null;

        while ((node = queue.poll()) != null)
          if ( node.isDataEqual(data))
              return node;
          else
            node.children.forEach(element -> queue.add(element));

        
        return null;
      }    




    private T elem;
    private ArrayList<Node<T>> children;
  } 