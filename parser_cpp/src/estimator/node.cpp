/******************************************************
 Copyright (c/c++) 2013-doomsday by Aleksey Slovesnov 
 homepage http://slovesnov.users.sourceforge.net/?parser
 email slovesnov@yandex.ru
 All rights reserved.
 ******************************************************/

//need #define _USE_MATH_DEFINES before any include
#define _USE_MATH_DEFINES
#include <cmath>
#include <random>

#include "node.h"
#include "expressionEstimator.h"

std::mutex Node::m_mutex;

void Node::init(ExpressionEstimator *estimator, OPERATOR_ENUM _operator,
		double value, Node *left) {
	m_estimator = estimator;
	m_estimator->m_vnode.push_back(this);
	m_operator = _operator;
	m_value = value;
	m_left = left;
	m_right = NULL;
#ifdef EXPRESSION_ESTIMATOR_COUNT_NODES
	ExpressionEstimator::totalCreated++;
	printf("new node %llx total created %d\n",uint64_t(this),ExpressionEstimator::totalCreated);
#endif
}

double Node::calculate() {
	double l, r;
	l = m_left ? m_left->calculate() : 0;
	r = m_right ? m_right->calculate() : 0;

	switch (m_operator) {
	case OPERATOR_ENUM::NUMBER:
		return m_value;

	case OPERATOR_ENUM::PLUS:
		return l + r;

	case OPERATOR_ENUM::MINUS:
		return l - r;

	case OPERATOR_ENUM::MULTIPLY:
		return l * r;

	case OPERATOR_ENUM::DIVIDE:
		return l / r;

	case OPERATOR_ENUM::UNARY_MINUS:
		return -l;

	case OPERATOR_ENUM::SIN:
		return sin(l);

	case OPERATOR_ENUM::COS:
		return cos(l);

	case OPERATOR_ENUM::TAN:
		return tan(l);

	case OPERATOR_ENUM::COT:
		return 1 / tan(l);

	case OPERATOR_ENUM::SEC:
		return 1 / cos(l);

	case OPERATOR_ENUM::CSC:
		return 1 / sin(l);

	case OPERATOR_ENUM::ASIN:
		return asin(l);

	case OPERATOR_ENUM::ACOS:
		return acos(l);

	case OPERATOR_ENUM::ATAN:
		return atan(l);

	case OPERATOR_ENUM::ACOT:
		return M_PI / 2 - atan(l);

	case OPERATOR_ENUM::ASEC:
		return acos(1 / l);

	case OPERATOR_ENUM::ACSC:
		return asin(1 / l);

	case OPERATOR_ENUM::SINH:
		return sinh(l);

	case OPERATOR_ENUM::COSH:
		return cosh(l);

	case OPERATOR_ENUM::TANH:
		return tanh(l);

	case OPERATOR_ENUM::COTH:
		return 1 / tanh(l);

	case OPERATOR_ENUM::SECH:
		return 1 / cosh(l);

	case OPERATOR_ENUM::CSCH:
		return 1 / sinh(l);

	case OPERATOR_ENUM::ASINH:
		return asinh(l);

	case OPERATOR_ENUM::ACOSH:
		return acosh(l);

	case OPERATOR_ENUM::ATANH:
		return atanh(l);

	case OPERATOR_ENUM::ACOTH:
		return atanh(1 / l);

	case OPERATOR_ENUM::ASECH:
		return acosh(1 / l);

	case OPERATOR_ENUM::ACSCH:
		return asinh(1 / l);

	case OPERATOR_ENUM::RANDOM:
		return random();

	case OPERATOR_ENUM::CEIL:
		return ceil(l);

	case OPERATOR_ENUM::FLOOR:
		return floor(l);

	case OPERATOR_ENUM::ROUND:
		return round(l);

	case OPERATOR_ENUM::ABS:
		return fabs(l);

	case OPERATOR_ENUM::EXP:
		return exp(l);

	case OPERATOR_ENUM::LOG:
		return log(l);

	case OPERATOR_ENUM::SQRT:
		return sqrt(l);

	case OPERATOR_ENUM::POW:
		return pow(l, r);

	case OPERATOR_ENUM::ATAN2:
		return atan2(l, r);

	case OPERATOR_ENUM::MIN:
		return std::min(l, r);

	case OPERATOR_ENUM::MAX:
		return std::max(l, r);

	case OPERATOR_ENUM::X:
		return m_estimator->m_argument[(int) m_value];

	default:
		throw std::runtime_error("Node.calculate error");
	}
}

double Node::random() {
	std::lock_guard<std::mutex> lock(m_mutex);
	std::random_device rd;
	std::default_random_engine eng(rd());
	std::uniform_real_distribution<double> distr(0, 1);
	return distr(eng);
}
