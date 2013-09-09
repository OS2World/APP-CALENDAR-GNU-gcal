/*
*  utils.c:  Pool of common functions.
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



#ifdef RCSID
static char rcsid[]="$Id: utils.c 2.40 1997/04/22 02:04:00 tom Exp $";
#endif



/*
*  Include header files.
*/
#include "tailor.h"
#if HAVE_CTYPE_H
#  include <ctype.h>
#endif
#if HAVE_ERRNO_H
#  include <errno.h>
#endif
#if HAVE_UNISTD_H
#  include <unistd.h>
#endif
#if TIME_WITH_SYS_TIME
#  include <sys/time.h>
#  include <time.h>
#else /* !TIME_WITH_SYS_TIME */
#  if HAVE_SYS_TIME_H
#    include <sys/time.h>
#  else /* !HAVE_SYS_TIME_H */
#    include <time.h>
#  endif /* !HAVE_SYS_TIME_H */
#endif /* !TIME_WITH_SYS_TIME */
#include "gcal.h"



/*
*  Function prototypes.
*/
#if __cplusplus
extern "C"
{
#endif
/*
************************************************** Defined in `holiday.c'.
*/
IMPORT int
eval_holiday __P_((      int  day,
                   const int  month,
                   const int  year,
                   const int  wd,
                   const Bool forwards));
/*
************************************************** Defined in `help.c'.
*/
IMPORT char *
usage_msg __P_((void));
IMPORT void
put_longopt_description __P_((FILE *fp));
#if USE_RC
/*
************************************************** Defined in `rc-utils.c'.
*/
IMPORT Line_struct *
rc_get_date __P_((      char        *the_line,
                        Line_struct *lineptrs,
                  const Bool         is_rc_file,
                        Bool        *is_weekday_mode,
                        int         *d,
                        int         *m,
                        int         *y,
                        int         *n,
                        int         *len,
                        char        *hc,
                        int         *hn,
                        int         *hwd,
                  const char        *filename,
                  const long         line_number,
                  const char        *line_buffer,
                  const Bool         on_error_exit));
IMPORT Bool
precomp_nth_wd __P_((      int         diff,
                     const int         wd,
                           int        *n,
                           int        *day,
                           int        *month,
                           int        *year,
                     const Cmode_enum  mode));
IMPORT Bool
precomp_date __P_((      int         diff,
                   const int         wd,
                         int        *day,
                         int        *month,
                   const int         year,
                   const Cmode_enum  mode));
IMPORT Bool
prev_date __P_((int *day,
                int *month,
                int *year));
IMPORT Bool
next_date __P_((int *day,
                int *month,
                int *year));
#endif /* USE_RC */
/*
************************************************** Defined in `utils.c'.
*/
EXPORT VOID_PTR
my_malloc __P_((const int   amount,
                const int   exit_status,
                const char *module_name,
                const long  module_line,
                const char *var_name,
                const int   var_contents));
EXPORT VOID_PTR
my_realloc __P_((      VOID_PTR  ptr_memblock,
                 const int       amount,
                 const int       exit_status,
                 const char     *module_name,
                 const long      module_line,
                 const char     *var_name,
                 const int       var_contents));
EXPORT void
allocate_all_strings __P_((const int   amount,
                           const char *module_name,
                           const long  module_line));
EXPORT void
resize_all_strings __P_((const int   amount,
                         const Bool  with_line_buffer,
                         const char *module_name,
                         const long  module_line));
EXPORT void
my_error __P_((const int   exit_status,
               const char *module_name,
               const long  module_line,
               const char *var_name,
               const int   var_contents));
#if HAVE_SIGNAL && (defined(SIGINT) || defined(SIGTERM) || defined(SIGHUP))
EXPORT RETSIGTYPE
handle_signal __P_((int the_signal));
#endif
EXPORT void
my_exit __P_((const int exit_status));
EXPORT int
my_atoi __P_((const char *string));
#if !HAVE_STRSTR
EXPORT char *
my_strstr __P_((const char *text,
                const char *pattern));
#endif /* !HAVE_STRSTR */
#if !HAVE_STRCSPN
EXPORT char *
my_strcspn __P_((const char *s1,
                 const char *s2));
#endif /* !HAVE_STRCSPN */
#if !HAVE_STRCASECMP
EXPORT int
my_strcasecmp __P_((const char *s1,
                    const char *s2));
#endif /* !HAVE_STRCASECMP */
#if !HAVE_STRNCASECMP
EXPORT int
my_strncasecmp __P_((const char *s1,
                     const char *s2,
                           int   len));
#endif /* !HAVE_STRNCASECMP */
EXPORT Bool
get_actual_date __P_((void));
EXPORT int
compare_d_m_name __P_((const char       *string,
                       const Cmode_enum  mode));
EXPORT int
asc_sort __P_((const char **a,
               const char **b));
EXPORT int
des_sort __P_((const char **a,
               const char **b));
EXPORT Bool
is_presorted __P_((char **table,
                   int    elems));
EXPORT void
resort __P_((      char **table,
             const int    elems));
EXPORT const char *
day_suffix __P_((int day));
EXPORT const char *
short3_day_name __P_((const int day));
EXPORT const char *
short_day_name __P_((const int day));
EXPORT const char *
day_name __P_((const int day));
EXPORT const char *
short_month_name __P_((const int month));
EXPORT const char *
month_name __P_((const int month));
EXPORT Ulint
date2num __P_((const int day,
               const int month,
               const int year));
EXPORT Bool
doy2date __P_((      int  doy,
               const int  is_leap_year,
                     int *day,
                     int *month));
EXPORT int
weekday_of_date __P_((const int day,
                      const int month,
                      const int year));
EXPORT int
day_of_year __P_((const int day,
                  const int month,
                  const int year));
EXPORT int
days_of_february __P_((const int year));
EXPORT Bool
valid_date __P_((const int day,
                 const int month,
                 const int year));
EXPORT int
week_number __P_((const int day,
                  const int month,
                  const int year));
EXPORT int
weekno2doy __P_((      int week,
                 const int year));
EXPORT int
knuth_easter_formula __P_((const int year));
EXPORT int
julian_gregorian_diff __P_((const int day,
                            const int month,
                            const int year));
LOCAL int
raw_week_number __P_((const int day,
                      const int month,
                      const int year));
LOCAL const char *
dflt_day_name __P_((const int day));
LOCAL const char *
dflt_month_name __P_((const int month));
#if __cplusplus
}
#endif



/*
*  Declare public(extern) variables.
*/
IMPORT const int    dvec[];         /* Amount of days in months */
IMPORT const int    mvec[];         /* Number of past days of month */
IMPORT Greg_struct *greg;           /* Points to the used Gregorian Reformation date */
#if USE_RC
IMPORT Line_struct *lineptrs;       /* Pointers to different parts of a (resource file) line */
IMPORT FILE        *rc_here_fp;     /* Temporary file used for managing `--here=ARG' options */
#endif
#ifdef GCAL_EMAIL
IMPORT FILE        *tfp;            /* Temporary file which is send by the mailer */
#endif
#ifdef DJG
IMPORT Usint        testval;        /* Set to SHRT_MAX for checking the maximum table range */
#else
IMPORT Uint         testval;        /* Set to INT_MAX for checking the maximum table range */
#endif
IMPORT Uint         maxlen_max;     /* Actual size of all string vectors */
IMPORT int          len_year_max;   /* String length of the maximum year able to compute */
IMPORT int          act_sec;        /* Actual second */
IMPORT int          act_min;        /* Actual minute */
IMPORT int          act_hour;       /* Actual hour */
IMPORT int          act_day;        /* Actual day */
IMPORT int          act_month;      /* Actual month */
IMPORT int          act_year;       /* Actual year */
IMPORT int          buf_ad;         /* Buffer of actual day */
IMPORT int          buf_am;         /* Buffer of actual month */
IMPORT int          buf_ay;         /* Buffer of actual year */
IMPORT char        *prgr_name;      /* Stores the actual program name */
IMPORT char        *s1;             /* General purpose text buffer */
IMPORT char        *s2;             /* General purpose text buffer */
IMPORT char        *s3;             /* General purpose text buffer */
IMPORT char        *s4;             /* General purpose text buffer */
#if USE_RC
IMPORT char        *s5;             /* General purpose text buffer */
IMPORT char        *s6;             /* General purpose text buffer */
IMPORT char        *s7;             /* General purpose text buffer */
IMPORT char        *line_buffer;    /* Text buffer of a line read from a resource file */
IMPORT char        *rc_adate;       /* Text containing modified actual date %... */
IMPORT char        *rc_here_fn;     /* Name of tempfile used for managing `--here=ARG' options */
#endif
#ifdef GCAL_EMAIL
IMPORT char        *tfn;            /* Name of tempfile used by the mailer */
#endif
IMPORT Bool         orthodox_calendar;   /* -O (compute leap years as done by Eastern churches) */
#ifdef GCAL_NLS
IMPORT Bool         is_en;          /* Support of English language? */
#endif



   PUBLIC VOID_PTR
