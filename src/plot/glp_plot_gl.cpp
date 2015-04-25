/* -*- C++ -*- *************************************************************
 *
 *   glp_plot_gl.cpp, Tue Oct 12 00:05:38 MSD 2004
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

#include "glp_plot.h"

#include "gl2ps.h"

#include <GL/gl.h>
#include <GL/glu.h>

#include <algorithm>
#include <functional>
#include <cctype>
#include <cmath>
using namespace std;

void
glp::plot::init() const
{
  if ( _valid ) return;

  // settings
  glEnable ( GL_BLEND );
  glBlendFunc ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
  gl2psEnable ( GL2PS_BLEND );
  gl2psBlendFunc ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
  glEnable ( GL_LINE_SMOOTH );
  glHint ( GL_LINE_SMOOTH_HINT, GL_DONT_CARE );
  glEnable ( GL_POINT_SMOOTH );
  glHint ( GL_POINT_SMOOTH_HINT, GL_NICEST );
  glLineStipple ( 1, 0x1111 );
//   glEnable ( GL_POLYGON_SMOOTH );
//   glHint ( GL_POLYGON_SMOOTH_HINT, GL_DONT_CARE );
//   glPolygonMode ( GL_FRONT_AND_BACK, GL_FILL );
  glEnable ( GL_POLYGON_OFFSET_FILL );
  glPolygonOffset ( 1.0, 1.0 );
  glDepthFunc ( GL_LEQUAL );

  glShadeModel ( GL_SMOOTH );
  //glEnable ( GL_CULL_FACE );

  // lighting
  glMatrixMode ( GL_MODELVIEW );
  glPushMatrix();
  glLoadIdentity();
  static const GLfloat light[4] = { 0.5, 0.5, 0.5, 1.0 };
  static const GLfloat col0[4] = { 1.0, 1.0, 1.0, 1.0 };
  static const GLfloat pos0[4] = { 0.0, 0.0, -1.0, 0.0 };
  static const GLfloat dir0[4] = { 0.0, 0.0, 1.0, 0.0 };
  glEnable(GL_NORMALIZE);
  glLightModeli ( GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE );
  glLightModelfv ( GL_LIGHT_MODEL_AMBIENT, light ); // global ambient light
  glEnable ( GL_LIGHT0);
  glLightfv ( GL_LIGHT0, GL_DIFFUSE, col0 );
  glLightfv ( GL_LIGHT0, GL_SPECULAR, col0 );
  glLightfv ( GL_LIGHT0, GL_POSITION, pos0 );
  glLightfv ( GL_LIGHT0, GL_SPOT_DIRECTION, dir0 );
  glPopMatrix();

  _valid = true;
}

void
glp::plot::draw() const
{
  init();

  // axial labels size
  int wtxt = 6 * width ( font() ), htxt = height ( font() );

  // palette key size
  static const int keyleft = 5;
  static const int keyright = 20;

  // offsets
  int loff, roff, boff, toff;
  loff = roff = boff = toff = 5;
  if ( ticks_visible() ) {
    if ( _width - ( loff + roff ) - 128 > wtxt ) loff += wtxt;
    if ( _height - ( boff + toff ) - 96 > htxt ) boff += htxt;
  }
  if ( key_visible() ) {
    wtxt += keyright;
    if ( _width - ( loff + roff ) - 128 > wtxt ) roff += wtxt;
  }

  const int w = _width - ( loff + roff );
  const int h = _height - ( boff + toff );
  const double xprj = roff - loff;
  const double yprj = toff - boff;

  glViewport ( 0, 0, _width, _height );
  glScissor ( loff, boff, w, h );

  glMatrixMode ( GL_PROJECTION );
  glLoadIdentity();
//   gluOrtho2D ( ( xprj - _width ) / w, ( xprj + _width ) / w, 
// 	       ( yprj - _height ) / h, ( yprj + _height ) / h );
  glOrtho ( ( xprj - _width ) / w, ( xprj + _width ) / w, 
	    ( yprj - _height ) / h, ( yprj + _height ) / h,
	    2, -2 );

  glMatrixMode ( GL_MODELVIEW );
  glLoadIdentity();

  if ( !_planes.empty() ) { // orientation
    glRotatef ( xrotation(), 1.0, 0.0, 0.0 );
    glRotatef ( yrotation(), 0.0, 1.0, 0.0 );
    glRotatef ( zrotation(), 0.0, 0.0, 1.0 );
  }

//   gluOrtho2D ( xlowend(), xhighend(), 
// 	       ylowend(), yhighend() );
  glOrtho ( xlowend(), xhighend(), 
	    ylowend(), yhighend(), 
	    -zhighend(), -zlowend() ); // <---- OpenGL specific !!!

  glClearColor ( background().r(), background().g(), background().b(), 1 );
  glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  glEnable ( GL_SCISSOR_TEST );
  glEnable ( GL_DEPTH_TEST );

  // clipping
  if ( _clipping ) {
    for ( unsigned int i = 0; i < _clip_planes.size(); ++i ) {
      glClipPlane ( GL_CLIP_PLANE0 + i, &*_clip_planes[i].begin() );
      glEnable ( GL_CLIP_PLANE0 + i );
    }
  }

  // graphics: 2D
  for ( base2d::const_iterator p2 = _lines.begin(); p2 != _lines.end(); ++p2 ) {

    glColor4ub ( p2->tone().r, p2->tone().g, p2->tone().b, p2->alpha() * 255 );
    enum PLOT_STYLE with = style_visible() ? style() : p2->style();

    if ( with == DOTS ) draw_with_dots ( *p2 );
    if ( with & PLOT_STYLE_HAS_LINE ) draw_with_lines ( *p2 );
    if ( with & PLOT_STYLE_HAS_POINT ) draw_with_points ( *p2 );
    if ( with & PLOT_STYLE_HAS_XERRORBAR ) draw_with_xerrorbars ( *p2 );
    if ( with & PLOT_STYLE_HAS_YERRORBAR ) draw_with_yerrorbars ( *p2 );
    if ( with == IMPULSES ) draw_with_impulses ( *p2 );
  }

  // graphics: 3D
  for ( base3d::const_iterator p3 = _planes.begin(); p3 != _planes.end(); ++p3 ) {

    glColor3ub ( p3->tone().r, p3->tone().g, p3->tone().b );
    enum PLOT_STYLE with = style_visible() ? style() : p3->style();

    if ( with == COLOREDLINES ) draw_with_lines ( *p3 );
    else if ( with == LIGHTING ) draw_with_lighting ( *p3 );
    else {
      if ( with == DOTS ) draw_with_dots ( *p3 );
      if ( with & PLOT_STYLE_HAS_LINE ) draw_with_lines ( *p3 );
      if ( with & PLOT_STYLE_HAS_POINT ) draw_with_points ( *p3 );
      if ( with & PLOT_STYLE_HAS_ERRORBAR ) draw_with_errorbars ( *p3 );
      draw ( *p3 );
    }
  }

  // graphics: on-screen labels
  glColor3ub ( foreground().r, foreground().g, foreground().b );
  for ( base1::const_iterator p1 = _marks.begin(); p1 != _marks.end(); ++p1 ) {
    p1->draw();
  }

  glDisable ( GL_SCISSOR_TEST );

  // clipping
  if ( _clipping ) {
    for ( unsigned int i = 0; i < _clip_planes.size(); ++i ) {
      glDisable ( GL_CLIP_PLANE0 + i );
    }
  }

  //glColor3ub ( foreground().r, foreground().g, foreground().b );
  glLineWidth ( frame_line_width() );
  gl2psLineWidth ( frame_line_width() );

  static axis::const_iterator tic;
  static label ticmark;
  ticmark.font ( font() );

  if ( ticks_visible() ) { // ticks & grig

    double phi = zrotation() * M_PI / 180.0;
    double abs_cos_phi = abs ( cos ( phi ) );
    double abs_sin_phi = abs ( sin ( phi ) );
    double hx = 4.1 * ( xhighend() - xlowend() ) / ( w * abs_cos_phi +  h * abs_sin_phi );
    double hy = 4.1 * ( yhighend() - ylowend() ) / ( h * abs_cos_phi +  w * abs_sin_phi );
    double hz = 4.1 * ( zhighend() - zlowend() ) / h;

    // x-axis
    ticmark.y ( ylowend() );
    ticmark.z ( zlowend() );
    ticmark.align ( TOP );
    for ( tic = _x_axis.begin(); tic != _x_axis.end(); ++tic ) {
      if ( xrotation() || yrotation() ) {
	glBegin ( GL_LINES ); {
	  glVertex3f ( _x_axis ( *tic ), ylowend(), zlowend() );
	  glVertex3f ( _x_axis ( *tic ), ylowend() + hy, zlowend() );
	} glEnd();
      } else {
	glBegin ( GL_LINES ); {
	  glVertex3f ( _x_axis ( *tic ), ylowend(), zhighend() );
	  glVertex3f ( _x_axis ( *tic ), ylowend() + hy, zhighend() );
	} glEnd();
      }
      ticmark.x ( _x_axis ( *tic ) );
      ticmark.text ( "%.4g", *tic );
      ticmark.draw();
      if ( grid_visible() ) {
	glPushAttrib ( GL_LINE_BIT ); {
	  glEnable ( GL_LINE_STIPPLE );
	  gl2psEnable ( GL2PS_LINE_STIPPLE );
	  glLineWidth ( grid_line_width() );
	  gl2psLineWidth ( grid_line_width() );
	  if ( xrotation() || yrotation() ) {
	    glBegin ( GL_LINE_LOOP ); {
	      glVertex3f ( _x_axis ( *tic ), ylowend(), zlowend() );
	      glVertex3f ( _x_axis ( *tic ), yhighend(), zlowend() );
	      glVertex3f ( _x_axis ( *tic ), yhighend(), zhighend() );
	      glVertex3f ( _x_axis ( *tic ), ylowend(), zhighend() );
	    } glEnd();
	  } else {
	    glBegin ( GL_LINES ); {
	      glVertex3f ( _x_axis ( *tic ), ylowend(), zhighend() );
	      glVertex3f ( _x_axis ( *tic ), yhighend(), zhighend() );
	    } glEnd();
	  }
	  gl2psDisable ( GL2PS_LINE_STIPPLE );
	} glPopAttrib();
      }
    }

    // y-axis
    ticmark.x ( xlowend() );
    ticmark.z ( zlowend() );
    ticmark.align ( RIGHT );
    for ( tic = _y_axis.begin(); tic != _y_axis.end(); ++tic ) {
      if ( xrotation() || yrotation() ) {
	glBegin ( GL_LINES ); {
	  glVertex3f ( xlowend(), _y_axis ( *tic ), zlowend() );
	  glVertex3f ( xlowend() + hx, _y_axis ( *tic ), zlowend() );
	} glEnd();
      } else {
	glBegin ( GL_LINES ); {
	  glVertex3f ( xlowend(), _y_axis ( *tic ), zhighend() );
	  glVertex3f ( xlowend() + hx, _y_axis ( *tic ), zhighend() );
	} glEnd();
      }
      ticmark.y ( _y_axis ( *tic ) );
      ticmark.text ( "%.4g", *tic );
      ticmark.draw();
      if ( grid_visible() ) {
	glPushAttrib ( GL_LINE_BIT ); {
	  glEnable ( GL_LINE_STIPPLE );
	  gl2psEnable ( GL2PS_LINE_STIPPLE );
	  glLineWidth ( grid_line_width() );
	  gl2psLineWidth ( grid_line_width() );
	  if ( xrotation() || yrotation() ) {
	    glBegin ( GL_LINE_LOOP ); {
	      glVertex3f ( xlowend(), _y_axis ( *tic ), zlowend() );
	      glVertex3f ( xhighend(), _y_axis ( *tic ), zlowend() );
	      glVertex3f ( xhighend(), _y_axis ( *tic ), zhighend() );
	      glVertex3f ( xlowend(), _y_axis ( *tic ), zhighend() );
	    } glEnd();
	  } else {
	    glBegin ( GL_LINES ); {
	      glVertex3f ( xlowend(), _y_axis ( *tic ), zhighend() );
	      glVertex3f ( xhighend(), _y_axis ( *tic ), zhighend() );
	    } glEnd();
	  }
	  gl2psDisable ( GL2PS_LINE_STIPPLE );
	} glPopAttrib();
      }
    }

    if ( abs ( xrotation() ) > 10.0 || abs ( yrotation() ) > 10.0 ) { // z-axis
      ticmark.x ( xlowend() );
      ticmark.y ( ylowend() );
      ticmark.align ( RIGHT );
      for ( tic = _z_axis.begin(); tic != _z_axis.end(); ++tic ) {
	glBegin ( GL_LINES ); {
	  glVertex3f ( xlowend(), ylowend(), _z_axis ( *tic ) );
	  glVertex3f ( xlowend() + hx, ylowend() + hy, _z_axis ( *tic ) );
	} glEnd();
	ticmark.z ( _z_axis ( *tic ) );
	ticmark.text ( "%.4g", *tic );
	ticmark.draw();
	if ( grid_visible() ) {
	  glPushAttrib ( GL_LINE_BIT ); {
	    glEnable ( GL_LINE_STIPPLE );
	    gl2psEnable ( GL2PS_LINE_STIPPLE );
	    glLineWidth ( grid_line_width() );
	    gl2psLineWidth ( grid_line_width() );
	    glBegin ( GL_LINE_LOOP ); {
	      glVertex3f ( xlowend(), ylowend(), _z_axis ( *tic ) );
	      glVertex3f ( xhighend(), ylowend(), _z_axis ( *tic ) );
	      glVertex3f ( xhighend(), yhighend(), _z_axis ( *tic ) );
	      glVertex3f ( xlowend(), yhighend(), _z_axis ( *tic ) );
	    } glEnd();
	    gl2psDisable ( GL2PS_LINE_STIPPLE );
	  } glPopAttrib();
	}
      }
    }

    // minor ticks
    hx /= 2;
    hy /= 2;
    hz /= 2;
    for ( tic = _x_axis.sbegin(); tic != _x_axis.send(); ++tic ) {
      if ( xrotation() || yrotation() ) {
	glBegin ( GL_LINES ); {
	  glVertex3f ( _x_axis ( *tic ), ylowend(), zlowend() );
	  glVertex3f ( _x_axis ( *tic ), ylowend() + hy, zlowend() );
	} glEnd();
      } else {
	glBegin ( GL_LINES ); {
	  glVertex3f ( _x_axis ( *tic ), ylowend(), zhighend() );
	  glVertex3f ( _x_axis ( *tic ), ylowend() + hy, zhighend() );
	} glEnd();
      }
    }
    for ( tic = _y_axis.sbegin(); tic != _y_axis.send(); ++tic ) {
      if ( xrotation() || yrotation() ) {
	glBegin ( GL_LINES ); {
	  glVertex3f ( xlowend(), _y_axis ( *tic ), zlowend() );
	  glVertex3f ( xlowend() + hx, _y_axis ( *tic ), zlowend() );
	} glEnd();
      } else {
	glBegin ( GL_LINES ); {
	  glVertex3f ( xlowend(), _y_axis ( *tic ), zhighend() );
	  glVertex3f ( xlowend() + hx, _y_axis ( *tic ), zhighend() );
	} glEnd();
      }
    }
    if ( abs ( xrotation() ) > 10.0 || abs ( yrotation() ) > 10.0 ) {
      for ( tic = _z_axis.sbegin(); tic != _z_axis.send(); ++tic ) {
	glBegin ( GL_LINES ); {
	  glVertex3f ( xlowend(), ylowend(), _z_axis ( *tic ) );
	  glVertex3f ( xlowend() + hx, ylowend() + hy, _z_axis ( *tic ) );
	} glEnd();
      }
    }
  }

  // frames
  if ( xrotation() || yrotation() ) {
    glBegin ( GL_LINE_LOOP ); {
      glVertex3f ( xhighend(), ylowend(), zlowend() );
      glVertex3f ( xlowend(), ylowend(), zlowend() );
      glVertex3f ( xlowend(), yhighend(), zlowend() );
      glVertex3f ( xhighend(), yhighend(), zlowend() );
    } glEnd();
    glBegin ( GL_LINES ); {
      glVertex3f ( xhighend(), ylowend(), zlowend() );
      glVertex3f ( xhighend(), ylowend(), zhighend() );
      glVertex3f ( xlowend(), ylowend(), zlowend() );
      glVertex3f ( xlowend(), ylowend(), zhighend() );
      glVertex3f ( xlowend(), yhighend(), zlowend() );
      glVertex3f ( xlowend(), yhighend(), zhighend() );
      glVertex3f ( xhighend(), yhighend(), zlowend() );
      glVertex3f ( xhighend(), yhighend(), zhighend() );
    } glEnd();
  }
  glBegin ( GL_LINE_LOOP ); {
    glVertex3f ( xhighend(), ylowend(), zhighend() );
    glVertex3f ( xlowend(), ylowend(), zhighend() );
    glVertex3f ( xlowend(), yhighend(), zhighend() );
    glVertex3f ( xhighend(), yhighend(), zhighend() );
  } glEnd();

  glDisable ( GL_SCISSOR_TEST );
  glDisable ( GL_DEPTH_TEST );

  if ( key_visible() && roff > loff ) { // palette key

    glMatrixMode ( GL_PROJECTION );
    glPushMatrix();
    glLoadIdentity();
    double keyprj = roff - _width;
    gluOrtho2D ( ( keyprj -= _width ) /= roff, 1, 
		 ( yprj - _height ) / h, ( yprj + _height ) / h );

    glMatrixMode ( GL_MODELVIEW );
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D ( 0, roff, 
		 zlowend(), zhighend() );

    glPushAttrib ( GL_ALL_ATTRIB_BITS ); {
      glBegin ( GL_QUAD_STRIP ); {
	static const int gradations = 16;
	const double amplitude = zhigh() - zlow();
  	for ( register int i = 0; i <= gradations; ++i ) {
	  double z = zlow() + amplitude * i / gradations;
	  color dye = _palette ( ( z - zlow() ) / amplitude );
	  glColor3f ( dye.r(), dye.g(), dye.b() );
	  glVertex2f ( keyleft, _z_axis ( z ) );
	  glVertex2f ( keyright, _z_axis ( z ) );
	}
      } glEnd();
    } glPopAttrib();

    ticmark.x ( keyright );
    ticmark.z ( 0.0 );
    ticmark.align ( LEFT );
    for ( tic = _z_axis.begin(); tic != _z_axis.end(); ++tic ) {
      ticmark.y ( _z_axis ( *tic ) );
      ticmark.text ( "%.4g", *tic );
      ticmark.draw();
    }

    glMatrixMode ( GL_PROJECTION );
    glPopMatrix();
    glMatrixMode ( GL_MODELVIEW );
    glPopMatrix();
  }
}

void
glp::plot::save ( const string & filename ) const
{
  string ext = filename.substr ( filename.find_last_of ( '.' ) );
  transform ( ext.begin(), ext.end(), ext.begin(), ptr_fun ( tolower ) );

  int format = 
    ( ext == ".ps"  ) ? GL2PS_PS  :
    ( ext == ".eps" ) ? GL2PS_EPS :
    ( ext == ".tex" ) ? GL2PS_TEX :
    ( ext == ".pdf" ) ? GL2PS_PDF :
    ( ext == ".svg" ) ? GL2PS_SVG :
    ( ext == ".pgf" ) ? GL2PS_PGF :
    GL2PS_EPS; // this is a default

  FILE * fp = fopen ( filename.c_str(), "wb" );

  int state = GL2PS_OVERFLOW, buffsize = 0;

  while ( state == GL2PS_OVERFLOW ) {
    buffsize += 1024*1024;
    gl2psBeginPage ( filename.c_str(), 
		     "gluplot", 
		     0, 
		     format, 
		     GL2PS_BSP_SORT, 
  		     GL2PS_USE_CURRENT_VIEWPORT | GL2PS_BEST_ROOT,
		     GL_RGBA, 0, NULL, 
		     8, 8, 8, 
		     buffsize, 
		     fp, 
		     filename.c_str() );
    draw();
    state = gl2psEndPage();
  }

  fclose(fp);
}

void
glp::plot::clipping ( bool yn ) const
{
  _clipping = yn;
}

void
glp::plot::clip ( const point & dir, const double & pos ) const
{
  double norm = distance ( dir, point ( 0, 0, 0 ) );
  double plane[] = { -dir.x() / norm, -dir.y() / norm, dir.z() / norm, pos };
  vector<double> clip_plane;
  copy ( plane, plane + 4, back_inserter ( clip_plane ) );
  _clip_planes.push_back ( clip_plane );
}

void
glp::plot::clip() const
{
  _clip_planes.clear();
}

/** Plot the curves in DOTS style */
void
glp::plot::draw_with_dots ( const curve & c ) const
{
  glPushAttrib ( GL_POINT_BIT ); {
    glBegin ( GL_POINTS ); {
      for ( unsigned int i = 0; i < c.size(); ++i ) {
	if ( visible ( c[i] ) ) {
	  glVertex2f ( _x_axis ( c[i].x() ), _y_axis ( c[i].y() ) );
	}
      }
    } glEnd();
  } glPopAttrib();
}

