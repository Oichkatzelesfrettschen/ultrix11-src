
/**********************************************************************
 *   Copyright (c) Digital Equipment Corporation 1984, 1985, 1986.    *
 *   All Rights Reserved. 					      *
 *   Reference "/usr/src/COPYRIGHT" for applicable restrictions.      *
 **********************************************************************/

/*
 * SCCSID: @(#)fwrite.c	3.0	4/22/86
 */
/*	(System 5)  fwrite.c  3.6  */
/*LINTLIBRARY*/
/*
 * This version writes directly to the buffer rather than looping on putc.
 * Ptr args aren't checked for NULL because the program would be a
 * catastrophic mess anyway.  Better to abort than just to return NULL.
 *
 * This version does buffered writes larger than BUFSIZ directly, when
 * the buffer is empty.
 */
#include "stdiom.h"
#include <stdio.h>

#define MIN(x, y) (x < y ? x : y)

extern char *memcpy();

/**
 * @brief Write an array of objects to a stream.
 *
 * @param ptr   Buffer of data to write.
 * @param size  Size of each object in bytes.
 * @param count Number of objects to write.
 * @param iop   Output stream.
 * @return Number of objects successfully written.
 */
int fwrite(ptr, size, count, iop)
char *ptr;
int size, count;
register FILE *iop;
{
  register unsigned nleft;
  register unsigned n;
  register unsigned char *cptr, *bufend;

  if (size <= 0 || count <= 0 || _WRTCHK(iop))
    return (0);

  bufend = _bufend(iop);
  nleft = count * size;

  /* if the file is unbuffered, or if the iop->ptr = iop->base, and there
     is > BUFSZ chars to write, we can do a direct write */
  if (iop->_base >= iop->_ptr) { /*this covers the unbuffered case, too*/
    if (((iop->_flag & _IONBF) != 0) || (nleft >= BUFSIZ)) {
      if ((n = write(fileno(iop), ptr, nleft)) != nleft) {
        iop->_flag |= _IOERR;
        n = (n >= 0) ? n : 0;
      }
      return n / size;
    }
  }
  /* Put characters in the buffer */
  /* note that the meaning of n when just starting this loop is
     irrelevant.  It is defined in the loop */
  for (;; ptr += n) {
    while ((n = bufend - (cptr = iop->_ptr)) <= 0) /* full buf */
      if (_xflsbuf(iop) == EOF)
        return (count - (nleft + size - 1) / size);
    n = MIN(nleft, n);
    (void)memcpy((char *)cptr, ptr, n);
    iop->_cnt -= n;
    iop->_ptr += n;
    _BUFSYNC(iop);
    /* done; flush if linebuffered with a newline */
    if ((nleft -= n) == 0) {
      if (iop->_flag & (_IOLBF | _IONBF)) {
        if ((iop->_flag & _IONBF) ||
            (memchr(iop->_base, '\n', count * size) != NULL)) {
          (void)_xflsbuf(iop);
        }
      }
      return (count);
    }
  }
}
