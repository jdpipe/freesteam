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

/*
	two-way solver interface, for unusual property pairs that are not
	implemented in official IAPWS releases.

	this solver will use provided 'first guess' estimates, meaning that
	if correlations, even approximate, can be provided, then this will
	decrease unneeded iteration.

	these solvers first require that you know which region you're in.
	This is because the correlation variables are different for the different
	regions.

	'freesteam_region_XX functions will calculate the region for you; but
	these need to be hand-coded at this stage.
*/

int freesteam_solver2_region1(char X, char Y, double x, double y, SteamState *S);
int freesteam_solver2_region2(char X, char Y, double x, double y, SteamState *S);
int freesteam_solver2_region3(char X, char Y, double x, double y, SteamState *S);
int freesteam_solver2_region4(char X, char Y, double x, double y, SteamState *S);

