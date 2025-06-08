/**********************************************************************
 *   Copyright (c) Digital Equipment Corporation 1984, 1985, 1986.
 *   All Rights Reserved.
 *   Reference "/usr/src/COPYRIGHT" for applicable restrictions.
 **********************************************************************/
/**
 * @file ac.c
 * @brief User accounting report tool.
 */

/**
 * @file ac.c
 * @brief User accounting utility: processes wtmp records to report
 *        user login times per terminal and per user.
 *
 * Usage: acct [ -w wtmp ] [ -d ] [ -p ] [ people ]
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/timeb.h>
#include <sys/types.h>
#include <time.h>
#include <utmp.h>

#define TSIZE 33  /**< Number of terminal slots to track. */
#define USIZE 200 /**< Maximum number of users tracked. */

/**
 * @struct ubuf
 * @brief Holds per-user accounting info.
 */
struct ubuf {
  char uname[8]; /**< User login name (null-terminated). */
  time_t utime;  /**< Cumulative time (in seconds) used by the user. */
};

/**
 * @struct tbuf
 * @brief Per-terminal accounting totals.
 */
struct tbuf {
  struct ubuf *userp; /**< Pointer to the user's ubuf entry. */
  time_t ttime;       /**< Last timestamp recorded for the terminal. */
};

static struct utmp ibuf;        /**< Current wtmp record buffer. */
static struct ubuf ubuf[USIZE]; /**< Array of per-user entries. */
static struct tbuf tbuf[TSIZE]; /**< Array of per-terminal entries. */

static char *wtmp;      /**< Path to wtmp file. */
static int pflag;       /**< Print per-user details if non-zero. */
static int byday;       /**< Split report by day if non-zero. */
static int pcount;      /**< Number of specific users to report. */
static char **pptr;     /**< List of user names to include. */
static time_t dtime;    /**< Timestamp of '|' record (begin day marker). */
static time_t midnight; /**< Timestamp of next midnight boundary. */
static time_t lastime;  /**< Timestamp of last processed record. */
static const time_t day = 86400L; /**< Number of seconds in a day. */

/* Function prototypes */
static void loop(void);
static void print_totals(void);
static void upall(int use_midnight);
static void update_term(struct tbuf *tp, int use_midnight);
static int among(int idx);
static void newday(void);
static void print_date(void);

/**
 * @brief Program entry point.
 *
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return EXIT_SUCCESS on success, EXIT_FAILURE on error.
 */
int main(int argc, char **argv) {
  int idx = 1;
  FILE *wf;

  wtmp = WTMP_FILE;
  while (idx < argc && argv[idx][0] == '-') {
    if (strcmp(argv[idx], "-d") == 0) {
      byday = 1;
    } else if (strcmp(argv[idx], "-p") == 0) {
      pflag = 1;
    } else if (strcmp(argv[idx], "-w") == 0 && idx + 1 < argc) {
      wtmp = argv[++idx];
    } else {
      fprintf(stderr, "Usage: acct [ -w wtmp ] [ -d ] [ -p ] [ people ]\n");
      return EXIT_FAILURE;
    }
    idx++;
  }
  pcount = argc - idx;
  pptr = &argv[idx];

  wf = fopen(wtmp, "r");
  if (wf == NULL) {
    perror(wtmp);
    return EXIT_FAILURE;
  }

  while (fread(&ibuf, sizeof(ibuf), 1, wf) == 1) {
    int fl = 0;
    for (int i = 0; i < (int)sizeof(ibuf.ut_name); i++) {
      int c = ibuf.ut_name[i];
      if (isdigit(c) || isalpha(c)) {
        if (fl) {
          fl = -1;
          break;
        }
      } else if (c == ' ' || c == '\0') {
        fl++;
        ibuf.ut_name[i] = '\0';
      } else {
        fl = -1;
        break;
      }
    }
    if (fl >= 0)
      loop();
  }

  /* Handle end-of-file "logout all" record */
  ibuf.ut_name[0] = '\0';
  ibuf.ut_line[0] = '~';
  time(&ibuf.ut_time);
  loop();

  print_totals();
  return EXIT_SUCCESS;
}

/**
 * @brief Process a single wtmp log entry.
 */
