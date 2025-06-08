
/**********************************************************************
 *   Copyright (c) Digital Equipment Corporation 1984, 1985, 1986.    *
 *   All Rights Reserved. 					      *
 *   Reference "/usr/src/COPYRIGHT" for applicable restrictions.      *
 **********************************************************************/

/*
 * acct [ -w wtmp ] [ -d ] [ -p ] [ people ]
 */

static char Sccsid[] = "@(#)ac.c 3.0 4/21/86";
#include <ctype.h>
#include <stdio.h>
#include <sys/timeb.h>
#include <sys/types.h>
#include <time.h>
#include <utmp.h>

#define TSIZE 33  /**< Number of terminal slots. */
#define USIZE 200 /**< Maximum number of users tracked. */
struct utmp ibuf;

/**
 * @struct ubuf
 * @brief Holds per-user accounting info.
 */
struct ubuf {
  char uname[8];
  long utime;
} ubuf[USIZE];

/**
 * @struct tbuf
 * @brief Terminal usage entry.
 */
struct tbuf {
  struct ubuf *userp;
  long ttime;
} tbuf[TSIZE];

char *wtmp;
int pflag, byday;
long dtime;
long midnight;
long lastime;
long day = 86400L;
int pcount;
char **pptr;

/**
 * @brief Entry point for the ac utility.
 *
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return Exit status.
 */
main(argc, argv) char **argv;
{
  int c, fl;
  register i;
  FILE *wf;

  wtmp = WTMP_FILE;
  while (--argc > 0 && **++argv == '-')
    switch (*++*argv) {
    case 'd':
      byday++;
      continue;

    case 'w':
      if (--argc > 0)
        wtmp = *++argv;
      continue;

    case 'p':
      pflag++;
      continue;
    }
  pcount = argc;
  pptr = argv;
  if ((wf = fopen(wtmp, "r")) == NULL) {
    printf("No %s\n", wtmp);
    exit(1);
  }
  for (;;) {
    if (fread((char *)&ibuf, sizeof(ibuf), 1, wf) != 1)
      break;
    fl = 0;
    for (i = 0; i < 8; i++) {
      c = ibuf.ut_name[i];
      if (isdigit(c) || isalpha(c)) {
        if (fl)
          goto skip;
        continue;
      }
      if (c == ' ' || c == '\0') {
        fl++;
        ibuf.ut_name[i] = '\0';
      } else
        goto skip;
    }
    loop();
  skip:;
  }
  ibuf.ut_name[0] = '\0';
  ibuf.ut_line[0] = '~';
  time(&ibuf.ut_time);
  loop();
  print();
  exit(0);
}

/**
 * @brief Process a log entry.
 */
loop() {
  register i;
  register struct tbuf *tp;
  register struct ubuf *up;

  if (ibuf.ut_line[0] == '|') {
    dtime = ibuf.ut_time;
    return;
  }
  if (ibuf.ut_line[0] == '}') {
    if (dtime == 0)
      return;
    for (tp = tbuf; tp < &tbuf[TSIZE]; tp++)
      tp->ttime += ibuf.ut_time - dtime;
    dtime = 0;
    return;
  }
  if (lastime > ibuf.ut_time || lastime + (1.5 * day) < ibuf.ut_time)
    midnight = 0;
  if (midnight == 0)
    newday();
  lastime = ibuf.ut_time;
  if (byday && ibuf.ut_time > midnight) {
    upall(1);
    print();
    newday();
    for (up = ubuf; up < &ubuf[USIZE]; up++)
      up->utime = 0;
  }
  if (ibuf.ut_line[0] == '~') {
    ibuf.ut_name[0] = '\0';
    upall(0);
    return;
  }
  if (ibuf.ut_line[0] == 't')
    i = (ibuf.ut_line[3] - '0') * 10 + (ibuf.ut_line[4] - '0');
  else
    i = TSIZE - 1;
  if (i < 0 || i >= TSIZE)
    i = TSIZE - 1;
  tp = &tbuf[i];
  update(tp, 0);
}

/**
 * @brief Print accumulated totals.
 */
print() {
  int i;
  long ttime, t;

  ttime = 0;
  for (i = 0; i < USIZE; i++) {
    if (!among(i))
      continue;
    t = ubuf[i].utime;
    if (t > 0)
      ttime += t;
    if (pflag && ubuf[i].utime > 0) {
      printf("\t%-8.8s%6.2f\n", ubuf[i].uname, ubuf[i].utime / 3600.);
    }
  }
  if (ttime > 0) {
    pdate();
    printf("\ttotal%9.2f\n", ttime / 3600.);
  }
}

/**
 * @brief Update all terminal buffers.
 *
 * @param f Non-zero to use midnight instead of current time.
 */
upall(f) {
  register struct tbuf *tp;

  for (tp = tbuf; tp < &tbuf[TSIZE]; tp++)
    update(tp, f);
}

/**
 * @brief Update a terminal entry.
 *
 * @param tp Terminal buffer to update.
 * @param f Non-zero to use midnight instead of current time.
 */
update(tp, f) struct tbuf *tp;
{
  int j;
  struct ubuf *up;
  long t, t1;

  if (f)
    t = midnight;
  else
    t = ibuf.ut_time;
  if (tp->userp) {
    t1 = t - tp->ttime;
    if (t1 > 0 && t1 < 1.5 * day)
      tp->userp->utime += t1;
  }
  tp->ttime = t;
  if (f)
    return;
  if (ibuf.ut_name[0] == '\0') {
    tp->userp = 0;
    return;
  }
  for (up = ubuf; up < &ubuf[USIZE]; up++) {
    if (up->uname[0] == '\0')
      break;
    for (j = 0; j < 8 && up->uname[j] == ibuf.ut_name[j]; j++)
      ;
    if (j >= 8)
      break;
  }
  for (j = 0; j < 8; j++)
    up->uname[j] = ibuf.ut_name[j];
  tp->userp = up;
}

/**
 * @brief Check if user is in the specified list.
 *
 * @param i Index of user buffer.
 * @return 1 if listed or list empty, 0 otherwise.
 */
among(i) {
  register j, k;
  register char *p;

  if (pcount == 0)
    return (1);
  for (j = 0; j < pcount; j++) {
    p = pptr[j];
    for (k = 0; k < 8; k++) {
      if (*p == ubuf[i].uname[k]) {
        if (*p++ == '\0')
          return (1);
      } else
        break;
    }
  }
  return (0);
}

/**
 * @brief Initialize or advance to a new day.
 */
newday() {
  long ttime;
  struct timeb tb;
  struct tm *localtime();

  time(&ttime);
  if (midnight == 0) {
    ftime(&tb);
    midnight = 60 * (long)tb.timezone;
    if (localtime(&ttime)->tm_isdst)
      midnight -= 3600;
  }
  while (midnight <= ibuf.ut_time)
    midnight += day;
}

/**
 * @brief Print the date header when displaying totals.
 */
pdate() {
  long x;
  char *ctime();

  if (byday == 0)
    return;
  x = midnight - 1;
  printf("%.6s", ctime(&x) + 4);
}
