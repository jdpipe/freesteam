#ifndef ZEROINVISITOR_H
#define ZEROINVISITOR_H

#include "designbycontract.h"

#include <iostream>
#include <cmath>
using namespace std;

#define DBL_EPSILON 2e-16

/**
	Whacko, this class does root finding with units intact!
	
	We will be varying the x (Abscissa) value until the y (Ordinate) value is zero.
	
	@param Subject class on which solving of a function like y(x)=0 is to be done
	@param Ordinate y-value type
	@param Abscissa x-value type
	
	@see units.h
*/
template < class Subject, class Ordinate = double, class Abscissa = double >
class ZeroIn : public DesignByContract {

    private:
    Abscissa ax;		///< lower bound
    Abscissa bx;		///< upper bound
    Ordinate(Subject::*method) (Abscissa);	///< pointer to member function that will be solved
    Abscissa tol;		///< tolerance, stop once we move less than this amount (absolute) in an iteration
    Abscissa x;			///< solution result
    bool issolved;

    Abscissa a, b, c;		///<  Abscissae, descr. see above.
    Ordinate fa;		///<  f(a)
    Ordinate fb;		///<  f(b)
    Ordinate fc;		///<  f(c)

    Subject *subject;

    public:

    ZeroIn() {
	    this->tol = 0.0 * Abscissa();
    }
    
    /// Set the lower bound of the range that will be searched for a solution
    void setLowerBound(const Abscissa& ax) {
	    this->ax = ax;
    }

    /// @see setLowerBound
    Abscissa getLowerBound() {
	    return ax;
    }

    /// Set the upper bound of the range that will be searched for a solution
    void setUpperBound(const Abscissa bx) {
	    this->bx = bx;
    }

    /// @see setUpperBound
    Abscissa getUpperBound() {
	    return bx;
    }

    /// set tolerance of the solution
    void setTolerance(const Abscissa tol) {
	    this->tol = tol;
    }

    /// Set the member function that will be solved
    /**
		The method Ordinate(Subject::*method)(Abscissa) returns values like y(x)
		
		@example
		z->setMethod(&Quadratic::evaluate);
     */
    void setMethod(Ordinate(Subject::*method) (Abscissa)) {
	    this->method = method;
    }

    /// ZeroIn visit method
    /**
    	This is a visitor pattern implementation of the Brent ZEROIN routine adapted from the C math library version.
    	
    	USAGE
    	
    		

    	@see brent.shar at http://www.netlib.org/c/
    		
    	Original documentation:
    	
    	 function ZEROIN - obtain a function zero within the given range

    	 Input
    		double zeroin(ax,bx,f,tol)
    		double ax; 				Root will be sought for within a range [ax,bx]
    		double bx;  			
    		
    		double (*f)(double x);	Name of the function whose zero will be sought for
    								
    		double tol;				Acceptable tolerance for the root value.
    								May be specified as 0.0 to cause the program to find the root as accurate as possible

    	 Output
    		Zeroin returns an estimate for the root with accuracy
    		4*EPSILON*abs(x) + tol

    	 Algorithm
    		
    		G.Forsythe, M.Malcolm, C.Moler, Computer methods for mathematical computations. M., Mir, 1980, p.180 of the Russian edition

    		The function makes use of the bissection procedure combined with the linear or quadric inverse interpolation.
    		
    		At every step program operates on three abscissae - a, b, and c.
    		
    		b - the last and the best approximation to the root
    		a - the last but one approximation
    		c - the last but one or even earlier approximation than a that
    			1) |f(b)| <= |f(c)|
    			2) f(b) and f(c) have opposite signs, i.e. b and c confine
    			   the root



    */
    void visit(Subject * subject) {

	    //cerr << "Begin zeroin with lower bound " << K_TO_C(getLowerBound()) << "°C and upper bound " << K_TO_C(getUpperBound()) << endl;

	    this->subject = subject;
	    this->issolved = false;

	    a = ax;
	    b = bx;
	    fa = ((*subject).*method) (a);
	    fb = ((*subject).*method) (b);
	    c = a;
	    fc = fa;

	    //  Main iteration loop

	    for (;;) {
		    Abscissa prev_step = b - a;    ///<  Distance from the last but one to the last approximation
		    Abscissa tol_act;                ///<  Actual tolerance
		    Abscissa p;                    ///<  Interpolation step is calculated in the form p/q; division
		    double q;                      ///<  operations is delayed until the last moment
		    Abscissa new_step;             ///<  Step at this iteration

		    if (fabs(fc) < fabs(fb)) {
			    a = b;
			    b = c;
			    c = a;  //  Swap data for b to be the best approximation
			    fa = fb;
			    fb = fc;
			    fc = fa;
		    }

		    // DBL_EPSILON is defined in math.h
		    tol_act = 2.0* DBL_EPSILON * fabs(b) + tol / 2.0;

		    new_step = (c - b) / 2.0;

		    if (fabs(new_step) <= tol_act || fb == 0.0 * Ordinate()) {
			    this->issolved = true;       //  Acceptable approx. is found
			    //cerr << "Best solution is b, f(b=" << b << ")=" << fb <<",f(a="<<a<<")="<< fa <<endl;
			    return;
		    }
		    //  Decide if the interpolation can be tried

		    if (fabs(prev_step) >= tol_act   // If prev_step was large enough and was in true direction,
		        && fabs(fa) > fabs(fb))      // Interpolatiom may be tried
		    {
			    register double t1, t2;
			    Abscissa cb;

			    cb = c - b;
			    if (a == c) {
				    // If we have only two distinct points
				    // then only linear interpolation can be applied
				    t1 = fb / fa;
				    p = cb * t1;
				    q = 1.0 - t1;
			    } else {
				    // Quadric inverse interpolation

				    q = fa / fc;
				    t1 = fb / fc;
				    t2 = fb / fa;
				    p = t2 * (cb * q * (q - t1) - (b - a) * (t1 - 1.0));
				    q = (q - 1.0) * (t1 - 1.0) * (t2 - 1.0);
			    }
			    if (p > 0.0 * Abscissa()) {
				    // p was calculated with the opposite sign; make p positive-
				    q = -q;	// and assign possible minus to q
			    } else {
				    p = -p;
			    }

			    if (p < (0.75 * cb * q - fabs(tol_act * q) / 2.0)
			        && p < fabs(prev_step * q / 2.0)
			       ) {
				    // If b+p/q falls in [b,c] and
				    // isn't too large it is accepted
				    new_step = p / q;
			    }
			    // If p/q is too large then the bissection procedure can
			    // reduce [b,c] range to more extent
		    }

		    if (fabs(new_step) < tol_act) {	// Adjust the step to be not less
			    if (new_step > 0.0 * Abscissa())	// than tolerance
				    new_step = tol_act;
			    else
				    new_step = -tol_act;
		    }

		    a = b;
		    fa = fb;		// Save the previous approx.
		    b += new_step;
		    fb = ((*subject).*method) (b);	//  Do step to a new approxim.

		    if ((fb > 0.0 * Ordinate() && fc > 0.0 * Ordinate())
		        || (fb < 0.0 * Ordinate() && fc < 0.0 * Ordinate())) {
			    c = a;
			    fc = fa;	//  Adjust c for it to have a sign opposite to that of b
		    }
	    }
	    // (((we never arrive here)))
    }

    bool isSolved(void) {
	    return issolved;
    }

    bool isSolved(Ordinate error) {
	    if (fabs(fb) > error) {
		    return false;
	    }
	    return true;
    }

    Abscissa getSolution(void) {
	    return a;
    }

    Ordinate getError(void) {
	    return fb;
    }
};

#endif
