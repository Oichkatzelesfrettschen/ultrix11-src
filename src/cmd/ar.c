
/**********************************************************************
 *   Copyright (c) Digital Equipment Corporation 1984, 1985, 1986.    *
 *   All Rights Reserved. 					      *
 *   Reference "/usr/src/COPYRIGHT" for applicable restrictions.      *
 **********************************************************************/
/**
 * @file ar.c
 * @brief Archive utility.
 */

static char Sccsid[] = "@(#)ar.c 3.0 4/21/86";
#include <ar.h>
#include <signal.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
struct stat stbuf;
struct ar_hdr arbuf;

#define SKIP 1
#define IODD 2
#define OODD 4
#define HEAD 8

char *man = {"mrxtdpq"};
char *opt = {"uvnbail"};

int signum[] = {SIGHUP, SIGINT, SIGQUIT, 0};
int sigdone();
long lseek();
int rcmd();
int dcmd();
int xcmd();
int tcmd();
int pcmd();
int mcmd();
int qcmd();
int (*comfun)();
char flg[26];
char **namv;
int namc;
char *arnam;
char *ponam;
char *tmp0nam = {"/tmp/vXXXXX"};
char *tmp1nam = {"/tmp/v1XXXXX"};
char *tmp2nam = {"/tmp/v2XXXXX"};
char *tfnam;
char *tf1nam;
char *tf2nam;
char *file;
char name[16];
int af;
int tf;
int tf1;
int tf2;
int qf;
int bastate;
char buf[1024];

char *trim();
char *mktemp();
char *ctime();

/**
 * @brief Program entry point.
 *
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return EXIT_SUCCESS on success.
 */
int main(int argc, char **argv) {
  register i;
  register char *cp;

  for (i = 0; signum[i]; i++)
    if (signal(signum[i], SIG_IGN) != SIG_IGN)
      signal(signum[i], sigdone);
  if (argc < 3)
    usage();
  cp = argv[1];
  for (cp = argv[1]; *cp; cp++)
    switch (*cp) {
    case 'l':
    case 'v':
    case 'u':
    case 'n':
    case 'a':
    case 'b':
    case 'c':
    case 'i':
      flg[*cp - 'a']++;
      continue;

    case 'r':
      setcom(rcmd);
      continue;

    case 'd':
      setcom(dcmd);
      continue;

    case 'x':
      setcom(xcmd);
      continue;

    case 't':
      setcom(tcmd);
      continue;

    case 'p':
      setcom(pcmd);
      continue;

    case 'm':
      setcom(mcmd);
      continue;

    case 'q':
      setcom(qcmd);
      continue;

    default:
      fprintf(stderr, "ar: bad option `%c'\n", *cp);
      done(1);
    }
  if (flg['l' - 'a']) {
    tmp0nam = "vXXXXX";
    tmp1nam = "v1XXXXX";
    tmp2nam = "v2XXXXX";
  }
  if (flg['i' - 'a'])
    flg['b' - 'a']++;
  if (flg['a' - 'a'] || flg['b' - 'a']) {
    bastate = 1;
    ponam = trim(argv[2]);
    argv++;
    argc--;
    if (argc < 3)
      usage();
  }
  arnam = argv[2];
  namv = argv + 3;
  namc = argc - 3;
  if (comfun == 0) {
    if (flg['u' - 'a'] == 0) {
      fprintf(stderr, "ar: one of [%s] must be specified\n", man);
      done(1);
    }
    setcom(rcmd);
  }
  (*comfun)();
  done(notfound());
}

/**
 * @brief Set the archive command to execute.
 *
 * Only one command may be specified on the command line.  This
 * function stores the function pointer for later invocation.
 *
 * @param fun Function implementing the selected command.
 */
setcom(fun) int (*fun)();
{

  if (comfun != 0) {
    fprintf(stderr, "ar: only one of [%s] allowed\n", man);
    done(1);
  }
  comfun = fun;
}

/**
 * @brief Replace or insert files into the archive.
 *
 * Iterates over the file list stored in @c namv and either
 * adds new entries or replaces existing ones.  The flags in
 * @c flg determine how files are processed.
 *
 * @return void
 */
