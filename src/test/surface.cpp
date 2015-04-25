/* -*- C++ -*- *************************************************************
 *
 *   surface.cpp, Fri Jan 14 17:16:47 MSK 2005
 *   Copyright (C) 2005 Soloviev Alexei <solovjev@cv.jinr.ru>
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

#include "gluplot.h"
using namespace glp;

#include <iostream>
#include <cstdlib>
#include <cmath>
using namespace std;

const double func ( const double & x, const double & y )
{
  double arg = sqrt ( x*x + y*y );
  return arg ? sin ( arg ) / arg : 1;
}

int main ( int argc, char** argv )
{
  if ( argc > 2 ) {
    cerr << "Usage: " << argv[0] << " [npoints]" << endl;
    return 1;
  }

  gluplot pout;

  pout.key_visible ( true );

  pout.background ( BLACK );
  pout.foreground ( WHITE );

  surface s;

  int npoints = 1024;
  if ( argc == 2 ) npoints = atoi ( argv[1] );

  const double low = -10;
  const double high = 10;
  for ( int i = 0; i < npoints; ++i ) {
    double x = low + ( high - low ) * (double) rand() / RAND_MAX;
    double y = low + ( high - low ) * (double) rand() / RAND_MAX;
    s.push_back ( point ( x, y, func ( x, y ) ) );
  }

  //s.alpha ( 0.5 );

  pout << COLORMAP << s;

  return 0;
}

