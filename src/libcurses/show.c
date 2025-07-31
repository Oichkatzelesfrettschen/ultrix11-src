
/**********************************************************************
 *   Copyright (c) Digital Equipment Corporation 1984, 1985, 1986.    *
 *   All Rights Reserved. 					      *
 *   Reference "/usr/src/COPYRIGHT" for applicable restrictions.      *
 **********************************************************************/

/* SCCSID: @(#)show.c	3.0	4/22/86 */
#include <curses.h>
#include <signal.h>

#undef LINES
#define LINES 20

static void done(void);

/**
 * @brief Display a file one screen at a time.
 *
 * Equivalent to a very small "more" utility. The program reads the
 * specified file and shows it using curses. Pressing 'q' quits.
 *
 * @param argc Argument count from the command line.
 * @param argv Argument vector from the command line.
 * @return int Program exit status.
 */
int main(int argc, char **argv) {
  FILE *fd;
  char linebuf[512];
  int line;

  if (argc < 2) {
    (void)fprintf(stderr, "Usage: show file\n");
    exit(1);
  }
  fd = fopen(argv[1], "r");
  if (fd == NULL) {
    perror(argv[1]);
    exit(2);
  }
  (void)signal(SIGINT, done); /* die gracefully */

  initscr(); /* initialize curses */
  noecho();  /* turn off tty echo */
  crmode();  /* enter cbreak mode */

  for (;;) { /* for each screen full */
    (void)move(0, 0);
    /* werase(stdscr); */
    for (line = 0; line < LINES; line++) {
      if (fgets(linebuf, sizeof linebuf, fd) == NULL) {
        clrtobot();
        done();
      }
      (void)mvprintw(line, 0, "%s", linebuf);
    }
    (void)refresh(); /* sync screen */
    printf(" --more-- [q to quit]\n");
    if (getch() == 'q') /* wait for user to read it */
      done();
  }
}

/**
 * @brief Clean up the curses session and exit.
 */
void done(void) {
  (void)move(LINES - 1, 0); /* to lower left corner */
  clrtoeol();               /* clear bottom line */
  (void)refresh();          /* flush out everything */
  endwin();                 /* curses cleanup */
  exit(0);
}
