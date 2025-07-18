
/**********************************************************************
 *   Copyright (c) Digital Equipment Corporation 1984, 1985, 1986.    *
 *   All Rights Reserved. 					      *
 *   Reference "/usr/src/COPYRIGHT" for applicable restrictions.      *
 **********************************************************************/
/**
 * @file cc.c
 * @brief C compiler driver.
 */

#
static char Sccsid[] = "@(#)cc.c 3.0 4/21/86";
#include <ctype.h>
#include <signal.h>
#include <stdio.h>

/* cc - front end for C compiler */

#define MAXINC 10
#define MAXFIL 100
#define MAXLIB 100
#define MAXOPT 100
#define CHSPACE 1000
char tmp0[16];
char *tmp1, *tmp2, *tmp3, *tmp4, *tmp5;
char *outfile;
char ts[CHSPACE + 50];
char *tsa = ts;
char *tsp = ts;
char *av[50];
char *clist[MAXFIL];
char *llist[MAXLIB];
int pflag, sflag, cflag, eflag, exflag, oflag, proflag, noflflag;
int sys_V_flag; /* true if System V prog. env. was specified */
/*
 * Vflag now defaults to 1.  This is so that
 * we can get rid of the overlay libraries and
 * have one common object format that will work
 * for both overlay and non-overlay code.
 * 6/18/84 -Dave Borman
 */
int Vflag = 1; /* for overlays */
int Nflag = 0; /* put switch tables into text space */
char *chpass;
char *npassname;
char right_c2[6]; /* is either c2_ov or c2_id, used with -B */
char pass0[20] = "/lib/c0";
char pass1[20] = "/lib/c1";
char pass2[23] = "/lib/c2_ov"; /* set to c2_id if /unix is 0411 type kernel */
char passp[20] = "/lib/cpp";
char *pref = "/lib/crt0.o";
char *copy();
char *setsuf();
char *strcat();
char *strcpy();

int k, debug_printout; /* debug_printout option, print each pass on stdout */

/**
 * @brief Program entry point.
 *
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return EXIT_SUCCESS on success.
 */
