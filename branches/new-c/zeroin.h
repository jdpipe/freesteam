/*
freesteam - IAPWS-IF97 steam tables library
Copyright (C) 2004-2009  John Pye

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#ifndef FREESTEAM_ZEROIN_H
#define FREESTEAM_ZEROIN_H

typedef double ZeroInSubjectFunction(double, void *user_data);

/**
	Attempt to solve the function y = f(x) = 0 by varying x between
	a lower and upper bound, using the Brent algorithm.

	Originally based on brent solver from netlib, then converted to C++ for
	used in earlier freesteam versions, and now converted back to pure C again.
	@see brent.shar at http://www.netlib.org/c/

	@param func the function being solved
	@param lowerbound the lower bound of the range in which a root is sought
	@param upperbound the upper bound of the range in which a root is sought
	@param tol maximum permissible magnitude of the function at the solved root location
	@param user_data additional data that will be passed to the subject function func.
	@param solution (returned) the value of 'x' at the solution
	@param error (returned) the value of 'y' at the solution.
	@return 0 on success
*/

char zeroin_solve(ZeroInSubjectFunction *func, void *user_data, double lowerbound, double upperbound, double tol, double *solution, double *error);

#endif

