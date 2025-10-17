#include <cstdio>
#include <cstdarg>

#include "help.h"

#define G_DIR_SEPARATOR '\\'
static int aslovOutputWide = 40;

void aslovPrintHelp(ASLOV_OUTPUT_TYPE t, const std::string &s, const char *f,
		const int l, const char *fu) {
	const char *p = strrchr(f, G_DIR_SEPARATOR);
	p = p ? p + 1 : f;
//	if (t==ASLOV_OUTPUT_TYPE::FILE) {
//		time_t t = time(NULL);
//		tm *q = localtime(&t);
//		FILE *w = openApplicationLog("a");
//		fprintf(w, "%s %s:%d %s() %02d:%02d:%02d %02d.%02d.%d\n", s.c_str(), p,
//				l, fu, q->tm_hour, q->tm_min, q->tm_sec, q->tm_mday,
//				q->tm_mon + 1, q->tm_year + 1900);
//		fclose(w);
//	} else
	if (t == ASLOV_OUTPUT_TYPE::STDERR) {
		fprintf(stderr, "%-*s %s:%d %s()\n", aslovOutputWide, s.c_str(), p, l,
				fu);
	} else {
		printf("%-*s %s:%d %s()\n", aslovOutputWide, s.c_str(), p, l, fu);
	}
}

//format to string example format("%d %s",1234,"some")
std::string format(const char *f, ...) {
	va_list a;
	va_start(a, f);
	size_t size = vsnprintf(nullptr, 0, f, a) + 1;
	va_end(a);
	std::string s;
	if (size > 1) {
		s.resize(size);
		va_start(a, f);
		vsnprintf(&s[0], size, f, a);
		va_end(a);
		s.resize(size - 1);
	}
	return s;
}

