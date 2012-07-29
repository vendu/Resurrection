/*
 * pty.c - pseudo-terminal management for Resurrection.
 * Copyright (C) 2003 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#include <Resurrection/Resurrection.h>

#define PTY_NAME_LENGTH (PATH_MAX + 1)

#define DEBUG_PTY 0

#if (USE_OPENRPTY)

int
R_open_pty_master_openrpty(char **masterpath, char **slavepath)
{
    int masterfd;

    if ((*masterpath = calloc(1, PTY_NAME_LENGTH)) == NULL) {

	return -1;
    }

    if ((*slavepath = calloc(1, PTY_NAME_LENGTH)) == NULL) {
	free(*masterpath);

	return -1;
    }

    /* FIXME: use OPTY_LOGIN only if user logging is enabled, OPTY_NOP
     * otherwise. Also, change "localhost" to something else. */

    if ((masterfd = openrpty(*slavepath, *masterpath, OPTY_LOGIN, getuid(),
			     "localhost")) < 0) {
	free(*masterpath);
	free(*slavepath);

	return -1;
    }

    return masterfd;
}

#elif (USE_USG_PTYS)

int
R_open_pty_master_usg(char **masterpath, char **slavepath)
{
    int masterfd;
    size_t len;
    char *slavename;

#if (USE_POSIX_OPENPT)

    if ((masterfd = posix_openpt(O_RDWR)) < 0) {

	return -1;
    }

#elif (USE_GETPT)

    if ((masterfd = getpt()) < 0) {

	return -1;
    }

#else /* !(USE_GETPT) */

    if ((masterfd = open(DEV_PTMX_DEVICE, O_RDWR)) < 0) {

	return -1;
    }

#endif

    if (grantpt(masterfd) != 0) {
        fprintf(stderr, "grantpt() failed\n");
	close(masterfd);

	return -1;
    }

    if (unlockpt(masterfd) != 0) {
        fprintf(stderr, "unlockpt() failed\n");
	close(masterfd);

	return -1;
    }

    if ((slavename = ptsname(masterfd)) == NULL) {
        fprintf(stderr, "ptsname() failed\n");
	close(masterfd);

	return -1;
    }

    len = strlen(slavename);

    if (masterpath) {
	if ((*masterpath = calloc(1, len + 1)) == NULL) {
	    close(masterfd);

	    return -1;
	}

	strncpy(*masterpath, slavename, len);
    }
    
    if (slavepath) {
	if ((*slavepath = calloc(1, len + 1)) == NULL) {
	    free(*masterpath);

	    close(masterfd);
	    
	    return -1;
	}

	strncpy(*slavepath, slavename, len);
    }

    return masterfd;
}

#elif (USE_OPENPTY)

int
R_open_pty_master_openpty(char **masterpath, char **slavepath)
{
    int masterfd, slavefd;
    size_t len;
    char slavename[PTY_NAME_LENGTH];

    if (openpty(&masterfd, &slavefd, slavename, NULL, NULL) < 0) {

	return -1;
    }

    close(slavefd);

    len = strlen(slavename);

    if (masterpath) {
	if ((*masterpath = calloc(1, len + 1)) == NULL) {
	    close(masterfd);

	    return -1;
	}

	strncpy(*masterpath, slavename, len);
    }
    
    if (slavepath) {
	if ((*slavepath = calloc(1, len + 1)) == NULL) {
	    free(*masterpath);

	    close(masterfd);

	    return -1;
	}

	strncpy(*slavepath, slavename, len);
    }

    return masterfd;
}

#elif (USE_AIXV3_PTYS)

int
R_open_pty_master_aixv3(char **masterpath, char **slavepath)
{
    int masterfd;
    size_t len;
    char *slavename;

    if ((masterfd = open(DEV_PTC_DEVICE, O_RDWR)) < 0) {

	return -1;
    }

    slavename = ttyname(masterfd);

    len = strlen(slavename);
    
    if (masterpath) {
	if ((*masterpath = calloc(1, len + 1)) == NULL) {
	    close(masterfd);
	    
	    return -1;
	}
	
	strncpy(*masterpath, slavename, len);
    }

    if (slavepath) {
	if ((*slavepath = calloc(1, len + 1)) == NULL) {
	    free(*masterpath);

	    close(masterfd);
	    
	    return -1;
	}

	strncpy(*slavepath, slavename, len);
    }

    return masterfd;
}

