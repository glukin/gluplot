noinst_PROGRAMS = \
data_file rainbow \
surface sphere \
terrain \
parzen_1D parzen_2D 

data_file_SOURCES = data_file.cpp
rainbow_SOURCES = rainbow.cpp
surface_SOURCES = surface.cpp
sphere_SOURCES = sphere.cpp
terrain_SOURCES = terrain.cpp ppm.c ppm.h
parzen_1D_SOURCES = parzen_1D.cpp
parzen_2D_SOURCES = parzen_2D.cpp

INCLUDES = -I$(top_srcdir)/src/plot
LDADD = \
$(top_srcdir)/src/plot/libgluplot.a \
$(top_srcdir)/src/gl2ps/libgl2ps.a

EXTRA_DIST = \
test.dat test.fit \
dubna_sh.ppm 
