/* -*- C++ -*- *************************************************************
 *
 *   glp_color.cpp, Sun Oct 10 21:39:03 MSD 2004
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

#include "glp_color.h"

#include <cmath>
using namespace std;

/***************************************************************************
 *
 * Standard colors. RGB values are uchars in range [0,255].
 *
 ***************************************************************************/

const struct glp::rgb
glp::BLACK   = {   0,   0,   0 }, \
glp::RED     = { 255,   0,   0 }, \
glp::GREEN   = {   0, 255,   0 }, \
glp::BLUE    = {   0,   0, 255 }, \
glp::CYAN    = {   0, 255, 255 }, \
glp::MAGENTA = { 255,   0, 255 }, \
glp::YELLOW  = { 255, 255,   0 }, \
glp::WHITE   = { 255, 255, 255 };

bool
glp::operator== ( const rgb & lhs, const rgb & rhs )
{
  return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b;
}

/***************************************************************************
 *
 * Color in RGB scheme. RGB values are all in range [0,1].
 *
 ***************************************************************************/

glp::color::color()
  : _r(), _g(), _b()
{}

glp::color::color ( const double & r, const double & g, const double & b )
  : _r ( r < 0 ? 0 : r > 1 ? 1 : r ),
    _g ( g < 0 ? 0 : g > 1 ? 1 : g ),
    _b ( b < 0 ? 0 : b > 1 ? 1 : b )
{}

glp::color::~color() {}

/** map RGB from uchars [0,255] to float values [0,1] */
glp::color::color ( const struct rgb & rhs )
  : _r ( static_cast<double> ( rhs.r ) / 255 ),
    _g ( static_cast<double> ( rhs.g ) / 255 ),
    _b ( static_cast<double> ( rhs.b ) / 255 )
{}

/** map RGB from float values [0,1] to uchars [0,255] */
glp::color::operator glp::rgb() const
{
  rgb lhs = { static_cast<unsigned char> ( 255 * r() + 0.5 ),
	      static_cast<unsigned char> ( 255 * g() + 0.5 ),
	      static_cast<unsigned char> ( 255 * b() + 0.5 ) };
  return lhs;
}

const glp::color
glp::average ( const color & c1, const color & c2, 
	       const double & weight )
{
  return color ( weight * c1.r() + ( 1 - weight ) * c2.r(), 
		 weight * c1.g() + ( 1 - weight ) * c2.g(), 
		 weight * c1.b() + ( 1 - weight ) * c2.b() );
}

/***************************************************************************
 *
 * Color palette
 *
 ***************************************************************************/

glp::palette::palette()
{
  rgbscheme ( RAINBOW );
  gamma ( 1.5 );
  grayscale ( false );
  negative ( false );
}

glp::palette::~palette() {}

/** map a gray value in [0,1] to the corresponding RGB values in [0,1] */
const glp::color
glp::palette::operator() ( const double & gray ) const
{
  double x = negative() ? 1 - gray : gray;
  double r, g, b;
  if ( grayscale() ) {
    r = g = b = pow ( x, 1.0 / gamma() );
  } else {
    r = _rmap ( x );
    g = _gmap ( x );
    b = _bmap ( x );
  }
  return color ( r, g, b );
}

namespace glp { /** original fixed color transformations */

  double rainbow_r ( double x );
  double rainbow_g ( double x );
  double rainbow_b ( double x );
  double hot_r ( double x );
  double hot_g ( double x );
  double hot_b ( double x );
  double ocean_r ( double x );
  double ocean_g ( double x );
  double ocean_b ( double x );
  double terrain_r ( double x );
  double terrain_g ( double x );
  double terrain_b ( double x );
  double traditional_r ( double x );
  double traditional_g ( double x );
  double traditional_b ( double x );

  namespace light { /** visible colors */

