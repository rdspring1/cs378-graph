// ----------------------
// projects/graph/Graph.h
// Copyright (C) 2013
// Glenn P. Downing
// ----------------------

#ifndef Graph_h
#define Graph_h

// --------
// includes
// --------
#include <boost/throw_exception.hpp>
#include <boost/graph/exception.hpp>
#include <exception> // exception
#include <stddef.h> // ptrdiff_t
#include <cassert> // assert
#include <cstddef> // size_t
#include <utility> // make_pair, pair
#include <vector> // vector
#include <stack> // stack
#include <queue> // heap
#include <functional> // greater


// -----
// Graph
// -----

///
/// A class designed to represent a directed graph
///
class Graph 
{
public:
	// --------
	// typedefs
	// --------

	typedef std::size_t vertices_size_type;
	typedef std::size_t edges_size_type;

	typedef std::size_t vertex_descriptor;
	typedef std::pair<vertex_descriptor, edges_size_type> edge_descriptor; // source, offset

	typedef std::vector<vertex_descriptor>::const_iterator vertex_iterator;
	typedef std::vector<vertex_descriptor>::const_iterator adjacency_iterator;


public:
	// --------
	// edge_iterator
	// --------

	///
	/// A modifiable bidirectional edge_iterator for the Graph class
	///
	class edge_iterator 
	{
	public:
		// --------
		// typedefs
		// --------

		typedef std::bidirectional_iterator_tag		iterator_category;
		typedef edge_descriptor		value_type;
		typedef ptrdiff_t			difference_type;
		typedef edge_descriptor*	pointer;
		typedef edge_descriptor&	reference;

	public:
		// -----------
		// operator ==
		// -----------

		/**
		* equal operator
		* @param lhs - the left hand side Iterator
		* @param rhs - the right hand side Iterator
		* @return true if the lhs Iterator is equal to the rhs Iterator
		*/
		friend bool operator == (const edge_iterator& lhs, const edge_iterator& rhs) 
		{
			return (lhs._g == rhs._g) && (lhs._index == rhs._index);
		}

		/**
		* not equal operator
		* @param lhs - the left hand side Iterator
		* @param rhs - the right hand side Iterator
		* @return true if the lhs Iterator is not equal to the rhs Iterator
		*/
		friend bool operator != (const edge_iterator& lhs, const edge_iterator& rhs) 
		{
			return !(lhs == rhs);
		}

		// ----------
		// operator +
		// ----------

		/**
		* addition operator
		* @param lhs - the left hand side Iterator
		* @param rhs - the right hand side difference_type
		* @return an Iterator shifted forward by the difference_type value
		*/
		friend edge_iterator operator + (edge_iterator lhs, difference_type rhs) 
		{
			return lhs += rhs;
		}

		// ----------
		// operator -
		// ----------

		/**
		* subtraction operator
		* @param lhs - the left hand side Iterator
		* @param rhs - the right hand side difference_type
		* @return an Iterator shifted backward by the difference_type value
		*/
		friend edge_iterator operator - (edge_iterator lhs, difference_type rhs) 
		{
			return lhs -= rhs;
		}

	private:
		// ----
		// data
		// ----
		const Graph* _g;
		edges_size_type    _index;

	private:
		// -----
		// valid
		// -----

		/**
		* @return true if the Iterator object is in a valid state
		*/
		bool valid () const 
		{
			return (_g == nullptr && _index == 0) || _index >= 0;
		}

	public:
		// -----------
		// constructor
		// -----------

		/**
		* Create an Iterator object using the MyDeque container
		* @param p - a pointer to the MyDeque container
		* @param i - index state for the Iterator
		*/
		edge_iterator (const Graph* g = nullptr, std::size_t i = 0) : _g(g), _index(i)
		{
			assert(valid());
		}

		// Default copy, destructor, and copy assignment.
		// edge_iterator (const edge_iterator&);
		// ~edge_iterator ();
		// edge_iterator& operator = (const edge_iterator&);

		// ----------
		// operator *
		// ----------

