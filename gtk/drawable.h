//this is a Cairomm drawable class, just give a cairo context as argument to draw.
#pragma once
#include<complex>
#include<vector>
#include<string>
#include<sstream>
#include<gtkmm.h>

struct Point : std::complex<double>
{
	Point(int x=0, int y=0) : std::complex<double>(x, y) {}
	Point(std::complex<double> im) : std::complex<double>(im) {}
	int x() const { return real(); } 		
	int y() const { return imag(); }
};

class Drawable
{///all the drawing dependent part should be inside the operator()()
public://base destructor should exist and be virtual for polymorphism
	virtual ~Drawable() = default;
	virtual void operator()(const Cairo::RefPtr<Cairo::Context>& cr) = 0;
	void line_width(int w);
	void set_rgba(double r, double g, double b, double a);
	
protected:
	double color[4];
	int line_width_ = 3;
	Point start_, end_;
};

class Text : public Drawable
{
public:
	Text(Point s, Point e, std::string txt);
	virtual void operator()(const Cairo::RefPtr<Cairo::Context>& cr);
	void set_family(std::string family);
	void set_weight(int weight);
	void set_size(int sz);
	void txt(std::string s);
protected:
	std::string txt_;
	Pango::FontDescription font_;
};

class Line : public Drawable 
{
public:
	Line(std::initializer_list<Point> li);
	virtual void operator()(const Cairo::RefPtr<Cairo::Context>& cr);
protected:
	Line() = default;
	std::vector<Point> points_;
};

struct Arrow : Line 
{
	Arrow(Point s, Point e, int tip = 1);
};

struct Box : Line
{
	Box(Point s, Point e);
};

struct Diamond : Line
{
	Diamond(Point s, Point e);
};

struct Ellipse : Drawable
{
	Ellipse(Point s, Point e);
	virtual void operator()(const Cairo::RefPtr<Cairo::Context>& cr);
};

class Pix : public Box
{
public:
	Pix(std::string filename, Point s, Point e);
	virtual void operator()(const Cairo::RefPtr<Cairo::Context>& cr) override;

protected:
	Glib::RefPtr<Gdk::Pixbuf> image;
	int w, h;
	std::string filename;
	Point start, end;
	bool initialized = false;
};

