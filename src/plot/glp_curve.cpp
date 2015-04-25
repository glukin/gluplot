/* -*- C++ -*- *************************************************************
 *
 *   glp_curve.cpp, Fri Oct  1 11:36:25 MSD 2004
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

#include "glp_curve.h"

#include <algorithm>
using namespace std;

glp::curve_traits::curve_traits()
  : _style ( LINES ),
    _color ( BLACK ),
    _alpha ( 1 )
{}

glp::curve_traits::~curve_traits() {}

glp::curve::curve()
  : curve_traits(), base(),
    _xlow(), _xhigh(),
    _ylow(), _yhigh(),
    _zlow(), _zhigh()
{}

glp::curve::curve ( size_type n, const double * x, const double * y  )
  : curve_traits(), base(),
    _xlow(), _xhigh(),
    _ylow(), _yhigh(),
    _zlow(), _zhigh()
{
  for ( unsigned int i = 0; i < n; ++i ) 
    push_back ( point ( *x++, *y++ ) );
}

glp::curve::~curve() {}

void
glp::curve::push_back ( const point & p )
{
  fit_to ( p );
  base::push_back ( p );
}

glp::curve::iterator
glp::curve::insert ( iterator position, const point & p )
{
  fit_to ( p );
  return base::insert ( position, p );
}

void
glp::curve::fit_to ( const point & p )
{
  if ( empty() ) {
    _xlow = _xhigh = p.x();
    _ylow = _yhigh = p.y();
    _zlow = _zhigh = p.z();
  } else {
    _xlow = min ( _xlow, p.x() );
    _xhigh = max ( _xhigh, p.x() );
    _ylow = min ( _ylow, p.y() );
    _yhigh = max ( _yhigh, p.y() );
    _zlow = min ( _zlow, p.z() );
    _zhigh = max ( _zhigh, p.z() );
  }
}

glp::curve::iterator
glp::curve::erase ( iterator first, iterator last )
{
  iterator res = base::erase ( first, last );
  const_iterator p = begin();
  if ( p != end() ) {
    _xlow = _xhigh = p->x();
    _ylow = _yhigh = p->y();
    _zlow = _zhigh = p->z();
    while ( ++p != end() ) {
      _xlow = min ( _xlow, p->x() );
      _xhigh = max ( _xhigh, p->x() );
      _ylow = min ( _ylow, p->y() );
      _yhigh = max ( _yhigh, p->y() );
      _zlow = min ( _zlow, p->z() );
      _zhigh = max ( _zhigh, p->z() );
    }
  } else {
    _xlow = _xhigh = 0;
    _ylow = _yhigh = 0;
    _zlow = _zhigh = 0;
  }
  return res;
}

void
glp::curve::clear()
{
  base::clear();
  _xlow = _xhigh = 0;
  _ylow = _yhigh = 0;
  _zlow = _zhigh = 0;
}
