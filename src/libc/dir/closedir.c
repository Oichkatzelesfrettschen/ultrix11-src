
/**********************************************************************
 *   Copyright (c) Digital Equipment Corporation 1984, 1985, 1986.    *
 *   All Rights Reserved. 					      *
 *   Reference "/usr/src/COPYRIGHT" for applicable restrictions.      *
 **********************************************************************/

/*
 * static char sccsid[] = "@(#)closedir.c	3.0	4/22/86";
 */

#include <ndir.h>
#include <sys/types.h>

/**
 * @brief Close a directory stream.
 *
 * Releases all resources associated with @p dirp and
 * closes the underlying file descriptor.
 *
 * @param dirp Directory stream to close.
 */
void closedir(dirp) register DIR *dirp;
{
  close(dirp->dd_fd);
  dirp->dd_fd = -1;
  dirp->dd_loc = 0;
  free(dirp);
}