my_malloc (amount, exit_status, module_name, module_line, var_name, var_contents)
   const int   amount;
   const int   exit_status;
   const char *module_name;
   const long  module_line;
   const char *var_name;
   const int   var_contents;
/*
   Allocates AMOUNT bytes of memory dynamically, with error checking.  Calls `my_error()'
     and terminates program if any errors occur.  AMOUNT is limited to `int' range
     instead of `size_t' range; this is wanted!
*/
{
   auto VOID_PTR  ptr_memblock;


   if ((Uint)amount > testval)
     /*
        Error, table size overflow!
     */
     my_error (107, module_name, module_line, var_name, (int)testval);
   ptr_memblock = (VOID_PTR)malloc((int)amount);
   if (ptr_memblock == (VOID_PTR)NULL)
     /*
        Error, `malloc()' have failed.
     */
     my_error (exit_status, module_name, module_line, var_name, var_contents);

   return(ptr_memblock);
}



   PUBLIC VOID_PTR
my_realloc (ptr_memblock, amount, exit_status, module_name, module_line, var_name, var_contents)
         VOID_PTR  ptr_memblock;
   const int       amount;
   const int       exit_status;
   const char     *module_name;
   const long      module_line;
   const char     *var_name;
   const int       var_contents;
/*
   Changes the size of an allocated block of memory PTR_MEMBLOCK to AMOUNT bytes,
     with error checking.  Calls `my_error()' and terminates program if any errors
     occur.  AMOUNT is limited to `int' range instead of `size_t' range; this is
     wanted!  If PTR_MEMBLOCK is NULL, `my_malloc()' is called instead.
*/
{
   if ((Uint)amount > testval)
     /*
        Error, table size overflow!
     */
     my_error (107, module_name, module_line, var_name, (int)testval);
   if (ptr_memblock == (VOID_PTR)NULL)
     return(my_malloc (amount, exit_status, module_name, module_line, var_name, var_contents));
   ptr_memblock = (VOID_PTR)realloc(ptr_memblock, (int)amount);
   if (ptr_memblock == (VOID_PTR)NULL)
     /*
        Error, `realloc()' have failed.
     */
     my_error (exit_status, module_name, module_line, var_name, var_contents);

   return(ptr_memblock);
}



   PUBLIC void
allocate_all_strings (amount, module_name, module_line)
   const int   amount;
   const char *module_name;
   const long  module_line;
/*
   Initially allocates AMOUNT bytes of memory dynamically for all string vectors used,
     with error checking.  Calls `my_error()' indirectly and terminates program if any
     errors occur.  AMOUNT is limited to `int' range instead of `size_t' range; this
     is wanted!  Can only be called once!
*/
{
   static Bool  all_strings_initialized=FALSE;


   if (!all_strings_initialized)
    {
      s1 = (char *)my_malloc (amount, 124, module_name, module_line, "s1", 0);
      s2 = (char *)my_malloc (amount, 124, module_name, module_line, "s2", 0);
      s3 = (char *)my_malloc (amount, 124, module_name, module_line, "s3", 0);
      s4 = (char *)my_malloc (amount, 124, module_name, module_line, "s4", 0);
#if USE_RC
      s5 = (char *)my_malloc (amount, 124, module_name, module_line, "s5", 0);
      s6 = (char *)my_malloc (amount, 124, module_name, module_line, "s6", 0);
      s7 = (char *)my_malloc (amount, 124, module_name, module_line, "s7", 0);
      line_buffer = (char *)my_malloc (amount, 124, module_name, module_line, "line_buffer", 0);
#endif
      all_strings_initialized = TRUE;
    }
}



   PUBLIC void
resize_all_strings (amount, with_line_buffer, module_name, module_line)
   const int   amount;
   const Bool  with_line_buffer;
   const char *module_name;
   const long  module_line;
/*
   Changes the size of all string vectors used to AMOUNT bytes, with error checking.
     Calls `my_error()' indirectly and terminates program if any errors occur.
     AMOUNT is limited to `int' range instead of `size_t' range; this is wanted!
*/
{
   if (   ((Uint)amount > testval)
       && (maxlen_max < testval))
     maxlen_max = testval;
   else
     maxlen_max = (Uint)amount;
   s1 = (char *)my_realloc ((VOID_PTR)s1, maxlen_max, 124, module_name, module_line, "s1", maxlen_max);
   s2 = (char *)my_realloc ((VOID_PTR)s2, maxlen_max, 124, module_name, module_line, "s2", maxlen_max);
   s3 = (char *)my_realloc ((VOID_PTR)s3, maxlen_max, 124, module_name, module_line, "s3", maxlen_max);
   s4 = (char *)my_realloc ((VOID_PTR)s4, maxlen_max, 124, module_name, module_line, "s4", maxlen_max);
#if USE_RC
   s5 = (char *)my_realloc ((VOID_PTR)s5, maxlen_max, 124, module_name, module_line, "s5", maxlen_max);
   s6 = (char *)my_realloc ((VOID_PTR)s6, maxlen_max, 124, module_name, module_line, "s6", maxlen_max);
   s7 = (char *)my_realloc ((VOID_PTR)s7, maxlen_max, 124, module_name, module_line, "s7", maxlen_max);
   if (with_line_buffer)
     line_buffer = (char *)my_realloc ((VOID_PTR)line_buffer, maxlen_max,
                                       124, module_name, module_line,
                                       "line_buffer", maxlen_max);
#endif
}



   PUBLIC void
my_error (exit_status, module_name, module_line, var_name, var_contents)
   const int   exit_status;
   const char *module_name;
   const long  module_line;
   const char *var_name;
   const int   var_contents;
