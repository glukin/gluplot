/* -*- C++ -*- *************************************************************
 *
 *   gluplot.h, Fri Oct  1 11:37:50 MSD 2004
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

#ifndef GLUPLOT_H
#define GLUPLOT_H

#include "glp_point.h"
#include "glp_curve.h"
#include "glp_surface.h"
#include "glp_style.h"
#include "glp_axis.h"
#include "glp_color.h"
#include "glp_plot.h"
#include "glp_option.h"
#include "glp_window.h"

#include "glp_glut_window.h"
typedef glp::window gluplot;

#ifdef HAVE_FLTK
#include "glp_fltk_window.h"
typedef glp::fl_window fluplot;
#endif

#endif // GLUPLOT_H

