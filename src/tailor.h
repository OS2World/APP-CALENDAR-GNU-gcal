#ifndef __TAILOR_H
#  define __TAILOR_H
/*
*  tailor.h:  Machine/target dependent definitions.
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
*  $Id: tailor.h 2.40 1997/02/14 02:04:00 tom Exp $
*/



/*
*  Include definitions (sub)header files  ;<
*/
#include "defines.h"
#if HAVE_CONFIG_H
#  include <config.h>
#endif
#if ENABLE_NLS && !USE_DE
/*
*  National language support is only allowed on these systems.
*/
#  define  GCAL_NLS
#  if HAVE_LOCALE_H
#    include <locale.h>
#  endif
#  include <libintl.h>
#if 0
#  define  _(Str)  (Str)
#endif
#  define  _(Str) gettext(Str)
#  ifdef gettext_noop
#    define  N_(Str)  gettext_noop(Str)
#  else /* !gettext_noop */
#    define  N_(Str)  (Str)
#  endif /* !gettext_noop */
#else /* !ENABLE_NLS || USE_DE */
#  define   _(Str)  (Str)
#  define  N_(Str)  (Str)
#endif /* !ENABLE_NLS || USE_DE */



/*
*  Detect the machine / os.
*/
/*
*  Define the symbol MSDOS for MS-DOS machines.
*/
#if defined(__MSDOS__) || defined(__MSDOS) || defined(_MSDOS)
#  ifndef MSDOS
#    define MSDOS     /* Now let's define the symbol MSDOS generally */
#  endif
#endif
#if defined(__TURBOC__) || defined(__ZTC__) || defined(M_I86)
#  ifndef MSDOS
#    define MSDOS     /* Now let's define the symbol MSDOS generally */
#  endif
#endif
/*
*  Define the symbol OS2 for OS/2 machines.
*/
#if defined(__OS2__) || defined(__OS2) || defined(_OS2)
#  ifndef OS2
#    define OS2       /* Now let's define the symbol OS2 generally */
#  endif
#endif
/*
*  Microsoft-C or Borland-C or Waterloo-C under OS/2.
*/
#if defined(OS2)
#  if defined(MSDOS) || defined(__TURBOC__)
#    ifdef MSDOS
#      undef MSDOS
#    endif
#  endif
#endif
/*
*  Define the symbol LINUX for Linux machines.
*/
#if defined(__LINUX__) || defined(__LINUX) || defined(_LINUX) || defined(LINUX)
#  ifndef LINUX
#    define LINUX     /* Now let's define the symbol LINUX generally */
#  endif
#endif
#if defined(__linux__) || defined(__linux) || defined(_linux) || defined(linux)
#  ifndef LINUX
#    define LINUX     /* Now let's define the symbol LINUX generally */
#  endif
#endif
/*
*  Define the symbol UNIX for UN*X machines.
*/
#if defined(__UNIX__) || defined(__UNIX) || defined(_UNIX)
#  ifndef UNIX
#    define UNIX      /* Now let's define the symbol UNIX generally */
#  endif
#endif
#if defined(__unix__) || defined(__unix) || defined(_unix) || defined(unix)
#  ifndef UNIX
#    define UNIX      /* Now let's define the symbol UNIX generally */
#  endif
#endif
#ifdef MSDOS
#  ifdef UNIX
#    if defined(__GNUC__) || defined(__GO32__)
#      ifndef DJG
#        define DJG   /* Now let's define the symbol DJG (DJ-GCC under MSDOS) for these machines */
#      endif
#    endif
#    undef MSDOS
#  else /* !UNIX */
#    if defined(__i386) || defined(i386)
#      ifndef SCO
#        define SCO   /* Now let's define the symbol SCO for these machines, too */
#      endif
#      undef MSDOS
#    endif
#  endif /* !UNIX */
#endif /* !MSDOS */