/*
   Displays a specific error message on STDERR channel
     and terminates the program with status `exit_status'.
*/
{
#if USE_DE
   fprintf(stderr, "\n%s: Abbruch, ", prgr_name);
#else /* !USE_DE */
   fprintf(stderr, _("\n%s: abort, "), prgr_name);
#endif /* !USE_DE */
   switch(exit_status)
    {
#if USE_DE
      case 125:
        fprintf(stderr, "unzul%sssiges Jahr f%sr Ostertagsberechnung\nJahr mu%s im Bereich (%d...%d) sein",
                AE, UE, SZ, EASTER_MIN, EASTER_MAX);
        break;
      case 124:
        fprintf(stderr, "`%s' Zeile %ld: virtueller Speicher ersch%spft (%s=%d)",
                module_name, module_line, OE, var_name, var_contents);
        break;
#  if USE_RC
      case 123:
        fprintf(stderr, "unzul%sssiger Datumeintrag in Datei `%s'\nZeile %ld: %s",
                AE, module_name, module_line, var_name);
        break;
      case 122:
        fprintf(stderr, "ung%sltiger Monat(%02d) in Datei `%s'\nZeile %ld: %s",
                UE, var_contents, module_name, module_line, var_name);
        break;
      case 121:
        fprintf(stderr, "ung%sltiger Tag(%02d) in Datei `%s'\nZeile %ld: %s",
                UE, var_contents, module_name, module_line, var_name);
        break;
      case 120:
        fprintf(stderr, "mi%sgebildetes %s in Datei `%s'\nZeile %ld: %s",
                SZ, RC_INCL_STMENT, module_name, module_line, var_name);
        break;
      case 119:
        fprintf(stderr, "rekursives/zyklisches %s unzul%sssig in Datei `%s'\nZeile %ld: %s",
                RC_INCL_STMENT, AE, module_name, module_line, var_name);
        break;
      case 118:
        fprintf(stderr, "Datei `%s' nicht gefunden", module_name);
        break;
      case 117:
        fprintf(stderr, "ung%sltiges N'ter Wochentagfeld(%d) in Datei `%s'\nZeile %ld: %s",
                UE, var_contents, module_name, module_line, var_name);
        break;
      case 116:
        fprintf(stderr, "fehlendes `whitespace' Trennzeichen nach Datumteil in Datei `%s'\nZeile %ld: %s",
                module_name, module_line, var_name);
        break;
      case 114:
        if (*module_name)
          fprintf(stderr, "illegale Datumvariablen-Operation in Datei `%s'\nZeile %ld: %s",
                  module_name, module_line, var_name);
        else
         {
           fprintf(stderr, "illegale Datumvariablen-Operation (%s)\n%s\n",
                   var_name, usage_msg ());
           put_longopt_description (stderr);
         }
        break;
#  endif /* USE_RC */
      case 115:
        fprintf(stderr, "kann Datei `%s' nicht schreiben\nSpeichermedium ist voll!",
                var_name);
        break;
#  ifdef GCAL_EMAIL
      case 111:
        fprintf(stderr, "versenden der eMail an `%s' hat versagt", var_name);
#    if HAVE_ERRNO_H
        fprintf(stderr, "\n`%s' Zeile %ld: ", module_name, module_line);
        perror("perror");
#    endif
        break;
#  endif
#  if defined(GCAL_EPAGER) || USE_RC
      case 110:
        fprintf(stderr, "`%s' Zeile %ld: (`%s') `%s%d' hat versagt",
                module_name, module_line, INTERNAL_TXT, var_name, var_contents);
#    if HAVE_ERRNO_H
        perror("\nperror");
#    endif
        break;
#  endif
      case 109:
        fprintf(stderr, "Lesefehler in Datei `%s'", var_name);
#  if HAVE_ERRNO_H
        fprintf(stderr, "\n`%s' Zeile %ld: ", module_name, module_line);
        perror("perror");
#  endif
        break;
      case 108:
        fprintf(stderr, "illegales Zeichen in Antwortdatei `%s'\nZeile %ld: %s",
                module_name, module_line, var_name);
        break;
      case 107:
        fprintf(stderr, "`%s' Zeile %ld: (`%s') ung%sltiger Wert f%sr Tabellengr%s%se `sizeof %s>%d'",
                module_name, module_line, INTERNAL_TXT, UE, UE, OE, SZ, var_name, var_contents);
        break;
      case 106:
        fprintf(stderr, "(`%s') Datumformat `%s' ist ung%sltig", module_name, var_name, UE);
        break;
#  if USE_RC
      case 105:
        if (*module_name)
          fprintf(stderr, "illegale Textvariablen-Definition in Datei `%s'\nZeile %ld: %s",
                  module_name, module_line, var_name);
        else
         {
           fprintf(stderr, "illegale Textvariablen-Definition (%s)\n%s\n",
                   var_name, usage_msg ());
           put_longopt_description (stderr);
         }
        break;
      case 104:
       if (*module_name)
         fprintf(stderr, "%s im Suchmuster `%s'", module_name, var_name);
       else
         fprintf(stderr, "ung%sltiges Suchmuster `%s' angegeben", UE, var_name);
       break;
#  endif
      default:
        fprintf(stderr, "`%s' Zeile %ld: (`%s') unbehandelter Fehler (%d)",
                module_name, module_line, INTERNAL_TXT, exit_status);
#else /* !USE_DE */
      case 125:
        fprintf(stderr, _("invalid year for computing Easter Sundays date\nYear must be in range (%d...%d)"),
                EASTER_MIN, EASTER_MAX);
        break;
      case 124:
        fprintf(stderr, _("`%s' line %ld: virtual memory exhausted (%s=%d)"),
                module_name, module_line, var_name, var_contents);
        break;
#  if USE_RC
      case 123:
        fprintf(stderr, _("invalid date part in file `%s'\nLine %ld: %s"),
                module_name, module_line, var_name);
        break;
      case 122:
        fprintf(stderr, _("invalid month field(%02d) in file `%s'\nLine %ld: %s"),
                var_contents, module_name, module_line, var_name);
        break;
      case 121:
        fprintf(stderr, _("invalid day field(%02d) in file `%s'\nLine %ld: %s"),
                var_contents, module_name, module_line, var_name);
        break;
      case 120:
        fprintf(stderr, _("malformed %s in file `%s'\nLine %ld: %s"),
                RC_INCL_STMENT, module_name, module_line, var_name);
        break;
      case 119:
        fprintf(stderr, _("invalid recursive/cyclic %s in file `%s'\nLine %ld: %s"),
                RC_INCL_STMENT, module_name, module_line, var_name);
        break;
      case 118:
        fprintf(stderr, _("file `%s' not found"), module_name);
        break;
      case 117:
        fprintf(stderr, _("invalid N'th weekday field(%d) in file `%s'\nLine %ld: %s"),
                var_contents, module_name, module_line, var_name);
        break;
      case 116:
        fprintf(stderr, _("missing `whitespace' character after date part in file `%s'\nLine %ld: %s"),
                module_name, module_line, var_name);
        break;
      case 114:
        if (*module_name)
          fprintf(stderr, _("illegal date variable operation in file `%s'\nLine %ld: %s"),
                  module_name, module_line, var_name);
        else
         {
           fprintf(stderr, _("illegal date variable operation (%s)\n%s\n"),
                   var_name, usage_msg ());
           put_longopt_description (stderr);
         }
        break;
#  endif /* USE_RC */
      case 115:
        fprintf(stderr, _("file `%s' can't be written\nStorage media full!"),
                var_name);
        break;
#  ifdef GCAL_EMAIL
      case 111:
        fprintf(stderr, _("sending eMail to `%s' have failed"), var_name);
#    if HAVE_ERRNO_H
        fprintf(stderr, _("\n`%s' line %ld: "), module_name, module_line);
        perror("perror");
#    endif
        break;
#  endif
#  if defined(GCAL_EPAGER) || USE_RC
      case 110:
        fprintf(stderr, _("`%s' line %ld: (`%s') `%s%d' have failed"),
                module_name, module_line, _("Internal"), var_name, var_contents);
#    if HAVE_ERRNO_H
        perror("\nperror");
#    endif
        break;
#  endif
      case 109:
        fprintf(stderr, _("read error in file `%s'"), var_name);
#  if HAVE_ERRNO_H
        fprintf(stderr, _("\n`%s' line %ld: "), module_name, module_line);
        perror("perror");
#  endif
        break;
      case 108:
        fprintf(stderr, _("illegal character in response file `%s'\nLine %ld: %s"),
                module_name, module_line, var_name);
        break;
      case 107:
        fprintf(stderr, _("`%s' line %ld: (`%s') invalid value for table size `sizeof %s>%d'"),
                module_name, module_line, _("Internal"), var_name, var_contents);
        break;
      case 106:
        fprintf(stderr, _("(`%s') date format `%s' is invalid"), module_name, var_name);
        break;
#  if USE_RC
      case 105:
        if (*module_name)
          fprintf(stderr, _("illegal text variable definition in file `%s'\nLine %ld: %s"),
                  module_name, module_line, var_name);
        else
         {
           fprintf(stderr, _("illegal text variable definition (%s)\n%s\n"),
                   var_name, usage_msg ());
           put_longopt_description (stderr);
         }
        break;
      case 104:
       if (*module_name)
         fprintf(stderr, _("%s in search pattern `%s'"), module_name, var_name);
       else
         fprintf(stderr, _("invalid search pattern `%s' specified"), var_name);
       break;
#  endif /* USE_RC */
      default:
        fprintf(stderr, _("`%s' line %ld: (`%s') unmanaged error (%d)"),
                module_name, module_line, _("Internal"), exit_status);
#endif /* !USE_DE */
    }
   S_NEWLINE(stderr);
   my_exit (exit_status);
}



