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

		static const int NUDS = 19;
		/// This will initialise the unitsmap
		UnitsMapInitialiser(){
			UnitsDimScale uds[NUDS] = {
				{"kg", {{ 1, 0, 0, 0, 0}, 1.    }}
				, {"m",  {{ 0, 1, 0, 0, 0}, 1.    }}
				, {"mm",  {{ 0, 1, 0, 0, 0}, 1e-3 }}
				, {"cm",  {{ 0, 1, 0, 0, 0}, 1e-2 }}
				, {"m2",   {{ 0, 2, 0, 0, 0}, 1.  }}
				, {"m3",   {{ 0, 3, 0, 0, 0}, 1.  }}
				, {"s",    {{ 0, 0, 1, 0, 0}, 1.  }}
				, {"K",    {{ 0, 0, 0, 1, 0}, 1.  }}
				, {"kg/m3",{{ 1,-3, 0, 0, 0}, 1.  }}
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
			};

			for(int i=0;i<NUDS;i++){
				unitsmap[uds[i].units] = uds[i].ds;
			}

			cerr << "INITIALISED " << unitsmap.size() << " MEASUREMENT TYPES" << endl;

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
