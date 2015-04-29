/*******************************************************************************
 *
 * GRAPH TOPOLOGY GENERATORS
 *
 *******************************************************************************/

namespace graybat {

  namespace pattern {

    typedef unsigned                                                        VertexID;
    typedef std::pair<VertexID, VertexID>                                   EdgeDescription;
    typedef std::pair<std::vector<VertexID>, std::vector<EdgeDescription> > GraphDescription;

    
    struct Star {

      const unsigned verticesCount;

      Star(const unsigned verticesCount) :
	verticesCount(verticesCount) {

      }
    

      GraphDescription operator()(){
	std::vector<VertexID> vertices(verticesCount);
    
	std::vector<EdgeDescription> edges;

	for(unsigned i = 0; i < vertices.size(); ++i){
	  vertices.at(i) = i;
	  if(i != 0){
	    edges.push_back(std::make_pair(i, 0));
	  }
		
	}

	return std::make_pair(vertices,edges);
      }

    };

  } /* pattern */

} /* graybat */
