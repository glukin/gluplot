# This spec file was generated using Kpp
# If you find any problems with this spec file please report
# the error to ian geiser <geiseri@msoe.edu>
Summary:   The Gluplot Plotting Package
Name:      gluplot
Version:   1.9
Release:   20110412
License:   GPL
Vendor:    Alexei Soloviev <solovjev@cv.jinr.ru>
Url:       http://www.jinr.ru/programs/jinrlib/gluplot/index.html

Packager:  Alexei Soloviev <solovjev@cv.jinr.ru>
Group:     Applications
Source:    %{name}-%{version}.tar.gz
BuildRoot: /var/tmp/%{name}-buildroot
Prefix:    /usr/local

%description
Gluplot is a data plotting package. It was originally intended 
both as graphical library and as standalone program which would 
allow scientists and students to visualize data. 
(The "GLU" in gluplot means that it is OpenGL/GLU/glut based.) 
Gluplot handles both curves (2D) and surfaces (3D). For 2D plots, 
there are many plot styles, including lines, points, lines with 
points, error bars and impulses. Surfaces are plotted as a color 
map on the x-y plane. 
Gluplot supplies both linear and logarithmic axes. Moreover, 
any other axis distortion is available using gluplot library. 
Axis scale and also many other settings are changeable by gluplot 
command-line options or by menu items and hot-keys after it runs. 
After all, gluplot supports high quality plot output to PostScript (PS), 
Encapsulated PostScript (EPS) and Portable Document Format (PDF) files.

%package devel
Summary: Gluplot - development environment
Group: Development/Libraries

%description devel
Install gluplot-devel if you need to develop gluplot applications. 
You'll need to install the gluplot package if you plan to run
dynamically linked applications.

%prep
%setup
CFLAGS="$RPM_OPT_FLAGS" CXXFLAGS="$RPM_OPT_FLAGS" LDFLAGS="$RPM_OPT_FLAGS" \
./configure --enable-fltk \
--build=i386-linux --host=i386-linux --target=i386-linux \
--prefix=%{prefix} \
$LOCALFLAGS

%build
make

%install
# these lines just make sure the directory structure in the
# RPM_BUILD_ROOT exists
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT

make -e prefix=$RPM_BUILD_ROOT/%{prefix} install

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
%dir %{prefix}/bin
%{prefix}/bin/%{name}
%dir %{prefix}/share/man/man1
%{prefix}/share/man/man1/%{name}.1.gz
%dir %{prefix}/share/doc/%{name}
%{prefix}/share/doc/%{name}/*

%files devel
%defattr(-,root,root)
%dir %{prefix}/include
%{prefix}/include/*
%dir %{prefix}/lib
%{prefix}/lib/lib%{name}*.a
%{prefix}/lib/libgl2ps.a
