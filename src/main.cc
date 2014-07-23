#include <types.hpp>
#include <Graph.hpp>
#include <BGL.hpp>
#include <Communicator.hpp>
#include <GraphCommunicator.hpp>
#include <MPI.hpp>
#include <NameService.hpp>

#include <iostream> /* cout */
#include <tuple>    /* pair */
#include <vector>   /* vector   */
#include <array>    /* array */
#include <math.h>   /* ceil */
#include <time.h>   /* time */

#define FALSE 0
#define TRUE  1

/*******************************************************************************
 *
 * CONFIGURATION
 *
 ******************************************************************************/
// Graph
// Property is struct with at least ID id and typename UUDI
typedef GraphPolicy::NoProperty NoProperty; 
typedef GraphPolicy::BGL<NoProperty, NoProperty> BGL;
typedef Graph<BGL>                                     BGLGraph;
typedef typename BGLGraph::Vertex                      Vertex;
typedef typename BGLGraph::Edge                        Edge;
typedef typename BGLGraph::EdgeDescriptor              EdgeDescriptor;

// Communicator
// Vertex / Edge is a struct with at least ID id public member
typedef CommunicationPolicy::MPI                   Mpi;
typedef Communicator<Mpi>                          MpiCommunicator;
typedef typename MpiCommunicator::Context          Context;
typedef typename MpiCommunicator::BinaryOperations BinaryOperations;
typedef typename MpiCommunicator::Event            Event;
typedef typename MpiCommunicator::CommID           CommID;

typedef NameService<BGLGraph, MpiCommunicator>     NS;
typedef GraphCommunicator<BGLGraph, MpiCommunicator, NS> GC;

/*******************************************************************************
 *
 * GRAPH AUXILARY
 *
 *******************************************************************************/
std::vector<Vertex> generateVertices(const size_t numVertices){
    std::vector<Vertex> vertices;
    for(unsigned i = 0; i < numVertices; ++i){
	vertices.push_back(Vertex(i));
    }
    return vertices;
}

std::vector<EdgeDescriptor> generateFullyConnectedTopology(const unsigned verticesCount, std::vector<Vertex> &vertices){
    vertices = generateVertices(verticesCount);
    std::cout << "Create fully connected with " << vertices.size() << " cells" << std::endl;

    unsigned edgeCount = 0;    
    std::vector<EdgeDescriptor> edges;

    for(unsigned i = 0; i < vertices.size(); ++i){
	for(unsigned j = 0; j < vertices.size(); ++j){
	    if(vertices[i].id == vertices[j].id){
		continue;
	    } 
	    else {
		edges.push_back(std::make_tuple(vertices[i], vertices[j], Edge(edgeCount++)));
	    }
	}

    }

    return edges;
}

std::vector<EdgeDescriptor> generateStarTopology(const unsigned verticesCount, std::vector<Vertex> &vertices){
    vertices = generateVertices(verticesCount);
    
    unsigned edgeCount = 0;    
    std::vector<EdgeDescriptor> edges;

    for(unsigned i = 0; i < vertices.size(); ++i){
	if(i != 0){
	    edges.push_back(std::make_tuple(vertices[i], vertices[0], Edge(edgeCount++)));
	}
		
    }

    return edges;
}

unsigned hammingDistance(unsigned a, unsigned b){
    unsigned abXor = a xor b;
    return (unsigned) __builtin_popcount(abXor);
}


std::vector<EdgeDescriptor> generateHyperCubeTopology(const unsigned dimension, std::vector<Vertex> &vertices){
    assert(dimension >= 1);
    std::vector<EdgeDescriptor> edges;

    unsigned verticesCount = pow(2, dimension);
    unsigned edgeCount = 0;
    vertices  = generateVertices(verticesCount);

    for(Vertex v1 : vertices){
	for(Vertex v2 : vertices){
	    if(hammingDistance(v1.id, v2.id) == 1){
		edges.push_back(std::make_tuple(v1, v2, Edge(edgeCount++)));
	    }

	}
    }
    
    return edges;
}

// TODO
// Make n-dimensional
// Make connected borders
std::vector<EdgeDescriptor> generate2DMeshTopology(const unsigned height, const unsigned width, std::vector<Vertex> &vertices){
    const unsigned verticesCount = height * width;
    vertices = generateVertices(verticesCount);
    std::vector<EdgeDescriptor> edges;

    unsigned edgeCount = 0;

    for(Vertex v: vertices){
	unsigned i    = v.id;

	if(i >= width){
	    unsigned up   = i - width;
	    edges.push_back(std::make_tuple(vertices[i], vertices[up], Edge(edgeCount++)));
	}

	if(i < (verticesCount - width)){
	    unsigned down = i + width;
	    edges.push_back(std::make_tuple(vertices[i], vertices[down], Edge(edgeCount++)));
	}


	if((i % width) != (width - 1)){
	    int right = i + 1;
	    edges.push_back(std::make_tuple(vertices[i], vertices[right], Edge(edgeCount++)));
	}

	if((i % width) != 0){
	    int left = i - 1;
	    edges.push_back(std::make_tuple(vertices[i], vertices[left], Edge(edgeCount++)));
	}
	

    }

    return edges;
}