rcmd() {
  register f;

  init();
  getaf();
  while (!getdir()) {
    bamatch();
    if (namc == 0 || match()) {
      f = stats();
      if (f < 0) {
        if (namc)
          fprintf(stderr, "ar: cannot open %s\n", file);
        goto cp;
      }
      if (flg['u' - 'a'])
        if (stbuf.st_mtime <= arbuf.ar_date) {
          close(f);
          goto cp;
        }
      mesg('r');
      copyfil(af, -1, IODD + SKIP);
      movefil(f);
      continue;
    }
  cp:
    mesg('c');
    copyfil(af, tf, IODD + OODD + HEAD);
  }
  cleanup();
}

/**
 * @brief Delete files from the archive.
 *
 * Removes each file listed in @c namv from the archive if
 * it exists.  Non-matching files are copied to the temporary
 * archive unchanged.
 *
 * @return void
 */
dcmd() {

  init();
  if (getaf())
    noar();
  while (!getdir()) {
    if (match()) {
      mesg('d');
      copyfil(af, -1, IODD + SKIP);
      continue;
    }
    mesg('c');
    copyfil(af, tf, IODD + OODD + HEAD);
  }
  install();
}

/**
 * @brief Extract files from the archive.
 *
 * Creates each matching file on disk with the
 * archived metadata and copies its contents out of the
 * archive.
 *
 * @return void
 */
xcmd() {
  register f;

  if (getaf())
    noar();
  while (!getdir()) {
    if (namc == 0 || match()) {
      f = creat(file, arbuf.ar_mode & 0777);
      if (f < 0) {
        fprintf(stderr, "ar: %s cannot create\n", file);
        goto sk;
      }
      mesg('x');
      copyfil(af, f, IODD);
      close(f);
      continue;
    }
  sk:
    mesg('c');
    copyfil(af, -1, IODD + SKIP);
    if (namc > 0 && !morefil())
      done(0);
  }
}

/**
 * @brief Print files from the archive to standard output.
 *
 * Extracts files to stdout, optionally printing a
 * header when verbose mode is active.
 *
 * @return void
 */
pcmd() {

  if (getaf())
    noar();
  while (!getdir()) {
    if (namc == 0 || match()) {
      if (flg['v' - 'a']) {
        printf("\n<%s>\n\n", file);
        fflush(stdout);
      }
      copyfil(af, 1, IODD);
      continue;
    }
    copyfil(af, -1, IODD + SKIP);
  }
}

/**
 * @brief Move files within the archive.
 *
 * Reorders files based on the position arguments
 * specified with `a` or `b` options.
 *
 * @return void
 */
mcmd() {

  init();
  if (getaf())
    noar();
  tf2nam = mktemp(tmp2nam);
  close(creat(tf2nam, 0600));
  tf2 = open(tf2nam, 2);
  if (tf2 < 0) {
    fprintf(stderr, "ar: cannot create third temp\n");
    done(1);
  }
  while (!getdir()) {
    bamatch();
    if (match()) {
      mesg('m');
      copyfil(af, tf2, IODD + OODD + HEAD);
      continue;
    }
    mesg('c');
    copyfil(af, tf, IODD + OODD + HEAD);
  }
  install();
}

/**
 * @brief List archive contents.
 *
 * Displays each archive member name, optionally in
 * verbose format.
 *
 * @return void
 */
tcmd() {

  if (getaf())
    noar();
  while (!getdir()) {
    if (namc == 0 || match()) {
      if (flg['v' - 'a'])
        longt();
      printf("%s\n", trim(file));
    }
    copyfil(af, -1, IODD + SKIP);
  }
}

/**
 * @brief Quick append files to the archive without checks.
 *
 * Appends files directly to the archive without verifying
 * whether they already exist.
 *
 * @return void
 */
