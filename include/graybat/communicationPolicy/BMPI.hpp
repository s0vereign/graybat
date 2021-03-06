/**
 * Copyright 2016 Erik Zenker
 *
 * This file is part of Graybat.
 *
 * Graybat is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Graybat is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Graybat.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

// CLIB
#include <assert.h>   /* assert */

// STL
#include <array>     /* array */
#include <numeric>   /* std::accumulate */
#include <iostream>  /* std::cout */
#include <map>       /* std::map */
#include <exception> /* std::out_of_range */
#include <sstream>   /* std::stringstream */
#include <algorithm> /* std::transform */

// BOOST
#include <boost/mpi/environment.hpp>
#include <boost/mpi/communicator.hpp>
#include <boost/mpi/collectives.hpp>
#include <boost/mpi/datatype.hpp>
#include <boost/optional.hpp>
namespace mpi = boost::mpi;

// MPI
#include <mpi.h>     /* MPI_* */

// GRAYBAT
#include <graybat/utils/serialize_tuple.hpp>
#include <graybat/communicationPolicy/bmpi/Context.hpp> /* Context */
#include <graybat/communicationPolicy/bmpi/Event.hpp>   /* Event */
#include <graybat/communicationPolicy/bmpi/Config.hpp>   /* Config */
#include <graybat/communicationPolicy/Base.hpp> 
#include <graybat/communicationPolicy/Traits.hpp>

namespace graybat {
    
    namespace communicationPolicy {

	/************************************************************************//**
	 *
	 * @class BMPI
	 *
	 * @brief Implementation of the Cage communicationPolicy interface
	 *        based on the MPI implementation boost::mpi.
	 *
	 ***************************************************************************/

        struct BMPI;
        
        namespace traits {

            template<>
            struct ContextType<BMPI> {
                using type = graybat::communicationPolicy::bmpi::Context<BMPI>;
            };

			template<>
			struct ContextIDType<BMPI> {
				using type = unsigned;
			};

			template<>
            struct EventType<BMPI> {
                using type = graybat::communicationPolicy::bmpi::Event;
            };

            template<>
            struct ConfigType<BMPI> {
                using type = graybat::communicationPolicy::bmpi::Config;
            };

        }
        
	struct BMPI : Base<BMPI>{
            
    	    // Type defs
            using Tag       = typename graybat::communicationPolicy::Tag<BMPI>;
            using ContextID = typename graybat::communicationPolicy::ContextID<BMPI>;
            using MsgType   = typename graybat::communicationPolicy::MsgType<BMPI>;
            using VAddr     = typename graybat::communicationPolicy::VAddr<BMPI>;
            using Context   = typename graybat::communicationPolicy::Context<BMPI>;
            using Event     = typename graybat::communicationPolicy::Event<BMPI>;
            using Config    = typename graybat::communicationPolicy::Config<BMPI>;                        
            using Uri       = int;

	    BMPI(Config const) :contextCount(0),
							uriMap(0),
							initialContext(contextCount, mpi::communicator()){

			uriMap.push_back(std::vector<Uri>());
		
			for(auto const &vAddr : initialContext){
				uriMap.back().push_back(vAddr);
			}

	    }

