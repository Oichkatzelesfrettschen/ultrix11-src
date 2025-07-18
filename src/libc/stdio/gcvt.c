
/**********************************************************************
 *   Copyright (c) Digital Equipment Corporation 1984, 1985, 1986.    *
 *   All Rights Reserved. 					      *
 *   Reference "/usr/src/COPYRIGHT" for applicable restrictions.      *
 **********************************************************************/

/*
 * SCCSID: @(#)gcvt.c	3.0	4/22/86
 */
/*
 * gcvt  - Floating output conversion to
 * minimal length string
 */

char *ecvt();

/**
 * @brief Convert a floating point value to a minimal-length string.
 *
 * Chooses E-style or F-style output based on the magnitude of
 * @p number and stores the result in @p buf.
 *
 * @param number Number to convert.
 * @param ndigit Number of significant digits.
 * @param buf    Destination buffer.
 * @return Pointer to @p buf.
 */
char *gcvt(number, ndigit, buf)
double number;
char *buf;
{
  int sign, decpt;
  register char *p1, *p2;
  register i;

  p1 = ecvt(number, ndigit, &decpt, &sign);
  p2 = buf;
  if (sign)
    *p2++ = '-';
  for (i = ndigit - 1; i > 0 && p1[i] == '0'; i--)
    ndigit--;
  if (decpt >= 0 && decpt - ndigit > 4 ||
      decpt < 0 && decpt < -3) { /* use E-style */
    decpt--;
    *p2++ = *p1++;
    *p2++ = '.';
    for (i = 1; i < ndigit; i++)
      *p2++ = *p1++;
    *p2++ = 'e';
    if (decpt < 0) {
      decpt = -decpt;
      *p2++ = '-';
    } else
      *p2++ = '+';
    *p2++ = decpt / 10 + '0';
    *p2++ = decpt % 10 + '0';
  } else {
    if (decpt <= 0) {
      if (*p1 != '0')
        *p2++ = '.';
      while (decpt < 0) {
        decpt++;
        *p2++ = '0';
      }
    }
    for (i = 1; i <= ndigit; i++) {
      *p2++ = *p1++;
      if (i == decpt)
        *p2++ = '.';
    }
    if (ndigit < decpt) {
      while (ndigit++ < decpt)
        *p2++ = '0';
      *p2++ = '.';
    }
  }
  if (p2[-1] == '.')
    p2--;
  *p2 = '\0';
  return (buf);
}