/** Plot the curves in LINES, COLOREDLINES style */
void
glp::plot::draw_with_lines ( const curve & c ) const
{
  bool tone = ( style_visible() ? style() : c.style() ) & PLOT_STYLE_HAS_TONE;
  double amplitude = yhigh() - ylow();

  glPushAttrib ( GL_LINE_BIT ); {
    glLineWidth ( line_width() );
    gl2psLineWidth ( line_width() );
    for ( unsigned int i = 1; i < c.size(); ++i ) {
      if ( visible ( c[i-1] ) && visible ( c[i] ) ) {
	glBegin ( GL_LINES ); {
	  if ( tone ) {
	    color dye = _palette ( ( c[i-1].y() - ylow() ) / amplitude );
	    glColor4f ( dye.r(), dye.g(), dye.b(), c.alpha() );
	  }
	  glVertex2f ( _x_axis ( c[i-1].x() ), _y_axis ( c[i-1].y() ) );
	  if ( tone ) {
	    color dye = _palette ( ( c[i].y() - ylow() ) / amplitude );
	    glColor4f ( dye.r(), dye.g(), dye.b(), c.alpha() );
	  }
	  glVertex2f ( _x_axis ( c[i].x() ), _y_axis ( c[i].y() ) );
	} glEnd();
      }
    }
  } glPopAttrib();
}

