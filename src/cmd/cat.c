
/**********************************************************************
 *   Copyright (c) Digital Equipment Corporation 1984, 1985, 1986.    *
 *   All Rights Reserved. 					      *
 *   Reference "/usr/src/COPYRIGHT" for applicable restrictions.      *
 **********************************************************************/
/**
 * @file cat.c
 * @brief Concatenate files.
 */

/*
 *  cat	- conCATenate and print
 *
 *  Calling format: cat [-e] [-n] [-u] [-v] [-s] [-b] [-t]
 *	-e print a dollarsign ($) at the end of every line.
 *	-n number lines
 *	-u unbuffered output (ordinarily 1024-byte blocks if not terminal)
 *	-v show non-printables as ^x (or M-x if > 0177), except newline and tab
 *	-s reduce multiple blank lines to a single blank line
 *	-b same as -n, but don't number (or count) blank lines
 *	-t same as -v, but show tabs as well.
 *
 *  a '-' as the input file will cause input to be requested from stdin.
 */

static char Sccsid[] = "@(#)cat.c	3.0	4/21/86";

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

char stdbuf[BUFSIZ];
int bflg, eflg, nflg, sflg, tflg, vflg;
int spaced, col, lno, inline;

/**
 * @brief Program entry point.
 *
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return EXIT_SUCCESS on success.
 */
int main(int argc, char **argv) {
  int fflg = 0;
  register FILE *fi;
  register c;
  int dev, ino = -1;
  struct stat statb;

  lno = 1;
  setbuf(stdout, stdbuf);
  for (; argc > 1 && argv[1][0] == '-'; argc--, argv++) {
    switch (argv[1][1]) {
    case 0:
      break;
    case 'u':
      setbuf(stdout, (char *)NULL);
      continue;
    case 'n':
      nflg++;
      continue;
    case 'b':
      bflg++;
      nflg++;
      continue;
    case 'v':
      vflg++;
      continue;
    case 's':
      sflg++;
      continue;
    case 'e':
      eflg++;
      continue;
    case 't':
      tflg++;
      vflg++;
      continue;
    }
    break;
  }
  if (fstat(fileno(stdout), &statb) == 0) {
    statb.st_mode &= S_IFMT;
    if (statb.st_mode != S_IFCHR && statb.st_mode != S_IFBLK) {
      dev = statb.st_dev;
      ino = statb.st_ino;
    }
  }
  if (argc < 2) {
    argc = 2;
    fflg++;
  }
  while (--argc > 0) {
    if (fflg || (*++argv)[0] == '-' && (*argv)[1] == '\0')
      fi = stdin;
    else {
      if ((fi = fopen(*argv, "r")) == NULL) {
        perror(*argv);
        continue;
      }
    }
    if (fstat(fileno(fi), &statb) == 0) {
      if ((statb.st_mode & S_IFMT) == S_IFREG && statb.st_dev == dev &&
          statb.st_ino == ino) {
        fprintf(stderr, "cat: input %s is output\n", fflg ? "-" : *argv);
        fclose(fi);
        continue;
      }
    }
    if (nflg || sflg || vflg || eflg)
      copyopt(fi);
    else {
      while ((c = getc(fi)) != EOF)
        putchar(c);
    }
    if (fi != stdin)
      fclose(fi);
  }
  if (ferror(stdout))
    fprintf(stderr, "cat: output write error\n");
  return (0);
}

/*
 *  copyopt
 *
 *  Implement copy options.  Called if any major option is
 *  set.
 */
copyopt(f) register FILE *f;
{
  register int c;

top:
  c = getc(f);
  if (c == EOF)
    return;
  if (c == '\n') {
    if (inline == 0) {
      if (sflg && spaced)
        goto top;
      spaced = 1;
    }
    if (nflg && bflg == 0 && inline == 0)
      printf("%6d\t", lno++);
    if (eflg)
      putchar('$');
    putchar('\n');
    inline = 0;
    goto top;
  }
  if (nflg && inline == 0)
    printf("%6d\t", lno++);
  inline = 1;
  if (vflg) {
    if (tflg == 0 && c == '\t')
      putchar(c);
    else {
      if (c > 0177) {
        printf("M-");
        c &= 0177;
      }
      if (c < ' ')
        printf("^%c", c + '@');
      else if (c == 0177)
        printf("^?");
      else
        putchar(c);
    }
  } else
    putchar(c);
  spaced = 0;
  goto top;
}
