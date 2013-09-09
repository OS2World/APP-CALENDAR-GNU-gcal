#ifndef __GCAL_H
#  define __GCAL_H
/*
*  gcal.h:  Program specific macros, typedefs, prototypes...
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
*  $Id: gcal.h 2.40 1997/04/09 02:04:00 tom Exp $
*/



/*
*  Include (sub)header files   ;<
*/
#if HAVE_STDIO_H || STDC_HEADERS
#  include <stdio.h>
#endif
#if HAVE_STDLIB_H || STDC_HEADERS
#  include <stdlib.h>
#endif
#if HAVE_STRING_H || STDC_HEADERS
#  include <string.h>
#else /* !HAVE_STRING_H && !STDC_HEADERS */
#  include <strings.h>
#  if !HAVE_STRCHR
#    ifndef strchr
#      define strchr       index
IMPORT char *strchr __P_((const char *s, int c));
#    endif
#  endif /* !HAVE_STRCHR */
#  if !HAVE_STRRCHR
#    ifndef strrchr
#      define strrchr      rindex
IMPORT char *strrchr __P_((const char *s, int c));
#    endif
#  endif /* !HAVE_STRRCHR */
#endif
#if HAVE_SIGNAL_H && HAVE_SIGNAL
#  if HAVE_SYS_TYPES_H
#    include <sys/types.h>
#  endif
#  include <signal.h>
#endif
#ifdef __50SERIES
IMPORT char *getenv __P_((const char *env_var));
#  if HAVE_SIGNAL && defined(SIGTERM)
#    undef  SIGTERM   /* We don't want a signal handler for SIGTERM */
#  endif
#endif



/*
*  Basic preprocessor statements and macros.
*/
#ifndef SPECIAL_VALUE
#  define SPECIAL_VALUE    (-1)
#endif
#ifndef TRUE
#  define  TRUE            (0==0)
#endif
#ifndef FALSE
#  define  FALSE           (!TRUE)
#endif
#define  LOOP              for(;;)
#if HAVE_VOID
#  define  VOID_PTR        void *
#else /* !HAVE_VOID */
#  define  VOID_PTR        char *
#  define  void            int
#endif /* !HAVE_VOID */
#ifndef RETSIGTYPE
#  define  RETSIGTYPE      void
#endif
#ifndef NULL
#  if !defined __cplusplus || defined __GNUC__
#    define  NULL            ((VOID_PTR)0)
#  else /* __cplusplus && !__GNUC__ */
#    define  NULL            (0)
#  endif /* __cplusplus && !__GNUC__ */
#endif
#if !HAVE_STRSTR
#  ifndef strstr
#    define  strstr          my_strstr
#  endif
#endif
#if !HAVE_STRCSPN
#  ifndef strcspn
#    define strcspn          my_strcspn
#  endif
#endif
#if !HAVE_STRCASECMP
#  ifndef strcasecmp
#    define  strcasecmp      my_strcasecmp
#  endif
#endif
#if !HAVE_STRNCASECMP
#  ifndef strncasecmp
#    define  strncasecmp     my_strncasecmp
#  endif
#endif
#if HAVE_TIME_T
#  define  MY_TIME_T       time_t
#else /* !HAVE_TIME_T */
#  define  MY_TIME_T       long
#endif /* !HAVE_TIME_T */
#if !HAVE_UPPER_LOWER
#  ifndef islower
#    define  islower(gc_c) (((gc_c) >= 'a') && ((gc_c) <= 'z'))
#  endif
#  ifndef isupper
#    define  isupper(gc_c) (((gc_c) >= 'A') && ((gc_c) <= 'Z'))
#  endif
#  ifndef tolower
#    define  tolower(gc_c) ((isupper(gc_c)) ? (gc_c) - 'A' + 'a' : (gc_c))
#  endif
#  ifndef toupper
#    define  toupper(gc_c) ((islower(gc_c)) ? (gc_c) - 'a' + 'A' : (gc_c))
#  endif
#endif
#if !HAVE_ISDIGIT
#  ifndef isdigit
#    define  isdigit(gc_c) (((gc_c) >= '0') && ((gc_c) <= '9'))
#  endif
#endif
#if !HAVE_ISALPHA
#  ifndef isalpha
#    define  isalpha(gc_c) (isupper(gc_c) || islower(gc_c))
#  endif
#endif
#if !HAVE_ISALNUM
#  ifndef isalnum
#    define  isalnum(gc_c) (isalpha(gc_c) || isdigit(gc_c))
#  endif
#endif
#if !HAVE_ISSPACE
#  ifndef isspace
#    define  isspace(gc_c) (   (gc_c) == ' '  || (gc_c) == '\f' || (gc_c) == '\n' \
                            || (gc_c) == '\r' || (gc_c) == '\t' || (gc_c) == '\v')
#  endif
#endif


