
SUBDIRS = html 

EXTRA_DIST = gluplot.1.gz man.tex 

install-data-local:
	$(mkinstalldirs) $(mandir)/man1/
	$(INSTALL_DATA) $(srcdir)/gluplot.1.gz $(mandir)/man1/gluplot.1.gz

uninstall-local:
	-rm -f $(mandir)/man1/gluplot.1.gz