/** Plot the curves in POINTS style */
void
glp::plot::draw_with_points ( const curve & c ) const
{
  glPushAttrib ( GL_POINT_BIT ); {
    glPointSize ( point_size() );
    gl2psPointSize ( point_size() );
    glBegin ( GL_POINTS ); {
      for ( unsigned int i = 0; i < c.size(); ++i ) {
	if ( visible ( c[i] ) ) {
	  glVertex2f ( _x_axis ( c[i].x() ), _y_axis ( c[i].y() ) );
	}
      }
    } glEnd();
  } glPopAttrib();
}

/** Plot the curves in XERRORBARS, XERRORLINES style */
void
glp::plot::draw_with_xerrorbars ( const curve & c ) const
{
  glPushAttrib ( GL_LINE_BIT ); {
    glLineWidth ( line_width() );
    gl2psLineWidth ( line_width() );
    for ( unsigned int i = 0; i < c.size(); ++i ) {
      if ( visible ( c[i] ) ) {
	double x_low = c[i].xlow() < xlow() ? c[i].x() : c[i].xlow();
	double x_high = c[i].xhigh() > xhigh() ? c[i].x() : c[i].xhigh();
	glBegin ( GL_LINES ); {
	  glVertex2f ( _x_axis ( x_low ), _y_axis ( c[i].y() ) );
	  glVertex2f ( _x_axis ( x_high ), _y_axis ( c[i].y() ) );
	} glEnd();
      }
    }
  } glPopAttrib();
}