/*
*  Program specific preprocessor statements.
*/
#ifndef BUF_LEN
#  define BUF_LEN         16384        /* Maximum length of a file block buffer */
#endif
#ifndef MAXLEN_MAX
#  define  MAXLEN_MAX     1024         /* Default length of a "string" text buffer */
#endif
#ifndef MY_ARGC_MAX
#  define  MY_ARGC_MAX    64           /* Default number of command line arguments in `my_argv[]' table */
#endif
#ifndef  EXIT_STAT_HLP
#  define  EXIT_STAT_HLP  0            /* Programs exit status on: help, version, license */
#endif
#define  HD_MAX         372          /* Maximum number of holidays per year (12*31) */
#define  HD_ELEMS_MAX   512          /* Maximum amount of eternal holidays in the table */
#define  LEN_HD_NAME    37           /* Maximum length of a holiday name inclusive `\0' */
#define  WARN_LVL_MAX   4            /* Maximum amount of debug/warning levels */
/*
*  ISO-3166 A2 (2-letters) country codes of supported country specific holidays.
*/
#define  CC_AT  "AT"
#define  CC_AU  "AU"
#define  CC_BE  "BE"
#define  CC_CA  "CA"
#define  CC_CH  "CH"
#define  CC_CZ  "CZ"
#define  CC_DE  "DE"
#define  CC_DK  "DK"
#define  CC_ES  "ES"
#define  CC_FI  "FI"
#define  CC_FR  "FR"
#define  CC_GB  "GB"
#define  CC_GR  "GR"
#define  CC_HU  "HU"
#define  CC_IT  "IT"
#define  CC_MX  "MX"
#define  CC_NL  "NL"
#define  CC_NO  "NO"
#define  CC_PL  "PL"
#define  CC_PT  "PT"
#define  CC_SE  "SE"
#define  CC_SI  "SI"
#define  CC_TH  "TH"
#define  CC_US  "US"
/*
*  Preprocessor statements for long options `Lopt_struct' field `symbolic_name'.
*/
#define  SYM_NIL                    0
/*
*  Common values.
*/
#define  SYM_BLOCKS                 1
#define  SYM_CALENDAR_DATES         2
#define  SYM_DATE_FORMAT            3
#define  SYM_DEBUG                  4
#define  SYM_DESC_HOLIDAY_LIST      5
#define  SYM_DISABLE_HIGHLIGHTING   6
#define  SYM_EXCLUDE_HD_TITLE       7
#define  SYM_EXIT_STAT_HELP_127     8
#define  SYM_FORCE_HIGHLIGHTING     9
#define  SYM_GREG_REFORM            10
#define  SYM_HELP                   11
#define  SYM_HIGHLIGHTING           12
#define  SYM_HOLIDAY_DATES          13
#define  SYM_HOLIDAY_LIST           14
#define  SYM_LICENSE1               15
#define  SYM_LICENSE2               16
#define  SYM_LICENSE3               17
#define  SYM_LONG_HELP1             18
#define  SYM_LONG_HELP2             19
#define  SYM_ORTHODOX_CALENDAR      20
#define  SYM_RESPONSE_FILE          21
#define  SYM_STARTING_DAY           22
#define  SYM_SUPPRESS_CALENDAR      23
#define  SYM_TYPE_OF_CALENDAR       24
#define  SYM_VERSION                25
#define  SYM_CALENDAR_WITH_WEEKNO   26
/*
*  Holiday values.
*/
#define  SYM_STANDARD_HDY           50
#define  SYM_CHRISTIAN_HDY          51
#define  SYM_CC_HDY                 52
/*
*  System depending values.
*/
#ifdef GCAL_SHELL
#  define  SYM_SCRIPT_FILE            100
#endif
#ifdef GCAL_EMAIL
#  define  SYM_MAIL                   101
#endif
/*
*  Configuration depending values.
*/
#if USE_PAGER
#  define  SYM_PAGER                  150
#endif
#if USE_RC
#  define  SYM_ALTERNATIVE_FORMAT     200
#  define  SYM_BIORHYTHM_AXIS_LEN     201
#  define  SYM_BYPASS_SHELL_CMD       202
#  define  SYM_DATE_VARIABLE          203
#  define  SYM_DESC_FIXED_DATES_LIST  204
#  define  SYM_END_OF_MONTH           205
#  define  SYM_END_OF_WEEK            206
#  define  SYM_END_OF_YEAR            207
#  define  SYM_EXCLUDE_RC_TITLE       208
#  define  SYM_EXPORT_LOCAL_DVARS     209
#  define  SYM_EXPORT_LOCAL_TVARS     210
#  define  SYM_FILTER_DAY             211
#  define  SYM_FILTER_PERIOD          212
#  define  SYM_FILTER_TEXT            213
#  define  SYM_FIXED_DATES            214
#  define  SYM_FIXED_DATES_LIST       215
#  define  SYM_GROUPING_TEXT          216
#  define  SYM_HERE_FILE              217
#  define  SYM_INCLUDE_CONS_NO        218
#  define  SYM_INCLUDE_FILENAME       219
#  define  SYM_INCLUDE_HOLIDAY        220
#  define  SYM_INCLUDE_TODAY          221
#  define  SYM_INCLUDE_WEEK_NO        222
#  define  SYM_LEAP_DAY               223
#  define  SYM_LIST_MODE              224
#  define  SYM_MONTH                  225
#  define  SYM_MOONIMAGE_LINES        226
#  define  SYM_OMIT_DATE_PART         227
#  define  SYM_PERIOD                 228
#  define  SYM_RESOURCE_FILE          229
#  define  SYM_START_OF_MONTH         230
#  define  SYM_START_OF_WEEK          231
#  define  SYM_START_OF_YEAR          232
#  define  SYM_SUPPRESS_DATE_PART     233
#  define  SYM_TEXT_VARIABLE          234
#  define  SYM_TODAY                  235
#  define  SYM_TOMORROW               236
#  define  SYM_WEEK                   237
#  define  SYM_YEAR                   238
#  define  SYM_ZERO_DATES             239
#endif
/*
*  Preprocessor statements for long options `Lopt_struct' field `larg_mode'.
*/
#define  LARG_NO         0          /* --foo */
#define  LARG_NO_OR_ONE  1          /* --foo or --foo=BAR */
#define  LARG_ONE        2          /* --foo=BAR */
#define  LARG_ONE_OR_ARG 3          /* --foo=BAR or --foo=ARG */
/*
*  Further preprocessor statements used for long-style options.
*/
#define  LARG_MAX        6          /* Maximum number of arguments a long option may have */
#define  LARG_SEP        "="        /* Argument separator of a long option, e.g. --foo=BAR */
#if USE_DE
#  define  LARG_TXT        "ARG"      /* 3 chars text of a symbolic long option argument shown in help pages */
#endif
/*
*  Some other common environment variable names.
*/
#define  ENV_VAR_TZ    "TZ"          /* Name of the timezone environment variable */
#if USE_PAGER
#  define  ENV_VAR_LI    "LINES"       /* Name of number of terminal rows environment variable */
#  define  ENV_VAR_CO    "COLUMNS"     /* Name of number of terminal columns environment variable */
#  define  ENV_VAR_LI2   "LI"          /* Name of number of terminal rows environment variable */
#  define  ENV_VAR_CO2   "CO"          /* Name of number of terminal columns environment variable */
#endif
/*
*  Further preprocessor symbols.
*/
#define  PSEUDO_BLANK  '_'           /* Used to insert a blank (' ') in places the OS normally forbits its use */
#define  REM_CHAR      ';'           /* Comment character of a resource/response file line */
#define  RSP_CHAR      '@'           /* Marker character of a response file (@response file) */
#define  QUOTE_CHAR    '\\'          /* Character used to quote RC_NL_CHAR etc. */
#define  PSEUDO_QUOTE  '\001'        /* Character used to replace a QUOTE_CHAR */
#define  DFORMAT_CHAR  '%'           /* Identifier leading the character used for ordering a displayed date */
#define  SEP           ":"           /* Separator of user defined sequences: -H<SEQ:SEQ> -v<...> -r<...> */
#define  SPLIT_SEP     ","           /* Separator used for splitting lists, e.g. `-P e[DATE][,e[DATE],...] */
#define  CONNECT_SEP   "+"           /* Separator used for connecting lists, e.g. `--cc-holidays=CC[+CC+...]' */
#define  SWITCH        "-"           /* Default switch char, which defines an command line option */
#define  SWITCH2       "/"           /* First additional switch char */
#define  MLIST_SEP     ","           /* Separator of a list of months (mm,mm) */
#define  MRANGE_SEP    "-"           /* Separator of a range of months (mm-mm) */
#define  YLIST_SEP     ";"           /* Separator of a list of years (yyyy;yyyy) */
#define  YRANGE_SEP    "+"           /* Separator of a range of years (yyyy+yyyy) */
#define  YEAR_SEP      "/"           /* Separator of a standard year (mm/yyyy) */
#define  FYEAR_SEP     ":"           /* Separator of a fiscal year (mm:yyyy) */
#define  MONTH3_LIT    "."           /* Command for enabling 3-month mode (.|..|.+|.-) */
#define  DIS_HLS_PREF  "-"           /* Prefix to disable highlighting of a holiday */
#define  DIS_HLS_PREF2 "*"           /* Prefix to disable highlighting of a holiday */
#define  ASC_LIT       "+"           /* Modifier for moving ascending/forwards in date */
#define  DES_LIT       "-"           /* Modifier for moving descending/backwards in date */
#define  FMT_LEN_MIN   3             /* Mininum length of a day "cell" used in calendar sheets */
#define  TXTLEN_DAY    2             /* Minimum length of a textual day name */
#define  TXTLEN_MONTH  3             /* Minimum length of a textual month name */
#define  S_OUT_ROWS    3             /* Number of standard calsheet format default rows standard date */
#define  S_OUT_COLS    4             /* Number of standard calsheet format default columns standard date */
#define  J_OUT_ROWS    4             /* Number of standard calsheet format default rows special date */
#define  J_OUT_COLS    3             /* Number of standard calsheet format default columns special date */
#define  B_OUT_ROWS    12            /* Number of standard calsheet format default rows both dates */
#define  B_OUT_COLS    1             /* Number of standard calsheet format default columns both dates */
#define  SI_OUT_ROWS   4             /* Number of special calsheet format default rows standard date */
#define  SI_OUT_COLS   3             /* Number of special calsheet format default columns standard date */
#define  JI_OUT_ROWS   6             /* Number of special calsheet format default rows special date */
#define  JI_OUT_COLS   2             /* Number of special calsheet format default columns special date */
#define  BI_OUT_ROWS   B_OUT_ROWS    /* Number of special calsheet format default rows both dates */
#define  BI_OUT_COLS   B_OUT_COLS    /* Number of special calsheet format default columns both dates */
#if USE_RC
#  define  A2_OUT_ROWS   1             /* No of default 2-month mode rows both dates */
#  define  A2_OUT_COLS   2             /* No of default 2-month mode columns both dates */
#  define  B2_OUT_ROWS   2             /* No of default 2-month mode rows both dates */
#  define  B2_OUT_COLS   1             /* No of default 2-month mode columns both dates */
#endif
#define  S3_OUT_ROWS   1             /* No of standard calsheet format default 3-month mode rows standard date */
#define  S3_OUT_COLS   3             /* No of standard calsheet format default 3-month mode columns standard date */
#define  J3_OUT_ROWS   S3_OUT_ROWS   /* No of standard calsheet format default 3-month mode rows special date */
#define  J3_OUT_COLS   S3_OUT_COLS   /* No of standard calsheet format default 3-month mode columns special date */
#define  B3_OUT_ROWS   3             /* No of standard calsheet format default 3-month mode rows both dates */
#define  B3_OUT_COLS   1             /* No of standard calsheet format default 3-month mode columns both dates */
#define  SI3_OUT_ROWS  S3_OUT_ROWS   /* No of special calsheet format default 3-month mode rows standard date */
#define  SI3_OUT_COLS  S3_OUT_COLS   /* No of special calsheet format default 3-month mode columns standard date */
#define  JI3_OUT_ROWS  B3_OUT_ROWS   /* No of special calsheet format default 3-month mode rows special date */
#define  JI3_OUT_COLS  B3_OUT_COLS   /* No of special calsheet format default 3-month mode columns special date */
#define  BI3_OUT_ROWS  B3_OUT_ROWS   /* No of special calsheet format default 3-month mode rows both dates */
#define  BI3_OUT_COLS  B3_OUT_COLS   /* No of special calsheet format default 3-month mode columns both dates */
#define  CENTURY       1900          /* Operating system standard starting century, DON'T change ! */
/*
*  German language dependent preprocessor statements.
*/
#if USE_DE
#  define  HD_LIST_TITLE  "Ewige Feiertagsliste"
#  define  RESPONSE_TXT   "Antwortdatei"
#  define  CREATED_TXT    "Erzeugt"
#  define  INTERNAL_TXT   "Intern"
#  define  TIME_SEP       "."             /* Used time separator hh.mm */
#  if USE_RC
#    define  RC_AM_TXT      "am"            /* Trailing text of %time12 special text */
#    define  RC_PM_TXT      "pm"            /* Trailing text of %time12 special text */
#    define  RC_LIST_TITLE  "Terminliste"
#  endif
#  ifdef GCAL_SHELL
#    define  SCRIPT_TXT     "shell script"
#  endif
#endif /* USE_DE */
/*
*  Important preprocessor symbols for the internal ranges.
*/
#define  DAY_LAST    365             /* Last day in a NON leap year */
#define  DAY_MIN     1               /* Minimum day of week/month/year */
#define  DAY_MAX     7               /* Maximum day/amount of days of week */
#define  WEEK_MAX    52              /* Maximum week number of year */
#define  MONTH_LAST  31              /* Highest day number in a month */
#define  MONTH_MIN   1               /* Minimum month of year */
#define  MONTH_MAX   12              /* Maximum month of year */
#define  YEAR_MIN    1               /* Minimum year able to compute */
#define  YEAR_MAX    9999            /* Maximum year able to compute */
#define  EASTER_MIN  30              /* Minimum year for computing Easter Sunday (29+1) */
#define  EASTER_MAX  YEAR_MAX        /* Maximum year for computing Easter Sunday */
#define  MONTH_COLS  6               /* Maximum number of columns of a month */
#define  VEC_BLOCK   42              /* Maximum number of elements per month (7*6) */
#define  VEC_ELEMS   504             /* Maximum number of elements per year (42*12) */
/*
*  Fixed dates/resource file specific preprocessor statements.
*/
#if USE_RC
#  ifndef RC_ELEMS_MAX
#    define  RC_ELEMS_MAX       1024      /* Default number of `rc_elems_table[]' entries */
#  endif
#  ifndef RC_FILES_MAX
#    define  RC_FILES_MAX       8         /* Default number of `rc_files_table[]' entries */
#  endif
#  define  RC_MIN_BCE_TO_1_CE 1721424L  /* Days between 1.5-Jan-4713 BCE and 1.5-Jan-0001 CE */
#  define  RC_DVAR_MAX        26        /* Maximum number of date variables */
#  define  RC_TVAR_MAX        RC_DVAR_MAX  /* Maximum number of text variables */
#  define  RC_DLIST_CHAR      ','       /* Separator of a list of days (www,www... or Nwww,Nwww... or wwwN,wwwN...) */
#  define  RC_DRANGE_CHAR     '#'       /* Separator of a range of days (www#www or Nwww#Nwww or wwwN#wwwN) */
#  define  RC_REPEAT_CHAR     ':'       /* Separator of a "repeat for N days since..." coding (e.g.: yyyymmdd:10) */
#  define  RC_APPEARS_CHAR    '.'       /* Separator of a "appears each N'th day since..." coding (e.g.: yyyymmdd:10.3) */
#  define  RC_ADATE_CHAR      '%'       /* Leading char of a modified actual date %... */
#  define  RC_HDY_CHAR        '@'       /* RC-file holiday/date-variable prefix in date part */
#  define  RC_NWD_CHAR        '*'       /* RC-file n'th weekday prefix in date part */
#  define  RC_NL_CHAR         '~'       /* Inserts a real newline '\n' character if found */
#  define  RC_NL2_CHAR        '^'       /* Inserts a real newline '\n' character if found */
#  define  RC_EASTER_CHAR     'e'       /* Leading char of a @easter date part */
#  define  RC_TODAY_CHAR      't'       /* Leading char of a @today date part */
#  define  RC_TVAR_CHAR       '$'       /* RC-file text variable prefix */
#  define  RC_DVAR_ASSIGN     "="       /* Assignment operator of a date variable (dvar=mmdd) */
#  define  RC_TVAR_ASSIGN     RC_DVAR_ASSIGN   /* Assignment operator of a text variable ($tvar=TEXT) */
#  define  RC_DVAR_ADD        "+"       /* Add operator of a date variable (dvar++ | dvar+=<n> ...) */
#  define  RC_DVAR_SUB        "-"       /* Subtract operator of a date variable (dvar-- | dvar-=<n> ...) */
#  define  RC_GROUP_SEP       ""        /* Default grouping `text' of fixed date list [-c]g */
#  define  RC_INCL_STMENT     "#include"         /* Name of RC-file include statement (upper) */
#  define  RC_INCL_USR_ID     "\""               /* User include file name starts with */
#  define  RC_INCL_USR_OD     RC_INCL_USR_ID     /* User include file name ends with */
#  define  RC_INCL_SYS_ID     "<"                /* System include file name start with */
#  define  RC_INCL_SYS_OD     ">"                /* System include file name ends with */
#  define  ENV_VAR_USR_DATADIR "GCAL_USR_DATADIR"  /* Environment variable name of user datadir */
#  define  ENV_VAR_SYS_DATADIR "GCAL_SYS_DATADIR"  /* Environment variable name of system data dir */
#  define  RC_SUFFIX          "rc"               /* Name part of default resource file */
/*
*  The suffixes used by the moonphase %-[DATE] and %_[DATE] special texts.
*/
#  define  MOON_MIN           "!"
#  define  MOON_MAX           "@"
#  define  MOON_WANES         "-"
#  define  MOON_WAXES         "+"
/*
*  The minimum, maximum and default number of lines of the moonphase text graphics image.
*/
#  define  MOONIMAGE_MIN      6
#  define  MOONIMAGE_MAX      30
#  define  MOONIMAGE_DEF      12
/*
*  The widgets of the moonphase text graphics image.
*/
#  define  MOONIMAGE_BRIGHT   MOON_MAX
#  define  MOONIMAGE_LEDGE    "("
#  define  MOONIMAGE_REDGE    ")"
/*
*  Some mathematical constants used by the moonphase text graphics function
*    and the `my_cos()' cosine emulation function called within `draw_moon()'.
*/
#  define MY_PI               3.14159265358979323846
#  define MY_TWO_PI           6.28318530717958647692
#  if HAVE_LIBM
/*
*  Some astronomical constants used by John Walkers `phase()' function.
*/
#    define  EPOCH                         2444238.5    /* 1980 January 0.0. */
#    define  SUN_ELONG_EPOCH               278.833540   /* Ecliptic longitude of the Sun at epoch 1980.0. */
#    define  SUN_ELONG_PERIGEE             282.596403   /* Ecliptic longitude of the Sun at perigee. */
#    define  ECCENT_EARTH_ORBIT            0.016718     /* Eccentricity of Earth's orbit. */
#    define  MOON_MEAN_LONGITUDE_EPOCH     64.975464    /* Moon's mean lonigitude at the epoch. */
#    define  MOON_MEAN_LONGITUDE_PERIGREE  349.383063   /* Mean longitude of the perigee at the epoch. */
#    define  KEPLER_EPSILON                1E-6         /* Accurancy of the Kepler equation. */
/*
*  Some useful mathematical functions used by John Walkers `phase()' function.
*/
#    define  FIXANGLE(a)  ((a) - 360.0 * (floor((a) / 360.0)))
#    define  TORAD(d)     ((d) * (MY_PI / 180.0))
#    define  TODEG(r)     ((r) * (180.0 / MY_PI))
#  endif /* HAVE_LIBM */
/*
*  The suffixes used by the biorhythm %,[DATE] and %;[DATE] special texts.
*/
#  define  BIO_WANES          MOON_WANES
#  define  BIO_WAXES          MOON_WAXES
#  define  BIO_STABLE         MOON_MAX
#  define  BIO_CRITICAL       MOON_MIN
#  define  BIO_OVERLAPS       BIO_STABLE
/*
*  The minimum, maximum and default single length of the biorhythm text graphics bar.
*/
#  define  BIO_AXIS_MIN       1
#  define  BIO_AXIS_MAX       100
#  define  BIO_AXIS_DEF       20
/*
*  Some scaling symbols/macros needed for creating the biorhythm text graphics bar.
*/
#  define  BIO_AXIS_EXTRA     2
#  define  BIO_AXIS_SCALE(x)  (100/(x))
#  define  BIO_AXIS_ZERO(x)   (BIO_AXIS_EXTRA+(x))
#  define  BIO_AXIS_TOTAL(x)  (((x)*2)+(BIO_AXIS_EXTRA*2)+2)
/*
*  Prefix character of a "special text" given in a line of a Gcal resource file.
*/
#  define  RC_SPECIAL_TEXT_CHAR  '%'
/*
*  %?[TEXT_TILL_EOL]  ---  Runs commands.
*/
/* %shell escape special text */
#  define  RC_SHELL_ESC_CHAR     '!'
/*
*  %?  ---  Excludes special dates.
*/
/* Exclude if date is in eternal holiday list (legal days only) */
#  define  RC_EX_LHDY_CHAR       'V'
/* Exclude if date is not in eternal holiday list (legal days only) */
#  define  RC_EX_NLHDY_CHAR      'v'
/* Exclude if date is in eternal holiday list (all days) */
#  define  RC_EX_AHDY_CHAR       'X'
/* Exclude if date is not in eternal holiday list (all days) */
#  define  RC_EX_NAHDY_CHAR      'x'
/* Exclude if date is a Monday */
#  define  RC_EX_MON_CHAR        'A'
/* Exclude if date is no Monday */
#  define  RC_EX_NMON_CHAR       'a'
/* Exclude if date is a Tuesday */
#  define  RC_EX_TUE_CHAR        'C'
/* Exclude if date is no Tuesday */
#  define  RC_EX_NTUE_CHAR       'c'
/* Exclude if date is a Wednesday */
#  define  RC_EX_WED_CHAR        'F'
/* Exclude if date is no Wednesday */
#  define  RC_EX_NWED_CHAR       'f'
/* Exclude if date is a Thursday */
#  define  RC_EX_THU_CHAR        'G'
/* Exclude if date is no Thursday */
#  define  RC_EX_NTHU_CHAR       'g'
/* Exclude if date is a Friday */
#  define  RC_EX_FRI_CHAR        'H'
/* Exclude if date is no Friday */
#  define  RC_EX_NFRI_CHAR       'h'
/* Exclude if date is a Saturday */
#  define  RC_EX_SAT_CHAR        'L'
/* Exclude if date is no Saturday */
#  define  RC_EX_NSAT_CHAR       'l'
/* Exclude if date is a Sunday */
#  define  RC_EX_SUN_CHAR        'P'
/* Exclude if date is no Sunday */
#  define  RC_EX_NSUN_CHAR       'p'
/* Exclude if date is a Monday...Thursday */
#  define  RC_EX_MON_2_THU_CHAR  'Q'
/* Exclude if date is no Monday...Thursday */
#  define  RC_EX_NMON_2_THU_CHAR 'q'
/* Exclude if date is a Monday...Friday */
#  define  RC_EX_MON_2_FRI_CHAR  'R'
/* Exclude if date is no Monday...Friday */
#  define  RC_EX_NMON_2_FRI_CHAR 'r'
/*
*  %?[STARTING_DATE][#[ENDING_DATE]]  ---  Excludes special dates.
*/
/* %inclusive date special text */
#  define  RC_IDATE_CHAR         'i'
/* %exclusive date special text */
#  define  RC_EDATE_CHAR         'e'
/*
*  %?[DATE]  --- Creates special texts.
*/
/* %complete weekday name special text */
#  define  RC_WDNAME_CHAR        'o'
/* %3-letter weekday name special text */
#  define  RC_WDNAME3_CHAR       'O'
/* %2-letter weekday name special text */
#  define  RC_WDNAME2_CHAR       'K'
/* %weekday number (Mon=1...Sun=7) special text */
#  define  RC_WDNR_M1_2_S7_CHAR  'S'
/* %weekday number (Mon=0...Sun=6) special text */
#  define  RC_WDNR_M0_2_S6_CHAR  'E'
/* %weekday number with trailing Ordinal Number Suffix (Mon=1...Sun=7) special text */
#  define  RC_WDNR_M1_2_S7S_CHAR 'I'
/* %weekday number with trailing Ordinal Number Suffix (Mon=0...Sun=6) special text */
#  define  RC_WDNR_M0_2_S6S_CHAR 'J'
/* %weekday number (Sun=1...Sat=7) special text */
#  define  RC_WDNR_S1_2_S7_CHAR  '('
/* %weekday number (Sun=0...Sat=6) special text */
#  define  RC_WDNR_S0_2_S6_CHAR  ')'
/* %weekday number with trailing Ordinal Number Suffix (Sun=1...Sat=7) special text */
#  define  RC_WDNR_S1_2_S7S_CHAR '<'
/* %weekday number with trailing Ordinal Number Suffix (Sun=0...Sat=6) special text */
#  define  RC_WDNR_S0_2_S6S_CHAR '>'
/* %weekday number (--starting-day=DAY=1...) special text */
#  define  RC_WDNR_X1_2_X7_CHAR  '['
/* %weekday number (--starting-day=DAY=0...) special text */
#  define  RC_WDNR_X0_2_X6_CHAR  ']'
/* %weekday number with trailing Ordinal Number Suffix (--starting-day=DAY=1...) special text */
#  define  RC_WDNR_X1_2_X7S_CHAR '{'
/* %weekday number with trailing Ordinal Number Suffix (--starting-day=DAY=0...) special text */
#  define  RC_WDNR_X0_2_X6S_CHAR '}'
/* %day of year number without leading zero special text */
#  define  RC_DOYNR_NZ_CHAR      '+'
/* %day of year number with leading zero special text */
#  define  RC_DOYNR_LZ_CHAR      '*'
/* %day of year number without leading zero and trailing Ordinal Number Suffix special text */
#  define  RC_DOYNR_NZS_CHAR     '&'
/* %day of year number with leading zero and trailing Ordinal Number Suffix special text */
#  define  RC_DOYNR_LZS_CHAR     '#'
/* %day number without leading zero special text */
#  define  RC_DAYNR_NZ_CHAR      'D'
/* %day number with leading zero special text */
#  define  RC_DAYNR_LZ_CHAR      'N'
/* %day number without leading zero and trailing Ordinal Number Suffix special text */
#  define  RC_DAYNR_NZS_CHAR     's'
/* %day number with leading zero and trailing Ordinal Number Suffix special text */
#  define  RC_DAYNR_LZS_CHAR     'u'
/* %complete month name special text */
#  define  RC_MONTHNAME_CHAR     'M'
/* %3-letter month name special text */
#  define  RC_MONTHNAME3_CHAR    'T'
/* %month number without leading zero special text */
#  define  RC_MONTHNR_NZ_CHAR    'U'
/* %month number with leading zero special text */
#  define  RC_MONTHNR_LZ_CHAR    'W'
/* %month number without leading zero and trailing Ordinal Number Suffix special text */
#  define  RC_MONTHNR_NZS_CHAR   'z'
/* %month number with leading zero and trailing Ordinal Number Suffix special text */
#  define  RC_MONTHNR_LZS_CHAR   'Z'
/* %year number without leading zero special text */
#  define  RC_YEARNR_NZ_CHAR     'Y'
/* %year number with leading zero special text */
#  define  RC_YEARNR_LZ_CHAR     '='
/* %iso week number special text */
#  define  RC_WEEKNR_CHAR        'k'
/* %birth age number special text */
#  define  RC_BYEAR_CHAR         'b'
/* %birth age number with trailing Ordinal Number Suffix special text */
#  define  RC_BYEAR_S_CHAR       'B'
/* %moonphase without leading zero special text */
#  define  RC_MOON_NZ_CHAR       '-'
/* %moonphase with leading zero special text */
#  define  RC_MOON_LZ_CHAR       '_'
/* %moonphase text graphics image */
#  define  RC_MOON_IMAGE_CHAR    ':'
/* %biorhythm special text */
#  define  RC_BIO_CHAR           ','
/* %biorhythm text graphics bar */
#  define  RC_BIO_BAR_CHAR       ';'
/*
*  %?[OTHER_MODIFIER]  ---  Creates special texts.
*/
/* %textual date special text */
#  define  RC_TDATE_CHAR         'n'
/* %julian days since 1.5-Jan-4713 BCE special text */
#  define  RC_JDAYS_CHAR         'j'
/* %time special text */
#  define  RC_TIME_CHAR          't'
/*
*  %?  ---  Creates difference values.
*/
/* %day difference special text */
#  define  RC_DAY_DIFF_CHAR      'd'
/* %week difference special text */
#  define  RC_WEEK_DIFF_CHAR     'w'
/* %month difference special text */
#  define  RC_MONTH_DIFF_CHAR    'm'
/* %year | %year[DATE] difference special text */
#  define  RC_YEAR_DIFF_CHAR     'y'
/*
*  %?  ---  Emits highlighting sequences/marking characters.
*/
/* %highlighting sequence 1 enable special text */
#  define  RC_HLS1S_CHAR         '1'
/* %highlighting sequence 1 disable special text */
#  define  RC_HLS1E_CHAR         '2'
/* %highlighting sequence 2 enable special text */
#  define  RC_HLS2S_CHAR         '3'
/* %highlighting sequence 2 disable special text */
#  define  RC_HLS2E_CHAR         '4'
/* %highlighting sequence 1 enable special text if fixed date is on today's date */
#  define  RC_HLS3S_CHAR         '5'
/* %highlighting sequence 1 disable special text if fixed date is on today's date */
#  define  RC_HLS3E_CHAR         '6'
/* %highlighting sequence 2 enable special text if fixed date is on a legal holiday date */
#  define  RC_HLS4S_CHAR         '7'
/* %highlighting sequence 2 disable special text if fixed date is on a legal holiday date */
#  define  RC_HLS4E_CHAR         '8'
/* %highlighting sequence 1|2 enable special text if fixed date is on today's or legal holiday date */
#  define  RC_HLS5S_CHAR         '9'
/* %highlighting sequence 1|2 disable special text if fixed date is on today's or legal holiday date */
#  define  RC_HLS5E_CHAR         '0'
/*
*  Additional usage texts.
*/
#  define  USAGE_RC1          "CDFIP"
#  define  USAGE_RC2          "cf"
#  define  USAGE_RC3          "r"
#  define  USAGE_RC4          "v"
#else /* !USE_RC */
#  define  USAGE_RC1          ""
#  define  USAGE_RC2          ""
#  define  USAGE_RC3          ""
#  define  USAGE_RC4          ""
#endif /* !USE_RC */
#if USE_PAGER
#  define  PAGER_QUIT         "q"             /* Quits the internal pager */
#  define  USAGE_PAGER        "p"             /* Additional usage text */
#else /* !USE_PAGER */
#  define  USAGE_PAGER        ""
#endif /* !USE_PAGER */
#ifdef GCAL_SHELL
#  define USAGE_SHELL         "S"             /* Additional usage text */
#else /* !GCAL_SHELL */
#  define USAGE_SHELL         ""
#endif /* !GCAL_SHELL */


