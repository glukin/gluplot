/* -*- C++ -*- *************************************************************
 *
 *   glp_surface.cpp, Tue Jan 18 14:17:16 MSK 2005
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

#include "glp_surface.h"

#include <iostream>
#include <algorithm>
#include <numeric>
#include <cmath>
using namespace std;

glp::surface_traits::surface_traits()
  : _style ( COLORMAP ),
    _alpha ( 1 )
{}

glp::surface_traits::~surface_traits() {}

glp::surface::surface()
  : surface_traits(),
    base(), mesh(),
    _xlow(), _xhigh(),
    _ylow(), _yhigh(),
    _zlow(), _zhigh(),
    _volume(false),
    _normals()
{}

glp::surface::surface ( const surface & rhs )
  : surface_traits(rhs),
    base(rhs), mesh(rhs),
    _xlow(rhs.xlow()), _xhigh(rhs.xhigh()),
    _ylow(rhs.ylow()), _yhigh(rhs.yhigh()),
    _zlow(rhs.ylow()), _zhigh(rhs.yhigh()),
    _volume(rhs.volume()),
    _normals(rhs.n())
{
  for ( size_type i = 0; i < size(); ++i ) (*this)[i].on ( this );
}

glp::surface &
glp::surface::operator= ( const surface & rhs )
{
  if ( this == &rhs ) return *this;
  static_cast<surface_traits&>(*this) = rhs;
  static_cast<base&>(*this) = rhs;
  static_cast<mesh&>(*this) = rhs;
  _xlow = rhs.xlow(); _xhigh = rhs.xhigh();
  _ylow = rhs.ylow(); _yhigh = rhs.yhigh();
  _zlow = rhs.ylow(); _zhigh = rhs.yhigh();
  _volume = rhs.volume();
  _normals = rhs.n();
  for ( size_type i = 0; i < size(); ++i ) (*this)[i].on ( this );
  return *this;
}

glp::surface::~surface() {}

void
glp::surface::push_back ( const point & p )
{
  fit_to ( p );
  base::push_back ( p );
}

glp::surface::iterator
glp::surface::insert ( iterator position, const point & p )
{
  fit_to ( p );
  return base::insert ( position, p );
}

void
glp::surface::fit_to ( const point & p )
{
  if ( empty() ) {
    _xlow = _xhigh = p.x();
    _ylow = _yhigh = p.y();
    _zlow = _zhigh = p.z();
  } else {
    _xlow = min ( _xlow, p.x() );
    _xhigh = max ( _xhigh, p.x() );
    _ylow = min ( _ylow, p.y() );
    _yhigh = max ( _yhigh, p.y() );
    _zlow = min ( _zlow, p.z() );
    _zhigh = max ( _zhigh, p.z() );
  }
}

glp::surface::iterator
glp::surface::erase ( iterator first, iterator last )
{
  iterator res = base::erase ( first, last );
  const_iterator p = begin();
  if ( p != end() ) {
    _xlow = _xhigh = p->x();
    _ylow = _yhigh = p->y();
    _zlow = _zhigh = p->z();
    while ( ++p != end() ) {
      _xlow = min ( _xlow, p->x() );
      _xhigh = max ( _xhigh, p->x() );
      _ylow = min ( _ylow, p->y() );
      _yhigh = max ( _yhigh, p->y() );
      _zlow = min ( _zlow, p->z() );
      _zhigh = max ( _zhigh, p->z() );
    }
  } else {
    _xlow = _xhigh = 0;
    _ylow = _yhigh = 0;
    _zlow = _zhigh = 0;
  }
  return res;
}

void
glp::surface::clear()
{
  mesh::clear();
  base::clear();
  _xlow = _xhigh = 0;
  _ylow = _yhigh = 0;
  _zlow = _zhigh = 0;
  _volume = false;
  _normals.clear();
}

void
glp::surface::push_back ( const bundle & b )
{
  mesh::push_back ( b );
}

void
glp::surface::mkmesh()
{
  if ( size() ) { // having mesh? nothing to do!
    for ( size_type i = 0; i < size(); ++i ) (*this)[i].on ( this );
  } else { // default meshing otherwise...
    triangulate();
  }
}

namespace glp { /** functions used for triangulation */

  namespace delaunay { /** Delaunay tests */

    /** is the point pd inside the circle passing through pa, pb, and pc? */
    bool incircle ( const point & pa, const point & pb, const point & pc,
		    const point & pd );
    /** is the point p inside the polygon [first:last)? */
    bool inside ( const point * first, const point * last, const point & p );
    /** arrange points by gift wrap method */
    void sort ( point * first, point * last );

  } /** namespace delaunay */

  /** distance between the point pd ant the plane passing through pa, pb, and pc */
  const double
  distance ( const point & pa, const point & pb, const point & pc,
	     const point & pd );

} /** namespace glp */

