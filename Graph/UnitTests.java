

import org.junit.BeforeClass;

import org.junit.Test;
import static org.junit.Assert.*;


public class UnitTests
  {


    @BeforeClass
    public static void init()
      {
        GraphBuilder<String> graphBuilder = new GraphBuilder<String>("King");
        graphBuilder.append("James")
                    .append("Mary")
                    .append("John")
                    .append("Howard");

        graphBuilder.toChild(2);

        graphBuilder.append("Stewart")
                    .append("Kristen");

        graphBuilder.toChild(0);

        graphBuilder.append("Neron")
                    .append("Rebecca");                       

        graph = graphBuilder.build();
      }



    @Test
    public void DFSFoundTest()
      {
        final String DEFAULT_STRING = "Rebecca";

        assertNotNull(graph.depthFirstSearch(DEFAULT_STRING));
      }



    @Test
    public void DFSNotFoundTest()
      {
        final String DEFAULT_STRING = "Harry";

        assertNull(graph.depthFirstSearch(DEFAULT_STRING));
      }  



    @Test
    public void BFSFoundTest()
      {
        final String DEFAULT_STRING = "Kristen";

        assertNotNull(graph.breadthFirstSearch(DEFAULT_STRING)); 
      } 



    @Test
    public void BFSNotFoundTest()
      {
        final String DEFAULT_STRING = "ABCDE";

        assertNull(graph.depthFirstSearch(DEFAULT_STRING));
      }  


    private static Graph<String> graph;  
  }