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

#include <boost/exception/all.hpp> // boost exception
#include "boost/graph/adjacency_list.hpp"  // adjacency_list
#include "boost/graph/topological_sort.hpp"// topological_sort

#include "gtest/gtest.h"

#include "Graph.h"

using namespace std;

typedef boost::error_info<struct tag_errmsg, std::string> errmsg_info; 
typedef testing::Types<boost::adjacency_list<boost::setS, boost::vecS, boost::directedS> > testlist;

template <typename T>
class TestGraphSample :  public testing::Test
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
			vdA  = add_vertex(this->g);
			vdB  = add_vertex(this->g);
			vdC  = add_vertex(this->g);
			vdD  = add_vertex(this->g);
			vdE  = add_vertex(this->g);
			vdF  = add_vertex(this->g);
			vdG  = add_vertex(this->g);
			vdH  = add_vertex(this->g);
			edAB = add_edge(vdA, vdB, this->g).first;
			edAC = add_edge(vdA, vdC, this->g).first;
			edAE = add_edge(vdA, vdE, this->g).first;
			edBD = add_edge(vdB, vdD, this->g).first;
			edBE = add_edge(vdB, vdE, this->g).first;
			edCD = add_edge(vdC, vdD, this->g).first;
			edDE = add_edge(vdD, vdE, this->g).first;
			edDF = add_edge(vdD, vdF, this->g).first;
			edFD = add_edge(vdF, vdD, this->g).first;
			edFH = add_edge(vdF, vdH, this->g).first;
			edGH = add_edge(vdG, vdH, this->g).first;
		}
};

template <typename T>
class TestGraphBasic :  public testing::Test
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

		edge_descriptor edAC;
		edge_descriptor edDB;
		edge_descriptor edDC;
		edge_descriptor edEB;
		edge_descriptor edFB;
		edge_descriptor edFC;
		edge_descriptor edGC;

		// -----
		// setUp
		// -----

		virtual void SetUp () 
		{
			vdA  = add_vertex(this->g);
			vdB  = add_vertex(this->g);
			vdC  = add_vertex(this->g);
			vdD  = add_vertex(this->g);
			vdE  = add_vertex(this->g);
			vdF  = add_vertex(this->g);
			vdG  = add_vertex(this->g);
			edAC = add_edge(vdA, vdC, this->g).first;
			edDB = add_edge(vdD, vdB, this->g).first;
			edDC = add_edge(vdD, vdC, this->g).first;
			edEB = add_edge(vdE, vdB, this->g).first;
			edFB = add_edge(vdF, vdB, this->g).first;
			edFC = add_edge(vdF, vdC, this->g).first;
			edGC = add_edge(vdG, vdC, this->g).first;
		}
};

TYPED_TEST_CASE(TestGraphSample, testlist);
TYPED_TEST_CASE(TestGraphBasic, testlist);

// -------------
// test_add_edge
// -------------

TYPED_TEST(TestGraphBasic, test_add_edge)
{
        std::pair<edge_descriptor, bool> p = add_edge(vdA, vdC, this->g);
        ASSERT_EQ(p.first , edAC);
        ASSERT_EQ(p.second, false);
}

// ----------------------
// test_adjacent_vertices
// ----------------------

TYPED_TEST(TestGraphBasic, test_adjacent_vertices_A)
{
    std::pair<adjacency_iterator, adjacency_iterator> p = adjacent_vertices(vdA, this->g);
    adjacency_iterator b = p.first;
    adjacency_iterator e = p.second;
    ASSERT_TRUE(b != e);
    if (b != e) 
	{
        vertex_descriptor vd = *b;
        ASSERT_EQ(vd, vdC);
	}
    ++b;
    ASSERT_TRUE(b == e);
}

TYPED_TEST(TestGraphBasic, test_adjacent_vertices_B)
{
    std::pair<adjacency_iterator, adjacency_iterator> p = adjacent_vertices(vdB, this->g);
    adjacency_iterator b = p.first;
    adjacency_iterator e = p.second;
    ASSERT_TRUE(b == e);
}

TYPED_TEST(TestGraphBasic, test_adjacent_vertices_C)
{
    std::pair<adjacency_iterator, adjacency_iterator> p = adjacent_vertices(vdC, this->g);
    adjacency_iterator b = p.first;
    adjacency_iterator e = p.second;
    ASSERT_TRUE(b == e);
}