/*
*  Target specific predefines.
*/
#if defined(MSDOS) || defined(DJG) || defined(WIN32)
#              define  USE_PROTOTYPES 1
#              define  STDC_HEADERS 1
#              define  HAVE_ASSERT_H 1
#              define  HAVE_CTYPE_H 1
#              define  HAVE_ERRNO_H 1
#              define  HAVE_SIGNAL_H 1
#              define  HAVE_LIMITS_H 1
#              define  HAVE_SYS_STAT_H 1
#              define  HAVE_SYS_TYPES_H 1
#              define  HAVE_UPPER_LOWER 1
#              define  HAVE_ISDIGIT 1
#              define  HAVE_ISALPHA 1
#              define  HAVE_ISALNUM 1
#              define  HAVE_ISSPACE 1
#              define  HAVE_SIGNAL 1
#              define  HAVE_STRTOL 1
#              define  HAVE_STRSTR 1
#              define  HAVE_STRCSPN 1
#              define  HAVE_TIME_T 1
#              define  HAVE_VOID 1
#              ifdef DJG
#                define  HAVE_UNISTD_H 1
#                define  HAVE_MATH_H 1
#                define  HAVE_LIBM 1
#                define  DIR2_SEP      "/"         /* Directory/folder separator */
#  if 0
#                define  HAVE_REGEX_H 1
#                define  HAVE_GNU_RE_COMPILE_PATTERN 1
#  endif
#                define  HAVE_V8_REGCOMP 1
#              else /* !DJG */
#                include <io.h>
#                define  HAVE_V8_REGCOMP 1
#              endif /* !DJG */
#              if USE_PAGER
#                define  SCREEN_ROWS   25          /* Default number of terminal rows */
#              endif
#              define  DISK_SEP      ":"         /* Disk/Drive separator */
#              define  DIR_SEP       "\\"        /* Directory/folder separator */
#              define  PATH_SEP      ";"         /* Path separator */
#  if 0
#              define  ENV_VAR_DPATH "PATH"      /* Name of data search path environment variable */
#  endif
#else /* !MSDOS && !DJG && !WIN32 */
#  if defined(OS2)
#              define  USE_PROTOTYPES 1
#    if !defined(__GNUC__) && !defined(__EMX__)
#              define  STDC_HEADERS 1
#              define  HAVE_ASSERT_H 1
#              define  HAVE_CTYPE_H 1
#              define  HAVE_ERRNO_H 1
#              define  HAVE_LIMITS_H 1
#              define  HAVE_SIGNAL_H 1
#              define  HAVE_SYS_STAT_H 1
#              define  HAVE_SYS_TYPES_H 1
#              define  HAVE_UPPER_LOWER 1
#              define  HAVE_ISDIGIT 1
#              define  HAVE_ISALPHA 1
#              define  HAVE_ISALNUM 1
#              define  HAVE_ISSPACE 1
#              define  HAVE_SIGNAL 1
#              define  HAVE_STRTOL 1
#              define  HAVE_STRSTR 1
#              define  HAVE_STRCSPN 1
#              define  HAVE_TIME_T 1
#              define  HAVE_VOID 1
#              define  HAVE_V8_REGCOMP 1
#    endif /* !__GCC__ && !__EMX__ */
#  if 0
#              define  HAVE_REGEX_H 1
#              define  HAVE_GNU_RE_COMPILE_PATTERN 1
#              define  HAVE_MATH_H 1
#              define  HAVE_LIBM 1
#              define  HAVE_LIBTERMCAP 1
#              define  HAVE_TERMCAP_H 1
#              define  HAVE_TERMIOS_H 1
#              define  HAVE_TERMIOS_FUNCS 1
#              define  HAVE_UNISTD_H 1
#              define  HAVE_FCNTL_H 1
#              define  HAVE_SYS_IOCTL_H 1
#              define  HAVE_SYS_STREAM_H 1
#              define  HAVE_SYS_TIME_H 1
#              define  HAVE_SYS_WAIT_H 1
#              define  HAVE_DUP 1
#              define  HAVE_DUP2 1
#              define  HAVE_STRCHR 1
#              define  HAVE_STRRCHR 1
#  endif
#              include <io.h>
#              if USE_PAGER
#                define  SCREEN_ROWS   25          /* Default number of terminal rows */
#              endif
#              define  DISK_SEP      ":"         /* Disk/Drive separator */
#              define  DIR_SEP       "\\"        /* Directory/folder separator */
#              define  PATH_SEP      ";"         /* Path separator */
#  if 0
#              define  ENV_VAR_DPATH "DPATH"     /* Name of data search path environment variable */
#  endif
#              ifdef __GNUC__
#                define  ENV_VAR_TCAP  "TERMCAP"      /* Env-var holding path to termcap data base file */
#                define  FNAME_TCAP    "termcap.dat"  /* Name of termcap database file */
#                define  DFLT_TERM     "ansi"         /* Name of default terminal type */
#              endif
#  else /* !OS2 */
#    if defined(ACORN)
#              define  USE_PROTOTYPES 1
#              define  STDC_HEADERS 1
#              define  HAVE_ASSERT_H 1
#              define  HAVE_CTYPE_H 1
#              define  HAVE_ERRNO_H 1
#              define  HAVE_LIMITS_H 1
#              define  HAVE_SIGNAL_H 1
#              define  HAVE_SYS_STAT_H 1
#              define  HAVE_SYS_TYPES_H 1
#              define  HAVE_UPPER_LOWER 1
#              define  HAVE_ISDIGIT 1
#              define  HAVE_ISALPHA 1
#              define  HAVE_ISALNUM 1
#              define  HAVE_ISSPACE 1
#              define  HAVE_SIGNAL 1
#              define  HAVE_STRTOL 1
#              define  HAVE_STRSTR 1
#              define  HAVE_STRCSPN 1
#              define  HAVE_TIME_T 1
#              define  HAVE_VOID 1
#              define  DISK_SEP      "$"         /* Disk/Drive separator */
#              define  DIR_SEP       "."         /* Directory/Folder separator */
#              define  PATH_SEP      ","         /* Path separator */
#              define  ENV_VAR_PATH  "Run$Path"  /* Name of program search path environment variable */
#  if 0
#              define  ENV_VAR_DPATH "Run$Path"  /* Name of data search path environment variable */
#  endif
#              ifdef __GNUC__
#                define  HAVE_UNISTD_H 1
#  if 0
#                define  HAVE_REGEX_H 1
#                define  HAVE_GNU_RE_COMPILE_PATTERN 1
#  endif
#                define  HAVE_V8_REGCOMP 1
#              else /* !__GNUC__ */
#                define  HAVE_V8_REGCOMP 1
#              endif /* !__GNUC__ */
#    else /* !ACORN */
#      if defined(AMIGA)
#              define  USE_PROTOTYPES 1
#              define  STDC_HEADERS 1
#              define  HAVE_ASSERT_H 1
#              define  HAVE_CTYPE_H 1
#              define  HAVE_ERRNO_H 1
#              define  HAVE_LIMITS_H 1
#              define  HAVE_SIGNAL_H 1
#              define  HAVE_SYS_STAT_H 1
#              define  HAVE_SYS_TYPES_H 1
#              define  HAVE_UPPER_LOWER 1
#              define  HAVE_ISDIGIT 1
#              define  HAVE_ISALPHA 1
#              define  HAVE_ISALNUM 1
#              define  HAVE_ISSPACE 1
#              define  HAVE_SIGNAL 1
#              define  HAVE_STRTOL 1
#              define  HAVE_STRSTR 1
#              define  HAVE_STRCSPN 1
#              define  HAVE_TIME_T 1
#              define  HAVE_VOID 1
#              ifdef __GNUC__
#                define  HAVE_UNISTD_H 1
#  if 0
#                define  HAVE_REGEX_H 1
#                define  HAVE_GNU_RE_COMPILE_PATTERN 1
#  endif
#                define  HAVE_V8_REGCOMP 1
#              else /* !__GNUC__ */
#                define  HAVE_V8_REGCOMP 1
#              endif /* !__GNUC__ */
#      else /* !AMIGA */
#        if defined(ATARI)
#              define  USE_PROTOTYPES 1
#              ifdef __GNUC__
#                ifndef STDC_HEADERS
#                  define  STDC_HEADERS 1
#                endif
#                define  HAVE_UNISTD_H 1
#  if 0
#                define  HAVE_REGEX_H 1
#                define  HAVE_GNU_RE_COMPILE_PATTERN 1
#  endif
#                define  HAVE_V8_REGCOMP 1
#              else /* !__GNUC__ */
#                define  STDC_HEADERS 1
#                define  HAVE_V8_REGCOMP 1
#              endif /* !__GNUC__ */
#              define  HAVE_ASSERT_H 1
#              define  HAVE_CTYPE_H 1
#              define  HAVE_ERRNO_H 1
#              define  HAVE_LIMITS_H 1
#              define  HAVE_SIGNAL_H 1
#              define  HAVE_SYS_STAT_H 1
#              define  HAVE_SYS_TYPES_H 1
#              define  HAVE_UPPER_LOWER 1
#              define  HAVE_ISDIGIT 1
#              define  HAVE_ISALPHA 1
#              define  HAVE_ISALNUM 1
#              define  HAVE_ISSPACE 1
#              define  HAVE_SIGNAL 1
#              define  HAVE_STRTOL 1
#              define  HAVE_STRSTR 1
#              define  HAVE_STRCSPN 1
#              define  HAVE_TIME_T 1
#              define  HAVE_VOID 1
#              define  DIR_SEP       "\\"        /* Directory/folder separator */
#              define  PATH_SEP      ","         /* Path separator */
#        else /* !ATARI */
#          if defined(__50SERIES) || defined(PRIMOS)   /* Prime/PRIMOS */
#              define  USE_PROTOTYPES 1
#              define  HAVE_V8_REGCOMP 1
#              define  STDC_HEADERS 1
#              define  HAVE_ASSERT_H 1
#              define  HAVE_CTYPE_H 1
#              define  HAVE_ERRNO_H 1
#              define  HAVE_LIMITS_H 1
#              define  HAVE_SIGNAL_H 1
#              define  HAVE_SYS_STAT_H 1
#              define  HAVE_SYS_TYPES_H 1
#              define  HAVE_UPPER_LOWER 1
#              define  HAVE_ISDIGIT 1
#              define  HAVE_ISALPHA 1
#              define  HAVE_ISALNUM 1
#              define  HAVE_ISSPACE 1
#              define  HAVE_SIGNAL 1
#              define  HAVE_STRTOL 1
#              define  HAVE_STRSTR 1
#              define  HAVE_STRCSPN 1
#              define  HAVE_TIME_T 1
#              define  HAVE_VOID 1
#              define  DIR_SEP       ">"         /* Directory/folder separator */
#          else /* !__50SERIES */
#            if defined(VAXC) || defined(VMS)  /* DEC Vax */
#              ifndef UNIX
#                define  UNIX     /* Now let's define the symbol UNIX as default */
#              endif
#              define  USE_PROTOTYPES 1
#              ifdef __GNUC__
#                define  HAVE_REGEX_H 1
#                define  HAVE_GNU_RE_COMPILE_PATTERN 1
#              else /* !__GNUC__ */
#                define  HAVE_V8_REGCOMP 1
#              endif /* !__GNUC__ */
#              define  STDC_HEADERS 1
#              define  HAVE_ASSERT_H 1
#              define  HAVE_CTYPE_H 1
#              define  HAVE_ERRNO_H 1
#              define  HAVE_LIMITS_H 1
#              define  HAVE_SIGNAL_H 1
#              define  HAVE_SYS_STAT_H 1
#              define  HAVE_SYS_TYPES_H 1
#              define  HAVE_UPPER_LOWER 1
#              define  HAVE_ISDIGIT 1
#              define  HAVE_ISALPHA 1
#              define  HAVE_ISALNUM 1
#              define  HAVE_ISSPACE 1
#              define  HAVE_SIGNAL 1
#              define  HAVE_STRTOL 1
#              define  HAVE_STRSTR 1
#              define  HAVE_STRCSPN 1
#              define  HAVE_TIME_T 1
#              define  HAVE_VOID 1
#              ifdef VAXC
#                include <unixio.h>
#              endif
#              define  DISK_SEP       ":"        /* Disk/Drive separator */
#              define  DIR_SEP        "]"        /* Directory/folder separator */
#              define  SUFFIX_SEP     ";"        /* Suffix (version number) separator */
#              define  ENV_VAR_GCAL   "GCAL_OPT" /* Name of programs default options variable */
#            else /* default: UNIX.  Let `configure' detect what's in store .) */
#              ifndef UNIX
#                define  UNIX     /* Now let's define the symbol UNIX (NOT GNU or RMS, hehe) as default */
#              endif
#            endif /* default: UNIX */
#          endif /* !__50SERIES */
#        endif /* !ATARI */
#      endif /* !AMIGA */
#    endif /* !ACORN */
#  endif /* !OS2 */
#endif /* !MSDOS && !DJG && !WIN32 */



