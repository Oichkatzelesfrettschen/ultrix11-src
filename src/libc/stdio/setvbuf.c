
/**********************************************************************
 *   Copyright (c) Digital Equipment Corporation 1984, 1985, 1986.    *
 *   All Rights Reserved. 					      *
 *   Reference "/usr/src/COPYRIGHT" for applicable restrictions.      *
 **********************************************************************/

/*
 * SCCSID: @(#)setvbuf.c	3.0	4/22/86
 */
/*	(System 5)  setvbuf.c  1.2  */
/*LINTLIBRARY*/
#include "stdio.h"

extern void free();
extern int isatty();

/**
 * @brief Configure buffering for a FILE stream.
 *
 * Adjusts the buffering mode and buffer size for @p iop.
 * The @p type parameter uses the standard _IONBF, _IOLBF or
 * _IOFBF values.
 *
 * @param iop   Stream to configure.
 * @param type  Buffering mode.
 * @param buf   Caller supplied buffer or NULL.
 * @param size  Size of @p buf if provided.
 * @return Zero on success, non-zero on failure.
 */
int setvbuf(iop, type, buf, size)
register FILE *iop;
register int type;
register char *buf;
register int size;
{
  char *malloc();

  if (iop->_base != NULL && iop->_flag & _IOMYBUF)
    free((char *)iop->_base);
  iop->_flag &= ~(_IOMYBUF | _IONBF | _IOLBF);
  switch (type) {
  /*note that the flags are the same as the possible values for type*/
  case _IONBF:
    /* file is unbuffered */
    iop->_flag |= _IONBF;
    _bufend(iop) = iop->_base = NULL;
    break;
  case _IOLBF:
  case _IOFBF:
    iop->_flag |= type;
    size = (size == 0) ? BUFSIZ : size;
    if ((size > BUFSIZ) || (iop->_base == NULL))
      iop->_base = (unsigned char *)malloc((unsigned)(size + 8));
    _bufend(iop) = iop->_base + size;
    break;
  default:
    return -1;
  }
  iop->_ptr = iop->_base;
  iop->_cnt = 0;
  return 0;
}