#if HAVE_SIGNAL && (defined(SIGINT) || defined(SIGTERM) || defined(SIGHUP))
   PUBLIC RETSIGTYPE
handle_signal (the_signal)
   int the_signal;
/*
   Signal handler function which displays the numeric ID of the received signal
     on stderr channel and terminates the program with an exit status 3.
*/
{
   fflush(stdout);
#if USE_DE
   fprintf(stderr, "\n%s: Programmabbruch durch Signal %d\n", prgr_name, the_signal);
#else /* !USE_DE */
   fprintf(stderr, _("\n%s: program aborted by signal %d\n"), prgr_name, the_signal);
#endif /* */
   my_exit (3);
}
#endif /* HAVE_SIGNAL && (SIGINT || SIGTERM || SIGHUP) */



   PUBLIC void
my_exit (exit_status)
   const int exit_status;
/*
   Tries to erase all temporary files before without error checking
     and quits the program by calling the systems `exit()' function.
*/
{
#if USE_RC
   if (rc_here_fp != (FILE *)NULL)
     (void)fclose(rc_here_fp);
   if (rc_here_fn != (char *)NULL)
     (void)unlink(rc_here_fn);
#endif
#ifdef GCAL_EMAIL
   if (tfp != (FILE *)NULL)
     (void)fclose(tfp);
   if (tfn != (char *)NULL)
     (void)unlink(tfn);
#endif
   exit(exit_status);
}



   PUBLIC int
my_atoi (string)
   const char *string;
/*
   Converts given `string' to a positiv integer value
     skipping leading zeroes and returns values in
     range 0...YEAR_MAX only (invalid values are set to 0).
*/
{
   while (*string == '0')
     string++;
   if (   *string == '-'
       || (int)strlen(string) > len_year_max)
     return(0);

   return(atoi(string));
}



#if !HAVE_STRSTR
   PUBLIC char *
my_strstr (text, pattern)
   const char *text;
   const char *pattern;
/*
   Search the `pattern' needle in haystack `text'   =8^)
     (emulates the ANSI C strstr() function; not very well optimized).
*/
{
   register       int    i=(int)strlen(text);
   register       int    j=(int)strlen(pattern);
   register       int    k;
   auto     const char  *ptr_char=text;


   if (!j)
     return(ptr_char);
   if (i < j)
     return(NULL);
   for (i=0 ; *ptr_char ; i++,ptr_char++)
    {
      for (k=0,j=i ; pat[k] && (text[j]==pattern[k]) ; j++,k++)
        ;
      if (   k
          && !pattern[k])
        return(ptr_char);
    }

   return(NULL);
}
#endif /* !HAVE_STRSTR */



#if !HAVE_STRCSPN
   PUBLIC int
my_strcspn (s1, s2)
   const char *s1;
   const char *s2;
/*
   Find length of initial segment of `s1' consisting entirely
   of characters not from `s2' (emulates the ANSI C strcspn() function).
*/
{
   register       int    count=0;
   auto     const char  *scan1;
   auto     const char  *scan2;


   for (scan1=s1 ; *scan1 ; scan1++)
    {
      for (scan2=s2 ; *scan2 ; )
       if (*scan1 == *scan2++)
         return(count);
      count++;
    }

   return(count);
}
#endif /* !HAVE_STRCSPN */



#if !HAVE_STRCASECMP
   PUBLIC int
my_strcasecmp (s1, s2)
   const char *s1;
   const char *s2;
/*
   Same as the ANSI C `strcmp()' function, but case insensitive.
*/
{
   auto const Uchar  *p1=(const Uchar *)s1;
   auto const Uchar  *p2=(const Uchar *)s2;
   auto       Uchar   c1;
   auto       Uchar   c2;


   if (p1 == p2)
     return(0);
   do
    {
      c1 = tolower(*p1++);
      c2 = tolower(*p2++);
      if (!c1)
        break;
    } while (c1 == c2);

   return(c1-c2);
}
#endif /* !HAVE_STRCASECMP */



#if !HAVE_STRNCASECMP
   PUBLIC int
my_strncasecmp (s1, s2, len)
   const char *s1;
   const char *s2;
         int   len;
/*
   Same as the ANSI C `strncmp()' function, but case insensitive.
*/
{
   auto const Uchar  *p1=(const Uchar *)s1;
   auto const Uchar  *p2=(const Uchar *)s2;
   auto       Uchar   c1;
   auto       Uchar   c2;


   if (   !len
       || p1 == p2)
     return(0);
   if (len < 0)
     len = -len;
   do
    {
      c1 = tolower(*p1++);
      c2 = tolower(*p2++);
      if (   !c1
          || c1 != c2)
        return(c1-c2);
    } while (--len);

   return(c1-c2);
}
#endif /* !HAVE_STRNCASECMP */



   PUBLIC Bool
