AUTOMAKE_OPTIONS = foreign subdir-objects

AM_CFLAGS = @R_CFLAGS@ @R_INCLUDES@

AM_LDFLAGS = -static -L/usr/local/lib -L/usr/local/X11R6/lib -L/usr/X11R6/lib

bin_PROGRAMS = Rwm

Rwm_SOURCES = main.c

Rwm_LDADD = ../../lib/Resurrection/libResurrection.la ../../lib/Rwm/libRwm.la -lX11 -lImlib2

