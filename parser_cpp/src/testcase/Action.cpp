#include <cassert>

#include "help.h"
#include "Action.h"

void Action::set(std::string s, int line) {
	if (s == "static_compile") {
		action = ActionEnum::STATIC_COMPILE;
	} else if (s == "compile_calculate") {
		action = ActionEnum::COMPILE_CALCULATE;
	} else {
		printel("invalid string",s,"at line",line)
		;
		assert(0);
	}
}

std::string Action::toString() const {
	return action == ActionEnum::STATIC_COMPILE ?
			"static_compile" : "compile_calculate";
}

std::ostream& operator <<(std::ostream &os, const Action &a) {
	os << a.toString();
	return os;
}