/** Plot the curves in YERRORBARS, YERRORLINES style */
void
glp::plot::draw_with_yerrorbars ( const curve & c ) const
{
  glPushAttrib ( GL_LINE_BIT ); {
    glLineWidth ( line_width() );
    gl2psLineWidth ( line_width() );
    for ( unsigned int i = 0; i < c.size(); ++i ) {
      if ( visible ( c[i] ) ) {
	double y_low = c[i].ylow() < ylow() ? c[i].y() : c[i].ylow();
	double y_high = c[i].yhigh() > yhigh() ? c[i].y() : c[i].yhigh();
	glBegin ( GL_LINES ); {
	  glVertex2f ( _x_axis ( c[i].x() ), _y_axis ( y_low ) );
	  glVertex2f ( _x_axis ( c[i].x() ), _y_axis ( y_high ) );
	} glEnd();
      }
    }
  } glPopAttrib();
}

/** Plot the curves in IMPULSES style */
void
glp::plot::draw_with_impulses ( const curve & c ) const
{
  glPushAttrib ( GL_LINE_BIT ); {
    glLineWidth ( line_width() );
    gl2psLineWidth ( line_width() );
    for ( unsigned int i = 0; i < c.size(); ++i ) {
      if ( visible ( c[i] ) ) {
	double y0 = yhigh() < 0 ? yhighend() : ylowend();
	glBegin ( GL_LINES ); {
	  glVertex2f ( _x_axis ( c[i].x() ), y0 );
	  glVertex2f ( _x_axis ( c[i].x() ), _y_axis ( c[i].y() ) );
	} glEnd();
      }
    }
  } glPopAttrib();
}