#elif (USE_SGI_4_PTYS)

int
R_open_pty_master_sgi_4(char **masterpath, char **slavepath)
{
    int masterfd;
    size_t len;
    char *slavename;

    masterfd = -1;

    user_enable_privileges(&(term->args->user));

    if ((slavename = _getpty(&masterfd, O_RDWR | O_NDELAY, PTY_SLAVE_MODE, 0))
	== NULL) {
	user_disable_privileges(&(term->args->user));

	return -1;
    }

    user_disable_privileges(&(term->args->user));

    len = strlen(slavename);

    if (masterpath) {
	if ((*masterpath = calloc(1, len + 1)) == NULL) {
	    close(masterfd);
	    
	    return -1;
	}

	strncpy(*masterpath, slavename, len);
    }

    if (slavepath) {
	if ((*slavepath = calloc(1, len + 1)) == NULL) {
	    free(*masterpath);

	    close(masterfd);
	    
	    return -1;
	}

	strncpy(*slavepath, slavename, len);
    }

    return masterfd;
}

#elif (USE_CONVEX_PTYS)

int
R_open_pty_master_convex(char **masterpath, char **slavepath)
{
    int masterfd;
    size_t len;
    char *mastername;

    while ((mastername = getpty()) != NULL) {
	if ((masterfd = open(mastername, O_RDWR)) >= 0) {
	    len = strlen(mastername);
	    
	    if (masterpath) {
		if ((*masterpath = calloc(1, len + 1)) == NULL) {
		    close(masterfd);
		    
		    return -1;
		}
		
		strncpy(*masterpath, mastername, len);
	    }
	    
	    if (slavepath) {
		if ((*slavepath = calloc(1, len + 1)) == NULL) {
		    free(*masterpath);

		    close(masterfd);
		    
		    return -1;
		}
		
		strncpy(*slavepath, mastername, len);
	    }

	    (*slavepath)[5] = 't';

	    return masterfd;
	}
    }

    return -1;
}

#elif (USE_GETPSEUDOTTY)

int
R_open_pty_master_getpseudotty(char **masterpath, char **slavepath)
{
    return (getpseudotty(slavepath, masterpath));
}

#elif (USE_DEV_PTC_PTYS)

/* FIXME: this doesn't return anything in masterpath on sgi systems... */

int
R_open_pty_master_dev_ptc(char **masterpath, char **slavepath)
{
    int masterfd;
#if !defined(sgi)
    int slavefd;
#endif
    struct stat masterstat;

    if ((masterfd = open(DEV_PTC_DEVICE, O_RDWR)) < 0) {

	return -1;
    }

    if (fstat(masterfd, &masterstat) < 0) {
	close(masterfd);

	return -1;
    }

    if (masterpath) {
	if ((*masterpath = calloc(1, PTY_NAME_LENGTH)) == NULL) {
	    close(masterfd);

	    return -1;
	}
    }

    if (slavepath) {
	if ((*slavepath = calloc(1, PTY_NAME_LENGTH)) == NULL) {
	    free(*masterpath);

	    close(masterfd);
	    
	    return -1;
	}
    }
    
    sprintf(*slavepath, "/dev/ttyq%d", minor(masterstat.st_rdev));

    /* FIXME: we shouldn't open the slave device here? */
    
#if !defined(sgi)

    sprintf(*masterpath, "/dev/ptyq%d", minor(masterstat.st_rdev));

    if ((slavefd = open(slavepath, O_RDWR)) < 0) {
	free(*masterpath);
	free(*slavepath);

	close(masterfd);

	return -1;
    }

    close(slavefd);

#endif /* !defined(sgi) */

    return masterfd;
}