template <typename T_Graph>
void printVertexDistribution(const std::vector<Vertex>& vertices,const T_Graph& graph, CommID commID){
    std::cout << "[" <<  commID << "] " << "Graph: " << graph.id << " " << "Vertices: ";
    for(Vertex v : vertices){
	std::cout << v.id << " ";
    }
    std::cout << std::endl;

}



/*******************************************************************************
 *
 * COMMUNICATION AUXILARY
 *
 *******************************************************************************/

template<typename T_Communicator, typename T_Graph>
void nearestNeighborExchange(T_Communicator &communicator, T_Graph &graph, std::vector<typename T_Graph::Vertex> myVertices){
    typedef typename T_Graph::Vertex Vertex;
    typedef typename T_Graph::Edge   Edge;
    typedef std::array<unsigned, 1>  Buffer;

    // Async send vertices data
    for(Vertex myVertex : myVertices){
    	std::vector<std::pair<Vertex, Edge> > outEdges = graph.getOutEdges(myVertex);
    	Buffer outBuffer{{myVertex.id}};

    	//Send data to out edges
    	for(std::pair<Vertex, Edge> outEdge : outEdges){
    	    Vertex dest = outEdge.first;
    	    Edge   e    = outEdge.second;
    	    communicator.asyncSend(graph, dest, e, outBuffer);// <== BUGGY leads to segfault on some configuration
    	}

    }

    // Sync recv vertices data
    for(Vertex myVertex : myVertices){
    	std::vector<std::pair<Vertex, Edge> > inEdges  = graph.getInEdges(myVertex);
    	std::vector<Buffer>  inBuffers (inEdges.size(), Buffer{{0}});


    	// Recv data from in edges
    	for(unsigned i = 0 ; i < inBuffers.size(); ++i){
    	    Vertex src = inEdges[i].first;
    	    Edge   e   = inEdges[i].second;
	    communicator.recv(graph, src, e, inBuffers[i]); // <== BUGGY leads to segfault on some configuration
    	}
	
	unsigned recvSum = 0;
	for(Buffer b : inBuffers){
	    recvSum += b[0];
	}
	std::cout << "Vertex: " << myVertex.id << " NeighborIDSum: " << recvSum <<  std::endl;
	
    }

}

template<typename T_Communicator, typename T_Graph>
void reduceVertexIDs(T_Communicator &communicator, T_Graph &graph, std::vector<typename T_Graph::Vertex> myVertices){
    typedef typename T_Graph::Vertex Vertex;

    Vertex rootVertex = graph.getVertices().at(0);
    unsigned recvData = 0;
    std::vector<unsigned> sendData(1,0);

    for(Vertex vertex : myVertices){
    	sendData[0] = vertex.id;
    	communicator.reduce(rootVertex, vertex, graph, sendData, recvData);
    }
    
    for(Vertex vertex : myVertices){
    	if(vertex.id == rootVertex.id){
	    std::cout << "Reduce graph " << graph.id << ": " << recvData << std::endl;

    	}

    }
    
}

// Collective
template<typename T_Communicator>
unsigned randomNumber(T_Communicator &communicator, const Context &context){
    srand (time(NULL) + context.getCommID());
    unsigned ownRandom = rand();
    unsigned random = 0;

    if(context.valid()){
	std::vector<int> sendData(1, ownRandom);
	std::vector<int> recvData(context.size(), 0);

	communicator.allGather(context, sendData, recvData);

	for(unsigned i = 0; i < recvData.size(); ++i){
	    random += recvData[i];
	}
    }
    return random;
}

// Collective
template<typename T_Communicator, typename T_NameService, typename T_Graph>
typename T_Communicator::CommID randomHostCommID(T_Communicator &communicator, T_NameService &nameService, T_Graph &graph){

    std::vector<CommID> commIDs = nameService.getGraphHostCommIDs(graph);
    Context context = nameService.getGraphContext(graph);
    unsigned random = randomNumber(communicator, context);
    return commIDs.at(random % commIDs.size());
}