qcmd() {
  register i, f;

  if (flg['a' - 'a'] || flg['b' - 'a']) {
    fprintf(stderr, "ar: abi not allowed with q\n");
    done(1);
  }
  getqf();
  for (i = 0; signum[i]; i++)
    signal(signum[i], SIG_IGN);
  lseek(qf, 0l, 2);
  for (i = 0; i < namc; i++) {
    file = namv[i];
    if (file == 0)
      continue;
    namv[i] = 0;
    mesg('q');
    f = stats();
    if (f < 0) {
      fprintf(stderr, "ar: %s cannot open\n", file);
      continue;
    }
    tf = qf;
    movefil(f);
    qf = tf;
  }
}

/**
 * @brief Initialize temporary files for archive operations.
 *
 * Creates one or more temporary files used during
 * modification of the archive.
 *
 * @return void
 */
init() {
  static mbuf = ARMAG;

  tfnam = mktemp(tmp0nam);
  close(creat(tfnam, 0600));
  tf = open(tfnam, 2);
  if (tf < 0) {
    fprintf(stderr, "ar: cannot create temp file\n");
    done(1);
  }
  if (write(tf, (char *)&mbuf, sizeof(int)) != sizeof(int))
    wrerr();
}

/**
 * @brief Open the archive file for reading.
 *
 * @return 0 on success, 1 if the archive does not exist.
 */
getaf() {
  int mbuf;

  af = open(arnam, 0);
  if (af < 0)
    return (1);
  if (read(af, (char *)&mbuf, sizeof(int)) != sizeof(int) || mbuf != ARMAG) {
    fprintf(stderr, "ar: %s not in archive format\n", arnam);
    done(1);
  }
  return (0);
}

/**
 * @brief Open the archive for writing in quick append mode.
 *
 * Creates the archive if it does not already exist and
 * verifies that the file header contains the proper
 * archive magic.
 *
 * @return void
 */
getqf() {
  int mbuf;

  if ((qf = open(arnam, 2)) < 0) {
    if (!flg['c' - 'a'])
      fprintf(stderr, "ar: creating %s\n", arnam);
    close(creat(arnam, 0666));
    if ((qf = open(arnam, 2)) < 0) {
      fprintf(stderr, "ar: cannot create %s\n", arnam);
      done(1);
    }
    mbuf = ARMAG;
    if (write(qf, (char *)&mbuf, sizeof(int)) != sizeof(int))
      wrerr();
  } else if (read(qf, (char *)&mbuf, sizeof(int)) != sizeof(int) ||
             mbuf != ARMAG) {
    fprintf(stderr, "ar: %s not in archive format\n", arnam);
    done(1);
  }
}

/**
 * @brief Display usage information and exit.
 *
 * Prints the valid command line options and terminates the
 * program.
 *
 * @return void
 */
usage() {
  printf("usage: ar [%s][%s] archive files ...\n", opt, man);
  done(1);
}

/**
 * @brief Abort when the archive does not exist.
 *
 * Prints an error message and terminates the program.
 *
 * @return void
 */
noar() {

  fprintf(stderr, "ar: %s does not exist\n", arnam);
  done(1);
}

/**
 * @brief Signal handler that terminates the program.
 *
 * @return void
 */
sigdone() { done(100); }

/**
 * @brief Clean up temporary files and exit.
 *
 * @param c Exit code.
 * @return void
 */
done(c) {

  if (tfnam)
    unlink(tfnam);
  if (tf1nam)
    unlink(tf1nam);
  if (tf2nam)
    unlink(tf2nam);
  exit(c);
}

/**
 * @brief Report names not found in the archive.
 *
 * @return Number of missing files.
 */
notfound() {
  register i, n;

  n = 0;
  for (i = 0; i < namc; i++)
    if (namv[i]) {
      fprintf(stderr, "ar: %s not found\n", namv[i]);
      n++;
    }
  return (n);
}

/**
 * @brief Determine whether more files remain to process.
 *
 * @return Number of remaining file names.
 */
morefil() {
  register i, n;

  n = 0;
  for (i = 0; i < namc; i++)
    if (namv[i])
      n++;
  return (n);
}

/**
 * @brief Complete file addition and install the archive.
 *
 * Processes files queued for insertion and then writes the
 * updated archive to disk.
 *
 * @return void
 */
