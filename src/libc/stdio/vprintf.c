
/**********************************************************************
 *   Copyright (c) Digital Equipment Corporation 1984, 1985, 1986.    *
 *   All Rights Reserved. 					      *
 *   Reference "/usr/src/COPYRIGHT" for applicable restrictions.      *
 **********************************************************************/

/*
 * SCCSID: @(#)vprintf.c	3.0	4/22/86
 */
/*	(System 5)  vprintf.c  1.1  */
/*LINTLIBRARY*/
#include <stdio.h>
#include <varargs.h>

extern int _doprnt();

/*VARARGS1*/
/**
 * @brief Print formatted output using a va_list.
 *
 * Behaves like printf but accepts a variable argument list
 * rather than a variable number of arguments.
 *
 * @param format Format string.
 * @param ap     Argument list initialized with va_start.
 * @return Number of characters printed or EOF on error.
 */
int vprintf(format, ap)
char *format;
va_list ap;
{
  register int count;

  if (!(stdout->_flag | _IOWRT)) {
    /* if no write flag */
    if (stdout->_flag | _IORW) {
      /* if ok, cause read-write */
      stdout->_flag |= _IOWRT;
    } else {
      /* else error */
      return EOF;
    }
  }
  count = _doprnt(format, ap, stdout);
  return (ferror(stdout) ? EOF : count);
}
