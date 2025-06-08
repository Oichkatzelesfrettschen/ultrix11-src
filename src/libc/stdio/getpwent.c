
/**********************************************************************
 *   Copyright (c) Digital Equipment Corporation 1984, 1985, 1986.    *
 *   All Rights Reserved. 					      *
 *   Reference "/usr/src/COPYRIGHT" for applicable restrictions.      *
 **********************************************************************/

/*	SCCSID: @(#)getpwent.c	3.0	4/22/86	*/
/*	(System 5)  getpwent.c	1.7	*/
/*LINTLIBRARY*/
#include <pwd.h>
#include <stdio.h>
#include <sys/param.h>

extern void rewind();
extern FILE *fopen();
extern int fclose();
extern char *fgets();

static char PASSWD[] = "/etc/passwd"; /**< Path to passwd file. */
static char EMPTY[] = "";             /**< Placeholder string. */
static FILE *pwf = NULL;              /**< Cached FILE pointer. */
static char line[BUFSIZ + 1];         /**< Temporary line buffer. */
static struct passwd passwd;          /**< Current passwd entry. */

/**
 * @brief Rewind the passwd file pointer.
 */
void setpwent() {
  if (pwf == NULL)
    pwf = fopen(PASSWD, "r");
  else
    rewind(pwf);
}

/**
 * @brief Close the passwd file if open.
 */
void endpwent() {
  if (pwf != NULL) {
    (void)fclose(pwf);
    pwf = NULL;
  }
}

/**
 * @brief Skip a field within a passwd entry.
 */
static char *pwskip(p)
register char *p;
{
  while (*p && *p != ':' && *p != '\n')
    ++p;
  if (*p == '\n')
    *p = '\0';
  else if (*p)
    *p++ = '\0';
  return (p);
}

/**
 * @brief Retrieve the next passwd entry.
 */
struct passwd *getpwent() {
  extern struct passwd *fgetpwent();

  if (pwf == NULL) {
    if ((pwf = fopen(PASSWD, "r")) == NULL)
      return (NULL);
  }
  return (fgetpwent(pwf));
}

/**
 * @brief Internal helper to read a passwd entry from @p f.
 */
struct passwd *fgetpwent(f)
FILE *f;
{
  register char *p;

  p = fgets(line, BUFSIZ, f);
  if (p == NULL)
    return (NULL);
  passwd.pw_name = p;
  p = pwskip(p);
  passwd.pw_passwd = p;
  p = pwskip(p);
  if (p == NULL || *p == ':')
    /* check for non-null uid */
    return (NULL);
  passwd.pw_uid = atoi(p);
  p = pwskip(p);
  if (p == NULL || *p == ':')
    /* check for non-null uid */
    return (NULL);
  passwd.pw_gid = atoi(p);
  p = pwskip(p);
  passwd.pw_comment = EMPTY;
  passwd.pw_gecos = p;
  p = pwskip(p);
  passwd.pw_dir = p;
  p = pwskip(p);
  passwd.pw_shell = p;

  /*** No passwd aging code on ULTRIX-11
      (void) pwskip(p);

      p = passwd.pw_passwd;
      while(*p && *p != ',')
              p++;
      if(*p)
              *p++ = '\0';
      passwd.pw_age = p;
  ***/

  while (*p && *p != '\n')
    p++;
  *p = '\0';
  return (&passwd);
}
