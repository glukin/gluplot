/* -*- C++ -*- *************************************************************
 *
 *   glp_label.cpp, Wed Oct 20 06:05:23 MSD 2004
 *   Copyright (C) 2004  <solovjev@semnon.localdomain>
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

#include "glp_label.h"

#include "gl2ps.h"

#include <GL/glut.h>

#include <cstdio>
#include <cstdarg>
using namespace std;

glp::label::label()
  :  _text(),
     _x ( 0 ), _y ( 0 ), _z ( 0 ),
     _align ( CENTER )
{
  font ( HELVETICA_12 );
}

glp::label::~label() {}

void
glp::label::text ( const char * format, ... )
{
  static va_list ap;
  va_start ( ap, format );
  _text.resize ( vsprintf ( &_text[0], format, ap ) );
  vsprintf ( &_text[0], format, ap );
  va_end ( ap );

  align ( align() );
}

void
glp::label::font ( enum TEXT_FONT fnt )
{
  switch ( _font = fnt ) {
  case FIXED_8x13:
    _glutfont = GLUT_BITMAP_8_BY_13;
    _fontname = "Symbol";
    break;
  case FIXED_9x15:
    _glutfont = GLUT_BITMAP_9_BY_15;
    _fontname = "Symbol";
    break;
  case HELVETICA_10:
    _glutfont = GLUT_BITMAP_HELVETICA_10;
    _fontname = "Helvetica";
    break;
  case HELVETICA_12:
    _glutfont = GLUT_BITMAP_HELVETICA_12;
    _fontname = "Helvetica";
    break;
  case HELVETICA_18:
    _glutfont = GLUT_BITMAP_HELVETICA_18;
    _fontname = "Helvetica";
    break;
  case TIMES_ROMAN_10:
    _glutfont = GLUT_BITMAP_TIMES_ROMAN_10;
    _fontname = "Times-Roman";
    break;
  case TIMES_ROMAN_24:
    _glutfont = GLUT_BITMAP_TIMES_ROMAN_24;
    _fontname = "Times-Roman";
    break;
  }
  _fontsize = height ( font() );

  align ( align() );
}

void
glp::label::align ( enum TEXT_ALIGN algn )
{
  _align = algn;

  _xshift = 0;
  if ( _align != LEFT ) {
    for ( unsigned int i = 0; i < _text.size(); ++i ) {
      _xshift -= glutBitmapWidth ( _glutfont, _text[i] );
    }
    if ( _align != RIGHT ) _xshift /= 2;
  }
  if ( _align == LEFT ) _xshift += 1;
  if ( _align == RIGHT ) _xshift -= 1;

  _yshift = 0;
  if ( _align != BOTTOM ) {
    _yshift = -_fontsize;
    if ( _align != TOP ) _yshift /= 2;
  }
}

void
glp::label::draw() const
{
  glRasterPos3d ( _x, _y, _z );

  static int valid;
  glGetIntegerv ( GL_CURRENT_RASTER_POSITION_VALID, &valid );
  if ( !valid ) return;

  static int viewport[4];
  static double modelview_matrix[16];
  static double projection_matrix[16];
  static double raster_position[4];

  glGetIntegerv ( GL_VIEWPORT, viewport );
  glGetDoublev ( GL_MODELVIEW_MATRIX, modelview_matrix );
  glGetDoublev ( GL_PROJECTION_MATRIX, projection_matrix );
  glGetDoublev ( GL_CURRENT_RASTER_POSITION, raster_position );

  gluUnProject ( raster_position[0] += _xshift, 
		 raster_position[1] += _yshift, 
		 raster_position[2],
		 modelview_matrix, projection_matrix, viewport, 
		 &raster_position[0], 
		 &raster_position[1], 
		 &raster_position[2] );

  glRasterPos4dv ( raster_position );

  // gl2psText should be called before the glut function
  gl2psText ( _text.c_str(), _fontname.c_str(), _fontsize );

  string::const_iterator p = _text.begin();
  while ( p != _text.end() ) {
    glutBitmapCharacter ( _glutfont, *p++ );
  }
}

const int
glp::width ( enum TEXT_FONT fnt )
{
  switch ( fnt ) {
  case FIXED_8x13:      return 8;
  case FIXED_9x15:      return 9;
  case HELVETICA_10:    return 6;
  case HELVETICA_12:    return 7;
  case HELVETICA_18:    return 10;
  case TIMES_ROMAN_10:  return 5;
  case TIMES_ROMAN_24:  return 12;
  default:              return 0;
  }
}

const int
glp::height ( enum TEXT_FONT fnt )
{
  switch ( fnt ) {
  case FIXED_8x13:      return 13;
  case FIXED_9x15:      return 15;
  case HELVETICA_10:    return 10;
  case HELVETICA_12:    return 12;
  case HELVETICA_18:    return 18;
  case TIMES_ROMAN_10:  return 10;
  case TIMES_ROMAN_24:  return 24;
  default:              return 0;
  }
}
