/* This is file ASIN.S */
/*
** Copyright (C) 1991 DJ Delorie, 24 Kirsten Ave, Rochester NH 03867-2954
**
** This file is distributed under the terms listed in the document
** "copying.dj", available from DJ Delorie at the address above.
** A copy of "copying.dj" should accompany this file; if not, a copy
** should be available from where this file was obtained.  This file
** may not be distributed without a verbatim copy of "copying.dj".
**
** This file is distributed WITHOUT ANY WARRANTY; without even the implied
** warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

/* History:11,23 */
	.text
	.align	2
	.globl	_asin
_asin:
	fldl	4(%esp)
	fld	%st(0)
	fmulp
	fld1
	fsubp
	fsqrt
	fldl	4(%esp)
	fxch	%st(1)
	fpatan
	ret
