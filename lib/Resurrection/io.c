/*
 * io.c - I/O routines for Resurrection.
 * Copyright (C) 2004 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#include <Resurrection/Resurrection.h>

ssize_t
Rread(int fd, void *buffer, size_t count)
{
    ssize_t len, nbread;

    if (fd < 0
	|| buffer == NULL
	|| count <= 0) {

	return -1;
    }

    len = 0;
    while (count) {
	errno = 0;
	nbread = read(fd, buffer, count);
	if (nbread <= 0) {
	    if (errno == EINTR) {

		continue;
	    } else if (len == 0) {

		return -1;
	    }

	    return len;
	}

	count -= nbread;
	len += nbread;
	buffer += nbread;
    }

    return len;
}

ssize_t
Rreadnb(int fd, void *buffer, size_t count)
{
    ssize_t len, nbread;

    if (fd < 0
	|| buffer == NULL
	|| count <= 0) {

	return -1;
    }

    len = 0;
    while (count) {
	errno = 0;
	nbread = Rnbread(fd, buffer, count);
	if (nbread <= 0) {
	    if (errno == EINTR) {

		continue;
	    } else if (errno == EAGAIN || errno == EWOULDBLOCK) {

		return len;
	    }

	    return -1;
        }

	count -= nbread;
	len += nbread;
	buffer += nbread;
    }

    return len;
}

ssize_t
Rwrite(int fd, void *buffer, size_t count)
{
    ssize_t len, nbwritten;

    if (fd <= 0
	|| buffer == NULL
	|| count <= 0) {

	return -1;
    }

    len = 0;
    while (count) {
	errno = 0;

	nbwritten = write(fd, buffer, count);
	if (nbwritten <= 0) {
	    if (errno == EINTR) {

		continue;
	    }

	    return len;
	}

	buffer += nbwritten;
	count -= nbwritten;
	len += nbwritten;
    }

    return len;
}

ssize_t
Rwritenb(int fd, void *buffer, size_t count)
{
    ssize_t len, nbwritten;

    if (fd <= 0
	|| buffer == NULL
	|| count <= 0) {

	return -1;
    }

    len = 0;
    while (count) {
	errno = 0;

	nbwritten = Rnbwrite(fd, buffer, count);
	if (nbwritten <= 0) {
	    if (errno == EINTR) {

		continue;
	    } else if (errno == EAGAIN || errno == EWOULDBLOCK) {

		return len;
	    }

	    return -1;
        }

	buffer += nbwritten;
	count -= nbwritten;
	len += nbwritten;
    }

    return len;
}

