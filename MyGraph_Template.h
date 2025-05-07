#ifndef _MYGRAPH_H_
#define _MYGRAPH_H_

#include <iostream>
#include <fstream>

#include "MyVector_c336t319.h"
#include "MyLinkedList_c336t319.h"
#include "MyQueue_c336t319.h"
#include "MyStack_c336t319.h"
#include "MyHashTable_c336t319.h"


// the graph is supposed to be a undirected graph
// no multi-edge is allowed (assumed to be ensured from the input)

typedef long long VertexIDType;
typedef long long EdgeIDType;

template <typename VertexDataType, typename EdgeDataType>
class MyGraph
{
  public:

    class Vertex
    {
      public: 
        // default constructor
        Vertex()
        {
            return;
        }

        // vertex copy constructor
        explicit Vertex(const VertexDataType& vertex_data) :
            data(vertex_data)
        {
            return;
        }

        // vertex move constructor
        explicit Vertex(VertexDataType && vertex_data) :
            data(std::move(vertex_data))
        {
            return;
        }

        // copy assignment
        Vertex& operator=(const Vertex& rhs)
        {
            id = rhs.id;
            tag = rhs.tag;
            data = rhs.data;
            return *this;
        }

        // move assignment
        Vertex& operator=(Vertex && rhs)
        {
            id = rhs.id;
            tag = rhs.tag;
            data = std::move(rhs.data);
            return *this;
        }

        VertexIDType id;        // the unique ID of the vertex
        int tag;                // a reserved tag variable
        VertexDataType data;    // the property associated with the vertex

        friend class MyGraph;

    };

    class Edge
    {
      public:
        // default constructor
        Edge()
        {
            return;
        }

        // copy constructor
        // sid: source vertex ID; vid: target vertex ID
        explicit Edge(const VertexIDType sid, const VertexIDType tid, const EdgeDataType& edge_data) :
            src(sid),
            tgt(tid),
            data(edge_data)
        {
            return;
        }

        // move constructor
        // sid: source vertex ID; vid: target vertex ID
        explicit Edge(const VertexIDType sid, const VertexIDType tid, EdgeDataType && edge_data) :
            src(sid),
            tgt(tid),
            data(std::move(edge_data))
        {
            return;
        }

        // copy assignment
        Edge& operator=(const Edge& rhs)
        {
            id = rhs.id;
            src = rhs.src;
            tgt = rhs.tgt;
            tag = rhs.tag;
            data = rhs.data;
            return *this;
        }

        // move assignment
        Edge& operator=(Edge && rhs)
        {
            id = rhs.id;
            src = rhs.src;
            tgt = rhs.tgt;
            tag = rhs.tag;
            data = std::move(rhs.data);
            return *this;
        }
      
        EdgeIDType id;          // the unique ID of the edge
        VertexIDType src, tgt;  // the IDs for the source vertex and the target vertex of the edge
        int tag;                // the reserved tag varaible
        EdgeDataType data;      // the property associated with the edge

        friend class MyGraph;

    };

    // default constructor of graph
    // initialize the graph with a size of 0
    MyGraph() :
        current_vertex_ID(0),
        current_edge_ID(0),
        num_vertices(0),
        num_edges(0),
        vertex_set(0),
        edge_set(0),
        vertex_map{},
        edge_map{},
        adj_list()
    {   
        return;
    }

    // destructor
    ~MyGraph()
    {
        for(size_t i = 0; i < vertex_set.size(); ++ i)
        {
            delete vertex_set[i];
        }
        for(size_t i = 0; i < edge_set.size(); ++ i)
        {
            delete edge_set[i];
        }
        for(size_t i = 0; i < adj_list.size(); ++ i)
        {
            adj_list[i]->clear();
            delete adj_list[i];
        }
        return;
    }

    // adds an vertex v to the graph (copy)
    // assign ID according the to oder of vertex insertion
    // for example, assign ID 1 to the first vertex, 2 to the second vertex, ..., and n to the nth vertex
    // returns the assigned vertex ID for v
    VertexIDType addVertex(const VertexDataType& v_data)
    {
        // code begins
        VertexIDType vid = ++current_vertex_ID;
        Vertex* v = new Vertex(v_data);
        v->id = vid;
        v->tag = 0;
        vertex_set.push_back(v);
        size_t pos = vertex_set.size() -1;
        vertex_map.insert(HashedObj<VertexIDType,size_t>(vid, pos));
        adj_list.push_back(new MyLinkedList<EdgeIDType>());
        ++num_vertices;
        return vid;
        // code ends
    }

