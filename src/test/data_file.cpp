/* -*- C++ -*- *************************************************************
 *
 *   data_file.cpp, Mon Dec  6 16:21:01 MSK 2004
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

int main ( int argc, char** argv )
{
  gluplot pout;
  pout.grid_visible ( true );

  curve data = curve_from_file ( "test.dat", "1:2:3" );
  curve fit = curve_from_file ( "test.fit" );

  pout << YERRORBARS << data
       << LINES << fit;

  return 0;
}

