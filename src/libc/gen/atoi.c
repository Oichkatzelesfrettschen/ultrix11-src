
/**********************************************************************
 *   Copyright (c) Digital Equipment Corporation 1984, 1985, 1986.    *
 *   All Rights Reserved.                                             *
 *   Reference "/usr/src/COPYRIGHT" for applicable restrictions.      *
 **********************************************************************/

/*
 * SCCSID: @(#)atoi.c   3.0     4/22/86
 */

/**
 * @file atoi.c
 * @brief Implementation of ASCII to integer conversion.
 *
 * Provides the classic `atoi` routine that converts a string of decimal digits,
 * with optional leading whitespace and sign, into an integer.
 */

/**
 * @brief Convert a string to an integer.
 *
 * Skips leading whitespace, handles an optional sign, and processes
 * subsequent decimal digits.
 *
 * @param p Pointer to the null-terminated string to convert.
 * @return Converted integer value.
 */
int atoi(register char *p) {
  register int n; /* Accumulated result */
  register int f; /* Sign flag: non-zero if '-' sign */

  n = 0;         /* Initialize result */
  f = 0;         /* Assume positive */
  for (;; p++) { /* Process optional sign and whitespace */
    switch (*p) {
    case ' ':
    case '\t':
      continue; /* Skip whitespace */
    case '-':
      f++; /* Remember negative sign */
    case '+':
      p++; /* Skip sign character */
    }
    break; /* End sign processing */
  }
  while (*p >= '0' && *p <= '9')
    n = n * 10 + *p++ - '0'; /* Accumulate digits */
  return (f ? -n : n);       /* Apply sign and return */
}