/*
*  More precise attributes.
*/
#define  LOCAL           static
#define  PUBLIC       /* Public */
#define  IMPORT          extern
#define  EXPORT          PUBLIC



/*
*  Compiler specific macro for prototyping.
*/
#ifndef __P_
#  if __STDC__ || defined(__cplusplus) || USE_PROTOTYPES
#    define  __P_(gc_proto)  gc_proto
#  else /* !__STDC__ && !__cplusplus && !USE_PROTOTYPES */
#    define  __P_(gc_proto)  ()
#  endif /* !__STDC__ && !__cplusplus && !USE_PROTOTYPES */
#endif



/*
*  Action specific preprocessor constants.
*/
/*
*  Termcap action is only allowed on these systems.
*/
#ifndef DJG
#  if defined(UNIX) || (defined(OS2) && defined(__GNUC__))
#    if HAVE_TTYLIBS
#      define  GCAL_TCAP
#    endif
#  endif
#endif
/*
*  Writing a shell script is only allowed on these systems.
*/
#if defined(UNIX) && !defined(DJG)
#  define  GCAL_SHELL
#endif
/*
*  Using an external pager is only allowed on these systems.
*/
#if defined(UNIX) && !defined(DJG) && USE_PAGER
#  define  GCAL_EPAGER
#endif
/*
*  Email option is only allowed on these systems.
*/
#if defined(__GNUC__) || defined(UNIX) || defined(OS2) || defined(MSDOS) || defined(DJG) || defined(WIN32)
#  define  GCAL_EMAIL
#endif



