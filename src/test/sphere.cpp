/* -*- C++ -*- *************************************************************
 *
 *   sphere.cpp, Tue Nov  6 15:37:05 MSK 2007
 *   Copyright (C) 2007 Soloviev Alexei <solovjev@cv.jinr.ru>
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
#include <numeric>
using namespace std;

int main ( int argc, char** argv )
{
  if ( argc > 2 ) {
    cerr << "Usage: " << argv[0] << " [npoints]" << endl;
    return 1;
  }

  gluplot pout;

  //pout.key_visible ( true );
  pout.key_visible ( false );

  pout.background ( BLACK );
  pout.foreground ( WHITE );

  pout.rgbscheme ( TRADITIONAL );

  surface s;

  int npoints = 256; // 1024;
  if ( argc == 2 ) npoints = atoi ( argv[1] );

  for ( int i = 0; i < npoints; ++i ) {
    double radius = 1;
    double phi = ( 2 * (double) rand() / RAND_MAX - 1 ) * M_PI;
    double theta = ( 2 * (double) rand() / RAND_MAX - 1 ) * M_PI / 2;
    double x = radius * cos ( theta ) * cos ( phi );
    double y = radius * cos ( theta ) * sin ( phi );
    double z = radius * sin ( theta );
    s.push_back ( point ( x, y, z ) );
  }

  s.triangulate_spherically ( true );
  //s.alpha ( 0.5 );

  //pout << COLORMAP << s;
  pout << LIGHTING << s;

  point c = accumulate ( s.begin(), s.end(), point ( 0, 0, 0 ) ) / ( s.end() - s.begin() );
  double area = 0;
  double volume = 0;
  for ( int i = 0; i < s.size(); ++i ) {
    point v1 = s[i][1] - s[i][0];
    point v2 = s[i][2] - s[i][0];
    point n = cross_product ( v1, v2 );
    double norm = glp::distance ( n, point ( 0, 0, 0 ) );
    area += 0.5 * norm;
    //n = n / norm;
    point p = ( s[i][0] + s[i][1] + s[i][2] ) / 3;
    point r = c - p;
    //double height = scalar_product ( r, n );
    //volume += 0.5 * norm * abs ( height ) / 3;
    volume += 0.5 * abs ( scalar_product ( r, n ) ) / 3;
  }
  cout << "sphere area = " << area 
       << " (should be " << M_PI * 4 << ")" << endl;
  cout << "sphere volume = " << volume 
       << " (should be " << M_PI * 4 / 3 << ")" << endl;

  return 0;
}