void
glp::surface::triangulate()
{
  base & b = *this;
  mesh & m = *this;

  if ( size() ) {
    mesh::clear();
    _normals.clear();
  }

  using namespace delaunay;

  sort ( &*begin(), &*begin() + ( end() - begin() ) );

  double dx = xhigh() - xlow();
  double dy = yhigh() - ylow();
  double xmid = ( xlow() + xhigh() ) / 2;
  dx *= 20;
  dy *= 20;

  unsigned int na = b.size();
  b.push_back ( point ( xmid, yhigh() + 2 * dy ) );
  unsigned int nb = b.size();
  b.push_back ( point ( xmid - 2 * dx, ylow() - dy ) );
  unsigned int nc = b.size();
  b.push_back ( point ( xmid + 2 * dx, ylow() - dy ) );

  m.push_back ( triangle ( na, nb, nc ) ); // supertriangle
  _normals.push_back ( point ( 0, 0, 1 ) );

  for ( unsigned int i = 0; i < na; ++i ) {
    vector<bundle> edges;
    vector<bool> ok;
    for ( unsigned int j = 0; j < m.size(); ) {
      if ( incircle ( m[j][0], m[j][1], m[j][2], b[i] ) ) {
	edges.push_back ( edge ( m[j].at(0), m[j].at(1) ) );
	edges.push_back ( edge ( m[j].at(1), m[j].at(2) ) );
	edges.push_back ( edge ( m[j].at(2), m[j].at(0) ) );
	ok.push_back ( true );
	ok.push_back ( true );
	ok.push_back ( true );
	m.erase ( m.begin() + j );
	_normals.erase ( _normals.begin() + j );
      } else ++j;
    }

    if ( edges.size() ) {
      for ( unsigned int j = 0; j < edges.size() - 1; ++j ) {
	for ( unsigned int k = j + 1; k < edges.size(); ++k ) {
	  if ( edges[j] == edges[k] ) ok[j] = ok[k] = false;
	}
      }
    }

    for ( unsigned int j = 0; j < edges.size(); ++j ) {
      if ( ok[j] ) {
	m.push_back ( triangle ( edges[j].at(0), edges[j].at(1), i ) );
	_normals.push_back ( point ( 0, 0, 1 ) );
      }
    }
  }

  for ( unsigned int j = 0; j < m.size(); ) {
    bool valid = true;
    if ( m[j].at(0) == na || m[j].at(1) == na || m[j].at(2) == na ||
	 m[j].at(0) == nb || m[j].at(1) == nb || m[j].at(2) == nb ||
	 m[j].at(0) == nc || m[j].at(1) == nc || m[j].at(2) == nc ) {
      valid = false;
    }
    if ( !valid ) {
      m.erase ( m.begin()+j );
      _normals.erase ( _normals.begin()+j );
    } else ++j;
  }

  b.erase ( b.begin() + na, b.end() );

  _volume = false;

//   cerr<<"triangulate() "<<b.size()<<endl;
//   cerr<<"triangulate() "<<m.size()<<endl;

  // normals...
  for ( unsigned int i = 0; i < m.size(); ++i ) {
    m[i].on ( this );
    _normals[i] = point ( 0, 0, 0 );
    for ( unsigned int j = 0; j < m[i].size(); ++j ) {
      _normals[i] = _normals[i] + cross_product ( m[i][j], m[i][(j+1)%m[i].size()] );
    }
    if ( _normals[i].z() < 0 ) {
      _normals[i] = -_normals[i];
      reverse ( m[i].begin(), m[i].end() );
    }
  }
}