int main(int argc, char **argv) {
  char *t;
  char *savetsp;
  char *assource;
  char **pv, *ptemp[MAXOPT], **pvt;
  int nc, nl, i, j, c, f20, nxo, na;
  int idexit();

  /*
   * Change optimizer pass to /lib/c2_id if running
   * separate I & D kernel (0411).
   * Physical location 0 tells which type of kernel.
   * 0 = 3 for OV, 0 = 4 for SID
   * Comments in /usr/src/cmd/mkconf/mkc_str.c warn not to change
   * location zero without checking cc.c first.
   */
  strcpy(right_c2, "c2_ov"); /* default if "-B" */
  if ((i = open("/dev/mem", 0)) >= 0) {
    read(i, (char *)&nl, sizeof(int));
    if (nl == 4) {
      pass2[8] = 'i';
      pass2[9] = 'd';
      strcpy(right_c2, "c2_id"); /* for "-B" */
    }
    close(i);
  }
  i = nc = nl = f20 = nxo = sys_V_flag = 0;
  debug_printout = 0;
  setbuf(stdout, (char *)NULL);
  pv = ptemp;
  /*
   * Check for Programming Environment,
   * If it is SYSTEM_FIVE, then set up
   * for System V environment.
   */
  if (strcmp("SYSTEM_FIVE", getenv("PROG_ENV")) == 0) {
    sys_V_flag++;
  }
  if (strcmp("DEBUG5", getenv("PROG_ENV")) == 0) {
    sys_V_flag++;
    debug_printout++;
  }
  if (strcmp("DEBUG", getenv("PROG_ENV")) == 0) {
    debug_printout++;
  }
  while (++i < argc) {
    if (*argv[i] == '-')
      switch (argv[i][1]) {
      default:
        goto passa;
      case 'Y':
        if (!sys_V_flag)
          sys_V_flag++;
        break;
      case 'S':
        sflag++;
        cflag++;
        break;
      case 'o':
        if (++i < argc) {
          outfile = argv[i];
          if ((c = getsuf(outfile)) == 'c' || c == 'o') {
            error("Would overwrite %s", outfile);
            exit(8);
          }
        }
        break;
      case 'O':
        oflag++;
        break;
      case 'p':
        proflag++;
        break;
      case 'E':
        exflag++;
      case 'P':
        pflag++;
        *pv++ = argv[i];
      case 'c':
        cflag++;
        break;

      case 'f':
        noflflag++;
        if (npassname || chpass)
          error("-f overwrites earlier option", (char *)NULL);
        npassname = "/lib/f";
        chpass = "1";
        break;

      case '2':
        if (argv[i][2] == '\0')
          pref = "/lib/crt2.o";
        else {
          pref = "/lib/crt20.o";
          f20 = 1;
        }
        break;
      case 'D':
      case 'I':
      case 'U':
      case 'C':
        *pv++ = argv[i];
        if (pv >= ptemp + MAXOPT) {
          error("Too many DIUC options", (char *)NULL);
          --pv;
        }
        break;
      case 't':
        if (chpass)
          error("-t overwrites earlier option", (char *)NULL);
        chpass = argv[i] + 2;
        if (chpass[0] == 0)
          chpass = "012p";
        break;

      case 'B':
        if (npassname)
          error("-B overwrites earlier option", (char *)NULL);
        npassname = argv[i] + 2;
        if (npassname[0] == 0)
          npassname = "/usr/c/o";
        break;

      case 'N': /* put switch tables into text space */
        Nflag++;
        break;

      case 'V':
        /*
         * -V means offset for overlays (now
         * the default), -V7 means don't.
         */
        Vflag = (argv[i][2] != '7');
        break;
      }
    else {
    passa:
      t = argv[i];
      if ((c = getsuf(t)) == 'c' || c == 's' || exflag) {
        clist[nc++] = t;
        if (nc >= MAXFIL) {
          error("Too many source files", (char *)NULL);
          exit(1);
        }
        t = setsuf(t, 'o');
      }
      if (nodup(llist, t)) {
        llist[nl++] = t;
        if (nl >= MAXLIB) {
          error("Too many object/library files", (char *)NULL);
          exit(1);
        }
        if (getsuf(t) == 'o')
          nxo++;
      }
    }
  }
  if (npassname && chpass == 0)
    chpass = "012p";
  if (chpass && npassname == 0) /* -t but no -B option */
    npassname = "/usr/src/cmd/c/";
  if (chpass)
    for (t = chpass; *t; t++) {
      switch (*t) {
      case '0':
        strcpy(pass0, npassname);
        strcat(pass0, "c0");
        continue;
      case '1':
        strcpy(pass1, npassname);
        strcat(pass1, "c1");
        continue;
      case '2':
        strcpy(pass2, npassname);
        /* "right_c2" next is either c2_ov or c2_id, setup earlier */
        strcat(pass2, right_c2);
        continue;
      case 'p':
        strcpy(passp, npassname);
        strcat(passp, "cpp");
        continue;
      }
    }
  if (noflflag)
    pref = proflag ? "/lib/fmcrt0.o" : "/lib/fcrt0.o";
  else if (proflag)
    pref = "/lib/mcrt0.o";
  if (nc == 0)
    goto nocom;
  if (pflag == 0)
    sprintf(tmp0, "/tmp/ctm0.%d", getpid());
  if (signal(SIGINT, SIG_IGN) != SIG_IGN)
    signal(SIGINT, idexit);
  if (signal(SIGTERM, SIG_IGN) != SIG_IGN)
    signal(SIGTERM, idexit);
  (tmp1 = copy(tmp0))[8] = '1';
  (tmp2 = copy(tmp0))[8] = '2';
  (tmp3 = copy(tmp0))[8] = '3';
  if (oflag)
    (tmp5 = copy(tmp0))[8] = '5';
  if (pflag == 0)
    (tmp4 = copy(tmp0))[8] = '4';
  pvt = pv;
  for (i = 0; i < nc; i++) {
    if (nc > 1)
      printf("%s:\n", clist[i]);
    if (getsuf(clist[i]) == 's') {
      assource = clist[i];
      goto assemble;
    } else
      assource = tmp3;
    if (pflag)
      tmp4 = setsuf(clist[i], 'i');
    savetsp = tsp;
    av[0] = "cpp";
    av[1] = clist[i];
    av[2] = exflag ? "-" : tmp4;
    na = 3;
    if (sys_V_flag) {
      av[na++] = "-DSYSTEM_FIVE"; /* used in System V header files */
    }
    for (pv = ptemp; pv < pvt; pv++)
      av[na++] = *pv;
    av[na++] = 0;

    if (debug_printout) {
      k = 0;
      while (av[k])
        printf("%s ", av[k++]);
      printf("\n");
    }

    if (callsys(passp, av)) {
      cflag++;
      eflag++;
      continue;
    }
    av[1] = tmp4;
    tsp = savetsp;
    av[0] = "c0";
    if (pflag) {
      cflag++;
      continue;
    }
    av[2] = tmp1;
    av[3] = tmp2;
    {
      int i = 4;
      if (proflag)
        av[i++] = "-P";
      if (Vflag)
        av[i++] = "-V";
      av[i] = 0;
    }

    if (debug_printout) {
      k = 0;
      while (av[k])
        printf("%s ", av[k++]);
      printf("\n");
    }

    if (callsys(pass0, av)) {
      cflag++;
      eflag++;
      continue;
    }
    av[0] = "c1";
    av[1] = tmp1;
    av[2] = tmp2;
    if (sflag) {
      assource = tmp3 = setsuf(clist[i], 's');
    }
    av[3] = tmp3;
    if (oflag)
      av[3] = tmp5;
    {
      int i = 4;
      if (Nflag)
        av[i++] = "-N";
      av[i] = 0;
    }

    if (debug_printout) {
      k = 0;
      while (av[k])
        printf("%s ", av[k++]);
      printf("\n");
    }

    if (callsys(pass1, av)) {
      cflag++;
      eflag++;
      continue;
    }
    if (oflag) {
      av[0] = "c2";
      av[1] = tmp5;
      av[2] = tmp3;
      av[3] = 0;

      if (debug_printout) {
        k = 0;
        while (av[k])
          printf("%s ", av[k++]);
        printf("\n");
      }

      if (callsys(pass2, av)) {
        unlink(tmp3);
        tmp3 = assource = tmp5;
      } else
        unlink(tmp5);
    }
    if (sflag)
      continue;
  assemble:
    av[0] = "as";
    av[1] = "-u";
    av[2] = "-V";
    av[3] = "-o";
    av[4] = setsuf(clist[i], 'o');
    av[5] = assource;
    av[6] = 0;
    cunlink(tmp1);
    cunlink(tmp2);
    cunlink(tmp4);

    if (debug_printout) {
      k = 0;
      while (av[k])
        printf("%s ", av[k++]);
      printf("\n");
    }

    if (callsys("/bin/as", av) > 1) {
      cflag++;
      eflag++;
      continue;
    }
  }
nocom:
  if (cflag == 0 && nl != 0) {
    i = 0;
    av[0] = "ld";
    av[1] = "-X";
    av[2] = pref;
    j = 3;
    if (noflflag) {
      j = 4;
      av[3] = "-lfpsim";
    }
    if (outfile) {
      av[j++] = "-o";
      av[j++] = outfile;
    }
    while (i < nl)
      av[j++] = llist[i++];
    if (!Vflag) {
      av[j++] = "/lib/v7csv.o";
    }
    if (f20)
      av[j++] = "-l2";
    else {
      if (sys_V_flag)     /* System V environment */
        av[j++] = "-lcV"; /* so load libcV.a first */
      av[j++] = "-lc";
    }
    av[j++] = 0;

    if (debug_printout) {
      k = 0;
      while (av[k])
        printf("%s ", av[k++]);
      printf("\n");
    }

    eflag |= callsys("/bin/ld", av);
    if (nc == 1 && nxo == 1 && eflag == 0)
      cunlink(setsuf(clist[0], 'o'));
  }
  dexit();
}