/*
*  And last but not least...
*/
#define  COPYRIGHT_TXT  "Copyright (C) 1994-1997 Thomas Esken"


/*
*  Common preprocessor macros.
*/
#ifdef S_NEWLINE
#  undef  S_NEWLINE
#endif
#define  S_NEWLINE(gc_fp)    fputc('\n', (gc_fp))
#ifdef MAX
#  undef  MAX
#endif
#define  MAX(gc_x1, gc_x2)   (((gc_x1) > (gc_x2)) ? (gc_x1) : (gc_x2))
#ifdef SYEAR
#  undef  SYEAR
#endif
#define  SYEAR(gc_d, gc_s)   (((gc_d)-(gc_s)+1) < DAY_MIN) \
                               ? ((gc_d)-(gc_s)+(DAY_MAX+1)) \
                               : ((gc_d)-(gc_s)+1)
#ifdef SMONTH
#  undef  SMONTH
#endif
#define  SMONTH(gc_d, gc_s)  (((gc_d)+(gc_s)-1) > MONTH_MAX) \
                               ? ((gc_d)+(gc_s)-(MONTH_MAX+1)) \
                               : ((gc_d)+(gc_s)-1)
#ifdef SDAY
#  undef  SDAY
#endif
#define  SDAY(gc_d, gc_s)    (((gc_d)+(gc_s)-1) > DAY_MAX) \
                               ? ((gc_d)+(gc_s)-(DAY_MAX+1)) \
                               : ((gc_d)+(gc_s)-1)
