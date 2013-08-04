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
#include <exception> // exception
#include <stddef.h> // ptrdiff_t
#include <cassert> // assert
#include <cstddef> // size_t
#include <utility> // make_pair, pair
#include <vector> // vector
#include <stack> // stack
#include <queue> // heap


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

	/**
	* <your documentation>
	*/
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

	/**
	* <your documentation>
	*/
	friend vertex_descriptor add_vertex (Graph& graph) 
	{
		graph.g.push_back(std::vector<vertex_descriptor>());
		graph.vertices.push_back(graph.g.size());
		return graph.g.size();
	}

	// -----------------
	// adjacent_vertices
	// -----------------

	/**
	* <your documentation>
	*/
	friend std::pair<adjacency_iterator, adjacency_iterator> adjacent_vertices (vertex_descriptor source, const Graph& graph) 
	{
		adjacency_iterator b = graph.g[source - 1].begin();
		adjacency_iterator e = graph.g[source - 1].end();
		return std::make_pair(b, e);
	}

	// ----
	// edge
	// ----

	/**
	* <your documentation>
	*/
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

	/**
	* <your documentation>
	*/
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

	/**
	* <your documentation>
	*/
	friend edges_size_type num_edges (const Graph& graph) 
	{
		return graph.edgesize;
	}

	// ------------
	// num_vertices
	// ------------

	/**
	* <your documentation>
	*/
	friend vertices_size_type num_vertices (const Graph& graph) 
	{
		return graph.g.size();
	}

	// ------
	// source
	// ------

	/**
	* <your documentation>
	*/
	friend vertex_descriptor source (edge_descriptor edge, const Graph& graph) 
	{
		return edge.first;
	}

	// ------
	// target
	// ------

	/**
	* <your documentation>
	*/
	friend vertex_descriptor target (edge_descriptor edge, const Graph& graph) 
	{
		return graph.g[edge.first - 1][edge.second];
	}

	// ------
	// vertex
	// ------

	/**
	* <your documentation>
	*/
	friend vertex_descriptor vertex (vertices_size_type index, const Graph& graph) 
	{
		if(index >= graph.g.size())
			throw new std::out_of_range("vector<T> too long");
		return index + 1;
	}

	// --------
	// vertices
	// --------

	/**
	* <your documentation>
	*/
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

	/**
	* <your documentation>
	*/
	bool valid () const 
	{
		// <your code>
		return true;
	}

public:
	// ------------
	// constructors
	// ------------

	/**
	* <your documentation>
	*/
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

/**
* depth-first traversal
* three colors
* <your documentation>
*/
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

/**
* depth-first traversal
* two colors
* <your documentation>
* @throws Boost's not_a_dag exception if has_cycle()
*/
template <typename G, typename OI>
void topological_sort (const G& graph, OI x) 
{
	if(has_cycle(graph))
		boost::throw_exception(std::domain_error("The graph must be a DAG."));

	std::priority_queue< typename G::vertex_descriptor, vector<typename G::vertex_descriptor> > min_heap;
	std::vector<typename G::vertex_descriptor> visited;
	size_t nodes = num_vertices(graph);
	while(nodes != 0)
	{
		std::pair<typename G::vertex_iterator, typename G::vertex_iterator> v = vertices(graph);
		while(v.first != v.second)
		{
			if(find(visited.begin(), visited.end(), *v.first) == visited.cend())
			{
				std::pair<typename G::adjacency_iterator, typename G::adjacency_iterator> av = adjacent_vertices(*v.first, graph);
				size_t requirements = av.second - av.first;
				while(av.first != av.second)
				{
					if(find(visited.begin(), visited.end(), *av.first) != visited.cend())
						--requirements;
					++av.first;
				}

				if(requirements == 0)
				{
					min_heap.push(*v.first);
					visited.push_back(*v.first);
					break;
				}
			}
			++v.first;
		}

		if(!min_heap.empty())
		{
			*x = min_heap.top() - 1;
			++x;
			min_heap.pop();
			--nodes;
		}
	}
}

#endif // Graph_h