/******************************************************
 Copyright (c/c++) 2013-doomsday by Aleksey Slovesnov 
 homepage http://slovesnov.users.sourceforge.net/?parser
 email slovesnov@yandex.ru
 All rights reserved.
 ******************************************************/

//need #define _USE_MATH_DEFINES before any include
#define _USE_MATH_DEFINES
#include <cmath>
#include <cstring> //_strdup
#include <regex>
#include <algorithm>

#include "expressionEstimator.h"

const std::string CONSTANT_NAME[] = { "PI", "E", "SQRT2", "SQRT1_2", "LN2",
		"LN10", "LOG2E", "LOG10E" };
const double CONSTANT_VALUE[] = { M_PI, M_E, M_SQRT2, M_SQRT1_2, M_LN2, M_LN10,
M_LOG2E, M_LOG10E };

//order same with OPERATOR_ENUM
const std::string FUNCTION[] = { "POW", "ATAN2", "MIN", "MAX", "SIN", "COS",
		"TAN", "COT", "SEC", "CSC", "ASIN", "ACOS", "ATAN", "ACOT", "ASEC",
		"ACSC", "SINH", "COSH", "TANH", "COTH", "SECH", "CSCH", "ASINH",
		"ACOSH", "ATANH", "ACOTH", "ASECH", "ACSCH", "RANDOM", "CEIL", "FLOOR",
		"ROUND", "ABS", "SIGN", "EXP", "LOG", "SQRT" };

const std::initializer_list<OPERATOR_ENUM> A[] = { { OPERATOR_ENUM::PLUS,
		OPERATOR_ENUM::MINUS }, { }, { OPERATOR_ENUM::MULTIPLY,
		OPERATOR_ENUM::DIVIDE }, { OPERATOR_ENUM::POW } };

static std::regex bregex(std::string const &s) {
	return std::regex("\\b" + s + "\\b");
}

static std::string prepareString(std::string const &s) {
	if (std::regex_search(s,  std::regex("[+-]{2}"))) {
		throw std::runtime_error("two operators in a row");
	}
	auto q = std::regex_replace(s, std::regex("\\s+"), "");
	return std::regex_replace(q, std::regex("\\*{2}"), "^");
}

#ifdef EXPRESSION_ESTIMATOR_COUNT_NODES
int ExpressionEstimator::totalDestroyed = 0;
int ExpressionEstimator::totalCreated = 0;
#endif

static int getIndex(std::string const *begin, std::string const *end,
		std::string const &s) {
	auto it = std::find(begin, end, s);
	return it == end ? -1 : it - begin;
}

