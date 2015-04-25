bin_PROGRAMS = gluplot

gluplot_SOURCES = \
getopt.h 	getopt.c 	\
usage.h 	usage.cpp 	\
main.cpp 

INCLUDES = -I$(top_srcdir)/src/plot
LDADD = \
$(top_srcdir)/src/plot/libgluplot.a \
$(top_srcdir)/src/gl2ps/libgl2ps.a

if HAVE_FLTK

gluplot_SOURCES += \
gui.h 	gui.cpp 	gui.fl

INCLUDES += -DHAVE_FLTK $(fltk_includes)
LDADD += $(fltk_libraries)

INCLUDES += -DDOCDIR=\"$(docdir)\"

INCLUDES += -I$(top_srcdir)/src/icon 

if HAVE_XPM
INCLUDES += -DHAVE_XPM
LDADD += -lXpm
endif

if MUSTDIE
LDADD += $(top_srcdir)/src/icon/gluplotres.o
endif

endif

SUBDIRS = plot gl2ps test icon
