#pragma once

template <class T_Cage>
struct EdgeTest {

    typedef T_Cage Cage;
    typedef typename Cage::GraphPolicy GraphPolicy;
    typedef typename Cage::Vertex Vertex;
    typedef typename GraphPolicy::EdgeProperty EdgeProperty;
    typedef typename Cage::Event Event;

	    
    unsigned id;
    const Vertex vertex;    
    EdgeProperty &edgeProperty;
    Cage &cage;
	    


    EdgeTest(const unsigned id, const Vertex vertex, EdgeProperty &edgeProperty, Cage &cage) :
	id(id),
	vertex(vertex),
	edgeProperty(edgeProperty),
	cage(cage){
	    
    }

    EdgeProperty operator()(){
	return edgeProperty;
    }

    template <class T_Send>
    Event operator<<(const T_Send &data){
	return cage.asyncSend(vertex, *this, data);
    }

    template <class T_Recv>
    void operator>>(T_Recv &data){
	cage.recv(vertex, *this, data);
    }
	
};
