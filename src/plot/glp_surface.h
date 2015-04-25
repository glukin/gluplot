/* -*- C++ -*- *************************************************************
 *
 *   glp_surface.h, Tue Jan 18 14:17:16 MSK 2005
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

#ifndef GLP_SURFACE_H
#define GLP_SURFACE_H

#include "glp_point.h"
#include "glp_bundle.h"
#include "glp_color.h"
#include "glp_style.h"

#include <vector>
using namespace std;

namespace glp {

  class surface_traits {
  public: /** c-tor(s) & d-tor */
    surface_traits();
    virtual ~surface_traits();
  public: /** interface */
    /** plot style */
    void style ( enum PLOT_STYLE with ) { _style = with; }
    enum PLOT_STYLE style() const { return _style; }
    /** color */
    void tone ( const rgb & c ) { _color = c; }
    const rgb & tone() const { return _color; }
    /** transparency */
    void alpha ( const double & a ) { _alpha = a; }
    const double & alpha() const { return _alpha; }
  protected: /** storages */
    enum PLOT_STYLE _style;
    rgb _color;
    double _alpha;
  };

  class surface
    : public surface_traits,
      protected vector<point>,
      protected vector<bundle> {
  private:
    typedef class vector<point> base;
    typedef class vector<bundle> mesh;
  public:
    typedef base::iterator iterator;
    typedef base::const_iterator const_iterator;
    typedef base::reverse_iterator reverse_iterator;
    typedef base::const_reverse_iterator const_reverse_iterator;
    typedef mesh::reference reference;
    typedef mesh::const_reference const_reference;
    typedef mesh::size_type size_type;
  public: /** c-tor(s) & d-tor */
    surface();
    surface ( const surface & rhs );
    surface & operator= ( const surface & rhs );
    virtual ~surface();
  public: /** interface */
    /** iterators */
    iterator begin() { return base::begin(); }
    const_iterator begin() const { return base::begin(); }
    iterator end() { return base::end(); }
    const_iterator end() const { return base::end(); }
    reverse_iterator rbegin() { return base::rbegin(); }
    const_reverse_iterator rbegin() const { return base::rbegin(); }
    reverse_iterator rend() { return base::rend(); }
    const_reverse_iterator rend() const { return base::rend(); }
    /** size/empty? */
    size_type size() const { return mesh::size(); }
    bool empty() const { return base::empty(); }
    /** references */
    reference operator[] ( size_type i )
    {
      return mesh::operator[] ( i );
    }
    const_reference operator[] ( size_type i ) const
    {
      return mesh::operator[] ( i );
    }
    /** accumulation */
    void push_back ( const point & p );
    iterator insert ( iterator position, const point & p );
    void fit_to ( const point & p );
    /** cleanup */
    iterator erase ( iterator first, iterator last );
    void clear();
    /** meshing */
    void push_back ( const bundle & b );
    void mkmesh();
    /** triangulation */
    void triangulate();
    void triangulate_spherically ( bool close = false );
    /** frame */
    const double & xlow() const { return _xlow; }
    const double & xhigh() const { return _xhigh; }
    const double & ylow() const { return _ylow; }
    const double & yhigh() const { return _yhigh; }
    const double & zlow() const { return _zlow; }
    const double & zhigh() const { return _zhigh; }
    /** has volume? */
    bool volume() const { return _volume; }
    /** normals */
    const base & n() const { return _normals; }
    void mknormals();
    void cknormals();
  protected: /** storages */
    double _xlow, _xhigh;
    double _ylow, _yhigh;
    double _zlow, _zhigh;
    bool _volume;
    base _normals;
  protected: /** setup */
    const bundle edge ( size_type a, size_type b );
    const bundle triangle ( size_type a, size_type b, size_type c );
    const bundle square ( size_type a, size_type b, size_type c, size_type d );
    const bundle convex_hull();
  };

  /** outer interface */
  bool operator< ( const surface & lhs, const surface & rhs );

} /** namespace glp */

#endif // GLP_SURFACE_H