void
glp::surface::triangulate_spherically ( bool close )
{
  mesh & m = *this;
  if ( size() ) {
    mesh::clear();
    _normals.clear();
  }

  base b;
  point center = accumulate ( begin(), end(), point ( 0, 0, 0 ) ) / ( end() - begin() );
  center.z ( zlow() );
  static const double epsilon = 1e-5;
  for ( const_iterator p = begin(); p != end(); ++p ) {
    double x = p->x() - center.x();
    double y = p->y() - center.y();
    double z = p->z() - center.z();
//     double phi = atan2 ( y, x );
//     double theta = atan2 ( z, sqrt ( x*x + y*y ) );
//     double phi = atan2 ( x, z );
//     double theta = atan2 ( y, z );
    double phi = atan2 ( x, sqrt ( y*y + z*z ) );
    double theta = atan2 ( y, sqrt ( x*x + z*z ) );
    //b.push_back ( point ( phi, theta ) );
    if ( close ) {
      double r = sqrt ( x*x + y*y + z*z ) + epsilon;
      b.push_back ( point ( phi /= r, theta /= r ) );
    } else {
      b.push_back ( point ( phi, theta ) );
    }
  }

  if ( close  ) {
    push_back ( center );
    push_back ( center );
    push_back ( center );
    push_back ( center );
  }

  double singularity = M_PI;
  if ( close ) singularity /= epsilon;
  unsigned int na = b.size();
  b.push_back ( point ( -singularity, -singularity ) );
  unsigned int nb = b.size();
  b.push_back ( point ( singularity, -singularity ) );
  unsigned int nc = b.size();
  b.push_back ( point ( singularity, singularity ) );
  unsigned int nd = b.size();
  b.push_back ( point ( -singularity, singularity ) );

  m.push_back ( triangle ( na, nb, nc ) ); // supertriangle
  m.rbegin()->on ( &b );
  _normals.push_back ( point ( 0, 0, 1 ) );
  m.push_back ( triangle ( na, nc, nd ) ); // supertriangle
  m.rbegin()->on ( &b );
  _normals.push_back ( point ( 0, 0, 1 ) );

  using namespace delaunay;

  for ( unsigned int i = 0; i < na; ++i ) {
    vector<bundle> edges;
    vector<bool> ok;
    for ( unsigned int j = 0; j < m.size(); ) {
      if ( incircle ( m[j][0], m[j][1], m[j][2], b[i] ) ) {
	edges.push_back ( edge ( m[j].at(0), m[j].at(1) ) );
	edges.push_back ( edge ( m[j].at(1), m[j].at(2) ) );
	edges.push_back ( edge ( m[j].at(2), m[j].at(0) ) );
	ok.push_back ( true );
	ok.push_back ( true );
	ok.push_back ( true );
	m.erase ( m.begin() + j );
	_normals.erase ( _normals.begin() + j );
      } else ++j;
    }

    if ( edges.size() ) {
      for ( unsigned int j = 0; j < edges.size() - 1; ++j ) {
	for ( unsigned int k = j + 1; k < edges.size(); ++k ) {
 	  if ( edges[j] == edges[k] ) ok[j] = ok[k] = false;
	}
      }
    }

    for ( unsigned int j = 0; j < edges.size(); ++j ) {
      if ( ok[j] ) {
	m.push_back ( triangle ( edges[j].at(0), edges[j].at(1), i ) );
	m.rbegin()->on ( &b );
	_normals.push_back ( point ( 0, 0, 1 ) );
      }
    }
  }

  if ( !close ) {
    for ( unsigned int j = 0; j < m.size(); ) {
      bool valid = true;
      if ( m[j].at(0) == na || m[j].at(1) == na || m[j].at(2) == na ||
	   m[j].at(0) == nb || m[j].at(1) == nb || m[j].at(2) == nb ||
	   m[j].at(0) == nc || m[j].at(1) == nc || m[j].at(2) == nc ||
	   m[j].at(0) == nd || m[j].at(1) == nd || m[j].at(2) == nd ) {
	valid = false;
      }
      if ( !valid ) {
	m.erase ( m.begin()+j );
	_normals.erase ( _normals.begin()+j );
      } else ++j;
    }

    b.erase ( b.begin() + na, b.end() );
  }

  for ( unsigned int j = 0; j < m.size(); ++j ) m[j].on ( this );

  _volume = true;

//   cerr<<"triangulate() "<<b.size()<<endl;
//   cerr<<"triangulate() "<<m.size()<<endl;

  // normals...
  for ( unsigned int i = 0; i < m.size(); ++i ) {
    m[i].on ( &b );
    point n ( 0, 0, 0 );
    for ( unsigned int j = 0; j < m[i].size(); ++j ) {
      n = n + cross_product ( m[i][j], m[i][(j+1)%m[i].size()] );
    }
    m[i].on ( this );
    _normals[i] = point ( 0, 0, 0 );
    for ( unsigned int j = 0; j < m[i].size(); ++j ) {
      _normals[i] = _normals[i] + cross_product ( m[i][j], m[i][(j+1)%m[i].size()] );
    }
    if ( n.z() < 0 ) {
      _normals[i] = -_normals[i];
      reverse ( m[i].begin(), m[i].end() );
    }
  }
}

