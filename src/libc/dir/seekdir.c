
/**********************************************************************
 *   Copyright (c) Digital Equipment Corporation 1984, 1985, 1986.    *
 *   All Rights Reserved. 					      *
 *   Reference "/usr/src/COPYRIGHT" for applicable restrictions.      *
 **********************************************************************/

/*
 * static char sccsid[] = "@(#)seekdir.c	3.0	4/22/86";
 */

#include "odir.h"
#include <ndir.h>
#include <sys/param.h>

/**
 * @brief Seek to a directory entry position.
 *
 * Repositions the stream to @p loc previously returned by
 * ::telldir. This routine will read forward through the
 * directory until the requested offset is reached.
 *
 * @param dirp Directory descriptor returned by ::opendir.
 * @param loc  Value obtained from ::telldir.
 */
void seekdir(dirp, loc) register DIR *dirp;
long loc;
{
  long base, offset;
  struct direct *dp;

  /* rti!trt: Always seek.  Slower, but safer. This may even fix a bug.
          if (loc == telldir(dirp))
                  return;
   */
  base = (loc / RDSZ) * RDSZ;
  offset = (loc % RDSZ) / sizeof(struct olddirect) * OENTSIZ;
  lseek(dirp->dd_fd, base, 0);
  dirp->dd_loc = 0;
  while (dirp->dd_loc < offset) {
    dp = readdir(dirp);
    if (dp == NULL)
      return;
  }
}
