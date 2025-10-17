#ifndef _expressionEstimator_h_
#define _expressionEstimator_h_

#include <vector>
#include <string>
#include <stdexcept>

#include "node.h"

class ExpressionEstimator {
	friend class Node;

	std::string m_expression;
	double m_tokenValue;
	OPERATOR_ENUM m_operator;
	unsigned m_position;
	const double *m_argument;
	unsigned m_argumentSize;
	unsigned m_arguments;
	Node *m_root;
	std::vector<Node*> m_vnode;

	bool isLetter() {
		return isalpha(m_expression[m_position]);
	}

	bool isDigit() {
		return isdigit(m_expression[m_position]);
	}

	bool isPoint() {
		return m_expression[m_position] == '.';
	}

	bool isDigitOrPoint() {
		return isDigit() || isPoint();
	}

	bool isFunctionSymbol() {
		char c = m_expression[m_position];
		return isalnum(c) || c == '_';
	}

	void getToken();

	Node* parse(int n = 0);
	Node* parse4();

	void checkBracketBalance(OPERATOR_ENUM open) {
		if (int(m_operator) != int(open) + 1) {
			throw std::runtime_error(
					"close bracket expected or another type of close bracket");
		}
	}

	void clear();

public:
	//version for calculator/graph
	static constexpr double version = 1.27; //format("%.2lf",)

#ifdef EXPRESSION_ESTIMATOR_COUNT_NODES
	static int totalCreated;
	static int totalDestroyed;
#endif

	ExpressionEstimator() {
		m_root = NULL; //do not remove
	}

	//forbid copy
	ExpressionEstimator(ExpressionEstimator const&) = delete;
	ExpressionEstimator& operator=(ExpressionEstimator const&) = delete;

	template<typename ...A>
	ExpressionEstimator(const std::string &expression, A ...a) {
		std::vector<std::string> v = { std::forward<std::string>(a)... };
		try {
			compile(expression, v);
		} catch (std::exception &e) {
			clear();
			throw;
		}
	}

	ExpressionEstimator(const std::string &expression,
			std::vector<std::string> const &v) :
			ExpressionEstimator() {
		try {
			compile(expression, v);
		} catch (std::exception &e) {
			clear();
			throw;
		}
	}

	ExpressionEstimator(const std::string &expression,
			std::initializer_list<std::string> const &v) :
			ExpressionEstimator() {
		try {
			compile(expression, std::vector<std::string>(v));
		} catch (std::exception &e) {
			clear();
			throw;
		}
	}
	//allow 3rd parameter int, unsigned, uint64_t=size_t, int64_t
	template<typename A>
	ExpressionEstimator(const std::string &expression, const std::string *x,
			A size) :
			ExpressionEstimator() {
		try {
			compile(expression, std::vector<std::string>(x, x + size));
		} catch (std::exception &e) {
			clear();
			throw;
		}
	}
	//allow 3rd parameter int, unsigned, uint64_t=size_t, int64_t
	template<typename A>
	ExpressionEstimator(const std::string &expression, std::string *x, A size) :
			ExpressionEstimator() {
		try {
			compile(expression, std::vector<std::string>(x, x + size));
		} catch (std::exception &e) {
			clear();
			throw;
		}
	}

	ExpressionEstimator(const char *expression) :
			ExpressionEstimator() {
		try {
			compile(expression);
		} catch (std::exception &e) {
			clear();
			throw;
		}
	}

	ExpressionEstimator(std::string const &expression) :
			ExpressionEstimator(expression.c_str()) {
	}

	void compile(const char *expression);
	void compile(const std::string &expression) {
		compile(expression.c_str());
	}

	/*compile with variable list*/
	template<typename ...A>
	void compile(const std::string &expression, A ...a) {
		std::vector<std::string> v = { std::forward<std::string>(a)... };
		compile(expression, v);
	}
	void compile(const std::string &expression,
			std::vector<std::string> const &v);

	void compile(const std::string &expression,
			std::initializer_list<std::string> const &v) {
		compile(expression, std::vector<std::string> { v });
	}
	void compile(const char *expression,
			std::initializer_list<std::string> const &v) {
		compile(expression, std::vector<std::string> { v });
	}
	//allow 3rd parameter int, unsigned, uint64_t=size_t, int64_t
	template<typename A>
	void compile(const std::string &expression, const std::string *x, A size) {
		compile(expression, std::vector<std::string>(x, x + size));
	}
	//allow 3rd parameter int, unsigned, uint64_t=size_t, int64_t
	template<typename A>
	void compile(const std::string &expression, std::string *x, A size) {
		compile(expression, std::vector<std::string>(x, x + size));
	}

	template<typename ...A>
	double calculate(A ...a) {
		std::vector<double> v = { std::forward<double>(a)... };
		return calculate(v);
	}

	double calculate(std::vector<double> const &v) {
		return calculate((const double*) v.data(), v.size());
	}

	double calculate(std::initializer_list<double> const &v) {
		return calculate(std::vector<double> { v });
	}

	//allow 2nd parameter int, unsigned, uint64_t=size_t, int64_t
	template<typename A>
	double calculate(const double *x, A size) {
		m_argument = x;
		m_argumentSize = size;
		return calculate();
	}

	//allow 2nd parameter int, unsigned, uint64_t=size_t, int64_t
	template<typename A>
	double calculate(double *x, A size) {
		m_argument = x;
		m_argumentSize = size;
		return calculate();
	}

	double calculate();

	int getArguments() {
		return m_arguments;
	}

	static double calculate(const char *s);
	static double calculate(std::string const &s) {
		return calculate(s.c_str());
	}

	~ExpressionEstimator();
};

#endif