#elif (USE_DEV_PTYM_CLONE_PTYS)

int
R_open_pty_master_dev_ptym_clone(char **masterpath, char **slavepath)
{
    int masterfd;
    size_t len;
    char *slavename;

    if ((masterfd = open(DEV_PTYM_CLONE_DEVICE, O_RDWR)) < 0) {

	return (R_open_pty_master_generic(masterpath, slavepath));
    }

    if ((slavename = ptsname(masterfd)) == NULL) {
	close(masterfd);

	return -1;
    }

    len = strlen(slavename);

    if (masterpath) {
	if ((*masterpath = calloc(1, len + 1)) == NULL) {
	    close(masterfd);

	    return -1;
	}
    }

    if (slavepath) {
	if ((*slavepath = calloc(1, len + 1)) == NULL) {
	    free(*masterpath);

	    close(masterfd);
	    
	    return -1;
	}
    }
    
}

#elif (USE_SCO_PTYS)

int
R_open_pty_master_sco(char **masterpath, char **slavepath)
{
    int i, masterfd;
    size_t len;
    char mastername[] = "/dev/ptyp\0\0\0\0\0\0\0";
    char slavename[] = "/dev/ttyp\0\0\0\0\0\0\0";
    
    for (i = 0 ; i < SCO_PTY_DEVICES ; i++) {
	/* FIXME: sprintf() isn't always safe. */

	sprintf(mastername, "%s%d", "/dev/ptyp", i);
	sprintf(slavename, "%s%d", "/dev/ttyp", i);

	if (access(slavename, F_OK) < 0) {
	    i = SCO_PTY_DEVICES;

	    break;
	}

	if ((masterfd = open(mastername, O_RDWR)) >= 0) {
	    if (access(slavename, R_OK | W_OK) == 0) {
		if (masterpath) {
		    len = strlen(mastername);
		    
		    if ((*masterpath = calloc(1, len + 1)) == NULL) {
			close(masterfd);
			
			return -1;
		    }
		    
		    strncpy(*masterpath, mastername, len);
		}
		
		if (slavepath) {
		    len = strlen(slavename);
		    
		    if ((*slavepath = calloc(1, len + 1)) == NULL) {
			free(*masterpath);
	
			close(masterfd);
			
			return -1;
		    }
		    
		    strncpy(*slavepath, slavename, len);
		}
		
		return masterfd;
	    }

	    close(masterfd);
	}
    }

    return -1;
}

#endif /* (USE_OPENRPTY) */

#if (USE_GENERIC_PTYS) || (USE_DEV_PTYM_CLONE_PTYS)

