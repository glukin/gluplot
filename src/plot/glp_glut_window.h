/* -*- C++ -*- *************************************************************
 *
 *   glp_glut_window.h, Fri Oct  1 11:35:30 MSD 2004
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

#ifndef GLP_GLUT_WINDOW_H
#define GLP_GLUT_WINDOW_H

#include "glp_window.h"

#include <map>
using namespace std;

namespace glp {

  class window 
    : public window_traits {
  public: /** c-tor(s) & d-tor */
    window ( const char * title = PACKAGE );
    virtual ~window();
  protected: /** callbacks */
    virtual void display();
    virtual void reshape ( int width, int height );
    virtual void keyboard ( unsigned char key, int x, int y );
    virtual void special ( int key, int x, int y );
    virtual void mouse ( int button, int state, int x, int y );
    virtual void motion ( int x, int y );
    virtual void menu ( int option );
    void menu_status ( int status, int x, int y );
  protected: /** callbacks for glut */
    void _create();
    static void _init();
    static map<int,window*> _registry;
    static void _display();
    static void _reshape ( int width, int height );
    static void _keyboard ( unsigned char key, int x, int y );
    static void _special ( int key, int x, int y );
    static void _mouse ( int button, int state, int x, int y );
    static void _motion ( int x, int y );
    static void _menu ( int option );
    static void _menu_status ( int status, int x, int y );
  protected: /** storages */
    bool _menu_in_use;
    int _scale_menu;
    int _decor_menu;
    int _style_menu;
    int _color_menu;
    int _font_menu;
    bool _fullscreen;
    int _x4fs, _y4fs, _w4fs, _h4fs;
  };

} /** namespace glp */

#endif // GLP_GLUT_WINDOW_H

