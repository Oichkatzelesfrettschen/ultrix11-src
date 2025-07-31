
/**********************************************************************
 *   Copyright (c) Digital Equipment Corporation 1984, 1985, 1986.    *
 *   All Rights Reserved. 					      *
 *   Reference "/usr/src/COPYRIGHT" for applicable restrictions.      *
 **********************************************************************/

/* SCCSID: @(#)addstr.c	3.0	4/22/86 */
#include "curses.ext"

/*
 *	This routine adds a string starting at (_cury,_curx)
 *
 * 1/26/81 (Berkeley) @(#)addstr.c	1.1
 */
/**
 * @brief Add a NUL-terminated string to the current cursor position.
 *
 * The string is written character by character using waddch().
 *
 * @param win Target window.
 * @param str String to display.
 * @return OK on success, ERR on failure.
 */
int waddstr(WINDOW *win, char *str) {
#ifdef DEBUG
  fprintf(outf, "WADDSTR(\"%s\")\n", str);
#endif
  while (*str)
    if (waddch(win, *str++) == ERR)
      return ERR;
  return OK;
}