get_actual_date ()
/*
   Gets the actual date from the system resp.,
     evaluates the "actual" date from global `rc_adate'-ptr to string.
     Returns TRUE if `rc_adate' could be evaluated, otherwise FALSE.
*/
{
   auto   struct tm         *sys_date;
   auto          MY_TIME_T   sys_time;
   static Bool               got_time=FALSE;


   sys_time  = time((MY_TIME_T *)NULL);
   sys_date  = localtime(&sys_time);
   if (!got_time)
    {
      got_time = TRUE;
      act_sec  = sys_date->tm_sec;
      act_min  = sys_date->tm_min;
      act_hour = sys_date->tm_hour;
    }
   buf_ad=act_day   = sys_date->tm_mday;
   buf_am=act_month = sys_date->tm_mon + 1;
   act_year  = sys_date->tm_year;
   if (act_year < CENTURY)
     act_year += CENTURY;
   buf_ay = act_year;
#if USE_RC
   if (rc_adate != (char *)NULL)
    {
      auto int    d=0;
      auto int    m=0;
      auto int    y=0;
      auto int    n=0;
      auto int    hn=0;
      auto int    hwd=0;
      auto int    i_dummy;
      auto char   hc='\0';
      auto char  *c_dummy=(char *)NULL;
      auto Bool   b_dummy;


      if (   !*rc_adate
          || *rc_adate == RC_HDY_CHAR)
        return(FALSE);
      /*
         Pre-initialize month and year to actual date.
      */
      m = act_month;
      y = act_year;
      /*
         Check if a numeric date is given.
      */
      if (isdigit(*rc_adate))
       {
         /*
            Check if a numeric day in month is given.
         */
         if ((int)strlen(rc_adate) <= 2)
          {
            d = atoi(rc_adate);
            if (!d)
              /*
                 Error, invalid day number given.
              */
              return(FALSE);
          }
         else
#  if USE_DE
           (void)rc_get_date (rc_adate, lineptrs, FALSE, &b_dummy, &d, &m, &y, &n,
                              &i_dummy, &hc, &hn, &hwd, INTERNAL_TXT, -1L, rc_adate, FALSE);
#  else /* !USE_DE */
           (void)rc_get_date (rc_adate, lineptrs, FALSE, &b_dummy, &d, &m, &y, &n,
                              &i_dummy, &hc, &hn, &hwd, _("Internal"), -1L, rc_adate, FALSE);
#  endif /* !USE_DE */
       }
      else
       {
         /*
            Check if textual weekday name is given.
         */
         d = compare_d_m_name (rc_adate, DAy);
         if (d)
          {
            /*
               If a digit (1...5, 9) trails the textual weekday name,
                 generate "n'th weekday of month" date.
            */
            c_dummy = rc_adate;
            while (isalpha(*c_dummy))
              c_dummy++;
            if (*c_dummy)
             {
               n = atoi(c_dummy);
               if (!n)
                 /*
                    Error, invalid "n'th weekday of month" given (must be 1...5, 9).
                 */
                 return(FALSE);
               /*
                  Check if the digit is trailed by anything further...
               */
               c_dummy++;
               if (*c_dummy)
                 /*
                    Error, invalid "n'th weekday of month" given.
                 */
                 return(FALSE);
             }
            else
             {
               /*
                  Only textual weekday name found, so compute absolute day
                    in month of that weekday (of current week).
               */
               hn = act_day;
               i_dummy = weekday_of_date (act_day, act_month, act_year);
               if (d > i_dummy)
                 do
                  {
                    next_date (&hn, &m, &y);
                  } while (d != weekday_of_date (hn, m, y));
               else
                 if (d < i_dummy)
                   do
                    {
                      prev_date (&hn, &m, &y);
                    } while (d != weekday_of_date (hn, m, y));
               d = hn;
             }
          }
         else
          {
            /*
               Check if short month name is given.
            */
            m = compare_d_m_name (rc_adate, MOnth);
            if (!m)
              /*
                 Error, invalid month name given.
              */
              return(FALSE);
            /*
               Check if a day number trails textual a month name.
            */
            c_dummy = rc_adate;
            while (   isupper(*c_dummy)
#  if USE_DE
#    if USE_EASC
                   || *c_dummy == *AE
                   || *c_dummy == *OE
                   || *c_dummy == *UE
                   || *c_dummy == *AAE
                   || *c_dummy == *OOE
                   || *c_dummy == *UUE
#    else /* !USE_EASC */
                   || *c_dummy == '"'
#    endif /* !USE_EASC */
#  endif /* USE_DE */
                   || islower(*c_dummy))
              c_dummy++;
            if (*c_dummy)
             {
               d = atoi(c_dummy);
               if (!d)
                 /*
                    Error, invalid trailing day number given.
                 */
                 return(FALSE);
             }
            else
              d = act_day;
          }
       }
      if (y >= 0)
       {
         if (!y)
           y = act_year;
         if (!m)
           m = act_month;
         if (n)
          {
            if (d > DAY_MAX)
              return(FALSE);
            if (!d)
              d = weekday_of_date (act_day, m, y);
            if (m == 2)
              i_dummy = days_of_february (y);
            else
              i_dummy = dvec[m-1];
            if (n == 9)
              d = eval_holiday (i_dummy, m, y, d, FALSE);
            else
              if (   (n >= 1)
                  && (n <= 5))
               {
                 d = eval_holiday (DAY_MIN, m, y, d, TRUE);
                 d += (DAY_MAX * (n - 1));
                 /*
                    The "n'th weekday of month" doesn't occur in month:
                      generate no "actual" date.
                 */
                 if (d > i_dummy)
                   return(FALSE);
               }
              else
                return(FALSE);
          }
         else
           if (hc)
            {
              /*
                 If no explicit year is given in actual date modifier
                   %0@<e|t|`dvar'>[[-]<n>[`ww[w]']] resp., %0*d|w<n>[`ww[w]'],
                   compute the date respecting the displacement, which was
                   returned by function `rc_get_date()' in variable `hn' an `hwd'.
              */
              switch (hc)
               {
                 case RC_EASTER_CHAR:
                 case RC_TODAY_CHAR:
                   if (!precomp_date (hn, hwd, &d, &m, y,
                       (hc == RC_EASTER_CHAR) ? EAster : TOday))
                     return(FALSE);
                   break;
                 case 'D':
                 case 'W':
                   if (!precomp_nth_wd (hn, hwd, &n, &d, &m, &y,
                                        ((hc == 'D') ? DAy : WEek)))
                     return(FALSE);
                   break;
                 default:
                   if (!islower(hc))
                     return(FALSE);
                   else
                     if (!precomp_date (hn, hwd, &d, &m, y, DVar))
                       return(FALSE);
               }
            }
           else
             if (!d)
               d = act_day;
             else
               if (d == 99)
                {
                  /*
                     Assume last day of month.
                  */
                  if (m == 2)
                    d = days_of_february (y);
                  else
                    d = dvec[m-1];
                }
         if (valid_date (d, m, y))
          {
            act_day = d;
            act_month = m;
            act_year = y;
          }
         else
           return(FALSE);
       }
      else
        return(FALSE);
    }
#endif /* USE_RC */

  return(TRUE);
}



   PUBLIC int
compare_d_m_name (string, mode)
   const char       *string;
   const Cmode_enum  mode;
/*
   Compares the given day/month name `string' with built-in names and
     returns (1...7|1...12) either if `string' matches partitially (until `\0'
     or first digit found in `string') or `string' matches complete.
     If `string'doesn't match or the length of `string' is less 2 for a day
     name or less 3 for a month name (we need such lengths to distinguish in a
     save way!), the function returns 0 always.
     This function checks first if a native language day/month name is given,
     if such a name can't be found, it checks next whether an "default" English
     day/month name is given.
   Depending on mode:
     mode==DAy:
       Compares delivered day name `string' with the built-in day names
         and returns: 1...7 == dayname 1...7 found ; 0 otherwise
     mode==MOnth:
       Compares delivered month name `string' with the built-in month names
         and returns: 1...12 == monthname 1...12 found ; 0 otherwise
*/
{
   register       int    len=(int)strlen(string);
   register const int    len_min=(mode==DAy) ? TXTLEN_DAY : TXTLEN_MONTH;
   register       int    i;
   register       int    imax=(mode==DAy) ? DAY_MAX : MONTH_MAX;
   register       int    j=0;
   register       int    checks;
   auto     const char  *ptr_char=string;


#if USE_DE
   checks = 2;
#else /* !USE_DE */
#  ifdef GCAL_NLS
   if (is_en)
     checks = 1;
   else
     checks = 2;
#  else /* !GCAL_NLS */
   checks = 1;
#  endif /* !GCAL_NLS */
#endif /* !USE_DE */
   if (len >= TXTLEN_DAY)
    {
      while (   *(ptr_char + j)
#if USE_RC
             && !isdigit(*(ptr_char + j))
             && (*(ptr_char + j) != RC_REPEAT_CHAR)
             && (*(ptr_char + j) != RC_APPEARS_CHAR)
#endif
            )
        j++;
      if (j >= len_min)
        do
         {
           for (i=(mode==DAy) ? DAY_MIN : MONTH_MIN ; i <= imax ; i++)
            {
              if (checks == 2)
                ptr_char = (mode==DAy) ? day_name (i) : month_name (i);
              else
                ptr_char = (mode==DAy) ? dflt_day_name (i) : dflt_month_name (i);
              j = 0;
              while (   *(ptr_char + j)
                     && string[j])
#if USE_DE && USE_EASC
                if (   (   (   string[j] == *AE
                            || string[j] == *AAE)
                        && (   *(ptr_char + j) == *AE
                            || *(ptr_char + j) == *AAE))
                    || (   (   string[j] == *OE
                            || string[j] == *OOE)
                        && (   *(ptr_char + j) == *OE
                            || *(ptr_char + j) == *OOE))
                    || (   (   string[j] == *UE
                            || string[j] == *UUE)
                        && (   *(ptr_char + j) == *UE
                            || *(ptr_char + j) == *UUE)))
                  j++;
                else
#endif
                  if (tolower(*(ptr_char+j)) == tolower(string[j]))
                    j++;
                  else
                    break;
              if (   j == len
#if USE_RC
                  || (   j
                      && (   isdigit(string[j])
                          || string[j] == RC_REPEAT_CHAR
                          || string[j] == RC_APPEARS_CHAR))
#endif
                  || !*(ptr_char + j))
                return(i);
            }
         } while (--checks);
    }

   return(0);
}



   PUBLIC int
