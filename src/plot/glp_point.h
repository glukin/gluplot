/* -*- C++ -*- *************************************************************
 *
 *   glp_point.h, Fri Oct  1 11:35:50 MSD 2004
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

#ifndef GLP_POINT_H
#define GLP_POINT_H

namespace glp {

  class point {
  public: /** c-tor(s) & d-tor */
    point();
    point ( const double & x, const double & y, const double & z = 0 );
    virtual ~point();
  public: /** interface */
    void x ( const double & val ) { _x = val; }
    const double & x() const { return _x; }
    void y ( const double & val ) { _y = val; }
    const double & y() const { return _y; }
    void z ( const double & val ) { _z = val; }
    const double & z() const { return _z; }
    /** errorbars (ignored in 3d) */
    void xerrorbars ( const double & minus, const double & plus );
    const double & xlow() const { return _xlow; }
    const double & xhigh() const { return _xhigh; }
    void yerrorbars ( const double & minus, const double & plus );
    const double & ylow() const { return _ylow; }
    const double & yhigh() const { return _yhigh; }
    void zerrorbars ( const double & minus, const double & plus );
    const double & zlow() const { return _zlow; }
    const double & zhigh() const { return _zhigh; }
  protected: /** storages */
    double _x, _y, _z;
    double _xlow, _xhigh;
    double _ylow, _yhigh;
    double _zlow, _zhigh;
  };

  /** outer interface */
  bool operator== ( const point & lhs, const point & rhs );
  const point operator+ ( const point & lhs, const point & rhs );
  const point operator- ( const point & lhs, const point & rhs );
  const point operator* ( const point & lhs, const double & rhs );
  const point operator* ( const double & lhs, const point & rhs );
  const point operator/ ( const point & lhs, const double & rhs );
  const point operator- ( const point & lhs );

  /** just useful function(s) */
  const double distance ( const point & lhs, const point & rhs );
  const double scalar_product ( const point & lhs, const point & rhs );
  const point cross_product ( const point & lhs, const point & rhs );

} /** namespace glp */

#endif // GLP_POINT_H

