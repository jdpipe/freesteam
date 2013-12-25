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

#include "fstm.h"
#include <xlw/XlfException.h>

/*
 * return freesteam_region as a double for fstm_PtrType_S compatibility
 */
double freesteam_region_double(SteamState S)
{
    return (double)freesteam_region(S) ;
}

/*
 * Custom exit function: stop the xll without crashing Excel
 */
void exit(int status)
{
    THROW_XLW("#ERROR freesteam exit (" << status << ")") ;
}