int
R_open_pty_master_generic(char **masterpath, char **slavepath)
{
    int masterfd;
#if defined(sun)
    int ptypgrp;
#endif
    static int devindex = 0, letter = 0;
    size_t masterlen, slavelen;
    char ptychars1[] = PTY_CHARS_1, ptychars2[] = PTY_CHARS_2;
    char mastername[PTY_NAME_LENGTH] = PTY_MASTER_PREFIX;
    char slavename[PTY_NAME_LENGTH] = PTY_SLAVE_PREFIX;

#if defined(CRAY)

    for ( ; devindex < MAX_PTYS ; devindex++) {
	sprintf(mastername, PTY_MASTER_FORMAT, devindex);
	sprintf(slavename, PTY_SLAVE_FORMAT, devindex);
//        fprintf(stderr, "opening %s/%s\n", *masterpath, *slavepath);

	if ((masterfd = open(mastername, O_RDWR)) < 0) {
#if defined(ENOENT)
	    if (errno == ENOENT) {

		return -1;
	    }
#endif /* defined(ENOENT) */

	    continue;
	}

	if (masterpath) {
	    masterlen = strlen(mastername);

	    if ((*masterpath = calloc(1, masterlen + 1)) == NULL) {
		close(masterfd);
		
		return -1;
	    }
	    
	    strncpy(*masterpath, mastername, masterlen);
	}
	
	if (slavepath) {
	    slavelen = strlen(slavename);

	    if ((*slavepath = calloc(1, slavelen + 1)) == NULL) {
		close(masterfd);
		
		return -1;
	    }
	    
	    strncpy(*slavepath, slavename, slavelen);
	}

	/* set this for next entry into this function. */
	
	devindex++;

	return masterfd;
    }

#else /* !defined(CRAY) */

    masterlen = strlen(mastername);
    slavelen = strlen(slavename);

    while (ptychars1[letter]) {
	mastername[(masterlen - 2)] = ptychars1[letter];
	slavename[(slavelen - 2)] = ptychars1[letter];

	while (ptychars2[devindex]) {
	    mastername[(masterlen - 1)] = ptychars2[devindex];
	    slavename[(slavelen - 1)] = ptychars2[devindex];

	    if ((masterfd = open(mastername, O_RDWR)) < 0) {
#if defined(ENOENT)
		if (errno == ENOENT) {

		    return -1;
		}

		continue;
	    }
#endif /* defined(ENOENT) */

#if defined(sun)		
	    if (PTY_IOCTL(masterfd, TIOCGPGRP, &ptypgrp) == 0
		|| errno != EIO) {
		close(masterfd);
		
		continue;
	    }
#endif /* defined(sun) */

	    if (masterpath) {
		if ((*masterpath = calloc(1, masterlen + 1)) == NULL) {
		    close(masterfd);
		    
		    return -1;
		}
		
		strncpy(*masterpath, mastername, masterlen);
	    }
	    
	    if (slavepath) {
		if ((*slavepath = calloc(1, slavelen + 1)) == NULL) {
		    close(masterfd);
		    
		    return -1;
		}
		
		strncpy(*slavepath, slavename, slavelen);
	    }
	    
	    return masterfd;
	}
    }

    devindex = 0;
    letter++;

#endif /* defined(CRAY) */

    return -1;
}

#endif /* ((USE_GENERIC_PTYS) || (USE_DEV_PTYM_CLONE_PTYS)) */

#if defined(__QNX__) && !defined(__QNXNTO__)
int qnx_set_login(const char *login, const char *slavepty)
{
    int sid;
    struct _proc_session procsession;
    struct _proc_session_reply procsessionreply;

    sid = getsid(getpid());

    memset(&procsession, 0, sizeof(procsession));
    memset(&procsessionreply, 0, sizeof(procsessionreply));

    procsession.type = _PROC_SESSION;
    procsession.subtype = _PROC_SUB_ACTION1;
    procsession.sid = sid;

    /* TODO: does QNX have strncpy()? */

    strncpy(procsession.name, login, sizeof(procsession.name));

    Send(1, &procsession, &procsessionreply,
	 sizeof(procsession), sizeof(procsessionreply));

    if (procsessionreply.status < 0) {

	return (procsessionreply.status);
    }

    procsession.type = _PROC_SESSION;
    procsession.subtype = _PROC_SUB_ACTION2;
    procsession.sid = sid;

    /* TODO: does QNX have snprintf? */

    snprintf(procsession.name, sizeof(procsession.name), "//%d%s",
	     getnid(), slavepty);

    Send(1, &procsession, &procsessionreply,
	 sizeof(procsession), sizeof(procsessionreply));

    return (procsessionreply.status);
}

#endif /* defined(__QNX__) && !defined(__QNXNTO) */

int
R_open_pty_master(char **masterpath, char **slavepath, int nonblocking)
{
    int fd, flags, retval;

    if ((fd = R_OPEN_PTY_MASTER(masterpath, slavepath)) < 0) {

	return -1;
    }

#if (DEBUG_PTY)
    fprintf(stderr, "R_open_pty_master(): opened master pty \"%s\"\n",
	    *masterpath);
#endif

    if (nonblocking) {
	if ((flags = fcntl(fd, F_GETFL, 0)) < 0) {
            fprintf(stderr, "%s: fcntl(F_GETFL) failed\n", __FUNCTION__);
	    close(fd);
	    
	    return -1;
	}

        fprintf(stderr, "flags1: %x\n", flags);
        if (!(flags & O_NONBLOCK)) {
            retval = 0;
            flags |= O_NONBLOCK;
            
            if ((retval = fcntl(fd, F_SETFL, flags)) < 0) {
                if (errno != EAGAIN) {
                    fprintf(stderr, "%s: fcntl(F_SETFL) failed\n", __FUNCTION__);
                    perror("errno");
                    close(fd);
                
                    return -1;
                }
            }
        }
    }
    
    return fd;
}

