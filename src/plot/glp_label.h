/* -*- C++ -*- *************************************************************
 *
 *   glp_label.h, Wed Oct 20 06:05:23 MSD 2004
 *   Copyright (C) 2004  <solovjev@semnon.localdomain>
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

#ifndef GLP_LABEL_H
#define GLP_LABEL_H

#include <string>
using namespace std;

namespace glp {

  enum TEXT_ALIGN {
    LEFT,
    RIGHT,
    BOTTOM,
    TOP,
    CENTER
  };

  enum TEXT_FONT {
    FIXED_8x13,
    FIXED_9x15,
    HELVETICA_10,
    HELVETICA_12,
    HELVETICA_18,
    TIMES_ROMAN_10,
    TIMES_ROMAN_24
  };

  static const enum TEXT_FONT DEFAULT_TEXT_FONT = HELVETICA_12;

  class label {
  public: /** c-tor(s) & d-tor */
    label();
    virtual ~label();
  public: /** interface */
    void text ( const char * format, ... ); /** printf's syntax */
    const string & text() const { return _text; }
    void x ( const double & val ) { _x = val; }
    const double & x() const { return _x; }
    void y ( const double & val ) { _y = val; }
    const double & y() const { return _y; }
    void z ( const double & val ) { _z = val; }
    const double & z() const { return _z; }
    void align ( enum TEXT_ALIGN algn );
    enum TEXT_ALIGN align() const { return _align; }
    void font ( enum TEXT_FONT fnt );
    enum TEXT_FONT font() const { return _font; }
    /** drawing */
    virtual void draw() const;
  protected: /** storages */
    string _text;
    double _x, _y, _z;
    enum TEXT_ALIGN _align;
    double _xshift, _yshift;
    enum TEXT_FONT _font;
    void * _glutfont;
    string _fontname;
    int _fontsize;
  };

  /** just useful function(s) */
  const int width ( enum TEXT_FONT fnt );   /** character width */
  const int height ( enum TEXT_FONT fnt );  /** character height */

} /** namespace glp */

#endif // GLP_LABEL_H