TYPED_TEST(TestGraphBasic, test_adjacent_vertices_D)
{
    std::pair<adjacency_iterator, adjacency_iterator> p = adjacent_vertices(vdD, this->g);
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

TYPED_TEST(TestGraphBasic, test_adjacent_vertices_E)
{
    std::pair<adjacency_iterator, adjacency_iterator> p = adjacent_vertices(vdE, this->g);
    adjacency_iterator b = p.first;
    adjacency_iterator e = p.second;
    ASSERT_TRUE(b != e);
    if (b != e) 
	{
        vertex_descriptor vd = *b;
        ASSERT_EQ(vd, vdB);
	}
    ++b;
    ASSERT_TRUE(b == e);
}

TYPED_TEST(TestGraphBasic, test_adjacent_vertices_F)
{
    std::pair<adjacency_iterator, adjacency_iterator> p = adjacent_vertices(vdF, this->g);
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

TYPED_TEST(TestGraphBasic, test_adjacent_vertices_G)
{
    std::pair<adjacency_iterator, adjacency_iterator> p = adjacent_vertices(vdG, this->g);
    adjacency_iterator b = p.first;
    adjacency_iterator e = p.second;
    ASSERT_TRUE(b != e);
    if (b != e) 
	{
        vertex_descriptor vd = *b;
        ASSERT_EQ(vd, vdC);
	}
    ++b;
    ASSERT_TRUE(b == e);
}

// ---------
// test_edge
// ---------

TYPED_TEST(TestGraphBasic, test_edge_AC) 
{
    std::pair<edge_descriptor, bool> p = edge(vdA, vdC, this->g);
    ASSERT_EQ(p.first , edAC);
    ASSERT_EQ(p.second, true);
}

TYPED_TEST(TestGraphBasic, test_edge_DB) 
{
    std::pair<edge_descriptor, bool> p = edge(vdD, vdB, this->g);
    ASSERT_EQ(p.first , edDB);
    ASSERT_EQ(p.second, true);
}

TYPED_TEST(TestGraphBasic, test_edge_DC) 
{
    std::pair<edge_descriptor, bool> p = edge(vdD, vdC, this->g);
    ASSERT_EQ(p.first , edDC);
    ASSERT_EQ(p.second, true);
}

TYPED_TEST(TestGraphBasic, test_edge_EB) 
{
    std::pair<edge_descriptor, bool> p = edge(vdE, vdB, this->g);
    ASSERT_EQ(p.first , edEB);
    ASSERT_EQ(p.second, true);
}

TYPED_TEST(TestGraphBasic, test_edge_FB) 
{
    std::pair<edge_descriptor, bool> p = edge(vdF, vdB, this->g);
    ASSERT_EQ(p.first , edFB);
    ASSERT_EQ(p.second, true);
}

TYPED_TEST(TestGraphBasic, test_edge_FC) 
{
    std::pair<edge_descriptor, bool> p = edge(vdF, vdC, this->g);
    ASSERT_EQ(p.first , edFC);
    ASSERT_EQ(p.second, true);
}

TYPED_TEST(TestGraphBasic, test_edge_GC) 
{
    std::pair<edge_descriptor, bool> p = edge(vdG, vdC, this->g);
    ASSERT_EQ(p.first , edGC);
    ASSERT_EQ(p.second, true);
}

// ----------
// test_edges
// ----------

TYPED_TEST(TestGraphBasic, test_edges)
{
    std::pair<edge_iterator, edge_iterator> p = edges(this->g);
    edge_iterator                           b = p.first;
    edge_iterator                           e = p.second;
    ASSERT_TRUE(b != e);
    if (b != e) 
	{
        edge_descriptor ed = *b;
        ASSERT_EQ(ed, edAC);
	}
    ++b;
    if (b != e) 
	{
        edge_descriptor ed = *b;
        ASSERT_EQ(ed, edDB);
	}
	++b;
    if (b != e) 
	{
        edge_descriptor ed = *b;
        ASSERT_EQ(ed, edDC);
	}
	++b;
    if (b != e) 
	{
        edge_descriptor ed = *b;
        ASSERT_EQ(ed, edEB);
	}
	++b;
    if (b != e) 
	{
        edge_descriptor ed = *b;
        ASSERT_EQ(ed, edFB);
	}
	++b;
    if (b != e) 
	{
        edge_descriptor ed = *b;
        ASSERT_EQ(ed, edFC);
	}
	++b;
    if (b != e) 
	{
        edge_descriptor ed = *b;
        ASSERT_EQ(ed, edGC);
	}
	++b;
	ASSERT_TRUE(b == e);
}

// --------------
// test_num_edges
// --------------

TYPED_TEST(TestGraphBasic, test_num_edges) 
{
    edges_size_type es = num_edges(this->g);
    ASSERT_EQ(es, 7);
}

// -----------------
// test_num_vertices
// -----------------

TYPED_TEST(TestGraphBasic, test_num_vertices)
{
    vertices_size_type vs = num_vertices(this->g);
    ASSERT_EQ(vs, 7);
}

// -----------
// test_source
// -----------

TYPED_TEST(TestGraphBasic, test_source_AC) 
{
    vertex_descriptor vd = source(edAC, this->g);
    ASSERT_EQ(vd, vdA);
}

TYPED_TEST(TestGraphBasic, test_source_DB) 
{
    vertex_descriptor vd = source(edDB, this->g);
    ASSERT_EQ(vd, vdD);
}

TYPED_TEST(TestGraphBasic, test_source_DC) 
{
    vertex_descriptor vd = source(edDC, this->g);
    ASSERT_EQ(vd, vdD);
}

TYPED_TEST(TestGraphBasic, test_source_EB) 
{
    vertex_descriptor vd = source(edEB, this->g);
    ASSERT_EQ(vd, vdE);
}

TYPED_TEST(TestGraphBasic, test_source_FB) 
{
    vertex_descriptor vd = source(edFB, this->g);
    ASSERT_EQ(vd, vdF);
}

TYPED_TEST(TestGraphBasic, test_source_FC) 
{
    vertex_descriptor vd = source(edFC, this->g);
    ASSERT_EQ(vd, vdF);
}

TYPED_TEST(TestGraphBasic, test_source_GC) 
{
    vertex_descriptor vd = source(edGC, this->g);
    ASSERT_EQ(vd, vdG);
}

// -----------
// test_target
// -----------

TYPED_TEST(TestGraphBasic, test_target_AC) 
{
    vertex_descriptor vd = target(edAC, this->g);
    ASSERT_EQ(vd, vdC);
}

TYPED_TEST(TestGraphBasic, test_target_DB) 
{
    vertex_descriptor vd = target(edDB, this->g);
    ASSERT_EQ(vd, vdB);
}

TYPED_TEST(TestGraphBasic, test_target_DC) 
{
    vertex_descriptor vd = target(edDC, this->g);
    ASSERT_EQ(vd, vdC);
}

TYPED_TEST(TestGraphBasic, test_target_EB) 
{
    vertex_descriptor vd = target(edEB, this->g);
    ASSERT_EQ(vd, vdB);
}

TYPED_TEST(TestGraphBasic, test_target_FB) 
{
    vertex_descriptor vd = target(edFB, this->g);
    ASSERT_EQ(vd, vdB);
}

TYPED_TEST(TestGraphBasic, test_target_FC) 
{
    vertex_descriptor vd = target(edFC, this->g);
    ASSERT_EQ(vd, vdC);
}

TYPED_TEST(TestGraphBasic, test_target_GC) 
{
    vertex_descriptor vd = target(edGC, this->g);
    ASSERT_EQ(vd, vdC);
}

// -----------
// test_vertex
// -----------

TYPED_TEST(TestGraphBasic, test_vertex_A) 
{
    vertex_descriptor vd = vertex(0, this->g);
    ASSERT_EQ(vd, vdA);
}

TYPED_TEST(TestGraphBasic, test_vertex_B) 
{
    vertex_descriptor vd = vertex(1, this->g);
    ASSERT_EQ(vd, vdB);
}

TYPED_TEST(TestGraphBasic, test_vertex_C) 
{
    vertex_descriptor vd = vertex(2, this->g);
    ASSERT_EQ(vd, vdC);
}

TYPED_TEST(TestGraphBasic, test_vertex_D) 
{
    vertex_descriptor vd = vertex(3, this->g);
    ASSERT_EQ(vd, vdD);
}

TYPED_TEST(TestGraphBasic, test_vertex_E) 
{
    vertex_descriptor vd = vertex(4, this->g);
    ASSERT_EQ(vd, vdE);
}

TYPED_TEST(TestGraphBasic, test_vertex_F) 
{
    vertex_descriptor vd = vertex(5, this->g);
    ASSERT_EQ(vd, vdF);
}

TYPED_TEST(TestGraphBasic, test_vertex_G) 
{
    vertex_descriptor vd = vertex(6, this->g);
    ASSERT_EQ(vd, vdG);
}

// -------------
// test_vertices
// -------------

TYPED_TEST(TestGraphBasic, test_vertices) 
{
    std::pair<vertex_iterator, vertex_iterator> p = vertices(this->g);
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
	++b;
    if (b != e) 
	{
        vertex_descriptor vd = *b;
        ASSERT_EQ(vd, vdC);
	}
	++b;
    if (b != e) 
	{
        vertex_descriptor vd = *b;
        ASSERT_EQ(vd, vdD);
	}
	++b;
    if (b != e) 
	{
        vertex_descriptor vd = *b;
        ASSERT_EQ(vd, vdE);
	}
	++b;
    if (b != e) 
	{
        vertex_descriptor vd = *b;
        ASSERT_EQ(vd, vdF);
	}
	++b;
    if (b != e) 
	{
        vertex_descriptor vd = *b;
        ASSERT_EQ(vd, vdG);
	}
}

// --------------
// test_has_cycle
// --------------

TYPED_TEST(TestGraphBasic, test_has_cycle) 
{
    ASSERT_TRUE(has_cycle(this->g));
}

// ---------------------
// test_topological_sort
// ---------------------

TYPED_TEST(TestGraphBasic, test_topological_sort) 
{
    std::ostringstream out;
    topological_sort(this->g, std::ostream_iterator<vertex_descriptor>(out, " "));
    ASSERT_EQ(out.str(), "2 0 1 3 4 5 6 ");
}

// -------------
// test_add_edge
// -------------

TYPED_TEST(TestGraphSample, test_add_edge_duplicate)
{
        std::pair<edge_descriptor, bool> p = add_edge(vdA, vdB, this->g);
        ASSERT_EQ(p.first , edAB);
        ASSERT_EQ(p.second, false);
}

// ----------------------
// test_adjacent_vertices
// ----------------------

TYPED_TEST(TestGraphSample, test_adjacent_vertices)
{
    std::pair<adjacency_iterator, adjacency_iterator> p = adjacent_vertices(vdA, this->g);
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

TYPED_TEST(TestGraphSample, test_edge) 
{
    std::pair<edge_descriptor, bool> p = edge(vdA, vdB, this->g);
    ASSERT_EQ(p.first , edAB);
    ASSERT_EQ(p.second, true);
}

// ----------
// test_edges
// ----------

TYPED_TEST(TestGraphSample, test_edges)
{
    std::pair<edge_iterator, edge_iterator> p = edges(this->g);
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

TYPED_TEST(TestGraphSample, test_num_edges) 
{
    edges_size_type es = num_edges(this->g);
    ASSERT_EQ(es, 11);
}

// -----------------
// test_num_vertices
// -----------------

TYPED_TEST(TestGraphSample, test_num_vertices)
{
    vertices_size_type vs = num_vertices(this->g);
    ASSERT_EQ(vs, 8);
}

// -----------
// test_source
// -----------

TYPED_TEST(TestGraphSample, test_source) 
{
    vertex_descriptor vd = source(edAB, this->g);
    ASSERT_EQ(vd, vdA);
}

// -----------
// test_target
// -----------

TYPED_TEST(TestGraphSample, test_target) 
{
    vertex_descriptor vd = target(edAB, this->g);
    ASSERT_EQ(vd, vdB);
}

// -----------
// test_vertex
// -----------

TYPED_TEST(TestGraphSample, test_vertex) 
{
    vertex_descriptor vd = vertex(0, this->g);
    ASSERT_EQ(vd, vdA);
}

// -------------
// test_vertices
// -------------

TYPED_TEST(TestGraphSample, test_vertices) 
{
    std::pair<vertex_iterator, vertex_iterator> p = vertices(this->g);
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

TYPED_TEST(TestGraphSample, test_has_cycle) 
{
    ASSERT_TRUE(has_cycle(this->g));
}

// ---------------------
// test_topological_sort
// ---------------------

TYPED_TEST(TestGraphSample, test_topological_sort) 
{
	try
	{
		std::ostringstream out;
		topological_sort(this->g, std::ostream_iterator<vertex_descriptor>(out, " "));
		ASSERT_TRUE(false);
	}
	catch (boost::exception &e)
	{
		std::exception const * se = dynamic_cast<std::exception const *>(&e);
		ASSERT_EQ(std::strcmp(se->what(), "The graph must be a DAG."), 0);
	}
}