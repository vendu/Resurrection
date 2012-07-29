/*
 * user.c - user and group management routines for Resurrection.
 * Copyright (C) 2003 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#include <Resurrection/Resurrection.h>

/* FIXME: fix these to just return when __CYGWIN__ is defined. */

struct R_user *
user_alloc(void)
{
    return (calloc(1, sizeof(struct R_user)));
}

void
user_init(struct R_user *user)
{
    if (user == NULL) {

	return;
    }

    user->realuid = getuid();
    user->effectiveuid = geteuid();
    user->realgid = getgid();
    user->effectivegid = getegid();

#if (DEBUG_USER)
    fprintf(stderr, "user_init(): realuid == %d, effectiveuid == %d, realgid == %d, effectivegid == %d\n", getuid(), geteuid(), getgid(), getegid());
#endif /* DEBUG_USER */

    return;
}

void
user_enable_privileges(struct R_user *user)
{
    if (user == NULL) {

	return;
    }

#if defined(USE_SETRESUID) && defined(USE_SETRESGID)
    setresuid(user->realuid, user->effectiveuid, user->effectiveuid);

    setresgid(user->realgid, user->effectivegid, user->effectivegid);
#elif defined(USE_SETEUID) && defined(USE_SETEGID)
    seteuid(user->effectiveuid);

    setegid(user->effectivegid);
#elif defined(USE_SAVED_IDS)
    setreuid(user->realuid, user->effectiveuid);

    setregid(user->realgid, user->effectivegid);
#else
    setreuid(-1, user->effectiveuid);
    setreuid(user->realuid, -1);

    setregid(-1, user->effectivegid);
    setregid(user->realgid, -1);
#endif

#if (DEBUG_USER)
    fprintf(stderr, "user_enable_privileges(): realuid == %d, effectiveuid == %d, realgid == %d, effectivegid == %d\n", getuid(), geteuid(), getgid(), getegid());
#endif /* DEBUG_USER */

    return;
}

void
user_disable_privileges(struct R_user *user)
{
    if (user == NULL) {

	return;
    }

#if defined(USE_SETRESUID) && defined(USE_SETRESGID)
    setresuid(user->realuid, user->realuid, user->effectiveuid);

    setresgid(user->realgid, user->realgid, user->effectivegid);
#elif defined(USE_SETEUID) && defined(USE_SETEGID)
    seteuid(user->realuid);
    setegid(user->realgid);
#elif defined(USE_SAVED_IDS)
    setreuid(user->realuid, user->realuid);

    setregid(user->realgid, user->realgid);
#else
    setreuid(user->effectiveuid, -1);
    setreuid(-1, user->realgid);

    setregid(user->effectivegid, -1);
    setregid(-1, user->effectivegid);
#endif

#if (DEBUG_USER)
    fprintf(stderr, "user_disable_privileges(): realuid == %d, effectiveuid == %d, realgid == %d, effectivegid == %d\n", getuid(), geteuid(), getgid(), getegid());
#endif /* DEBUG_USER */

    return;
}

