EXTRA_DIST = \
gluplot.ico \
gluplot.desktop \
gluplot.xpm gluplot.xbm \
gluplot.rc gluplotrc.h

if HAVE_FLTK

if MUSTDIE

all: gluplotres.o

gluplotres.o : gluplot.rc
	windres $^ $@

endif

endif