/*
*  Target specific preprocessor statements.
*/
#ifdef GCAL_TCAP
#  ifndef ENV_VAR_TERM
#    define  ENV_VAR_TERM  "TERM"           /* Name of terminal mode/type environment varable */
#  endif
#endif
#ifdef GCAL_SHELL
#  define  SHL_REM       "#"              /* The comment character used by shell scripts */
#  define  SHL_ESC       "\\"             /* The escape character used by shell scripts */
#  ifndef CHMOD_PRGR
#    define  CHMOD_PRGR    "chmod"          /* Name of default `chmod' program */
#  endif
#  define  CHMOD_OPTS    "+x"             /* Make created shell script executable for all users */
#  if HAVE_SYS_INTERPRETER
#    ifndef SHELL
#      define  SHELL         "! /bin/sh"     /* Id for the first line of a shell script */
#    endif
#  endif
#endif
#ifdef GCAL_EMAIL
#  ifndef REDIRECT
#    define REDIRECT       "<"              /* Input redirection symbol */
#  endif
#  ifndef ENV_VAR_MAILPROG
#    define  ENV_VAR_MAILPROG  "MAILPROG"   /* Stores the name of default mailer program in environment */
#  endif
#  ifndef MAIL_PRGR
#    define  MAIL_PRGR     "mail"           /* Name of default mailer if $MAILPROG is not set */
#  endif
#endif /* GCAL_EMAIL */
#if USE_PAGER
#  ifndef SCREEN_ROWS
#    define SCREEN_ROWS    24               /* Default number of terminal rows */
#  endif
#  ifndef SCREEN_COLS
#    define  SCREEN_COLS   80               /* Default number of terminal columns */
#  endif
#  ifdef GCAL_EPAGER
#    define  ENV_VAR_PAGER "PAGER"          /* Name of external pager program environment variable */
#    ifndef PAGER1_PROG
#      define  PAGER1_PROG   "less"           /* Name of preferred external pager program */
#    endif
#    ifndef PAGER2_PROG
#      define  PAGER2_PROG   "more"           /* Name of alternative external pager program */
#    endif
#    ifndef PAGER3_PROG
#      define  PAGER3_PROG   "pg"             /* Name of second alternative external pager program */
#    endif
#  endif /* GCAL_EPAGER */
#endif /* USE_PAGER */
#ifdef GCAL_NLS
#  define  ENV_VAR_LANGUAGE     "LANGUAGE"    /* Name of first used national language environment variable */
#  define  ENV_VAR_LC_ALL       "LC_ALL"      /* Name of second used national language environment variable */
#  if HAVE_LC_MESSAGES
#    define  ENV_VAR_LC_MESSAGES  "LC_MESSAGES" /* Name of third used national language environment variable */
#  endif
#  define  ENV_VAR_LANG         "LANG"        /* Name of fourth used national language environment variable */
#endif
#ifndef ACTUAL_DIR
#  define  ACTUAL_DIR    "."              /* The character used to specify the actual directory folder */
#endif
#ifndef DIR_SEP
#  define  DIR_SEP       "/"              /* Default directory/folder separator */
#endif
#ifndef PATH_SEP
#  define  PATH_SEP      ":"              /* Default path separator */
#endif
#ifndef ENV_VAR_HOME
#  define  ENV_VAR_HOME  "HOME"           /* Name of home directory environment variable */
#endif
#ifndef ENV_VAR_PATH
#  define  ENV_VAR_PATH  "PATH"           /* Name of program search path environment variable */
#endif
#ifndef ENV_VAR_DPATH
#  define  ENV_VAR_DPATH "GCALPATH"       /* Name of data search path environment variable */
#endif
#ifndef ENV_VAR_GCAL
#  define  ENV_VAR_GCAL  "GCAL"           /* Name of programs default options variable */
#endif
#ifndef ENV_VAR_GCALANSI
#  define  ENV_VAR_GCALANSI  "GCALANSI"   /* Name of variable for setting ANSI-sequences to default */
#endif
#ifndef ENV_VAR_GCAL_DATE_FORMAT
#  define  ENV_VAR_GCAL_DATE_FORMAT  "GCAL_DATE_FORMAT"   /* Name of variable for ordering the date */
#endif
/*
*  Now define the user and system data directories of Gcal's resource files.
*  Please note:  If your compiler does not support the ANSI-C string
*                  concatenation feature like `"foo""bar" => "foobar"'
*                  you have to provide your Makefile with the appropriate
*                  texts referenced by GCAL_USR_DATADIR and GCAL_SYS_DATADIR
*                  to avoid compilation errors.
*/
#if USE_RC
#  ifndef  GCAL_USR_DATADIR
#    define  GCAL_USR_DATADIR  "share"DIR_SEP""PRGR_NAME                                  /* relative to $HOME dir */
#  endif
#  ifndef  GCAL_SYS_DATADIR
#    define  GCAL_SYS_DATADIR  DIR_SEP"usr"DIR_SEP"local"DIR_SEP"share"DIR_SEP""PRGR_NAME /* absolute to root dir */
#  endif
#endif /* USE_RC */