		/**
		* dereference operator
		* @return an edge descriptor for the Iterator's current state
		*/
		value_type operator * () const 
		{
			assert(_index <= _g->edgesize); 
			edges_size_type offset = _index;
			vertex_descriptor source = 0;
			while(offset >= _g->g[source].size())
			{
				offset -= _g->g[source].size();
				++source;
			}
			return std::make_pair(source + 1, offset);
		}

		// -----------
		// operator ->
		// -----------

		/**
		* pointer member access operator
		* @return an edge descriptor for the Iterator's current state
		*/
		value_type operator -> () const 
		{
			return **this;
		}

		// -----------
		// operator ++
		// -----------

		/**
		* Pre-increment Operator
		* @return a Iterator reference incremented by 1
		*/
		edge_iterator& operator ++ () 
		{
			++_index;
			assert(valid());
			return *this;
		}

		/**
		* Post-Increment Operator
		* Does not effect the Iterator argument
		* @return an Iterator incremented by 1
		*/
		edge_iterator operator ++ (int) 
		{
			edge_iterator x = *this;
			++(*this);
			assert(valid());
			return x;
		}

		// -----------
		// operator --
		// -----------

		/**
		* Pre-decrement Operator
		* @return a Iterator reference decremented by 1
		*/
		edge_iterator& operator -- () 
		{
			--_index;
			assert(valid());
			return *this;
		}

		/**
		* Post-Decrement Operator
		* Does not effect the Iterator argument
		* @return an Iterator decremented by 1
		*/
		edge_iterator operator -- (int) 
		{
			edge_iterator x = *this;
			--(*this);
			assert(valid());
			return x;
		}

		// -----------
		// operator +=
		// -----------

		/**
		* Addition Assignent Operator
		* @param d - the right hand side difference type
		* @return a Iterator reference shifted forward by the difference_type value
		*/
		edge_iterator& operator += (difference_type d) 
		{
			_index += d;
			assert(valid());
			return *this;
		}

		// -----------
		// operator -=
		// -----------

		/**
		* Subtraction Assignent Operator
		* @param d - the right hand side difference type
		* @return a Iterator reference shifted backward by the difference_type value
		*/
		edge_iterator& operator -= (difference_type d) 
		{
			_index -= d;
			assert(valid());
			return *this;
		}
	};

public:
	// --------
	// add_edge
	// --------

	///
    /// Add an edge between a source and target vertex to the graph
    /// @param source - a vertex descriptor for the source vertex
    /// @param target - a vertex descriptor for the target vertex
    /// @param graph - a graph
    /// @return a std::pair<edge_descriptor, bool> - The edge_descriptor points to a new edge if the add_edge function was successful. Otherwise, the edge_descriptor points to the old edge already present in the graph. The bool value is true if the edge was successfully added. Otherwise, the bool value is false. 
    ///
	friend std::pair<edge_descriptor, bool> add_edge (vertex_descriptor source, vertex_descriptor target, Graph& graph) 
	{
		if(source > graph.g.size())
			throw std::out_of_range("vector<T> too long");

		std::pair<edge_descriptor, bool> isPresent = edge(source, target, graph);
		if(isPresent.second)
		{
			return std::make_pair(isPresent.first, false);
		}
		else
		{
			++graph.edgesize;
			edge_descriptor ed = std::make_pair(source, graph.g[source - 1].size());
			graph.g[source - 1].push_back(target);
			return std::make_pair(ed, true);
		}
	}

	// ----------
	// add_vertex
	// ----------

	///
    /// Add a vertex to the graph
    /// @param graph - a graph
    /// @return a vertex_descriptor representing the new vertex
    ///
	friend vertex_descriptor add_vertex (Graph& graph) 
	{
		graph.g.push_back(std::vector<vertex_descriptor>());
		graph.vertices.push_back(graph.g.size());
		return graph.g.size();
	}

	// -----------------
	// adjacent_vertices
	// -----------------