		// Copy constructor
		BMPI(BMPI&) = delete;
		// Copy assignment constructor
		BMPI& operator=(BMPI&) = delete;
		// Move constructor
		BMPI(BMPI&&) = delete;
		// Move assignment constructor
		BMPI& operator=(BMPI&&) = delete;
        // Destructor
	    ~BMPI(){}
	    /***********************************************************************//**
             *
	     * @name Point to Point Communication Interface
	     *
	     * @{
	     *
	     ***************************************************************************/
	    /**
	     * @brief Blocking transmission of a message sendData to peer with virtual address destVAddr.
	     * 
	     * @param[in] destVAddr  VAddr of peer that will receive the message
	     * @param[in] tag        Description of the message to better distinguish messages types
	     * @param[in] context    Context in which both sender and receiver are included
	     * @param[in] sendData   Data reference of template type T will be send to receiver peer.
	     *                       T need to provide the function data(), that returns the pointer
	     *                       to the data memory address. And the function size(), that
	     *                       return the amount of data elements to send. Notice, that
	     *                       std::vector and std::array implement this interface.
	     */
	    template <typename T_Send>
	    void send(const VAddr destVAddr, const Tag tag, const Context context, const T_Send& sendData){
	    	Uri destUri = getVAddrUri(context, destVAddr);
		context.comm.send(destUri, tag, sendData.data(), sendData.size());
	    }

            
            /** 
	     * @brief Non blocking transmission of a message sendData to peer with virtual address destVAddr.
	     * 
	     * @param[in] destVAddr  VAddr of peer that will receive the message
	     * @param[in] tag        Description of the message to better distinguish messages types
	     * @param[in] context    Context in which both sender and receiver are included
	     * @param[in] sendData   Data reference of template type T will be.
	     *                       T need to provide the function data(), that returns the pointer
	     *                       to the data memory address. And the function size(), that
	     *                       return the amount of data elements to send. Notice, that
	     *                       std::vector and std::array implement this interface.
	     *
	     * @return Event
	     */
	    template <typename T_Send>
	    Event asyncSend(const VAddr destVAddr, const Tag tag, const Context context, const T_Send& sendData){
		Uri destUri = getVAddrUri(context, destVAddr);
		mpi::request request = context.comm.isend(destUri, tag, sendData.data(), sendData.size());
	    	return Event(request);

	    }
	    /**
	     * @brief Blocking receive of a message recvData from peer with virtual address srcVAddr.
	     * 
	     * @param[in]  srcVAddr   VAddr of peer that sended the message
	     * @param[in]  tag        Description of the message to better distinguish messages types
	     * @param[in]  context    Context in which both sender and receiver are included
	     * @param[out] recvData   Data reference of template type T will be received from sender peer.
	     *                        T need to provide the function data(), that returns the pointer
	     *                        to the data memory address. And the function size(), that
	     *                        return the amount of data elements to send. Notice, that
	     *                        std::vector and std::array implement this interface.
	     */
	    template <typename T_Recv>
	    void recv(const VAddr srcVAddr, const Tag tag, const Context context, T_Recv& recvData){
	    	Uri srcUri = getVAddrUri(context, srcVAddr);
		context.comm.recv(srcUri, tag, recvData.data(), recvData.size());

	    }

            template <typename T_Recv>
	    Event recv(const Context context, T_Recv& recvData){
                //std::cerr << mpi::any_source << " " << mpi::any_tag << std::endl;
                
                //auto status = context.comm.recv(mpi::any_source, mpi::any_tag, recvData.data(), recvData.size());
                auto status = context.comm.probe();
                context.comm.recv(status.source(), status.tag(), recvData.data(), recvData.size());
                return Event(status);
                

                //auto status = context.comm.recv(boost::mpi::any_source, boost::mpi::any_tag, recvData.data(), recvData.size());
                //auto status = context.comm.recv(boost::mpi::any_source, boost::mpi::any_tag);

	    }


	    /**
	     * @brief Non blocking receive of a message recvData from peer with virtual address srcVAddr.
	     * 
	     * @param[in]  srcVAddr   VAddr of peer that sended the message
	     * @param[in]  tag        Description of the message to better distinguish messages types
	     * @param[in]  context    Context in which both sender and receiver are included
	     * @param[out] recvData   Data reference of template type T will be received from sender peer.
	     *                        T need to provide the function data(), that returns the pointer
	     *                        to the data memory address. And the function size(), that
	     *                        return the amount of data elements to send. Notice, that
	     *                        std::vector and std::array implement this interface.
	     *
	     * @return Event
	     *
	     */
	    template <typename T_Recv>
	    Event asyncRecv(const VAddr srcVAddr, const Tag tag, const Context context, T_Recv& recvData){
            Uri srcUri = getVAddrUri(context, srcVAddr);
			mpi::request request = context.comm.irecv(srcUri, tag, recvData.data(), recvData.size());
            return Event(request);
	    }


	    /** @} */
    
