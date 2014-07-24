#pragma once
#include <map> /* map */
#include <set> /* set */

#include <binaryOperation.hpp> /* op::maximum */

/************************************************************************//**
 * @class NameService
 *
 * @brief A central instance to locate the host Communicator
 *        of vertices.
 *
 * @remark A Communicator can host several vertices.
 * @todo   Think of vertices hosted by several Communicators
 *         for fault tolerance purpose
 *
 *
 ***************************************************************************/
template <typename T_Graph, typename T_Communicator>
struct NameService {
    typedef T_Communicator                   Communicator;
    typedef T_Graph                          Graph;
    typedef typename Graph::Vertex           Vertex;
    typedef typename Communicator::CommID    CommID;
    typedef typename Vertex::ID              VertexID;
    typedef typename Graph::GraphID          GraphID;
    typedef typename Communicator::Context   Context;
    typedef typename Communicator::ContextID ContextID;

    // Maps
    std::map<GraphID, std::map<VertexID, CommID> > commMap;
    std::map<GraphID, Context> contextMap; /** Each graph is mapped to a context of the Communicator*/
    std::map<GraphID, std::map<CommID, std::vector<Vertex>> > vertexMap;

    // References
    Graph& graph;
    Communicator& communicator;

    NameService(Graph& graph, Communicator& communicator) : 
	graph(graph),
	communicator(communicator){

    }

    /**
     * @brief Announces *vertices* of a *graph* to the network, so that other Communicators
     *        know that these *vertices* are hosted by this Communicator.
     *
     * The general workflow includes two steps:
     *  1. Each Communicator, that hosts vertices of the *graph* announces its *vertices*
     *     * Each Communicator will send its hosted vertices and update its vertices location
     *     * The host Communicators will create a new context for *graph*
     *  2. Vertices can now be located by locateVertex()
     *  3. use GraphCommunicator to communicate between vertices
     *
     * @remark This is a collective Operation on which either all host Communicators
     *         of the supergraph of *graph* have to take part or when *graph* has no
     *         supergraph then all Communicatos from the globalContext (which should
     *         be all Communicators in the network).
     *
     * @todo What happens if *vertices* is empty ?
     *
     * @todo What happens when there already exist an context for *graph* ?
     *
     * @param[in] graph  Its vertices will be announced
     * @param[in] vertices A set of vertices, that will be hosted by this Communicator
     *
     */
    void announce(Graph& graph, const std::vector<Vertex> vertices){
	// Get super context
	Context oldContext = getGraphContext(graph);
	
	if(!oldContext.valid()){
	    if(graph.hasSuperGraph()){
		oldContext = getGraphContext(graph.superGraph);
	    }
	    else {
		oldContext = communicator.getGlobalContext();
	    }

	}
	
	if(oldContext.valid()){
	    //Each process announces which vertices it manages
	    std::array<unsigned, 1> myVerticesCount {{(unsigned) vertices.size()}};
	    std::array<unsigned, 1> maxVerticesCount  {{0}};
	    communicator.allReduce(oldContext, op::maximum<unsigned>(), myVerticesCount, maxVerticesCount);
	
	    std::vector<std::vector<Vertex> > newVertexMap (oldContext.size(), std::vector<Vertex>());
	    for(unsigned i = 0; i < maxVerticesCount[0]; ++i){
	    	std::vector<int> sendData(1, -1);
	    	std::vector<int> recvData(oldContext.size(), 0);

	    	if(i < vertices.size()){
	    	    sendData[0] = graph.getLocalID(vertices.at(i));
	    	}
	    	else{
	    	    sendData[0] = -1;
	    	}

	    	communicator.allGather(oldContext, sendData, recvData);


	    	for(unsigned commID = 0; commID < newVertexMap.size(); ++commID){
	    	    if(recvData[commID] != -1){
	    		VertexID vertexID = (VertexID) recvData[commID];
	    		Vertex v = graph.getVertices().at(vertexID);
	    		commMap[graph.id][v.id] = commID; 
	    		newVertexMap[commID].push_back(v);
		    
	    	    }

	    	}
      
	    	for(unsigned commID = 0; commID < newVertexMap.size(); ++commID){
	    	    vertexMap[graph.id][commID] = newVertexMap[commID];

	    	}

	    }

	    //Create new sub context for the graph
	    if(graph.hasSuperGraph()){
	    	createGraphContext(graph.superGraph, graph);

	    }
	    else {
	    	createGraphContext(graph);

	    }


	}


    }