	///
    /// Provide access to the adjacent vertices to the source vertex in the graph
    /// For example, if an edge from vertex u to vertex v exists in the graph, the vertex v is an adjacent vertex.
    /// @param source - a vertex descriptor for the source vertex
    /// @param graph - a graph
    /// @return an iterator range representing the vertices adjacent to the source vertex in the graph
    ///
	friend std::pair<adjacency_iterator, adjacency_iterator> adjacent_vertices (vertex_descriptor source, const Graph& graph) 
	{
		adjacency_iterator b = graph.g[source - 1].begin();
		adjacency_iterator e = graph.g[source - 1].end();
		return std::make_pair(b, e);
	}

	// ----
	// edge
	// ----

	///
    /// Find the edge between a source and target vertex to the graph
    /// @param source - a vertex descriptor for the source vertex
    /// @param target - a vertex descriptor for the target vertex
    /// @param graph - a graph
    /// @return a std::pair<edge_descriptor, bool> - If there exists an edge from vertex source to vertex target in the graph, the edge_descriptor for the edge is returned. Otherwise, the default edge descriptor is returned. The bool value is true if the edge is present. Otherwise, the bool value is false. 
    ///
	friend std::pair<edge_descriptor, bool> edge (vertex_descriptor source, vertex_descriptor target, const Graph& graph) 
	{
		edges_size_type offset = 0;
		for(vertex_descriptor i : graph.g[source - 1])
		{
			if(i == target)
			{
				return std::make_pair(std::make_pair(source, offset), true);
			}
			++offset;
		}
		return std::make_pair(edge_descriptor(), false);
	}

	// -----
	// edges
	// -----

	///
    /// Provide access to the edges in the graph
    /// @param graph - a graph
    /// @return an iterator range representing the edges in the graph
    ///
	friend std::pair<edge_iterator, edge_iterator> edges (const Graph& graph) 
	{
		// <your code>
		edge_iterator b(&graph, 0);
		edge_iterator e(&graph, graph.edgesize);
		return std::make_pair(b, e);
	}

	// ---------
	// num_edges
	// ---------

	///
    /// Determine the number of edges in the graph
    /// @param graph - a graph
    /// @return the number of edges in the graph
    ///
	friend edges_size_type num_edges (const Graph& graph) 
	{
		return graph.edgesize;
	}

	// ------------
	// num_vertices
	// ------------

	///
    /// Determine the number of vertices in the graph
    /// @param graph - a graph
    /// @return the number of vertices in the graph
    ///
	friend vertices_size_type num_vertices (const Graph& graph) 
	{
		return graph.g.size();
	}

	// ------
	// source
	// ------

    ///
    /// Access the source vertex of an edge in a graph
    /// @param edge - the edge descriptor representing the edge in the graph
    /// @param graph - a graph
    /// @return the vertex descriptor of the source vertex
    ///
	friend vertex_descriptor source (edge_descriptor edge, const Graph& graph) 
	{
		return edge.first;
	}

	// ------
	// target
	// ------

    ///
    /// Access the target vertex of an edge in a graph
    /// @param edge - the edge descriptor representing the edge in the graph
    /// @param graph - a graph
    /// @return the vertex descriptor of the target vertex
    ///
	friend vertex_descriptor target (edge_descriptor edge, const Graph& graph) 
	{
		return graph.g[edge.first - 1][edge.second];
	}

	// ------
	// vertex
	// ------

    ///
    /// Access the nth vertex of the graph
    /// @param index - the nth position in the graph's vertex list
    /// @param graph - a graph
    /// @return the vertex descriptor of the nth vertex in the graph's vertex list
    ///
	friend vertex_descriptor vertex (vertices_size_type index, const Graph& graph) 
	{
		if(index >= graph.g.size())
			throw new std::out_of_range("vector<T> too long");
		return index + 1;
	}

	// --------
	// vertices
	// --------

    ///
    /// Provide access to the vertices in the graph
    /// @param graph - a graph
    /// @return an iterator range representing the vertices in the graph
    ///
	friend std::pair<vertex_iterator, vertex_iterator> vertices (const Graph& graph) 
	{
		vertex_iterator b = graph.vertices.begin();
		vertex_iterator e = graph.vertices.end();
		return std::make_pair(b, e);
	}

private:
	// ----
	// data
	// ----
	edges_size_type edgesize;
	std::vector<vertex_descriptor> vertices; // Vertex List
	std::vector<std::vector<vertex_descriptor> > g; // Adjacency List

