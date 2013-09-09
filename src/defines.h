#ifndef __DEFINES_H
#  define __DEFINES_H
/*
*  defines.h:  Program name, release number and modification symbols.
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
*  $Id: defines.h 2.40 1997/04/23 02:04:00 tom Exp $
*/



/*
*  Define the program name and version.
*/
#define  PRGR_NAME    "gcal"   /* Actual program name, 6 characters maximum */
#define  VERSION_NO   "2.40"   /* Actual version number */



/*
*
*  BEGIN: General program modification symbols,
*           which may be changed by the user !!
*
*/

/*
*  To adapt this program, EITHER define/activate the following symbols in
*    this file OR add the following symbols in Makefile (-D`symbol=value'...)
*    (this all isn't necessary if the GNU Autoconf(igure) technology is used):
*
*  a)  - Define `USE_EASC 1' for using an extended ASCII character set.
*      - Delete the symbol USE_EASC or write `USE_EASC 0' for using the
*          ISO-ASCII/EBCDIC character set...
*
*  b)  - Define `USE_DE 1' for using German messages and calendar style.
*      - Delete the symbol USE_DE or write `USE_DE 0' for using
*          English messages and calendar style...
*
*  c)  - Define `USE_HLS 1' to run this program using control sequences
*          for highlighting the current day/holidays/text (MS/PC-DOS will
*          use the [n]ansi.sys driver for emitting ANSI Escape highlighting
*          sequences; OS2/Linux/UN*X-like systems will use `termcap' for
*          emitting the terminal specific highlighting sequences.  If this
*          fails, default (generic) ANSI highlighting sequences will be used).
*      - Delete the symbol USE_HLS or write `USE_HLS 0' to run this program
*          without highlighting...
*
*  d)  - Define `USE_PAGER 1' for using an external pager (availabe only
*          on UN*X-like systems) or for using the simple, internal pager
*          (if external pager fails).
*      - Delete the symbol USE_PAGER or write `USE_PAGER 0' if you don't
*          like this feature...
*
*  e)  - Define `USE_RC 1' for using the special month dates functions
*          (print daily/weekly/monthly/yearly/eternal/special fixed dates
*          texts which are stated in the resource file(s)).
*      - Delete the symbol USE_RC or write `USE_RC 0' if you don't
*          like this feature...
*/

#if 0
#  define  USE_EASC   0
#  define  USE_DE     0
#  define  USE_HLS    1
#  define  USE_PAGER  1
#  define  USE_RC     1
#endif

/*
*
*  END: General program modification symbols,
*         which may be changed by the user !!
*
*/
#endif /* __DEFINES_H */