#if USE_RC
#  ifdef abs
#    undef  abs
#  endif
#  define  abs(gc_x)           (((gc_x) < 0) ? (-(gc_x)) : (gc_x))
#  ifdef SGN
#    undef  SGN
#  endif
#  define  SGN(gc_x)           (((gc_x) < 0) ? -1 : (((gc_x) > 0) ? 1 : 0))
#  ifdef ROUND
#    undef  ROUND
#  endif
#  define  ROUND(gc_x)         ((abs(gc_x)+0.5)*SGN(gc_x))
#  ifdef IDX
#    undef  IDX
#  endif
#  define  IDX(gc_c)           (tolower((gc_c)) - 'a')
#  ifdef CHR2DIG
#    undef  CHR2DIG
#  endif
#  define  CHR2DIG(gc_c)       ((gc_c) - '0')
#  ifdef DIG2CHR
#    undef  DIG2CHR
#  endif
#  define  DIG2CHR(gc_x)       ((gc_x) + '0')
#endif



#ifdef CUT_LONGNAMES
/*
*  Cut resp., replace some long variable or function names to 6 significant
*    places so ANY linker should be able to bind an executable file.
*/
#  if USE_RC
#    define  bio_emo_ptext                       V_bet
#    define  bio_emo_phase                       V_bep
#    define  bio_emo_waxes                       V_bew
#    define  bio_int_ptext                       V_bit
#    define  bio_int_phase                       V_bip
#    define  bio_int_waxes                       V_biw
#    define  bio_phy_ptext                       V_bpt
#    define  bio_phy_phase                       V_bpp
#    define  bio_phy_waxes                       V_bpw
#    define  bio_critical_day                    V_bcd
#    define  bio_positive_day                    V_bpd
#    define  bio_negative_day                    V_bnd
#  endif
#  define  compare_d_m_name                    F_cdm
#  define  comparemode                         V_cm
#  if !USE_RC
#    define  days_of_feb                         V_dof
#  endif
#  define  do_all_saints_day                   F_dsa
#  define  do_all_souls_day                    F_dso
#  define  do_christmas_day                    F_dcd
#  define  do_christmas_eve                    F_dce
#  define  do_christs_ascension_day            F_dca
#  define  do_easter_sunday                    F_des
#  define  do_easter_monday                    F_dem
#  define  do_good_friday                      F_dgf
#  define  do_good_saturday                    F_dgs
#  define  do_johns_day                        F_djd
#  define  do_johns_eve                        F_dje
#  define  do_marys_ascension_day              F_dma
#  define  do_marys_annunciation_day           F_dmn
#  define  do_marys_immaculate_conception      F_dmi
#  define  do_midsummer_day                    F_dmd
#  define  do_midsummer_eve                    F_dme
#  if USE_RC
#    define  exclusive_date_map                  V_edm
#    define  exclusive_weekday_map               V_ewm
#    define  fiscal_month                        V_fm
#    define  fiscal_year                         V_fy
#  endif
#  define  days_of_february                    F_dof
#  if USE_HLS || USE_PAGER
#    define  get_termcap_hls                     F_ghl
#    define  get_termcap_scr_attrib              F_gsc
#  endif
#  define  get_tty_hls                         F_gth
#  if USE_PAGER
#    define  get_tty_scr_size                    F_gts
#  endif
#  define  hd_title_flag                       V_htf
#  define  hd_title_shown                      V_hts
#  define  holiday_flag                        V_hf
#  define  holiday_name                        V_hn
#  define  holiday_prefix                      V_hp
#  define  is_3month_mode                      V_3m
#  define  is_3month_mode2                     V_3m2
#  if USE_RC
#    define  inclusive_date_map                  V_idm
#    define  inclusive_weekday_map               V_iwm
#    define  insert_line_into_table              F_ilt
#    define  insert_response_file                F_irf
#  endif
#  define  is_ext_list                         V_iel
#  define  is_ext_range                        V_ier
#  define  is_ext_year                         V_iey
#  define  is_tty                              V_it
#  define  is_tty1                             V_it1
#  define  is_tty2                             V_it2
#  define  longopt                             V_lo
#  define  longopt_symbolic                    V_los
#  define  module_line                         V_mdl
#  define  module_name                         V_mdn
#  define  month_list                          V_ml
#  define  month_list_max                      V_mlm
#  define  month_name                          F_mn
#  define  month_set                           V_ms
#  if USE_RC && HAVE_LIBM
#    define  moon_corrected_anomaly              V_mca
#    define  moon_corrected_longitude            V_mcl
#    define  moon_correction_equation_of_center  V_mce
#    define  moon_correction_term1               V_mc1
#    define  moon_correction_term2               V_mc2
#    define  moon_mean_anomaly                   V_mma
#    define  moon_mean_longitude                 V_mml
#    define  moon_present_age                    V_mpa
#    define  moon_present_longitude              V_mpl
#    define  moon_present_phase                  V_mpp
#  endif
#  define  my_argc                             V_mac
#  define  my_argc_max                         V_max
#  define  my_argv                             V_mav
#  define  my_help_head_text                   F_mhh
#  define  my_help_tail_text                   F_mht
#  define  my_strstr                           F_mss
#  define  my_strcspn                          F_msp
#  define  my_strcasecmp                       F_msc
#  define  my_strncasecmp                      F_msn
#  if USE_RC
#    define  num_of_range_chars                  V_nrc
#    define  num_of_repeat_chars                 V_noc
#    define  num_of_appears_chars                V_nfc
#    define  precomp_date                        F_pd
#    define  precomp_nth_wd                      F_pwd
#  endif
#  define  print_all_holidays                  F_pah
#  define  print_calendar                      F_pc
#  define  print_highlighted_date              F_phd
#  if USE_HLS
#    define  print_hls                           V_phl
#  endif
#  if USE_RC
#    define  print_line                          V_pl
#  endif
#  define  print_single_holiday                F_psh
#  define  print_text                          F_ptx
#  define  print_unhighlighted_date            F_pud
#  define  ptr_cc                              V_pcc
#  define  ptr_cc_holidays                     V_pcd
#  define  ptr_cc_id                           V_pid
#  define  ptr_info_date_format                V_pdf
#  if USE_RC
#    define  ptr_info_exclusive_day              V_ped
#    define  print_twice                         V_ptw
#    define  rc_bio_axis_len                     V_rba
#    define  rc_bio_emo_lit                      V_rbe
#    define  rc_bio_int_lit                      V_rbi
#    define  rc_bio_phy_lit                      V_rbp
#    define  rc_elems_table                      V_ret
#    define  rc_elems_max                        V_rem
#    define  rc_elems                            V_rei
#    define  rc_enable_fn_flag                   V_rfn
#    define  rc_enable_hda_flag                  V_rha
#    define  rc_enable_hdl_flag                  V_rhl
#    define  rc_export_ldvar_flag                V_red
#    define  rc_export_ltvar_flag                V_ret
#    define  rc_feb_29_to_feb_28                 V_f2f
#    define  rc_feb_29_to_mar_01                 V_f2m
#    define  rc_files_table                      V_rft
#    define  rc_files_max                        V_rfm
#    define  rc_files                            V_rfi
#    define  rc_filename                         V_rfn
#    define  rc_filter_day                       V_rfd
#    define  rc_filter_period                    V_rfp
#    define  rc_filter_text                      V_rfx
#    define  rc_period                           V_rp
#    define  rc_period_flag                      V_rpf
#    define  rc_period_list                      V_rpl
#    define  rc_use                              F_ru
#    define  rc_use_flag                         V_ruf
#    define  rc_valid_day                        F_rvd
#    define  rc_valid_period                     F_rvp
#    define  rc_week_flag                        V_rwf
#    define  rc_weekno_flag                      V_rwn
#    define  rc_week_year_flag                   V_rwy
#  endif
#  define  short_day_name                      F_sdn
#  define  short_month_name                    F_smn
#  define  special_flag                        V_sf
#  define  special_vector                      V_sv
#  define  users_date_format                   V_ud
#  define  users_greg                          V_ug
#  define  use_day_suffix                      V_uds
#  define  use_day_zeroleaded                  V_udz
#  if USE_RC
#    define  weekday_of_date                     F_wod
#    define  weekday_map                         V_wm
#    define  weekday_name_group                  V_wng
#  endif
#endif /* CUT_LONGNAMES */