    /** the signature of color is the light wavelength */
    static const double wavelength[8] = { /*[m]*/
      /*   violet    blue     cyan    green   yellow   orange     red    */
      380e-9,  435e-9,  500e-9,  520e-9,  565e-9,  590e-9,  625e-9,  740e-9 
      /* (hyperphysics.phy-astr.gsu.edu/hbase/vision/specol.html) */
    };

  } /** namespace light */

  namespace black_body { /** black body radiation */

    /** the intensity of radiation from a black body at temperature T
	is given by Planck's law */
    const double planck_intensity ( const double & wavelength /*[m]*/, 
				    const double & temperature /*[K]*/ );
    /** the wavelength at which the radiation is strongest
	is given by Wien's law */
    const double wien_wavelength ( const double & temperature /*[K]*/ );

  } /** namespace black_body */

} /** namespace glp */

void
glp::palette::rgbscheme ( enum COLOR_SCHEME scheme )
{
  grayscale ( false );
  switch ( scheme ) {
  case RAINBOW:
    rgbformulae ( rainbow_r, rainbow_g, rainbow_b );
    break;
  case HOT:
    rgbformulae ( hot_r, hot_g, hot_b );
    break;
  case OCEAN:
    rgbformulae ( ocean_r, ocean_g, ocean_b );
    break;
  case TERRAIN:
    rgbformulae ( terrain_r, terrain_g, terrain_b );
    break;
  case TRADITIONAL:
    rgbformulae ( traditional_r, traditional_g, traditional_b );
    break;
  default:
    grayscale ( true );
    break;
  }
}

void
glp::palette::rgbformulae ( formula r, formula g, formula b )
{
  _rmap = r;
  _gmap = g;
  _bmap = b;
}

/***************************************************************************
 *
 * original fixed color transformations
 *
 ***************************************************************************/

double
glp::rainbow_r ( double x )
{
  using namespace light;

  // http://www.physics.sfasu.edu/astro/color.html

  ( x = 400 * ( 1 - x ) + 700 * x ) *= 1e-9;
  register double res = 0;

  if ( x >= wavelength[0] && x < wavelength[1] ) {
    res = ( x - wavelength[0] ) / ( wavelength[0] - wavelength[1] );
    ( ( res *= res ) -= 1 ) *= res; // Tukey's bi-square
  }
  if ( x >= wavelength[3] && x < wavelength[5] ) {
    res = ( x - wavelength[5] ) / ( wavelength[3] - wavelength[5] );
    ( ( res *= res ) -= 1 ) *= res; // Tukey's bi-square
  }
  if ( x >= wavelength[5] && x < wavelength[7] )
    res = 1;

  // let the intensity fall off near the vision limits
  register double weight = 1;
  if ( x >= wavelength[0] && x < wavelength[1] ) {
    weight =  ( x - wavelength[1] ) / ( wavelength[0] - wavelength[1] );
    ( ( weight *= weight ) -= 1 ) *= weight; // Tukey's bi-square
  }
  if ( x >= wavelength[6] && x < wavelength[7] ) {
    weight =  ( x - wavelength[6] ) / ( wavelength[6] - wavelength[7] );
    ( ( weight *= weight ) -= 1 ) *= weight; // Tukey's bi-square
  }

  return res *= weight;
}

double
glp::rainbow_g ( double x )
{
  using namespace light;

  // http://www.physics.sfasu.edu/astro/color.html

  ( x = 400 * ( 1 - x ) + 700 * x ) *= 1e-9;
  register double res = 0;

  if ( x >= wavelength[1] && x < wavelength[2] ) {
    res = ( x - wavelength[2] ) / ( wavelength[1] - wavelength[2] );
    ( ( res *= res ) -= 1 ) *= res; // Tukey's bi-square
  }
  if ( x >= wavelength[2] && x < wavelength[5] )
    res = 1;
  if ( x >= wavelength[5] && x < wavelength[6] ) {
    res = ( x - wavelength[5] ) / ( wavelength[5] - wavelength[6] );
    ( ( res *= res ) -= 1 ) *= res; // Tukey's bi-square
  }

  // let the intensity fall off near the vision limits
  register double weight = 1;
  if ( x >= wavelength[0] && x < wavelength[1] ) {
    weight =  ( x - wavelength[1] ) / ( wavelength[0] - wavelength[1] );
    ( ( weight *= weight ) -= 1 ) *= weight; // Tukey's bi-square
  }
  if ( x >= wavelength[6] && x < wavelength[7] ) {
    weight =  ( x - wavelength[6] ) / ( wavelength[6] - wavelength[7] );
    ( ( weight *= weight ) -= 1 ) *= weight; // Tukey's bi-square
  }

  return res *= weight;
}