#if (USE_POSIX_TERMIOS) || (USE_TERMIOS)

/*
 * POSIX.1 termios interface with optional additions from BSD and System V.
 * This should also work as System V termios interface.
 */

void
pty_init_mode_termios(int fd, R_ptymode *ptymode)
{
#if defined(__FreeBSD__) || defined(__OpenBSD__)
    int ldisc;
#endif
    if (fd < 0
	|| ptymode == NULL) {

	return;
    }

    /*
     * NOTE: the c_cc special input characters for VDISCARD, VDSUSP, VEOL2,
     * VLNEXT, VREPRINT, VSTATUS, and VWERASE are not defined by POSIX.1. VSWTC
     * wasn't defined by the first version of POSIX.1. Some systems define
     * VSWTCH, which Resurrection defines to VSWTC, but I'm not sure if this
     * is correct.
     */

    if (PTY_GET_TERMIOS(fd, ptymode) < 0) {
	/* use system defaults. */
	ptymode->c_cc[VINTR] = CINTR;
	ptymode->c_cc[VQUIT] = CQUIT;
	ptymode->c_cc[VERASE] = CERASE;
	ptymode->c_cc[VKILL] = CKILL;
	ptymode->c_cc[VSTART] = CSTART;
	ptymode->c_cc[VSTOP] = CSTOP;
	ptymode->c_cc[VSUSP] = CSUSP;
#   if defined(VDSUSP)
	ptymode->c_cc[VDSUSP] = CDSUSP;
#   endif
#   if defined(VREPRINT)
	ptymode->c_cc[VREPRINT] = CRPRNT;
#   endif
#   if defined(VDISCARD)
	ptymode->c_cc[VDISCARD] = CFLUSH;
#   endif
#   if defined(VWERASE)
	ptymode->c_cc[VWERASE] = CWERASE;
#   endif
#   if defined(VLNEXT)
	ptymode->c_cc[VLNEXT] = CLNEXT;
#   endif
#   if defined(VSTATUS)
	ptymode->c_cc[VSTATUS] = CSTATUS;
#   endif
    }

    ptymode->c_cc[VEOF] = CEOF;
    ptymode->c_cc[VEOL] = VDISABLE;

#   if defined(VEOL2)
    ptymode->c_cc[VEOL2] = VDISABLE;
#   endif
#   if defined(VSWTC)
    ptymode->c_cc[VSWTC] = VDISABLE;
#   endif

    /* VMIN and VTIME are not needed for canonical mode. */

#if 0
#   if (VMIN != VEOF)
    ptymode->c_cc[VMIN] = 1;
#   endif
#   if (VTIME != VEOL)
    ptymode->c_cc[VTIME] = 0;
#   endif
#endif

    ptymode->c_cc[VERASE] = R_BACKSPACE_CHAR;

    /*
     * FIXME: looks like some of the values that are said to be not defined
     * by POSIX.1 might be defined by later versions of POSIX.
     */

    /*
     * input modes. 
     *
     * NOTE: IMAXBEL, IUCLC, and IXANY are not defined by POSIX.1.
     */

    ptymode->c_iflag = (BRKINT | IGNPAR | ICRNL | IXON
#   if defined(IMAXBEL)
		    | IMAXBEL
#   endif
	);
    
    /*
     * output modes.
     *
     * NOTE: only OPOST is defined by POSIX.1.
     */

    ptymode->c_oflag = (OPOST 
#   if defined(ONLCR)
			| ONLCR
#   endif
	);
    
    /*
     * control modes.
     *
     * NOTE: CBAUD, CCTS_OFLOW, CIBAUD, CIGNORE, CRTSCTS, CRTS_IFLOW, LOBLK,
     * and MDMBUF are not defined by POSIX.1. Also, baud rates over B38400 are
     * not defined by POSIX.1.
     */

#if 0
    ptymode->c_cflag = (CS8 | CLOCAL | CREAD);
#endif

    ptymode->c_cflag = (CS8 | CREAD);

    /* clear CLOCAL so the child will receive a HUP when the parent exits. */

    ptymode->c_cflag &= ~CLOCAL;

    /*
     * line discipline modes.
     *
     * NOTE: ALTWERASE, ECHOCTL, ECHOKE, ECHOPRT, FLUSHO, NOKERNINFO, PENDIN,
     * and XCASE are not defined by POSIX.1.
     */

    ptymode->c_lflag = (ISIG | ICANON | IEXTEN | ECHO | ECHOE | ECHOK
#   if defined (ECHOCTL) && defined (ECHOKE)
		    | ECHOCTL | ECHOKE
#   endif
	);

#   if defined(__FreeBSD__) || defined(__OpenBSD__)
    ldisc = 0;
    ioctl(fd, TIOCSETD, &ldisc);
#   elif (USE_POSIX_TERMIOS)
    ptymode->c_line = 0;
#   elif defined(NTTYDISC)
    ptymode->c_line = NTTYDISC;
#   else
    ptymode->c_line = 0;
#   endif

    return;
}