cleanup() {
  register i, f;

  for (i = 0; i < namc; i++) {
    file = namv[i];
    if (file == 0)
      continue;
    namv[i] = 0;
    mesg('a');
    f = stats();
    if (f < 0) {
      fprintf(stderr, "ar: %s cannot open\n", file);
      continue;
    }
    movefil(f);
  }
  install();
}

/**
 * @brief Finalize and write the new archive.
 *
 * Copies all temporary data to the output archive file and
 * cleans up signal handlers.
 *
 * @return void
 */
install() {
  register i;

  for (i = 0; signum[i]; i++)
    signal(signum[i], SIG_IGN);
  if (af < 0)
    if (!flg['c' - 'a'])
      fprintf(stderr, "ar: creating %s\n", arnam);
  close(af);
  af = creat(arnam, 0666);
  if (af < 0) {
    fprintf(stderr, "ar: cannot create %s\n", arnam);
    done(1);
  }
  if (tfnam) {
    lseek(tf, 0l, 0);
    while ((i = read(tf, buf, 1024)) > 0)
      if (write(af, buf, i) != i)
        wrerr();
  }
  if (tf2nam) {
    lseek(tf2, 0l, 0);
    while ((i = read(tf2, buf, 1024)) > 0)
      if (write(af, buf, i) != i)
        wrerr();
  }
  if (tf1nam) {
    lseek(tf1, 0l, 0);
    while ((i = read(tf1, buf, 1024)) > 0)
      if (write(af, buf, i) != i)
        wrerr();
  }
}

/*
 * insert the file 'file'
 * into the temporary file
 */
/**
 * @brief Insert the specified file into the temporary archive.
 *
 * @param f File descriptor of the input file.
 */
movefil(f) {
  register char *cp;
  register i;

  cp = trim(file);
  for (i = 0; i < 14; i++)
    if (arbuf.ar_name[i] = *cp)
      cp++;
  arbuf.ar_size = stbuf.st_size;
  arbuf.ar_date = stbuf.st_mtime;
  arbuf.ar_uid = stbuf.st_uid;
  arbuf.ar_gid = stbuf.st_gid;
  arbuf.ar_mode = stbuf.st_mode;
  copyfil(f, tf, OODD + HEAD);
  close(f);
}

/**
 * @brief Stat the named file and return an open descriptor.
 *
 * @return File descriptor or -1 on error.
 */
stats() {
  register f;

  f = open(file, 0);
  if (f < 0)
    return (f);
  if (fstat(f, &stbuf) < 0) {
    close(f);
    return (-1);
  }
  return (f);
}

/*
 * copy next file
 * size given in arbuf
 */
/**
 * @brief Copy the next file from fi to fo using the size in arbuf.
 *
 * @param fi   Input file descriptor.
 * @param fo   Output file descriptor.
 * @param flag Combination of SKIP, IODD, OODD, or HEAD.
 */
copyfil(fi, fo, flag) {
  register i, o;
  int pe;

  if (flag & HEAD)
    if (write(fo, (char *)&arbuf, sizeof arbuf) != sizeof arbuf)
      wrerr();
  pe = 0;
  while (arbuf.ar_size > 0) {
    i = o = 1024;
    if (arbuf.ar_size < i) {
      i = o = arbuf.ar_size;
      if (i & 1) {
        if (flag & IODD)
          i++;
        if (flag & OODD)
          o++;
      }
    }
    if (read(fi, buf, i) != i)
      pe++;
    if ((flag & SKIP) == 0)
      if (write(fo, buf, o) != o)
        wrerr();
    arbuf.ar_size -= 1024;
  }
  if (pe)
    phserr();
}

/**
 * @brief Read the next file header from the archive.
 *
 * @return 0 if a header was read, 1 on end of file.
 */
getdir() {
  register i;

  i = read(af, (char *)&arbuf, sizeof arbuf);
  if (i != sizeof arbuf) {
    if (tf1nam) {
      i = tf;
      tf = tf1;
      tf1 = i;
    }
    return (1);
  }
  for (i = 0; i < 14; i++)
    name[i] = arbuf.ar_name[i];
  file = name;
  return (0);
}

/**
 * @brief Check if the current file name matches any specified patterns.
 *
 * @return 1 on match, 0 otherwise.
 */