double
glp::rainbow_b ( double x )
{
  using namespace light;

  // http://www.physics.sfasu.edu/astro/color.html

  ( x = 400 * ( 1 - x ) + 700 * x ) *= 1e-9;
  register double res = 0;

  if ( x >= wavelength[0] && x < wavelength[2] )
    res = 1;
  if ( x >= wavelength[2] && x < wavelength[3] ) {
    res = ( x - wavelength[2] ) / ( wavelength[2] - wavelength[3] );
    ( ( res *= res ) -= 1 ) *= res; // Tukey's bi-square
  }

  // let the intensity fall off near the vision limits
  register double weight = 1;
  if ( x >= wavelength[0] && x < wavelength[1] ) {
    weight =  ( x - wavelength[1] ) / ( wavelength[0] - wavelength[1] );
    ( ( weight *= weight ) -= 1 ) *= weight; // Tukey's bi-square
  }
  if ( x >= wavelength[6] && x < wavelength[7] ) {
    weight =  ( x - wavelength[6] ) / ( wavelength[6] - wavelength[7] );
    ( ( weight *= weight ) -= 1 ) *= weight; // Tukey's bi-square
  }

  return res *= weight;
}

double
glp::hot_r ( double x )
{
  using namespace light;
  using namespace black_body;

  x = 2000 * ( 1 - x ) + 5400 * x;

  double wlmax = wien_wavelength ( x );
  if ( wlmax < wavelength[5] ) 
    return 
      planck_intensity ( wavelength[5], x ) / 
      planck_intensity ( wlmax, x );
  if ( wlmax > wavelength[7] ) 
    return 
      planck_intensity ( wavelength[7], x ) / 
      planck_intensity ( wlmax, x );
  return 1;
}

double
glp::hot_g ( double x )
{
  using namespace light;
  using namespace black_body;

  x = 2000 * ( 1 - x ) + 5400 * x;

  double wlmax = wien_wavelength ( x );
  if ( wlmax < wavelength[2] ) 
    return 
      planck_intensity ( wavelength[2], x ) / 
      planck_intensity ( wlmax, x );
  if ( wlmax > wavelength[5] ) 
    return 
      planck_intensity ( wavelength[5], x ) / 
      planck_intensity ( wlmax, x );
  return 1;
}

double
glp::hot_b ( double x )
{
  using namespace light;
  using namespace black_body;

  x = 2000 * ( 1 - x ) + 5400 * x;

  double wlmax = wien_wavelength ( x );
  if ( wlmax < wavelength[0] ) 
    return 
      planck_intensity ( wavelength[0], x ) / 
      planck_intensity ( wlmax, x );
  if ( wlmax > wavelength[2] ) 
    return 
      planck_intensity ( wavelength[2], x ) / 
      planck_intensity ( wlmax, x );
  return 1;
}

double
glp::ocean_r ( double x )
{
  // A. Morel and L. Prieur, "Analysis of variations in ocean color",
  // Limnol. Oceanogr., 22, 709--722, (1977).
  // (omlc.ogi.edu/spectra/water/data/morel77.dat)
  static const double absorption = 0.00370; // [1/cm]

  ( x = 2000 * ( 1 - x ) ) *= absorption;
  return exp ( -x );
}