	    /************************************************************************//**
	     *
	     * @name Collective Communication Interface
	     *
	     * @{
	     *
	     **************************************************************************/
	    /**
	     * @brief Collects *sendData* from all peers of the *context* and
	     *        transmits it as a list to the peer with
	     *        *rootVAddr*. Data of all peers has to be from the
	     *        **same** size.
	     *
	     * @param[in]  rootVAddr  Peer that will receive collcted data from *context* members
	     * @param[in]  context    Set of peers that want to send Data
	     * @param[in]  sendData   Data that every peer in the *context* sends.
	     *                        Data of all peers in the *context* need to have **same** size().
	     * @param[out] recvData   Data from all *context* members, that peer with virtual address 
	     *                        *rootVAddr* will receive. *recvData* of all other members of the 
	     *                        *context* will be empty.
	     */
	    template <typename T_Send, typename T_Recv>
	    void gather(const VAddr rootVAddr, const Context context, const T_Send& sendData, T_Recv& recvData){
	        Uri rootUri = getVAddrUri(context, rootVAddr);
	    	mpi::gather(context.comm, sendData.data(), sendData.size(), recvData, rootUri);
	    }

	
	    /**
	     * @brief Collects *sendData* from all members of the *context*
	     *        with **varying** size and transmits it as a list to peer
	     *        with *rootVAddr*.
	     *
	     * @todo Create some version of this function where recvCount is solid and
	     *       not dynamically determined. Since retrieving the size of send data
	     *       of every peer is a further gather operation and therefore extra
	     *       overhead.
	     *
	     * @todo Replace by boost gatherv version when available.
	     *       Patches are already submited
	     *
	     * @param[in]  rootVAddr  Peer that will receive collcted data from *context* members
	     * @param[in]  context    Set of peers that want to send Data
	     * @param[in]  sendData   Data that every peer in the *context* sends. The Data can have **varying** size
	     * @param[out] recvData   Data from all *context* peers, that peer with *rootVAddr* will receive.
	     *                        *recvData* of all other peers of the *context* will be empty. The received
	     *                        data is ordered by the VAddr of the peers.
	     * @param[out] recvCount  Number of elements each peer sends (can by varying).
	     *
	     */
	    template <typename T_Send, typename T_Recv>
	    void gatherVar(const VAddr rootVAddr, const Context context, const T_Send& sendData, T_Recv& recvData, std::vector<unsigned>& recvCount){
		// Retrieve number of elements each peer sends
		recvCount.resize(context.size());
		std::array<unsigned, 1> nElements{{(unsigned)sendData.size()}};
		allGather(context, nElements, recvCount);
		recvData.resize(std::accumulate(recvCount.begin(), recvCount.end(), 0U));

		Uri rootUri = getVAddrUri(context, rootVAddr);
                std::vector<int> rdispls(context.size(), 0);

		// Create offset map 
		unsigned offset  = 0;
                for(auto const &vAddr : context){                    
		    rdispls[vAddr] = offset; 
		    offset += recvCount[vAddr];
		
		}
	    
		// Gather data with varying size
		MPI_Gatherv(const_cast<typename T_Send::value_type*>(sendData.data()), sendData.size(),
			    mpi::get_mpi_datatype<typename T_Send::value_type>(*(sendData.data())),
			    const_cast<typename T_Recv::value_type*>(recvData.data()),
			    const_cast<int*>((int*)recvCount.data()), rdispls.data(),
			    mpi::get_mpi_datatype<typename T_Recv::value_type>(*(recvData.data())), 
			    rootUri, context.comm);
		
		
	    }

	
	    /**
	     * @brief Collects *sendData* from all members of the *context*  and transmits it as a list
	     *        to every peer in the *context*
	     *
	     * @param[in]  context  Set of peers that want to send Data
	     * @param[in]  sendData Data that every peer in the *context* sends with **same** size
	     * @param[out] recvData Data from all *context* members, that all peers* will receive.
	     *
	     */
	    template <typename T_Send, typename T_Recv>
	    void allGather(Context context, const T_Send& sendData, T_Recv& recvData){
		mpi::all_gather(context.comm, sendData.data(), sendData.size(), recvData.data());
		
	    }

	
	    /**
	     * @brief Collects *sendData* from all peers of the *context*. Size of *sendData* can vary in  size.
	     *        The data is received by every peer in the *context*.
	     *
	     * @param[in]  context    Set of peers that want to send Data
	     * @param[in]  sendData   Data that every peer in the *context* sends with **varying** size 
	     * @param[out] recvData   Data from all *context* members, that all peers* will receive.
	     * @param[out] recvCount  Number of elements each peer sends (can by varying).
	     *
	     */
	     template <typename T_Send, typename T_Recv>
	     void allGatherVar(const Context context, const T_Send& sendData, T_Recv& recvData, std::vector<unsigned>& recvCount){
	         // Retrieve number of elements each peer sends
	         recvCount.resize(context.size());
	         allGather(context, std::array<unsigned, 1>{{(unsigned)sendData.size()}}, recvCount);
	         recvData.resize(std::accumulate(recvCount.begin(), recvCount.end(), 0U));

                 std::vector<int> rdispls(context.size());

		 // Create offset map
		 unsigned offset  = 0;
		 for (unsigned i=0; i < context.size(); ++i) { 
		     rdispls[i] = offset; 
		     offset += recvCount[i];
		
		 }
	    
		 // Gather data with varying size
		 MPI_Allgatherv(const_cast<typename T_Send::value_type*>(sendData.data()), sendData.size(),
				mpi::get_mpi_datatype<typename T_Send::value_type>(*(sendData.data())),
				const_cast<typename T_Recv::value_type*>(recvData.data()),
				const_cast<int*>((int*)recvCount.data()), rdispls.data(),
				mpi::get_mpi_datatype<typename T_Recv::value_type>(*(recvData.data())), 
				context.comm);
		 
		 
	     }


