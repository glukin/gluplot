/* -*- C++ -*- *************************************************************
 *
 *   glp_point.cpp, Fri Oct  1 11:35:50 MSD 2004
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

#include "glp_point.h"

#include <cmath>
using namespace std;

glp::point::point()
  : _x(), _y(), _z(),
    _xlow(_x), _xhigh(_x),
    _ylow(_y), _yhigh(_y),
    _zlow(_z), _zhigh(_z)
{}

glp::point::point ( const double & x, const double & y, const double & z )
  : _x(x), _y(y), _z(z),
    _xlow(_x), _xhigh(_x),
    _ylow(_y), _yhigh(_y),
    _zlow(_z), _zhigh(_z)
{}

glp::point::~point() {}

void
glp::point::xerrorbars ( const double & minus, const double & plus )
{
  _xlow = _x - minus;
  _xhigh = _x + plus;
}

void
glp::point::yerrorbars ( const double & minus, const double & plus )
{
  _ylow = _y - minus;
  _yhigh = _y + plus;
}

void
glp::point::zerrorbars ( const double & minus, const double & plus )
{
  _zlow = _z - minus;
  _zhigh = _z + plus;
}

bool
glp::operator== ( const point & lhs, const point & rhs )
{
  return 
    lhs.x() == rhs.x() &&
    lhs.y() == rhs.y() &&
    lhs.z() == rhs.z();
}

const glp::point
glp::operator+ ( const point & lhs, const point & rhs )
{
  return point ( lhs.x() + rhs.x(), lhs.y() + rhs.y(), lhs.z() + rhs.z() );
}

const glp::point
glp::operator- ( const point & lhs, const point & rhs )
{
  return point ( lhs.x() - rhs.x(), lhs.y() - rhs.y(), lhs.z() - rhs.z() );
}

const glp::point
glp::operator* ( const point & lhs, const double & rhs )
{
  return point ( lhs.x() * rhs, lhs.y() * rhs, lhs.z() * rhs );
}

const glp::point
glp::operator* ( const double & lhs, const point & rhs )
{
  return point ( lhs * rhs.x(), lhs * rhs.y(), lhs * rhs.z() );
}

const glp::point
glp::operator/ ( const point & lhs, const double & rhs )
{
  return point ( lhs.x() / rhs, lhs.y() / rhs, lhs.z() / rhs );
}

const glp::point
glp::operator- ( const point & lhs )
{
  return point ( -lhs.x(), -lhs.y(), -lhs.z() );
}

/***************************************************************************
 *                                                                         *
 *   just useful function(s)                                               *
 *                                                                         *
 ***************************************************************************/

const double
glp::distance ( const point & lhs, const point & rhs )
{
  double dx = lhs.x() - rhs.x();
  double dy = lhs.y() - rhs.y();
  double dz = lhs.z() - rhs.z();
  return sqrt ( ( dx *= dx ) + ( dy *= dy ) + ( dz *= dz ) );
}

const double
glp::scalar_product ( const point & lhs, const point & rhs )
{
  return lhs.x() * rhs.x() + lhs.y() * rhs.y() + lhs.z() * rhs.z();
}

const glp::point
glp::cross_product ( const point & lhs, const point & rhs )
{
  return point ( lhs.y() * rhs.z() - lhs.z() * rhs.y(),
		 lhs.z() * rhs.x() - lhs.x() * rhs.z(),
		 lhs.x() * rhs.y() - lhs.y() * rhs.x() );
}
