
/**********************************************************************
 *   Copyright (c) Digital Equipment Corporation 1984, 1985, 1986.    *
 *   All Rights Reserved. 					      *
 *   Reference "/usr/src/COPYRIGHT" for applicable restrictions.      *
 **********************************************************************/

/*
 * SCCSID: @(#)popen.c	3.0	4/22/86
 */
#include <signal.h>
#include <stdio.h>
#define tst(a, b) (*mode == 'r' ? (b) : (a))
#define RDR 0
#define WTR 1
static int popen_pid[20];
/**
 * @brief Open a pipe to a subprocess.
 *
 * Spawns `/bin/sh -c cmd` and returns a stream connected to
 * either its standard input or output depending on @p mode.
 *
 * @param cmd Command to execute via the shell.
 * @param mode Either "r" or "w" indicating read or write.
 * @return `FILE` pointer for interacting with the process or NULL on error.
 */

FILE *popen(cmd, mode)
char *cmd;
char *mode;
{
  int p[2];
  register myside, hisside, pid;

  if (pipe(p) < 0)
    return NULL;
  myside = tst(p[WTR], p[RDR]);
  hisside = tst(p[RDR], p[WTR]);
  if ((pid = fork()) == 0) {
    /* myside and hisside reverse roles in child */
    close(myside);
    dup2(hisside, tst(0, 1));
    close(hisside);
    execl("/bin/sh", "sh", "-c", cmd, 0);
    _exit(1);
  }
  if (pid == -1) {
    close(myside);
    close(hisside);
    return NULL;
  }
  popen_pid[myside] = pid;
  close(hisside);
  return (fdopen(myside, mode));
}

pclose(ptr) FILE *ptr;
{
  register f, r, (*hstat)(), (*istat)(), (*qstat)();
  int status;

  f = fileno(ptr);
  fclose(ptr);
  istat = signal(SIGINT, SIG_IGN);
  qstat = signal(SIGQUIT, SIG_IGN);
  hstat = signal(SIGHUP, SIG_IGN);
  while ((r = wait(&status)) != popen_pid[f] && r != -1)
    ;
  if (r == -1)
    status = -1;
  signal(SIGINT, istat);
  signal(SIGQUIT, qstat);
  signal(SIGHUP, hstat);
  return (status);
}
