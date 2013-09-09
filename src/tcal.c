/*
*  tcal.c:  Start `gcal' with date set ONE day ahead (default mode,
*             like the `--shift=1' long-style option is passed to it).
*             Useful if you call this program in `~/.profile' and you
*             want to see all fixed dates related to tomorrow + 1 day
*             (advanced  -ct  respectively  --period-of-fixed-dates=t  option).
*             Works *only* for Gregorian years!
*
*  Copyright (C) 1995, 1996, 1997 Thomas Esken
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



static char rcsid[]="$Id: tcal.c 2.92 1997/03/31 02:09:02 tom Exp $";



/*
*  Include header files.
*/
#include "tailor.h"
#if HAVE_CTYPE_H
#  include <ctype.h>
#endif
#if HAVE_UNISTD_H
#  include <unistd.h>
#endif
#if HAVE_LIMITS_H
#  include <limits.h>
#endif
#if HAVE_ERRNO_H
#  include <errno.h>
#endif
#if (!HAVE_SIGNAL_H || !HAVE_SIGNAL) && HAVE_SYS_TYPES_H   /* Otherwise "gcal.h" includes <sys/types.h> */
#  include <sys/types.h>
#endif
#if HAVE_SYS_STAT_H
#  include <sys/stat.h>
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
#ifndef USE_RC
#  define  USE_RC  1
#endif
#include "gcal.h"



/*
*   Program specific preprocessor symbols.
*/
#define  ENV_VAR_GCALPROG  "GCALPROG"
#define  SHIFT_VALUE_DFT   "1"



/*
*  Function prototypes.
*/
#if __cplusplus
extern "C"
{
#endif
LOCAL void
usage_msg __P_((      FILE *fp,
                const char *prgr_name,
                      int   exit_status));
LOCAL void
version_msg __P_((      FILE *fp,
                  const char *prgr_name,
                        int   exit_status));
LOCAL VOID_PTR
my_malloc __P_((const int   amount,
                const int   exit_status,
                const char *module_name,
                const long  module_line,
                const char *var_name,
                const int   var_contents));
LOCAL VOID_PTR
my_realloc __P_((      VOID_PTR  ptr_memblock,
                 const int       amount,
                 const int       exit_status,
                 const char     *module_name,
                 const long      module_line,
                 const char     *var_name,
                 const int       var_contents));
LOCAL void
my_error __P_((const int   exit_status,
               const char *module_name,
               const long  module_line,
               const char *var_name,
               const int   var_contents));
#if HAVE_SIGNAL && (defined(SIGINT) || defined(SIGTERM) || defined(SIGHUP))
LOCAL RETSIGTYPE
handle_signal __P_((int the_signal));
#endif
#if !HAVE_STRNCASECMP
LOCAL int
my_strncasecmp __P_((const char *s1,
                     const char *s2,
                           int   len));
#endif /* !HAVE_STRNCASECMP */
LOCAL int
days_of_february __P_((const int year));
LOCAL Bool
valid_date __P_((const int day,
                 const int month,
                 const int year));
LOCAL void
get_actual_date __P_((int *day,
                      int *month,
                      int *year));
LOCAL void
prev_date __P_((int *day,
                int *month,
                int *year));
LOCAL void
next_date __P_((int *day,
                int *month,
                int *year));
EXPORT int
main __P_((int   argc,
           char *argv[]));
#if __cplusplus
}
#endif



/*
*  Define program global variables.
*/
/*
   Number of days in months.
*/
LOCAL const int  dvec[]=
 {
   31, 28, 31, 30, 31, 30,
   31, 31, 30, 31, 30, 31
 };
#ifdef DJG
LOCAL Usint  testval;                 /* Set to SHRT_MAX for checking the maximum table range */
#else
LOCAL Uint   testval;                 /* Set to INT_MAX for checking the maximum table range */
#endif
LOCAL Uint   maxlen_max=MAXLEN_MAX;   /* Actual length of all strings */
LOCAL char  *prgr_name;               /* The name of this executable */
LOCAL char  *s1;                      /* General purpose text buffer */



   LOCAL void
usage_msg (fp, prgr_name, exit_status)
         FILE *fp;
   const char *prgr_name;
         int   exit_status;
