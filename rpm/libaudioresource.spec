Name: libaudioresource
Version: 1.0.0
Release: 1
Summary: Nemo Mobile Audio Resource API
Group: Development/Libraries
License: LGPL
URL: http://github.com/nemomobile
Source: %{name}-%{version}.tar.bz2
BuildRequires: pkgconfig(libresource-glib)
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig

%description
This library provides a way to acquire audio resources for playback on Nemo
Mobile and Sailfish, as well as a way to get notified when audio resources
have been released, in which case audio playback must be stopped.

%package devel
Summary: Development library for %{name}
Requires: %{name} = %{version}

%description devel
This package contains the development library for %{name}.

%prep
%setup -q

%build
%cmake
make

%install
%make_install

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%defattr(-,root,root,-)
%{_libdir}/%{name}.so.*

%files devel
%defattr(-,root,root,-)
%doc README COPYING.LGPL
%{_libdir}/%{name}.so
%{_libdir}/pkgconfig/audioresource.pc
%{_includedir}/audioresource/audioresource.h
