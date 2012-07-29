AUTOMAKE_OPTIONS = foreign subdir-objects

AM_CFLAGS = @R_CFLAGS@ @R_INCLUDES@

AM_LDFLAGS = -static -L/usr/local/lib -L/usr/local/X11R6/lib -L/usr/X11R6/lib

bin_PROGRAMS = Rv

Rv_SOURCES = main.c

Rv_LDADD = ../../lib/Resurrection/libResurrection.la ../../lib/Rv/libRv.la -lX11 -lImlib2

