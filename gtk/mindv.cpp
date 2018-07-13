#include<gtkmm.h>
#include"sketch.h"
#include"graphview.h"
#include"src/graph.h"
#include"src/nodeexpr.h"
using namespace std;

GraphView<File> GV;

int main(int ac, char** av)
{
	if(ac < 2) return 0;

	int i = 0;
	cout << av[1];
	auto app = Gtk::Application::create(i, av);
	Win win{av[1]};
//	win.sketch_.set_drawables(a.first, a.second);
	win.set_title("File Browser inspired by MindMap");
	return app->run(win);
}


