#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <iostream>
#include <string>
using namespace std;

class Exception{
	private:
		string message;

	public:
		Exception(const char *c="Unknown exception!");

		Exception(string s);

		virtual ~Exception();

		virtual string what();

		void setMessage(string message);
};

#endif
