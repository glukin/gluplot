/* -*- C++ -*- *************************************************************
 *
 *   parzen_1D.cpp, Mon Oct 29 18:03:14 MSK 2007
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
  typedef const double ( *func ) ( const double & x );
public: /** c-tor(s) & d-tor */
  dplot ( const double & xmin, const double & xmax, const double & xstep,
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
	       func kernel, const double & hn )
  : gluplot ( "Parzen-Rozenblatt Estimation of Unknown Probability Pensity" ),
    _generator ( 0 ), _kernel ( kernel ), _hn ( hn )
{
  glutIdleFunc ( _idle );

  curve sample;
  curve spectrum;
  for ( double x = xmin; x < xmax; x += xstep ) {
    spectrum.push_back ( point ( x, 0.0 ) );
  }
  (*this) << POINTS << sample;
  (*this) << LINES << spectrum;

  label counter;
  counter.align ( LEFT );
  (*this) << TIMES_ROMAN_10 << counter << DEFAULT_TEXT_FONT;
}

dplot::~dplot()
{
  curve * f = &*_lines.begin()+1;
  xrange ( f->xlow(), f->xhigh() );
  yrange ( 0.0, 1.0 );
  label * l = &*_marks.begin();
  l->x ( xlow() );
  l->y ( yhigh() );
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
    curve * f = &*_lines.begin()+1;
    double x = 0.0, y = 0.0;
    if ( _generator ) x = _generator ( y );
    s->push_back ( point ( x, y ) );
    double ymin = 0.0, ymax = 0.0;
    for ( int i = 0; i < f->size(); ++i ) {
      double val = 0.0;
      for ( int j = 0; j < s->size(); ++j ) {
	double arg = ( (*f)[i].x() - (*s)[j].x() ) / _hn;
	val += _kernel ( arg );
      }
      ( val /= s->size() ) /= _hn;
      (*f)[i].y ( val );
      ymin = min ( ymin, val );
      ymax = max ( ymax, val );
    }
    yrange ( ymin, ymax );
    l->x ( xlow() );
    l->y ( yhigh() );
    tick = 0;
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
nrand ( const double & )
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
kernel ( const double & x )
{
  //  Kernel for Parzen-Rozenblatt Estimation. 
  if ( abs ( x ) > 1.0 ) return 0.0;
  double q54= ( 1.0 - x * x );
  return 0.9375 * q54 * q54;
}

static
const double
gauss ( const double & x )
{
  return exp ( -x*x / 2.0 ) / sqrt ( 2.0 * M_PI );
}

int main ( int argc, char** argv )
{
  if ( argc > 2 ) {
    cerr << "Usage: " << argv[0] << " [hn]" << endl;
    return 1;
  }

  double hn = 1.0;
  if ( argc == 2 ) hn = atof ( argv[1] );

  dplot pout ( -3.0, 3.0, 0.01, kernel, hn );
  pout.generator ( nrand );

  if ( argc == 2 ) {
    curve theory;
    for ( double x = -3.0; x < 3.0; x += 0.02 ) {
      theory.push_back ( point ( x, gauss ( x ) ) );
    }
    pout << LINES << theory;
  }

  return 0;
}

