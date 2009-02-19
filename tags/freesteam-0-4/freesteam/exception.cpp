#include "exception.h"

Exception::Exception(const char *c){
	message = string(c);
}

Exception::Exception(string s){
	message = s;
}

Exception::~Exception(){
	// Intentionally empty
}

string
Exception::what(){
	return message;
}

void
Exception::setMessage(string message){
	this->message=message;
}
