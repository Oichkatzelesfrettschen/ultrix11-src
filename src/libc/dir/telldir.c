
/**********************************************************************
 *   Copyright (c) Digital Equipment Corporation 1984, 1985, 1986.    *
 *   All Rights Reserved. 					      *
 *   Reference "/usr/src/COPYRIGHT" for applicable restrictions.      *
 **********************************************************************/

/*
 * static char sccsid[] = "@(#)telldir.c	3.0	4/22/86";
 */

#include "odir.h"
#include <ndir.h>
#include <sys/types.h>

extern long lseek(); /* needed for pdp 11s -- ikonas!mcm */

/**
 * @brief Obtain the current directory read offset.
 *
 * Calculates the position within the directory stream so it
 * can later be restored using ::seekdir.
 *
 * @param dirp Directory descriptor returned by ::opendir.
 * @return Offset suitable for ::seekdir.
 */
long telldir(dirp)
DIR *dirp;
{
  return (lseek(dirp->dd_fd, 0L, 1) -
          ((dirp->dd_size - dirp->dd_loc) / OENTSIZ) *
              sizeof(struct olddirect));
}
