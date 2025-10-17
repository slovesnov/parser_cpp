#include <cassert>
#include <regex>

#include "help.h"
#include "Data.h"

static std::vector<std::string> splitR(std::string const &s,
		std::string const &separator = "\\s+") {
	std::vector<std::string> v;
	std::regex rgx(separator);
	std::sregex_token_iterator iter(s.begin(), s.end(), rgx, -1);
	std::sregex_token_iterator end;
	for (; iter != end; ++iter) {
		v.push_back(*iter);
	}
	return v;
}

void Data::set(std::string s, int line) {
	std::string q;
	auto p = s.find('"');
	assert(p != std::string::npos);
	p++;
	assert(s.length() > p);
	auto p1 = s.find('"', p);
	assert(p1 != std::string::npos);
	compile = s.substr(p, p1 - p);

	variables.clear();
	values.clear();

	p = s.find('"', p1 + 1);
	if (p == std::string::npos) {
		return;
	}
	p++;
	assert(s.length() > p);
	p1 = s.find('"', p);
	q = s.substr(p, p1 - p);
	if (!q.empty()) {
		variables = splitR(q);
	}

	p = s.find('"', p1 + 1);
	assert(p != std::string::npos);
	p++;
	assert(s.length() > p);
	p1 = s.find('"', p);
	q = s.substr(p, p1 - p);
	if (!q.empty()) {
		for (auto &v : splitR(q)) {
			values.push_back(std::stod(v));
		}
	}

}

std::string Data::toString() const {
	int i = 0;
	std::string s = "compile=" + compile;
	if (!variables.empty()) {
		s += " variables=" + joinV(variables, ' ') + " values=";
		for (auto &a : values) {
			if (i) {
				s += ' ';
			}
			s += std::to_string(a);
			i++;
		}
	}
	return s;
}

std::ostream& operator <<(std::ostream &os, const Data &a) {
	os << a.toString();
	return os;
}
