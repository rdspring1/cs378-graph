// ----------------------------
// projects/graph/TestGraph.c++
// Copyright (C) 2013
// Glenn P. Downing
// ----------------------------

/*
To test the program:
    % locate libcppunit.a
    /usr/lib/libcppunit.a
    % ls /public/linux/include/boost-1_44/boost/graph/
    ...
    adjacency_list.hpp
    ...
    % g++ -pedantic -std=c++0x -Wall TestGraph.c++ -o TestGraph -lgtest -lpthread -lgtest_main
    % valgrind ./TestGraph >& TestGraph.c++.out
*/

#define private public
#define protected public

// --------
// includes
// --------

#include <stddef.h>
#include <iostream> // cout, endl
#include <iterator> // ostream_iterator
#include <sstream>  // ostringstream
#include <utility>  // pair

#include "boost/graph/adjacency_list.hpp"  // adjacency_list
#include "boost/graph/topological_sort.hpp"// topological_sort
#include "gtest/gtest.h"
#include "Graph.h"

using namespace std;

typedef testing::Types<boost::adjacency_list<boost::setS, boost::vecS, boost::directedS>, Graph > testlist;

template <typename T>
class TestGraph :  public testing::Test
{
	public:
		// --------
		// typedefs
		// --------

		typedef T										graph_type;

		typedef typename graph_type::vertex_descriptor  vertex_descriptor;
		typedef typename graph_type::edge_descriptor    edge_descriptor;

		typedef typename graph_type::vertex_iterator    vertex_iterator;
		typedef typename graph_type::edge_iterator      edge_iterator;
		typedef typename graph_type::adjacency_iterator adjacency_iterator;

		typedef typename graph_type::vertices_size_type vertices_size_type;
		typedef typename graph_type::edges_size_type    edges_size_type;

		// directed, sparse, unweighted
		// possibly connected
		// possibly cyclic
		graph_type g;

		vertex_descriptor vdA;
		vertex_descriptor vdB;
		vertex_descriptor vdC;
		vertex_descriptor vdD;
		vertex_descriptor vdE;
		vertex_descriptor vdF;
		vertex_descriptor vdG;
		vertex_descriptor vdH;

		edge_descriptor edAB;
		edge_descriptor edAC;
		edge_descriptor edAE;
		edge_descriptor edBD;
		edge_descriptor edBE;
		edge_descriptor edCD;
		edge_descriptor edDE;
		edge_descriptor edDF;
		edge_descriptor edFD;
		edge_descriptor edFH;
		edge_descriptor edGH;

		// -----
		// setUp
		// -----

		virtual void SetUp () 
		{
			vdA  = add_vertex(g);
			vdB  = add_vertex(g);
			vdC  = add_vertex(g);
			vdD  = add_vertex(g);
			vdE  = add_vertex(g);
			vdF  = add_vertex(g);
			vdG  = add_vertex(g);
			vdH  = add_vertex(g);
			edAB = add_edge(vdA, vdB, g).first;
			edAC = add_edge(vdA, vdC, g).first;
			edAE = add_edge(vdA, vdE, g).first;
			edBD = add_edge(vdB, vdD, g).first;
			edBE = add_edge(vdB, vdE, g).first;
			edCD = add_edge(vdC, vdD, g).first;
			edDE = add_edge(vdD, vdE, g).first;
			edDF = add_edge(vdD, vdF, g).first;
			edFD = add_edge(vdF, vdD, g).first;
			edFH = add_edge(vdF, vdH, g).first;
			edGH = add_edge(vdG, vdH, g).first;
		}
};

TYPED_TEST_CASE(TestGraph, testlist);

// -------------
// test_add_edge
// -------------

TYPED_TEST(TestGraph, test_add_edge)
{
        std::pair<edge_descriptor, bool> p = add_edge(vdA, vdB, g);
        ASSERT_EQ(p.first , edAB);
        ASSERT_EQ(p.second, false);
}

// ----------------------
// test_adjacent_vertices
// ----------------------

TYPED_TEST(TestGraph, test_adjacent_vertices)
{
    std::pair<adjacency_iterator, adjacency_iterator> p = adjacent_vertices(vdA, g);
    adjacency_iterator b = p.first;
    adjacency_iterator e = p.second;
    ASSERT_TRUE(b != e);
    if (b != e) 
	{
        vertex_descriptor vd = *b;
        ASSERT_EQ(vd, vdB);
	}
    ++b;
    if (b != e) 
	{
        vertex_descriptor vd = *b;
        ASSERT_EQ(vd, vdC);
	}
}

// ---------
// test_edge
// ---------

TYPED_TEST(TestGraph, test_edge) 
{
    std::pair<edge_descriptor, bool> p = edge(vdA, vdB, g);
    ASSERT_EQ(p.first , edAB);
    ASSERT_EQ(p.second, true);
}

// ----------
// test_edges
// ----------

TYPED_TEST(TestGraph, test_edges)
{
    std::pair<edge_iterator, edge_iterator> p = edges(g);
    edge_iterator                           b = p.first;
    edge_iterator                           e = p.second;
    ASSERT_TRUE(b != e);
    if (b != e) 
	{
        edge_descriptor ed = *b;
        ASSERT_EQ(ed, edAB);
	}
    ++b;
    if (b != e) 
	{
        edge_descriptor ed = *b;
        ASSERT_EQ(ed, edAC);
	}
}

// --------------
// test_num_edges
// --------------

TYPED_TEST(TestGraph, test_num_edges) 
{
    edges_size_type es = num_edges(g);
    ASSERT_EQ(es, 11);
}

// -----------------
// test_num_vertices
// -----------------

TYPED_TEST(TestGraph, test_num_vertices)
{
    vertices_size_type vs = num_vertices(g);
    ASSERT_EQ(vs, 8);
}

// -----------
// test_source
// -----------

TYPED_TEST(TestGraph, test_source) 
{
    vertex_descriptor vd = source(edAB, g);
    ASSERT_EQ(vd, vdA);
}

// -----------
// test_target
// -----------

TYPED_TEST(TestGraph, test_target) 
{
    vertex_descriptor vd = target(edAB, g);
    ASSERT_EQ(vd, vdB);
}

// -----------
// test_vertex
// -----------

TYPED_TEST(TestGraph, test_vertex) 
{
    vertex_descriptor vd = vertex(0, g);
    ASSERT_EQ(vd, vdA);
}

// -------------
// test_vertices
// -------------

TYPED_TEST(TestGraph, test_vertices) 
{
    std::pair<vertex_iterator, vertex_iterator> p = vertices(g);
    vertex_iterator                             b = p.first;
    vertex_iterator                             e = p.second;
    ASSERT_TRUE(b != e);
    if (b != e) 
	{
        vertex_descriptor vd = *b;
        ASSERT_EQ(vd, vdA);
	}
    ++b;
    if (b != e) 
	{
        vertex_descriptor vd = *b;
        ASSERT_EQ(vd, vdB);
	}
}

// --------------
// test_has_cycle
// --------------

TYPED_TEST(TestGraph, test_has_cycle) 
{
    ASSERT_TRUE(has_cycle(g));
}

// ---------------------
// test_topological_sort
// ---------------------

TYPED_TEST(TestGraph, test_topological_sort) 
{
    std::ostringstream out;
    topological_sort(g, std::ostream_iterator<vertex_descriptor>(out, " "));
    ASSERT_EQ(out.str(), "2 0 1 ");
}