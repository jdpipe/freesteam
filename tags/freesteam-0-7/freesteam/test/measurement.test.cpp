#include <iostream>

#include "../measurement.h"
#include "../units.h"

#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestFixture.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>

class MeasurementTest : public CppUnit::TestFixture{

	void TestSize(){

		// Why is the size what I expect, PLUS sizeof(int) ?
		CPPUNIT_ASSERT(sizeof(Measurement)==sizeof(double)+sizeof(Dimension)+sizeof(int) );
	}

	void TestOneMetre()
	{
		Measurement L2(1,"m");

		CPPUNIT_ASSERT(L2.value==1);
		CPPUNIT_ASSERT(L2.dim.m==0);
		CPPUNIT_ASSERT(L2.dim.l==1);
		CPPUNIT_ASSERT(L2.dim.t==0);
		CPPUNIT_ASSERT(L2.dim.k==0);
		CPPUNIT_ASSERT(L2.dim.i==0);
	}

	void TestFiveKilograms()
	{
		Measurement L2(5.0,"kg");

		CPPUNIT_ASSERT(L2.value==5.0);
		CPPUNIT_ASSERT(L2.dim.m==1);
		CPPUNIT_ASSERT(L2.dim.l==0);
		CPPUNIT_ASSERT(L2.dim.t==0);
		CPPUNIT_ASSERT(L2.dim.k==0);
		CPPUNIT_ASSERT(L2.dim.i==0);
	}

	void TestFiftyBar()
	{
		Measurement L2(50.0,"bar");

		CPPUNIT_ASSERT(L2.value==5000000.);
		CPPUNIT_ASSERT(L2.dim.m==1);
		CPPUNIT_ASSERT(L2.dim.l==-1);
		CPPUNIT_ASSERT(L2.dim.t==-2);
		CPPUNIT_ASSERT(L2.dim.k==0);
		CPPUNIT_ASSERT(L2.dim.i==0);
	}

	void TestCastToUnits()
	{
		Measurement M(50.0,"bar");

		Pressure p(M);

		CPPUNIT_ASSERT(p == 50.0 * bar);
	}

	void TestAddToUnits()
	{
		Measurement M(50.0,"bar");

		Pressure p = 1. * bar;

		CPPUNIT_ASSERT(p + M == 51.0 * bar);
	}

	void TestMultiplyWithUnits()
	{
		Measurement A(10,"m2");

		Pressure p = 1. * bar;

		Force F = p * Area(A);

		CPPUNIT_ASSERT(F == 1 * Mega*Newton);
	}

	void TestCastFromUnits(){
		Pressure p = 20. * bar;
		Measurement M = p;
		CPPUNIT_ASSERT(M.value==2000000.);
		CPPUNIT_ASSERT(M.dim.m==1);
		CPPUNIT_ASSERT(M.dim.l==-1);
		CPPUNIT_ASSERT(M.dim.t==-2);
		CPPUNIT_ASSERT(M.dim.k==0);
		CPPUNIT_ASSERT(M.dim.i==0);
	}

	void TestAssignToUnits(){
		Measurement M(50,"bar");
		Pressure p;
		p = M;
	}

	void TestCastFromConductivity(){
		ThermalConductivity k;
		Measurement M(10,"W/m/K");
		k = M;
		CPPUNIT_ASSERT(k == 10. * Watt / metre / Kelvin);
	}

	public:
		void setUp(){}
		void tearDown(){}

	public:
		// Invoke CPPUNIT macros to add tests to a suite:
		CPPUNIT_TEST_SUITE(MeasurementTest);
		CPPUNIT_TEST(TestSize);
		CPPUNIT_TEST(TestOneMetre);
		CPPUNIT_TEST(TestFiveKilograms);
		CPPUNIT_TEST(TestFiftyBar);
		CPPUNIT_TEST(TestCastToUnits);
		CPPUNIT_TEST(TestAddToUnits);
		CPPUNIT_TEST(TestMultiplyWithUnits);
		CPPUNIT_TEST(TestCastFromUnits);
		CPPUNIT_TEST(TestAssignToUnits);
		CPPUNIT_TEST(TestCastFromConductivity);
		CPPUNIT_TEST_SUITE_END();

};

CPPUNIT_TEST_SUITE_REGISTRATION(MeasurementTest);
