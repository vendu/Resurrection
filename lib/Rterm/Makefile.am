AUTOMAKE_OPTIONS = foreign subdir-objects

AM_CFLAGS = @R_CFLAGS@ @R_INCLUDES@

lib_LTLIBRARIES = libRterm.la

libRterm_la_SOURCES = \
	char.c \
	csi.c \
	draw.c \
	escape.c \
	eterm.c \
	eventhandler.c \
	exit.c \
	font.c \
	freetype.c \
	gc.c \
	i18n.c \
	menu.c \
	option.c \
	pixmap.c \
	print.c \
	privatemode.c \
	pty.c \
	screen.c \
	scrollbar.c \
	selection.c \
	sgr.c \
	signalhandler.c \
	term.c \
	user.c \
	userlog.c \
	util.c \
	wait.c \
	window.c \
	xterm.c

libRterm_la_LIBADD = ../Resurrection/libResurrection.la

