/* -*- C++ -*- *************************************************************
 *
 *   rainbow.cpp, Mon Dec  6 16:05:37 MSK 2004
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

#include "gluplot.h"
using namespace glp;

namespace glp {

  /** original fixed color transformations */

  double rainbow_r ( double x );
  double rainbow_g ( double x );
  double rainbow_b ( double x );

} /** namespace glp */

int main ( int argc, char** argv )
{
  gluplot pout;

  pout.grid_visible ( true );
  pout.key_visible ( true );

  pout.background ( BLACK );
  pout.foreground ( WHITE );

  curve cr, cg, cb;

  for ( double x = 0; x <= 1; x += 0.01 ) {
    cr.push_back ( point ( x, glp::rainbow_r ( x ) ) );
    cg.push_back ( point ( x, glp::rainbow_g ( x ) ) );
    cb.push_back ( point ( x, glp::rainbow_b ( x ) ) );
  }

  pout << cr << cg << cb;

  return 0;
}

