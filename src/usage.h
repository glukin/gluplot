/* -*- C++ -*- *************************************************************
 *
 *   usage.h, Wed Oct 20 13:53:13 MSD 2004
 *   Copyright (C) 2004 Soloviev Alexei <solovjev@cv.jinr.ru>
 *
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef USAGE_H
#define USAGE_H

#include "getopt.h"

/** long options for getopt */
extern struct option long_options[];

/** short options for getopt */
const char * short_options();

/** option index for getopt */
extern int option_index;

#include <map>
#include <string>
#include <iostream>
using namespace std;

typedef map < string, string > Section;
typedef map < string, Section > Book;

/** options handbook */
Book & book();

/** general usage */
void usage ( ostream & out = cerr );

#endif // USAGE_H
