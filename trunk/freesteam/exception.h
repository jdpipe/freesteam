#ifndef EXCEPTION_H
#define EXCEPTION_H

#include "designbycontract.h"

#include <iostream>
#include <string>
using namespace std;

class Exception{
	private:
		string message;

	public:
		Exception(const char *c="Unknown exception!"){
			//cerr << "Exception created! (char)" << endl;
			message=string(c);
		}

		Exception(string s){
			//cerr << "Exception created! (string)" << endl;
			//cerr << "String is " << s << endl;
			message=string(s);
		}

		virtual ~Exception(){}

		virtual string what(){
			//cerr << "Returning string now..." << endl;
			return message;
		}

		void setMessage(string message){
			this->message=message;
		}
};

class Pressure_Over_Range : public Exception{
	public:
		Pressure_Over_Range(double P) : Exception("Pressure over range"){
		}

};

class Pressure_Under_Range : public Exception{
	public:
		Pressure_Under_Range(double P) : Exception("Pressure under range"){
		}

};


class Air_Temp_Over_Range: public Exception{
	public:
		Air_Temp_Over_Range(double T) : Exception("Air temp over range"){
			this->T=T;
		}

		string what(){
			cerr << "Temp = " << this->T << endl;
			return Exception::what();
		}

	private:
		double T;
};

class Air_Temp_Under_Range: public Exception{
	public:
		Air_Temp_Under_Range(double T) : Exception("Air temp under range"){
			this->T=T;
		}

		string what(){
			cerr << "Temp = " << this->T << endl;
			return Exception::what();
		}

	private:
		double T;
};

class Temp_Over_Range: public Exception{
	public:
		Temp_Over_Range(double T) : Exception("temp over range"){
		}

};

class Temp_Under_Range: public Exception{
	public:
		Temp_Under_Range(double T) : Exception("temp under range"){
		}
};


class System_Overpressure: public Exception{
	public:
		System_Overpressure() : Exception("system pressure over range"){
		}
};

class System_Underpressure: public Exception{
	public:
		System_Underpressure() : Exception("system pressure under range"){
		}
};

class MartNels_InletPres_Above_Range: public Exception{
	public:
		MartNels_InletPres_Above_Range(): Exception("pressure above range in Mart Nels correlation"){
		}
};

class MartNels_InletPres_Below_Range: public Exception{
	public:
		MartNels_InletPres_Below_Range(): Exception("pressure below range in Mart Nels correlation"){
		}
};

#endif
