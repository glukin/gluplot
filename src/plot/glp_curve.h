/* -*- C++ -*- *************************************************************
 *
 *   glp_curve.h, Fri Oct  1 11:36:25 MSD 2004
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

#ifndef GLP_CURVE_H
#define GLP_CURVE_H

#include "glp_point.h"
#include "glp_color.h"
#include "glp_style.h"

#include <vector>
using namespace std;

namespace glp {

  class curve_traits {
  public: /** c-tor(s) & d-tor */
    curve_traits();
    virtual ~curve_traits();
  public: /** interface */
    /** plot style */
    void style ( enum PLOT_STYLE with ) { _style = with; }
    enum PLOT_STYLE style() const { return _style; }
    /** color */
    void tone ( const rgb & c ) { _color = c; }
    const rgb & tone() const { return _color; }
    /** transparency */
    void alpha ( const double & a ) { _alpha = a; }
    const double & alpha() const { return _alpha; }
  protected: /** storages */
    enum PLOT_STYLE _style;
    rgb _color;
    double _alpha;
  };

  class curve
    : public curve_traits,
      protected vector<point> {
  private:
    typedef class vector<point> base;
  public:
    typedef base::iterator iterator;
    typedef base::const_iterator const_iterator;
    typedef base::reverse_iterator reverse_iterator;
    typedef base::const_reverse_iterator const_reverse_iterator;
    typedef base::reference reference;
    typedef base::const_reference const_reference;
    typedef base::size_type size_type;
  public: /** c-tor(s) & d-tor */
    curve();
    curve ( size_type n, const double * x, const double * y );
    virtual ~curve();
  public: /** interface */
    /** iterators */
    iterator begin() { return base::begin(); }
    const_iterator begin() const { return base::begin(); }
    iterator end() { return base::end(); }
    const_iterator end() const { return base::end(); }
    reverse_iterator rbegin() { return base::rbegin(); }
    const_reverse_iterator rbegin() const { return base::rbegin(); }
    reverse_iterator rend() { return base::rend(); }
    const_reverse_iterator rend() const { return base::rend(); }
    /** size/empty? */
    size_type size() const { return base::size(); }
    bool empty() const { return base::empty(); }
    /** references */
    reference operator[] ( size_type i )
    {
      return base::operator[] ( i );
    }
    const_reference operator[] ( size_type i ) const
    {
      return base::operator[] ( i );
    }
    /** accumulation */
    void push_back ( const point & p );
    iterator insert ( iterator position, const point & p );
    void fit_to ( const point & p );
    /** cleanup */
    iterator erase ( iterator first, iterator last );
    void clear();
    /** frame */
    const double & xlow() const { return _xlow; }
    const double & xhigh() const { return _xhigh; }
    const double & ylow() const { return _ylow; }
    const double & yhigh() const { return _yhigh; }
    const double & zlow() const { return _zlow; }
    const double & zhigh() const { return _zhigh; }
  protected: /** storages */
    double _xlow, _xhigh;
    double _ylow, _yhigh;
    double _zlow, _zhigh;
  };

} /** namespace glp */

#endif // GLP_CURVE_H

