
/**********************************************************************
 *   Copyright (c) Digital Equipment Corporation 1984, 1985, 1986.    *
 *   All Rights Reserved. 					      *
 *   Reference "/usr/src/COPYRIGHT" for applicable restrictions.      *
 **********************************************************************/

/* SCCSID: @(#)move.c	3.0	4/22/86 */
#include "curses.ext"

/*
 *	This routine moves the cursor to the given point
 *
 * 1/26/81 (Berkeley) @(#)move.c	1.1
 */
/**
 * @brief Move the cursor in a window.
 *
 * @param win Window to operate on.
 * @param y   Row index.
 * @param x   Column index.
 * @return OK on success, ERR on failure.
 */
int wmove(WINDOW *win, int y, int x)

#ifdef DEBUG
    fprintf(outf, "MOVE to (%d, %d)\n", y, x);
#endif
if (x >= win->_maxx || y >= win->_maxy)
  return ERR;
win->_curx = x;
win->_cury = y;
return OK;
}
