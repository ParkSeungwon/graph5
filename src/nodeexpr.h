#pragma once
#include<cassert>
#include<sstream>
#include<string>
#include<json/json.h>

enum class OutlineShape {Picture, Rect, Diamond, Ellipse};
enum class ArrowShape {Plant, Direct};

template<class T> struct NodeExpr
{//intervene between data and graph structure. define node shape in gui.
	T data;//should have default constructor and operator>>, <<, == overloaded
	NodeExpr() = default;
	NodeExpr(T n) { data = n; }//this will make graph functions compatible
	operator T() { return data; }
	bool operator==(const NodeExpr<T>& r) { return data == r.data; }

	bool show = true;//show this node or not
	bool shrink = false;//show sub nodes or not
	OutlineShape outline = OutlineShape::Ellipse;
	ArrowShape arrowline = ArrowShape::Direct;
	unsigned char color[3][4] = {//3 : text arrow shape, 4 : r g b a
		{0,0,0,255},{0,0,0,255},{0,0,0,255}};
	unsigned short width = 30, height = 10;
	std::string tooltip, picture_file;
};


template<class T> std::ostream& operator<<(std::ostream& o, const NodeExpr<T>& node) {
	Json::Value jv;
	jv["show"] = node.show;
	jv["shrink"] = node.shrink;
	jv["outline"] = static_cast<int>(node.outline);
	jv["arrowline"] = static_cast<int>(node.arrowline);
	jv["width"] = node.width;
	jv["height"] = node.height;
	jv["tooltip"] = node.tooltip;
	jv["picture_file"] = node.picture_file;
	for(int i=0; i<3; i++) for(int j=0; j<4; j++) jv["color"][i][j] = node.color[i][j];

	Json::FastWriter fast;//in one line
	o << fast.write(jv);
	o << node.data;
	return o;
}

std::istream& get_line(std::istream& is, std::string& s);//deal trailing white space

template<class T> std::istream& operator>>(std::istream& is, NodeExpr<T>& r) {
	std::string s; std::stringstream ss; Json::Value jv;
	if(!get_line(is, s)) return is;//to check eof
	ss << s; ss >> jv;
	is >> r.data;//data must implement operator<< and >> and print in one line

	r.show = jv["show"].asBool();
	r.shrink = jv["shrink"].asBool();
	r.arrowline = static_cast<ArrowShape>(jv["arrowline"].asInt());
	r.outline = static_cast<OutlineShape>(jv["outline"].asInt());
	for(int i=0; i<3; i++) for(int j=0; j<4; j++)
		r.color[i][j] = jv["color"][i][j].asInt();
	r.width = jv["width"].asInt();
	r.height = jv["height"].asInt();
	r.tooltip = jv["tooltip"].asString();
	r.picture_file = jv["picture_file"].asString();
	
	return is;
}

struct File 
{//inner data. Graph - NodeExpr - File.
	File() = default;
	File(std:: string);
	std::string name, full_path;
	enum class Type {File = 4, Directory = 8, Virtual = 0} type;
	bool operator==(const File& r);
	static bool exist(std::string s);
};

std::ostream& operator<<(std::ostream& o, const File& r);//to save or console
std::stringstream& operator<<(std::stringstream& ss, const File& r);//to show in gui
std::istream& operator>>(std::istream& i, File& r);
