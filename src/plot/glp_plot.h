/* -*- C++ -*- *************************************************************
 *
 *   glp_plot.h, Fri Oct  1 11:35:17 MSD 2004
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

#ifndef GLP_PLOT_H
#define GLP_PLOT_H

#include "glp_axis.h"
#include "glp_curve.h"
#include "glp_surface.h"
#include "glp_color.h"
#include "glp_label.h"

#include <vector>
#include <string>
using namespace std;

namespace glp {

  class plot_traits {
  public: /** c-tor(s) & d-tor */
    plot_traits();
    virtual ~plot_traits();
  public: /** interface */
    /** plot style */
    enum PLOT_STYLE style() const { return _style; }
    void style ( enum PLOT_STYLE with ) { _style = with; }
    /** current color */
    const rgb & tone() const { return _color; }
    void tone ( const rgb & c ) { _color = c; }
    /** own colours */
    const color & background() const { return _background; }
    void background ( const color & c ) { _background = c; }
    const rgb & foreground() const { return _foreground; }
    void foreground ( const rgb & c ) { _foreground = c; }
    /** color palette */
    void rgbscheme ( enum COLOR_SCHEME scheme ) 
    {
      _palette.rgbscheme ( scheme );
    }
    const double & gamma() const { return _palette.gamma(); }
    void gamma ( const double & val ) { _palette.gamma ( val ); }
    void grayscale ( bool yn ) { _palette.grayscale ( yn ); }
    bool grayscale() const { return _palette.grayscale(); }
    void negative ( bool yn ) { _palette.negative ( yn ); }
    bool negative() const { return _palette.negative(); }
    /** sizes */
    const double & line_width() const { return _line_width; }
    void line_width ( const double & val ) { _line_width = val; }
    const double & point_size() const { return _point_size; }
    void point_size ( const double & val ) { _point_size = val; }
    const double & frame_line_width() const { return _frame_line_width; }
    void frame_line_width ( const double & val ) { _frame_line_width = val; }
    const double & grid_line_width() const { return _grid_line_width; }
    void grid_line_width ( const double & val ) { _grid_line_width = val; }
    /** font */
    enum TEXT_FONT font() const { return _font; }
    void font ( enum TEXT_FONT fnt ) { _font = fnt; }
    /** decoration */
    void ticks_visible ( bool yn ) { _ticks_visible = yn; }
    bool ticks_visible() const { return _ticks_visible; }
    void grid_visible ( bool yn ) { _grid_visible = yn; }
    bool grid_visible() const { return _grid_visible; }
    void key_visible ( bool yn ) { _key_visible = yn; }
    bool key_visible() const { return _key_visible; }
    void style_visible ( bool yn ) { _style_visible = yn; }
    bool style_visible() const { return _style_visible; }
  protected: /** storages */
    enum PLOT_STYLE _style;
    static const unsigned int _easel_size;
    static const rgb _easel[];
    unsigned int _color_index;
    rgb _color;
    color _background;
    rgb _foreground;
    palette _palette;
    double _line_width;
    double _point_size;
    double _frame_line_width;
    double _grid_line_width;
    enum TEXT_FONT _font;
    bool _ticks_visible;
    bool _grid_visible;
    bool _key_visible;
    bool _style_visible;
  protected: /** setup */
    void chtone();  /** rotate easel */
    void flush();   /** clear visibility flags */
  };

  class plot 
    : public plot_traits {
  private:
    typedef class vector<curve> base2d;
    typedef class vector<surface> base3d;
    typedef class vector<label> base1;
  public:
    typedef base2d::size_type size_type;
  public: /** c-tor(s) & d-tor */
    plot();
    virtual ~plot();
  public: /** interface */
    /** size/empty? */
    size_type size2() const { return _lines.size(); }
    size_type size3() const { return _planes.size(); }
    bool empty() const { return _lines.empty() && _planes.empty(); }
    /** references */
    curve & at2 ( size_type i ) { return _lines[i]; }
    const curve & at2 ( size_type i ) const { return _lines[i]; }
    surface & at3 ( size_type i ) { return _planes[i]; }
    const surface & at3 ( size_type i ) const { return _planes[i]; }
    /** accumulation */
    void push_back ( const curve & c );
    void push_back ( const surface & s );
    void push_back ( const label & l );
    /** cleanup */
    void clear();
    /** axial ranges */
    void xrange ( const double & low, const double & high );
    const double & xlow() const { return _x_axis.low(); }
    const double & xhigh() const { return _x_axis.high(); }
    void yrange ( const double & low, const double & high );
    const double & ylow() const { return _y_axis.low(); }
    const double & yhigh() const { return _y_axis.high(); }
    void zrange ( const double & low, const double & high );
    const double & zlow() const { return _z_axis.low(); }
    const double & zhigh() const { return _z_axis.high(); }
    void fit_to ( const curve & c );
    void fit_to ( const surface & s );
    void fit_to ( const label & l );
    bool visible ( const point & p ) const;
    /** axial distortions */
    void xdistortion ( axis::lens func );
    bool xdistortion() const { return _x_axis.distortion(); }
    void ydistortion ( axis::lens func );
    bool ydistortion() const { return _y_axis.distortion(); }
    void zdistortion ( axis::lens func );
    bool zdistortion() const { return _z_axis.distortion(); }
    /** logarithmic axial distortions embeded */
    void xlog ( bool yn );
    bool xlog() const { return _xlog; }
    void ylog ( bool yn );
    bool ylog() const { return _ylog; }
    void zlog ( bool yn );
    bool zlog() const { return _zlog; }
    /** axial offsets */
    void xoffsets ( const double & loff, const double & hoff );
    const double & xloff() const { return _x_axis.loff(); }
    const double & xhoff() const { return _x_axis.hoff(); }
    void yoffsets ( const double & loff, const double & hoff );
    const double & yloff() const { return _y_axis.loff(); }
    const double & yhoff() const { return _y_axis.hoff(); }
    void zoffsets ( const double & loff, const double & hoff );
    const double & zloff() const { return _z_axis.loff(); }
    const double & zhoff() const { return _z_axis.hoff(); }
    /** visible axial ranges */
    const double & xlowend() const { return _x_axis.lowend(); }
    const double & xhighend() const { return _x_axis.highend(); }
    const double & ylowend() const { return _y_axis.lowend(); }
    const double & yhighend() const { return _y_axis.highend(); }
    const double & zlowend() const { return _z_axis.lowend(); }
    const double & zhighend() const { return _z_axis.highend(); }
    /** axial ticks */
    void xticks ( const unsigned int tics ) { _x_axis.ticks ( tics ); }
    void yticks ( const unsigned int tics ) { _y_axis.ticks ( tics ); }
    void zticks ( const unsigned int tics ) { _z_axis.ticks ( tics ); }
    /** size (rules axial ticks!) */
    void size ( const int width, const int height );
    /** axial rotations */
    void xrotation ( const double & angle ) { _xrotation = angle; }
    const double & xrotation() const { return _xrotation; }
    void yrotation ( const double & angle ) { _yrotation = angle; }
    const double & yrotation() const { return _yrotation; }
    void zrotation ( const double & angle ) { _zrotation = angle; }
    const double & zrotation() const { return _zrotation; }
    /** drawing */
    virtual void init() const;
    virtual void draw() const;
    virtual void save ( const string & filename ) const;
    /** clipping */
    void clipping ( bool yn ) const;
    bool clipping() const { return _clipping; }
    void clip ( const point & dir, const double & pos ) const;
    void clip() const;
  protected: /** storages */
    axis _x_axis, _y_axis, _z_axis;
    base2d _lines;
    base3d _planes;
    base1 _marks;
    int _width, _height;
    /** logarithmic stuff */
    bool _xlog, _ylog, _zlog;
    double _xmin, _ymin, _zmin;
    double _xmin_positive, _ymin_positive, _zmin_positive;
    double _xrotation, _yrotation, _zrotation;
    /** drawing stuff */
    mutable bool _valid;
    /** clipping stuff */
    mutable bool _clipping;
    mutable vector<vector<double> > _clip_planes;
  protected: /** drawing stuff */
    virtual void draw_with_dots ( const curve & c ) const;
    virtual void draw_with_lines ( const curve & c ) const;
    virtual void draw_with_points ( const curve & c ) const;
    virtual void draw_with_xerrorbars ( const curve & c ) const;
    virtual void draw_with_yerrorbars ( const curve & c ) const;
    virtual void draw_with_impulses ( const curve & c ) const;
    virtual void draw_with_dots ( const surface & s ) const;
    virtual void draw_with_lines ( const surface & s ) const;
    virtual void draw_with_points ( const surface & s ) const;
    virtual void draw_with_errorbars ( const surface & s ) const;
    virtual void draw_with_lighting ( const surface & s ) const;
    virtual void draw ( const surface & s ) const;
  };

  /** outer interface */
  plot & operator<< ( plot & pout, enum PLOT_STYLE with );
  plot & operator<< ( plot & pout, const curve & c );
  plot & operator<< ( plot & pout, const surface & s );
  plot & operator<< ( plot & pout, enum TEXT_FONT with );
  plot & operator<< ( plot & pout, const label & l );

  /** just useful function(s) */
  const double find_x ( const point * first, const point * last,
			const double & threshold );
  const double find_y ( const point * first, const point * last,
			const double & threshold );
  const double find_z ( const point * first, const point * last,
			const double & threshold );

} /** namespace glp */

#endif // GLP_PLOT_H

