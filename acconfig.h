#ifndef __CONFIG_H
#  define __CONFIG_H
/*
*  config.h:  System and program specific preprocessor statements.
*
*
*  Copyright (C) 1994, 1995, 1996, 1997 Thomas Esken
*
*  This software doesn't claim completeness, correctness or usability.
*  On principle I will not be liable for ANY damages or losses (implicit
*  or explicit), which result from using or handling my software.
*  If you use this software, you agree without any exception to this
*  agreement, which binds you LEGALLY !!
*
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the `GNU General Public License' as published by
*  the `Free Software Foundation'; either version 2, or (at your option)
*  any later version.
*
*  You should have received a copy of the `GNU General Public License'
*  along with this program; if not, write to the:
*
*    Free Software Foundation, Inc.
*    59 Temple Place - Suite 330
*    Boston, MA 02111-1307,  USA
*/



/*
*  $Id: config.h 2.40 1997/01/26 02:04:00 tom Exp $
*/



@TOP@

/* Define to the name of the distribution.  */
#undef PACKAGE

/* Define to the version of the distribution.  */
#undef VERSION

/* Define as 1 if you have the stpcpy function.  */
#undef HAVE_STPCPY

/* Define to 1 if NLS is requested.  */
#undef ENABLE_NLS

/* Define as 1 if you have catgets and don't want to use GNU gettext.  */
#undef HAVE_CATGETS

/* Define as 1 if you have gettext and don't want to use GNU gettext.  */
#undef HAVE_GETTEXT

/* Define if your locale.h file contains LC_MESSAGES.  */
#undef HAVE_LC_MESSAGES

/*
 * Regular expression library.
 * Define exactly one of the following to be 1:
 * HAVE_GNU_RE_COMPILE_PATTERN: GNU re_compile_pattern() and regex.h
 * HAVE_POSIX_REGCOMP: POSIX regcomp() and regex.h
 * HAVE_RE_COMP: BSD re_comp()
 * HAVE_REGCMP: System V regcmp()
 * HAVE_V8_REGCOMP: Henry Spencer V8 regcomp() and regexp.h
 * NO_REGEX: pattern matching is supported, but without metacharacters.
 */
#undef HAVE_GNU_RE_COMPILE_PATTERN
#undef HAVE_POSIX_REGCOMP
#undef HAVE_RE_COMP
#undef HAVE_REGCMP
#undef HAVE_V8_REGCOMP
#undef NO_REGEX

/* Function/macro isalnum() is found.  */
#undef HAVE_ISALNUM

/* Function/macro isalpha() is found.  */
#undef HAVE_ISALPHA

/* Function/macro isdigit() is found.  */
#undef HAVE_ISDIGIT

/* Function/macro isspace() is found.  */
#undef HAVE_ISSPACE

/* Link -lncurses or -ltermcap or -ltermlib to executable.  */
#undef HAVE_TTYLIBS

/* Define if you have the m library (-lm), only necessary if USE_RC=1.  */
#undef HAVE_LIBM

/* The ospeed variable is defined in -lncurses or -ltermcap or -ltermlib.  */
#undef HAVE_OSPEED

/* Shell can interpret `#!' in first line of a shell script.  */
#undef HAVE_SYS_INTERPRETER

/* All termios functions are available.  */
#undef HAVE_TERMIOS_FUNCS

/* Type time_t is predefined.  */
#undef HAVE_TIME_T

/* Functions/macros toupper(), tolower(), isupper() and islower() are found.  */
#undef HAVE_UPPER_LOWER

/* Identifier void exists.  */
#undef HAVE_VOID

/* We must declare the ospeed variable.  */
#undef MUST_DCL_OSPEED

/* Enable preferred external pager program (`less').  */
#undef PAGER1_PROG

/* Enable alternative external pager program (`more').  */
#undef PAGER2_PROG

/* Enable second alternative external pager program (`pg').  */
#undef PAGER3_PROG

/* Enable displaying using the extended ASCII character set.  */
#undef USE_EASC

/* Enable highlighting sequences.  */
#undef USE_HLS

/* Enable simple internal pager feature.  */
#undef USE_PAGER

/* Enable fixed date feature.  */
#undef USE_RC

@BOTTOM@

#endif /* __CONFIG_H */
