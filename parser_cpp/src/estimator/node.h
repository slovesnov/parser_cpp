#ifndef _node_h_
#define _node_h_

#include <mutex>

//#define EXPRESSION_ESTIMATOR_COUNT_NODES
#include "operatorEnum.h"

class ExpressionEstimator;

class Node {
	friend class ExpressionEstimator;

	ExpressionEstimator *m_estimator;
	OPERATOR_ENUM m_operator;
	double m_value;
	Node *m_left, *m_right;
	static std::mutex m_mutex;

	Node(ExpressionEstimator *estimator, OPERATOR_ENUM _operator, Node *left =
	NULL) {
		init(estimator, _operator, 0, left);
	}

	Node(ExpressionEstimator *estimator, OPERATOR_ENUM _operator,
			double value) {
		init(estimator, _operator, value, NULL);
	}

	void init(ExpressionEstimator *estimator, OPERATOR_ENUM _operator,
			double value, Node *left);

	double calculate();

#ifdef EXPRESSION_ESTIMATOR_COUNT_NODES
	~Node() {
		printf("~node %llx\n", uint64_t(this));
	}
#endif

	static double random();
};

#endif