/*
   Writes the program "usage" text to file `fp' and
     terminates the program with `exit_status'.
*/
{
#if USE_DE
   fprintf(fp, "Aufruf:  %s  [--help | --version] | [--shift=[+|-]ZAHL] [ARGUMENT...]\n", prgr_name);
   if (!exit_status)
    {
      S_NEWLINE(fp);
      fprintf(fp, "Fehlerberichte via eMail an <esken@uni-muenster.de>");
      S_NEWLINE(fp);
      fprintf(fp, "oder (falls das fehlschl%sgt) an <bug-gnu-utils@prep.ai.mit.edu>.", AE);
      S_NEWLINE(fp);
    }
#else /* !USE_DE */
   fprintf(fp, _("Usage:  %s  [--help | --version] | [--shift=[+|-]NUMBER] [ARGUMENT...]\n"), prgr_name);
   if (!exit_status)
    {
      S_NEWLINE(fp);
      fprintf(fp, _("Email bug reports to <esken@uni-muenster.de>"));
      S_NEWLINE(fp);
      fprintf(fp, _("or (if this fails) to <bug-gnu-utils@prep.ai.mit.edu>."));
      S_NEWLINE(fp);
    }
#endif /* !USE_DE */
   exit(exit_status);
}



   LOCAL void
version_msg (fp, prgr_name, exit_status)
         FILE *fp;
   const char *prgr_name;
         int   exit_status;
/*
   Writes the program "version" text to file `fp' and
     terminates the program with `exit_status'.
*/
{
   auto char  *ptr_rcsid=rcsid+12;


   rcsid[16] = '\0';
   fprintf(fp, "%s (GNU cal %s) %s\n", prgr_name, VERSION_NO, ptr_rcsid);
   fprintf(fp, "Copyright (C) 1995-1997 Thomas Esken\n");
#if USE_DE
   fprintf(fp, "Dies ist freie Software; in den Quellen befindet sich die Lizenz-");
   S_NEWLINE(fp);
   fprintf(fp, "und Kopierbedingung.  Es gibt KEINERLEI Garantie, nicht einmal f%sr", UE);
   S_NEWLINE(fp);
   fprintf(fp, "die TAUGLICHKEIT oder die VERWENDBARKEIT ZU EINEM ANGEGEBENEN ZWECK.");
#else /* !USE_DE */
   fprintf(fp, _("This is free software; see the source for copying conditions."));
   S_NEWLINE(fp);
   fprintf(fp, _("There is NO warranty, without even the implied warranty of"));
   S_NEWLINE(fp);
   fprintf(fp, _("MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE."));
#endif /* !USE_DE */
   S_NEWLINE(fp);
   exit(exit_status);
}



   LOCAL VOID_PTR
my_malloc (amount, exit_status, module_name, module_line, var_name, var_contents)
   const int   amount;
   const int   exit_status;
   const char *module_name;
   const long  module_line;
   const char *var_name;
   const int   var_contents;
/*
   Allocate AMOUNT bytes of memory dynamically, with error checking.  Calls `my_error()'
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



   LOCAL VOID_PTR
my_realloc (ptr_memblock, amount, exit_status, module_name, module_line, var_name, var_contents)
         VOID_PTR  ptr_memblock;
   const int       amount;
   const int       exit_status;
   const char     *module_name;
   const long      module_line;
   const char     *var_name;
   const int       var_contents;
/*
   Change the size of an allocated block of memory PTR_MEMBLOCK to AMOUNT bytes,
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



   LOCAL void
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
      case 124:
        fprintf(stderr, "`%s' Zeile %ld: virtueller Speicher ersch%spft (%s=%d)",
                module_name, module_line, OE, var_name, var_contents);
        break;
      case 107:
        fprintf(stderr, "`%s' Zeile %ld: (`%s') ung%sltiger Wert f%sr Tabellengr%s%se `sizeof %s>%d'",
                module_name, module_line, INTERNAL_TXT, UE, UE, OE, SZ, var_name, var_contents);
        break;
      case 2:
        fprintf(stderr, "Versatzwert `%s' ist ung%sltig", var_name, UE);
        break;
      default:
        fprintf(stderr, "`%s' Zeile %ld: (`%s') unbehandelter Fehler (%d)",
                module_name, module_line, INTERNAL_TXT, exit_status);
#else /* !USE_DE */
      case 124:
        fprintf(stderr, _("`%s' line %ld: virtual memory exhausted (%s=%d)"),
                module_name, module_line, var_name, var_contents);
        break;
      case 107:
        fprintf(stderr, _("`%s' line %ld: (`%s') invalid value for table size `sizeof %s>%d'"),
                module_name, module_line, _("Internal"), var_name, var_contents);
        break;
      case 2:
        fprintf(stderr, _("shift value `%s' is invalid"), var_name);
        break;
      default:
        fprintf(stderr, _("`%s' line %ld: (`%s') unmanaged error (%d)"),
                module_name, module_line, _("Internal"), exit_status);