void ExpressionEstimator::getToken() {
	int i;
	unsigned j;
	char *pEnd;
	const char T[] = "+-*/()[]{},^";
	const char *p;
	bool b, er;
	std::string token[2];
	char c;
	double d;

	if (m_position == m_expression.length()) {
		m_operator = OPERATOR_ENUM::END;
	} else if ((p = strchr(T, m_expression[m_position])) != NULL) {
		m_position++;
		m_operator = (OPERATOR_ENUM) (p - T);
	} else if (isLetter()) {
		for (i = m_position++;
				m_position < m_expression.length() && isFunctionSymbol();
				m_position++)
			;
		std::string token = m_expression.substr(i, m_position - i);

		if (token[0] == 'X' && token.length() > 1 && isdigit(token[1])) {
			j = atoi(token.c_str() + 1);
			if (m_arguments < j + 1) {
				m_arguments = j + 1;
			}
			m_operator = OPERATOR_ENUM::X;
			m_tokenValue = j;
		} else {
			i = getIndex(std::begin(FUNCTION), std::end(FUNCTION), token);
			if (i == -1) {
				i = getIndex(std::begin(CONSTANT_NAME), std::end(CONSTANT_NAME),
						token);
				if (i == -1) {
					throw std::invalid_argument(
							"unknown keyword \"" + token + "\"");
				}
				m_tokenValue = CONSTANT_VALUE[i];
				m_operator = OPERATOR_ENUM::NUMBER;
			} else {
				m_operator = (OPERATOR_ENUM) (i + int(OPERATOR_ENUM::POW));
			}
		}
	} else if (isDigitOrPoint()) {
		m_operator = OPERATOR_ENUM::NUMBER;
		p = m_expression.c_str() + m_position;
		//binary or octal numbers
		if (*p == '0' && m_position + 1 < m_expression.length()
				&& strchr("OB", p[1])) {
			i = 0;
			b = p[1] == 'B';
			er = false;
			for (m_position += 2;
					m_position < m_expression.length() && isDigitOrPoint();
					m_position++) {
				if (isPoint()) {
					if (++i > 1) {
						er = true;
						break;
					}
				} else {
					c = m_expression[m_position];
					if (c - '0' >= (b ? 2 : 8)) {
						er = true;
						break;
					}
					token[i] += c;
				}
			}
			if (er || (token[0].empty() && token[1].empty())) {
				throw std::runtime_error(
						"invalid " + std::string(b ? "binary" : "octal")
								+ " constant");
			}
			m_tokenValue = 0;
			for (i = 0; i < 2; i++) {
				if (!token[i].empty()) {
					d = i ? 1ull << ((b ? 1 : 3) * token[1].length()) : 1;
					m_tokenValue += std::stoull(token[i], nullptr, b ? 2 : 8)
							/ d;
				}
			}
		} else {
			m_tokenValue = strtod(p, &pEnd); //also parse hex, 0x.34 also parsed here
			m_position += pEnd - p;
		}
	} else {
		throw std::runtime_error(
				std::string("unknown symbol ") + m_expression[m_position]);
	}

}

void ExpressionEstimator::compile(const char *expression) {
	m_position = 0;
	m_arguments = 0;
	m_root = NULL;
	m_argument = NULL;
	m_expression = prepareString(expression);
	std::transform(m_expression.begin(), m_expression.end(),
			m_expression.begin(), ::toupper);

	clear();

	//parse dot as decimal separator for strtod, so setup standard locale
	const char *lorig = _strdup(setlocale(LC_ALL, NULL));
	setlocale(LC_NUMERIC, "C");



	getToken();
	if (m_operator == OPERATOR_ENUM::END) {
		throw std::runtime_error("unexpected end of expression");
	}
	m_root = parse();
	if (m_operator != OPERATOR_ENUM::END) {
		throw std::runtime_error("end of expression expected");
	}

	//return original locale
	setlocale(LC_ALL, lorig);
	free((void*) lorig);
}

Node* ExpressionEstimator::parse(int n) {
	Node *node;
	if (n == 1) {
		if (m_operator == OPERATOR_ENUM::MINUS) {
			getToken();
			return new Node(this, OPERATOR_ENUM::UNARY_MINUS, parse(n + 1));
		} else {
			if (m_operator == OPERATOR_ENUM::PLUS) {
				getToken();
			}
			return parse(n + 1);
		}
	}
	if (n == 4) {
		return parse4();
	}
	node = parse(n + 1);
	while (std::find(std::begin(A[n]), std::end(A[n]), m_operator)
			!= std::end(A[n])) {
		node = new Node(this, m_operator, node);
		getToken();
		node->m_right = parse(n + 1);
	}
	return node;
}

