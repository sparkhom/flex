/** regex - regular expression functions related to POSIX regex lib. */

/*  Copyright (c) 1990 The Regents of the University of California. */
/*  All rights reserved. */

/*  This code is derived from software contributed to Berkeley by */
/*  Vern Paxson. */

/*  The United States Government has rights in this work pursuant */
/*  to contract no. DE-AC03-76SF00098 between the United States */
/*  Department of Energy and the University of California. */

/*  This file is part of flex. */

/*  Redistribution and use in source and binary forms, with or without */
/*  modification, are permitted provided that the following conditions */
/*  are met: */

/*  1. Redistributions of source code must retain the above copyright */
/*     notice, this list of conditions and the following disclaimer. */
/*  2. Redistributions in binary form must reproduce the above copyright */
/*     notice, this list of conditions and the following disclaimer in the */
/*     documentation and/or other materials provided with the distribution. */

/*  Neither the name of the University nor the names of its contributors */
/*  may be used to endorse or promote products derived from this software */
/*  without specific prior written permission. */

/*  THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR */
/*  IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED */
/*  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR */
/*  PURPOSE. */

#include "flexdef.h"

/** Extract a copy of the match, or NULL if no match.
 * @param m A match as returned by regexec().
 * @param src The source string that was passed to regexec().
 * @return The allocated string.
 */
char   *regmatch_dup (regmatch_t * m, const char *src)
{
	char   *str;
	int     len;

	if (m == NULL || m->rm_so < 0)
		return NULL;
	len = m->rm_eo - m->rm_so;
	str = (char *) flex_alloc ((len + 1) * sizeof (char));
	strncpy (str, src + m->rm_so, len);
	str[len] = 0;
	return str;
}

/** Copy the match.
 * @param m A match as returned by regexec().
 * @param dest The destination buffer.
 * @param src The source string that was passed to regexec().
 * @return dest
 */
char   *regmatch_cpy (regmatch_t * m, char *dest, const char *src)
{
	int     len;

	if (m == NULL || m->rm_so < 0) {
		if (dest)
			dest[0] = '\0';
		return dest;
	}

	len = m->rm_eo - m->rm_so;
	return strcpy (dest, src + m->rm_so + len);
}

/** Get the length in characters of the match.
 * @param m A match as returned by regexec().
 * @param src The source string that was passed to regexec().
 * @return The length of the match.
 */
int regmatch_len (regmatch_t * m)
{
	if (m == NULL || m->rm_so < 0) {
		return 0;
	}

	return m->rm_eo - m->rm_so;
}



/** Convert a regmatch_t object to an integer using the strtol() function.
 * @param m A match as returned by regexec().
 * @param src The source string that was passed to regexec().
 * @param endptr Same as the second argument to strtol().
 * @param base   Same as the third argument to strtol().
 * @return The converted integer or error (Return value is the same as for strtol()).
 */
int regmatch_strtol (regmatch_t * m, const char *src, char **endptr,
		     int base)
{
	int     n = 0;

#define bufsz 20
	char    buf[bufsz];
	char   *s;

	if (m == NULL || m->rm_so < 0)
		return 0;

	if (regmatch_len (m) < bufsz)
		s = regmatch_cpy (m, buf, src);
	else
		s = regmatch_dup (m, src);

	n = strtol (s, endptr, base);

	if (s != buf)
		free (s);

	return n;
}

/** Check for empty or non-existent match.
 * @param m A match as returned by regexec().
 * @return false if match length is non-zero.
 * Note that reg_empty returns true even if match did not occur at all.
 */
bool regmatch_empty (regmatch_t * m)
{
	return (m == NULL || m->rm_so < 0 || m->rm_so == m->rm_eo);
}

/* vim:set expandtab cindent tabstop=4 softtabstop=4 shiftwidth=4 textwidth=0: */