#elif (USE_SYSV_TERMIO)

void
pty_init_mode_sysv_termio(int fd, R_ptymode *ptymode)
{
    if (fd < 0
	|| ptymode == NULL) {

	return;
    }

    if (PTY_IOCTL(fd, TCGETA, &(ptymode->tio)) < 0) {
	ptymode->tio.c_cc[VINTR] = CINTR;
	ptymode->tio.c_cc[VQUIT] = CQUIT;
	ptymode->tio.c_cc[VERASE] = CERASE;
	ptymode->tio.c_cc[VKILL] = CKILL;
    }

    ptymode->tio.c_cc[VEOF] = CEOF;
    ptymode->tio.c_cc[VEOL] = VDISABLE;

    /* VMIN and VTIME are not needed for canonical mode. */

#if 0
#   if (VMIN != VEOF)
    ptymode->c_cc[VMIN] = 1;
#   endif
#   if (VTIME != VEOL)
    ptymode->c_cc[VTIME] = 0;
#   endif
#endif

    ptymode->tio.c_cc[VERASE] = R_BACKSPACE_CHAR;

    /* input modes. */

    ptymode->tio.c_iflag = (BRKINT | IGNPAR | ICRNL | IXON | IMAXBEL);

    /* output modes. */

    ptymode->tio.c_oflag = (OPOST | ONLCR);

    /* control modes. */

#if 0
    ptymode->tio.c_cflag = (PTY_BAUDRATE | CS8 | CLOCAL | CREAD);
#endif

    ptymode->tio.c_cflag = (PTY_BAUDRATE | CS8 | CREAD);

    /* clear CLOCAL so the child will receive a HUP when the parent exits. */

    ptymode->tio.c_cflag &= ~CLOCAL;

    /* line discipline modes. */

    ptymode->tio.c_lflag = (ISIG | ICANON | IEXTEN | ECHO | ECHOE | ECHOK
			    | ECHOCTL | ECHOKE);

    ptymode->tio.c_line = 0;

#   if (USE_LTCHARS)
    if (PTY_IOCTL(fd, TIOCGLTC, &(ptymode->lc)) < 0) {
	ptymode->lc.t_suspc = CSUSP;
	ptymode->lc.t_dsuspc = CDSUSP;
	ptymode->lc.t_rprntc = CRPRNT;
	ptymode->lc.t_flushc = CFLUSH;
	ptymode->lc.t_werasc = CWERASE;
	ptymode->lc.t_lnextc = CLNEXT;
    }
#   endif /* USE_LTCHARS */

#   if (USE_SYSV_TERMIO_WITH_LOCAL)
    if (PTY_IOCTL(fd, TIOCLGET, &(ptymode->local)) < 0) {
	ptymode->local = 0;
    }
#   endif

    return;
}