match() {
  register i;

  for (i = 0; i < namc; i++) {
    if (namv[i] == 0)
      continue;
    if (strcmp(trim(namv[i]), file) == 0) {
      file = namv[i];
      namv[i] = 0;
      return (1);
    }
  }
  return (0);
}

/**
 * @brief Handle 'a' or 'b' options for positioning new files.
 */
bamatch() {
  register f;

  switch (bastate) {

  case 1:
    if (strcmp(file, ponam) != 0)
      return;
    bastate = 2;
    if (flg['a' - 'a'])
      return;

  case 2:
    bastate = 0;
    tf1nam = mktemp(tmp1nam);
    close(creat(tf1nam, 0600));
    f = open(tf1nam, 2);
    if (f < 0) {
      fprintf(stderr, "ar: cannot create second temp\n");
      return;
    }
    tf1 = tf;
    tf = f;
  }
}

/**
 * @brief Report an archive phase error.
 *
 * This occurs when the archive structure is not as
 * expected during copying.
 *
 * @return void
 */
phserr() { fprintf(stderr, "ar: phase error on %s\n", file); }

/**
 * @brief Print an operation message when verbose mode is enabled.
 *
 * @param c Operation code character.
 * @return void
 */
mesg(c) {

  if (flg['v' - 'a'])
    if (c != 'c' || flg['v' - 'a'] > 1)
      printf("%c - %s\n", c, file);
}

/**
 * @brief Remove any leading directory components from a path name.
 *
 * @param s Input path.
 * @return Pointer to the base name within s.
 */
char *trim(s)
char *s;
{
  register char *p1, *p2;

  for (p1 = s; *p1; p1++)
    ;
  while (p1 > s) {
    if (*--p1 != '/')
      break;
    *p1 = 0;
  }
  p2 = s;
  for (p1 = s; *p1; p1++)
    if (*p1 == '/')
      p2 = p1 + 1;
  return (p2);
}

#define IFMT 060000
#define ISARG 01000
#define LARGE 010000
#define SUID 04000
#define SGID 02000
#define ROWN 0400
#define WOWN 0200
#define XOWN 0100
#define RGRP 040
#define WGRP 020
#define XGRP 010
#define ROTH 04
#define WOTH 02
#define XOTH 01
#define STXT 01000

/**
 * @brief Print a verbose listing of the current archive header.
 *
 * @return void
 */
longt() {
  register char *cp;

  pmode();
  printf("%3d/%1d", arbuf.ar_uid, arbuf.ar_gid);
  printf("%7D", arbuf.ar_size);
  cp = ctime(&arbuf.ar_date);
  printf(" %-12.12s %-4.4s ", cp + 4, cp + 20);
}

int m1[] = {1, ROWN, 'r', '-'};
int m2[] = {1, WOWN, 'w', '-'};
int m3[] = {2, SUID, 's', XOWN, 'x', '-'};
int m4[] = {1, RGRP, 'r', '-'};
int m5[] = {1, WGRP, 'w', '-'};
int m6[] = {2, SGID, 's', XGRP, 'x', '-'};
int m7[] = {1, ROTH, 'r', '-'};
int m8[] = {1, WOTH, 'w', '-'};
int m9[] = {2, STXT, 't', XOTH, 'x', '-'};

int *m[] = {m1, m2, m3, m4, m5, m6, m7, m8, m9};

/**
 * @brief Print file mode bits from arbuf.
 *
 * @return void
 */
pmode() {
  register int **mp;

  for (mp = &m[0]; mp < &m[9];)
    select(*mp++);
}

/**
 * @brief Helper for pmode to output permission characters.
 *
 * @param pairp Permission mapping table entry.
 * @return void
 */
select(pairp) int *pairp;
{
  register int n, *ap;

  ap = pairp;
  n = *ap++;
  while (--n >= 0 && (arbuf.ar_mode & *ap++) == 0)
    ap++;
  putchar(*ap);
}

/**
 * @brief Print a write error message and exit.
 *
 * @return void
 */
wrerr() {
  perror("ar write error");
  done(1);
}
