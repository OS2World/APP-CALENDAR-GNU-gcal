/*
*  gcal2txt.c:  Creates a verbatim text file from a Gcal resource file.
*
*
*  Copyright (C) 1996, 1997 Thomas Esken
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



static char rcsid[]="$Id: gcal2txt.c 1.24 1997/03/31 01:02:04 tom Exp $";



#include "tailor.h"
#if HAVE_CTYPE_H
#  include <ctype.h>
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
#ifndef USE_RC
#  define  USE_RC  1
#endif
#include "gcal.h"



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
EXPORT int
main __P_((int   argc,
           char *argv[]));
#if __cplusplus
}
#endif



/*
*  Define program global variables.
*/
#ifdef DJG
LOCAL Usint  testval;                 /* Set to SHRT_MAX for checking the maximum table range */
#else
LOCAL Uint   testval;                 /* Set to INT_MAX for checking the maximum table range */
#endif
LOCAL Uint   maxlen_max=MAXLEN_MAX;   /* Actual length of all strings */
LOCAL char  *prgr_name;               /* The name of this executable */



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
   fprintf(fp, "Aufruf:  %s  [--help | --version] | [RESSOURCENDATEI | -]\n", prgr_name);
   if (!exit_status)
    {
      S_NEWLINE(fp);
      fprintf(fp, "Fehlerberichte via eMail an <esken@uni-muenster.de>");
      S_NEWLINE(fp);
      fprintf(fp, "oder (falls das fehlschl%sgt) an <bug-gnu-utils@prep.ai.mit.edu>.", AE);
      S_NEWLINE(fp);
    }
#else /* !USE_DE */
   fprintf(fp, _("Usage:  %s  [--help | --version] | [RESOURCE-FILE | -]\n"), prgr_name);
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
   auto char  *ptr_rcsid=rcsid+16;


   rcsid[20] = '\0';
   fprintf(fp, "%s (GNU cal %s) %s\n", prgr_name, VERSION_NO, ptr_rcsid);
   fprintf(fp, "Copyright (C) 1996-1997 Thomas Esken\n");
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
      default:
        fprintf(stderr, "`%s' Zeile %ld: (`%s') unbehandelter Fehler (%d)",
                module_name, module_line, INTERNAL_TXT, exit_status);
        break;
#else /* !USE_DE */
      case 124:
        fprintf(stderr, _("`%s' line %ld: virtual memory exhausted (%s=%d)"),
                module_name, module_line, var_name, var_contents);
        break;
      case 107:
        fprintf(stderr, _("`%s' line %ld: (`%s') invalid value for table size `sizeof %s>%d'"),
                module_name, module_line, _("Internal"), var_name, var_contents);
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
     on stderr channel and terminates the program with an exit status of 3.
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



   PUBLIC int
main (argc, argv)
   int   argc;
   char *argv[];
