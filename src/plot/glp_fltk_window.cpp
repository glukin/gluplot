/* -*- C++ -*- *************************************************************
 *
 *   glp_fltk_window.cpp, Wed Oct 24 16:20:45 MSD 2007
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

#include "glp_fltk_window.h"

#include <GL/glut.h>

#include <algorithm>
#include <cmath>
using namespace std;

glp::fl_window::fl_window ( int x, int y, int w, int h, const char * l )
  : window_traits ( l ),
    Fl_Gl_Window ( x, y, w, h, l )
//     _fullscreen(false)
{
//   static int argc = 1;
//   static char * argv[] = {PACKAGE};
//   glutInit ( &argc, argv );
  static int argc = 0;
  static char * argv[] = {PACKAGE};
  if ( !argc ) {
    argc = 1;
    glutInit ( &argc, argv );
  }
}

glp::fl_window::~fl_window() {}

void
glp::fl_window::special ( int key, int x, int y )
{
  switch ( key ) {
  case FL_Down:
    xrotation ( xrotation() - 1.0 );
    break;
  case FL_Up:
    xrotation ( xrotation() + 1.0 );
    break;
  case FL_Left:
    zrotation ( zrotation() - 1.0 );
    break;
  case FL_Right:
    zrotation ( zrotation() + 1.0 );
    break;
  case FL_Home:
    xrotation ( 0.0 );
    yrotation ( 0.0 );
    zrotation ( 0.0 );
    break;
  default:
    break;
  }
//   size ( _width, _height );
}

void
glp::fl_window::mouse ( int button, int state, int x, int y )
{
  if ( button == FL_LEFT_MOUSE ) {
    if ( state == FL_PUSH ) {
      if ( _planes.empty() ) {
	unproject ( x, y, _x_min_mark, _y_min_mark );
	if ( _x_min_mark > xlowend() && _x_min_mark < xhighend() && 
	     _y_min_mark > ylowend() && _y_min_mark < yhighend() ) {
	  _x_max_mark = _x_min_mark;
	  _y_max_mark = _y_min_mark;
	  _do_mark = true;
	} else {
	  xoffsets ( 0.05, 0.05 );
	  yoffsets ( 0.05, 0.05 );
	}
      } else {
	//unproject ( x, y, _x_min_mark, _y_min_mark );
	_x_min_mark = x;
	_y_min_mark = y;
	_x_max_mark = _x_min_mark;
	_y_max_mark = _y_min_mark;
	_do_rotate = true;
      }
    } else { // state == FL_PUSH
      if ( _planes.empty() ) {
	unproject ( x, y, _x_max_mark, _y_max_mark );
	if ( _x_max_mark > xlowend() && _x_max_mark < xhighend() && 
	     _y_max_mark > ylowend() && _y_max_mark < yhighend() ) {
	  if ( _x_min_mark > _x_max_mark ) swap ( _x_min_mark, _x_max_mark );
	  if ( _y_min_mark > _y_max_mark ) swap ( _y_min_mark, _y_max_mark );
	  double XMIN = _x_axis ( xlow() ), XMAX = _x_axis ( xhigh() );
	  double YMIN = _y_axis ( ylow() ), YMAX = _y_axis ( yhigh() );
	  if ( XMIN > XMAX ) swap ( XMIN, XMAX );
	  if ( YMIN > YMAX ) swap ( YMIN, YMAX );
	  const double XLENGTH = XMAX - XMIN;
	  const double YLENGTH = YMAX - YMIN;
	  if ( _x_max_mark - _x_min_mark > 0.01 * XLENGTH &&
	       _y_max_mark - _y_min_mark > 0.01 * YLENGTH ) {
	    ( XMIN -= _x_min_mark ) /= XLENGTH;
	    ( XMAX -= _x_max_mark ) /= XLENGTH;
	    ( YMIN -= _y_min_mark ) /= YLENGTH;
	    ( YMAX -= _y_max_mark ) /= YLENGTH;
	    xoffsets ( XMIN, -XMAX );
	    yoffsets ( YMIN, -YMAX );
	  }
	}
	_do_mark = false;
      } else {
	//unproject ( x, y, _x_max_mark, _y_max_mark );
	_x_max_mark = x;
	_y_max_mark = x;
	_do_rotate = false;
      }
    }
  }

  if ( button == FL_MIDDLE_MOUSE ) {
    if ( state == FL_PUSH ) {
      if ( _planes.empty() ) {
	_mouse_x = x;
	_mouse_y = y;
	_do_move = true;
      } else {
	xrotation ( 0.0 );
	yrotation ( 0.0 );
	zrotation ( 0.0 );
      }
    } else { // state == FL_PUSH
      if ( _planes.empty() ) {
	_do_move = false;
      }
    }
  }

}

// void
// glp::fl_window::menu ( int option )
// {
//   window_traits::menu ( option );
//   switch ( option ) {
//   case 'f':
//     _fullscreen = !_fullscreen;
//     if ( _fullscreen ) {
//       _x4fs = x();
//       _y4fs = y();
//       _w4fs = w();
//       _h4fs = h();
//       fullscreen();
//     } else {
//       fullscreen_off ( _x4fs, _y4fs, _w4fs, _h4fs );
//     }
//     break;
//   }
// }

/***************************************************************************
 *                                                                         *
 *   callbacks for fltk                                                    *
 *                                                                         *
 ***************************************************************************/

void
glp::fl_window::draw()
{
  window_traits::display();
  Fl_Gl_Window::draw();
}

void
glp::fl_window::resize ( int x, int y, int w, int h )
{
  window_traits::reshape ( w, h );
  Fl_Gl_Window::resize ( x, y, w, h );
}

int
glp::fl_window::handle ( int event )
{
  switch ( event ) {
  case FL_FOCUS :
  case FL_UNFOCUS :
    return 1;
  case FL_KEYBOARD:
    keyboard ( *Fl::event_text(), x(), y() );
    special ( Fl::event_key(), x(), y() );
    damage ( FL_DAMAGE_ALL );
    return 1;
  case FL_PUSH:
  case FL_RELEASE:
    mouse ( Fl::event_button(), 
	    event,
	    Fl::event_x(), Fl::event_y() );
    damage ( FL_DAMAGE_ALL );
    return 1;
  case FL_DRAG:
    motion ( Fl::event_x(), Fl::event_y() );
    damage ( FL_DAMAGE_ALL );
    return 1;
  default:
    // pass other events to the base class...
    return Fl_Gl_Window::handle ( event );
  }
}

