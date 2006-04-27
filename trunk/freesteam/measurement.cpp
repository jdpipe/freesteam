#include "measurement.h"

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <map>
using namespace std;

struct DimScale{
	Dimension dim;
	double scale;
};

struct UnitsDimScale{
	string units;
	DimScale ds;
};

typedef map<string,DimScale> UnitsMap;

class UnitsMapInitialiser{
	public:
		/// Data member: this will store the unitsmap
		UnitsMap unitsmap;

		/// This will initialise the unitsmap
		UnitsMapInitialiser(){

			static const int NUDS = 22;

			UnitsDimScale uds[NUDS] = {
				{"kg", {{ 1, 0, 0, 0, 0}, 1.    }}
				, {"m",    {{ 0, 1, 0, 0, 0}, 1.  }}
				, {"mm",   {{ 0, 1, 0, 0, 0}, 1e-3}}
				, {"cm",   {{ 0, 1, 0, 0, 0}, 1e-2}}
				, {"m2",   {{ 0, 2, 0, 0, 0}, 1.  }}
				, {"m3",   {{ 0, 3, 0, 0, 0}, 1.  }}
				, {"s",    {{ 0, 0, 1, 0, 0}, 1.  }}
				, {"K",    {{ 0, 0, 0, 1, 0}, 1.  }}
				, {"kg/m3",{{ 1,-3, 0, 0, 0}, 1.  }}
				, {"J/kg", {{ 0, 2,-2, 0, 0}, 1.  }}
				, {"kJ/kg",{{ 0, 2,-2, 0, 0}, 1e3 }}
				, {"Pa",   {{ 1,-1,-2, 0, 0}, 1.  }}
				, {"bar",  {{ 1,-1,-2, 0, 0}, 1e5 }}
				, {"kPa",  {{ 1,-1,-2, 0, 0}, 1e3 }}
				, {"MPa",  {{ 1,-1,-2, 0, 0}, 1e6 }}
				, {"W",    {{ 1, 2,-2, 0, 0}, 1.  }}
				, {"kW",   {{ 1, 2,-2, 0, 0}, 1e3 }}
				, {"MW",   {{ 1, 2,-2, 0, 0}, 1e6 }}
				, {"W/m2", {{ 1, 0,-3, 0, 0}, 1.  }}
				, {"W/m/K",{{ 1, 1,-3,-1, 0}, 1.  }}
				,{"J/kg/K",{{ 0, 2,-2,-1, 0}, 1.  }}
			   ,{"kJ/kg/K",{{ 0, 2,-2,-1, 0}, 1e3 }}
			};

			for(int i=0;i<NUDS;i++){
				unitsmap[uds[i].units] = uds[i].ds;
				//cerr << "ADDING " << uds[i].units << endl;
			}

			//cerr << "INITIALISED " << unitsmap.size() << " MEASUREMENT TYPES" << endl;

		}

};

/// Create a static unitsmap that is created a loadtime
UnitsMapInitialiser unitsmapinitialiser;

/// An alias to the unitsmap stored inside the unitsmap initialiser
const UnitsMap &unitsmap = unitsmapinitialiser.unitsmap;

Measurement::Measurement(const double &value, const char *units){

	Dimension dim;
	double scale;

	// interpret the units given and work out their dimension and scale
	UnitsMap::const_iterator ds = unitsmap.find(units);
	if(ds == unitsmap.end()){
		stringstream ss;
		ss << "Unknown measurement '" << units << "'! (" << unitsmap.size() << " available)";
		throw runtime_error(ss.str());
	}
	this->dim = ds->second.dim;
	this->value = value * ds->second.scale;
}

Measurement::Measurement(){
	value = 0;
	Dimension d = {0,0,0,0,0};
	dim = d;
}

Measurement::Measurement(const double &value, const Dimension &dim, const double &scale){
	if(scale==1){
		this->value = value;
	}else{
		this->value = value * scale;
	}
	this->dim = dim;
}

const std::string
Measurement::toString(){
	stringstream ss;
	ss << value << "<" << dim.m << "," << dim.l << "," << dim.t << "," \
	<< dim.k << "," << dim.i << ">";
	return ss.str();
}

int dimension_cmp(const Dimension &d1, const Dimension &d2){
	if(d1.m==d2.m && d1.l==d2.l && d1.t==d2.t && d1.k==d2.k && d1.i==d2.i){
		return 0;
	}
	if(d1.m > d2.m || d1.l > d2.l || d1.t > d2.t || d1.k > d2.k || d1.i > d2.i){
		return 1;
	}
	return -1;
}

Dimension dimension_add(const Dimension &d1, const Dimension &d2){
	Dimension d;
	d.m = d1.m + d2.m;
	d.l = d1.l + d2.l;
	d.t = d1.t + d2.t;
	d.k = d1.k + d2.k;
	d.i = d1.i + d2.i;
	return d;
}

Dimension dimension_sum(const Dimension &d1, const Dimension &d2){
	Dimension d;
	d.m = d1.m - d2.m;
	d.l = d1.l - d2.l;
	d.t = d1.t - d2.t;
	d.k = d1.k - d2.k;
	d.i = d1.i - d2.i;
	return d;
}

Dimension::~Dimension(){
	// nothing to do, just keeping SWIG happy
}