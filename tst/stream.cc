#include<catch.hpp>
#include<fstream>
#include<iostream>
#include<complex>
#include"gtk/drawable.h"
#include"src/graph.h"
#include"src/nodeexpr.h"
#include"gtk/graphview.h"
#include"gtk/sketch.h"
using namespace std;

TEST_CASE("complex stream") {
//	complex<double> im = 1.0 + 1i;
//	stringstream ss;
//	ss << "(3, 2)";
//	im = 3 + 2i;
//	REQUIRE(im == (3. + 2i));
}

TEST_CASE("graph") {
	Graph<int> g;
	g.insert_vertex(1);
	g.insert_vertex(2);
	g.insert_vertex(3);
	g.insert_vertex(4);
	g.insert_vertex(5);
	g.insert_vertex(6);
	g.insert_vertex(7);
	g.insert_edge(1, 2, 3);
	g.insert_edge(1, 3, 2);
	g.insert_edge(2, 6, 2);
	g.insert_edge(6, 5, 2);
	g.insert_edge(3, 1, 0);
	g.insert_edge(4, 5, 1);
	g.remove_edge(1, 3);
	g.remove_vertex(1);
	g.insert_vertex(1);
	g.insert_edge(2, 1, 0);
	g.sub_apply(2, [](int& k) { k+=10; });
	stringstream ss;
	ss << g;
	REQUIRE(ss.str() == "7\n2\n3\n4\n15\n16\n7\n11\n\n2\n16\n(2,0)\n2\n11\n(0,0)\n4\n15\n(1,0)\n16\n15\n(2,0)\n");
	Graph<int> h;
	ss >> h;
	REQUIRE(h.data()->data == 2);
}



TEST_CASE("Korean") {
	GraphView<string> gr;
	string s[3] = {"한국", "Hello", "fhksalfd"};
	gr.insert_vertex(s[0]);
	gr.insert_vertex(s[1]);
	gr.insert_vertex(s[2]);
	gr.set_width_height(s[0], 50, 30);
//	gr.set_show(1, false);
	gr.insert_edge(s[0],s[1], {150, 100});
	ofstream f("mindmap.txt");
	f << gr;
//	auto** a = gr.generate_drawables(string{"hang"}, {300,300});

}
TEST_CASE("node stream") {
	ifstream f("mindmap2.txt");
	NodeExpr<string> g1, g2;
	f >> g1 >> g2;
	cout << g1 << g2;
}
TEST_CASE("mm") {
	ifstream f("mm.txt");
	GraphView<string> g;
	f >> g;
}

TEST_CASE("File ss") {
	File a;
	a.name = "haha";
	a.full_path = "fjskadl/fjasl";
	cout << a;
	stringstream ss;
	ss << a;
	cout << ss.str() << endl;
}

TEST_CASE("file system") {
	File f{"."};
	cout << "File status :" << f;
}
