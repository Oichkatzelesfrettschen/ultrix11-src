
/**********************************************************************
 *   Copyright (c) Digital Equipment Corporation 1984, 1985, 1986.    *
 *   All Rights Reserved. 					      *
 *   Reference "/usr/src/COPYRIGHT" for applicable restrictions.      *
 **********************************************************************/

/*
 * SCCSID: @(#)getpw.c	3.0	4/22/86
 */
#include <stdio.h>

/**
 * @brief Retrieve the password entry for a user.
 *
 * Looks up @p uid in /etc/passwd and copies the corresponding
 * entry into @p buf.
 *
 * @param uid User ID to search for.
 * @param buf Destination buffer for the passwd line.
 * @return 0 on success, 1 on failure.
 */
getpw(uid, buf) int uid;
char buf[];
{
  static FILE *pwf;
  register n, c;
  register char *bp;

  if (pwf == 0)
    pwf = fopen("/etc/passwd", "r");
  if (pwf == NULL)
    return (1);
  rewind(pwf);

  for (;;) {
    bp = buf;
    while ((c = getc(pwf)) != '\n') {
      if (c == EOF)
        return (1);
      *bp++ = c;
    }
    *bp++ = '\0';
    bp = buf;
    n = 3;
    while (--n)
      while ((c = *bp++) != ':')
        if (c == '\n')
          return (1);
    while ((c = *bp++) != ':') {
      if (c < '0' || c > '9')
        continue;
      n = n * 10 + c - '0';
    }
    if (n == uid)
      return (0);
  }
}