Node* ExpressionEstimator::parse4() {
	Node *node;
	OPERATOR_ENUM open;
	int arguments;

	if (m_operator >= OPERATOR_ENUM::POW && m_operator <= OPERATOR_ENUM::SQRT) {
		arguments =
				m_operator <= OPERATOR_ENUM::MAX ?
						2 : m_operator != OPERATOR_ENUM::RANDOM;
		node = new Node(this, m_operator);
		getToken();
		open = m_operator;
		if (m_operator != OPERATOR_ENUM::LEFT_BRACKET
				&& m_operator != OPERATOR_ENUM::LEFT_SQUARE_BRACKET
				&& m_operator != OPERATOR_ENUM::LEFT_CURLY_BRACKET) {
			throw std::runtime_error("open bracket expected");
		}
		getToken();

		if (arguments > 0) {
			node->m_left = parse();

			if (arguments == 2) {
				if (m_operator != OPERATOR_ENUM::COMMA) {
					throw std::runtime_error("comma expected");
				}
				getToken();
				node->m_right = parse();
			}
		}
		checkBracketBalance(open);
	} else {
		switch (m_operator) {

		case OPERATOR_ENUM::X:
		case OPERATOR_ENUM::NUMBER:
			node = new Node(this, m_operator, m_tokenValue);
			break;

		case OPERATOR_ENUM::LEFT_BRACKET:
		case OPERATOR_ENUM::LEFT_SQUARE_BRACKET:
		case OPERATOR_ENUM::LEFT_CURLY_BRACKET:
			open = m_operator;
			getToken();
			node = parse();
			checkBracketBalance(open);
			break;

		default:
			throw std::runtime_error("unexpected operator");
		}

	}
	getToken();
	return node;
}

double ExpressionEstimator::calculate() {
	if (m_root == NULL) {
		throw std::runtime_error("using of calculate() without compile()");
	}

	unsigned length = m_argument == NULL ? 0 : m_argumentSize;

	if (length != m_arguments) {
		throw std::runtime_error("invalid number of expression arguments");
	}
	return m_root->calculate();
}

double ExpressionEstimator::calculate(const char *s) {
	ExpressionEstimator estimator;
	estimator.compile(s);
	estimator.m_argument = NULL; //clear all arguments
	return estimator.calculate();
}

void ExpressionEstimator::clear() {
	for (auto &a : m_vnode) {
		delete a;
	}
#ifdef EXPRESSION_ESTIMATOR_COUNT_NODES
	totalDestroyed += m_vnode.size();
	if(m_vnode.size()!=0){
		printf("destroyed %lld node(s) total %d\n",m_vnode.size(),totalDestroyed);
	}
#endif
	m_vnode.clear();

}

ExpressionEstimator::~ExpressionEstimator() {
	clear();
}

void ExpressionEstimator::compile(const std::string &expression,
		std::vector<std::string> const &variables) {
	int i;
	std::string s;
	std::vector<std::string> v;
	std::smatch m;
	const char r = '#';

	v = variables;
	std::sort(v.begin(), v.end());
	i = 0;
	for (auto &a : v) {
		s = a;
		std::transform(s.begin(), s.end(), s.begin(), ::toupper);
		if (getIndex(std::begin(CONSTANT_NAME), std::end(CONSTANT_NAME), s)
				!= -1
				|| getIndex(std::begin(FUNCTION), std::end(FUNCTION), s)
						!= -1) {
			throw std::invalid_argument(
					"reserved word \"" + a + "\" is used as variable");
		}
		//also check empty
		if (!std::regex_search(a, std::regex("^_*[A-Za-z]\\w*$"))) {
			throw std::runtime_error("invalid variable name \"" + a + "\"");
		}
		if (i > 0 && a == v[i - 1]) {
			throw std::runtime_error("repeated variable \"" + a + "\" in list");
		}
		i++;
	}

	s = prepareString(expression);
	if (s.find(r) != std::string::npos) {
		throw std::invalid_argument(r + std::string(" found in string"));
	}

	i = 0;
	for (auto &e : variables) {
		s = std::regex_replace(s, bregex(e), r + std::to_string(i));
		i++;
	}

	if (std::regex_search(s, m, bregex("[xX]\\d*"))) {
		throw std::invalid_argument("unknown variable \"" + m.str() + "\"");
	}

	std::replace(s.begin(), s.end(), r, 'X');

	compile(s);
	m_arguments = v.size();
}
