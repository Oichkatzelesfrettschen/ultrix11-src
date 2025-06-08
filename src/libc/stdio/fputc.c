
/**********************************************************************
 *   Copyright (c) Digital Equipment Corporation 1984, 1985, 1986.    *
 *   All Rights Reserved. 					      *
 *   Reference "/usr/src/COPYRIGHT" for applicable restrictions.      *
 **********************************************************************/

/*
 * SCCSID: @(#)fputc.c	3.0	4/22/86
 */
#include <stdio.h>

/**
 * @brief Write a character to a FILE stream.
 *
 * @param c   Character to write.
 * @param fp  Output stream.
 * @return The character written or EOF on error.
 */
fputc(c, fp) FILE *fp;
{
  return (putc(c, fp));
}