#endif /* !USE_DE */
    }
   S_NEWLINE(stderr);
   exit(exit_status);
}



#if HAVE_SIGNAL && (defined(SIGINT) || defined(SIGTERM) || defined(SIGHUP))
   LOCAL RETSIGTYPE
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
   exit(3);
}
#endif /* HAVE_SIGNAL && (SIGINT || SIGTERM || SIGHUP) */



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



   LOCAL int
days_of_february (year)
   const int year;
/*
   Computes the number of days in February and returns them.
*/
{
   return((year&3) ? 28 : (!(year%100)&&(year%400)) ? 28 : 29);
}



   LOCAL Bool
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



   LOCAL void
prev_date (day, month, year)
   int *day;
   int *month;
   int *year;
/*
   Sets a delivered date back by one day (to yesterday's date).
*/
{
   (*day)--;
   if (   !*day
       || !valid_date (*day, *month, *year))
    {
      (*month)--;
      if (*month < MONTH_MIN)
       {
         *month = MONTH_MAX;
         (*year)--;
       }
      if (*month == 2)
        *day = days_of_february (*year);
      else
        *day = dvec[*month-1];
    }
}



   LOCAL void
next_date (day, month, year)
   int *day;
   int *month;
   int *year;
/*
   Sets the delivered date forwards by one day (to tomorrow's date).
*/
{
   (*day)++;
   if (!valid_date (*day, *month, *year))
    {
      *day = DAY_MIN;
      if (*month == MONTH_MAX)
       {
         *month = MONTH_MIN;
         (*year)++;
       }
      else
        (*month)++;
    }
}



   LOCAL void
get_actual_date (day, month, year)
  int *day;
  int *month;
  int *year;
/*
   Gets the actual date from the system.
*/
{
   auto struct tm         *sys_date;
   auto        MY_TIME_T   sys_time;


   sys_time  = time((MY_TIME_T *)NULL);
   sys_date  = localtime(&sys_time);
   *day   = sys_date->tm_mday;
   *month = sys_date->tm_mon + 1;
   *year  = sys_date->tm_year;
   if (*year < CENTURY)
     *year += CENTURY;
}



   PUBLIC int
main (argc, argv)
   int   argc;
   char *argv[];
