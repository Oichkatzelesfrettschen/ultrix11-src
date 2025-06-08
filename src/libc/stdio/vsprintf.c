
/**********************************************************************
 *   Copyright (c) Digital Equipment Corporation 1984, 1985, 1986.    *
 *   All Rights Reserved. 					      *
 *   Reference "/usr/src/COPYRIGHT" for applicable restrictions.      *
 **********************************************************************/

/*
 * SCCSID: @(#)vsprintf.c	3.0	4/22/86
 */
/*	(System 5)  vsprintf.c  1.1 */
/*LINTLIBRARY*/
#include <stdio.h>
#include <values.h>
#include <varargs.h>

extern int _doprnt();

/*VARARGS2*/
/**
 * @brief Format a string using a variable argument list.
 *
 * Writes formatted output to @p string according to
 * @p format using the argument list @p ap.
 *
 * @param string Destination buffer for the formatted string.
 * @param format printf-style format string.
 * @param ap     Argument list initialized with va_start.
 * @return Number of characters written (excluding the null terminator).
 */
int vsprintf(string, format, ap)
char *string, *format;
va_list ap;
{
  register int count;
  FILE siop;

  siop._cnt = MAXINT;
  siop._base = siop._ptr = (unsigned char *)string;
  siop._flag = _IOWRT;
  siop._file = _NFILE;
  count = _doprnt(format, ap, &siop);
  *siop._ptr = '\0'; /* plant terminating null character */
  return (count);
}