#else /* !((USE_POSIX_TERMIOS) || (USE_TERMIOS) || (USE_SYSV_TERMIO)) */

/* sgtty interface. */
    
void
pty_init_mode_sgtty(int fd, R_ptymode *ptymode)
{
#   if defined(sony)
    struct jtchars defaultjc = { 'J', 'B' };
#   endif

    if (fd < 0
	|| ptymode == NULL) {

	return;
    }

    /* get parameters. */
    if (PTY_IOCTL(fd, TIOCGETP, &(ptymode->sg)) < 0) {
	ptymode->sg.sg_erase = CERASE;
	ptymode->sg.sg_kill = CKILL;
    }
    ptymode->sg.sg_flags = (CRMOD | ECHO | EVENP | ODDP);
    
    /* get special characters. */
    if (PTY_IOCTL(fd, TIOCGETC, &(ptymode->tc)) < 0) {
	ptymode->tc.t_intrc = CINTR;
	ptymode->tc.t_quitc = CQUIT;
	ptymode->tc.t_startc = CSTART;
	ptymode->tc.t_stopc = CSTOP;
	ptymode->tc.t_eofc = CEOF;
	ptymode->tc.t_brkc = -1;
    }

#   if (USE_LTCHARS)

    /* get local special characters. */
    if (PTY_IOCTL(fd, TIOCGLTC, &(ptymode->lc)) < 0) {
	ptymode->lc.t_suspc = CSUSP;
	ptymode->lc.t_dsuspc = CDSUSP;
	ptymode->lc.t_rprntc = CRPRNT;
	ptymode->lc.t_flushc = CFLUSH;
	ptymode->lc.t_werasc = CWERASE;
	ptymode->lc.t_lnextc = CLNEXT;
    }

#       if defined(__hpux)
    ptymode->lc.t_rprntc = _POSIX_VDISABLE;
    ptymode->lc.t_flushc = _POSIX_VDISABLE;
    ptymode->lc.t_werasc = _POSIX_VDISABLE;
    ptymode->lc.t_lnextc = _POSIX_VDISABLE;
#       endif

#   endif /* USE_LTCHARS */

    ptymode->sg.sg_erase = R_BACKSPACE_CHAR;

    /* get line discipline. */
    PTY_IOCTL(fd, TIOCGETD, &(ptymode->line));

#   if defined(NTTYDISC)
    ptymode->line = NTTYDISC;
#   endif

    ptymode->local = (LCRTBS | LCRTERA | LCTLECH | LPASS8 | LCRTKIL);

#   if defined(sony)
    if (PTY_IOCTL(fd, TIOCKGET, (char *)&(ptymode->jmode)) < 0) {
	ptymode->jmode = KM_SYSSJIS | KM_ASCII;
    }
    
    ptymode->jmode &= ~KM_KANJI;

    if (PTY_IOCTL(fd, TIOCKGETC, (char *)&(ptymode->jc)) < 0) {
	ptymode->jc = defaultjc;
    }
#   endif /* sony */

    return;
}

#endif /* USE_POSIX_TERMIOS || USE_TERMIOS */

