/* -*- C++ -*- *************************************************************
 *
 *   glp_style.h, Sun Oct 21 20:27:54 MSD 2007
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

#ifndef GLP_STYLE_H
#define GLP_STYLE_H

namespace glp {

  enum PLOT_STYLE_FLAGS {
    PLOT_STYLE_HAS_LINE       = (1<<0),
    PLOT_STYLE_HAS_POINT      = (1<<1),
    PLOT_STYLE_HAS_XERRORBAR  = (1<<2),
    PLOT_STYLE_HAS_YERRORBAR  = (1<<3),
    PLOT_STYLE_HAS_ZERRORBAR  = (1<<4),
    PLOT_STYLE_HAS_ERRORBAR   = (PLOT_STYLE_HAS_XERRORBAR |
				 PLOT_STYLE_HAS_YERRORBAR |
				 PLOT_STYLE_HAS_ZERRORBAR),
    PLOT_STYLE_HAS_TONE       = (1<<5),
    PLOT_STYLE_BITS           = (1<<6)
  };

  enum PLOT_STYLE {
    DOTS           = PLOT_STYLE_BITS + 0,
    LINES          = PLOT_STYLE_BITS + PLOT_STYLE_HAS_LINE,
    POINTS         = PLOT_STYLE_BITS + PLOT_STYLE_HAS_POINT,
    LINESPOINTS    = PLOT_STYLE_BITS + (PLOT_STYLE_HAS_POINT | 
					PLOT_STYLE_HAS_LINE),
    XERRORBARS     = PLOT_STYLE_BITS + (PLOT_STYLE_HAS_POINT | 
					PLOT_STYLE_HAS_XERRORBAR),
    YERRORBARS     = PLOT_STYLE_BITS + (PLOT_STYLE_HAS_POINT | 
					PLOT_STYLE_HAS_YERRORBAR),
    XYERRORBARS    = PLOT_STYLE_BITS + (PLOT_STYLE_HAS_POINT | 
					PLOT_STYLE_HAS_XERRORBAR | 
					PLOT_STYLE_HAS_YERRORBAR),
    XERRORLINES    = PLOT_STYLE_BITS + (PLOT_STYLE_HAS_LINE | 
					PLOT_STYLE_HAS_POINT | 
					PLOT_STYLE_HAS_XERRORBAR),
    YERRORLINES    = PLOT_STYLE_BITS + (PLOT_STYLE_HAS_LINE | 
					PLOT_STYLE_HAS_POINT | 
					PLOT_STYLE_HAS_YERRORBAR),
    XYERRORLINES   = PLOT_STYLE_BITS + (PLOT_STYLE_HAS_LINE | 
					PLOT_STYLE_HAS_POINT | 
					PLOT_STYLE_HAS_XERRORBAR | 
					PLOT_STYLE_HAS_YERRORBAR),
    IMPULSES       = 2*PLOT_STYLE_BITS + 0,
    COLOREDLINES   = PLOT_STYLE_BITS + (PLOT_STYLE_HAS_LINE |
					PLOT_STYLE_HAS_TONE),
    COLORMAP       = PLOT_STYLE_BITS + PLOT_STYLE_HAS_TONE,
    ERRORBARS      = PLOT_STYLE_BITS + (PLOT_STYLE_HAS_POINT | 
					PLOT_STYLE_HAS_ERRORBAR),
    ERRORLINES     = PLOT_STYLE_BITS + (PLOT_STYLE_HAS_LINE | 
					PLOT_STYLE_HAS_POINT | 
					PLOT_STYLE_HAS_ERRORBAR),
    LIGHTING       = 3*PLOT_STYLE_BITS + 0,
    KILL           = 0
  };

} /** namespace glp */

#endif // GLP_STYLE_H