/*
   Creates a verbatim text file from a Gcal resource file (optional argument 1)
     and displays the results on the stdout channel.  If no text file name or
     only a dash '-' is given, read input from stdin channel.
*/
{
   auto     FILE  *fp=(FILE *)NULL;
   register int    i;
   register int    ch;
   register int    ch2;
   register int    ch3;
   auto     char  *s1;
   auto     char  *ptr_char;
   auto     Bool   is_regular_file=TRUE;


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
      Detect the own program name.
   */
   i = (int)strlen(argv[0]);
   if ((Uint)i >= maxlen_max)
     s1 = my_realloc ((VOID_PTR)s1, i+1, 124, __FILE__, (long)__LINE__, "s1", i+1);
   if (!i)
    {
      strcpy(s1, rcsid+5);
      s1[9] = '\0';
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
      auto char  *buf_ptr_char=ptr_char;


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
   if (argc > 1)
    {
      if (   (strlen(argv[1]) == 1)
          && (*argv[1] == *SWITCH))
        /*
           Stdin channel explicitly wanted!
        */
        fp = stdin;
      else
       {
         if (   (strlen(argv[1]) > 2)
             && (*argv[1] == *SWITCH)
             && (*(argv[1]+1) == *SWITCH))
          {
            if (!strncasecmp(argv[1]+2, "help", strlen(argv[1]+2)))
              usage_msg (stdout, prgr_name, 0);
            if (!strncasecmp(argv[1]+2, "version", strlen(argv[1]+2)))
              version_msg (stdout, prgr_name, 0);
            /*
               Error, unknown long-style option given.
            */
#if USE_DE
            fprintf(stderr, "%s: unbekannte Option `%s'", prgr_name, argv[1]);
#else /* !USE_DE */
            fprintf(stderr, _("%s: unrecognized option `%s'"), prgr_name, argv[1]);
#endif /* !USE_DE */
            S_NEWLINE(stderr);
            usage_msg (stderr, prgr_name, 1);
          }
         else
          {
#if HAVE_SYS_STAT_H && defined(S_IFMT) && defined(S_IFREG)
             auto struct stat  statbuf;


             /*
                Test if the file is a regular file, if not, this is an error!
             */
             if (!stat(argv[1], &statbuf))
              {
                if ((statbuf.st_mode & S_IFMT) == S_IFREG)
                  fp = fopen(argv[1], "r");
                else
                  is_regular_file = FALSE;
              }
#else /* !HAVE_SYS_STAT_H || !S_IFMT || !S_IFREG */
             fp = fopen(argv[1], "r");
#endif /* !HAVE_SYS_STAT_H || !S_IFMT || !S_IFREG */
           }
       }
    }
   else
     fp = stdin;
   if (fp == (FILE *)NULL)
    {
      i = (int)strlen(prgr_name) + strlen(argv[1]) + 5;
      if ((Uint)i >= maxlen_max)
        s1 = (char *)my_realloc ((VOID_PTR)s1, i+1, 124,
                                 __FILE__, (long)__LINE__, "s1", 0);
      sprintf(s1, "%s: (%s) ", prgr_name, argv[1]);
#if HAVE_ERRNO_H
      if (!is_regular_file)
#  if USE_DE
        fprintf(stderr, "%s: keine regul%sre Datei\n", s1, AE);
#  else /* !USE_DE */
        fprintf(stderr, _("%s: no regular file\n"), s1);
#  endif /* !USE_DE */
      else
        perror(s1);
#else /* !HAVE_ERRNO_H */
#  if USE_DE
      fprintf(stderr, "%s: Datei nicht gefunden\n", s1);
#  else /* !USE_DE */
      fprintf(stderr, _("%s: file not found\n"), s1);
#  endif /* !USE_DE */
#endif /* !HAVE_ERRNO_H */
      exit(1);
    }
   /*
      Skip the "date"-part of the line.
   */
   while ((ch=fgetc(fp)) != EOF)
     if (isspace(ch))
      {
        ch = fgetc(fp);
        if (ch != EOF)
         {
           if (ch != QUOTE_CHAR)
             ungetc(ch, fp);
         }
        break;
      }
   /*
      Manage the "text"-part of the line.
   */
   while ((ch=fgetc(fp)) != EOF)
    {
      switch (ch)
       {
         case RC_NL_CHAR:
         case RC_NL2_CHAR:
           ch2 = fgetc(fp);
           if (ch2 != EOF)
             switch (ch2)
              {
                case QUOTE_CHAR:
                  ch3 = fgetc(fp);
                  if (ch3 != EOF)
                    switch (ch3)
                     {
                       case '\n':
                         fputc(ch3, stdout);
                         break;
                       default:
                         fprintf(stdout, "%c%c%c", ch, ch2, ch3);
                     }
                  else
                    fprintf(stdout, "%c%c", ch, ch2);
                  break;
                default:
                  fprintf(stdout, "%c%c", ch, ch2);
              }
           else
             fputc(ch, stdout);
           break;
         case QUOTE_CHAR:
           ch2 = fgetc(fp);
           if (ch2 != EOF)
             switch (ch2)
              {
                case RC_TVAR_CHAR:
                  ch3 = fgetc(fp);
                  if (ch3 != EOF)
                   {
                     if (isalpha(ch3))
                       fprintf(stdout, "%c%c", ch2, ch3);
                     else
                       fprintf(stdout, "%c%c%c", ch, ch2, ch3);
                     break;
                   }
                  else
                    fputc(ch, stdout);
                case '\n':
                case RC_NL_CHAR:
                case RC_NL2_CHAR:
                  fputc(ch2, stdout);
                  break;
                case RC_SPECIAL_TEXT_CHAR:
                  ch3 = fgetc(fp);
                  if (ch3 != EOF)
                    switch(ch3)
                     {
                       case RC_SHELL_ESC_CHAR:
                       case RC_EX_LHDY_CHAR:
                       case RC_EX_NLHDY_CHAR:
                       case RC_EX_AHDY_CHAR:
                       case RC_EX_NAHDY_CHAR:
                       case RC_EX_MON_CHAR:
                       case RC_EX_NMON_CHAR:
                       case RC_EX_TUE_CHAR:
                       case RC_EX_NTUE_CHAR:
                       case RC_EX_WED_CHAR:
                       case RC_EX_NWED_CHAR:
                       case RC_EX_THU_CHAR:
                       case RC_EX_NTHU_CHAR:
                       case RC_EX_FRI_CHAR:
                       case RC_EX_NFRI_CHAR:
                       case RC_EX_SAT_CHAR:
                       case RC_EX_NSAT_CHAR:
                       case RC_EX_SUN_CHAR:
                       case RC_EX_NSUN_CHAR:
                       case RC_EX_MON_2_THU_CHAR:
                       case RC_EX_NMON_2_THU_CHAR:
                       case RC_EX_MON_2_FRI_CHAR:
                       case RC_EX_NMON_2_FRI_CHAR:
                       case RC_IDATE_CHAR:
                       case RC_EDATE_CHAR:
                       case RC_WDNAME_CHAR:
                       case RC_WDNAME3_CHAR:
                       case RC_WDNAME2_CHAR:
                       case RC_WDNR_M1_2_S7_CHAR:
                       case RC_WDNR_M0_2_S6_CHAR:
                       case RC_WDNR_M1_2_S7S_CHAR:
                       case RC_WDNR_M0_2_S6S_CHAR:
                       case RC_WDNR_S1_2_S7_CHAR:
                       case RC_WDNR_S0_2_S6_CHAR:
                       case RC_WDNR_S1_2_S7S_CHAR:
                       case RC_WDNR_S0_2_S6S_CHAR:
                       case RC_WDNR_X1_2_X7_CHAR:
                       case RC_WDNR_X0_2_X6_CHAR:
                       case RC_WDNR_X1_2_X7S_CHAR:
                       case RC_WDNR_X0_2_X6S_CHAR:
                       case RC_DOYNR_NZ_CHAR:
                       case RC_DOYNR_LZ_CHAR:
                       case RC_DOYNR_NZS_CHAR:
                       case RC_DOYNR_LZS_CHAR:
                       case RC_DAYNR_NZ_CHAR:
                       case RC_DAYNR_LZ_CHAR:
                       case RC_DAYNR_NZS_CHAR:
                       case RC_DAYNR_LZS_CHAR:
                       case RC_MONTHNAME_CHAR:
                       case RC_MONTHNAME3_CHAR:
                       case RC_MONTHNR_NZ_CHAR:
                       case RC_MONTHNR_LZ_CHAR:
                       case RC_MONTHNR_NZS_CHAR:
                       case RC_MONTHNR_LZS_CHAR:
                       case RC_YEARNR_NZ_CHAR:
                       case RC_YEARNR_LZ_CHAR:
                       case RC_WEEKNR_CHAR:
                       case RC_BYEAR_CHAR:
                       case RC_BYEAR_S_CHAR:
                       case RC_MOON_NZ_CHAR:
                       case RC_MOON_LZ_CHAR:
                       case RC_MOON_IMAGE_CHAR:
                       case RC_BIO_CHAR:
                       case RC_BIO_BAR_CHAR:
                       case RC_TDATE_CHAR:
                       case RC_JDAYS_CHAR:
                       case RC_TIME_CHAR:
                       case RC_DAY_DIFF_CHAR:
                       case RC_WEEK_DIFF_CHAR:
                       case RC_MONTH_DIFF_CHAR:
                       case RC_YEAR_DIFF_CHAR:
                       case RC_HLS1S_CHAR:
                       case RC_HLS1E_CHAR:
                       case RC_HLS2S_CHAR:
                       case RC_HLS2E_CHAR:
                       case RC_HLS3S_CHAR:
                       case RC_HLS3E_CHAR:
                       case RC_HLS4S_CHAR:
                       case RC_HLS4E_CHAR:
                       case RC_HLS5S_CHAR:
                       case RC_HLS5E_CHAR:
                         fprintf(stdout, "%c%c", ch2, ch3);
                         break;
                       default:
                         fprintf(stdout, "%c%c%c", ch, ch2, ch3);
                     }
                  else
                    fprintf(stdout, "%c%c", ch, ch2);
                  break;
                default:
                  fputc(ch, stdout);
                  ungetc(ch2, fp);
              }
           break;
         default:
           fputc(ch, stdout);
       }
    }
   fclose(fp);

   return(0);
}
