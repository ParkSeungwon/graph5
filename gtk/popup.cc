#include<functional>
#include"popup.h"
#include"sketch.h"
using namespace std;
using namespace Gtk;

AttributeDialog::AttributeDialog() : Picture(outline, "picture"), 
	Rect(outline, "rect"), Diamond(outline, "diamond"), Ellipse(outline, "ellipse"), 
	Plant(line, "plant"), Direct(line, "direct"), sl("Shape"), al("Arrow"),
	arrow(Gtk::Adjustment::create(1, 0, 255, 1, 10, 0)),
	shape(Gtk::Adjustment::create(1, 0, 255, 1, 10, 0))
{
	set_size_request(100,100);
	vb = get_content_area();
	vb->pack_start(name);
	vb->pack_start(hb1);
	vb->pack_start(hb2);
	vb->pack_start(hb3);
	vb->pack_start(hb4);
	hb1.pack_start(Picture, PACK_SHRINK);
	hb1.pack_start(Rect, PACK_SHRINK);
	hb1.pack_start(Diamond, PACK_SHRINK);
	hb1.pack_start(Ellipse, PACK_SHRINK);
	hb2.pack_start(Plant, PACK_SHRINK);
	hb2.pack_start(Direct, PACK_SHRINK);
	hb3.pack_start(al, PACK_SHRINK);
	hb3.pack_start(arrow);
	hb4.pack_start(sl, PACK_SHRINK);
	hb4.pack_start(shape);
	add_button("_Ok", 1);
	add_button("_Cancel", 0);
	outline_bts[0] = &Picture;
	outline_bts[1] = &Rect;
	outline_bts[2] = &Diamond;
	outline_bts[3] = &Ellipse;
	line_bts[0] = &Plant;
	line_bts[1] = &Direct;
	show_all_children();
}

ResizeDialog::ResizeDialog() : fixed_ratio("fix rate"), wl("width"), hl("height"),
	width(Gtk::Adjustment::create(1, 0, 1000, 1, 10, 0)), 
	height(Gtk::Adjustment::create(1, 0, 1000, 1, 10, 0))
{
	set_size_request(100, 100);
	Gtk::Box* box = get_content_area();
	box->pack_start(fixed_ratio, PACK_SHRINK);
	whb.pack_start(wl, PACK_SHRINK);
	whb.pack_end(width, PACK_SHRINK);
	hhb.pack_start(hl, PACK_SHRINK);
	hhb.pack_end(height, PACK_SHRINK);
	box->pack_start(whb, PACK_SHRINK);
	box->pack_start(hhb, PACK_SHRINK);
	width.set_numeric();
	height.set_numeric();
	width.signal_value_changed().connect(bind(&ResizeDialog::on_change, this, 'w'));
	height.signal_value_changed().connect(bind(&ResizeDialog::on_change, this, 'h'));
	fixed_ratio.signal_toggled().connect(bind(&ResizeDialog::on_toggle, this));
	add_button("_Ok", 1);
	add_button("_Cancel", 0);
	show_all_children();
}

void ResizeDialog::on_change(char c) 
{
	if(fixed_ratio.get_active()) {
		switch(c) {
			case 'w': height.set_value(width.get_value() / rate); break;
			case 'h': width.set_value(rate * height.get_value()); break;
		}
	}
}

void ResizeDialog::on_toggle()
{
	if(fixed_ratio.get_active()) rate = width.get_value() / height.get_value();
}
		

void GraphSketch::shape_chooser(File f) {
	AttributeDialog ad;
	ad.name.set_text(f.name);
	ad.outline_bts[static_cast<int>(gv_.get_outline(f))]->set_active();
	ad.line_bts[static_cast<int>(gv_.get_arrowline(f))]->set_active();
	auto a = gv_.get_color_a(f);
	ad.arrow.set_value(a[1]);//a
	ad.shape.set_value(a[2]);
	if(ad.run()) {
		for(int i=0; i<5; i++) if(ad.outline_bts[i]->get_active()) {
			gv_.set_outline(f, static_cast<OutlineShape>(i));
			break;
		}
		for(int i=0; i<2; i++) if(ad.line_bts[i]->get_active()) {
			gv_.set_arrowline(f, static_cast<ArrowShape>(i));
			break;
		}
		a[1] = ad.arrow.get_value();
		a[2] = ad.shape.get_value();
		gv_.set_color_a(f, a);
		bool same_name = false;
		if(f.name != ad.name.get_text()) {//if changed & no same name 
			string s = f.full_path.substr(0, f.full_path.rfind('/'));
			s += ad.name.get_text();
			if(!File::exist(s)) {//rename it
				string command = "mv '";
				command += f.full_path + "' '" + s + "'";
				system(command.data());
				f.name = ad.name.get_text();
				f.full_path = s;
			}
		}
	}
}

void GraphSketch::color_chooser(File f) {
	int result;
	{
		Gtk::Dialog dia;
		dia.add_button("_Text",1);
		dia.add_button("_Line",2);
		dia.add_button("_Shape",3);
		dia.add_button("Cancel",0);
		result = dia.run();
	}
	if(0 < result && result < 4) {
		Gtk::ColorChooserDialog dia;
		if(dia.run() == Gtk::RESPONSE_OK) {
			auto color = dia.get_rgba();
			array<unsigned char, 4> ar = {
				color.get_red() * 255, color.get_green() * 255, 
				color.get_blue() * 255, color.get_alpha() * 255};
			switch(result) {
				case 1: gv_.set_text_color(f, ar); break;
				case 2: gv_.set_outline_color(f, ar); break;
				case 3: gv_.set_arrow_color(f, ar); break;
			}
		}
	}
}

static void app_chooser(File f) {
	string command = 
		"python -c 'import webbrowser,sys; webbrowser.open(sys.argv[1])' '";
	command += f.full_path + "'";//for spaced file name
	system(command.data());
}

void GraphSketch::file_chooser(File f) {
	Gtk::FileChooserDialog dia("Choose files to expose or unexpose");
	dia.set_select_multiple();
	dia.set_current_folder(f.full_path);
	dia.add_button("_Ok", 1);
	dia.add_button("_Cancel", 0);
	if(dia.run() == 1) {
		string s = dia.get_filename();
		File f2{s};
		if(gv_.find_vertex(f2)) gv_.toggle_show(f2);
		else {
			gv_.insert_vertex(f2);
			gv_.insert_edge(f, f2, {50, 50});
		}
	}
}

void GraphSketch::resize(File f) {
	ResizeDialog dia;
	dia.width.set_value(gv_.get_width(f));
	dia.height.set_value(gv_.get_height(f));
	if(dia.run()) gv_.set_width_height(f,
			dia.width.get_value_as_int(), dia.height.get_value_as_int());
}

void GraphSketch::popup(File f) {
	int result;
	{
		Gtk::Dialog dia("Selelct one");
		dia.add_button("_Shape",1);
		dia.add_button("_Color",2);
		dia.add_button("_Open",3);
		dia.add_button("_Resize",4);
		if(f.type == File::Type::Directory) dia.add_button("_Expose", 5);
		dia.add_button("Cancel",0);
		result = dia.run();
	}

	switch(result) {
		case 1: shape_chooser(f); break;
		case 2: color_chooser(f); break;
		case 3: app_chooser(f); break;
		case 4: resize(f); break;
		case 5: file_chooser(f); break;
		default:;
	}
}
