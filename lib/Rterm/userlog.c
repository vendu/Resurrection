/*
 * userlog.c - utmp and other user logging for Resurrection.
 * Copyright (C) 2003 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#if 0

#include <Resurrection/Resurrection.h>

#define USERLOG_WRITE_LOCK_RETRIES 16

/*
 * FIXME: I have a horrible belief that UTMPX support may be broken, but I'm
 * too annoyed to figure out how GNU libc does it... :)
 */

void
userlog_init(struct R_userlog *userlog, int fd, const char *ptyname,
	     const char *hostname)
{
    size_t len;

    if (ptyname == NULL) {

	return;
    }

    userlog->fd = fd;

    len = strlen(ptyname);

    if (len) {
	if ((userlog->ptyname = calloc(1, len + 1)) == NULL) {

	    return;
	}

	strncpy(userlog->ptyname, ptyname, len);
    }
    
    len = strlen(hostname);

    if (len) {
	if ((userlog->hostname = calloc(1, len + 1)) == NULL) {
	    free(userlog->ptyname);

	    return;
	}

	strncpy(userlog->hostname, hostname, len);
    }
    
    user_init(&(userlog->user));

    return;
}

#if (USE_SYSV_UTMP) && !USE_UTEMPTER

void
userlog_add_log_entries(struct R_userlog *userlog)
{
    int i;
    char *ptyname;
    struct passwd *passwd;
#   if (USE_UTMPX)
    struct utmpx utmp;
    struct utmp utmp2;
#   else /* !USE_UTMPX */
    struct utmp utmp;
#   endif /* USE_UTMPX */
    
    if (userlog == NULL) {
	
	return;
    }

    ptyname = userlog->ptyname;
    
    passwd = getpwuid(userlog->user.realuid);
    
    memset(&utmp, 0, sizeof(utmp));

#if (USE_UTMPX)
    memset(&utmp2, 0, sizeof(utmp2));
#endif

    if (!strncmp(ptyname, "/dev/", sizeof("/dev/") - 1)) {
        ptyname += sizeof("/dev/") - 1;
    }
    
    if (!strncmp(ptyname, "pty", sizeof("pty") - 1)
        || !strncmp(ptyname, "tty", sizeof("tty") - 1)) {
            /* BSD naming */
	    
        strncpy(userlog->utmpid, ptyname + sizeof("pty") - 1,
		sizeof(userlog->utmpid));
    } else if (sscanf(ptyname, "pts/%d", &i) == 1) {
            /* SysV naming. */
	    
            snprintf(userlog->utmpid, sizeof(userlog->utmpid), "vt%02x", i);
    } else {
	fprintf(stderr, "userlog_add_log_entries(): unable to parse tty name \"%s\"", ptyname);
	
	userlog->utmpid[0] = '\0';
	
	return;
    }
    
    utmp.ut_type = DEAD_PROCESS;
    
    strncpy(utmp.ut_id, userlog->utmpid, sizeof(utmp.ut_id));

#if 0    
    user_enable_privileges(&(userlog->user));
#endif

    utmpname(UTMP_FILENAME);

    setutent();

#   if (USE_UTMPX)

    getutmp(&utmp, &utmp2);
    getutid(&utmp2);

#   else /* !USE_UTMPX */

    getutid(&utmp);

#   endif /* USE_UTMPX */

    strncpy(utmp.ut_line, ptyname, sizeof(utmp.ut_line));
    strncpy(utmp.ut_name, passwd->pw_name, sizeof(utmp.ut_name));
    strncpy(utmp.ut_user, passwd->pw_name, sizeof(utmp.ut_user));

    if (userlog->hostname == NULL) {
        strncpy(utmp.ut_host, "localhost", sizeof(utmp.ut_host));
    } else {
        strncpy(utmp.ut_host, userlog->hostname, sizeof(utmp.ut_host));
    }
    
    utmp.ut_type = USER_PROCESS;
    utmp.ut_pid = getpid();

#   if (USE_UTMPX)

    utmp.ut_session = getsid(0);
    utmp.ut_xtime = time(NULL);
    utmp.ut_tv.tv_usec = 0;

    getutmp(&utmp, &utmp2);

    pututline(&utmp2);
    pututxline(&utmp);

#   else /* !USE_UTMPX */

    utmp.ut_time = time(NULL);

    pututline(&utmp);

#   endif /* USE_UTMPX */

    userlog_update_wtmp(WTMP_FILENAME, &utmp);
    
    endutent();

#if 0
    user_disable_privileges(&(userlog->user));
#endif
    
    return;
}

#   if (USE_UTMPX)

