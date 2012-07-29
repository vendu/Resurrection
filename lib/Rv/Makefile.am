AUTOMAKE_OPTIONS = foreign subdir-objects

AM_CFLAGS = @R_CFLAGS@ @R_INCLUDES@

lib_LTLIBRARIES = libRv.la

libRv_la_SOURCES = \
	global.c \
	binding.c \
	init.c \
	load.c \
	file.c

#libRv_la_LIBADD = ../rc/librc.la ../Resurrection/libResurrection.la
libRv_la_LIBADD = ../Resurrection/libResurrection.la

