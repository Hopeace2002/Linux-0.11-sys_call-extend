/* This file may have been modified by DJ Delorie (Jan 1991).  If so,
** these modifications are Coyright (C) 1991 DJ Delorie, 24 Kirsten Ave,
** Rochester NH, 03867-2954, USA.
*/

/*-
 * Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Chris Torek.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that: (1) source distributions retain this entire copyright
 * notice and comment, and (2) distributions including binaries display
 * the following acknowledgement:  ``This product includes software
 * developed by the University of California, Berkeley and its contributors''
 * in the documentation or other materials provided with the distribution
 * and in all advertising materials mentioning features or use of this
 * software. Neither the name of the University nor the names of its
 * contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)atexit.c	5.1 (Berkeley) 5/15/90";
#endif /* LIBC_SCCS and not lint */

#include <stddef.h>
#include <stdlib.h>
#include <atexit.h>

/*
 * Register a function to be performed at exit.
 */
int
atexit(void (*fn)(void))
{
	static struct atexit __atexit0;	/* one guaranteed table */
	register struct atexit *p;

	if ((p = __atexit) == NULL)
		__atexit = p = &__atexit0;
	if (p->ind >= ATEXIT_SIZE) {
		if ((p = (struct atexit *) malloc(sizeof(*p))) == NULL)
			return (-1);
		__atexit->next = p;
		__atexit = p;
	}
	p->fns[p->ind++] = fn;
	return (0);
}