void
userlog_remove_log_entries(struct R_userlog *userlog)
{
    struct utmp utmp;
    struct utmpx utmpx;
    
    if (userlog == NULL
	|| userlog->utmpid[0] == '\0') {
	
        return;
    }

#if 0
    user_enable_privileges(&(userlog->user));
#endif

    utmpname(UTMP_FILENAME);

    setutent();

    strncpy(utmp.ut_id, userlog->utmpid, sizeof(utmp.ut_id));

    utmp.ut_type = USER_PROCESS;

    if (getutid(&utmp) == NULL) {
#if 0
        user_disable_privileges(&(userlog->user));
#endif

        return;
    }

    utmp.ut_type = DEAD_PROCESS;
    utmp.ut_time = time(NULL);

    pututline(&utmp);

    getutmpx(&utmp, &utmpx);

    userlog_update_wtmp(WTMP_FILENAME, &utmpx);

    endutent();

#if 0
    user_disable_privileges(&(userlog->user));
#endif

    return;
}

#   else /* !USE_UTMPX */

void
userlog_remove_log_entries(struct R_userlog *userlog)
{
    struct utmp *utmp;
    pid_t pid;

    if (userlog == NULL
	|| userlog->utmpid[0] == '\0') {

	return;
    }

    pid = getpid();

#if 0
    user_enable_privileges(&(userlog->user));
#endif

    utmpname(UTMP_FILENAME);

    setutent();

    while ((utmp = getutent()) != NULL) {
        if (utmp->ut_pid == pid) {
            utmp->ut_type = DEAD_PROCESS;
            utmp->ut_pid = 0;
            utmp->ut_user[0] = '\0';
            utmp->ut_time = time(NULL);

            pututline(utmp);

            userlog_update_wtmp(WTMP_FILENAME, utmp);

            break;
        }
    }

    endutent();

#if 0
    user_disable_privileges(&(userlog->user));
#endif

    return;
}

#   endif /* USE_UTMPX */

#endif /* USE_SYSV_UTMP && !USE_UTEMPTER */

#if !USE_SYSV_UTMP

#   if defined(__FreeBSD__) || defined(__NetBSD__) || defined(__bsdi__)

int
userlog_get_tty_slot(const char *ptyname)
{
    int slot;
    struct ttyent *ttyent;

    if (ptyname == NULL) {

	return -1;
    }

    setttyent();

    for (slot = 1 ; (ttyent = getttyent()) ; ++slot) {
        if (!strcmp(ttyent->ty_name, ptyname)) {
            endttyent();

            return slot;
        }
    }

    endttyent();

    return -1;
}

void
userlog_bsd_login(struct utmp *utmp)
{
    int fd, slot;

    if (utmp == NULL) {

	return;
    }

    if ((slot = userlog_get_tty_slot(utmp->ut_line)) < 0) {

	return;
    }

    if ((fd = open(UTMP_FILENAME, O_WRONLY | O_CREAT, 0644)) < 0) {

	return;
    }

    lseek(fd, slot * sizeof(struct utmp), L_SET);

    write(fd, utmp, sizeof(struct utmp));
    
    close(fd);

    if ((fd = open(WTMP_FILENAME, O_WRONLY | O_CREAT | O_APPEND, 0)) < 0) {

	return;
    }

    write(fd, utmp, sizeof(struct utmp));

    close(fd);

    return;
}

#   else /* !(defined(__FreeBSD__) || defined(__NetBSD__) || defined(__bsdi__))
*/

int
userlog_get_tty_slot(const char *ptyname)
{
    int i;
    FILE *fp;
    char buffer[256], devicename[256];

    if (ptyname == NULL) {

	return -1;
    }

    if ((fp = fopen(UTMP_FILENAME, "r")) == NULL) {

	return -1;
    }
     
    for (i = 1 ; fgets(buffer, sizeof(buffer), fp) != NULL ; i++) {
	if (*buffer == '#' || sscanf(buffer, "%s", devicename) != 1) {

	    continue;
	}
	
	if (!strcmp(ptyname, devicename)) {
	    fclose(fp);
	    
	    return i;
	}

	fclose(fp);
    }

    return -1;
}

/* TODO: check return values. */

int
userlog_write_utmp_entry(struct R_userlog *userlog, struct utmp *utmp)
{
    FILE *fp;

    if (userlog == NULL
	|| utmp == NULL) {

	return -1;
    }

#if 0
    user_enable_privileges(&(userlog->user));
#endif

    if ((fp = fopen(UTMP_FILENAME, "r+")) == NULL) {

	return -1;
    }

    if ((userlog->utmppos = userlog_get_tty_slot(utmp->ut_line)) < 0) {

	return -1;
    }
    
    fseek(fp, userlog->utmppos, 0);
	
    fwrite(utmp, sizeof(struct utmp), 1, fp);
	
    fclose(fp);

#if 0
    user_disable_privileges(&(userlog->user));
#endif

    return 0;
}

void
userlog_remove_log_entries(struct R_userlog *userlog)
{
    if (userlog == NULL
	|| userlog->utmpline[0] == '\0') {

	return;
    }

    logout(userlog->utmpline);

    logwtmp(userlog->utmpline, "", "");

    return ;
}

