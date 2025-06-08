
/**********************************************************************
 *   Copyright (c) Digital Equipment Corporation 1984, 1985, 1986.    *
 *   All Rights Reserved. 					      *
 *   Reference "/usr/src/COPYRIGHT" for applicable restrictions.      *
 **********************************************************************/

/*
 * SCCSID: @(#)ungetc.c	3.0	4/22/86
 */
#include <stdio.h>

/**
 * @brief Push a character back onto an input stream.
 *
 * @param c   Character to push back.
 * @param iop Stream to modify.
 * @return The character pushed back or EOF on error.
 */
ungetc(c, iop) register FILE *iop;
{
  if (c == EOF)
    return (EOF);
  if ((iop->_flag & _IOREAD) == 0 || iop->_ptr <= iop->_base)
    if (iop->_ptr == iop->_base && iop->_cnt == 0)
      *iop->_ptr++;
    else
      return (EOF);
  iop->_cnt++;
  *--iop->_ptr = c;
  return (c);
}
