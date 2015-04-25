/* -*- C++ -*- *************************************************************
 *
 *   glp_glut_window.cpp, Fri Oct  1 11:35:30 MSD 2004
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

#include "glp_glut_window.h"

#include <GL/glut.h>

#include <algorithm>
#include <cmath>
using namespace std;

glp::window::window ( const char * title )
  : window_traits(title),
    _fullscreen(false),
    _menu_in_use(false)
{
  _init();
  //_create();
}

glp::window::~window()
{
  if ( style() != KILL ) {
    //_init();
    _create();
    // correct menu items and run!
    menu ( 0 );
    glutMainLoop();
  }
}

void
glp::window::display()
{
  window_traits::display();
  glutSwapBuffers();
}

void
glp::window::reshape ( int width, int height )
{
  window_traits::reshape ( width, height );
  glutPostRedisplay();
}

void
glp::window::keyboard ( unsigned char key, int x, int y )
{
  window_traits::keyboard ( key, x, y );
  glutPostRedisplay();
}

void
glp::window::special ( int key, int x, int y )
{
  window_traits::special ( key, x, y );
  switch ( key ) {
  case GLUT_KEY_DOWN:
    xrotation ( xrotation() - 1.0 );
    break;
  case GLUT_KEY_UP:
    xrotation ( xrotation() + 1.0 );
    break;
  case GLUT_KEY_LEFT:
    zrotation ( zrotation() - 1.0 );
    break;
  case GLUT_KEY_RIGHT:
    zrotation ( zrotation() + 1.0 );
    break;
  case GLUT_KEY_HOME:
    xrotation ( 0.0 );
    yrotation ( 0.0 );
    zrotation ( 0.0 );
    break;
  default:
    break;
  }
//   size ( _width, _height );
  glutPostRedisplay();
}

void
glp::window::mouse ( int button, int state, int x, int y )
{
  if ( _menu_in_use ) return;

  if ( button == GLUT_LEFT_BUTTON ) {
    if ( state == GLUT_DOWN ) {
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
    } else { // state == GLUT_UP
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

  if ( button == GLUT_MIDDLE_BUTTON ) {
    if ( state == GLUT_DOWN ) {
      if ( _planes.empty() ) {
	_mouse_x = x;
	_mouse_y = y;
	_do_move = true;
      } else {
	xrotation ( 0.0 );
	yrotation ( 0.0 );
	zrotation ( 0.0 );
      }
    } else { // state == GLUT_UP
      if ( _planes.empty() ) {
	_do_move = false;
      }
    }
  }

  glutPostRedisplay();
}

void
glp::window::motion ( int x, int y )
{
  window_traits::motion ( x, y );
  glutPostRedisplay();
}

void
glp::window::menu ( int option )
{
  window_traits::menu ( option );

  switch ( option ) {
  case 'f':
    _fullscreen = !_fullscreen;
    if ( _fullscreen ) {
      _x4fs = glutGet ( GLUT_WINDOW_X );
      _y4fs = glutGet ( GLUT_WINDOW_Y );
      _w4fs = glutGet ( GLUT_WINDOW_WIDTH );
      _h4fs = glutGet ( GLUT_WINDOW_HEIGHT );
      glutFullScreen();
    } else {
      glutPositionWindow ( _x4fs, _y4fs );
      glutReshapeWindow ( _w4fs, _h4fs );
    }
    break;
  case 'p':
    clipping ( !clipping() );
    break;
  default:
    break;
  }

  // "Scale..." menu update
  glutSetMenu ( _scale_menu );
  if ( xlog() ) {
    glutChangeToMenuEntry ( 1, "[x]-axis: log->lin",      'x' );
  } else {
    glutChangeToMenuEntry ( 1, "[x]-axis: lin->log",      'x' );
  }
  if ( ylog() ) {
    glutChangeToMenuEntry ( 2, "[y]-axis: log->lin",      'y' );
  } else {
    glutChangeToMenuEntry ( 2, "[y]-axis: lin->log",      'y' );
  }
  if ( zlog() ) {
    glutChangeToMenuEntry ( 3, "[z]-axis: log->lin",      'z' );
  } else {
    glutChangeToMenuEntry ( 3, "[z]-axis: lin->log",      'z' );
  }

  // "Decor..." menu update
  glutSetMenu ( _decor_menu );
  if ( ticks_visible() ) {
    glutChangeToMenuEntry ( 2, "[t]icks: yes",            't' );
  } else {
    glutChangeToMenuEntry ( 2, "[t]icks: no",            't' );
  }
  if ( grid_visible() ) {
    glutChangeToMenuEntry ( 3, "[g]rid: yes",              'g' );
  } else {
    glutChangeToMenuEntry ( 3, "[g]rid: no",              'g' );
  }
  if ( key_visible() ) {
    glutChangeToMenuEntry ( 4, "[k]ey: yes",               'k' );
  } else {
    glutChangeToMenuEntry ( 4, "[k]ey: no",               'k' );
  }
  if ( style_visible() ) {
    glutChangeToMenuEntry ( 5, "[c]ommon style: yes",      'c' );
  } else {
    glutChangeToMenuEntry ( 5, "[c]ommon style: no",      'c' );
  }
  if ( background() == BLACK ) {
    glutChangeToMenuEntry ( 6, "[b]lack background: yes",  'b' );
  } else {
    glutChangeToMenuEntry ( 6, "[b]lack background: no",  'b' );
  }
  if ( negative() ) {
    glutChangeToMenuEntry ( 7, "[n]egative: yes",          'n' );
  } else {
    glutChangeToMenuEntry ( 7, "[n]egative: no",          'n' );
  }
  if ( _fullscreen ) {
    glutChangeToMenuEntry ( 8, "[f]ullscreen: yes",        'f' );
  } else {
    glutChangeToMenuEntry ( 8, "[f]ullscreen: no",        'f' );
  }
  if ( clipping() ) {
    glutChangeToMenuEntry ( 9, "cli[p]ping: yes",          'p' );
  } else {
    glutChangeToMenuEntry ( 9, "cli[p]ping: no",          'p' );
  }

  glutPostRedisplay();
}

/***************************************************************************
 *                                                                         *
 *   callbacks for glut                                                    *
 *                                                                         *
 ***************************************************************************/

