/* -*- C++ -*- *************************************************************
 *
 *   glp_window.cpp, Fri Oct  1 11:35:30 MSD 2004
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

#include "glp_window.h"

//#include <GL/gl.h>
//#include <GL/glu.h>
#include <GL/glut.h>

#include <algorithm>
#include <cmath>
using namespace std;

glp::window_traits::window_traits ( const char * title )
  : plot(),
    _title(title),
    _do_mark(false), _do_rotate(false),
    _do_move(false)
{}

glp::window_traits::~window_traits() {}

void
glp::window_traits::display()
{
  draw();

  if ( _do_mark ) {
    glEnable ( GL_BLEND );
    glBlendFunc ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    color marker = average ( foreground(), WHITE, 0.25 );
    glColor4d ( marker.r(), marker.g(), marker.b(), 0.3333333 );
    glRectd ( _x_min_mark, _y_min_mark,
	      _x_max_mark, _y_max_mark );

    if ( !xdistortion() && !ydistortion() ) {
      glColor3ub ( foreground().r, foreground().g, foreground().b );

      static label mark;
      mark.font ( font() );
      mark.align ( BOTTOM ); // mark shouldn't be hidden by mouse

      mark.x ( _x_min_mark );
      mark.y ( _y_min_mark );
      mark.text ( "(%.4g,%.4g)", _x_min_mark, _y_min_mark );
      mark.draw();

      mark.x ( _x_max_mark );
      mark.y ( _y_max_mark );
      mark.text ( "(%.4g,%.4g)", _x_max_mark, _y_max_mark );
      mark.draw();
    }
  }
}

void
glp::window_traits::reshape ( int width, int height )
{
  size ( width, height );
}

void
glp::window_traits::keyboard ( unsigned char key, int x, int y )
{
  menu ( key );
}

void
glp::window_traits::special ( int key, int x, int y ) {}

void
glp::window_traits::mouse ( int button, int state, int x, int y ) {}

void
glp::window_traits::motion ( int x, int y )
{
  if ( _do_mark ) {

    unproject ( x, y, _x_max_mark, _y_max_mark );

  }

  if ( _do_rotate ) {

    //unproject ( x, y, _x_max_mark, _y_max_mark );
    _x_max_mark = x;
    _y_max_mark = y;

//     if ( _x_max_mark > _x_min_mark ) zrotation ( zrotation() + 1.0 );
//     if ( _x_max_mark < _x_min_mark ) zrotation ( zrotation() - 1.0 );
    zrotation ( zrotation() + _x_max_mark - _x_min_mark );

//     if ( _y_max_mark > _y_min_mark ) xrotation ( xrotation() - 1.0 );
//     if ( _y_max_mark < _y_min_mark ) xrotation ( xrotation() + 1.0 );
    xrotation ( xrotation() - _y_max_mark + _y_min_mark );

    _x_min_mark = _x_max_mark;
    _y_min_mark = _y_max_mark;

  }

  if ( _do_move ) {

    double xold, yold, xnew, ynew;
    unproject ( _mouse_x, _mouse_y, xold, yold );
    unproject ( x, y, xnew, ynew );
    const double XLENGTH = abs ( _x_axis ( xhigh() ) - _x_axis ( xlow() ) );
    const double YLENGTH = abs ( _y_axis ( yhigh() ) - _y_axis ( ylow() ) );
    ( xnew -= xold ) /= XLENGTH;
    ( ynew -= yold ) /= YLENGTH;

    xoffsets ( xloff() + xnew, xhoff() - xnew );
    yoffsets ( yloff() + ynew, yhoff() - ynew );

    _mouse_x = x;
    _mouse_y = y;

  }
}

void
glp::window_traits::menu ( int option )
{
  switch ( option ) {

  // Scale...
  case 'x': xlog ( !xlog() ); break;
  case 'y': ylog ( !ylog() ); break;
  case 'z': zlog ( !zlog() ); break;

  // Decor...
  case 'a':
    xoffsets ( 0.05, 0.05 );
    yoffsets ( 0.05, 0.05 );
    break;
  case 't':
    ticks_visible ( !ticks_visible() );
    break;
  case 'g':
    grid_visible ( !grid_visible() );
    break;
  case 'k':
    key_visible ( !key_visible() );
    break;
  case 'c':
    style_visible ( !style_visible() );
    break;
  case 'b':
    {
      rgb back = background();
      rgb front = foreground();
      swap ( back, front );
      background ( back );
      foreground ( front );
    }
    break;
  case 'n':
    negative ( !negative() );
    break;

  // Style...
  case 1000 + DOTS:
  case 1000 + LINES:
  case 1000 + POINTS:
  case 1000 + LINESPOINTS:
  case 1000 + XERRORBARS:
  case 1000 + YERRORBARS:
  case 1000 + XYERRORBARS:
  case 1000 + XERRORLINES:
  case 1000 + YERRORLINES:
  case 1000 + XYERRORLINES:
  case 1000 + IMPULSES:
  case 1000 + COLOREDLINES:
  case 1000 + COLORMAP:
  case 1000 + ERRORBARS:
  case 1000 + ERRORLINES:
  case 1000 + LIGHTING:
    style ( static_cast<PLOT_STYLE> ( option - 1000 ) );
    break;

  // Color...
  case 2000 + RAINBOW:
  case 2000 + HOT:
  case 2000 + OCEAN:
  case 2000 + TERRAIN:
  case 2000 + TRADITIONAL:
  case 2000 + GRAYSCALE:
    rgbscheme ( static_cast<COLOR_SCHEME> ( option - 2000 ) );
    break;

  // Font...
  case 3000 + FIXED_8x13:
  case 3000 + FIXED_9x15:
  case 3000 + HELVETICA_10:
  case 3000 + HELVETICA_12:
  case 3000 + HELVETICA_18:
  case 3000 + TIMES_ROMAN_10:
  case 3000 + TIMES_ROMAN_24:
    font ( static_cast<TEXT_FONT> ( option - 3000 ) );
    break;

  // Main menu...
  case 's':
    save ( string(PACKAGE)+=".eps" );
    break;
  case 'q':
  case 27: /** Escape */
    exit(0);
    break;
  }
}

/***************************************************************************
 *                                                                         *
 *   just useful functions                                                 *
 *                                                                         *
 ***************************************************************************/

void
glp::unproject ( int xw, int yw, double & x, double & y )
{
  static double modvmatrix[16];
  static double projmatrix[16];
  static int viewport[4];

  glGetDoublev ( GL_MODELVIEW_MATRIX, modvmatrix );
  glGetDoublev ( GL_PROJECTION_MATRIX, projmatrix );
  glGetIntegerv ( GL_VIEWPORT, viewport );

  static double zw;
  static double z;

  gluUnProject ( (double)xw, (double)(viewport[3]-yw-1), zw,
		 modvmatrix, projmatrix, viewport, 
		 &x, &y, &z );
}

#include <time.h>  /* for random seed */

int
glp::fps()
{
  static int fps = 0;
  static int fpscount = 0;
  static time_t fpstime = 0;
  ++fpscount;
  time_t curtime = time(NULL);
  if ( curtime - fpstime ) {
    fps = ( fps + fpscount / ( curtime - fpstime ) ) / 2;
    fpstime  = curtime;
    fpscount = 0;
  }
  return fps;
}