    void announce2(Graph& graph, const std::vector<Vertex> vertices){
	// Get super context
	Context oldContext = getGraphContext(graph);
	//bool graphHasContext = false;

	if(!oldContext.valid()){
	    if(graph.hasSuperGraph()){
		oldContext = getGraphContext(graph.superGraph);

	    }
	    else {
		oldContext = communicator.getGlobalContext();

	    }

	}

	// TODO
	// is this "if" needed ?
	if(oldContext.valid()){
	    // Create new context for communicators which have vertices
	    std::vector<unsigned> sendHasVertices(1, (unsigned)!vertices.empty());
	    std::vector<unsigned> recvHasVertices(oldContext.size(), 0);
	    communicator.allGather(oldContext, sendHasVertices, recvHasVertices);

	    std::vector<CommID> commIDsWithVertices;
	    for(unsigned i = 0; i < oldContext.size(); ++i){
		if(recvHasVertices[i] == 1){
		    commIDsWithVertices.push_back(i);
		}
	    }

	    Context newContext = communicator.createContext(commIDsWithVertices, oldContext);
	    if(newContext.valid()){
	    
		contextMap[graph.id] = newContext;

		//Each process announces which vertices it manages
		std::array<unsigned, 1> myVerticesCount {{(unsigned) vertices.size()}};
		std::array<unsigned, 1> maxVerticesCount  {{0}};
		communicator.allReduce(newContext, op::maximum<unsigned>(), myVerticesCount, maxVerticesCount);

		std::vector<std::vector<Vertex> > newVertexMap (newContext.size(), std::vector<Vertex>());
		for(unsigned i = 0; i < maxVerticesCount[0]; ++i){
		    std::vector<int> sendData(1, -1);
		    std::vector<int> recvData(newContext.size(), 0);

		    if(i < vertices.size()){
			sendData[0] = graph.getLocalID(vertices.at(i));
		    }
		    else{
			sendData[0] = -1;
		    }

		    communicator.allGather(newContext, sendData, recvData);
		
		    // BUG
		    // When updating context of graph that still
		    // exist (by reducing number of host comms)
		    // the range of CommIDs will be reduced and
		    // therefore commID of old context are
		    // not valid anymore.
		    for(unsigned commID = 0; commID < newVertexMap.size(); ++commID){
			if(recvData[commID] != -1){
			    VertexID vertexID = (VertexID) recvData[commID];
			    Vertex v = graph.getVertices().at(vertexID);
			    commMap[graph.id][v.id] = commID; 
			    newVertexMap[commID].push_back(v);
		    
			}

		    }
      
		    for(unsigned commID = 0; commID < newVertexMap.size(); ++commID){
			vertexMap[graph.id][commID] = newVertexMap[commID];

		    }

		}

		// // Update old context
		// if(graphHasContext){
		//     createGraphContext(graph, graph);
		// }
		// // Create new sub context for the graph
		// else{

		//     if(graph.hasSuperGraph()){
		// 	createGraphContext(graph.superGraph, graph);
		
		//     }
		//     else {
		// 	createGraphContext(graph);
		
		//     }

		// }
	    
	    }

	}
	
    }
  
    /**
     * @brief Returns the CommID of the host Communicator of *vertex* in the *graph*
     *
     * @bug When the location of *vertex* is not known then
     *      the programm crashes by an exception. 
     *      This exception should be handled for better
     *      debugging behaviour.
     *
     * @param[in] graph Contains *vertex*.
     * @param[in] vertex Will be located.
     *
     */
    CommID locateVertex(Graph& graph, Vertex vertex){
	return commMap[graph.id][vertex.id];

    }

    /**
     * @brief Opposite operation of locateVertex(). It returns the
     *        vertices that are hosted by the Communicator with
     *        *commID*
     *
     * @bug Exception when commID is not host of vertices in *graph*.
     *      This Exception should be hanled somehow!
     *
     */
    std::vector<Vertex> getHostedVertices(Graph& graph, CommID commID){
	return vertexMap[graph.id][commID];

    }

    /**
     * @brief Returns the context of the *graph*. All host
     *        Communicators of the *graph* are part of the
     *        returned context.
     *
     */
    Context getGraphContext(Graph& graph){
	return contextMap[graph.id];

    }

    /**
     * @brief Returns a set of all host Communicator CommIDs of the *graph*
     *
     */
    std::vector<CommID> getGraphHostCommIDs(Graph& graph){
	std::vector<Vertex> vertices = graph.getVertices();

	std::set<CommID> commIDs;
	for(Vertex vertex : vertices){
	    commIDs.insert(locateVertex(graph, vertex));
	}

	return std::vector<CommID>(commIDs.begin(), commIDs.end());

    }

private:

    /**
     * @brief Creates a context from the given *subgraph* inherited from
     *        the context of the given graph.
     *
     * @param[in] graph is the supergraph of subGraph
     * @param[in] subGraph is a subgraph of graph
     *
     */
    void createGraphContext(Graph& graph, Graph& subGraph){
	std::vector<CommID> commIDs = getGraphHostCommIDs(subGraph);

	Context oldContext = getGraphContext(graph);
	Context newContext = communicator.createContext(commIDs, oldContext);
	if(newContext.valid()){
	    contextMap[subGraph.id] = newContext;
	}

    }

    /**
     * @brief Creates a context for the the given graph inherited from
     *        the global context. After this step, vertices within
     *        this graph can do communication.
     *
     * @param[in] graph for which a new context from global context will be created.
     */
    void createGraphContext(Graph& graph){
	std::vector<CommID> commIDs = getGraphHostCommIDs(graph);

	Context oldContext = communicator.getGlobalContext();
	Context newContext = communicator.createContext(commIDs, oldContext);
	
	contextMap[graph.id] = newContext;
    
    }



};
