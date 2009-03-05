%module freesteam

%include <python/std_string.i>
%include <python/std_except.i>
%include <python/std_vector.i>
%include <python/std_set.i>

%{
#include "steamcalculator.h"
#include "solver2.h"
#include "measurement.h"
%}

#define FREESTEAM_SWIG

// All STL runtime_errors caught to Python

%exception {
	try {
		$action
	}
	catch (std::range_error &e) {
		std::cerr << "RANGE ERROR: " << e.what();
		exit(1);
	}
	catch (std::runtime_error &e) {
		SWIG_exception(SWIG_RuntimeError,e.what());
	}catch(...){
		SWIG_exception(SWIG_RuntimeError,"uncaught unknown exception!");
	}
}

// Common stuff, values, etc. Needed for definition of SOLVE_ENTHALPY etc.

#include <freesteam/common.h>

// The Solver2 class, stripped of its base class and private/protected methods:

template<class FirstProp,class SecondProp,int FirstPropAlt=0, int SecondPropAlt=0>
class Solver2{
	public:
		Solver2();
		~Solver2();
		%extend{
			SteamCalculator solve(const Measurement &fp, const Measurement &sp){
				//cerr << "SETTING...." << endl;
				//cerr << "fp = " << fp << endl;
				//cerr << "sp = " << sp << endl;
				//cerr << "fp is " << SteamProperty<FirstProp,FirstPropAlt>::name() << endl;
				FirstProp fp1=fp;
				SecondProp sp1=sp;
				//cerr << "fp1 = " << fp1 << endl;
				//cerr << "sp1 = " << sp1 << endl;
				return self->solve(fp1,sp1);
				//return SteamCalculator();
			}
		}
};

// Convenient names to allow python syntax like steam_ph().solve(p,h);

%template(steam_ph) Solver2<Pressure,SpecificEnergy,0,SOLVE_ENTHALPY>;
%template(steam_uv) Solver2<SpecificEnergy,SpecificVolume,SOLVE_IENERGY,0>;
%template(steam_Ts) Solver2<Temperature,SpecificEntropy,0,SOLVE_ENTROPY>;
%template(steam_ps) Solver2<Pressure,SpecificEntropy,0,SOLVE_ENTROPY>;
%template(steam_pT) Solver2<Pressure,Temperature,0,0>;

%rename(steam) SteamCalculator;

%extend SteamCalculator{
	%pythoncode{
		def __reduce__(self):
			r = self.whichRegion()
			if r==1 or r==2:
				tup = (r, self.pres(),self.temp())
			elif r==3:
				tup = (r, self.dens(),self.temp())
			elif r==4:
				tup = (r, self.temp(),self.quality())
			return (steam, tuple(), tup)

		def __setstate__(self,tup):
			r=tup[0]
			if r==1:
				self.setRegion1_pT(tup[1],tup[2])
			elif r==2:
				self.setRegion2_pT(tup[1],tup[2])
			elif r==3:
				self.setRegion3_rhoT(tup[1],tup[2])
			elif r==4:
				self.setRegion4_Tx(tup[1],tup[2])
	}				
}

%ignore DesignByContract;
%ignore SteamCalculator::operator=;

%include "steamcalculator.h"

%extend SteamCalculator{
	Measurement SteamCalculator::temp(){
		return self->temp();
	}
}

%ignore DesignByContract;

/*
	Measurement class, with arithmetic operators to make it behave
	sanely -- like a runtime-checking equivalent of the
	compiletime-checking functionality of the Units class
*/
%ignore operator<<;
%include "measurement.h"

Dimension dimension_add(const Dimension &d1, const Dimension &d2);
Dimension dimension_sub(const Dimension &d1, const Dimension &d2);
Dimension dimension_mul(const Dimension &d1, const int &n);

%extend Measurement{
	%pythoncode{
		def to(self,units):
			v = self/Measurement(1,units)
			if not v.isNonDim():
				raise ValueError("Incompatible dimensions");
			return v.value

		def __reduce__(self):
			return (Measurement, tuple(), (self.value,self.dim.m,self.dim.l,self.dim.t,self.dim.k,self.dim.i) )

		def __setstate__(self,tup):
			self.value,self.dim.m,self.dim.l,self.dim.t,self.dim.k,self.dim.i = tup			
			
		def __sub__(self,other):
			if str(other.__class__) != "<class 'freesteam.Measurement'>" \
					and self.__class__ != other.__class__ \
					and not self.__class__ in other.__class__.__bases__:
				raise RuntimeError("Second value in subtraction is not a Measurement (it's a '%s')" % other.__class__)
			if dimension_cmp(self.dim,other.dim)!=0:
				raise RuntimeError("Incompatible dimensions: self=%s, other=%s" %(self,other));
			else:
				return Measurement(self.value - other.value, self.dim);

		def __add__(self,other):
			if str(other.__class__) != "<class 'freesteam.Measurement'>" \
					and self.__class__ != other.__class__ \
					and not self.__class__ in other.__class__.__bases__:
				raise RuntimeError("Second value in addition is not a Measurement (it's a '%s')" % other.__class__)

			if dimension_cmp(self.dim,other.dim)!=0:
				raise RuntimeError("Incompatible dimensions: self=%s, other=%s" %(self,other));
			else:
				return Measurement(self.value + other.value, self.dim);

		def __mul__(self,other):
			if str(other.__class__) != "<class 'freesteam.Measurement'>" \
					and self.__class__ != other.__class__ \
					and not self.__class__ in other.__class__.__bases__:
				raise RuntimeError("Second value in multiplication is not a Measurement (it's a '%s')" % other.__class__)

			return Measurement(self.value * other.value, dimension_add(self.dim,other.dim));

		def __rmul__(self,other):
			return Measurement(self.value * other, dimension_add(self.dim,Dimension()) )

		def __div__(self,other):
			if str(other.__class__) != "<class 'freesteam.Measurement'>" \
					and self.__class__ != other.__class__ \
					and not self.__class__ in other.__class__.__bases__:
				raise RuntimeError("Second value in division is not a Measurement (it's a '%s')" % other.__class__)

			return Measurement(self.value / other.value, dimension_sub(self.dim,other.dim));

		def __pow__(self,other):
			if other.__class__!=int:
				raise RuntimeError('Second value in Measurement^power must be an integer')
			return Measurement(self.value **other, dimension_mul(self.dim,other))

		def __neg__(self):
			return Measurement(-self.value, self.dim)

		def __float__(self):
			return self.value;

		def __repr__(self):
			return self.toString();
	}
}