int
R_init_pty_slave(struct R_user *user, const char *slavepath, int maxfds)
{
    int i, fd, result;
    pid_t pid;

    if (user == NULL
	|| slavepath == NULL) {

	return -1;
    }

    if (maxfds <= 0) {
	maxfds = open_max();
    }

#if (DEBUG_PTY)
    fprintf(stderr, "R_init_pty_slave(): closing %d fds\n", maxfds);
#endif

    for (i = 0 ; i < maxfds ; i++) {
	close(i);
    }

    /* become process group leader. */

    PROCESS_CREATE_SESSION(pid);

    if (pid < 0) {
        fprintf(stderr, "R_init_pty_slave(): failed to create pty session: %s\n",
		strerror(errno));
    }

#if defined(__QNX__) && !defined(__QNXNTO__)
    qnx_set_login(getlogin(), slavepath)
#endif

    PTY_DETACH_CONTROLLING_TERMINAL(slavepath);

    PTY_UNLOCK_SLAVE(slavepath);

    /* open slave pty. */

    user_enable_privileges(user);

    if ((fd = open(slavepath, O_RDWR)) < 0) {
	user_disable_privileges(user);

	return -1;
    }

    user_disable_privileges(user);

    do {
	result = dup2(fd, STDIN_FILENO);
    } while (result == -1 && errno == EINTR);

    if (result == -1) {

	return -1;
    }

    do {
	result = dup2(fd, STDOUT_FILENO);
    } while (result == -1 && errno == EINTR);

    if (result == -1) {

	return -1;
    }

    do {
	result = dup2(fd, STDERR_FILENO);
    } while (result == -1 && errno == EINTR);

    if (result == -1) {

	return -1;
    }

    if (fd > STDERR_FILENO) {
	close(fd);
    }

#if (DEBUG_PTY)
    fprintf(stderr, "R_init_pty_slave(): opened slave pty \"%s\"\n", slavepath);
#endif

    /* initialize slave pty.
     * HAZARD: on some systems, this will temporarily use root privileges. */

    PTY_INIT_SLAVE(user, fd);

    PTY_DETACH_SLAVE(fd, pid, slavepath);

    return 0;
}

ssize_t
R_read_pty(int fd, R_text_t *buffer, size_t count)
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

	if ((nbread = Rreadnb(fd, buffer, count)) <= 0) {
            if (nbread == 0) {

                return len;
            } else if (errno == EINTR) {
		
		continue;
	    } else if (errno == EAGAIN || errno == EWOULDBLOCK) {
		
		return len;
	    } else if (len == 0) {

		return -1;
	    }

            return -1;
//	    return len;
	}

	buffer += nbread;
	count -= nbread;
	len += nbread;
    }

    return len;
}

/*
 * FIXME: these should probably be changed to switch to blocking mode...?
 */
#if (SUPPORT_RTERM_UTF8_CHARS)
ssize_t
R_write_pty_utf8(int fd, R_text_t *buffer, size_t count)
{
    ssize_t len, nbwritten;
    struct R_term *term = R_global.app->client;
    static size_t buflen = 0;
    int slen;
    static R_text_t *buf = NULL;

    if (fd < 0
	|| buffer == NULL
	|| count <= 0) {

	return -1;
    }

    if (term->privmodes & RTERM_PRIVMODE_UTF8_CHARS) {
        if (buflen < 4 * count) {
            buf = malloc(4 * count);
            buflen = 4 * count;
        }
        RTERM_UTF8_ENC(buffer, buf, &slen, count);
        count = slen;
    } else {
        buf = buffer;
    }

    len = count;

    while (count) {
	errno = 0;

        if ((nbwritten = Rwritenb(fd, buf,
				  ((count <= PTY_MAX_WRITE)
				   ? count
				   : PTY_MAX_WRITE))) < 0) {
            if (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK) {
		
                continue;
            }
	    
	    return -1;
        }
	
        buf += nbwritten;
        count -= nbwritten;
	len += nbwritten;
    }
    if (term->privmodes & RTERM_PRIVMODE_UTF8_CHARS) {
        free(buf);
        buflen = 0;
        buf = NULL;
    }

    return len;
}
#endif

ssize_t
R_write_pty(int fd, R_text_t *buffer, size_t count)
{
    ssize_t len, nbwritten;

    if (fd < 0
	|| buffer == NULL
	|| count <= 0) {

	return -1;
    }

    len = count;

    while (count) {
	errno = 0;

        if ((nbwritten = Rwritenb(fd, buffer,
				  ((count <= PTY_MAX_WRITE)
				   ? count
				   : PTY_MAX_WRITE))) < 0) {
            if (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK) {
		
                continue;
            }
	    
	    return -1;
        }
	
        buffer += nbwritten;
        count -= nbwritten;
	len += nbwritten;
    }

    return len;
}