/*
*  Type definitions.
*/
typedef
  unsigned char
  Uchar;
typedef
  unsigned int
  Uint;
#ifdef DJG
typedef
#  if __STDC__
  unsigned short int
#  else /* !__STDC__ */
  unsigned short
#  endif /* !__STDC__ */
  Usint;
#endif /* DJG */
typedef
#if __STDC__
  signed long int
#else /* !__STDC__ */
  long
#endif /* !__STDC__ */
  Slint;
typedef
#if __STDC__
  unsigned long int
#else /* !__STDC__ */
  unsigned long
#endif /* !__STDC__ */
  Ulint;
typedef
  int
  Bool;
/*
*  The quicksort (`qsort()') compare function type.
*/
typedef
  int
  (*Cmp_func)__P_((const VOID_PTR, const VOID_PTR));
#if HAVE_SIGNAL && (defined(SIGINT) || defined(SIGTERM) || defined(SIGHUP))
/*
*  The signal (`signal()') handler function return type.
*/
typedef
  RETSIGTYPE
  (*Sig_type)__P_((int));
#endif
/*
*  The country code specific holiday handling function type.
*/
typedef
  void
  (*Cc_hdy_handler)__P_((Bool *, const Bool, const int, const int, int  *, const int, const int));
/*
*  The long option record, see file `gcal.c' for a detailed description of it!
*/
typedef
  struct long_option_type
   {
     int    symbolic_name;
     char  *long_name;
     char  *short_name[LARG_MAX];
     int    larg_mode;
     char  *largs[LARG_MAX];
   }
  Lopt_struct;
