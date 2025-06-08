
/**********************************************************************
 *   Copyright (c) Digital Equipment Corporation 1984, 1985, 1986.    *
 *   All Rights Reserved. 					      *
 *   Reference "/usr/src/COPYRIGHT" for applicable restrictions.      *
 **********************************************************************/

/*
 * SCCSID: @(#)fprintf.c	3.0	4/22/86
 */
/*	(System 5)  fprintf.c	1.5	*/
/*
 * Modified to return the value the old V7 routine returned,
 * to remain compatible, yet offer line buffering for speed.
 */
/*LINTLIBRARY*/
#include <stdio.h>
#include <varargs.h>

extern int _doprnt();

/*VARARGS2*/
/**
 * @brief Print formatted output to a stream.
 *
 * @param iop    Destination stream.
 * @param format Format string.
 * @param ...    Arguments referenced by @p format.
 * @return Number of characters printed or EOF on error.
 */
int fprintf(iop, format, va_alist)
FILE *iop;
char *format;
va_dcl {
  register int count;
  va_list ap;

  va_start(ap);
  if (!(iop->_flag | _IOWRT)) {
    /* if no write flag */
    if (iop->_flag | _IORW) {
      /* if ok, cause read-write */
      iop->_flag |= _IOWRT;
    } else {
      /* else error */
      return EOF;
    }
  }
  count = _doprnt(format, ap, iop);
  va_end(ap);
  return (ferror(iop) ? EOF : 0); /* SYSV: used to return count, not 0 */
}
