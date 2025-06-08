
/**********************************************************************
 *   Copyright (c) Digital Equipment Corporation 1984, 1985, 1986.    *
 *   All Rights Reserved. 					      *
 *   Reference "/usr/src/COPYRIGHT" for applicable restrictions.      *
 **********************************************************************/
/**
 * @file basename.c
 * @brief Strip directory names.
 */

static char Sccsid[] = "@(#)basename.c 3.0 4/21/86";
#include "stdio.h"

/**
 * @brief Program entry point.
 *
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return EXIT_SUCCESS on success.
 */
int main(int argc, char **argv) {
  register char *p1, *p2, *p3;

  if (argc < 2) {
    putchar('\n');
    exit(1);
  }
  p1 = argv[1];
  p2 = p1;
  while (*p1) {
    if (*p1++ == '/')
      p2 = p1;
  }
  if (argc > 2) {
    for (p3 = argv[2]; *p3; p3++)
      ;
    while (p3 > argv[2])
      if (p1 <= p2 || *--p3 != *--p1)
        goto output;
    *p1 = '\0';
  }
output:
  fputs(p2, stdout);
  exit(0);
}