/*
*  The highlighting sequence record.
*/
typedef
  struct   hls_type
   {
     const char  *seq;
           int    len;
   }
  Hls_struct;
/*
*  The list/range of months/years record.
*/
typedef
  struct  ml_type
   {
     int  month;
     int  year;
   }
  Ml_struct;
/*
*  The country code record.
*/
typedef
  struct   country_code_type
   {
     char  *id;                  /* The ISO-3166 2-characters country code identifer */
     char  *info;                /* Description text of the country code; the country namef */
     void (*Cc_hdy_handler)();   /* The country code specific holiday handling function */
   }
  Cc_struct;
/*
*  The supported date format record.
*/
typedef
  struct   date_format_type
   {
     char  *id;        /* Identifying text of the format */
     char  *info;      /* Description text of the format */
     char  *format;    /* The format text itself */
   }
  Df_struct;
/*
*  The Gregorian Reformation date record.
*/
typedef
  struct  greg_type
   {
     int  year;        /* Year of Gregorian Reformation */
     int  month;       /* Month of Gregorian Reformation */
     int  first_day;   /* First missing day of Reformation period */
     int  last_day;    /* Last missing day of Reformation period */
   }
  Greg_struct;
#if USE_RC
/*
*  The exclusion date record.
*/
typedef
  struct   exclusion_date_type
   {
     char   id;        /* The character used in the %? inclusive/exclusive day special text */
     char  *info;      /* Description text of the %? inclusive/exclusive day special text */
   }
  Ed_struct;