/** Plot the surfaces in DOTS style */
void
glp::plot::draw_with_dots ( const surface & s ) const
{
  if ( s.empty() ) return;
  glPushAttrib ( GL_POINT_BIT ); {
    glBegin ( GL_POINTS ); {
      for ( surface::const_iterator p = s.begin(); p != s.end(); ++p ) {
	if ( visible ( *p ) ) {
	  glVertex3f ( _x_axis ( p->x() ), _y_axis ( p->y() ), _z_axis ( p->z() ) );
	}
      }
    } glEnd();
  } glPopAttrib();
}

/** Plot the surfaces in LINES, COLOREDLINES style */
void
glp::plot::draw_with_lines ( const surface & s ) const
{
  bool tone = ( style_visible() ? style() : s.style() ) & PLOT_STYLE_HAS_TONE;
  double amplitude = zhigh() - zlow();

  glPushAttrib ( GL_LINE_BIT ); {
    glLineWidth ( line_width() );
    gl2psLineWidth ( line_width() );
    for ( unsigned int i = 0; i < s.size(); ++i ) {
      bool yes = true;
      for ( unsigned int k = 0; k < s[i].size(); ++k ) {
	if ( !visible (  s[i][k]) ) yes = false;
      }
      if ( yes ) {
	glBegin ( GL_LINE_LOOP ); {
	  for ( unsigned int j = 0; j < s[i].size(); ++j ) {
	    if ( tone ) {
	      color dye = _palette ( ( s[i][j].z() - zlow() ) / amplitude );
	      glColor3f ( dye.r(), dye.g(), dye.b() );
	    }
	    glVertex3f ( _x_axis ( s[i][j].x() ), _y_axis ( s[i][j].y() ), _z_axis ( s[i][j].z() ) );
	  }
	} glEnd();
      }
    }
  } glPopAttrib();
}

