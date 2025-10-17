#ifndef TESTCASE_ACTION_H_
#define TESTCASE_ACTION_H_

#include <ostream>
#include <string>

class Action {
public:
	enum ActionEnum {
		STATIC_COMPILE, COMPILE_CALCULATE
	};
	ActionEnum action;
	void set(std::string s, int line);
	std::string toString() const;
};

std::ostream& operator<<(std::ostream &os, const Action &a);

#endif /* TESTCASE_ACTION_H_ */