    // adds an vertex v to the graph (move)
    // assign ID according the to oder of vertex insertion
    // for example, assign ID 1 to the first vertex, 2 to the second vertex, ..., and n to the nth vertex
    // returns the assigned vertex ID for v
    VertexIDType addVertex(VertexDataType && v_data)
    {
        // code begins
        VertexIDType vid = ++current_vertex_ID;
        Vertex* v = new Vertex(std::move(v_data));
        v->id = vid;
        v->tag = 0;
        vertex_set.push_back(v);
        size_t pos = vertex_set.size() -1;
        vertex_map.insert(HashedObj<VertexIDType,size_t>(vid, pos));
        adj_list.push_back(new MyLinkedList<EdgeIDType>());
        ++num_vertices;
        return vid;
        // code ends
    }

    // retrieves the information of the vertex specified by the vid
    // returns the vertex
    Vertex* getVertex(const VertexIDType vid)
    {
        // code begins
        size_t pos = vertexID2SetPos(vid);
        return vertex_set[pos];
        // code ends
    }

    // adds an edge e between the source node src and the target node tgt (copy)
    // adds to both of the src's adjacency list and tgt's adjacency list (as undirected graph)
    // assign ID according the to oder of edge insertion
    // for example, assign ID 1 to the first edge, 2 to the second edge, ..., and n to the nth edge
    // returns the assigned edge ID for e
    EdgeIDType addEdge(const VertexIDType sid, const VertexIDType tid, const EdgeDataType& e_data)
    {
        // code begins
        EdgeIDType eid = ++current_edge_ID;
        Edge* e = new Edge(sid, tid, e_data);
        e->id = eid;
        e->tag = 0;
        
        edge_set.push_back(e);
        size_t pos = edge_set.size() -1;
        edge_map.insert(HashedObj<VertexIDType,size_t>(eid, pos));
        
        size_t pos_s = vertexID2SetPos(sid);
        size_t pos_x = vertexID2SetPos(tid);
        adj_list[pos_s] -> push_back(eid);
        adj_list[pos_x] -> push_back(eid);

        ++num_edges;
        return eid;
        // code ends
    }

    // adds an edge e between the source node src and the target node tgt (copy)
    // adds to both of the src's adjacency list and tgt's adjacency list (as undirected graph)
    // assign ID according the to oder of vertex insertion
    // for example, assign ID 1 to the first vertex, 2 to the second vertex, ..., and n to the nth vertex
    // returns the assigned edge ID for e
    EdgeIDType addEdge(const VertexIDType sid, const VertexIDType tid, EdgeDataType && e_data)
    {
        // code begins
        EdgeIDType eid = ++current_edge_ID;
        Edge* e = new Edge(sid, tid, std::move(e_data));
        e->id = eid;
        e->tag = 0;
        
        edge_set.push_back(e);
        size_t pos = edge_set.size() -1;
        edge_map.insert(HashedObj<VertexIDType,size_t>(eid, pos));
        
        size_t pos_s = vertexID2SetPos(sid);
        size_t pos_x = vertexID2SetPos(tid);
        adj_list[pos_s] -> push_back(eid);
        adj_list[pos_x] -> push_back(eid);

        ++num_edges;
        return eid;
        // code ends
    }

    // retrieves the information of the edge specified by the eid
    // returns the edge
    Edge* getEdge(const EdgeIDType eid)
    {
        // code begins
        size_t pos = edgeID2SetPos(eid);
        return edge_set[pos];
        // code ends
    }

    // checks whether an exists between src and tgt
    // if yes, return TRUE and record the corresponding edge ID in eid
    // if no, return FALSE
    bool probeEdge(const VertexIDType sid, const VertexIDType tid, EdgeIDType& eid)
    {
        // code begins
        auto* list_s = adj_list[vertexID2SetPos(sid)];
        for(auto itr = list_s->begin(); itr != list_s->end(); ++itr)
        {
            EdgeIDType canidate = *itr;
            Edge* e = getEdge(canidate);
            if((e->src == sid && e->tgt == tid) || (e->src == tid && e->tgt == sid))
            {
                eid = canidate;
                return true;
            }
        }
        return false;
        // code ends
    }

