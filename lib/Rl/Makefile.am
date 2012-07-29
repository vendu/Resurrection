AUTOMAKE_OPTIONS = foreign subdir-objects

AM_CFLAGS = @R_CFLAGS@ @R_INCLUDES@

lib_LTLIBRARIES = libRl.la

libRl_la_SOURCES = \
	init.c

#libRl_la_LIBADD = ../rc/librc.la ../Resurrection/libResurrection.la
libRl_la_LIBADD = ../Rterm/libRterm.la ../Resurrection/libResurrection.la

