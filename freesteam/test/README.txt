freesteam test/README.txt


RUNNING THE TESTS
-----------------

You need to have CppUnit installed. This should be easy - you can just install an RPM in most cases. If you have an unusual case, such as the sourceforge.net compile farms, then you can follow the instructions below.

Once CppUnit is installed, assuming you have the command 'cppunit-config' in your path, you should just be able to type

cd /path/to/freesteam
cd test
make

And you should see some output a bit like this:

Running ExampleTestCase::example : OK
ExampleTestCase::anotherExample : OK
ExampleTestCase::testEquals : OK
EmptyTest::testTestFail : OK
EmptyTest::testTestPass : OK
UnitsTest::testSizeOfUnitsObject : OK
UnitsTest::testUnits : OK
  : :
 (etc)


STRANGE CPPUNIT INSTALLS
------------------------

If you don't have the command 'cppunit-config' in your path, you can still build the tests, as follows:

export CPPUNIT_CONFIG=/path/to/cppunit-config
cd /path/to/freesteam
cd test
make -e

When the test starts running, you should see something like

BUILDING TESTS with CppUnit version 1.10.2
(cppunit-config command is  /home/jp/cppunit-1.10.2/cppunit-config )
Flags:  -I/home/jp/local/include
Libs:   -L/home/jp/local/lib -lcppunit -ldl


This shows you that the cppunit-config command is functioning and returning the correct information.


John Pye
Thursday, 3 March 2005