// template<typename T_Communicator, typename T_Graph>
// typename T_Graph::Vertex randomVertex( T_Communicator& communicator, T_Graph& graph, const std::vector<typename T_Graph::Vertex> vertices ){
//     typedef T_Graph                Graph;
//     typedef typename Graph::Vertex Vertex;
//     typedef typename Vertex::ID    VertexID;
    
//     VertexID vertexID  = 0;
//     unsigned myRandom = 0;
//     std::vector<unsigned> recvData;

//     for(Vertex vertex : vertices){
// 	srand(time(NULL) + vertex.id);
// 	myRandom = rand();
// 	communicator.allGather(vertex, graph, myRandom, recvData);

//     }

//     unsigned greatestRandom = 0;

//     for(VertexID i = 0; i < recvData.size(); ++i){
// 	if(recvData[i] > greatestRandom){
// 	    vertexID = i;
// 	    greatestRandom = recvData[i];
// 	}
//     }


//     return graph.getVertices().at(vertexID);  
// }


template<typename T_Communicator, typename T_Graph, typename T_NameService>
void occupyRandomVertex(T_Communicator& communicator, T_Graph& graph, T_NameService& nameService, std::vector<typename T_Graph::Vertex>& myVertices){
    typedef T_Graph Graph;
    typedef typename Graph::Vertex          Vertex;
    typedef typename Vertex::ID             VertexID;
    typedef typename T_Communicator::CommID CommID;

    CommID masterCommID = 3;//randomHostCommID(communicator, nameService, graph);
    Context context = nameService.getGraphContext(graph);

    if(nameService.getHostedVertices(graph, masterCommID).empty()){
	std::cout << "The masterCommID does not host any vertices of this graph" << std::endl;
	return;
    }

    if(context.valid()){
     	CommID commID    = context.getCommID();
     	std::array<VertexID, 1> randomVertex{{0}};

     	bool haveVertex = false;
     	if(commID == masterCommID){
	    srand (time(NULL));
     	    randomVertex[0] = 1;//rand() % graph.getVertices().size();

	    communicator.broadcast(masterCommID, context, randomVertex);

	    VertexID vertexID = randomVertex[0];
	    Vertex vertex     = graph.getVertices().at(vertexID);

     	    for(Vertex v : myVertices){
     	    	if(v.id == vertex.id){
     	    	    std::cout << "[" << masterCommID << "] " << "Allready have random vertex: " << vertex.id << std::endl;
     	    	    haveVertex = true;
     	    	}
     	    }
	
     	    if(!haveVertex){
     	    	std::cout << "[" << masterCommID << "] " << "Occupy vertex: " << vertex.id << std::endl;
     	    	myVertices.push_back(vertex);
     	    }

     	}
     	else {
	    communicator.broadcast(masterCommID, context, randomVertex);
     	    VertexID vertexID = randomVertex[0];
     	    Vertex vertex     = graph.getVertices().at(vertexID);

     	     for(auto vertexIter = myVertices.begin(); vertexIter != myVertices.end();){
     	     	if(vertexIter->id == vertex.id)
     	     	    vertexIter = myVertices.erase(vertexIter);
		else
		    ++vertexIter;

     	     }

     	}

    }

}



/*******************************************************************************
 *
 * VERTEX DISTRIBUTION
 *
 *******************************************************************************/
template<typename T_Graph>
std::vector<Vertex> distributeVerticesEvenly(const unsigned processID, const unsigned processCount, T_Graph &graph){
    typedef typename T_Graph::Vertex Vertex;
    // Distribute and announce vertices
    unsigned vertexCount   = graph.getVertices().size();
    unsigned maxVertex     = ceil((float)vertexCount / processCount);

    std::vector<Vertex> myVertices;
    for(unsigned i = 0; i < maxVertex; ++i){
	unsigned vertex_i = processID + (i * processCount);
	if(vertex_i >= vertexCount){
	    break;
	}
	else {
	    myVertices.push_back(graph.getVertices().at(vertex_i));
	}
	
    }
    return myVertices;
}

/*******************************************************************************
 *
 * VISUALIZATION OF GRAPH
 *
 *******************************************************************************/
std::vector<std::string> colors {"red", "green", "cyan", "blue", "magenta", "brown", "orange", "yellow"};

template<class Graph, class NameService>
struct vertexIDWriter{
    vertexIDWriter(Graph &graph, NameService &nameService) : graph(graph), nameService(nameService) {}
    void operator()(std::ostream& out, const BGL::Vertex& v) const {
	unsigned i = (unsigned) v;
	CommID commID= nameService.locateVertex(graph, graph.getVertices().at(i));
	out << "[color =" << colors[commID % colors.size()] << "]";
	out << "[label=\"" << graph.getVertices().at(i).id << "\"]";
    }
private:
    Graph& graph;
    NameService& nameService;
};