	// -----
	// valid
	// -----

	///
	/// @return true if the Graph object is in a valid state
	///
	bool valid () const 
	{
		return (edgesize == 0 && vertices.size() == 0 && g.size() == 0) || (edgesize >= 0 && vertices.size() == g.size() && vertices.size() > 0 && g.size() > 0);
	}

public:
	// ------------
	// constructors
	// ------------

    ///
	/// Default Constructor - Empty Graph
	///
	Graph () : edgesize(0)
	{
		assert(valid());
	}

	// Default copy, destructor, and copy assignment
	// Graph  (const Graph<T>&);
	// ~Graph ();
	// Graph& operator = (const Graph&);
};

// ---------
// has_cycle
// ---------

///
/// Determine whether the graph contains a cycle
/// A cycle is a sequence of vertices starting and ending at the same vertex
/// depth-first traversal
/// three colors
/// @tparam G - Graph Class Template
/// @param graph - a graph
/// @return true if the graph contains a cycle; Otherwise, false
///
template <typename G>
bool has_cycle (const G& graph) 
{
	std::pair<typename G::vertex_iterator, typename G::vertex_iterator> v = vertices(graph);
	while(v.first != v.second)
	{
		std::vector<bool> explored(num_vertices(graph) + 1);
		std::stack<typename G::vertex_descriptor> vlist;
		vlist.push(*v.first);
		while(!vlist.empty())
		{
			std::pair<typename G::adjacency_iterator, typename G::adjacency_iterator> av = adjacent_vertices(vlist.top(), graph);
			explored[vlist.top()] = true;
			vlist.pop();
			while(av.first != av.second)
			{
				if(!explored[*av.first])
					vlist.push(*av.first);
				if(*v.first == *av.first)
					return true;
				++av.first;
			}
		}
		++v.first;
	}
	return false;
}

// ----------------
// topological_sort
// ----------------

///
/// depth-first traversal
/// two colors
/// Generate a topological sort for the directed, acyclic graph
/// @tparam G - Graph Class Template
/// @tparam OI - Output Iterator Template
/// @param graph - graph
/// @param x - an output iterator
/// @throws Boost's not_a_dag exception if has_cycle()
///
template <typename G, typename OI>
void topological_sort (const G& graph, OI x) 
{
	if(has_cycle(graph))
		boost::throw_exception(boost::not_a_dag());

	std::vector<bool> visited(num_vertices(graph) + 1);
	std::pair<typename G::vertex_iterator, typename G::vertex_iterator> v = vertices(graph);
	while(v.first != v.second)
	{
		if(!visited[*v.first])
			depth_search(graph, x, *v.first, visited);
		++v.first;
	}
}

///
/// A helper function for the topological sort function
/// This function executes a depth-first search of the graph
/// The function recursively executes, following the requirement vertices for the current vertex
/// The function processes the vertices in ascending order of their vertex number
/// @tparam G - Graph Class Template
/// @tparam OI - Output Iterator Template
/// @param graph - graph
/// @param x - an output iterator
/// @param v -  the vertex descriptor representing the current vertex
/// @param visited - a std::vector marking which vertices of the graph are visited by the depth first-search
///
template <typename G, typename OI>
void depth_search (const G& graph, OI x, const typename G::vertex_descriptor& v, std::vector<bool>& visited) 
{
	std::pair<typename G::adjacency_iterator, typename G::adjacency_iterator> av = adjacent_vertices(v, graph);
	std::priority_queue< typename G::vertex_descriptor, std::vector<typename G::vertex_descriptor>, std::greater<typename G::vertex_descriptor> > min_heap;
	while(av.first != av.second)
	{
		if(!visited[*av.first])
			min_heap.push(*av.first);
		++av.first;
	}

	while(!min_heap.empty())
	{
		if(!visited[min_heap.top()])
		{
			depth_search(graph, x, min_heap.top(), visited);
		}
		min_heap.pop();
	}
	visited[v] = true;
	*x = v - 1;
	++x;
}

#endif // Graph_h