    // returns the degree of the vertex specified by vid
    size_t degree(const VertexIDType vid)
    {
        // code begins
        auto* list_v = adj_list[vertexID2SetPos(vid)];
        size_t deg = 0;
        for(auto itr = list_v -> begin(); itr != list_v -> end(); ++itr)
        {
            ++deg;
        }
        return deg;
        // code ends
    }

    // deletes the vertex specified by vid from the graph
    // also delete all edges associated with it
    // expected time complexity: O(d^2), where d is the average degree of the graph
    void deleteVertex(const VertexIDType vid)
    {
        // code begins
        size_t vpos = vertexID2SetPos(vid);
        Vertex* v = vertex_set[vpos];

        MyVector<EdgeIDType> to_remove;
        for(auto itr = adj_list[vpos]->begin(); itr != adj_list[vpos]->end(); ++itr)
        {
            to_remove.push_back(*itr);
        }
        for(size_t i = 0; i < to_remove.size(); ++i)
        {
            deleteEdge(to_remove[i]);
        }
        adj_list[vpos]->clear();
        delete adj_list[vpos];
        delete v;

        size_t last = vertex_set.size()-1;
        if(vpos != last)
        {
            Vertex* back = vertex_set[last];
            vertex_set[vpos] = back;
            vertex_map.remove(back->id); //sgjdsjgdjsgljdslkfjkdljflskjfldsjlfjsdlfjsdjflsdjflsdjfkljsdlfjsdlfjsld
            vertex_map.insert(HashedObj<VertexIDType,size_t>(back->id, vpos));
            adj_list[vpos] = adj_list[last];
        }
        vertex_set.pop_back();
        adj_list.pop_back();
        vertex_map.remove(vid);
        --num_vertices;
        // code ends
    }

    // deletes the edge specified by eid from the graph
    // expected time complexity: O(d), where d is the average degree of the graph
    void deleteEdge(const EdgeIDType eid)
    {
        // code begins
        size_t epos = edgeID2SetPos(eid);
        Edge* e = edge_set[epos];
        size_t pos_s = vertexID2SetPos(e->src);
        size_t pos_x = vertexID2SetPos(e->tgt);
        adj_list[pos_s]->remove(eid);
        adj_list[pos_x]->remove(eid);
        delete e;

        size_t last = edge_set.size()-1;
        if(epos != last)
        {
            Edge* back = edge_set[last];
            edge_set[epos] = back;
            edge_map.remove(back->id);
            edge_map.insert(HashedObj<VertexIDType,size_t>(back->id, epos));
        }
        edge_set.pop_back();
        edge_map.remove(eid);
        --num_edges;
        // code ends
    }

    // performs breath-first-search on the graph, starting from v_src
    // record the path in path
    // outgoing edge visit order should be determined based on their ID (smaller ID visited earlier)
    // the source vertex should also be included in the traversal path
    void breadthFirstSearch(const VertexIDType v_src, MyVector<VertexIDType>& path)
    {
        // code begins
        path.resize(0);
        MyHashTable<VertexIDType, bool> visited;
        MyQueue<VertexIDType> queue;
        visited.insert(HashedObj<VertexIDType,bool>(v_src, true));
        queue.enqueue(v_src);
        while(!queue.empty())
        {
            VertexIDType u = queue.front();
            queue.dequeue();
            path.push_back(u);
            size_t pos_u = vertexID2SetPos(u);
            MyVector<EdgeIDType> edges;
            for(auto itr = adj_list[pos_u] ->begin(); itr != adj_list[pos_u]->end(); ++itr)
            {
                edges.push_back(*itr);
            }
            for(size_t i = 0; i < edges.size(); ++i)
            {
                for(size_t j = i+1; j < edges.size(); ++j)
                {
                    if(edges[i] > edges[j])
                    {
                        std::swap(edges[i], edges[j]);
                    }
                }
            }
            for(size_t i = 0; i < edges.size(); ++i)
            {
                Edge* e = getEdge(edges[i]);
                VertexIDType v = (e->src == u ? e->tgt : e->src);
                bool seen;
                if(!visited.get(v, seen))
                {
                    visited.insert(HashedObj<VertexIDType,bool>(v, true));
                    queue.enqueue(v);
                }
            }
        }
        // code ends
    }

