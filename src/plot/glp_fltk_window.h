/* -*- C++ -*- *************************************************************
 *
 *   glp_fltk_window.h, Wed Oct 24 16:20:45 MSD 2007
 *   Copyright (C) 2007 Soloviev Alexei <solovjev@cv.jinr.ru>
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

#ifndef GLP_FLTK_WINDOW_H
#define GLP_FLTK_WINDOW_H

#include "glp_window.h"

#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>

namespace glp {

  class fl_window 
    : public window_traits,
      public Fl_Gl_Window {
  public: /** c-tor(s) & d-tor */
    fl_window ( int x, int y, int w, int h, const char * l = 0 );
    virtual ~fl_window();
  protected: /** callbacks */
    //virtual void display();
    //virtual void reshape ( int width, int height );
    //virtual void keyboard ( unsigned char key, int x, int y );
    virtual void special ( int key, int x, int y );
    virtual void mouse ( int button, int state, int x, int y );
    //virtual void motion ( int x, int y );
//     virtual void menu ( int option );
  protected: /** callbacks for fltk */
    virtual void draw();
    virtual void resize ( int x, int y, int w, int h );
    virtual int handle ( int event );
  protected: /** storages */
//     bool _fullscreen;
//     int _x4fs, _y4fs, _w4fs, _h4fs;
  };

} /** namespace glp */

#endif // GLP_FLTK_WINDOW_H