	    /**
	     * @brief Distributes *sendData* from peer *rootVAddr* to all peers in *context*.
	     *        Every peer will receive different data.
	     *
	     * @remark In Contrast to broadcast where every peer receives the same data
	     *
	     * @param[in]  rootVAddr peer that want to distribute its data
	     * @param[in]  context    Set of peers that want to receive Data
	     * @param[in]  sendData   Data that peer with *rootVAddr* will distribute over the peers of the *context*
	     * @param[out] recvData   Data from peer with *rootVAddr*.
	     *
	     */
	    template <typename T_Send, typename T_Recv>
	    void scatter(const VAddr rootVAddr, const Context context, const T_Send& sendData, T_Recv& recvData){
	    	 Uri rootUri = getVAddrUri(context, rootVAddr);
		 mpi::scatter(context.comm, sendData.data(), recvData.data(), recvData.size(), rootUri);

	    }

	
	    /**
	     * @brief Distributes *sendData* of all peer in the *context* to all peers in the *context*.
	     *        Every peer will receive data from every other peer (also the own data)
	     *
	     * @param[in]  context  Set of peers that want to receive Data
	     * @param[in]  sendData Data that each peer wants to send. Each peer will receive 
	     *             same number of data elements, but not the same data elements. sendData
	     *             will be divided in equal chunks of data and is then distributed.
	     *             
	     * @param[out] recvData Data from all peer.
	     *
	     */
	    template <typename T_Send, typename T_Recv>
	    void allToAll(const Context context, const T_Send& sendData, T_Recv& recvData){
	        unsigned elementsPerPeer = sendData.size() / context.size();

		mpi::all_to_all(context.comm, sendData.data(), elementsPerPeer, recvData.data());

	    }

	
	    /**
	     * @brief Performs a reduction with a binary operator *op* on all *sendData* elements from all peers
	     *        whithin the *context*. The result will be received by the peer with *rootVAddr*.
	     *        Binary operations like std::plus, std::minus can be used. But, they can also be
	     *        defined as binary operator simular to std::plus etc.
	     *        
	     *
	     * @param[in]  rootVAddr peer that will receive the result of reduction
	     * @param[in]  context   Set of peers that 
	     * @param[in]  op        Binary operator that should be used for reduction
	     * @param[in]  sendData  Data that every peer contributes to the reduction
	     * @param[out] recvData  Reduced sendData that will be received by peer with *rootVAddr*.
	     *                       It will have same size of sendData and contains the ith
	     *                       reduced sendData values.
	     *
	     */
	    template <typename T_Send, typename T_Recv, typename T_Op>
	    void reduce(const VAddr rootVAddr, const Context context, const T_Op op, const T_Send& sendData, T_Recv& recvData){
	    	 Uri rootUri = getVAddrUri(context, rootVAddr);
		 mpi::reduce(context.comm, sendData.data(), sendData.size(), recvData.data(), op, rootUri);

	    }