/*
*  The date variable records.
*/
typedef
  struct   dvar_base_type
   {
     char  month;
     char  day;
   }
  Dvar_base_struct;
typedef
  struct dvar_type
   {
     Dvar_base_struct  g;   /* Global date variables */
     Dvar_base_struct  l;   /* Local date variables */
   }
  Dvar_struct;
/*
*  The text variable records.
*/
typedef
  struct   tvar_base_type
   {
     char  *text;
   }
  Tvar_base_struct;
typedef
  struct   tvar_type
   {
     Tvar_base_struct  g;   /* Global text variables */
     Tvar_base_struct  l;   /* Local text variables */
   }
  Tvar_struct;
/*
*  The line part record.
*/
typedef
  struct   line_type
   {
     Bool   day_list;       /* List of days given? */
     Bool   day_range;      /* Range of days given? */
     char  *day_part;       /* Points to the list/range of days of a line, otherwise NULL */
     char  *repeat_part;    /* Points to the "repeat for N days since..." field of a line, otherwise NULL */
     char  *appears_part;   /* Points to the "appears each N'th day since..." field of a line, otherwise NULL */
     char  *text_part;      /* Points to the text part of a line (always) */
   }
  Line_struct;
/*
*  The file type record.
*/
typedef
  struct file_type
   {
     FILE              *fp;                         /* Actual file pointer */
     char              *filename;                   /* Name of actual resource/include file */
     long               line_number;                /* Actual line number of actual resource/include file */
     int                in_pool;                    /* Number of bytes still unhandled in self-managed buffer */
     char              *pool;                       /* Memory region of self-managed buffer */
     char              *ptr_pool;                   /* Pointer to next unmanaged character in self-managed buffer */
     Dvar_base_struct   local_dvars[RC_DVAR_MAX];   /* Buffered local date variables of a "previous" file */
     Tvar_base_struct   local_tvars[RC_TVAR_MAX];   /* Buffered local text variables of a "previous" file */
   }
  File_struct;
#endif /* USE_RC */
/*
*  General purpose enumerations.
*/
typedef
  enum compare_mode
   {
       DAy=1,
#if USE_RC
       DVar,
       EAster,
       TOday,
       WEek,
#endif
       MOnth
   }
  Cmode_enum;
typedef
  enum file_mode
   {
       REsponse=1,
#if USE_RC
       USr_include,
       SYs_include,
       REsource,
       HEre,
#endif
#ifdef GCAL_SHELL
       SCript,
#endif
       COmmon
   }
  Fmode_enum;
#if USE_RC
typedef
  enum dvar_mode
   {
       GLobal=1,
       LOcal
   }
  Dvar_enum;
typedef
  Dvar_enum
  Tvar_enum;
#endif
#endif /* __GCAL_H */
