/* -*- C++ -*- *************************************************************
 *
 *   parzen_2D.cpp, Mon Oct 29 18:03:14 MSK 2007
 *   Copyright (C) 2007 Alexei Soloviev <solovjev@nica30>
 *
 *   Parzen-Rozenblatt Estimation of Unknown Probability Pensity.
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

#include <GL/glut.h>

#include <iostream>
#include <cstdlib>
#include <cmath>
using namespace std;

#include <time.h>  /* for random seed */

class dplot
  : public gluplot {
  typedef const double ( *func ) ( const double & x, const double & y );
public: /** c-tor(s) & d-tor */
  dplot ( const double & xmin, const double & xmax, const double & xstep,
	  const double & ymin, const double & ymax, const double & ystep,
	  func kernel, const double & hn );
  ~dplot();
public: /** interface */
  void generator ( func g ) { _generator = g; }
protected: /** callbacks */
  void idle();
protected: /** callbacks for glut */
  static void _idle();
protected: /** storages */
  func _generator;
  func _kernel;
  double _hn;
};

dplot::dplot ( const double & xmin, const double & xmax, const double & xstep,
	       const double & ymin, const double & ymax, const double & ystep,
	       func kernel, const double & hn )
  : gluplot ( "Parzen-Rozenblatt Estimation of Unknown Probability Pensity" ),
    _generator ( 0 ), _kernel ( kernel ), _hn ( hn )
{
  glutIdleFunc ( _idle );

  curve sample;
  surface spectrum;
  int xsize = 0, ysize = 0;
  for ( double x = xmin; x < xmax; x += xstep ) {
    for ( double y = ymin; y < ymax; y += ystep ) {
      spectrum.push_back ( point ( x, y ) );
      if ( !xsize ) ++ysize;
    }
    ++xsize;
  }
//   int xsize = ( xmax - xmin ) / xstep;
//   int ysize = ( ymax - ymin ) / ystep;
  for ( int i = 1; i < xsize; ++i ) {
    for ( int j = 1; j < ysize; ++j ) {
      bundle node;
      node.insert ( (i-1) * ysize + (j-1) );
      node.insert ( (i) * ysize + (j-1) );
      node.insert ( (i) * ysize + (j) );
      node.insert ( (i-1) * ysize + (j) );
      spectrum.push_back ( node );
    }
  }
  (*this) << POINTS << sample;
  (*this) << COLORMAP << spectrum;

  label counter;
  counter.align ( LEFT );
  (*this) << TIMES_ROMAN_10 << counter << DEFAULT_TEXT_FONT;
}

dplot::~dplot()
{
  surface * f = &*_planes.begin();
  xrange ( f->xlow(), f->xhigh() );
  yrange ( f->ylow(), f->yhigh() );
  zrange ( 0.0, 1.0 );
  label * l = &*_marks.begin();
  l->x ( xlow() );
  l->y ( ( yhigh() + yhighend() ) / 2 );
  l->z ( zhigh() );
}

void
dplot::idle()
{
  static int tick = 0;
  int frames = fps();
  label * l = &*_marks.begin();
  l->text ( "fps: %d", frames );
  frames = 0.2 * frames;
  if ( ++tick > frames ) {
    curve * s = &*_lines.begin();
    surface * f = &*_planes.begin();
    double x = 0.0, y = 0.0, z = 0.0;
    if ( _generator ) {
      x = _generator ( z, z );
      y = _generator ( z, z );
    }
    s->push_back ( point ( x, y ) );
    double zmin = 0.0, zmax = 0.0;
    for ( surface::iterator p = f->begin(); p != f->end(); ++p ) {
      double val = 0.0;
      for ( int j = 0; j < s->size(); ++j ) {
	double xarg = ( p->x() - (*s)[j].x() ) / _hn;
	double yarg = ( p->y() - (*s)[j].y() ) / _hn;
	val += _kernel ( xarg, yarg );
      }
      ( val /= s->size() ) /= _hn;
      p->z ( val );
      zmin = min ( zmin, val );
      zmax = max ( zmax, val );
    }
    zrange ( zmin, zmax );
    l->x ( xlow() );
    l->y ( ( yhigh() + yhighend() ) / 2 );
    l->z ( zhigh() );
    tick = 0;
    if ( style() == LIGHTING ) f->cknormals();
  }
  glutPostRedisplay();
}

void
dplot::_idle()
{
  ((dplot*)_registry[glutGetWindow()])->idle();
}

static
const double
nrand ( const double &, const double & )
{
  // generates random numbers distributed by normal law 
  // with mean value 0 and standard deviation 1 
  double r1 = (double) rand() / RAND_MAX;
  double r2 = (double) rand() / RAND_MAX;
  double x = sqrt ( -2.0 * log ( r1 ) );
  double y = 2.0 * M_PI * r2;
  return x * sin ( y );
}

static
const double
kernel ( const double & x, const double & y )
{
  //  Kernel for Parzen-Rozenblatt Estimation. 
  double r2 = x * x + y * y;
  if ( r2 > 1.0 ) return 0.0;
  double q54= ( 1.0 - r2 );
  return 0.4775 * q54 * q54;
}

static
const double
gauss ( const double & x, const double & y )
{
  return exp ( -(x*x+y*y) / 2.0 ) / ( 2.0 * M_PI );
}

int main ( int argc, char** argv )
{
  if ( argc > 2 ) {
    cerr << "Usage: " << argv[0] << " [hn]" << endl;
    return 1;
  }

  double hn = 1.0;
  if ( argc == 2 ) hn = atof ( argv[1] );

  dplot pout ( -3.0, 3.0, 0.05, -3.0, 3.0, 0.05, kernel, hn );
  pout.generator ( nrand );

  if ( argc == 2 ) {
    surface theory;
    int xsize = 0, ysize = 0;
    for ( double x = -3.0; x < 3.0; x += 0.2 ) {
      for ( double y = -3.0; y < 3.0; y += 0.2 ) {
	theory.push_back ( point ( x, y, gauss ( x, y ) ) );
	if ( !xsize ) ++ysize;
      }
      ++xsize;
    }
    for ( int i = 1; i < xsize; ++i ) {
      for ( int j = 1; j < ysize; ++j ) {
	bundle node;
	node.insert ( (i-1) * ysize + (j-1) );
	node.insert ( (i) * ysize + (j-1) );
	node.insert ( (i) * ysize + (j) );
	node.insert ( (i-1) * ysize + (j) );
	theory.push_back ( node );
      }
    }
    pout << COLOREDLINES << theory;
  }

  return 0;
}