/** Plot the surfaces in POINTS style */
void
glp::plot::draw_with_points ( const surface & s ) const
{
  if ( s.empty() ) return;
  glPushAttrib ( GL_POINT_BIT ); {
    glPointSize ( point_size() );
    gl2psPointSize ( point_size() );
    glBegin ( GL_POINTS ); {
      for ( surface::const_iterator p = s.begin(); p != s.end(); ++p ) {
	if ( visible ( *p ) ) {
	  glVertex3f ( _x_axis ( p->x() ), _y_axis ( p->y() ), _z_axis ( p->z() ) );
	}
      }
    } glEnd();
  } glPopAttrib();
}

/** Plot the surfaces in ERRORBARS style */
void
glp::plot::draw_with_errorbars ( const surface & s ) const
{
  glPushAttrib ( GL_LINE_BIT ); {
    glLineWidth ( line_width() );
    gl2psLineWidth ( line_width() );
    for ( surface::const_iterator p = s.begin(); p != s.end(); ++p ) {
      if ( visible ( *p ) ) {
	glBegin ( GL_LINES ); {
	  glVertex3f ( _x_axis ( p->xlow() ), _y_axis ( p->ylow() ), _z_axis ( p->zlow() ) );
	  glVertex3f ( _x_axis ( p->xhigh() ), _y_axis ( p->yhigh() ), _z_axis ( p->zhigh() ) );
	} glEnd();
      }
    }
  } glPopAttrib();
}

