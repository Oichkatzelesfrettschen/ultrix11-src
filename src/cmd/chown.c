
/**********************************************************************
 *   Copyright (c) Digital Equipment Corporation 1984, 1985, 1986.    *
 *   All Rights Reserved. 					      *
 *   Reference "/usr/src/COPYRIGHT" for applicable restrictions.      *
 **********************************************************************/
/**
 * @file chown.c
 * @brief Change file owner.
 */

/*
 * chown uid file ...
 */

static char Sccsid[] = "@(#)chown.c 3.0 4/21/86";
#include <ctype.h>
#include <pwd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

struct passwd *pwd, *getpwnam();
struct stat stbuf;
int uid;
int status;

/**
 * @brief Program entry point.
 *
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return EXIT_SUCCESS on success.
 */
int main(int argc, char **argv) {
  register c;

  if (argc < 3) {
    printf("usage: chown uid file ...\n");
    exit(4);
  }
  if (isnumber(argv[1])) {
    uid = atoi(argv[1]);
    goto cho;
  }
  if ((pwd = getpwnam(argv[1])) == NULL) {
    printf("unknown user id: %s\n", argv[1]);
    exit(4);
  }
  uid = pwd->pw_uid;

cho:
  for (c = 2; c < argc; c++) {
    stat(argv[c], &stbuf);
    if (chown(argv[c], uid, stbuf.st_gid) < 0) {
      perror(argv[c]);

      /* SYSTEM V compatibility */
      /* printf("%s: permission denied, must be super-user\n",
              argv[c]); */
      status = 1;
    }
  }
  exit(status);
}

isnumber(s) char *s;
{
  register c;

  while (c = *s++)
    if (!isdigit(c))
      return (0);
  return (1);
}