#   endif /* defined(__FreeBSD__) || defined(__NetBSD__) || defined(__bsdi__) */

void
userlog_add_log_entries(struct R_userlog *userlog)
{
    char *ptyname;
    struct passwd *passwd;
    struct utmp utmp;

    if (userlog == NULL) {

	return;
    }

    ptyname = userlog->ptyname;

    passwd = getpwuid(userlog->user.realuid);

    memset(&utmp, 0, sizeof(utmp));

    if (!strncmp(ptyname, "/dev/", sizeof("/dev/") - 1)) {
        ptyname += sizeof("/dev/") - 1;
    }

    if (!strncmp(ptyname, "pty", sizeof("pty") - 1)
        || strncmp(ptyname, "tty", sizeof("tty") - 1)) {
        strncpy(userlog->utmpid, ptyname + sizeof("pty") - 1,
		sizeof(userlog->utmpid));
    } else {
        fprintf(stderr, "%s: userlog_add_log_entries(): unable to parse tty name \"%s\"", R_global.app->name, ptyname);

        userlog->utmpid[0] = '\0';

        return;
    }

#   if defined(__FreeBSD__) || defined(__NetBSD__) || defined(__bsdi__)

    strncpy(userlog->utmpline, ptyname, sizeof(userlog->utmpline));

    strncpy(utmp.ut_line, ptyname, UT_LINESIZE);

    strncpy(utmp.ut_name, passwd->pw_name, sizeof(utmp.ut_name));

    if (userlog->hostname == NULL) {
        strncpy(utmp.ut_host, "localhost", sizeof(utmp.ut_host));
    } else {
        strncpy(utmp.ut_host, userlog->hostname, sizeof(utmp.ut_host));
    }

    utmp.ut_time = time(NULL);

    userlog_bsd_login(&utmp);

#   else /* !(defined(__FreeBSD__) || defined(__NetBSD__) || defined(__bsdi__))
*/

    strncpy(utmp.ut_line, userlog->utmpid, sizeof(utmp.ut_line));

    strncpy(utmp.ut_name, passwd->pw_name, sizeof(utmp.ut_name));

    if (hostname == NULL) {
        strncpy(utmp.ut_host, "localhost", sizeof(utmp.ut_host));
    } else {
        strncpy(utmp.ut_host, hostname, sizeof(utmp.ut_host));
    }

    utmp.ut_time = time(NULL);

    if (userlog_write_utmp_entry(userlog, &utmp) < 0) {
        utmpid[0] = '\0';
    }

#   endif /* defined(__FreeBSD__) || defined(__NetBSD__) || defined(__bsdi__) */

    return;
}

/* TODO: check return values. */

void
userlog_remove_log_entries(struct R_userlog *userlog)
{
    FILE *fp;
    struct utmp utmp;

    if (userlog == NULL
	|| userlog->utmpid[0] == '\0') {

	return;
    }

    memset(&utmp, 0, sizeof(utmp));

#if 0
    user_enable_privileges(&(userlog->user));
#endif

    if ((fp = fopen(UTMP_FILENAME, "r+")) == NULL) {

	return;
    }

    fseek(fp, userlog->utmppos, 0);

    fwrite(&utmp, sizeof(utmp), 1, fp);

    fclose(fp);

#if 0
    user_disable_privileges(&(userlog->user));
#endif

    return;
}

#endif /* !USE_SYSV_UTMP */

#if !(USE_SYSV_UTMP && USE_UTMPX) && !USE_UTEMPTER

void
userlog_update_wtmp(const char *filename, struct utmp *utmp)
{
    int fd, retry;
    struct flock flock;

    if (filename == NULL
	|| utmp == NULL) {

	return;
    }
    
    if ((fd = open(filename, O_WRONLY | O_APPEND, 0)) < 0) {
	
	return;
    }
    
    flock.l_type = F_WRLCK;
    flock.l_start = 0;
    flock.l_whence = SEEK_END;
    flock.l_len = 0;
    
    retry = USERLOG_WRITE_LOCK_RETRIES;

    /* attempt lock with F_SETLK - F_SETLKW would cause a deadlock. */
    while (retry--) {
        if ((fcntl(fd, F_SETLK, &flock) < 0)
	    && errno != EACCES
	    && errno != EAGAIN) {
            fprintf(stderr, "userlog_update_wtmp(): unable to establish write lock on \"%s\": %s\n", filename, strerror(errno));
	    
            close(fd);
	    
            return;
        } else if (errno == EACCES || errno == EAGAIN) {
            fprintf(stderr, "userlog_update_wtmp(): unable to establish write lock on \"%s\": %s\n", filename, strerror(errno));
        }
    }
    
    write(fd, utmp, sizeof(struct utmp));
    
    flock.l_type = F_UNLCK;
    
    fcntl(fd, F_SETLK, &flock);
    
    close(fd);
    
    return;
}

#endif /* !(USE_SYSV_UTMP && USE_UTMPX) && !USE_UTEMPTER */

#endif
