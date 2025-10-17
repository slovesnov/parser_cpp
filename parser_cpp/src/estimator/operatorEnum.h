#ifndef _operatorEnum_h_
#define _operatorEnum_h_

/*
 notes for OPERATOR_ENUM 
 PLUS should be first in enum. Common operators should go in a row. Order is important. 
 POW, ATAN2, MIN, MAX should go in a row see parse3 function
 PLUS-POW should go in row getToken()
 POW-SQRT should go in row from parse3()
 finally PLUS should be first, PLUS-SQRT should goes in row
 */
enum class OPERATOR_ENUM {
	PLUS,
	MINUS,
	MULTIPLY,
	DIVIDE,
	LEFT_BRACKET,
	RIGHT_BRACKET,
	LEFT_SQUARE_BRACKET,
	RIGHT_SQUARE_BRACKET,
	LEFT_CURLY_BRACKET,
	RIGHT_CURLY_BRACKET,
	COMMA,
	POW,
	ATAN2,
	MIN,
	MAX,
	SIN,
	COS,
	TAN,
	COT,
	SEC,
	CSC,
	ASIN,
	ACOS,
	ATAN,
	ACOT,
	ASEC,
	ACSC,
	SINH,
	COSH,
	TANH,
	COTH,
	SECH,
	CSCH,
	ASINH,
	ACOSH,
	ATANH,
	ACOTH,
	ASECH,
	ACSCH,
	RANDOM,
	CEIL,
	FLOOR,
	ROUND,
	ABS,
	SIGN,
	EXP,
	LOG,
	SQRT,
	X,
	NUMBER,
	UNARY_MINUS,
	END
};

#endif