void
glp::surface::mknormals()
{
  if ( n().size() != size() ) {
    _normals.clear();
    surface & s = *this;
    point center ( 0, 0, 0 );
    if ( s.volume() ) {
      center = accumulate ( s.begin(), s.end(), center ) / ( s.end() - s.begin() );
    }
    point n ( 0, 0, 0 ), c ( 0, 0, 0 );
    for ( unsigned int i = 0; i < s.size(); ++i ) {
      for ( unsigned int j = 0; j < s[i].size(); ++j ) {
	n = n + cross_product ( s[i][j], s[i][(j+1)%s[i].size()] );
	if ( s.volume() ) c = c + s[i][j];
      }
      if ( s.volume() ) c = c / s[i].size();
      if ( s.volume() && scalar_product ( n, c - center ) < 0 ||
	   !s.volume() && n.z() < 0 ) {
	n = -n;
	reverse ( s[i].begin(), s[i].end() );
      }
      _normals.push_back ( n );
    }
  }
}

void
glp::surface::cknormals()
{
  if ( n().size() == size() ) {
    surface & s = *this;
    for ( unsigned int i = 0; i < s.size(); ++i ) {
      _normals[i] = point ( 0, 0, 0 );
      for ( unsigned int j = 0; j < s[i].size(); ++j ) {
	_normals[i] = _normals[i] + cross_product ( s[i][j], s[i][(j+1)%s[i].size()] );
      }
    }
  }
}

const glp::bundle
glp::surface::edge ( size_type a, size_type b )
{
  bundle node;
  node.on ( this );
  node.insert ( a );
  node.insert ( b );
  return node;
}

const glp::bundle
glp::surface::triangle ( size_type a, size_type b, size_type c )
{
  bundle node;
  node.on ( this );
  node.insert ( a );
  node.insert ( b );
  node.insert ( c );
  return node;
}

const glp::bundle
glp::surface::square ( size_type a, size_type b, size_type c, size_type d )
{
  bundle node;
  node.on ( this );
  node.insert ( a );
  node.insert ( b );
  node.insert ( c );
  node.insert ( d );
  return node;
}

const glp::bundle
glp::surface::convex_hull()
{
//   delaunay::sort ( &*begin(), &*begin() + ( end() - begin() ) );
//   const base & s = *this;
//   const_iterator p = s.begin();
//   while ( p != s.end() && !delaunay::inside ( &*s.begin(), &*p, *p ) ) ++p;
//   unsigned int n = p - s.begin();
//   bundle node;
//   node.on ( this );
//   while ( n ) node.insert ( --n );
//   return node;
  bundle node;
  node.on ( this );
  base hull = *this;
  delaunay::sort ( &*hull.begin(), &*hull.begin() + ( hull.end() - hull.begin() ) );
  for ( const_iterator p = hull.begin();
	!delaunay::inside ( &*hull.begin(), &*p, *p ) && p != hull.end();
	node.insert ( find ( begin(), end(), *p ) - begin() ) ) ++p;
  return node;
}