static void loop(void) {
  if (ibuf.ut_line[0] == '|') {
    dtime = ibuf.ut_time;
    return;
  }
  if (ibuf.ut_line[0] == '}') {
    if (dtime == 0)
      return;
    for (int i = 0; i < TSIZE; i++)
      tbuf[i].ttime += ibuf.ut_time - dtime;
    dtime = 0;
    return;
  }
  if (lastime > ibuf.ut_time || lastime + (time_t)(1.5 * day) < ibuf.ut_time)
    midnight = 0;
  if (midnight == 0)
    newday();
  lastime = ibuf.ut_time;

  if (byday && ibuf.ut_time > midnight) {
    upall(1);
    print_totals();
    newday();
    for (int i = 0; i < USIZE; i++)
      ubuf[i].utime = 0;
  }

  if (ibuf.ut_line[0] == '~') {
    ibuf.ut_name[0] = '\0';
    upall(0);
    return;
  }

  int idx;
  if (ibuf.ut_line[0] == 't')
    idx = (ibuf.ut_line[3] - '0') * 10 + (ibuf.ut_line[4] - '0');
  else
    idx = TSIZE - 1;
  if (idx < 0 || idx >= TSIZE)
    idx = TSIZE - 1;

  update_term(&tbuf[idx], 0);
}

/**
 * @brief Display the accumulated accounting totals.
 */
static void print_totals(void) {
  time_t total_time = 0;
  for (int i = 0; i < USIZE; i++) {
    if (!among(i))
      continue;
    time_t t = ubuf[i].utime;
    if (t > 0)
      total_time += t;
    if (pflag && t > 0) {
      printf("  %-8.8s %6.2f\n", ubuf[i].uname, (double)t / 3600.0);
    }
  }
  if (total_time > 0) {
    print_date();
    printf("  total   %6.2f\n", (double)total_time / 3600.0);
  }
}

/**
 * @brief Update all per-terminal totals.
 *
 * @param use_midnight If non-zero, use the midnight boundary time.
 */
static void upall(int use_midnight) {
  for (int i = 0; i < TSIZE; i++)
    update_term(&tbuf[i], use_midnight);
}

/**
 * @brief Update totals for a specific terminal.
 *
 * @param tp             Terminal buffer to update.
 * @param use_midnight   If non-zero, use the midnight boundary time.
 */
static void update_term(struct tbuf *tp, int use_midnight) {
  time_t now = use_midnight ? midnight : ibuf.ut_time;
  if (tp->userp) {
    time_t delta = now - tp->ttime;
    if (delta > 0 && delta < (time_t)(1.5 * day))
      tp->userp->utime += delta;
  }
  tp->ttime = now;
  if (use_midnight)
    return;
  if (ibuf.ut_name[0] == '\0') {
    tp->userp = NULL;
    return;
  }

  /* Find or allocate user entry */
  struct ubuf *up = NULL;
  for (int i = 0; i < USIZE; i++) {
    if (ubuf[i].uname[0] == '\0' ||
        strncmp(ubuf[i].uname, ibuf.ut_name, sizeof(ubuf[i].uname)) == 0) {
      up = &ubuf[i];
      break;
    }
  }
  if (up == NULL)
    return;

  /* Initialize or confirm user name */
  strncpy(up->uname, ibuf.ut_name, sizeof(up->uname));
  up->uname[sizeof(up->uname) - 1] = '\0';

  tp->userp = up;
}

/**
 * @brief Determine whether a user index is in the report list.
 *
 * @param idx Index of user buffer.
 * @return 1 if listed or list empty, 0 otherwise.
 */
static int among(int idx) {
  if (pcount == 0)
    return 1;
  for (int j = 0; j < pcount; j++) {
    if (strncmp(pptr[j], ubuf[idx].uname, strlen(pptr[j])) == 0)
      return 1;
  }
  return 0;
}

/**
 * @brief Advance to the next midnight boundary.
 */
static void newday(void) {
  struct timeb tb;
  if (midnight == 0) {
    ftime(&tb);
    midnight = tb.timezone * 60;
    if (localtime(&lastime)->tm_isdst)
      midnight -= 3600;
  }
  while (midnight <= ibuf.ut_time)
    midnight += day;
}

/**
 * @brief Print the date header when reporting by day.
 */
static void print_date(void) {
  if (!byday)
    return;
  time_t x = midnight - 1;
  char *s = ctime(&x);
  printf("%.6s", s + 4);
}
