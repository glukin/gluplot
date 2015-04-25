/* -*- C++ -*- *************************************************************
 *
 *   glp_bundle.h, Thu Feb 10 16:48:00 MSK 2005
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

#ifndef GLP_BUNDLE_H
#define GLP_BUNDLE_H

#include "glp_point.h"

#include <vector>
using namespace std;

namespace glp {

  class bundle
    : protected vector<size_t> {
  private:
    typedef class vector<size_t> base;
    typedef class vector<point> root;
  public: /** interface */
    typedef base::iterator iterator;
    typedef base::const_iterator const_iterator;
    typedef base::reverse_iterator reverse_iterator;
    typedef base::const_reverse_iterator const_reverse_iterator;
    typedef root::reference reference;
    typedef root::const_reference const_reference;
  public: /** c-tor(s) & d-tor */
    bundle();
    ~bundle();
  public: /** interface */
    /** should be done first! */
    void on ( root * origin );
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
    size_t size() const { return base::size(); }
    bool empty() const { return base::empty(); }
    /** references */
    reference operator[] ( size_t i )
    {
      return (*_root) [ base::operator[] ( i ) ];
    }
    const_reference operator[] ( size_t i ) const
    {
      return (*_root) [ base::operator[] ( i ) ];
    }
    size_t at ( size_t i ) const { return base::operator[] ( i ); }
    /** accumulation */
    void insert ( size_t i );
    /** cleanup */
    void clear();
  protected: /** storages */
    root * _root;
  protected: /** setup */
    void arrange();
  };

  /** outer interface */
  bool operator== ( const bundle & lhs, const bundle & rhs );
  bool operator!= ( const bundle & lhs, const bundle & rhs );

} /** namespace glp */

#endif // GLP_BUNDLE_H