#include <numeric>

/** Plot the surfaces in LIGHTING style */
void
glp::plot::draw_with_lighting ( const surface & s ) const
{
  glEnable ( GL_LIGHTING );

  unsigned int npass = 1;
  if ( s.alpha() < 1 ) {
    glEnable ( GL_CULL_FACE );
    npass = 2;
  }

  point center ( 0, 0, 0 );
  double amplitude = 0;
  if ( s.volume() ) {
    center = accumulate ( s.begin(), s.end(), center ) / ( s.end() - s.begin() );
    for ( surface::const_iterator p = s.begin(); p != s.end(); ++p )
      amplitude = max ( amplitude, distance ( *p, center ) );
    amplitude = max ( amplitude, xhigh() - center.x() );
    amplitude = max ( amplitude, center.x() - xlow() );
    amplitude = max ( amplitude, yhigh() - center.y() );
    amplitude = max ( amplitude, center.y() - ylow() );
    amplitude = max ( amplitude, zhigh() - center.z() );
    amplitude = max ( amplitude, center.z() - zlow() );
  } else {
    amplitude = zhigh() - zlow();
  }
  color dye;

  glPushAttrib ( GL_POLYGON_BIT ); {
    for ( unsigned int pass = 0; pass < npass; ++pass ) {
      npass > 1 && pass == 0 ? glFrontFace ( GL_CW ) : glFrontFace ( GL_CCW );
      for ( unsigned int i = 0; i < s.size(); ++i ) {
	bool yes = true;
	for ( unsigned int k = 0; k < s[i].size(); ++k ) {
	  if ( !visible (  s[i][k]) ) yes = false;
	}
	if ( yes ) {
	  const point & n = s.n()[i];
	  glBegin ( GL_POLYGON ); {
	    for ( unsigned int j = 0; j < s[i].size(); ++j ) {
	      if ( amplitude ) {
		if ( s.volume() ) {
		  dye = _palette ( distance ( s[i][j], center ) / amplitude );
		} else {
		  dye = _palette ( ( s[i][j].z() - zlow() ) / amplitude );
		}
		float col[] = { dye.r(), dye.g(), dye.b(), s.alpha() };
		glColor4fv ( col );
		glMaterialfv ( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, col );
		//glMaterialfv ( GL_FRONT_AND_BACK, GL_DIFFUSE, col );
	      }
// 	      point n = s[i][j] - center;
// 	      glNormal3f ( _x_axis ( n.x() ),
// 			   _y_axis ( n.y() ),
// 			   _z_axis ( n.z() ) );
	      glNormal3f ( n.x(), n.y(), n.z() );
	      glVertex3f ( _x_axis ( s[i][j].x() ),
			   _y_axis ( s[i][j].y() ),
			   _z_axis ( s[i][j].z() ) );
	    }
	  } glEnd();
	}
      }
    }
  } glPopAttrib();

  if ( s.alpha() < 1 ) glEnable ( GL_CULL_FACE );

  glDisable ( GL_LIGHTING );
}