/*
   Starts Gcal with date set one day ahead (default mode, like the `--shift=1'
     long-style option is passed to it) and all other arguments which are given
     in the command line.  The program first tries to use the `gcal' executable
     which is specified in the environment variable $GCALPROG if it is set,
     otherwise it will be searched using the $PATH environment variable.
*/
{
   auto     Slint  shift=(Slint)CHR2DIG(*SHIFT_VALUE_DFT);
   register int    len_year_max;
   register int    i;
   register int    act_len;
   register int    arg_len;
   register int    status;
   auto     int    day;
   auto     int    month;
   auto     int    year;
   auto     char  *gcal_name;
   auto     char  *gcal_line;
   auto     char  *ptr_char;
   auto     char  *buf_ptr_char;
   auto     Bool   shift_set=FALSE;


#ifdef GCAL_NLS
   /*
      Now initialize the NLS functions.
   */
#    if HAVE_SETLOCALE
   setlocale(LC_ALL, "");
#    endif
#    ifndef LOCALEDIR
#      define LOCALEDIR  NULL
#    endif
   bindtextdomain(PACKAGE, LOCALEDIR);
   textdomain(PACKAGE);
#endif
   /*
      Let's set `testval' to SHRT_MAX/INT_MAX if SHRT_MAX/INT_MAX itself isn't
        defined.  This solution only works on machines with internal arithmethics
        based on "two complements".
   */
#ifdef DJG
#  ifdef SHRT_MAX
   testval = SHRT_MAX;
#  else /* !SHRT_MAX */
   testval = ~0;
   testval >>= 1;
#  endif /* !SHRT_MAX */
#else /* !DJG */
#  ifdef INT_MAX
   testval = INT_MAX;
#  else /* !INT_MAX */
   testval = ~0;
   testval >>= 1;
#  endif /* !INT_MAX */
#endif /* !DJG */
   /*
      Initial memory allocation for global string.
   */
   s1 = (char *)my_malloc (MAXLEN_MAX, 124, __FILE__, (long)__LINE__, "s1", 0);
   /*
      Compute the string length of the maximum year able to compute.
   */
   sprintf(s1, "%d", YEAR_MAX);
   len_year_max = (int)strlen(s1);
   /*
      Detect the own program name.
   */
   i = (int)strlen(argv[0]);
   if ((Uint)i >= maxlen_max)
     s1 = my_realloc ((VOID_PTR)s1, i+1, 124, __FILE__, (long)__LINE__, "s1", i+1);
   if (!i)
    {
      strcpy(s1, rcsid+5);
      s1[5] = '\0';
    }
   else
     strcpy(s1, argv[0]);
#ifdef SUFFIX_SEP
   /*
      Eliminate version suffix under VMS.
   */
   ptr_char = strrchr(s1, *SUFFIX_SEP);
   if (ptr_char != (char *)NULL)
     *ptr_char = '\0';
#endif
   i = (int)strlen(s1);
#ifdef DJG
   ptr_char = strrchr(s1, *DIR2_SEP);
#else /* !DJG */
   ptr_char = strrchr(s1, *DIR_SEP);
#endif /* !DJG */
   if (ptr_char != (char *)NULL)
    {
      ptr_char++;
      i = (int)strlen(ptr_char);
    }
   else
     ptr_char = s1;
   if (tolower('A') == 'a')
    {
      buf_ptr_char = ptr_char;
      for ( ; *ptr_char ; ptr_char++)
        *ptr_char = (char)tolower(*ptr_char);
      ptr_char = buf_ptr_char;
    }
   /*
      Suppress ".exe" suffix for MSDOS, OS/2 and VMS.
   */
   if (   (i > 4)
       && !strcmp(ptr_char+i-4, ".exe"))
    {
      i -= 4;
      *(ptr_char + i) = '\0';
    }
   prgr_name = (char *)my_malloc (i+1, 124, __FILE__, (long)__LINE__, "prgr_name", 0);
   strcpy(prgr_name, ptr_char);
#if HAVE_SIGNAL
   /*
      Now let's modify the signal handling a bit to make sure that
        temporary files are always deleted if such signals are raised.
   */
#  ifdef SIGINT
   if (signal(SIGINT, SIG_IGN) != SIG_IGN)
     (void)signal(SIGINT, (Sig_type)handle_signal);
#  endif
#  ifdef SIGTERM
   if (signal(SIGTERM, SIG_IGN) != SIG_IGN)
     (void)signal(SIGTERM, (Sig_type)handle_signal);
#  endif
#  ifdef SIGHUP
   if (signal(SIGHUP, SIG_IGN) != SIG_IGN)
     (void)signal(SIGHUP, (Sig_type)handle_signal);
#  endif
#endif /* HAVE_SIGNAL */
   buf_ptr_char = SHIFT_VALUE_DFT;
   if (argc > 1)
    {
      /*
         Check if `--help', `--version' or `--shift=[+|-]NUMBER' long-style option is given.
      */
      if (   (strlen(argv[1]) > 2)
          && (*argv[1] == *SWITCH)
          && (*(argv[1]+1) == *SWITCH))
       {
         auto Bool  is_number=TRUE;


         if (!strncasecmp(argv[1]+2, "help", strlen(argv[1]+2)))
           usage_msg (stdout, prgr_name, 0);
         if (!strncasecmp(argv[1]+2, "version", strlen(argv[1]+2)))
           version_msg (stdout, prgr_name, 0);
         if (!strncasecmp(argv[1]+2, "shi", 3))
          {
            ptr_char = strchr(argv[1]+2, *LARG_SEP);
            if (   ptr_char == (char *)NULL
                || (   (ptr_char != (char *)NULL)
                    && !*(ptr_char+1)))
             {
               /*
                  Error, option requires an argument.
               */
#if USE_DE
               fprintf(stderr, "%s: Option `%s' ben%stigt ein Argument",
                       prgr_name, argv[1], OE);
#else /* !USE_DE */
               fprintf(stderr, _("%s: option `%s' requires an argument"),
                       prgr_name, argv[1]);
#endif /* !USE_DE */
               S_NEWLINE(stderr);
               usage_msg (stderr, prgr_name, 1);
             }
            else
             {
               i = (int)(ptr_char - (argv[1] + 2));
               switch (i)
                {
                  case 3:
                  case 4:
                  case 5:
                    if (!strncasecmp(argv[1]+2, "shift", i))
                      break;
                  default:
                    /*
                       Error, unrecognized option.
                    */
#if USE_DE
                    fprintf(stderr, "%s: unbekannte Option `%s'",
                            prgr_name, argv[1]);
#else /* !USE_DE */
                    fprintf(stderr, _("%s: unrecognized option `%s'"),
                            prgr_name, argv[1]);
#endif /* !USE_DE */
                    S_NEWLINE(stderr);
                    usage_msg (stderr, prgr_name, 1);
                }
               /*
                  Let's get the argument of the `--shift=[+|-]NUMBER' long-style option.
               */
               ptr_char++;
               shift = atol(ptr_char);
               /*
                  Check if the argument is a NUMBER.
               */
               buf_ptr_char = ptr_char;
               if (   *ptr_char == '+'
                   || *ptr_char == '-')
                 ptr_char++;
               if (!*ptr_char)
                 is_number = FALSE;
               else
                {
                  while (isdigit(*ptr_char))
                    ptr_char++;
                  if (*ptr_char)
                    is_number = FALSE;
                }
               if (!is_number)
                {
                  /*
                     Error, invalid argument.
                  */
#if USE_DE
                  fprintf(stderr, "%s: Option mit unzul%sssigem Argument -- %s",
                          prgr_name, AE, argv[1]);
#else /* !USE_DE */
                  fprintf(stderr, _("%s: option with invalid argument -- %s"),
                          prgr_name, argv[1]);
#endif /* !USE_DE */
                  S_NEWLINE(stderr);
                  usage_msg (stderr, prgr_name, 1);
                }
               shift_set = TRUE;
             }
          }
       }
    }
   /*
      Detect the name of the Gcal executable.
   */
   gcal_name = getenv(ENV_VAR_GCALPROG);
   if (gcal_name != (char *)NULL)
    {
      if (!*gcal_name)
        gcal_name = PRGR_NAME;
    }
   else
     gcal_name = PRGR_NAME;
   /*
      Compute the actual date.
   */
   get_actual_date (&day, &month, &year);
   /*
      Now calculate the shifted date (default mode is tomorrow [+1]).
   */
   if (shift > 0L)
     for ( ; shift ; shift--)
      {
        next_date (&day, &month, &year);
        if (year > YEAR_MAX)
          my_error (2, "", 0L, buf_ptr_char, 0);
      }
   else
     if (shift < 0L)
       for ( ; shift ; shift++)
        {
          prev_date (&day, &month, &year);
          if (year < YEAR_MIN)
            my_error (2, "", 0L, buf_ptr_char, 0);
        }
   /*
      Create the base command line including the actual date modifier %...
        which is used in the system() call to run Gcal.
   */
   gcal_line = (char *)my_malloc (MAXLEN_MAX, 124, __FILE__, (long)__LINE__, "gcal_line", 0);
   sprintf(gcal_line, "%s %c%0*d%02d%02d",
           gcal_name, RC_ADATE_CHAR, len_year_max, year, month, day);
   act_len = (int)strlen(gcal_line);
   /*
      Skip the locally respected `--shift=[+|-]NUMBER' long-style option
        if given in the command line.
   */
   if (shift_set)
    {
      argc--;
      argv++;
    }
   /*
      Now append all command line arguments to "gcal_line".
   */
   while (argc > 1)
    {
      strcat(gcal_line, " ");
      argv++;
      arg_len = (int)strlen(*argv);
      if ((Uint)act_len+arg_len >= maxlen_max)
       {
         maxlen_max <<= 1;
         if (maxlen_max > testval)
           maxlen_max = testval;
         gcal_line = (char *)my_realloc ((VOID_PTR)gcal_line, maxlen_max,
                                         124, __FILE__, (long)__LINE__,
                                         "gcal_line", maxlen_max);
       }
      strcat(gcal_line, *argv);
      act_len += (arg_len + 1);
      argc--;
    }
   status = system(gcal_line);
   if (status == -1)
    {
#if USE_DE
      fprintf(stderr, "%s: Fehler bei Programmausf%shrung von `%s'\n",
              prgr_name, UE, gcal_name);
#else /* !USE_DE */
      fprintf(stderr, _("%s: error during program execution of `%s'\n"),
              prgr_name, gcal_name);
#endif /* !USE_DE */
#if HAVE_ERRNO_H
      perror(gcal_name);
#endif
      exit(status);
    }

   return(status);
}
