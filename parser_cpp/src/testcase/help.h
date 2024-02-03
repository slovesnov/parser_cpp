/*
 * help.h
 *
 *  Created on: 25.01.2024
 *      Author: alexey slovesnov
 * copyright(c/c++): 2014-doomsday
 *           E-mail: slovesnov@yandex.ru
 *         homepage: slovesnov.users.sourceforge.net
 */

#include <string>
#include <sstream>
#include <cstring>
#include <cassert>
#include <cstdint>
#include <vector>

//format to string example format("%d %s",1234,"some")
std::string format(const char *f, ...);

//output info to screen example printl(1234,"some")
#define printl printai
#define printel printeai

//output info to screen example println("%d %s",1234,"some")
#define println(...)  aslovPrintHelp(ASLOV_OUTPUT_TYPE::STDOUT,format(__VA_ARGS__),__FILE__,__LINE__,__func__);
#define printeln(...)  aslovPrintHelp(ASLOV_OUTPUT_TYPE::STDERR,format(__VA_ARGS__),__FILE__,__LINE__,__func__);

#define printi println("")
#define printei printeln("")

#define printsi(...) aslovPrintHelp(ASLOV_OUTPUT_TYPE::STDOUT,formats(__VA_ARGS__),__FILE__,__LINE__,__func__);
#define printai(...) aslovPrintHelp(ASLOV_OUTPUT_TYPE::STDOUT,formata(__VA_ARGS__),__FILE__,__LINE__,__func__);
#define printzi(...) aslovPrintHelp(ASLOV_OUTPUT_TYPE::STDOUT,formatz(__VA_ARGS__),__FILE__,__LINE__,__func__);

#define printesi(...) aslovPrintHelp(ASLOV_OUTPUT_TYPE::STDERR,formats(__VA_ARGS__),__FILE__,__LINE__,__func__);
#define printeai(...) aslovPrintHelp(ASLOV_OUTPUT_TYPE::STDERR,formata(__VA_ARGS__),__FILE__,__LINE__,__func__);
#define printezi(...) aslovPrintHelp(ASLOV_OUTPUT_TYPE::STDERR,formatz(__VA_ARGS__),__FILE__,__LINE__,__func__);

#define forma formata

enum class ASLOV_OUTPUT_TYPE {
	STDOUT, STDERR, FILE
};

template<typename A, typename ... B>
std::string formats(const std::string &separator, A const &a, B const &... b) {
	std::stringstream c;
	c << a;
	((c << separator << b), ...);
	return c.str();
}

template<typename ... A>
std::string formats(const char separator, A const &...a) {
	return formats(std::string(1, separator), a...);
}

template<typename ... A>
std::string formatz(A const &...a) {
	return formats("", a...);
}

//format to string example forma(1234,"some")
template<typename ... A>
std::string formata(A const &...a) {
	return formats(" ", a...);
}

void aslovPrintHelp(ASLOV_OUTPUT_TYPE t, const std::string &s, const char *f,
		const int l, const char *fu);

/* parseString("0xff",i,16), parseString("ff",i,16), parseString("+0xff",i,16) ok
 * t is changed only if parse is valid
 */
template<class T>
bool parseString(const char *d, T &t, int radix = 10) {
	/* strtol("") is ok so check whether empty string
	 * strtol(" 4") "\r4", "\n4", "\t4" is ok so check for space
	 * */
	if (!d || *d == 0 || isspace(*d)) {
		return false;
	}
	/*strtoul("-1") is ok*/
	if (std::is_unsigned<T>::value && *d == '-') {
		return false;
	}
	char *p;
	T a;

	/* if correct parse then errno is not changed
	 * so set errno=0
	 */
	errno = 0;
	if (std::is_same_v<T, long>
			|| (std::is_same_v<T, int> && sizeof(int) == sizeof(long))) {
		a = strtol(d, &p, radix);
	} else if (std::is_same_v<T, unsigned long>
			|| (std::is_same_v<T, unsigned> && sizeof(int) == sizeof(long))) {
		a = strtoul(d, &p, radix);
	} else if (std::is_same_v<T, int64_t>
			|| (std::is_same_v<T, int> && sizeof(int) == sizeof(int64_t))) {
		a = strtoll(d, &p, radix);
	} else if (std::is_same_v<T, uint64_t>
			|| (std::is_same_v<T, unsigned> && sizeof(int) == sizeof(int64_t))) {
		a = strtoull(d, &p, radix);
	} else if (std::is_same_v<T, float>) {
		a = strtof(d, &p);
	} else if (std::is_same_v<T, double>) {
		a = strtod(d, &p);
	} else {
		assert(0);
		return false;
	}
	/*errno!=0 - out of range, *p!=0 - not full string recognized*/
	bool b = errno == 0 && *p == 0;
	if (b) {
		t = a;
	}
	return b;
}

template<class T>
bool parseString(std::string const &s, T &t, int radix = 10) {
	return parseString(s.c_str(), t, radix);
}

//separator can be default so use as 2nd parameter. It differs from other functions arguments order.
template<typename T>
std::string joinV(std::vector<T> const &v, std::string const &separator = " ") {
	std::stringstream c;
	bool f = true;
	for (auto const &a : v) {
		if (f) {
			f = false;
		} else {
			c << separator;
		}
		c << a;
	}
	return c.str();
}

template<typename T>
std::string joinV(std::vector<T> const &v, const char separator) {
	return joinV(v, std::string(1, separator));
}
