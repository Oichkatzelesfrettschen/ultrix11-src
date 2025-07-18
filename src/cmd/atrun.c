
/**********************************************************************
 *   Copyright (c) Digital Equipment Corporation 1984, 1985, 1986.    *
 *   All Rights Reserved. 					      *
 *   Reference "/usr/src/COPYRIGHT" for applicable restrictions.      *
 **********************************************************************/
/**
 * @file atrun.c
 * @brief Run jobs scheduled by at.
 */

/*
 * Run programs submitted by at.
 */
static char Sccsid[] = "@(#)atrun.c 3.0 4/21/86";
#include <stdio.h>
#include <sys/dir.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

#define DIR "/usr/spool/at"
#define PDIR "past"
#define LASTF "/usr/spool/at/lasttimedone"

int nowtime;
int nowdate;
int nowyear;

/**
 * @brief Program entry point.
 *
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return EXIT_SUCCESS on success.
 */
int main(int argc, char **argv) {
  int tt, day, year, uniq;
  struct direct dirent;
  char file[DIRSIZ + 1];
  FILE *dirf;

  chdir(DIR);
  makenowtime();
  if ((dirf = fopen(".", "r")) == NULL) {
    fprintf(stderr, "Cannot read at directory\n");
    exit(1);
  }
  while (fread((char *)&dirent, sizeof(dirent), 1, dirf) == 1) {
    if (dirent.d_ino == 0)
      continue;
    strncpy(file, dirent.d_name, DIRSIZ);
    file[DIRSIZ] = '\0';
    if (sscanf(file, "%2d.%3d.%4d.%2d", &year, &day, &tt, &uniq) != 4)
      continue;
    if (nowyear < year)
      continue;
    if (nowyear == year && nowdate < day)
      continue;
    if (nowyear == year && nowdate == day && nowtime < tt)
      continue;
    run(file);
  }
  fclose(dirf);
  updatetime(nowtime);
  exit(0);
}

makenowtime() {
  long t;
  struct tm *localtime();
  register struct tm *tp;

  time(&t);
  tp = localtime(&t);
  nowtime = tp->tm_hour * 100 + tp->tm_min;
  nowdate = tp->tm_yday;
  nowyear = tp->tm_year;
}

updatetime(t) {
  FILE *tfile;

  tfile = fopen(LASTF, "w");
  if (tfile == NULL) {
    fprintf(stderr, "can't write lastfile\n");
    exit(1);
  }
  fprintf(tfile, "%04d\n", t);
}

run(file) char *file;
{
  struct stat stbuf;
  register pid, i;
  char sbuf[64];

  if (fork() != 0)
    return;
  for (i = 0; i < 15; i++)
    close(i);
  dup(dup(open("/dev/null", 0)));
  sprintf(sbuf, "/bin/mv %.14s %s", file, PDIR);
  system(sbuf);
  chdir(PDIR);
  if (stat(file, &stbuf) == -1)
    exit(1);
  setgid(stbuf.st_gid);
  setuid(stbuf.st_uid);
  if (pid = fork()) {
    if (pid == -1)
      exit(1);
    wait((int *)0);
    unlink(file);
    exit(0);
  }
  nice(3);
  execl("/bin/sh", "sh", file, 0);
  execl("/usr/bin/sh", "sh", file, 0);
  fprintf(stderr, "Can't execl shell\n");
  exit(1);
}
