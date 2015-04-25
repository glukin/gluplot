/* -*- C++ -*- *************************************************************
 *
 *   glp_color.h, Sun Oct 10 21:39:03 MSD 2004
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

#ifndef GLP_COLOR_H
#define GLP_COLOR_H

namespace glp {

  /***************************************************************************
   *
   * Standard colors. RGB values are uchars in range [0,255].
   *
   ***************************************************************************/

  extern const struct rgb { 
    unsigned char r, g, b;
  }
  BLACK, \
  RED, GREEN, BLUE, \
  CYAN, MAGENTA, YELLOW, \
  WHITE;

  /** outer interface */
  bool operator== ( const rgb & lhs, const rgb & rhs );

  /***************************************************************************
   *
   * Color in RGB scheme. RGB values are all in range [0,1].
   *
   ***************************************************************************/

  class color {
  public: /** c-tor(s) & d-tor */
    color();
    color ( const double & r, const double & g, const double & b );
    virtual ~color();
  public: /** interface */
    const double & r() const { return _r; }
    const double & g() const { return _g; }
    const double & b() const { return _b; }
  public: /** color <--> rgb cast */
    color ( const rgb & rhs );
    operator rgb() const;
  protected: /** storages */
    double _r, _g, _b;
  };

  /** outer interface */
  const color average ( const color & c1, const color & c2, 
			const double & weight = 0.5 );

  /***************************************************************************
   *
   * Some nice schemes in RGB color space.
   *
   ***************************************************************************/

  enum COLOR_SCHEME {
    RAINBOW,        /** violet-indigo-blue-green-yellow-orange-red */
    HOT,            /** black-red-yellow-white */
    OCEAN,          /** black-blue-cyan-white */
    TERRAIN,        /** green-yellow-brown-gray */
    TRADITIONAL,    /** black-blue-red-yellow (gnuplot) */
    GRAYSCALE       /** grayscale */
  };

  /***************************************************************************
   *
   * Color palette.
   *
   ***************************************************************************/

  class palette {
  public:
    typedef double ( *formula ) ( double );
  public: /** c-tor(s) & d-tor */
    palette();
    virtual ~palette();
  public: /** interface */
    /** main palette purpose: gray --> color mapping */
    const color operator() ( const double & gray ) const;
    /** mapping formulae for RGB */
    void rgbscheme ( enum COLOR_SCHEME scheme );
    void rgbformulae ( formula r, formula g, formula b );
    /** gamma for grayscale only */
    void gamma ( const double & val ) { _gamma = val; }
    const double & gamma() const { return _gamma; }
    /** grayscale palette? */
    void grayscale ( bool yn ) { _grayscale = yn; }
    bool grayscale() const { return _grayscale; }
    /** negative picture? */
    void negative ( bool yn ) { _negative = yn; }
    bool negative() const { return _negative; }
  protected: /** storages */
    formula _rmap, _gmap, _bmap;
    double _gamma;
    bool _grayscale;
    bool _negative;
  };

} /** namespace glp */

#endif // GLP_COLOR_H

