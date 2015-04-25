/* -*- C++ -*- *************************************************************
 *
 *   terrain.cpp, Mon Mar  7 15:55:20 MSK 2005
 *   Copyright (C) 2005 Soloviev Alexei <solovjev@cv.jinr.ru>
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

#include "gluplot.h"
using namespace glp;

#include "ppm.h"

#include <iostream>
#include <fstream>
using namespace std;

int main ( int argc, char** argv )
{
  if ( argc > 2 ) {
    cerr << "Usage: " << argv[0] << " [every]" << endl;
    return 1;
  }

  int every = 1;
  if ( argc == 2 ) every = atoi ( argv[1] );

  // load image
  int width, height;
  unsigned char * image = read_ppm ( "dubna_sh.ppm", &width, &height );

  // convert image to grayscale
  int size = width*height;
  //float grayscale[size];
  float * grayscale = new float[size];
  for ( int i = 0; i < size; ++i ) {
    grayscale[i] = 
      0.30 * image[3*i] +    // red
      0.59 * image[3*i+1] +  // green
      0.11 * image[3*i+2];   // blue
    grayscale[i] /= 255;
  }

  // interpret grayscale as height map
  surface relief;
  int xsize = 0, ysize = 0;
  for ( int h = 0; h < height; h += every ) {
    for ( int w = 0; w < width; w += every ) {
      relief.push_back ( point ( w, height-h-1, grayscale[h*width+w] ) );
      if ( !ysize ) ++xsize;
    }
    ++ysize;
  }
  delete [] grayscale;
  // manual mesing if needed
  if ( every < 10 ) {
    for ( int i = 1; i < xsize; ++i ) {
      for ( int j = 1; j < ysize; ++j ) {
	bundle node;
	node.insert ( (i-1) + (j-1) * xsize );
	node.insert ( (i) + (j-1) * xsize );
	node.insert ( (i) + (j) * xsize );
	node.insert ( (i-1) + (j) * xsize );
	relief.push_back ( node );
      }
    }
  }

  gluplot pout;

  pout.key_visible ( false );

  pout.background ( BLACK );
  pout.foreground ( WHITE );

  pout.rgbscheme ( glp::TERRAIN );

  pout << COLORMAP << relief;

  label legend;
  legend.x ( pout.xhigh() - ( pout.xhighend() - pout.xhigh() ) );
  legend.y ( pout.yhigh() - ( pout.yhighend() - pout.yhigh() ) );
  legend.z ( pout.zhigh() );
  legend.align ( RIGHT );
  legend.text ( "Dubna is a small town located on the North of Moscow Region..." );

  pout << legend;

  return 0;
}

