#include<catch.hpp>
#include<fstream>
#include<sstream>
#include<iostream>
#include<complex>
#include<json/json.h>
#include"src/nodeexpr.h"
using namespace std;

TEST_CASE("imageinary") {
	complex<double> im;
	stringstream ss;
	ss << "(100,3.2)";
	ss >> im;
	REQUIRE(im == complex<double>{100, 3.2});
}


struct A {
	virtual ~A() { cout << "A" << endl; }
};
struct B : A {
	//virtual ~B() {cout << 'B' << endl; }
};
struct C : B {
	~C() { cout << 'C' << endl; }
};


TEST_CASE("virtual destructor") {
	B* p = new C{};//{{2,1},{3,4}, "tst"};
	delete p;
}

const char* cc = R"({ "text" : 3}, { "text" : 2 })";

TEST_CASE("json stream") {
	Json::Value jv1, jv2;
	stringstream ss; ss << cc;
	ss >> jv1 ;
//	cout << "jv1 : " << jv1 << endl << "jv2 :" << jv2 << endl;
}
TEST_CASE("get_line") {
	ifstream f("mm.txt");
	int k; string s;
	f >> k;
	while(get_line(f, s)) {
		cout << s << endl;
		f >> s;
	}
}

ostream& operator<<(ostream& o, const A& a) {return o;}
stringstream& operator<<(stringstream& o, const A& a) {return o;}
TEST_CASE("overload") {
}

