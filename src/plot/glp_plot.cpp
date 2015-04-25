/* -*- C++ -*- *************************************************************
 *
 *   glp_plot.cpp, Fri Oct  1 11:35:17 MSD 2004
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

#include "glp_plot.h"

#include <algorithm>
#include <cmath>
using namespace std;

const unsigned int
glp::plot_traits::_easel_size = 9;

const glp::rgb
glp::plot_traits::_easel[] = {
  { 0xff, 0x00, 0x00 }, /* color 01: red           */
  { 0x00, 0xc0, 0x00 }, /* color 02: dark green    */
  { 0x00, 0x80, 0xff }, /* color 03: dark blue     */
  { 0xc0, 0x00, 0xff }, /* color 04: dark magenta  */
  { 0x00, 0xee, 0xee }, /* color 05: cyan          */
  { 0xc0, 0x40, 0x00 }, /* color 06: orange        */
  { 0xee, 0xee, 0x00 }, /* color 07: yellow        */
  { 0x20, 0x20, 0xc0 }, /* color 08: royal blue    */
  { 0xff, 0xc0, 0x20 }, /* color 09: goldenrod1    */
};

glp::plot_traits::plot_traits()
{
  style ( LINES );

  tone ( _easel [ _color_index = 0 ] ); // first line color

  background ( WHITE );
  foreground ( BLACK );

  line_width ( 1.5 );
  point_size ( 3.0 );

  frame_line_width ( 1.5 );
  grid_line_width ( 0.7 );

  font ( DEFAULT_TEXT_FONT );

  flush();
}

glp::plot_traits::~plot_traits() {}

void
glp::plot_traits::chtone()
{
  if ( tone() == _easel [ _color_index ] ) {
    if ( ++_color_index >= _easel_size ) {
      _color_index = 0;
    }
  }
  tone ( _easel [ _color_index ] );
}

void
glp::plot_traits::flush()
{
  ticks_visible ( true );
  grid_visible ( false );
  key_visible ( false );
  style_visible ( false );
}

glp::plot::plot()
  : plot_traits()
{
  clear();
  size ( 640, 480 );
}

glp::plot::~plot() {}

void
glp::plot::clear()
{
  _lines.clear();
  _planes.clear();
  _marks.clear();

  _x_axis.clear();
  _y_axis.clear();
  _z_axis.clear();

  /** logarithmic stuff */
  _xlog = _ylog = _zlog = false;
  _xmin = xlow(), _ymin = ylow(), _zmin = zlow();
  _xmin_positive = _ymin_positive = _zmin_positive = 1e-5;

  /** view angles */
  _xrotation = _yrotation = _zrotation = 0.0;

  /** reset color */
  tone ( _easel [ _color_index = 0 ] );

  size ( _width, _height );

  _valid = false;

  clipping ( false );
  clip();
}

void
glp::plot::push_back ( const curve & c )
{
  _lines.push_back ( c );
  _lines.rbegin()->style ( style() );
  _lines.rbegin()->tone ( tone() );
  chtone();
}

void
glp::plot::push_back ( const surface & s )
{
  _planes.push_back ( s );
  _planes.rbegin()->style ( style() );
  _planes.rbegin()->tone ( tone() );
  chtone();
  _planes.rbegin()->mkmesh();
  _planes.rbegin()->mknormals();
  sort ( _planes.rbegin(), _planes.rend() );
}

void
glp::plot::push_back ( const label & l )
{
  _marks.push_back ( l );
  _marks.rbegin()->font ( font() );
}

void
glp::plot::xrange ( const double & low, const double & high )
{
  _x_axis.range ( low, high );
}

void
glp::plot::yrange ( const double & low, const double & high )
{
  _y_axis.range ( low, high );
}

void
glp::plot::zrange ( const double & low, const double & high )
{
  _z_axis.range ( low, high );
}

