
/**********************************************************************
 *   Copyright (c) Digital Equipment Corporation 1984, 1985, 1986.    *
 *   All Rights Reserved. 					      *
 *   Reference "/usr/src/COPYRIGHT" for applicable restrictions.      *
 **********************************************************************/
/**
 * @file accton.c
 * @brief Enable or disable system accounting.
 */

static char Sccsid[] = "@(#)accton.c 3.0 4/21/86";
/**
 * @brief Program entry point.
 *
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return EXIT_SUCCESS on success.
 */
int main(int argc, char **argv) {
  extern errno;
  if (argc > 1)
    acct(argv[1]);
  else
    acct((char *)0);
  if (errno) {
    perror("accton");
    exit(1);
  }
  exit(0);
}