    // performs depth-first-search on the graph, starting from v_src
    // record the path in path
    // outgoing edge visit order should be determined based on their ID (smaller ID visited earlier)
    // the source vertex should also be included in the traversal path
    void depthFirstSearch(const VertexIDType v_src, MyVector<VertexIDType>& path)
    {
        // code begins
        path.resize(0);
        MyHashTable<VertexIDType, bool> visited;
        MyStack<VertexIDType> stack;
        stack.push(v_src);
        while(!stack.empty())
        {
            VertexIDType u = stack.top();
            stack.pop();
            bool was_seen;
            if(visited.get(u, was_seen) && was_seen)
            {
                continue;
            }
            visited.insert(HashedObj<VertexIDType,bool>(u, true));
            path.push_back(u);
            size_t pos_u = vertexID2SetPos(u);
            MyVector<EdgeIDType> edges;
            for(auto itr = adj_list[pos_u]->begin(); itr != adj_list[pos_u]->end(); ++itr)
            {
                edges.push_back(*itr);
            }
            for(size_t i = 0; i < edges.size(); ++i)
            {
                for(size_t j = i+1; j < edges.size(); ++j)
                {
                    if(edges[i] > edges[j])
                    {
                        std::swap(edges[i], edges[j]);
                    }
                }
            }
            for(int i = (int)edges.size()-1; i >= 0; --i)
            {
                Edge* e = getEdge(edges[i]);
                VertexIDType v = (e->src == u ? e->tgt : e->src);
                bool seen_v;
                if(!(visited.get(v, seen_v) && seen_v))
                {
                    stack.push(v);
                }
            }
        }
        // code ends
    }

    // determines whether the two nodes vid1 and vid2 are connected
    // if yes, return TRUE
    // if no, return FASLE
    bool isConnected(const VertexIDType vid1, const VertexIDType vid2)
    {
        // code begins
        if(vid1 == vid2)
        {
            return true;
        }
        MyHashTable<VertexIDType, bool> visited;
        MyQueue<VertexIDType> queue;
        visited.insert(HashedObj<VertexIDType,bool>(vid1, true));
        queue.enqueue(vid1);
        while(!queue.empty())
        {
            VertexIDType u = queue.front(); 
            queue.dequeue();
            size_t pos_u = vertexID2SetPos(u);
            for(auto itr = adj_list[pos_u]->begin(); itr != adj_list[pos_u]->end(); ++itr)
            {
                Edge* e = getEdge(*itr);
                VertexIDType v = (e->src == u ? e->tgt : e->src);
                bool seen;
                if(!visited.get(v,seen))
                {
                    if(v == vid2)
                    {
                        return true;
                    }
                    visited.insert(HashedObj<VertexIDType,bool>(v, true));
                    queue.enqueue(v);
                }
            }
        }
        return false;
        // code ends
    }

    // returns the number of vertices in the graph
    size_t numVertices(void) const
    {
        return num_vertices;
    }

    // returns the number of edges in the graph
    size_t numEdges(void) const
    {
        return num_edges;
    }

    // checks whether the graph is empty (no vertex)
    bool empty(void) const
    {
        return (num_vertices == 0);
    }

    // prints the content in vertex_set
    void printInfoVertexSet(void) 
    {
        for(size_t i = 0; i < num_vertices; ++ i)
        {
            std::cout << "Array Position: " << i << "\t";
            std::cout << "Vertex ID: " << vertex_set[i]->id << "\t";
            std::cout << "Vertex data: " << vertex_set[i]->data << std::endl;
        }
        return;
    }

    // prints the content in edge_set
    void printInfoEdgeSet(void) 
    {
        for(size_t i = 0; i < num_edges; ++ i)
        {
            std::cout << "Array Position: " << i << "\t";
            std::cout << "Edge ID: " << edge_set[i]->id << "\t";
            std::cout << "Edge ends: " << edge_set[i]->src << "\t" << edge_set[i]->tgt << "\t";
            std::cout << "Edge data: " << edge_set[i]->data << std::endl;
        }
        return;
    }

