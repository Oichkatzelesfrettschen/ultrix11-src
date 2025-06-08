
/**********************************************************************
 *   Copyright (c) Digital Equipment Corporation 1984, 1985, 1986.    *
 *   All Rights Reserved. 					      *
 *   Reference "/usr/src/COPYRIGHT" for applicable restrictions.      *
 **********************************************************************/

/*
 * SCCSID: @(#)getw.c	3.0	4/22/86
 */
/*	(System 5)  getw.c	1.3	*/
/*	3.0 SID #	1.2	*/
/*LINTLIBRARY*/

/*
 * The intent here is to provide a means to make the order of
 * bytes in an io-stream correspond to the order of the bytes
 * in the memory while doing the io a `word' at a time.
 */
#include <stdio.h>

/**
 * @brief Read a word from a stream.
 *
 * Reads a machine word worth of bytes from @p stream and
 * assembles them into an integer using the host byte order.
 *
 * @param stream Stream to read from.
 * @return The word read or EOF on error.
 */
int getw(stream)
register FILE *stream;
{
  int w;
  register char *s = (char *)&w;
  register int i = sizeof(int);

  while (--i >= 0)
    *s++ = getc(stream);
  return (feof(stream) || ferror(stream) ? EOF : w);
}
