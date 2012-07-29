AUTOMAKE_OPTIONS = foreign subdir-objects

AM_CFLAGS = @R_CFLAGS@ @R_INCLUDES@

lib_LTLIBRARIES = libRwm.la

libRwm_la_SOURCES = \
	test.c \
	global.c \
	init.c \
	root.c \
	event.c \
	binding.c \
	desktop.c \
	frame.c \
	client.c \
	override.c \
	menu.c \
	pager.c \
	clock.c \
	util.c

libRwm_la_LIBADD = ../Resurrection/libResurrection.la

