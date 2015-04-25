/* -*- C++ -*- *************************************************************
 *
 *   glp_window.h, Fri Oct  1 11:35:30 MSD 2004
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

#ifndef GLP_WINDOW_H
#define GLP_WINDOW_H

#ifdef HAVE_CONFIG_H
# include <config.h>
#else
# define PACKAGE "gluplot"
#endif

#include "glp_plot.h"

namespace glp {

  class window_traits 
    : public plot {
  public: /** c-tor(s) & d-tor */
    window_traits ( const char * title = PACKAGE );
    virtual ~window_traits();
  protected: /** callbacks */
    virtual void display();
    virtual void reshape ( int width, int height );
    virtual void keyboard ( unsigned char key, int x, int y );
    virtual void special ( int key, int x, int y );
    virtual void mouse ( int button, int state, int x, int y );
    virtual void motion ( int x, int y );
    virtual void menu ( int option );
  protected: /** storages */
    const char * _title;
    bool _do_mark, _do_rotate;
    double _x_min_mark;
    double _x_max_mark;
    double _y_min_mark;
    double _y_max_mark;
    bool _do_move;
    int _mouse_x;
    int _mouse_y;
  };

  /** just useful functions */
  void unproject ( int xw, int yw, double & x, double & y );
  int fps();

} /** namespace glp */

#endif // GLP_WINDOW_H