void
glp::plot::fit_to ( const curve & c )
{
  // FIXME: wrong fit_to() afrer empty curve!
  if ( !c.empty() ) {
    if ( empty() ) {
      xrange ( c.xlow(), c.xhigh() );
      yrange ( c.ylow(), c.yhigh() );
      zrange ( c.zlow(), c.zhigh() );
      _xmin = xlow();
      _ymin = ylow();
      _zmin = zlow();
      _xmin_positive = find_x ( &*c.begin(), &*c.begin() +  c.size(), 0 );
      _ymin_positive = find_y ( &*c.begin(), &*c.begin() +  c.size(), 0 );
      _zmin_positive = find_z ( &*c.begin(), &*c.begin() +  c.size(), 0 );
    } else {
      xrange ( min ( xlow(), c.xlow() ), max ( xhigh(), c.xhigh() ) );
      yrange ( min ( ylow(), c.ylow() ), max ( yhigh(), c.yhigh() ) );
      zrange ( min ( zlow(), c.zlow() ), max ( zhigh(), c.zhigh() ) );
      _xmin = min ( _xmin, xlow() );
      _ymin = min ( _ymin, ylow() );
      _zmin = min ( _zmin, zlow() );
      _xmin_positive = min ( _xmin_positive,
			     find_x ( &*c.begin(), &*c.begin() +  c.size(), 0 ) );
      _ymin_positive = min ( _ymin_positive,
			     find_y ( &*c.begin(), &*c.begin() +  c.size(), 0 ) );
      _zmin_positive = min ( _zmin_positive,
			     find_z ( &*c.begin(), &*c.begin() +  c.size(), 0 ) );
    }
    xlog ( xlog() );
    ylog ( ylog() );
    zlog ( zlog() );
  }
}

void
glp::plot::fit_to ( const surface & s )
{
  if ( !s.empty() ) {
    if ( empty() ) {
      xrange ( s.xlow(), s.xhigh() );
      yrange ( s.ylow(), s.yhigh() );
      zrange ( s.zlow(), s.zhigh() );
      _xmin = xlow();
      _ymin = ylow();
      _zmin = zlow();
      _xmin_positive = find_x ( &*s.begin(), &*s.begin() +  ( s.end() - s.begin() ), 0 );
      _ymin_positive = find_y ( &*s.begin(), &*s.begin() +  ( s.end() - s.begin() ), 0 );
      _zmin_positive = find_z ( &*s.begin(), &*s.begin() +  ( s.end() - s.begin() ), 0 );
    } else {
      xrange ( min ( xlow(), s.xlow() ), max ( xhigh(), s.xhigh() ) );
      yrange ( min ( ylow(), s.ylow() ), max ( yhigh(), s.yhigh() ) );
      zrange ( min ( zlow(), s.zlow() ), max ( zhigh(), s.zhigh() ) );
      _xmin = min ( _xmin, xlow() );
      _ymin = min ( _ymin, ylow() );
      _zmin = min ( _zmin, zlow() );
      _xmin_positive = min ( _xmin_positive,
			     find_x ( &*s.begin(), &*s.begin() +  ( s.end() - s.begin() ), 0 ) );
      _ymin_positive = min ( _ymin_positive,
			     find_y ( &*s.begin(), &*s.begin() +  ( s.end() - s.begin() ), 0 ) );
      _zmin_positive = min ( _zmin_positive,
			     find_z ( &*s.begin(), &*s.begin() +  ( s.end() - s.begin() ), 0 ) );
    }
    xlog ( xlog() );
    ylog ( ylog() );
    zlog ( zlog() );
  }
}

void
glp::plot::fit_to ( const label & l )
{
  if ( empty() ) {
    xrange ( l.x(), l.x() );
    yrange ( l.y(), l.y() );
    zrange ( l.z(), l.z() );
    _xmin = xlow();
    _ymin = ylow();
    _zmin = zlow();
    _xmin_positive = l.x() > 0 ? l.x() : 1e-5;
    _ymin_positive = l.y() > 0 ? l.y() : 1e-5;
    _zmin_positive = l.z() > 0 ? l.z() : 1e-5;
  } else {
    xrange ( min ( xlow(), l.x() ), max ( xhigh(), l.x() ) );
    yrange ( min ( ylow(), l.y() ), max ( yhigh(), l.y() ) );
    zrange ( min ( zlow(), l.z() ), max ( zhigh(), l.z() ) );
    _xmin = min ( _xmin, xlow() );
    _ymin = min ( _ymin, ylow() );
    _zmin = min ( _zmin, zlow() );
    _xmin_positive = min ( _xmin_positive, l.x() > 0 ? l.x() : 1e-5 );
    _ymin_positive = min ( _ymin_positive, l.y() > 0 ? l.y() : 1e-5 );
    _zmin_positive = min ( _zmin_positive, l.z() > 0 ? l.z() : 1e-5 );
  }
  xlog ( xlog() );
  ylog ( ylog() );
  zlog ( zlog() );
}

