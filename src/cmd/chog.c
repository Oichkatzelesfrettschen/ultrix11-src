
/**********************************************************************
 *   Copyright (c) Digital Equipment Corporation 1984, 1985, 1986.    *
 *   All Rights Reserved. 					      *
 *   Reference "/usr/src/COPYRIGHT" for applicable restrictions.      *
 **********************************************************************/
/**
 * @file chog.c
 * @brief Change file owner and group.
 */

/*
 * Chog: CHange Owner and Group
 * chog user file ...
 */

static char Sccsid[] = "@(#)chog.c	3.0	4/21/86";

#include <ctype.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

/**
 * @brief Program entry point.
 *
 * @param argc Number of command line arguments.
 * @param argv Array of argument strings.
 * @return EXIT_SUCCESS on success, EXIT_FAILURE on error.
 */
int main(int argc, char **argv) {
  int uid, gid;
  int status = 0;
  struct passwd *pwd, *getpwnam(), *getpwuid();

  if (argc < 3) {
    printf("usage: chog user file ...\n");
    exit(1);
  }
  --argc;
  ++argv;
  if (isnumber(*argv)) {
    uid = atoi(*argv);
    pwd = getpwuid(uid);
  } else
    pwd = getpwnam(*argv);
  if (pwd == NULL) {
    printf("unknown user id: %s\n", *argv);
    exit(1);
  }
  uid = pwd->pw_uid;
  gid = pwd->pw_gid;

  while (++argv, --argc) {
    if (chown(*argv, uid, gid) < 0) {
      perror(*argv);
      status = 1;
    }
  }
  return status;
}

/**
 * @brief Determine if a string contains only digits.
 *
 * @param s Input string to check.
 * @return 1 if the string is numeric, otherwise 0.

 */
static int isnumber(const char *s) {
  while (isdigit((unsigned char)*s))
    s++;
  return (*s == '\0') ? 1 : 0;

}