    // prints the content in vertex_map
    void printInfoVertexMap(void) 
    {
        MyVector<VertexIDType> v_keys;
        vertex_map.keys(v_keys);
        //adding
        
        std::cerr 
        << "DEBUG: vertex_set.size()=" << vertex_set.size()
        << "  num_vertices="    << num_vertices
        << "  vertex_map.size()="<< vertex_map.size()
        << "\n";

        for(size_t i = 0; i < v_keys.size(); ++ i)
        {
            std::cerr 
                << "  key["<<i<<"]="<< v_keys[i] 
                << "  maps to pos="<< vertexID2SetPos(v_keys[i])
                << "\n";
        }

        //
        for(size_t i = 0; i < v_keys.size(); ++ i)
        {
            std::cout << "VertexID-Position:\t" << v_keys[i] << "\t" << vertexID2SetPos(v_keys[i]) << std::endl;
        }
        return;
    }

    // prints the content in edge_map
    void printInfoEdgeMap(void) 
    {
        MyVector<EdgeIDType> e_keys;
        edge_map.keys(e_keys);
        for(size_t i = 0; i < e_keys.size(); ++ i)
        {
            std::cout << "EdgeID-Position:\t" << e_keys[i] << "\t" << edgeID2SetPos(e_keys[i]) << std::endl;
        }
        return;
    }

    // prints the content in the adjacency list
    void printAdjList(void) 
    {
        for(size_t i = 0; i < adj_list.size(); ++ i)
        {
            std::cout << "Vertex: " << vertex_set[i]->id << " degree: " << degree(vertex_set[i]->id) << "\tIt is connected with: ";
            for(auto itr = adj_list[i]->begin(); itr != adj_list[i]->end(); ++ itr)
            {
                if(vertex_set[i]->id == getEdge(*itr)->src)
                    std::cout << getEdge(*itr)->tgt << "\t";
                else if(vertex_set[i]->id == getEdge(*itr)->tgt)
                    std::cout << getEdge(*itr)->src << "\t";
                else
                    throw std::domain_error("MyGraph::printAdjList: edge information may have been corrupted.");
            }
            std::cout << std::endl;
        }
    }

  private:
    VertexIDType current_vertex_ID;                 // the ID to be assigned to the next vertex (ID are assigned based on the order of insertion)
    EdgeIDType current_edge_ID;                     // the ID to be assigned to the next edge (IDs are assigned based on the order of insertion)
    size_t num_vertices;                            // the number of vertices
    size_t num_edges;                               // the number of edges
    MyVector<Vertex*> vertex_set;                   // the set of vertices
    MyVector<Edge*> edge_set;                       // the set of edges
    MyHashTable<VertexIDType, size_t> vertex_map;   // the mapping between a vertex ID and its index in vertex_set
    MyHashTable<EdgeIDType, size_t> edge_map;       // the mapping between an edge ID and its index in vertex_set
    MyVector<MyLinkedList<EdgeIDType>* > adj_list;  // the adjacency list (stores the IDs for the corresponding edges)

    // maps the vertex ID to its position in the vertex_set array
    size_t vertexID2SetPos(const VertexIDType vid)
    {
        // code begins
        size_t pos;
        std::cerr << "DEBUG: Lookin gup vertex id = " <<vid  << " and total len of " << num_vertices << std::endl;
        if(!vertex_map.get(vid,pos))
        {
            throw std::out_of_range("MyGraph vertexID2SetPos: vertex ID not found");
        }
        return pos;
        // code ends
    }

    // maps the edge ID to its position in the edge_set array
    size_t edgeID2SetPos(const VertexIDType eid)
    {
        // code begins
        size_t pos;
        if(!edge_map.get(eid, pos))
        {
            throw std::out_of_range("MyGraph edgeID2SetPos: edge ID not found");
        }
        return pos;
        // code ends
    }

    // gets the vertex ID for one in a given position in the vertex_set array
    VertexIDType vertexSetPos2ID(const size_t vpos)
    {
        // code begins
        std::cerr << "DEBUG: vpos = " << vpos << ", num_vertices = " << num_vertices <<std::endl;
        if(vpos >= num_vertices)
        {
            throw std::out_of_range("MyGraph vertexSetPos2ID: vpos >= num_vertices");
        }
        return vertex_set[vpos]->id;
        // code ends
    }

    // gets the edge ID for one in a given position in the edge_set array
    EdgeIDType edgeSetPos2ID(const size_t epos)
    {
        // code begins
        if(epos >= num_edges)
        {
            throw std::out_of_range("MyGraph edgeSetPos2ID: epos > num_edges");
        }
        return edge_set[epos]->id;
        // code ends
    }
  
};

#endif  // _MYGRAPH_H_