bool
glp::operator< ( const surface & lhs, const surface & rhs )
{
  return lhs.alpha() < rhs.alpha();
}

/***************************************************************************
 *
 * functions used for triangulation
 *
 ***************************************************************************/

/** is the point pd inside the circle passing through pa, pb, and pc? */
bool
glp::delaunay::incircle ( const point & pa, const point & pb, const point & pc,
			  const point & pd )
{
  double adx = pa.x() - pd.x();
  double ady = pa.y() - pd.y();
  double bdx = pb.x() - pd.x();
  double bdy = pb.y() - pd.y();
  double cdx = pc.x() - pd.x();
  double cdy = pc.y() - pd.y();

  double abdet = adx * bdy - bdx * ady;
  double bcdet = bdx * cdy - cdx * bdy;
  double cadet = cdx * ady - adx * cdy;

  double det = abdet + bcdet + cadet;

  abdet *= cdx * cdx + cdy * cdy;
  bcdet *= adx * adx + ady * ady;
  cadet *= bdx * bdx + bdy * bdy;

  det *= abdet + bcdet + cadet;

  return det > 0;
}

/** is the point p inside the polygon [first:last)? */
bool
glp::delaunay::inside ( const point * first, const point * last,
			const point & p )
{
  unsigned int count = 0; // count of ray/edge intersections
  for ( const point * i = first; i != last; ++i ) {
    const point * j = i + 1;
    if ( j == last ) j = first;
    if ( i->y() == j->y() ) continue;
    if ( i->y() > p.y() && j->y() > p.y() ) continue;
    if ( i->y() < p.y() && j->y() < p.y() ) continue;
    if ( max ( i->y(), j->y() ) == p.y() ) ++count;
    else if ( min ( i->y(), j->y() ) == p.y() ) continue;
    else {
      double t = ( p.y() - i->y() ) / ( j->y() - i->y() );
      if ( i->x() + t * ( j->x() - i->x() ) >= p.x() ) ++count;
    }
  }
  return count & 1;
}

/** arrange points by gift wrap method */
void
glp::delaunay::sort ( point * first, point * last )
{
  // find leftmost point
  point * found = first;
  for ( point * i = first + 1; i != last; ++i )
    if ( i->x() < found->x() ) found = i;

  // arrange points by gift wrap method
  for ( ; first != last; ++first ) {
    swap ( *found, *first );
    found = first + 1;
    if ( found == last ) break;
    for ( point * j = first + 2; j != last; ++j ) {
      double x1 = found->x() - first->x();
      double y1 = found->y() - first->y();
      double x2 = j->x() - first->x();
      double y2 = j->y() - first->y();
      double vp = x1 * y2 - x2 * y1;
      if ( vp > 0 ) found = j;
      else if ( vp == 0 ) {
	double sp = x1 * x2 + y1 * y2;
	if ( sp > 0 ) {
	  double l1 = x1 * x1 + y1 * y1;
	  double l2 = x2 * x2 + y2 * y2;
	  if ( l1 > l2 ) found = j;
	}
      }
    }
  }
}

/** distance between the point pd ant the plane passing through pa, pb, and pc */
const double
glp::distance ( const point & pa, const point & pb, const point & pc,
		const point & pd )
{
  const double &x1 = pa.x(), &x2 = pb.x(), &x3 = pc.x(), &x = pd.x();
  const double &y1 = pa.y(), &y2 = pb.y(), &y3 = pc.y(), &y = pd.y();
  const double &z1 = pa.z(), &z2 = pb.z(), &z3 = pc.z(), &z = pd.z();

  double A = y1 * (z2 - z3) + y2 * (z3 - z1) + y3 * (z1 - z2);
  double B = z1 * (x2 - x3) + z2 * (x3 - x1) + z3 * (x1 - x2);
  double C = x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2);
  double D = -x1 * (y2 * z3 - y3 * z2) -x2 * (y3 * z1 - y1 * z3) -x3 * (y1 * z2 - y2 * z1);

  return ( A * x + B * y + C * z + D ) / sqrt ( A*A + B*B + C*C );
}