bool
glp::plot::visible ( const point & p ) const
{
  return 
    p.x() >= xlow() && p.x() <= xhigh() &&
    p.y() >= ylow() && p.y() <= yhigh() &&
    p.z() >= zlow() && p.z() <= zhigh();
}

void
glp::plot::xdistortion ( axis::lens func )
{
  _x_axis.distortion ( func );
}

void
glp::plot::ydistortion ( axis::lens func )
{
  _y_axis.distortion ( func );
}

void
glp::plot::zdistortion ( axis::lens func )
{
  _z_axis.distortion ( func );
}

void
glp::plot::xlog ( bool yn )
{
  if ( yn && _xmin_positive ) _xlog = true;
  else _xlog = false;
  if ( xlog() ) {
    xrange ( _xmin_positive, xhigh() );
    xdistortion ( log10 );
  } else {
    xdistortion ( 0 );
    xrange ( _xmin, xhigh() );
  }
}

void
glp::plot::ylog ( bool yn )
{
  if ( yn && _ymin_positive ) _ylog = true;
  else _ylog = false;
  if ( ylog() ) {
    yrange ( _ymin_positive, yhigh() );
    ydistortion ( log10 );
  } else {
    ydistortion ( 0 );
    yrange ( _ymin, yhigh() );
  }
}

void
glp::plot::zlog ( bool yn )
{
  if ( yn && _zmin_positive ) _zlog = true;
  else _zlog = false;
  if ( zlog() ) {
    zrange ( _zmin_positive, zhigh() );
    zdistortion ( log10 );
  } else {
    zdistortion ( 0 );
    zrange ( _zmin, zhigh() );
  }
}

void
glp::plot::xoffsets ( const double & loff, const double & hoff )
{
  _x_axis.offsets ( loff, hoff );
}

void
glp::plot::yoffsets ( const double & loff, const double & hoff )
{
  _y_axis.offsets ( loff, hoff );
}

void
glp::plot::zoffsets ( const double & loff, const double & hoff )
{
  _z_axis.offsets ( loff, hoff );
}

void
glp::plot::size ( const int width, const int height )
{
  _width = width;
  _height = height;

  double phi = zrotation() * M_PI / 180.0;
  xticks ( ( _width * abs ( cos ( phi ) ) +  _height * abs ( sin ( phi ) ) ) / 50 );
  yticks ( ( _height * abs ( cos ( phi ) ) +  _width * abs ( sin ( phi ) ) ) / 50 );
  zticks ( _height / 50 );
}

glp::plot &
glp::operator<< ( plot & pout, enum PLOT_STYLE with )
{
  pout.style ( with );
  return pout;
}

glp::plot &
glp::operator<< ( plot & pout, const curve & c )
{
  pout.fit_to ( c );
  pout.push_back ( c );
  return pout;
}

glp::plot &
glp::operator<< ( plot & pout, const surface & s )
{
  pout.fit_to ( s );
  pout.push_back ( s );
  return pout;
}

glp::plot &
glp::operator<< ( plot & pout, enum TEXT_FONT with )
{
  pout.font ( with );
  return pout;
}

glp::plot &
glp::operator<< ( plot & pout, const label & l )
{
  pout.fit_to ( l );
  pout.push_back ( l );
  return pout;
}

/***************************************************************************
 *                                                                         *
 *   just useful function(s)                                               *
 *                                                                         *
 ***************************************************************************/

const double
glp::find_x ( const point * first, const point * last,
	      const double & threshold )
{
  double res = threshold;
  for ( ; first != last; ++first ) {
    if ( first->x() > threshold ) {
      if ( res == threshold ) res = first->x();
      else res = min ( res, first->x() );
    }
  }
  return res;
}

const double
glp::find_y ( const point * first, const point * last,
	      const double & threshold )
{
  double res = threshold;
  for ( ; first != last; ++first ) {
    if ( first->y() > threshold ) {
      if ( res == threshold ) res = first->y();
      else res = min ( res, first->y() );
    }
  }
  return res;
}

const double
glp::find_z ( const point * first, const point * last,
	      const double & threshold )
{
  double res = threshold;
  for ( ; first != last; ++first ) {
    if ( first->z() > threshold ) {
      if ( res == threshold ) res = first->z();
      else res = min ( res, first->z() );
    }
  }
  return res;
}