double
glp::ocean_g ( double x )
{
  // A. Morel and L. Prieur, "Analysis of variations in ocean color",
  // Limnol. Oceanogr., 22, 709--722, (1977).
  // (omlc.ogi.edu/spectra/water/data/morel77.dat)
  static const double absorption = 0.000697; // [1/cm]

  ( x = 2000 * ( 1 - x ) ) *= absorption;
  return exp ( -x );
}

double
glp::ocean_b ( double x )
{
  // A. Morel and L. Prieur, "Analysis of variations in ocean color",
  // Limnol. Oceanogr., 22, 709--722, (1977).
  // (omlc.ogi.edu/spectra/water/data/morel77.dat)
  static const double absorption = 0.000181; // [1/cm]

  ( x = 2000 * ( 1 - x ) ) *= absorption;
  return exp ( -x );
}

double
glp::terrain_r ( double x )
{
  // http://www.ferret.noaa.gov/Ferret/FAQ/graphics/colorpalettes.html

  static const double elevation[]  = { 0.0, 0.3,  0.8,  1.0 };
  static const double density[]    = { 0.0, 0.67, 0.75, 0.75 };

  if ( x < elevation[0] ) return 0;
  if ( x == elevation[0] ) return density[0];

  for ( int i = 1; i < 4; ++i ) {
    if ( x < elevation[i] ) {
      double xx = ( x - elevation[i] ) / ( x - elevation[i-1] );
      return ( density[i] - xx * density[i-1] ) / ( 1 - xx );
    }
    if ( x == elevation[i] ) return density[i];
  }

  return 0;
}

double
glp::terrain_g ( double x )
{
  // http://www.ferret.noaa.gov/Ferret/FAQ/graphics/colorpalettes.html

  static const double elevation[]  = { 0.0, 0.3,  0.8, 1.0 };
  static const double density[]    = { 0.5, 0.67, 0.4, 0.7 };

  if ( x < elevation[0] ) return 0;
  if ( x == elevation[0] ) return density[0];

  for ( int i = 1; i < 4; ++i ) {
    if ( x < elevation[i] ) {
      double xx = ( x - elevation[i] ) / ( x - elevation[i-1] );
      return ( density[i] - xx * density[i-1] ) / ( 1 - xx );
    }
    if ( x == elevation[i] ) return density[i];
  }

  return 0;
}

double
glp::terrain_b ( double x )
{
  // http://www.ferret.noaa.gov/Ferret/FAQ/graphics/colorpalettes.html

  static const double elevation[]  = { 0.0, 0.8, 1.0  };
  static const double density[]    = { 0.0, 0.0, 0.65 };

  if ( x < elevation[0] ) return 0;
  if ( x == elevation[0] ) return density[0];

  for ( int i = 1; i < 3; ++i ) {
    if ( x < elevation[i] ) {
      double xx = ( x - elevation[i] ) / ( x - elevation[i-1] );
      return ( density[i] - xx * density[i-1] ) / ( 1 - xx );
    }
    if ( x == elevation[i] ) return density[i];
  }

  return 0;
}

double
glp::traditional_r ( double x )
{
  return sqrt ( x );
}

double
glp::traditional_g ( double x )
{
  return x * x * x;
}

double
glp::traditional_b ( double x )
{
  return sin ( 2 * M_PI * x );
}

/** the intensity of radiation from a black body at temperature T
    is given by Planck's law */
const double
glp::black_body::planck_intensity ( const double & wavelength /*[m]*/, 
				    const double & temperature /*[K]*/ )
{
  static const double h = 6.6260693e-34;  // Planck's constant [J*s]
  static const double c = 299792458;      // speed of light [m/s]
  static const double k = 1.3806503e-23;  // Boltzmann's constant [J/K]

  static const double c1 = 2 * h * c * c;
  static const double c2 = h * c / k;

  return 
    c1 / pow ( wavelength, 5 ) / 
    ( exp ( c2 / wavelength / temperature ) - 1 );
}

/** the wavelength at which the radiation is strongest
    is given by Wien's law */
const double
glp::black_body::wien_wavelength ( const double & temperature /*[K]*/ )
{
  return 0.002898 / temperature;
}