asc_sort (a, b)
   const char **a;
   const char **b;
/*
   The (q)sort compare function; ascending order.
*/
{
   return(strcmp(*a, *b));
}



   PUBLIC int
des_sort (a, b)
   const char **a;
   const char **b;
/*
   The (q)sort compare function; descending order.
*/
{
   return(strcmp(*b, *a));
}



   PUBLIC Bool
is_presorted (table, elems)
   char **table;
   int    elems;
/*
   Checks whether the textual entries in `table[]' are presorted in
      ascending sort order.  Returns TRUE if the entries in `table'
      are presorted, otherwise FALSE.
*/
{
   if (--elems > 0)
    {
      register int  i=0;
      register int  j=0;


      while (elems--)
       {
         j++;
         if (*table[i] >= *table[j])
           if (strcmp(table[i], table[j]) > 0)
             return(FALSE);
         i++;
       }
    }

  return(TRUE);
}



   PUBLIC void
resort (table, elems)
         char **table;
   const int    elems;
/*
   Rearranges (reverts) the sort order of the textual entries in `table[]' from
     ascending sort order to descending sort order by swapping the `table' pointers.
*/
{
   register int  right=elems-1;


   if (right > 0)
    {
      register int    left=0;
      auto     char  *ptr_elem;


      while (left < right)
       {
         ptr_elem = table[left];
         table[left++] = table[right];
         table[right--] = ptr_elem;
       }
    }
}



   PUBLIC const char *
day_suffix (day)
   int day;
/*
   Returns the ordinal suffix (st, nd, rd or th) which is added to a single day number.
*/
{
#if USE_DE
   static   const char  *suffix[]={"'ter", "'ter", "'ter", "'ter"};
#else /* !USE_DE */
   static   const char  *suffix[]={N_("th"), N_("st"), N_("nd"), N_("rd")};
#endif /* !USE_DE */
   register       int    i=0;


/*
   Necessary for numbers > [9]9999.

   if (day > 10000)
     day %= 10000;
   if (day > 1000)
     day %= 1000;
*/
   if (day > 100)
     day %= 100;
   if (   day < 11
       || day > 13)
     i = day % 10;
   if (i > 3)
     i = 0;

   return(_(suffix[i]));
}



   PUBLIC const char *
short3_day_name (day)
   const int day;
/*
   Returns the short name of the day using the printf()/scanf() format "%-3s".
*/
{
#if USE_DE
   static const char  *name[]={
#  if USE_EASC
                                "ung"UE"ltiger Tag",
#  else /* !USE_EASC */
                                "ungueltiger Tag",
#  endif /* !USE_EASC */
                                "Mon", "Die", "Mit", "Don",
                                "Fre", "Sam", "Son"
                              };
#else /* !USE_DE */
   static const char  *name[]={
                                N_("invalid day"),
                                N_("Mon"), N_("Tue"), N_("Wed"), N_("Thu"),
                                N_("Fri"), N_("Sat"), N_("Sun")
                              };
#endif /* !USE_DE */

   return(((day<DAY_MIN)||(day>DAY_MAX)) ? _(name[0]) : _(name[day]));
}



   PUBLIC const char *
short_day_name (day)
   const int day;
/*
   Returns the short name of the day using the printf()/scanf() format "%-2s".
*/
{
#if USE_DE
   static const char  *name[]={
#  if USE_EASC
                                "ung"UE"ltiger Tag",
#  else /* !USE_EASC */
                                "ungueltiger Tag",
#  endif /* !USE_EASC */
                                "Mo", "Di", "Mi", "Do", "Fr", "Sa", "So"
                              };
#else /* !USE_DE */
   static const char  *name[]={
                                N_("invalid day"),
                                N_("Mo"), N_("Tu"), N_("We"), N_("Th"),
                                N_("Fr"), N_("Sa"), N_("Su")
                              };
#endif /* !USE_DE */

   return(((day<DAY_MIN)||(day>DAY_MAX)) ? _(name[0]) : _(name[day]));
}



   PUBLIC const char *
day_name (day)
   const int day;
/*
   Returns the complete name of the day.
*/
{
#if USE_DE
   static const char  *name[]={
#  if USE_EASC
                                "ung"UE"ltiger Tag",
#  else /* !USE_EASC */
                                "ungueltiger Tag",
#  endif /* !USE_EASC */
                                "Montag",  "Dienstag", "Mittwoch", "Donnerstag",
                                "Freitag", "Samstag",  "Sonntag"
                              };
#else /* !USE_DE */
   static const char  *name[]={
                                N_("invalid day"),
                                N_("Monday"), N_("Tuesday"),  N_("Wednesday"), N_("Thursday"),
                                N_("Friday"), N_("Saturday"), N_("Sunday")
                              };
#endif /* !USE_DE */

   return(((day<DAY_MIN)||(day>DAY_MAX)) ? _(name[0]) : _(name[day]));
}



   PUBLIC const char *
short_month_name (month)
   const int month;
/*
   Returns the short name of the month using the printf()/scanf() format "%-3s".
*/
{
#if USE_DE
   static const char  *name[]={
#  if USE_EASC
                                "ung"UE"ltiger Monat",
#  else /* !USE_EASC */
                                "ungueltiger Monat",
#  endif /* !USE_EASC */
                                "Jan", "Feb",
#  if USE_EASC
                                "M"AE"r",
#  else /* !USE_EASC */
                                "Mae",
#  endif /* !USE_EASC */
                                "Apr", "Mai", "Jun", "Jul", "Aug",
                                "Sep", "Okt", "Nov", "Dez"
                              };
#else /* !USE_DE */
   static const char  *name[]={
                                N_("invalid month"),
                                N_("Jan"), N_("Feb"), N_("Mar"), N_("Apr"), N_("May"), N_("Jun"),
                                N_("Jul"), N_("Aug"), N_("Sep"), N_("Oct"), N_("Nov"), N_("Dec")
                              };
#endif /* !USE_DE */

   return(((month<MONTH_MIN)||(month>MONTH_MAX)) ? _(name[0]) : _(name[month]));
}



   PUBLIC const char *
month_name (month)
   const int month;
/*
   Returns the complete name of the month.
*/
{
#if USE_DE
   static const char  *name[]={
#  if USE_EASC
                                "ung"UE"ltiger Monat",
#  else /* !USE_EASC */
                                "ungueltiger Monat",
#  endif /* !USE_EASC */
                                "Januar",  "Februar",
#  if USE_EASC
                                "M"AE"rz",
#  else /* !USE_EASC */
                                "Maerz",
#  endif /* !USE_EASC */
                                "April",   "Mai",      "Juni",
                                "Juli",    "August",   "September",
                                "Oktober", "November", "Dezember"
                              };
#else /* !USE_DE */
   static const char  *name[]={
                                N_("invalid month"),
                                N_("January"), N_("February"), N_("March"),
                                N_("April"),   N_("May"),      N_("June"),
                                N_("July"),    N_("August"),   N_("September"),
                                N_("October"), N_("November"), N_("December")
                              };
#endif /* !USE_DE */

   return(((month<MONTH_MIN)||(month>MONTH_MAX)) ? _(name[0]) : _(name[month]));
}



   PUBLIC Ulint
date2num (day, month, year)
   const int day;
   const int month;
   const int year;
/*
   Computes the absolute number of days of the given date since
     00010101(==YYYYMMDD) respecting the missing period of the
     Gregorian Reformation.
*/
{
   auto Ulint  julian_days=(Ulint)((year-1)*(Ulint)(DAY_LAST)+((year-1)>>2));


   if (   year > greg->year
       || (   (year == greg->year)
           && (   month > greg->month
               || (   (month == greg->month)
                   && (day > greg->last_day)))))
     julian_days -= (Ulint)(greg->last_day - greg->first_day + 1);
   if (year > greg->year)
    {
      julian_days += (((year - 1) / 400) - (greg->year / 400));
      julian_days -= (((year - 1) / 100) - (greg->year / 100));
      if (   !(greg->year % 100)
          && (greg->year % 400))
        julian_days--;
    }
   julian_days += (Ulint)mvec[month-1];
   julian_days += day;
   if (   (days_of_february (year) == 29)
       && (month > 2))
     julian_days++;

   return(julian_days);
}



   PUBLIC Bool
