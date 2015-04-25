/* -*- C++ -*- *************************************************************
 *
 *   usage.cpp, Wed Oct 20 13:53:13 MSD 2004
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

#include "usage.h"

// gluplot long options for getopt
struct option long_options[] = {
  // common options: scale
  {"xlog", 0, 0, 'x'},
  {"ylog", 0, 0, 'y'},
  {"zlog", 0, 0, 'z'},
  // common options: decor
  {"no-ticks", 0, 0, 't'},
  {"grid", 0, 0, 'g'},
  {"key", 0, 0, 'k'},
  {"black", 0, 0, 'b'},
  // common options: color
  {"palette", 1, 0, 'p'},
  {"negative", 0, 0, 'n'},
  // common options: font
  {"font", 1, 0, 'f'},
  // common options: clip
  {"clip", 1, 0, 'c'},
  // plot options
  {"using", 1, 0, 'u'},
  {"with", 1, 0, 'w'},
  {"slice", 1, 0, 's'},
  {"3D", 0, 0, '3'},
  {"volume", 0, 0, 'v'},
  {"alpha", 1, 0, 'a'},
  // output
  {"output", 1, 0, 'o'},
  // info
  {"help", 0, 0, 'h'},
  {"version", 0, 0, 'V'},
  {0, 0, 0, 0}
};

// gluplot options handbook
Book & book()
{
  static Book fBook;

  if ( fBook.empty() ) {

    static Section fCommon;
    if ( fCommon.empty() ) {
      fCommon["xlog"] = "set logarithmic scale for x-axis [linear]";
      fCommon["ylog"] = "set logarithmic scale for y-axis [linear]";
      fCommon["zlog"] = "set logarithmic scale for z-axis [linear]";
      fCommon["no-ticks"] = "hide axis ticks [no]";
      fCommon["grid"] = "show grid [no]";
      fCommon["key"] = "show palette key [no]";
      fCommon["black"] = "set background black [white]";
      fCommon["palette"] = "set color palette [rainbow]";
      fCommon["negative"] = "invert palette [no]";
      fCommon["font"] = "set labels font [helvetica-12]";
      fCommon["clip"] = "set clipping plane [none]";
      fCommon["SectionArgument"] = "name";
    }
    fBook["Common"] = fCommon;

    static Section fPlot;
    if ( fPlot.empty() ) {
      fPlot["using"] = "set columns in file for plot [1:2]";
      fPlot["with"] = "set plot style [lines]";
      fPlot["slice"] = "set lines in file for plot (from:to:stride) [all]";
      fPlot["3D"] = "perform 3D plot [no]";
      fPlot["volume"] = "show volume in 3D plot [surface]";
      fPlot["alpha"] = "set surface transparency [1.0]";
      fPlot["SectionArgument"] = "string";
    }
    fBook["Plot"] = fPlot;

    static Section fOutput;
    if ( fOutput.empty() ) {
      fOutput["output"] = "output plot to file in EPS format [GLUT window]";
      fOutput["SectionArgument"] = "file";
    }
    fBook["Output"] = fOutput;

    static Section fHelp;
    if ( fHelp.empty() ) {
      fHelp["help"] = "display this help and exit";
      fHelp["version"] = "display version information and exit";
    }
    fBook["Help"] = fHelp;

  }
  return fBook;
}

#include <string>
using namespace std;

// short options for getopt
const char * short_options()
{
  option_index = 0;
  static string optstring;
  if ( optstring.empty() ) {
    option * optptr = long_options;
    while ( optptr->name != 0 ) {
      if ( optptr->val && ( isalpha ( optptr->val ) || isdigit ( optptr->val ) ) ) {
	optstring += optptr->val;
	if ( optptr->has_arg == 1 ) optstring += ':';
	if ( optptr->has_arg == 2 ) optstring += '?';
      }
      ++optptr;
    }
  }
  return optstring.c_str();
}

// option index for getopt
int option_index;

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <functional>
#include <cctype>
using namespace std;

// general usage
void usage ( ostream & out )
{
#ifdef PACKAGE
  out << "Usage: " << PACKAGE << " [options] [plot options] <file>" << endl;
#endif

  out << "Options: [defaults in brackets after descriptions]" << endl;

  static const int opt_width = 24;
  out.setf(ios::left,ios::adjustfield);

  Book::const_iterator sect = book().begin();
  while ( sect != book().end() ) {
    out << sect->first << " options:" << endl;
    option * optptr = long_options;
    while ( optptr->name != 0 ) {
      if ( book()[sect->first].find( optptr->name ) != 
	   book()[sect->first].end() ) {
	string optstr = "  ";
	if ( optptr->val && ( isalpha ( optptr->val ) || isdigit ( optptr->val ) ) ) {
	  ( ( optstr += "-" ) += optptr->val ) += ", ";
	}
	( optstr += "--" ) += optptr->name;
	if ( optptr->has_arg ) {
	  string arg = sect->first;
	  if ( book()[sect->first].find( "SectionArgument" ) !=
	       book()[sect->first].end() ) {
	    arg = book()[sect->first]["SectionArgument"];
	  }
	  transform ( arg.begin(), arg.end(), arg.begin(),
		      ptr_fun ( ::tolower ) );
	  ( ( optstr += " <" ) += arg ) += "> ";
	}
	out << setw(opt_width) << optstr.c_str();
	out << book()[sect->first][optptr->name] << endl;
      }
      ++optptr;
    }
    ++sect;
  }
}