template<class Graph>
struct edgeIDWriter{
    edgeIDWriter(Graph &graph) : graph(graph) {}
    void operator()(std::ostream& out, const BGL::Edge& e) const {
	//unsigned i = (unsigned) e;
	//out << "[label=\""<<  << " \"]";

    }
private:
    Graph& graph;
};

struct graphWriter {
    void operator()(std::ostream& out) const {
	out << "graph [bgcolor=white]" << std::endl;
	out << "node [shape=circle color=black]" << std::endl;
	out << "edge [color=black]" << std::endl;
    }
};


/*******************************************************************************
 *
 * MAIN
 *
 *******************************************************************************/
int main(){

    /***************************************************************************
     * Create graph
     ****************************************************************************/
    std::vector<Vertex> graphVertices;
    //std::vector<EdgeDescriptor> edges = generateFullyConnectedTopology(10, graphVertices);
    //std::vector<EdgeDescriptor> edges = generateStarTopology(10, graphVertices);
    //std::vector<EdgeDescriptor> edges = generateHyperCubeTopology(4, graphVertices);
    std::vector<EdgeDescriptor> edges = generate2DMeshTopology(2, 4, graphVertices);
    BGLGraph graph (edges, graphVertices); //graph.print();




    /***************************************************************************
     * Create some subgraph
     ****************************************************************************/
    std::vector<Vertex> subGraphVertices;
    for(unsigned vertex_i = 0; vertex_i < graphVertices.size() / 2; ++vertex_i){
	subGraphVertices.push_back(graph.getVertices().at(vertex_i));
    }
    BGLGraph& subGraph = graph.createSubGraph(subGraphVertices); //subGraph.print();



    /***************************************************************************
     * Create communicator
     ****************************************************************************/
    MpiCommunicator communicator; 
    communicator.synchronize();
    CommID myCommID  = communicator.getGlobalContext().getCommID();
    unsigned commCount = communicator.getGlobalContext().size();
    NS nameService(graph, communicator);
    GC graphCommunicator(communicator, nameService);



    /***************************************************************************
     * Examples communication schemas
     ****************************************************************************/

    // Distribute vertices to communicators
    std::vector<Vertex> myGraphVertices    = distributeVerticesEvenly(myCommID, commCount, graph);
    std::vector<Vertex> mySubGraphVertices = distributeVerticesEvenly(myCommID, commCount, subGraph);

    // Output vertex property
    printVertexDistribution(myGraphVertices, graph, myCommID);
    printVertexDistribution(mySubGraphVertices, subGraph, myCommID);

    // Synchronize after output
    communicator.synchronize();

    // Announce distribution on network
    nameService.announce(graph, myGraphVertices);
    nameService.announce(subGraph, mySubGraphVertices);

    // Write graph to dot file
    if(!myGraphVertices.empty()){
	graph.writeGraph(vertexIDWriter<BGLGraph, NS>(graph, nameService), edgeIDWriter<BGLGraph>(graph), graphWriter(), std::string("graph.dot"));
    }


    // BUG
    // does not work !!!
    // if(!mySubGraphVertices.empty()){
    // 	graph.writeGraph(vertexIDWriter<BGLGraph, NS>(subGraph, nameService), edgeIDWriter<BGLGraph>(graph), graphWriter(), std::string("graph.dot"));
    // }


    //Communication on graph level
    if(!myGraphVertices.empty()){
	nearestNeighborExchange(graphCommunicator, graph, myGraphVertices); 
	reduceVertexIDs(graphCommunicator, graph, myGraphVertices);

    }

    // Communication on subgraph level
    if(!mySubGraphVertices.empty()){
	nearestNeighborExchange(graphCommunicator, subGraph, mySubGraphVertices);
	reduceVertexIDs(graphCommunicator, subGraph, mySubGraphVertices);

    }

    /***************************************************************************
     * Redistribution of vertex
     ****************************************************************************/

    // TODO
    // Communicator which has no vertex of subgraph
    // can´t occupy vertex from this subgraph!
    // Because this communicator is not part
    // of the subgraph context!
    // Need to recreate context first!
    if(!mySubGraphVertices.empty()){
    	occupyRandomVertex(communicator, subGraph, nameService, mySubGraphVertices);
    	printVertexDistribution(mySubGraphVertices, subGraph, myCommID);
    	nameService.announce2(subGraph, mySubGraphVertices); // <== Leads to some problem with context
    }

    if(!mySubGraphVertices.empty()){    
	//reduceVertexIDs(myGraphCommunicator, subGraph, mySubGraphVertices);
	nearestNeighborExchange(graphCommunicator, subGraph, mySubGraphVertices); // <== Leads finally to exception map::at in MPI.hpp asyncSend
    }


}