doy2date (doy, is_leap_year, day, month)
         int  doy;
   const int  is_leap_year;
         int *day;
         int *month;
/*
   Converts a given number of days of a year to a standard date
     (returned in &day and &month) and returns:
       TRUE in case the `day_of_year' number is valid;
       FALSE otherwise.
*/
{
   register int   i;
   auto     Bool  decrement_date;


   if (   doy > DAY_LAST+is_leap_year
       || doy < DAY_MIN)
     return(FALSE);
   decrement_date = (Bool)(   is_leap_year
                           && (doy > 59));
   if (decrement_date)
     doy--;
   for (i=MONTH_MIN ; i < MONTH_MAX ; i++)
    {
      doy -= dvec[i-1];
      if (doy <= 0)
       {
         doy += dvec[i-1];
         break;
       }
    }
   *month = i;
   *day = doy;
   if (   decrement_date
       && (*month == 2)
       && (*day == 28))
     (*day)++;

   return(TRUE);
   /*
      Alternative floating-point algorithm for managing this stuff:
      ...
      if (doy-is_leap_year > 59)
       {
         month = (int)((63 + doy - is_leap_year) / 30.61) - 1;
         day = (int)(63 + doy - is_leap_year - abs((int)((month + 1) * 30.61)));
       }
      else
       {
         month = (int)((428 + doy) / 30.61) - 13;
         day = (int)(428 + doy - abs((int)((month + 13) * 30.61)));
       }
      ...
   */
}



   PUBLIC int
weekday_of_date (day, month, year)
   const int day;
   const int month;
   const int year;
/*
   Computes the weekday of a Gregorian/Julian calendar date
     (month must be 1...12) and returns 1...7 (1==mo, 2==tu...7==su).
*/
{
   auto Ulint  julian_days=date2num (day, month, year)%DAY_MAX;

   return((julian_days>2) ? (int)julian_days-2 : (int)julian_days+5);
}



   PUBLIC int
day_of_year (day, month, year)
   const int day;
   const int month;
   const int year;
/*
   Computes the day of the year of a Gregorian or Julian calendar date
     (month must be 1...12) and returns 1...365|366.
*/
{
   register int  i;


   if (month < 3)
     i = mvec[month-1] + day;
   else
     i = mvec[month-1] + day + (days_of_february (year) == 29);

   return(i);
}



   PUBLIC int
days_of_february (year)
   const int year;
/*
   Computes the number of days in February --- respecting the Gregorian
     Reformation period likewise the leap year rule as used by the
     Eastern orthodox churches --- and returns them.
*/
{
   register int  day;


   if (   (year > greg->year)
       || (   (year == greg->year)
           && (   greg->month == 1
               || (   (greg->month == 2)
                   && (greg->last_day >= 28)))))
    {
      if (orthodox_calendar)
        day = (year&3) ? 28 : ((!(year%100)) ? (((year%9)==2||(year%9)==6) ? 29 : 28) : 29);
      else
        day = (year&3) ? 28 : ((!(year%100)&&(year%400)) ? 28 : 29);
    }
   else
     day = (year&3) ? 28 : 29;
   /*
      Exception, the year 4 AD was historically NO leap year!
   */
   if (year == 4)
     day--;

   return(day);
}



   PUBLIC Bool
valid_date (day, month, year)
   const int day;
   const int month;
   const int year;
/*
   Checks whether a delivered date is valid.
*/
{
   if (   day < 0
       || month < MONTH_MIN
       || month > MONTH_MAX
       || (   (month != 2)
           && (day > dvec[month-1]))
       || (   (month == 2)
           && (day > days_of_february (year))))
     return(FALSE);

   return(TRUE);
}



   PUBLIC int
week_number (day, month, year)
   const int day;
   const int month;
   const int year;
/*
   Returns either a ISO-8601:1988 standard week number of the given date
   or a special value for marking a special event, which can be managed
   by the caller in a special way if needed!  Return values are:
     -(WEEK_MAX+1) : Date appears both in 53rd (=last) week of previous year
                       and first days of year, BUT these first days of year
                       are NOT IN the first week of year.
     -WEEK_MAX     : Date appears both in 52nd (=last) week of previous year
                       and first days of year, BUT these first days of year
                       are NOT IN the first week of year.
     -(WEEK_MAX-1) : Date appears both in 51rd (=last) week of previous year
                       and first days of year, BUT these first days of year
                       are NOT IN the first week of year.
                       This special event only happens in the December of
                       the year of Gregorian Reformation or in the January of
                       the year after the year of Gregorian Reformation!
      0            : Date appears both in 53rd (=last) week of previous year
                       and first days of year, AND these first days of year
                       are IN the first week of year.
      1...WEEK_MAX : Date appears in the year.
   Note that an ISO week starts with a monday(=1) and ends with a sunday(=7)!
*/
{
   register int  i=raw_week_number (day, month, year);
   register int  j;
   register int  wmax=WEEK_MAX;


   if (   (i > 1)
       && (year == greg->year))
     wmax = ((DAY_LAST + (days_of_february (greg->year) == 29)
            - (greg->last_day - greg->first_day + 1)) / DAY_MAX) + 1;
   if (i >= wmax)
    {
      if (month == MONTH_MIN)
       {
         j = raw_week_number (dvec[MONTH_MAX-1], MONTH_MAX, year-1);
         if (i == j)
           i = -i;
         else
           i = 0;
       }
      else
       {
         j = raw_week_number (DAY_MIN, MONTH_MIN, year+1);
         if (i == j)
           i = -i;
         else
           if (   (j == 1)
               && (i == wmax+1))
             i = 0;
       }
    }
   else
     if (i == 1)
      {
        if (weekday_of_date (DAY_MIN, MONTH_MIN, year) <= 4)
         {
           j = raw_week_number (dvec[MONTH_MAX-1], MONTH_MAX, year-1);
           if (year-1 == greg->year)
             wmax = ((DAY_LAST + (days_of_february (greg->year) == 29)
                    - (greg->last_day - greg->first_day + 1)) / DAY_MAX) + 1;
           if (j != wmax)
             i = 0;
         }
      }
     else
       if (   (i == wmax-1)
           && (month == MONTH_MIN)
           && (year == greg->year+1))
        i = -i;

   return(i);
}



   PUBLIC int
weekno2doy (week, year)
         int week;
   const int year;
/*
   Returns the "day_of_year" number of a Julian or Gregorian calendar year,
   the given ISO-8601:1988 week number starts at.
     Week number may be:
       (a)  0           == Returns day_of_year number of first week of year.
                             resp., that dates, which occur both in last week
                             of previous year and first week of year
                             (in this case, the function return value is
                              -5...0 indicating how many days+1 are in
                              the previous year).
       (b)  1...52      == Returns day_of_year number always.
       (c) 53           == Returns day_of_year_number or if year has NO 53rd week,
                             returns -WEEK_MAX [=special value]).
       (d) 99           == Returns day_of_year number of last week of year.
     Return values are:
           -WEEK_MAX    == Event (c) has occurred and year has NO 53rd week.
           -5...0       == In case event (a) has occurred and the first days
                             of year occur both in last week of previous year
                             and first week of year.
            1...365|366 == Events (b), (c) and (d).
*/
{
   register int  wd=weekday_of_date (DAY_MIN, MONTH_MIN, year);
   register int  ww=week_number (DAY_MIN, MONTH_MIN, year);
   register int  wm=week_number (dvec[MONTH_MAX-1], MONTH_MAX, year);
   register int  wmax=WEEK_MAX;
   register int  i=1;


   if (wm < 0)
     wm = -wm;
   if (   ww == -(wmax - 1)
       || year == greg->year)
     wmax = ((DAY_LAST + (days_of_february (greg->year) == 29)
            - (greg->last_day - greg->first_day + 1)) / DAY_MAX) + 1;
   if (ww < 0)
     ww = -ww;
   if (ww > 1)
     i += (DAY_MAX - wd + 1);
   else
     if (!ww)
       i -= (wd - 1);
   if (   wm
       && (wm != wmax+1)
       && (week == wmax+1))
     return(-WEEK_MAX);
   else
     if (week == 99)
      {
        week = wm;
        if (!wm)
          week = wmax + 1;
        i += ((week - 1) * DAY_MAX);
      }
     else
      {
        i += ((week - 1) * DAY_MAX);
        if (   !week
            && (i <= -(DAY_MAX-1)))
          i += DAY_MAX;
      }
   /*
      Correction in case week occurs during Gregorian Reformation period.
   */
   if (year == greg->year)
     if (i+(greg->last_day-greg->first_day+1) > day_of_year (greg->last_day, greg->month, greg->year))
       i += (greg->last_day - greg->first_day + 1);
   if (i > DAY_LAST+(days_of_february (year)==29))
     return(-WEEK_MAX);

   return(i);
}



   PUBLIC int
