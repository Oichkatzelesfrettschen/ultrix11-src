
/**********************************************************************
 *   Copyright (c) Digital Equipment Corporation 1984, 1985, 1986.    *
 *   All Rights Reserved. 					      *
 *   Reference "/usr/src/COPYRIGHT" for applicable restrictions.      *
 **********************************************************************/

/*	SCCSID: @(#)getgrent.c	3.0	4/22/86	*/
/*	(System 5)  getgrent.c	1.4	*/
/*	3.0 SID #	1.2	*/
/*LINTLIBRARY*/
#include <grp.h>
#include <stdio.h>

#define CL ':'
#define CM ','
#define NL '\n'
#define MAXGRP 100

extern int atoi(), fclose();
extern char *fgets();
extern FILE *fopen();
extern void rewind();

static char GROUP[] = "/etc/group"; /**< Path to the group database. */
static FILE *grf = NULL;            /**< Cached FILE pointer. */
static char line[BUFSIZ + 1];       /**< Temporary line buffer. */
static struct group grp;            /**< Current group entry. */
static char *gr_mem[MAXGRP];        /**< Member list pointers. */

/**
 * @brief Rewind the group file pointer.
 */
void setgrent() {
  if (grf == NULL)
    grf = fopen(GROUP, "r");
  else
    rewind(grf);
}

/**
 * @brief Close the group file if open.
 */
void endgrent() {
  if (grf != NULL) {
    (void)fclose(grf);
    grf = NULL;
  }
}

/**
 * @brief Skip characters in a string until @p c or NUL.
 */
static char *grskip(p, c)
register char *p;
register int c;
{
  while (*p != '\0' && *p != c)
    ++p;
  if (*p != '\0')
    *p++ = '\0';
  return (p);
}

/**
 * @brief Retrieve the next group entry.
 */
struct group *getgrent() {
  extern struct group *fgetgrent();

  if (grf == NULL && (grf = fopen(GROUP, "r")) == NULL)
    return (NULL);
  return (fgetgrent(grf));
}

/**
 * @brief Internal helper to read a group entry from @p f.
 */
struct group *fgetgrent(f)
FILE *f;
{
  register char *p, **q;

  if ((p = fgets(line, BUFSIZ, f)) == NULL)
    return (NULL);
  grp.gr_name = p;
  grp.gr_passwd = p = grskip(p, CL);
  grp.gr_gid = atoi(p = grskip(p, CL));
  grp.gr_mem = gr_mem;
  p = grskip(p, CL);
  (void)grskip(p, NL);
  q = gr_mem;
  while (*p != '\0') {
    *q++ = p;
    p = grskip(p, CM);
  }
  *q = NULL;
  return (&grp);
}