void
glp::window::_create()
{
  glutInitWindowSize ( _width, _height );
  glutInitWindowPosition ( glutGet ( GLUT_INIT_WINDOW_X ),
			   glutGet ( GLUT_INIT_WINDOW_Y ) );

  //glutInitDisplayMode ( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_ALPHA );
  glutInitDisplayMode ( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );

  _registry[ glutCreateWindow ( _title ) ] = this;

  glutDisplayFunc ( _display );
  glutReshapeFunc ( _reshape );

  glutKeyboardFunc ( _keyboard );
  glutSpecialFunc ( _special );
  glutMouseFunc ( _mouse );
  glutMotionFunc ( _motion );

  _scale_menu = glutCreateMenu ( _menu );
  glutAddMenuEntry ( "[x]-axis: lin->log",      'x' );
  glutAddMenuEntry ( "[y]-axis: lin->log",      'y' );
  glutAddMenuEntry ( "[z]-axis: lin->log",      'z' );

  _decor_menu = glutCreateMenu ( _menu );
  glutAddMenuEntry ( "[a]utoscale",             'a' );
  glutAddMenuEntry ( "[t]icks: yes",            't' );
  glutAddMenuEntry ( "[g]rid: no",              'g' );
  glutAddMenuEntry ( "[k]ey: no",               'k' );
  glutAddMenuEntry ( "[c]ommon style: no",      'c' );
  glutAddMenuEntry ( "[b]lack background: no",  'b' );
  glutAddMenuEntry ( "[n]egative: no",          'n' );
  glutAddMenuEntry ( "[f]ullscreen: no",        'f' );
  glutAddMenuEntry ( "cli[p]ping: no",          'p' );

  _style_menu = glutCreateMenu ( _menu );
  glutAddMenuEntry ( "dots",                1000 + DOTS );
  glutAddMenuEntry ( "lines",               1000 + LINES );
  glutAddMenuEntry ( "points",              1000 + POINTS );
  glutAddMenuEntry ( "linespoints",         1000 + LINESPOINTS );
  glutAddMenuEntry ( "xerrorbars",          1000 + XERRORBARS );
  glutAddMenuEntry ( "yerrorbars",          1000 + YERRORBARS );
  glutAddMenuEntry ( "xyerrorbars",         1000 + XYERRORBARS );
  glutAddMenuEntry ( "xerrorlines",         1000 + XERRORLINES );
  glutAddMenuEntry ( "yerrorlines",         1000 + YERRORLINES );
  glutAddMenuEntry ( "xyerrorlines",        1000 + XYERRORLINES );
  glutAddMenuEntry ( "impulses",            1000 + IMPULSES );
  glutAddMenuEntry ( "coloredlines",        1000 + COLOREDLINES );
  glutAddMenuEntry ( "colormap",            1000 + COLORMAP );
  glutAddMenuEntry ( "errorbars",           1000 + ERRORBARS );
  glutAddMenuEntry ( "errorlines",          1000 + ERRORLINES );
  glutAddMenuEntry ( "lighting",            1000 + LIGHTING );

  _color_menu = glutCreateMenu ( _menu );
  glutAddMenuEntry ( "rainbow",             2000 + RAINBOW );
  glutAddMenuEntry ( "hot",                 2000 + HOT );
  glutAddMenuEntry ( "ocean",               2000 + OCEAN );
  glutAddMenuEntry ( "terrain",             2000 + TERRAIN );
  glutAddMenuEntry ( "traditional",         2000 + TRADITIONAL );
  glutAddMenuEntry ( "grayscale",           2000 + GRAYSCALE );

  _font_menu = glutCreateMenu ( _menu );
  glutAddMenuEntry ( "Symbol 8x13",         3000 + FIXED_8x13 );
  glutAddMenuEntry ( "Symbol 9x15",         3000 + FIXED_9x15 );
  glutAddMenuEntry ( "Helvetica 10",        3000 + HELVETICA_10 );
  glutAddMenuEntry ( "Helvetica 12",        3000 + HELVETICA_12 );
  glutAddMenuEntry ( "Helvetica 18",        3000 + HELVETICA_18 );
  glutAddMenuEntry ( "Times-Roman 10",      3000 + TIMES_ROMAN_10 );
  glutAddMenuEntry ( "Times-Roman 24",      3000 + TIMES_ROMAN_24 );

  glutCreateMenu ( _menu );
  glutAddSubMenu ( "Scale...", _scale_menu );
  glutAddSubMenu ( "Decor...", _decor_menu );
  glutAddSubMenu ( "Style...", _style_menu );
  glutAddSubMenu ( "Color...", _color_menu );
  glutAddSubMenu ( "Font...", _font_menu );
  glutAddMenuEntry ( "[S]ave EPS",              's' );
  glutAddMenuEntry ( "[Q]uit",                  'q' );
  glutAttachMenu ( GLUT_RIGHT_BUTTON );

  glutMenuStatusFunc ( _menu_status );
}

