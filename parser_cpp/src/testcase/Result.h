/*
 * Result.h
 *
 *  Created on: 24.04.2022
 *      Author: alexey slovesnov
 * copyright(c/c++): 2014-doomsday
 *           E-mail: slovesnov@yandex.ru
 *         homepage: slovesnov.users.sourceforge.net
 */

#ifndef TESTCASE_RESULT_H_
#define TESTCASE_RESULT_H_

#include <string>

class Result {
public:
	enum ErrorCode {
		OK, COMPILE_ERROR, CALCULATE_ERROR
	};
	ErrorCode errorCode;
	double value;
	void set(ErrorCode code, double v = 0);
	void set(std::string, int line);
	bool operator==(Result const &a) const;
	bool operator!=(Result const &a) const;
	std::string toString() const;
};

std::ostream& operator<<(std::ostream &os, const Result &a);

#endif /* TESTCASE_RESULT_H_ */