/*
*  Define how the German `Umlaute' letters are displayed
*    and how the default marking characters are displayed.
*/
#if USE_EASC
#  if defined(NeXT)
         /* NeXTstep character set */
#        define  AE          "\331"   /* \xd9 */
#        define  OE          "\360"   /* \xf0 */
#        define  UE          "\366"   /* \xf6 */
#        define  SZ          "\373"   /* \xfb */
#        define  AAE         "\205"   /* \x85 */
#        define  OOE         "\226"   /* \x96 */
#        define  UUE         "\232"   /* \x9a */
#        define  BUF_HLS1S   "<"      /* Marking character 1 start (current day) */
#        define  BUF_HLS1E   ">"      /* Marking character 1 end (current day) */
#        define  BUF_HLS2S   ":"      /* Marking character 2 start (holiday) */
#        define  BUF_HLS2E   ":"      /* Marking character 2 end (holiday) */
#  else
#    if defined(ATARI)
         /* Atari character set */
#        define  AE          "\204"   /* \x84 */
#        define  OE          "\224"   /* \x94 */
#        define  UE          "\201"   /* \x81 */
#        define  SZ          "\236"   /* \x9e */
#        define  AAE         "\216"   /* \x8e */
#        define  OOE         "\231"   /* \x99 */
#        define  UUE         "\232"   /* \x9a */
#        define  BUF_HLS1S   "<"      /* Marking character 1 start (current day) */
#        define  BUF_HLS1E   ">"      /* Marking character 1 end (current day) */
#        define  BUF_HLS2S   ":"      /* Marking character 2 start (holiday) */
#        define  BUF_HLS2E   ":"      /* Marking character 2 end (holiday) */
#    else
#      if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(SCO) || defined(COHERENT)
         /* IBM PC character set */