knuth_easter_formula (year)
   const int year;
/*
   This procedure calculates the day and month of Easter giving the year.
   It returns the "actual day_of_year date" of Western Eastern (not the
   Eastern Easter of the Eastern orthodox churches) after A.D.463.
   ***
   *** ANNOTATION by Thomas Esken <esken@uni-muenster.de>, 12-DEC-1996
   ***
   *** But in fact, this procedure works correctly for
   *** calculating the Easter Sunday's date after A.D.29.
   *** (Ref.: Hans Lietzmann, Zeitrechnung, De gryter 1984, 4te Auflage)
   ***
   `golden_number' is the number of the year in the Metonic cycle, used
   to determine the position of the calendar moon.
   `gregorian_correction' is the number of preceding years like 1700, 1800,
   1900 when leap year was not held.
   `clavian_correction' is a correction for the Metonic cycle of about
   8 days every 2500 years.
   `epact' is the age of the calendar moon at the beginning of the year.
   `extra_days' specifies when Sunday occurs in March.  `epact' specifies
   when full moon occurs.  Easter is the first Sunday following the first
   full moon which occus on or after March 21.  (Ref.: De Morgan, A Budget
   of Paradoxes)
   This basic Algorithm was given by Don Knuth in CACM 5(1962), 209-210,
   and is altered accordingly for managing different Gregorian Reformation
   periods.
*/
{
   auto     long  extra_days;
   register int   golden_number=(year%19)+1;
   register int   century;
   register int   gregorian_correction;
   register int   clavian_correction;
   register int   epact;
   register int   day;
   register int   month;
   register int   easter;
   auto     Bool  was_julian=FALSE;


   if (   (year > greg->year)
       || (   (year == greg->year)
           && (   greg->month < 3
               || (   (greg->month == 3)
                   && (greg->last_day < 22)))))
    {
      /*
         Computation for Gregorian years.
      */
LABEL_compute_gregorian:
      century = year / 100 + 1;
      gregorian_correction = (3 * century / 4) - 12;
      clavian_correction = ((8 * century + 5) / 25) - 5;
      extra_days = (5L * year / 4) - gregorian_correction - 10;
      epact = (11 * golden_number + 20 + clavian_correction - gregorian_correction) % 30;
      /*
         BTW, the earliest year the calculated epact is negative or null
           is the year 1710!
         (see chapter 1.3.2 "THE MIX ASSEMBLY LANGUAGE", page 155,
         question 15 of Don Knuth's "The Art of Computer Programming", Vol. 1)
      */
      if (epact <= 0)
        epact += 30;
      if (   (   (epact == 25)
              && (golden_number > 11))
          || epact == 24)
        epact++;
    }
   else
    {
      /*
         Computation for Julian calendar years.
      */
      was_julian = TRUE;
      extra_days = (5L * year) / 4;
      epact = ((11 * golden_number - 4) % 30) + 1;
    }
   day = 44 - epact;
   if (day < 21)
     day += 30;
   day += 7 - (int)((extra_days + day) % 7);
   if (day > 31)
    {
      day -= 31;
      month = 4;
    }
   else
     month = 3;
   easter = day_of_year (day, month, year);
   /*
      Correction for potential Reformation period which occurred
        during the period Easter Sunday may take place (22-MAR...25-APR).
        I assume if such a Reformation periods happens, the proper
        solution for computing Easter Sunday's date has to be
        done for the Gregorian year instead of the Julian year.
   */
   if (   was_julian
       && (year == greg->year)
       && (   greg->month == 3
           || (   (greg->month == 4)
               && (greg->last_day <= 25))))
    {
      was_julian = FALSE;
      if (   (easter >= day_of_year (greg->first_day, greg->month, greg->year))
          && (easter <= day_of_year (greg->last_day, greg->month, greg->year)))
        goto LABEL_compute_gregorian;
    }

   return(easter);
}



   PUBLIC int
julian_gregorian_diff (day, month, year)
   const int day;
   const int month;
   const int year;
/*
   Returns the day difference between given Gregorian calendar date
     and the according Julian calendar date, i.e. the amount of days,
     the Julian calendar is past the Gregorian calendar.
*/
{
   auto Ulint  julian_days=(Ulint)((year-1)*(Ulint)(DAY_LAST)+((year-1)>>2));


   julian_days += (Ulint)(mvec[month-1] + day);
   if (   !(year & 3)
       && (month > 2))
     julian_days++;

   return((int)(julian_days-date2num (day, month, year)));
}



   LOCAL int
raw_week_number (day, month, year)
   const int day;
   const int month;
   const int year;
/*
   Returns the raw ISO-8601:1988 standard week number of delivered date ONLY
     (week starts with monday(1) and ends with sunday(7)).
*/
{
   register int  jd=day_of_year (day, month, year);
   register int  sd=weekday_of_date (DAY_MIN, MONTH_MIN, year);
   register int  ww;


   if (   (year == greg->year)
       && (   month > greg->month
           || (   (month == greg->month)
               && (day > greg->last_day))))
     jd -= (greg->last_day - greg->first_day + 1);
   /*
      Correct the computed day_of_year number of delivered date.
   */
   if (sd > 4)
     /*
        Starting day of year is Friday...Sunday.
     */
     jd -= ((DAY_MAX - sd) + 1);
   else
     /*
        Starting day of year is Monday...Thursday.
     */
     jd += (sd - 1);
   /*
      Compute week number of delivered date.
   */
   if (jd > 0)
     /*
        Date has consecutive week number of current year: Compute it.
     */
     ww = (jd - 1) / DAY_MAX + 1;
   else
     /*
        Date has week number of last week of previous year:
          Detect that week number by calling this function recursivly  ;)
     */
     ww = raw_week_number (dvec[MONTH_MAX-1], MONTH_MAX, year-1);

   return(ww);
}



   LOCAL const char *
dflt_day_name (day)
   const int day;
/*
   Returns the complete default (==English) name of the day
     (needed by the `compare_d_m_name()' function if another native language
     other than English must be supported).
*/
{
   static const char  *name[]={
                                "invalid day",
                                "Monday", "Tuesday",  "Wednesday", "Thursday",
                                "Friday", "Saturday", "Sunday"
                              };

   return(((day<DAY_MIN)||(day>DAY_MAX)) ? name[0] : name[day]);
}



   LOCAL const char *
dflt_month_name (month)
   const int month;
/*
   Returns the complete default (==English) name of the month
     (needed by the `compare_d_m_name()' function if another native lanuage
     other than English must be supported).
*/
{
   static const char  *name[]={
                                "invalid month",
                                "January", "February", "March",
                                "April",   "May",      "June",
                                "July",    "August",   "September",
                                "October", "November", "December"
                              };

   return(((month<MONTH_MIN)||(month>MONTH_MAX)) ? name[0] : name[month]);
}