	    /**
	     * @brief Performs a reduction with a binary operator *op* on all *sendData* elements from all peers
	     *        whithin the *context*. The result will be received by all peers.
	     *        
	     * @param[in] context    Set of peers that 
	     * @param[in] op         Binary operator that should be used for reduction
	     * @param[in] sendData   Data that every peer contributes to the reduction
	     * @param[out] recvData  Reduced sendData that will be received by all peers.
	     *                       It will have same size of sendData and contains the ith
	     *                       reduced sendData values.
	     *
	     */
	    template <typename T_Send, typename T_Recv, typename T_Op>
	    void allReduce(const Context context, T_Op op, const T_Send& sendData, T_Recv& recvData){
		mpi::all_reduce(context.comm, sendData.data(), sendData.size(), recvData.data(), op);
	     
	    }

	
	    /**
	     * @brief Send *sendData* from peer *rootVAddr* to all peers in *context*.
	     *        Every peer will receive the same data.
	     *
	     * @remark In Contrast to scatter where every peer receives different data
	     *
	     * @param[in] rootVAddr Source peer
	     * @param[in] context    Set of peers that want to receive Data
	     * @param[in] sendData   Data that peer with *rootVAddr* will send to the peers of the *context*
	     * @param[out] recvData  Data from peer with *rootVAddr*.
	     *
	     */
	    template <typename T_SendRecv>
	    void broadcast(const VAddr rootVAddr, const Context context, T_SendRecv& data){
	    	 Uri rootUri = uriMap.at(context.getID()).at(rootVAddr);
		 mpi::broadcast(context.comm, data.data(), data.size(), rootUri);
	    }

	
	    /**
	     * @brief Synchronizes all peers within *context* to the same point
	     *        in the programm execution (barrier).
	     *        
	     */
	     void synchronize(const Context context){
		 context.comm.barrier();
	     }

	
	    /**
	     * @brief Synchronizes all peers within the globalContext
	     *        in the programm execution (barrier).
	     *
	     * @see getGlobalContext()
	     *        
	     */
	     void synchronize(){
	         synchronize(getGlobalContext());
	     }
	    /** @} */

    
	    /*************************************************************************//**
	     *
	     * @name Context Management Interface
	     *
	     * @{
	     *
	     ***************************************************************************/
	    /**
	     * @brief Creates a new context with all peers that declared isMember as true.
	     *
	     */
            Context splitContext(const bool isMember, const Context oldContext){
                mpi::communicator newComm = oldContext.comm.split(isMember);

                if(isMember){
	    	    Context newContext(++contextCount, newComm);
                    std::array<Uri, 1> uri {{ (int) newContext.getVAddr() }};
                    uriMap.push_back(std::vector<Uri>(newContext.size()));

                    std::vector<Event> events;
                    
                    for(auto const &vAddr : newContext){
                        events.push_back(Event(newContext.comm.isend(vAddr, 0, uri.data(), 1)));
                    }

                    for(auto const &vAddr : newContext){                        
                        std::array<Uri, 1> otherUri {{ 0 }};
                        newContext.comm.recv(vAddr, 0, otherUri.data(), 1);
                        uriMap.at(newContext.getID()).at(vAddr) = otherUri[0];
                        
                    }

                    for(unsigned i = 0; i < events.size(); ++i){
                        events.back().wait();
                        events.pop_back();
                    }

                    return newContext;

                    
                }
                else {
                    return Context();
                }

                
            }

	
	    /**
	     * @brief Returns the context that contains all peers
	     *
	     */
	    Context getGlobalContext(){
	     	return initialContext;
	    }
	    /** @} */

	    	private:

	    /***************************************************************************
	     *
	     * @name Private Member
	     *
	     ***************************************************************************/
	    ContextID                      contextCount;
	    std::vector<std::vector<Uri>>  uriMap;
	    Context                        initialContext;
	    mpi::environment               env;

	    /***************************************************************************
	     *
	     * @name Helper Functions
	     *
	     ***************************************************************************/
	    
	    
	    void error(VAddr vAddr, std::string msg){
		std::cout << "[" << vAddr << "] " << msg;

	    }

	    /**
	     * @brief Returns the uri of a vAddr in a
	     *        specific context.
	     *
	     */
	    template <typename T_Context>
	    inline Uri getVAddrUri(const T_Context context, const VAddr vAddr){
	    	Uri uri  = 0;
	    	try {
	    	    uri = uriMap.at(context.getID()).at(vAddr);

	    	} catch(const std::out_of_range& e){
	    	    std::stringstream errorStream;
	    	    errorStream << "MPI::getVAddrUri::" << e.what()<< " : Communicator with ID " << vAddr << " is not part of the context " << context.getID() << std::endl;
	    	    error(context.getID(), errorStream.str());
	    	    exit(1);
	    	}

	    	return uri;
	    }

	};

    } // namespace communicationPolicy
	
} // namespace graybat
