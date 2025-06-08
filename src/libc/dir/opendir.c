
/**********************************************************************
 *   Copyright (c) Digital Equipment Corporation 1984, 1985, 1986.    *
 *   All Rights Reserved. 					      *
 *   Reference "/usr/src/COPYRIGHT" for applicable restrictions.      *
 **********************************************************************/

/*
 * static char sccsid[] = "@(#)opendir.c	3.0	4/22/86";
 */

#include <ndir.h>
#include <sys/stat.h>
#include <sys/types.h>

/**
 * @brief Open a directory for reading.
 *
 * This routine allocates a new DIR structure and opens the
 * directory referenced by @p name. The returned structure
 * tracks the file descriptor and current offset used by the
 * directory traversal routines.
 *
 * @param name Path of the directory to open.
 * @return Pointer to a DIR descriptor or NULL on failure.
 */
DIR *opendir(name)
char *name;
{
  register DIR *dirp;
  register int fd;
  struct stat sbuf;

  if ((fd = open(name, 0)) == -1)
    return NULL;
  fstat(fd, &sbuf);
  if (((sbuf.st_mode & S_IFDIR) == 0) ||
      ((dirp = (DIR *)malloc(sizeof(DIR))) == NULL)) {
    close(fd);
    return NULL;
  }
  dirp->dd_fd = fd;
  dirp->dd_loc = 0;
  return dirp;
}
