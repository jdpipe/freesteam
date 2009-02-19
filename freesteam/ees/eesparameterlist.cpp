/* EESParameterList.cpp : Implementation of EESParameterList utilities.
 *
 * This file provides type definition and a useful class for
 * handling EES parameters record in DLF and DLP files.
 *
 * You can freely use this file in your own applications.
 *
 * June 2001
 *
 * Bruno ROSE - b.rose@sherpa-eng.com
 * Sherpa Engineering
 * 269-287, rue de la Garenne
 * 92000 Nanterre - France
 *
 */

#include "EESParameterList.h"

/// Constructor initialize the 'begin' pointer to the first record
EESParameterList::EESParameterList(EES_PARAM_REC * begin_init)
		: begin(begin_init)
{
}


/// Return list length (useful to get number of I/O)
int
EESParameterList::length(){

	EES_PARAM_REC *i_rec = begin;
	int len = 0;

	while (i_rec != 0)
	{
		i_rec = i_rec->next;
		len++;
	}

	return len;
}



/// Overloaded subscript operator to access I/O for r/w operations
double &
EESParameterList::operator[] (const int &i){

	EES_PARAM_REC *i_rec = begin;
	int j = 0;

	while (j<i)
	{
		i_rec = i_rec->next;
		j++;
	}

	return i_rec->value;
}

