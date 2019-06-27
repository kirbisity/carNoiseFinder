#include <sstream>
#include <vector>
#include <string>
/* in current directory */
#include "parser.h"

using namespace std;

vector<double> parse_tabs(string line) {
	std::vector<double> tabs;
	vector<string> items = split(line);
	for (vector<string>::const_iterator i = items.begin(); i != items.end(); ++i) {
		double speedentry = -1;
		size_t idx = i->find("km/h", 0);
		if (idx != string::npos) {
			speedentry = atof(i->substr(0, (int)idx).c_str());
		}
		if (*i == "idle") {
			speedentry = 0;
		}
		if (speedentry != -1) {
			tabs.push_back(speedentry);
		}	
	}
	return tabs;
}

vector<string> split(string line) {
	stringstream ss(line);
	istream_iterator<string> begin(ss);
	istream_iterator<string> end;
	vector<string> items(begin, end);
	return items;
}