#        define  AE          "\204"   /* \x84 */
#        define  OE          "\224"   /* \x94 */
#        define  UE          "\201"   /* \x81 */
#        define  SZ          "\341"   /* \xe1 */
#        define  AAE         "\216"   /* \x8e */
#        define  OOE         "\231"   /* \x99 */
#        define  UUE         "\232"   /* \x9a */
#        define  BUF_HLS1S   "\021"   /* \x11 Marking character 1 start (current day) */
#        define  BUF_HLS1E   "\020"   /* \x10 Marking character 1 end (current day) */
#        define  BUF_HLS2S   "\256"   /* \xae Marking character 2 start (holiday) */
#        define  BUF_HLS2E   "\257"   /* \xaf Marking character 2 end (holiday) */
#      else
         /* (defined(UNIX) && !defined(SCO) && !defined(COHERENT) && !defined(NeXT)) || defined(AMIGA) || ... */
         /* ISO Latin-1 character set */
#        define  AE          "\344"   /* \xe4 */
#        define  OE          "\366"   /* \xf6 */
#        define  UE          "\374"   /* \xfc */
#        define  SZ          "\337"   /* \xdf */
#        define  AAE         "\304"   /* \xc4 */
#        define  OOE         "\326"   /* \xd6 */
#        define  UUE         "\334"   /* \xdc */
#        define  BUF_HLS1S   "<"      /* Marking character 1 start (current day) */
#        define  BUF_HLS1E   ">"      /* Marking character 1 end (current day) */
#        define  BUF_HLS2S   ":"      /* Marking character 2 start (holiday) */
#        define  BUF_HLS2E   ":"      /* Marking character 2 end (holiday) */
#      endif
#    endif
#  endif
#else /* !USE_EASC */
#  define  AE          "ae"
#  define  OE          "oe"
#  define  UE          "ue"
#  define  SZ          "ss"
#  define  AAE         "AE"
#  define  OOE         "OE"
#  define  UUE         "UE"
#  define  BUF_HLS1S    "<"          /* Marking character 1 start (current day) */
#  define  BUF_HLS1E    ">"          /* Marking character 1 end (current day) */
#  define  BUF_HLS2S    ":"          /* Marking character 2 start (holiday) */
#  define  BUF_HLS2E    ":"          /* Marking character 2 end (holiday) */
#endif /* !USE_EASC */



