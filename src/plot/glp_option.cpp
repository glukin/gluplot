/* -*- C++ -*- *************************************************************
 *
 *   glp_option.cpp, Fri Nov 19 20:13:29 MSK 2004
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

#ifdef HAVE_CONFIG_H
# include <config.h>
#else
# define PACKAGE "gluplot"
#endif

#include "glp_option.h"

#include <cstring>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include <cctype>
using namespace std;

enum glp::PLOT_STYLE
glp::plot_style ( const char * key )
{
  static const struct {
    const char * key;
    enum PLOT_STYLE value;
  } plot_style_table[16+5] = {
    { "d$ots", DOTS },
    { "l$ines", LINES },
    { "p$oints", POINTS },
    { "linesp$oints", LINESPOINTS },
    { "xe$rrorbars", XERRORBARS },
    { "ye$rrorbars", YERRORBARS },
    { "xye$rrorbars", XYERRORBARS },
    { "xerrorl$ines", XERRORLINES },
    { "yerrorl$ines", YERRORLINES },
    { "xyerrorl$ines", XYERRORLINES },
    { "i$mpulses", IMPULSES },
    { "coloredl$ines", COLOREDLINES },
    { "colorm$ap", COLORMAP },
    { "e$rrorbars", ERRORBARS },
    { "errorl$ines", ERRORLINES },
    { "lig$hting", LIGHTING },
    // just for comfort
    { "lp", LINESPOINTS },
    { "e", YERRORBARS },
    { "el", YERRORLINES },
    { "cl", COLOREDLINES },
    { "cm", COLORMAP },
  };

  if ( key ) { // table lookup
    const int length = strlen ( key );
    for ( int i = 0; i < 21; ++i ) {
      const char * str = plot_style_table[i].key;
      register int num = 0, after = 0;
      for ( num = 0; num < length; ++num ) {
	if ( str[num] == '$' ) after = 1;
	if ( str[num+after] != key[num] ) break;
      }
      if ( str[num] == '$' || !str[num] ) after = 1;
      if ( after && num == length ) 
	return plot_style_table[i].value;
    }
  }

  // wrong key: display help and exit
  cerr << PACKAGE << ": ";
  if ( key ) cerr << "unknown plot style '" << key << "', ";
  else cerr << "missing plot style, ";
  cerr << "expecting " << endl;
  for ( int i = 0; i < 16; ++i ) {
    cerr << "'";
    for ( const char * p = plot_style_table[i].key; *p; ++p )
      if ( *p != '$' ) cerr << *p;
    cerr << "'";
    if ( i != 15 ) cerr << ", ";
    if ( i == 3 || i == 6 || i == 9 || i == 12 ) cerr << endl;
  }
  cerr << endl;

  exit ( 1 );
}

enum glp::TEXT_FONT
glp::text_font ( const char * fnt )
{
  static const struct {
    const char * key;
    enum TEXT_FONT value;
  } text_font_table[7] = {
    { "8x13", FIXED_8x13 },
    { "9x15", FIXED_9x15 },
    { "helvetica-10", HELVETICA_10 },
    { "helvetica-12", HELVETICA_12 },
    { "helvetica-18", HELVETICA_18 },
    { "times-roman-10", TIMES_ROMAN_10 },
    { "times-roman-24", TIMES_ROMAN_24 }
  };

  if ( fnt ) { // table lookup
    const int length = strlen ( fnt );
    for ( int i = 0; i < 7; ++i ) {
      const char * str = text_font_table[i].key;
      register int num = 0, after = 0;
      for ( num = 0; num < length; ++num ) {
	if ( str[num] == '$' ) after = 1;
	if ( str[num+after] != fnt[num] ) break;
      }
      if ( str[num] == '$' || !str[num] ) after = 1;
      if ( after && num == length ) 
	return text_font_table[i].value;
    }
  }

  // wrong key: display help and exit
  cerr << PACKAGE << ": ";
  if ( fnt ) cerr << "unknown text font '" << fnt << "', ";
  else cerr << "missing text font, ";
  cerr << "expecting " << endl;
  for ( int i = 0; i < 7; ++i ) {
    cerr << "'";
    for ( const char * p = text_font_table[i].key; *p; ++p )
      if ( *p != '$' ) cerr << *p;
    cerr << "'";
    if ( i != 6 ) cerr << ", ";
    if ( i == 1 || i == 4  ) cerr << endl;
  }
  cerr << endl;

  exit ( 1 );
}

enum glp::COLOR_SCHEME
glp::color_scheme ( const char * key )
{
  static const struct {
    const char * key;
    enum COLOR_SCHEME value;
  } color_scheme_table[6] = {
    { "r$ainbow", RAINBOW },
    { "h$ot", HOT },
    { "o$cean", OCEAN },
    { "ter$rain", TERRAIN },
    { "tr$aditional", TRADITIONAL },
    { "g$rayscale", GRAYSCALE }
  };

  if ( key ) { // table lookup
    const int length = strlen ( key );
    for ( int i = 0; i < 6; ++i ) {
      const char * str = color_scheme_table[i].key;
      register int num = 0, after = 0;
      for ( num = 0; num < length; ++num ) {
	if ( str[num] == '$' ) after = 1;
	if ( str[num+after] != key[num] ) break;
      }
      if ( str[num] == '$' || !str[num] ) after = 1;
      if ( after && num == length ) 
	return color_scheme_table[i].value;
    }
  }

  // wrong key: display help and exit
  cerr << PACKAGE << ": ";
  if ( key ) cerr << "unknown color scheme '" << key << "', ";
  else cerr << "missing color scheme, ";
  cerr << "expecting " << endl;
  for ( int i = 0; i < 6; ++i ) {
    cerr << "'";
    for ( const char * p = color_scheme_table[i].key; *p; ++p )
      if ( *p != '$' ) cerr << *p;
    cerr << "'";
    if ( i != 5 ) cerr << ", ";
  }
  cerr << endl;

  exit ( 1 );
}

const glp::curve
glp::curve_from_file ( const char * filename, const char * cols, const char * slice )
{
  curve line;

  ifstream in ( filename );
  if ( !in ) {
    cerr << PACKAGE << ": can't read data file \""
	 << filename << "\"" << endl;
    return line;
  }

  // defining columns
  unsigned int xcol = 0, ycol = 0, dycol = 0, dxcol = 0;
  if ( cols ) {
    const string str ( cols );
    const unsigned int waste = count_if ( str.begin(), str.end(),
					  not1 ( ptr_fun ( ::isdigit ) ) );
    static const char delimiter = ':';
    unsigned int ncols = count ( str.begin(), str.end(), delimiter );
    if ( ncols == waste ) ++ncols;
    else ncols = 0;
    istringstream optstr ( str );
    static char del;
    switch ( ncols ) {
    case 1:
      optstr >> ycol;
      break;
    case 2:
    case 3:
    case 4:
      ( optstr >> xcol ).clear();
      ( optstr >> del >> ycol ).clear();
      ( optstr >> del >> dycol ).clear();
      ( optstr >> del >> dxcol ).clear();
      break;
    default:
      cerr << PACKAGE << ": " << "expecting " 
  	   << "'x:y', 'x:y:dy', 'x:y:dy:dx' or just 'y'"
	   << endl;
      return line;
    }
  } else { // defaults
    xcol = 1, ycol = 2;
  }

  // defining lines
  unsigned int from = 0, to = 0, stride = 1;
  if ( slice ) {
    const string str ( slice );
    const unsigned int waste = count_if ( str.begin(), str.end(),
					  not1 ( ptr_fun ( ::isdigit ) ) );
    static const char delimiter = ':';
    unsigned int ncols = count ( str.begin(), str.end(), delimiter );
    if ( ncols == waste ) ++ncols;
    else ncols = 0;
    istringstream optstr ( str );
    static char del;
    switch ( ncols ) {
    case 3:
      ( optstr >> from ).clear();
      ( optstr >> del >> to ).clear();
      ( optstr >> del >> stride ).clear();
      break;
    default:
      cerr << PACKAGE << ": " << "expecting " 
  	   << "'from:to:stride'"
	   << endl;
      return line;
    }
  }
  unsigned int l = 0;

  // reading columns to curve
  double x = 0, y = 0, dy, dx;
  while ( in ) {
    string buff;
    char c;
    while ( in.get ( c ) && c != '\n' ) buff += c;
    ++l;
    if ( l < from ) continue;
    if ( ( l - from - 1 ) % stride ) continue;
    if ( to > from && l > to ) break;
    istringstream str ( buff );
    if ( str >> c && c != '#' ) str.putback ( c );
    else continue;
//      istream_iterator<double> iter ( str ), eof;
//      vector<double> record ( iter, eof );
    vector<double> record;
    string item;
    while ( str >> item ) record.push_back ( atof ( item.c_str() ) );
    if ( xcol > record.size() ||
	 ycol > record.size() ||
	 dycol > record.size() ||
	 dxcol > record.size() ) break;
    if ( xcol ) x = record[xcol-1];
    else x += 1;
    if ( ycol ) y = record[ycol-1];
    else y += 1;
    line.push_back ( point ( x, y ) );
    if ( dycol ) {
      dy = record[dycol-1];
      line.rbegin()->yerrorbars ( dy, dy );
    }
    if ( dxcol ) {
      dx = record[dxcol-1];
      line.rbegin()->xerrorbars ( dx, dx );
    }
  }
  if ( line.empty() ) {
    cerr << PACKAGE << ": no data point found in ";
    if ( cols ) cerr << cols << " column(s) of ";
    cerr << "specified file \"" << filename << "\"" << endl;
    return line;
  }
  return line;
}

const glp::surface
glp::surface_from_file ( const char * filename, const char * cols, const char * slice )
{
  surface plane;

  ifstream in ( filename );
  if ( !in ) {
    cerr << PACKAGE << ": can't read data file \""
	 << filename << "\"" << endl;
    return plane;
  }

  // defining columns
  unsigned int xcol = 0, ycol = 0, zcol = 0, dxcol = 0, dycol = 0, dzcol = 0;
  if ( cols ) {
    const string str ( cols );
    const unsigned int waste = count_if ( str.begin(), str.end(),
					  not1 ( ptr_fun ( ::isdigit ) ) );
    static const char delimiter = ':';
    unsigned int ncols = count ( str.begin(), str.end(), delimiter );
    if ( ncols == waste ) ++ncols;
    else ncols = 0;
    istringstream optstr ( str );
    static char del;
    switch ( ncols ) {
    case 3:
    case 6:
      ( optstr >> xcol ).clear();
      ( optstr >> del >> ycol ).clear();
      ( optstr >> del >> zcol ).clear();
      ( optstr >> del >> dxcol ).clear();
      ( optstr >> del >> dycol ).clear();
      ( optstr >> del >> dzcol ).clear();
      break;
    default:
      cerr << PACKAGE << ": " << "expecting " 
  	   << "'x:y:z' or 'x:y:z:dx:dy:dz'"
	   << endl;
      return plane;
    }
  } else { // defaults
    xcol = 1, ycol = 2, zcol = 3;
  }

  // defining lines
  unsigned int from = 0, to = 0, stride = 1;
  if ( slice ) {
    const string str ( slice );
    const unsigned int waste = count_if ( str.begin(), str.end(),
					  not1 ( ptr_fun ( ::isdigit ) ) );
    static const char delimiter = ':';
    unsigned int ncols = count ( str.begin(), str.end(), delimiter );
    if ( ncols == waste ) ++ncols;
    else ncols = 0;
    istringstream optstr ( str );
    static char del;
    switch ( ncols ) {
    case 3:
      ( optstr >> from ).clear();
      ( optstr >> del >> to ).clear();
      ( optstr >> del >> stride ).clear();
      break;
    default:
      cerr << PACKAGE << ": " << "expecting " 
  	   << "'from:to:stride'"
	   << endl;
      return plane;
    }
  }
  unsigned int l = 0;

  // reading columns to surface
  double x = 0, y = 0, z = 0, dy, dx, dz;
  while ( in ) {
    string buff;
    char c;
    while ( in.get ( c ) && c != '\n' ) buff += c;
    ++l;
    if ( l < from ) continue;
    if ( ( l - from - 1 ) % stride ) continue;
    if ( to > from && l > to ) break;
    istringstream str ( buff );
    if ( str >> c && c != '#' ) str.putback ( c );
    else continue;
//      istream_iterator<double> iter ( str ), eof;
//      vector<double> record ( iter, eof );
    vector<double> record;
    string item;
    while ( str >> item ) record.push_back ( atof ( item.c_str() ) );
    if ( xcol > record.size() ||
	 ycol > record.size() ||
	 zcol > record.size() ||
	 dxcol > record.size() ||
	 dycol > record.size() ||
	 dzcol > record.size() ) break;
    if ( xcol ) x = record[xcol-1];
    else x += 1;
    if ( ycol ) y = record[ycol-1];
    else y += 1;
    if ( zcol ) z = record[zcol-1];
    else z += 1;
    plane.push_back ( point ( x, y, z ) );
    if ( dxcol ) {
      dx = record[dxcol-1];
      plane.rbegin()->xerrorbars ( dx, dx );
    }
    if ( dycol ) {
      dy = record[dycol-1];
      plane.rbegin()->yerrorbars ( dy, dy );
    }
    if ( dzcol ) {
      dz = record[dzcol-1];
      plane.rbegin()->zerrorbars ( dz, dz );
    }
  }
  if ( plane.empty() ) {
    cerr << PACKAGE << ": no data point found in ";
    if ( cols ) cerr << cols << " column(s) of ";
    cerr << "specified file \"" << filename << "\"" << endl;
    return plane;
  }
  return plane;
}

void
glp::plane_from_str ( const char * key, point & dir, double & pos )
{
  const string str ( key );
  static const char delimiter = ',';
  unsigned int ncols = count ( str.begin(), str.end(), delimiter );
  ++ncols;
  if ( ncols == 4 ) {
    istringstream optstr ( str );
    static double x, y, z;
    static char del;
    ( optstr >> x ).clear();
    ( optstr >> del >> y ).clear();
    ( optstr >> del >> z ).clear();
    ( optstr >> del >> pos ).clear();
    dir.x ( x );
    dir.y ( y );
    dir.z ( z );
  } else {
    cerr << PACKAGE << ": " << "expecting " 
	 << "'x,y,z,d'"
	 << endl;
    exit ( 1 );
  }
}
