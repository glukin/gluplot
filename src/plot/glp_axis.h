/* -*- C++ -*- *************************************************************
 *
 *   glp_axis.h, Mon Oct  4 11:15:02 MSD 2004
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

#ifndef GLP_AXIS_H
#define GLP_AXIS_H

#include <vector>
using namespace std;

namespace glp {

  class axis {
  public:
    typedef double ( *lens ) ( double );
    typedef vector<double> grid;
    typedef grid::const_iterator const_iterator;
  public: /** c-tor(s) & d-tor */
    axis();
    virtual ~axis();
  public: /** interface */
    /** distorted mapping */
    void distortion ( lens function );
    bool distortion() const { return _distortion != 0; }
    const double operator() ( const double & arg ) const;
    /** range */
    void range ( const double & low, const double & high );
    const double & low() const { return _low; }
    const double & high() const { return _high; }
    /** artificial zone between axes ends and graphics in percents */
    void offsets ( const double & loff, const double & hoff );
    const double & loff() const { return _loff; }
    const double & hoff() const { return _hoff; }
    /** visible range */
    const double & lowend() const { return _lowend; }
    const double & highend() const { return _highend; }
    /** ticks (none by default!) */
    void ticks ( const unsigned int number );
    const_iterator begin() const { return _ticks.begin(); }
    const_iterator end() const { return _ticks.end(); }
    const_iterator sbegin() const { return _sticks.begin(); }
    const_iterator send() const { return _sticks.end(); }
    /** set defaults */
    void clear();
  protected: /** storages */
    lens _distortion;
    double _low, _high;
    double _loff, _hoff;
    double _lowend, _highend;
    grid _ticks, _sticks;  /** major and minor ticks */
    unsigned int _guide;   /** upper limit on number of tics allowed */
  protected: /** setup */
    void mkends();
    void mkticks();
  };

} /** namespace glp */

#endif // GLP_AXIS_H

