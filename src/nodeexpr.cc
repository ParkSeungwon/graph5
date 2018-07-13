#include<cctype>
#include<string>
#include<iostream>
#include<experimental/filesystem>
#include"nodeexpr.h"
using namespace std;
using namespace std::experimental::filesystem;

istream& get_line(istream& is, string& s) {
	while(getline(is, s)) for(char c : s) if(!isspace(c)) return is;
	return is;
}

File::File(std::string s)
{
	path p = path(s);
	path abs_path = canonical(p);
	name = p.filename().string();
	full_path = abs_path.string();
	type = is_directory(abs_path) ? File::Type::Directory : File::Type::File;
}

bool File::exist(string s)
{
	return exists(path(s));
}

bool File::operator==(const File& r)
{
	return full_path == r.full_path;
}

ostream& operator<<(ostream& o, const File& r)
{
	Json::Value jv;
	jv["name"] = r.name;
	jv["full_path"] = r.full_path;
	jv["type"] = static_cast<int>(r.type);
	Json::FastWriter fast;
	fast.omitEndingLineFeed();
	o << fast.write(jv);
	return o;
}

stringstream& operator<<(stringstream& ss, const File& r)
{
	ss << r.name;
	return ss;
}

istream& operator>>(istream& is, File& r)
{
	Json::Value jv; string s; stringstream ss;
	get_line(is, s); ss << s; ss >> jv;
	r.name = jv["name"].asString();
	r.full_path = jv["full_path"].asString();
	r.type = static_cast<File::Type>(jv["type"].asInt());
	return is;
}