/*
*  Define the default highlighting sequences.
*/
#if USE_HLS
#  if defined(MSDOS) || defined(OS2) || defined(LINUX) || defined(DJG)
#    define  HLS1S        "\033[1m"    /* Highlighting sequence 1 start (current day) */
#    define  HLS2S        "\033[36m"   /* Highlighting sequence 2 start (holiday) */
#  else /* !MSDOS && !OS2 && !LINUX && !DJG */
#    define  HLS1S        "\033[7m"    /* Highlighting sequence 1 start (current day) */
#    define  HLS2S        "\033[1m"    /* Highlighting sequence 2 start (holiday) */
#  endif /* !MSDOS && !OS2 && !LINUX && !DJG */
#  define  HLS1E        "\033[0m"    /* Highlighting sequence 1 end (current day) */
#  define  HLS2E        "\033[0m"    /* Highlighting sequence 2 end (holiday) */
#  define  TC_MC_HL1S   "mr"         /* Termcap mode character HL 1 start (current day) */
#  define  TC_MC_HL1E   "me"         /* Termcap mode character HL 1 end (current day) */
#  define  TC_MC_HL2S   "md"         /* Termcap mode character HL 2 start (holiday) */
#  define  TC_MC_HL2E   "me"         /* Termcap mode character HL 2 end (holiday) */
#else /* !USE_HLS */
#  define  HLS1S        BUF_HLS1S    /* Highlighting sequence 1 start (current day) */
#  define  HLS1E        BUF_HLS1E    /* Highlighting sequence 1 end (current day) */
#  define  HLS2S        BUF_HLS2S    /* Highlighting sequence 2 start (holiday) */
#  define  HLS2E        BUF_HLS2E    /* Highlighting sequence 2 end (holiday) */
#endif /* !USE_HLS */
#define  TC_MC_MAX    4            /* Maximum number of termcap mode chars/single HL sequences used */
#define  NO_HLS       " "          /* Print blank character if highlighting is disabled */
#endif /* __TAILOR_H */
