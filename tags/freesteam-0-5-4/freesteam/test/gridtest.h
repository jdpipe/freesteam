#ifndef GRIDTEST_H
#define GRIDTEST_H

#include "test.h"

#include <iostream>
#include <iomanip>
#include <map>
#include <algorithm>
using namespace std;

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestFixture.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/CompilerOutputter.h>

template < class XValue, class YValue, class Ordinate >
class GridTest : public CppUnit::TestFixture {

	typedef std::vector<YValue> YContainer;
	typedef typename YContainer::iterator YValueIterator;

	typedef typename std::vector<YValue>::iterator YVecIterator;
	typedef typename std::vector<Ordinate>::iterator OrdVecIterator;

	typedef std::map<YValue,Ordinate> Column;
	typedef typename Column::iterator ColumnIterator;
	typedef typename std::map<XValue,Column>::iterator TableIterator;

	private:
		YContainer yvalues;

		map<XValue,Column> data;

	public:

		void testAllPoints() {

			TableIterator it;
			for(it = data.begin(); it != data.end(); ++it) {
				testColumn(it);
			}
		}

		void testColumn(const TableIterator& it){

			XValue x=it->first;
			Column c=it->second;

			ColumnIterator ic;
			for(ic = c.begin(); ic != c.end(); ++ic){

				YValue y=ic->first;
				Ordinate z=ic->second;

				testOnePoint(x,y,z);
			}

		}

		virtual void testOnePoint(XValue& x, YValue& y, Ordinate& z) {
			CPPUNIT_FAIL("'testOnePoint' not implemented.");
		}

	protected:
		/**
			The values are stored as |: x,(yz yz yz yz) :| so print out the y values at the top and loop through x values row by row.
		*/
		void printData() {

			// Output all the known yvalues at the top
			cout << endl << setw(15) << "\\\\";

			for (YValueIterator iy = yvalues.begin() ; iy != yvalues.end() ; ++iy ) {
				cout << right << setw(10) << *iy;
			}
			cout << endl;

			// Iterate through the columns:

			for (TableIterator it=data.begin();it != data.end();++it){

				XValue x=it->first;
				Column c=it->second;

				cout << right << setw(10) << x;
				cout << setw(5) << ":";

				for (YValueIterator iy = yvalues.begin(); iy != yvalues.end(); ++iy) {

					// Is this y index present in this column?
					YValue y = *iy;

					//cerr << "Searching for value " << y << " in column of " << c.size() << " items." << endl;

					//cerr << "Length of column: " << c.size() << endl;

					ColumnIterator ic=c.find(y);
					if(ic==c.end()){
						cout << setw(10)<<".";
					}else{
						Ordinate z = ic->second;
						cout << setw(10) << z;
					}
				}

				cout << endl;
			}
		}

		void addPoint(const XValue& x, const YValue& y, const Ordinate z) {

			YValueIterator iy = findOrInsert(y);

			Column *c = &data[x];
			ColumnIterator ic;

			// THIS DOESNT WORK BECUASE MAPS ARE ORDERED AND YIterator HAS NO ORDERING:
			ic = c->find(y);

			if (ic == c->end()) {


				pair<ColumnIterator,bool> res;

				res = c->insert(pair<YValue,Ordinate>(y,z));

				if(res.second){
					//cerr << "Added point " << x << ", " << y << ", " << z;
					//cerr << ", now contains " << c->size() << endl;
				}//ic= res.first;
			} else {
				throw Exception("Invalid X,Y,Z: this point already defined");
			}
		}

		void addVector(const YValue& y,const double *x, const XValue x_base, const double *z, const Ordinate z_base,int npairs){

			for(int i=0; i < npairs; i++){
				const double x1=x[i];
				const double z1=z[i];

				addPoint(x1 * x_base, y, z1 * z_base);
			}
		}

		YValueIterator findOrInsert(const YValue & y) {
			YValueIterator iy;
			iy = find(yvalues.begin(),yvalues.end(),y);
			if (iy == yvalues.end()) {
				iy = yvalues.insert(iy,y);
			}
			return iy;
		}

	public:

		virtual void setUp(){}

		virtual void tearDown(){
			data.clear();
		}

};

#endif
