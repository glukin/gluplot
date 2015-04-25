lib_LIBRARIES = libgluplot.a

libgluplot_a_SOURCES = \
glp_point.h 	glp_point.cpp 		\
glp_curve.h 	glp_curve.cpp 		\
glp_bundle.h 	glp_bundle.cpp 		\
glp_surface.h 	glp_surface.cpp 	\
glp_axis.h 	glp_axis.cpp 		\
glp_color.h 	glp_color.cpp 		\
glp_label.h 	glp_label.cpp 		\
glp_plot.h 	glp_plot.cpp 	glp_plot_gl.cpp		\
glp_window.h 	glp_window.cpp  	\
glp_glut_window.h 	glp_glut_window.cpp  	\
glp_option.h 	glp_option.cpp 		\
glp_style.h 		\
gluplot.h 

if HAVE_FLTK

libgluplot_a_SOURCES += \
glp_fltk_window.h 	glp_fltk_window.cpp 

endif

include_HEADERS = $(filter %.h,$(libgluplot_a_SOURCES))

INCLUDES = -I$(top_srcdir)/src/gl2ps