/** Plot the surfaces */
void
glp::plot::draw ( const surface & s ) const
{
  unsigned int npass = 1;
  if ( s.alpha() < 1 ) {
    glEnable ( GL_CULL_FACE );
    npass = 2;
  }

  double amplitude = zhigh() - zlow();
  color dye;

  glPushAttrib ( GL_POLYGON_BIT ); {
    for ( unsigned int pass = 0; pass < npass; ++pass ) {
      npass > 1 && pass == 0 ? glFrontFace ( GL_CW ) : glFrontFace ( GL_CCW );
      for ( unsigned int i = 0; i < s.size(); ++i ) {
	bool yes = true;
	for ( unsigned int k = 0; k < s[i].size(); ++k ) {
	  if ( !visible (  s[i][k] ) ) yes = false;
	}
	if ( yes ) {
	  glBegin ( GL_POLYGON ); {
	    for ( unsigned int j = 0; j < s[i].size(); ++j ) {
	      if ( amplitude ) {
		dye = _palette ( ( s[i][j].z() - zlow() ) / amplitude );
		glColor4f ( dye.r(), dye.g(), dye.b(), s.alpha() );
	      }
	      glVertex3f ( _x_axis ( s[i][j].x() ),
			   _y_axis ( s[i][j].y() ),
			   _z_axis ( s[i][j].z() ) );
	    }
	  } glEnd();
	}
      }
    }
  } glPopAttrib();

  if ( s.alpha() < 1 ) glEnable ( GL_CULL_FACE );
}