idexit() {
  eflag = 100;
  dexit();
}

dexit() {
  if (!pflag) {
    cunlink(tmp1);
    cunlink(tmp2);
    if (sflag == 0)
      cunlink(tmp3);
    cunlink(tmp4);
    cunlink(tmp5);
  }
  exit(eflag);
}

error(s, x) char *s, *x;
{
  fprintf(exflag ? stderr : stdout, s, x);
  putc('\n', exflag ? stderr : stdout);
  cflag++;
  eflag++;
}

getsuf(as) char as[];
{
  register int c;
  register char *s;
  register int t;

  s = as;
  c = 0;
  while (t = *s++)
    if (t == '/')
      c = 0;
    else
      c++;
  s -= 3;
  if (c <= 14 && c > 2 && *s++ == '.')
    return (*s);
  return (0);
}

char *setsuf(as, ch)
char *as;
{
  register char *s, *s1;

  s = s1 = copy(as);
  while (*s)
    if (*s++ == '/')
      s1 = s;
  s[-1] = ch;
  return (s1);
}

callsys(f, v) char f[], *v[];
{
  int t, status;

  if ((t = fork()) == 0) {
    execv(f, v);
    printf("Can't find %s\n", f);
    exit(100);
  } else if (t == -1) {
    printf("Try again\n");
    return (100);
  }
  while (t != wait(&status))
    ;
  if (t = status & 0377) {
    if (t != SIGINT) {
      printf("Fatal error in %s\n", f);
      eflag = 8;
    }
    dexit();
  }
  return ((status >> 8) & 0377);
}

char *copy(as)
char *as;
{
  char *malloc();
  register char *otsp, *s;

  otsp = tsp;
  s = as;
  while (*tsp++ = *s++)
    ;
  if (tsp > tsa + CHSPACE) {
    tsp = tsa = malloc(CHSPACE + 50);
    if (tsp == NULL) {
      error("no space for file names", (char *)NULL);
      dexit();
    }
  }
  return (otsp);
}

nodup(l, os) char **l, *os;
{
  register char *t, *s;
  register int c;

  s = os;
  if (getsuf(s) != 'o')
    return (1);
  while (t = *l++) {
    while (c = *s++)
      if (c != *t++)
        break;
    if (*t == '\0' && c == '\0')
      return (0);
    s = os;
  }
  return (1);
}

cunlink(f) char *f;
{
  if (f == NULL)
    return;
  unlink(f);
}
