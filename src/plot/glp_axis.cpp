/* -*- C++ -*- *************************************************************
 *
 *   glp_axis.cpp, Mon Oct  4 11:15:02 MSD 2004
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

#include "glp_axis.h"

#include <algorithm>
#include <cmath>
using namespace std;

glp::axis::axis()
  : _distortion ( 0 ),
    _low ( 0 ), _high ( 0 ),
    _loff ( 0 ), _hoff ( 0 ),
    _lowend ( 0 ), _highend ( 0 ),
    _ticks(), _sticks(),
    _guide ( 0 )
{
  clear();
}

glp::axis::~axis() {}

void
glp::axis::clear()
{
  ticks ( 0 );             // should be first to prevent sectoring
  distortion ( 0 );        // linear
  range ( -10, 10 );       // like in gnuplot
  offsets ( 0.05, 0.05 );  // 5% offsets look nice
}

void
glp::axis::distortion ( lens function )
{
  _distortion = function;
  mkends(), mkticks();
}

const double
glp::axis::operator() ( const double & arg ) const
{
  return _distortion ? _distortion ( arg ) : arg;
}

void
glp::axis::range ( const double & low, const double & high )
{
  if ( ( _low = low ) > ( _high = high ) ) swap ( _low, _high );
  if ( _low == _high ) { // empty range adjusting up to 1%
    _low -= 0.01 * abs ( _low );
    _high += 0.01 * abs ( _high );
    if ( _low == _high ) // still empty due to zero
      return range ( -10, 10 );
  }
  mkends(), mkticks();
}

void
glp::axis::offsets ( const double & loff, const double & hoff )
{
  if ( loff > -1 && hoff > -1 && loff + hoff > -1 ) {
    _loff = loff;
    _hoff = hoff;
    mkends(), mkticks();
  }
}

void
glp::axis::ticks ( const unsigned int number )
{
  _guide = number;
  mkticks();
}

void
glp::axis::mkends()
{
  const axis & f = *this;
  _lowend = f ( low() );
  _highend = f ( high() );
  if ( _lowend > _highend ) swap ( _lowend, _highend );
  const double length = _highend - _lowend;
  _lowend -= length * loff();
  _highend += length * hoff();
}

namespace glp { /** functions used for ticks setup */

  const double roundoff ( const double & arg, const unsigned int signs = 0 );
  const double root ( double ( *f ) ( double ), const double & rhs, 
		      const double & left, const double & right, 
		      const double & epsilon = 0.01 );
  const double quantum ( const double & range, const unsigned int tics );

} /** namespace glp */

void
glp::axis::mkticks()
{
  _ticks.clear();
  _sticks.clear();

  if ( !_guide ) return;

  // visible ticks are desired to be equidistant
  const double vlength = highend() - lowend();
  const double vtic = quantum ( vlength, _guide );
  double vfirst = vtic * floor ( lowend() / vtic );

  while ( vfirst < lowend() ) vfirst += vtic;
  while ( vfirst < highend() ) {
    if ( distortion() ) {
      double first = roundoff ( root ( _distortion, vfirst, low(), high() ) );
      _ticks.push_back ( first );
      // is tick still visible after roundoff?
      first = _distortion ( first );
      if ( first < lowend() || first > highend() ) _ticks.pop_back();
    } else {
      _ticks.push_back ( vfirst );
    }
    vfirst += vtic;
  }
  sort ( _ticks.begin(), _ticks.end() );
  _ticks.erase ( unique ( _ticks.begin(), _ticks.end() ), _ticks.end() );

  // minor ticks are always equidistant
  for ( unsigned int i = 1; i < _ticks.size(); ++i ) {
    const double length = _ticks[i] - _ticks[i-1];
    const double tic = quantum ( length, _guide );
    double first = _ticks[i-1];
    while ( first < _ticks[i] ) {
      _sticks.push_back ( first );
      first += tic;
    }
  }
  sort ( _sticks.begin(), _sticks.end() );
}

/***************************************************************************
 *
 * functions used for ticks setup
 *
 ***************************************************************************/

const double
glp::roundoff ( const double & arg, const unsigned int signs )
{
  double order = ( floor ( log10 ( abs ( arg ) ) ) );
  order -= signs;
  double power = pow ( 10, order );
  return power * floor ( arg / power + 0.5 );
}

const double
glp::root ( double ( *f ) ( double ), const double & rhs, 
	    const double & left, const double & right, 
	    const double & epsilon )
{
  bool up = f ( left ) < f ( right );
  register double below = left;
  register double above = right;
  register double middle, lhs, error;
  do {
    middle = ( above + below ) / 2;
    error = ( above - below ) / middle;
    lhs = f ( middle );
    if ( lhs == rhs ) break;
    else if ( up && lhs < rhs || !up && lhs > rhs ) below = middle;
    else if ( up && lhs > rhs || !up && lhs < rhs ) above = middle;
  } while ( abs ( error ) > epsilon );
  return middle;
}

const double
glp::quantum ( const double & range, const unsigned int tics )
{
  // the tics parameter is the approximate upper limit 
  // on number of tics allowed

  // order of magnitude of argument:
  double power = pow ( 10.0, floor ( log10 ( abs ( range ) ) ) );
  // approx number of decades ( 1 <= xnorm <= 10 ):
  double xnorm = range / power;
  // approx number of tic posns per decade:
  double posns = tics / xnorm;

  double tic;
  if      ( posns > 40  ) tic = 0.05; // eg 0, 0.05, 0.10, ...
  else if ( posns > 20  ) tic = 0.1;  // eg 0, 0.1, 0.2, ...
  else if ( posns > 10  ) tic = 0.2;  // eg 0, 0.2, 0.4, ...
  else if ( posns > 4   ) tic = 0.5;  // eg 0, 0.5, 1, ...
  else if ( posns > 2   ) tic = 1;    // eg 0, 1, 2, ...
  else if ( posns > 0.5 ) tic = 2;    // eg 0, 2, 4, ...
  else
    // getting desperate... 
    // the ceil is to make sure we go over rather than under 
    // (eg plot [-10:10] x*x gives a range of about 99.999, 
    // tics=xnorm gives tics at 0, 99.99 and 109.98  - BAD!)
    // This way, inaccuracy the other way will round up 
    // (eg 0->100.0001 => tics at 0 and 101)
    // I think latter is better than former
    tic = ceil ( xnorm );

  return tic *= power;
}

