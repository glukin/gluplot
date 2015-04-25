/* -*- C++ -*- *************************************************************
 *
 *   glp_option.h, Fri Nov 19 20:13:29 MSK 2004
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

#ifndef GLP_OPTION_H
#define GLP_OPTION_H

#include "glp_curve.h"
#include "glp_surface.h"
#include "glp_label.h"
#include "glp_point.h"

namespace glp {

  /***************************************************************************
   *
   * Some settings are driven by option strings
   *
   ***************************************************************************/

  enum PLOT_STYLE plot_style ( const char * key );
  enum TEXT_FONT text_font ( const char * fnt );
  enum COLOR_SCHEME color_scheme ( const char * key );

  const curve curve_from_file ( const char * filename,
				const char * cols = 0, const char * slice = 0 );
  const surface surface_from_file ( const char * filename,
				    const char * cols = 0, const char * slice = 0 );

  void plane_from_str ( const char * key, point & dir, double & pos );

} /** namespace glp */

#endif // GLP_OPTION_H

