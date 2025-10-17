#ifndef TESTCASE_CASE_H_
#define TESTCASE_CASE_H_

#include "Data.h"
#include "Action.h"
#include "Result.h"
#include "../estimator/expressionEstimator.h"

class Case {
public:
	int line;
	Data data;
	Action action;
	Result result;
	ExpressionEstimator e;
	void set(std::string s[], int lines[]);
	bool test();
};

#endif /* TESTCASE_CASE_H_ */
