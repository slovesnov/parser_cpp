/******************************************************
 Copyright (c/c++) 2013-doomsday by Aleksey Slovesnov
 homepage http://slovesnov.users.sourceforge.net/?parser
 email slovesnov@yandex.ru
 All rights reserved.
 ******************************************************/

/*
 * TYPE 1 - sample1
 * TYPE 2 - sample2
 * TYPE 3 - parser check using test cases
 * TYPE 4 - sample random() & threads
 * TYPE 5 - copy testcases from cpp to java, js, php
 * else   - sample
 */
#define TYPE 3

#include <iostream>
#include <sstream>

#include "estimator/expressionEstimator.h"

std::string vectorToString(const std::vector<double> &v) {
	std::stringstream ss;
	int i = 0;
	for (auto &a : v) {
		ss << (i ? ", " : "[") << a;
		i++;
	}
	ss << "]";
	return ss.str();
}

#if TYPE==1

int main() {
	try {
		const char *a[] = { "sin(pi/4)", "pow( sin(pi/10)*4+1 , 2)",
				"(sqrt(28/27)+1)^(1/3)-(sqrt(28/27)-1)^(1/3)",
				"sqrt(28/3)*sin(asin( sqrt(243/343)) /3 )" };
		for(auto e:a){
			std::cout << ExpressionEstimator::calculate(e) << std::endl;
		}
	} catch (std::exception &ex) {
		std::cout << ex.what() << std::endl;
	}
}

#elif TYPE==2

int main() {
	ExpressionEstimator e;
	double v;
	std::vector<double> d;
	std::vector<std::string> s;
	try {
		e.compile("x0+2*sin(pi*x1)");
		v = e.calculate(1, .25);
		std::cout << v << std::endl;
		d = { 1, 1. / 6 };
		v = e.calculate(d);
		std::cout << v << std::endl;
		e.compile("x0+2*X1"); // case insensitive variable names
		v = e.calculate( { 1, .25 });
		std::cout << v << std::endl;

		ExpressionEstimator e1("x0+2*x1");
		const double f[] = { 1, 3 };
		v = e1.calculate(f, std::size(f));
		std::cout << v << std::endl;

		e.compile("a+2*b", "a", "b"); // case sensitive variable names
		// or e.compile("a+2*b", std::vector<std::string> { "a", "b" });
		// or e.compile("a+2*b", { "a", "b" });
		// or std::string s[]={ "a", "b" }; e.compile("a+2*b", s, std::size(s));
		v = e.calculate(1, 1);
		std::cout << v << std::endl;

		ExpressionEstimator e2("a+2*A", "a", "A");// case sensitive variable names
		// or ExpressionEstimator e2("a+2*A", std::vector<std::string> { "a", "A" });
		// or ExpressionEstimator e2("a+2*A", { "a", "A" });
		// or std::string s[] = { "a", "A" }; ExpressionEstimator e2("a+2*A", s, std::size(s));
		v = e2.calculate(1, 2);
		std::cout << v << std::endl;

	} catch (std::exception &e) {
		std::cout << e.what() << std::endl;
	}
}

#elif TYPE==3

#include <vector>
#include <fstream>
#include <cmath>

#include "testcase/Case.h"
#include "testcase/help.h"

int main() {
	const std::string fn = "testcases.txt";
	std::ifstream infile(fn);
	if (!infile.is_open()) {		//not open if run using shortcut
		printl("cann't open file, need to run under eclipse")
		;
		return 0;
	}
	std::string s, q[3];
	int i, j, k, r[] = { 0, 0 }, li[3], line;
	Case t;

	i = 0;
	line = 0;
	while (std::getline(infile, s)) {
		line++;
		if (s.empty()) {
			continue;
		}
		q[i] = s;
		li[i] = line;
		if (i == 2) {
			t.set(q, li);
			r[t.test()]++;
			i = 0;
		} else {
			i++;
		}
	}

	j = r[0] + r[1];
	k = log10(j) + 1;
	for (i = 1; i >= 0; i--) {
		println("%-5s %*d/%d=%5.1lf%%", i ? "ok" : "error", k, r[i], j,
				r[i] * 100. / j);
	}

	//println("created %d destroyed %d",ExpressionEstimator::totalCreated,ExpressionEstimator::totalDestroyed);
}

