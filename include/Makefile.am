AUTOMAKE_OPTIONS = foreign

pkginclude_HEADERS = \
	Resurrection/autoconf.h \
	Resurrection/Resurrection.h \
	Resurrection/binding.h \
	Resurrection/cc.h \
	Resurrection/debug.h \
	Resurrection/global.h \
	Resurrection/image.h \
	Resurrection/macros.h \
	Resurrection/paths.h \
	Resurrection/port.h \
	Resurrection/prof.h \
	Resurrection/protos.h \
	Resurrection/select.h \
	Resurrection/setup.h \
	Resurrection/struct.h \
	Resurrection/system.h \
	Resurrection/typedef.h \
	Resurrection/term.h \
	Resurrection/wm.h \
	Resurrection/launch.h

@resurrection_subst_dirs@

Resurrection/paths.h:
	rm -f $(srcdir)/Resurrection/paths.h
	$(substdirs) $(srcdir)/Resurrection/paths.h.in > $(srcdir)/Resurrection/paths.h