void
glp::window::menu_status ( int status, int x, int y )
{
  _menu_in_use = ( status == GLUT_MENU_IN_USE );
}

void
glp::window::_init()
{
//   if ( _registry.empty() ) {
//     static int argc = 1;
//     static char * argv[] = {PACKAGE};
//     glutInit ( &argc, argv );
//   }
  static int argc = 0;
  static char * argv[] = {PACKAGE};
  if ( !argc ) {
    argc = 1;
    glutInit ( &argc, argv );
  }
}

map<int,glp::window*> glp::window::_registry;

void
glp::window::_display()
{
  _registry[glutGetWindow()]->display();
}

void
glp::window::_reshape ( int width, int height )
{
  _registry[glutGetWindow()]->reshape(width,height);
}

void
glp::window::_keyboard ( unsigned char key, int x, int y )
{
  _registry[glutGetWindow()]->keyboard(key,x,y);
}

void
glp::window::_special ( int key, int x, int y )
{
  _registry[glutGetWindow()]->special(key,x,y);
}

void
glp::window::_mouse ( int button, int state, int x, int y )
{
  _registry[glutGetWindow()]->mouse(button,state,x,y);
}

void
glp::window::_motion ( int x, int y )
{
  _registry[glutGetWindow()]->motion(x,y);
}

void
glp::window::_menu ( int option )
{
  _registry[glutGetWindow()]->menu(option);
}

void
glp::window::_menu_status ( int status, int x, int y )
{
  _registry[glutGetWindow()]->menu_status(status,x,y);
}