#elif TYPE==4

#include <thread>

void f(int t) {
	for(int i=0;i<3;i++){
	printf("t%d %lf\n",t, ExpressionEstimator::calculate("random()"));
	}
}

int main() {
	int cores=3;
	int i,j;
	std::vector<std::thread> vt;

	for (i = 0; i < cores; ++i) {
		vt.push_back(std::thread(f, i));
	}

	for (auto& a : vt){
		a.join();
	}
	printf("\n");

	const int arguments = 2;
	const double v[arguments] = { 3, 6 };

	for(i=0;i<3;i++){
		printf("%lf\n", ExpressionEstimator::calculate("random()"));
	}

	printf("\n");
	ExpressionEstimator estimator[2];
	for(j=0;j<2;j++){
		estimator[j].compile("random()+x0+x1");
	}

	try {
		for(j=0;j<2;j++){
			for(i=0;i<3;i++){
				printf("%d %d %lf\n",i,j, estimator[j].calculate(v, arguments));
			}
		}
	} catch (std::exception &e) {
		printf("%s\n", e.what());
	}

}
#elif TYPE==5

#include <fstream>

const int cpp = 0;
const int java = 1;
const int js = 2;
const int php = 3;

const std::string p[] = { R"(C:\Users\user\git\parser_cpp\parser_cpp)",
		R"(C:\Users\user\git\javaPlotterCalculator\javaPlotterCalculator)",
		R"(c:\slovesno\vscodegit\parser\javascript\testcase\)",
		R"(c:\slovesno\vscodegit\parser\php\)"
};

std::string path(int n) {
	const char bs='\\';
	auto s=p[n];
	return s + (s[s.length()-1]==bs?"":std::string(1,bs))+"testcases." + (n == js ? "js" : "txt");
}

int main(int argc, char *argv[]) {
	std::size_t i;
	std::string s;
	std::ostringstream ss;

	std::ifstream f(path(cpp));
	if (!f) {
		printf("%s",path(cpp).c_str());
		std::cout << "cann't open cpp file";
		return -1;
	}

	ss << f.rdbuf();
	for (i = 1; i < std::size(p); i++) {
		std::ofstream o(path(i));
		if (!o) {
			std::cout << "cann't open file to write" + std::to_string(i);
			return -1;
		}
		s = ss.str();
		if (i == js) {
			s = "let gdata = `" + s + "`";
		}
		o << s;
	}
	std::cout<<"all copied successfully"<<std::endl;

}

#else
int main() {
	ExpressionEstimator estimator;

	try {
		printf("%lf\n", ExpressionEstimator::calculate("abs(.5)"));
//		std::string s[] = { "a" };
//		ExpressionEstimator estimator("a^a", s, std::size(s));

		estimator.compile("x0^x0");
		double d[] = { 2 };
		const int i=1;
		int const& j=i;
		printf("%lf\n", estimator.calculate(d, std::size(d)));
		printf("%lf\n", estimator.calculate(d,1));
		printf("%lf\n", estimator.calculate(d,i));
		printf("%lf\n", estimator.calculate(d,j));
		printf("%lf\n", estimator.calculate(d,unsigned(1)));
		printf("%lf\n", estimator.calculate(d,int64_t(1)));
		//printf("%lf\n", ExpressionEstimator::calculate("sqrt(28/3)*sin(asin(-sqrt(243/343))/3+4*pi/3 )"));

	} catch (std::exception &e) {
		std::cout << e.what() << std::endl;
	}

}
#endif
