# freesteam - IAPWS-IF97 steam tables library
# Copyright (C) 2004-2005  John Pye
# 
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#
# platform specific options for i686-pc-cygwin
#

EXE_SUFFIX = .exe
SO_SUFFIX = .dll
SO_PREFIX = 

PLATFORM_CXX_FLAGS = -DWIN32
PLATFORM_LD_FLAGS = -L/usr/lib -lc
PLATFORM_CPP_FLAGS = -fmessage-length=170 -DHAVE_CPPUNIT
