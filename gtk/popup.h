#include<gtkmm.h>

class AttributeDialog : public Gtk::Dialog
{///Popup window to set the attributes of a MindNode
public:
	AttributeDialog();
	Gtk::RadioButton *outline_bts[4], *line_bts[2];
	Gtk::Entry name;//file name
	Gtk::Scale arrow, shape;//for alpha value setting

protected:
	Gtk::RadioButton::Group outline, line;
	Gtk::RadioButton Picture, Rect, Diamond, Ellipse;
	Gtk::RadioButton Plant, Direct;
	Gtk::Label sl, al;
	Gtk::Box* vb;
	Gtk::HBox hb1, hb2, hb3, hb4;
};

class ResizeDialog : public Gtk::Dialog
{
public:
	ResizeDialog();
	Gtk::SpinButton width, height;
	Gtk::CheckButton fixed_ratio;

protected:
	Gtk::Label wl, hl;
	Gtk::HBox whb, hhb;
	void on_change(char c);
	void on_toggle();
	float rate;
};
