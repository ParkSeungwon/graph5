#include<gtkmm.h>
#include<fstream>
#include<iostream>
#include"drawable.h"
#include"sketch.h"
#include"src/graph.h"
using namespace std;

SketchBook::SketchBook()
{
	set_size_request(5000, 5000);
}

void SketchBook::set_drawables(Drawable** begin, Drawable** end) 
{
	begin_ = begin; end_ = end;
}

bool SketchBook::on_draw(const Cairo::RefPtr<Cairo::Context>& cr) 
{
	for(auto** it = begin_; it != end_; it++) (**it)(cr);
	return true;
}

void SketchBook::refresh()
{
	auto win = get_window();
    if (win) {
        Gdk::Rectangle r(0, 0,
				get_allocation().get_width(), get_allocation().get_height());
        win->invalidate_rect(r, false);
    }
}


GraphSketch::GraphSketch(string root) : root_position_{300,300}, root_{root}
{
	add_events(Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK);
	string s = getenv("HOME");
	ifstream f(s + "/.mindmap");
	if(f.is_open()) f >> gv_;
	else gv_.insert_vertex(root_);
	auto dr = gv_.generate_drawables(root_, root_position_);
	set_drawables(dr.first, dr.second);
}

GraphSketch::~GraphSketch()
{
	string s = getenv("HOME");
	ofstream f(s + "/.mindmap");
	f << gv_;
}

bool GraphSketch::on_button_release_event(GdkEventButton* e)
{
	if(e->button == 1) {
		to_ = {e->x, e->y};
		File f = gv_.pick(from_);
		if(f == root_) root_position_ = to_;
		else {
			auto parent = gv_.find_parent(f);
			for(auto* e = gv_.find_vertex(parent)->edge; e; e = e->edge)
				if(e->vertex->data.data == f) e->weight += to_ - from_;
		}
		auto dr = gv_.generate_drawables(root_, root_position_);
		set_drawables(dr.first, dr.second);
		refresh();	
		return true;//does not propagate
	}
	return false;//propagate
}

bool GraphSketch::on_button_press_event(GdkEventButton* e)
{
	if(e->button == 1) {
		from_ = {e->x, e->y};
		return true;//does not propagate
	} else if(e->button == 3) {
		File f = gv_.pick({e->x, e->y});
		popup(f);
		auto dr = gv_.generate_drawables(root_, root_position_);
		set_drawables(dr.first, dr.second);
	}
	return false;//propagate
}

bool Win::on_button_press_event(GdkEventButton* e)
{
	cout << e->x << ' ' << e->y << endl;
	return true;
}

Win::Win(string s) : sketch_{s}
{
	add(scwin_);
	scwin_.add(sketch_);
	set_default_size(1000, 900);
	show_all_children();
}

