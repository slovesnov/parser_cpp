#ifndef TESTCASE_DATA_H_
#define TESTCASE_DATA_H_

#include <vector>

class Data {
public:
	std::string compile;
	std::vector<std::string> variables;
	std::vector<double> values;
	void set(std::string s, int line);
	std::string toString() const;
};

std::ostream& operator<<(std::ostream &os, const Data &a);

#endif /* TESTCASE_DATA_H_ */
