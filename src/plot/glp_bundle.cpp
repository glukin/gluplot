/* -*- C++ -*- *************************************************************
 *
 *   glp_bundle.cpp, Thu Feb 10 16:48:00 MSK 2005
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

#include "glp_bundle.h"

#include <algorithm>
using namespace std;

glp::bundle::bundle()
{
  clear();
  //at ( 0 );
}

glp::bundle::~bundle() {}

void
glp::bundle::on ( root * origin )
{
  _root = origin;
}

void
glp::bundle::insert ( size_t i )
{
  base::push_back ( i );
  arrange();
}

void
glp::bundle::clear()
{
  base::clear();
}

void
glp::bundle::arrange()
{
  iterator i = min_element ( begin(), end() );
  rotate ( begin(), i, end() );
  ++( i = begin() );
  if ( i != end() && *i > *rbegin() ) reverse ( i, end() );
}

bool
glp::operator== ( const bundle & lhs, const bundle & rhs )
{
  return 
    lhs.size() == rhs.size() &&
    equal ( lhs.begin(), lhs.end(), rhs.begin() );
}

bool
glp::operator!= ( const bundle & lhs, const bundle & rhs )
{
  return !( lhs == rhs );
}
