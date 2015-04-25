/* -*- C++ -*- *************************************************************
 *
 *   main.cpp, Wed Oct 20 08:44:37 MSD 2004
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
# define VERSION "1.x"
#endif

#include "gluplot.h"
#include "usage.h"

#ifdef HAVE_FLTK
#include "gui.h"
#endif

#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>
using namespace std;

int main ( int argc, char** argv )
{
  if ( argc == 1 ) {
#ifdef HAVE_FLTK
    glpUI w;
    w.show ( argc, argv );
    return Fl::run();
#else
    cerr << PACKAGE << ": missing arguments" << endl;
    cerr << "Try \"" << PACKAGE << " --help\" for more information." << endl;
    return 1;
#endif
  }

  gluplot pout;
  string output;

  // prevent graphics before end
  pout << glp::KILL;

  int c = 0;

  // getopt first pass: common options
  //char* argv1[argc];
  char** argv1 = new char*[argc];
  copy ( argv, argv+argc, argv1 );
  while ( c != -1 ) {
    c = getopt_long ( argc, argv1, 
                      short_options(), long_options, 
                      &option_index );
    switch (c) {
    case 'x': pout.xlog ( true ); break;
    case 'y': pout.ylog ( true ); break;
    case 'z': pout.zlog ( true ); break;

    case 't': pout.ticks_visible ( false ); break;
    case 'g': pout.grid_visible ( true ); break;
    case 'k': pout.key_visible ( true ); break;

    case 'b':
      pout.background ( glp::BLACK );
      pout.foreground ( glp::WHITE );
      break;

    case 'p': pout.rgbscheme ( glp::color_scheme ( optarg ) ); break;
    case 'n': pout.negative ( true ); break;

    case 'f': pout.font ( glp::text_font ( optarg ) ); break;

    case 'o': output = optarg; break;

    case 'c':
      pout.clipping ( true );
      {
	glp::point dir;
	double pos;
	glp::plane_from_str ( optarg, dir, pos );
	pout.clip ( dir, pos );
      }
      break;

    case 'V':
      cout << PACKAGE << " version " << VERSION << endl;
      return 0;
      break;

    case 'h':
      usage();
      return 0;
      break;

    default:
      break;
    }
  }

  if ( optind == argc ) {
    cerr << PACKAGE << ": no input file(s) specified"<< endl;
    return 1;
  }

  int nfiles = argc - optind;
  //int filepos[nfiles];
  int * filepos = new int[nfiles];
  for ( int i = 0; i < nfiles; ++i )
    filepos[i] = find ( argv, argv+argc, argv1[optind++] ) - argv;

  // getopt second pass: plot options
  for ( int i = 0; i < nfiles; ++i ) {
    const char * cols = 0;
    const char * slice = 0;
    const char * style = "lines";
    bool _3D = false;
    bool _volume = false;
    double _alpha = 1.0;
    int shift = i==0 ? 0 : filepos[i-1];
    c = 0;
    option_index = optind = 0;
    while ( c != -1 ) {
      c = getopt_long ( filepos[i]-shift, argv+shift, 
			short_options(), long_options, 
			&option_index );
      switch (c) {
      case 'u':
	cols = optarg;
	break;

      case 'w':
	style = optarg;
	break;

      case 's':
	slice = optarg;
	break;

      case '3':
	_3D = true;
	break;

      case 'v':
	_volume = true;
	break;

      case 'a':
	_alpha = atof ( optarg );
	break;

      default:
	break;
      }
    }

    if ( _3D ) {
      glp::surface s = glp::surface_from_file ( argv[filepos[i]], cols, slice );
      if ( _volume ) s.triangulate_spherically ( true );
      s.alpha ( _alpha );
      pout << glp::plot_style ( style ) << s;
    } else {
      glp::curve c = glp::curve_from_file ( argv[filepos[i]], cols, slice );
      pout << glp::plot_style ( style ) << c;
    }
  }

  if ( !output.empty() ) {
    pout.save ( output );
  }

  delete [] argv1;
  delete [] filepos;

  return 0;
}
