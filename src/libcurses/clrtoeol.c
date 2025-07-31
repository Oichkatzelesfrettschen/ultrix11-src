
/**********************************************************************
 *   Copyright (c) Digital Equipment Corporation 1984, 1985, 1986.    *
 *   All Rights Reserved. 					      *
 *   Reference "/usr/src/COPYRIGHT" for applicable restrictions.      *
 **********************************************************************/

/* SCCSID: @(#)clrtoeol.c	3.0	4/22/86 */
#include "curses.ext"

/*
 *	This routine clears up to the end of line
 *
 * 3/5/81 (Berkeley) @(#)clrtoeol.c	1.2
 */
/**
 * @brief Clear from the current position to the end of the line.
 *
 * @param win Window to operate on.
 */
void wclrtoeol(WINDOW *win)

    reg char *sp,
    *end;
reg int y, x;
reg char *maxx;
reg int minx;

y = win->_cury;
x = win->_curx;
end = &win->_y[y][win->_maxx];
minx = _NOCHANGE;
maxx = &win->_y[y][x];
for (sp = maxx; sp < end; sp++)
  if (*sp != ' ') {
    maxx = sp;
    if (minx == _NOCHANGE)
      minx = sp - win->_y[y];
    *sp = ' ';
  }
/*
 * update firstch and lastch for the line
 */
#ifdef DEBUG
fprintf(outf, "CLRTOEOL: minx = %d, maxx = %d, firstch = %d, lastch = %d\n",
        minx, maxx - win->_y[y], win->_firstch[y], win->_lastch[y]);
#endif
if (minx != _NOCHANGE) {
  if (win->_firstch[y] > minx || win->_firstch[y] == _NOCHANGE)
    win->_firstch[y] = minx;
  if (win->_lastch[y] < maxx - win->_y[y])
    win->_lastch[y] = maxx - win->_y[y];
}
}
