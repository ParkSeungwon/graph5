#include<gtkmm.h>
#include<fstream>
#include"sketch.h"
#include"graphview.h"
#include"src/nodeexpr.h"
using namespace std;
int main(int ac, char** av)
{//show basic usage
	GraphView<string> gr, gr2;
	string s[3] = {"우리가", "Hello", "Exit"};
	gr.insert_vertex(s[0]);
	gr.insert_vertex(s[1]);
	gr.insert_vertex(s[2]);
	gr.insert_edge(s[0], s[1], {0, 200});
	gr.insert_edge(s[1], s[2], {300, 0});
	gr.set_width_height(s[0], 250, 230);
	gr.set_outline(s[0], OutlineShape::Picture);
	gr.set_outline(s[2], OutlineShape::Diamond);
	gr.set_width_height(s[2], 60, 30);
	gr.set_arrow_color(s[2], {255, 0, 125, 150});
	gr.set_picture_file(s[0], "1.jpg");
	gr.set_outline_color(s[0], {0, 255,0,255});
//	ifstream f("mindmap.txt");
	ifstream f("mm.txt");
//	f2 << gr;
	f >> gr2;
	cout << gr << gr2;
	auto a = gr2.generate_drawables(s[0], {300,300});
	auto b = gr.generate_drawables(s[0], {800, 600});
	cout << a.second - a.first << endl;
	cout << b.second - b.first << endl;
	vector<Drawable*> v;
	for(auto it = b.first; it != b.second; it++) v.push_back(*it);
	for(auto it = a.first; it != a.second; it++) v.push_back(*it);
	
	auto app = Gtk::Application::create(ac, av);
	Gtk::Window win;
	Gtk::ScrolledWindow sc;
	SketchBook sketch;
	win.add(sc);
	sc.add(sketch);
	win.set_default_size(800, 700);
	sketch.set_drawables(v.data(), v.data() + v.size());
	win.show_all_children();
	return app->run(win);
}

