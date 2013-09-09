/*
*  help.c:  The help, version and license screens.
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
static char rcsid[]="$Id: help.c 2.40 1997/04/09 02:04:00 tom Exp $";
#endif



/*
*  Include header files.
*/
#include "tailor.h"
#if HAVE_CTYPE_H
#  include <ctype.h>
#endif
#include "gcal.h"



/*
*  Function prototypes.
*/
#if __cplusplus
extern "C"
{
#endif
/*
************************************************** Defined in `tty.c'.
*/
IMPORT void
print_text __P_((FILE *fp,
                 char *text_line));
/*
************************************************** Defined in `utils.c'.
*/
IMPORT VOID_PTR
my_malloc __P_((const int   amount,
                const int   exit_status,
                const char *module_name,
                const long  module_line,
                const char *var_name,
                const int   var_contents));
IMPORT VOID_PTR
my_realloc __P_((      VOID_PTR  ptr_memblock,
                 const int       amount,
                 const int       exit_status,
                 const char     *module_name,
                 const long      module_line,
                 const char     *var_name,
                 const int       var_contents));
#if !HAVE_STRNCASECMP
IMPORT int
my_strncasecmp __P_((const char *s1,
                     const char *s2,
                           int   len));
#endif /* !HAVE_STRNCASECMP */
IMPORT const char *
short_day_name __P_((const int day));
IMPORT const char *
short3_day_name __P_((const int day));
IMPORT const char *
short_month_name __P_((const int month));
/*
************************************************** Defined in `help.c'.
*/
EXPORT void
my_help_on_help __P_((      FILE *fp,
                      const char *longopt,
                      const Bool  lopt_ambig,
                      const int   cols));
EXPORT void
my_extended_help __P_((      FILE *fp,
                       const int   longopt_symbolic));
EXPORT void
my_basic_help __P_((FILE *fp));
EXPORT void
my_license __P_((FILE *fp));
EXPORT void
my_version __P_((FILE *fp));
EXPORT char *
usage_msg __P_((void));
EXPORT void
put_longopt_description __P_((FILE *fp));
LOCAL const char *
get_longopt_description __P_((const int  longopt_symbolic,
                              const Bool with_larglist));
LOCAL void
my_copyright __P_((      FILE *fp,
                   const Bool  with_short_licence));
LOCAL void
my_help_head_text __P_((FILE *fp));
LOCAL void
my_help_tail_text __P_((FILE *fp));
LOCAL void
print_compiler_info __P_((FILE *fp));
#if __cplusplus
}
#endif



/*
*  Declare public(extern) variables.
*/
IMPORT const Lopt_struct  lopt[];               /* The table of long-style options */
IMPORT const Cc_struct  cc_holidays[];          /* The table of country specific holidays */
#if USE_RC
IMPORT const Ed_struct  info_exclusive_day[];   /* The table of info of %? special texts for day exclusion */
#endif
IMPORT const Df_struct  info_date_format[];     /* The table of info of supported date formats */
IMPORT Df_struct    supported_date_format[];    /* The table of supported date formats */
IMPORT Df_struct   *date_format;                /* Points to the used date format */
IMPORT Greg_struct  greg_reform_date[];         /* The table of Gregorian Reformation dates */
IMPORT Greg_struct *greg;                       /* Points to the used Gregorian Reformation date */
IMPORT Hls_struct   ehls1s;                     /* Effective hls 1 start (current day) */
IMPORT Hls_struct   ehls1e;                     /* Effective hls 1 end (current day) */
IMPORT Hls_struct   ehls2s;                     /* Effective hls 2 start (holiday) */
IMPORT Hls_struct   ehls2e;                     /* Effective hls 2 end (holiday) */
IMPORT int          len_year_max;               /* String length of the maximum year able to compute */
IMPORT int          act_year;                   /* Actual year */
IMPORT int          is_tty;                     /* Is output displayed on a terminal? */
IMPORT int          len_prgr_name;              /* Length of actual program name */
IMPORT char        *yy_lit;                     /* The "yy" text */
IMPORT char        *yyyy_lit;                   /* The "yyyy" text */
IMPORT char        *mm_lit;                     /* The "mm" text */
IMPORT char        *www_lit;                    /* The "www" text */
IMPORT char        *dd_lit;                     /* The "dd" text */
IMPORT char        *larg_lit;                   /* The "ARG" text */
IMPORT char        *s1;                         /* General purpose text buffer */
IMPORT char        *s2;                         /* General purpose text buffer */
IMPORT char        *prgr_name;                  /* Stores the actual program name */
#ifdef GCAL_EPAGER
IMPORT char        *ext_pager;                  /* Name of external pager program */
#endif
IMPORT Bool         use_short3_day_name;        /* 3 char day name format specifier given in date format? */
IMPORT Bool         emu_hls;                    /* Must we emulate the highlighting sequences? */
IMPORT Bool         highlight_flag;             /* -H<yes> or -H<no> */



/*
*  Define local(static) variables.
*/
#if USE_DE
LOCAL const char *compiler_info[]={"Kompiliert mit %s%s f"UE"r %s%s%s%s"};   /* Compiler infotext */
#else /* !USE_DE */
LOCAL const char *compiler_info[]={N_("Compiled with %s%s for %s%s%s%s")};    /* Compiler infotext */
#endif /* !USE_DE */



   PUBLIC void
my_help_on_help (fp, longopt, lopt_ambig, cols)
         FILE *fp;
   const char *longopt;
   const Bool  lopt_ambig;
   const int   cols;
/*
   Prints the "help on help" text to file `fp' using the central output
     function `print_text()' and uses global text buffer `s1' internally.
     If delivered `lopt_ambig' is TRUE, only display the names of all
     longoptions stored in `lopt' structure member `long_name', which
     are equal to delivered `longopt'.
*/
{
   auto     const Lopt_struct  *ptr_lopt=lopt;
   register const int           filler_blanks=2;
   register const int           len_longopt=(int)strlen(longopt);
   register       int           len_line;
   register       int           len_string;


#if USE_DE
   sprintf(s1, "%s: Benutze `%s %s' mit einem dieser Argumente",
           prgr_name, prgr_name, get_longopt_description (SYM_LONG_HELP2, TRUE));
#else /* !USE_DE */
   sprintf(s1, _("%s: Use `%s %s' with one of these arguments"),
           prgr_name, prgr_name, get_longopt_description (SYM_LONG_HELP2, TRUE));
#endif /* !USE_DE */
   print_text (fp, s1);
   print_text (fp, s1);
   for (len_line=0 ; len_line < filler_blanks ; len_line++)
     s1[len_line] = ' ';
   s1[len_line] = '\0';
   if (lopt_ambig)
    {
      while (tolower(*ptr_lopt->long_name) < tolower(*longopt))
        ptr_lopt++;
      while (strncasecmp(longopt+1, ptr_lopt->long_name+1, len_longopt-1))
        ptr_lopt++;
    }
   while (ptr_lopt->long_name != NULL)
    {
      len_string = (int)strlen(ptr_lopt->long_name) + 1;
      if ((ptr_lopt+1)->long_name != NULL)
        len_string++;
      if (len_line + len_string >= cols)
       {
         print_text (fp, s1);
         for (len_line=0 ; len_line < filler_blanks ; len_line++)
           s1[len_line] = ' ';
         s1[len_line] = '\0';
       }
      strcat(s1, " ");
      strcat(s1, ptr_lopt->long_name);
      if ((ptr_lopt+1)->long_name != NULL)
       {
         if (   lopt_ambig
             && strncasecmp(longopt, (ptr_lopt+1)->long_name, len_longopt))
           break;
         strcat(s1, ",");
         len_line += len_string;
       }
      ptr_lopt++;
    }
   print_text (fp, s1);
}



   PUBLIC void
my_extended_help (fp, longopt_symbolic)
         FILE *fp;
   const int   longopt_symbolic;
/*
   Prints the "extended help" text to file `fp' using the central
     output function `print_text()' and uses global text buffer
     `s1' and `s2' internally.
     If `longopt_symbolic' is set to SYM_NIL, the complete text
     is shown, otherwise the text corresponding to SYM_??? only!
*/
{
#if USE_RC
   auto const Ed_struct    *ptr_info_exclusive_day=info_exclusive_day;
#endif
   auto const Cc_struct    *ptr_cc_holidays=cc_holidays;
   auto const Df_struct    *ptr_info_date_format=info_date_format;
   auto       Df_struct    *ptr_date_format=supported_date_format;
   auto       Greg_struct  *ptr_greg=greg_reform_date;
   auto       char         *ptr_env;
   auto       Bool          print_hls=(Bool)(is_tty&&highlight_flag&&!emu_hls);


   if (longopt_symbolic == SYM_NIL)
    {
      my_help_head_text (fp);
#if USE_RC
#  if USE_DE
      sprintf(s1, "Aufruf: %s [[OPTION...] [%cDATUM] [%cDATEI...]] [KOMMANDO]",
              prgr_name, RC_ADATE_CHAR, RSP_CHAR);
#  else /* !USE_DE */
      sprintf(s1, _("Usage:  %s [[OPTION...] [%cDATE] [%cFILE...]] [COMMAND]"),
              prgr_name, RC_ADATE_CHAR, RSP_CHAR);
#  endif /* !USE_DE */
#else /* !USE_RC */
#  if USE_DE
      sprintf(s1, "Aufruf: %s [[OPTION...] [%cDATEI...]] [KOMMANDO]", prgr_name, RSP_CHAR);
#  else /* !USE_DE */
      sprintf(s1, _("Usage:  %s [[OPTION...] [%cFILE...]] [COMMAND]"), prgr_name, RSP_CHAR);
#  endif /* !USE_DE */
#endif /* !USE_RC */
      print_text (fp, s1);
      print_text (fp, s1);
      print_text (fp, s1);
      if (print_hls)
        strcpy(s1, ehls2s.seq);
      strcat(s1, _("OPTION"));
      if (print_hls)
        strcat(s1, ehls2e.seq);
      print_text (fp, s1);
    }
#if USE_DE
   switch (longopt_symbolic)
    {
      case SYM_NIL:
      case SYM_HELP:
        sprintf(s1, "%s?        %s", SWITCH, get_longopt_description (SYM_HELP, TRUE));
        print_text (fp, s1);
        sprintf(s1, "%sh        %s", SWITCH, get_longopt_description (SYM_HELP, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          Hilfetext ausgeben und Programm beenden");
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_LONG_HELP1:
      case SYM_LONG_HELP2:
        sprintf(s1, "%s??       %s", SWITCH, get_longopt_description (SYM_LONG_HELP1, TRUE));
        print_text (fp, s1);
        sprintf(s1, "%shh       %s", SWITCH, get_longopt_description (SYM_LONG_HELP1, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          %s", get_longopt_description (SYM_LONG_HELP2, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          Erweiterten Hilfetext ausgeben und Programm beenden");
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_LICENSE1:
      case SYM_LICENSE2:
      case SYM_LICENSE3:
        sprintf(s1, "%sL        %s", SWITCH, get_longopt_description (SYM_LICENSE1, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          %s", get_longopt_description (SYM_LICENSE2, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          %s", get_longopt_description (SYM_LICENSE3, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          Softwarelizenz ausgeben und Programm beenden");
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_VERSION:
        sprintf(s1, "%sV        %s", SWITCH, get_longopt_description (SYM_VERSION, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          Versionsnummer ausgeben und Programm beenden");
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_RESPONSE_FILE:
        sprintf(s1, "%sR %-3s    %s", SWITCH, larg_lit, get_longopt_description (SYM_RESPONSE_FILE, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          Antwortdatei f%sr die `%cDATEI' Option erzeugen", UE, RSP_CHAR);
        print_text (fp, s1);
        sprintf(s1, "   %-3s    = Schreibe Argumente der Kommandozeile in Datei %s", larg_lit, larg_lit);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
#  ifdef GCAL_SHELL
      case SYM_SCRIPT_FILE:
        sprintf(s1, "%sS %-3s    %s", SWITCH, larg_lit, get_longopt_description (SYM_SCRIPT_FILE, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          Shell script mit den Argumenten der Kommandozeile erzeugen");
        print_text (fp, s1);
        sprintf(s1, "   %-3s    = Dateiname des shell scripts", larg_lit);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
#  endif
#  if USE_RC
      case SYM_DATE_VARIABLE:
        sprintf(s1, "%sv %-3s    %s", SWITCH, larg_lit, get_longopt_description (SYM_DATE_VARIABLE, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          Globale Datumvariable(n) \"dvar->a...d|f...s|u...|z\" definieren");
        print_text (fp, s1);
        sprintf(s1, "   %-3s    = Definitionen der Form \"dvar%s`%s%s'\" getrennt durch `%s' Zeichen",
                larg_lit, RC_DVAR_ASSIGN, mm_lit, dd_lit, SEP);
        print_text (fp, s1);
        sprintf(s1, "            z.B.  %sv a%s1127%sb%s054   Setze `a' auf 27 Nov und `b' auf 4 Mai",
                SWITCH, RC_DVAR_ASSIGN, SEP, RC_DVAR_ASSIGN);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_EXPORT_LOCAL_DVARS:
        sprintf(s1, "          %s", get_longopt_description (SYM_EXPORT_LOCAL_DVARS, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          Lokale Datumvariablen von Datei zu Datei exportieren");
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_TEXT_VARIABLE:
        sprintf(s1, "%sr %-3s    %s", SWITCH, larg_lit, get_longopt_description (SYM_TEXT_VARIABLE, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          Globale Textvariable(n) \"tvar->%ca...%cz\" definieren",
                RC_TVAR_CHAR, RC_TVAR_CHAR);
        print_text (fp, s1);
        sprintf(s1, "   %-3s    = Definitionen der Form \"tvar%s`text'\" getrennt durch `%s' Zeichen",
                larg_lit, RC_TVAR_ASSIGN, SEP);
        print_text (fp, s1);
        sprintf(s1, "            z.B.  %sr %ca%sfoo%s%cb%sbar   Setze `%ca' auf `foo' und `%cb' auf `bar'",
                SWITCH, RC_TVAR_CHAR, RC_TVAR_ASSIGN, SEP, RC_TVAR_CHAR, RC_TVAR_ASSIGN,
                RC_TVAR_CHAR, RC_TVAR_CHAR);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_EXPORT_LOCAL_TVARS:
        sprintf(s1, "          %s", get_longopt_description (SYM_EXPORT_LOCAL_TVARS, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          Lokale Textvariablen von Datei zu Datei exportieren");
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_FILTER_DAY:
        sprintf(s1, "%sD %-3s    %s", SWITCH, larg_lit, get_longopt_description (SYM_FILTER_DAY, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          Nur die Termine ausgeben, deren Datum %s nicht ausschlie%st", larg_lit, SZ);
        print_text (fp, s1);
        sprintf(s1, "   %-3s    = Eines oder mehrere der folgenden Zeichen.  Wenn Zeichen ein", larg_lit);
        print_text (fp, s1);
        sprintf(s1, "            KLEINBUCHSTABE ist, so bedeutet das einen `nicht'-Ausschlu%s!", SZ);
        print_text (fp, s1);
        do
         {
           sprintf(s1, "            %c = Ausschlu%s aller %s",
                   ptr_info_exclusive_day->id, SZ, ptr_info_exclusive_day->info);
           print_text (fp, s1);
         } while ((++ptr_info_exclusive_day)->info != (char *)NULL);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_FILTER_PERIOD:
        sprintf(s1, "%sP %-3s    %s", SWITCH, larg_lit, get_longopt_description (SYM_FILTER_PERIOD, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          Nur die Termine ausgeben, deren Datum %s nicht ausschlie%st", larg_lit, SZ);
        print_text (fp, s1);
        sprintf(s1, "   %-3s    = Liste von `%c[DATUM][%c[DATUM]]' und/oder `%c[DATUM][%c[DATUM]]'",
                larg_lit, RC_IDATE_CHAR, RC_DRANGE_CHAR, RC_EDATE_CHAR, RC_DRANGE_CHAR);
        print_text (fp, s1);
        sprintf(s1, "            Ausdr%scken, die durch `%s' Zeichen getrennt sind",
                UE, SPLIT_SEP);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_FILTER_TEXT:
        sprintf(s1, "%sI %-3s    %s", SWITCH, larg_lit, get_longopt_description (SYM_FILTER_TEXT, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          Nur die Termine ausgeben, in denen %s im Text enthalten ist", larg_lit);
        print_text (fp, s1);
#    if NO_REGEX
        sprintf(s1, "   %-3s    = Jeder beliebige Text, Metazeichen werden nicht ber%scksichtigt",
                larg_lit, UE);
#    else /* !NO_REGEX */
        sprintf(s1, "   %-3s    = Jeder beliebige Text bzw. \"regul%sre Ausdruck\"",
                larg_lit, AE);
#    endif /* !NO_REGEX */
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
#  endif /* USE_RC */
      case SYM_DEBUG:
        sprintf(s1, "          %s", get_longopt_description (SYM_DEBUG, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          Einige Debug-Informationen ausgeben");
        print_text (fp, s1);
        sprintf(s1, "  [%-3s]   = internal    Informationen bei Erreichen programminterner Maxima", larg_lit);
        print_text (fp, s1);
        strcpy(s1, "          = handled     Wie `internal' und zudem gehandhabte Dateinamen");
        print_text (fp, s1);
        strcpy(s1, "          = unhandled   Wie `internal' und zudem nicht gehandhabte Dateinamen");
        print_text (fp, s1);
        strcpy(s1, "          = all         Wie `handled' und `unhandled' zusammen");
        print_text (fp, s1);
        strcpy(s1, "          = abort       Wie `all' und Abbruch bei ungehandhabten Dateinamen");
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_TYPE_OF_CALENDAR:
        sprintf(s1, "%si[MOD]   %s", SWITCH, get_longopt_description (SYM_TYPE_OF_CALENDAR, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          Spezial-Kalenderformat f%sr Kalenderblatt verwenden", UE);
        print_text (fp, s1);
        sprintf(s1, "  [MOD]   = -   Standard-Kalenderformat f%sr Kalenderblatt verwenden", UE);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_ORTHODOX_CALENDAR:
        sprintf(s1, "%sO        %s", SWITCH, get_longopt_description (SYM_ORTHODOX_CALENDAR, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          Schaltjahrregel der %sstlichen orthodoxen Kirchen verwenden", OE);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_CALENDAR_WITH_WEEKNO:
        sprintf(s1, "%sK        %s", SWITCH, get_longopt_description (SYM_CALENDAR_WITH_WEEKNO, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          Kalenderblatt mit ISO-Wochennummern ausstatten");
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_SUPPRESS_CALENDAR:
        sprintf(s1, "%su        %s", SWITCH, get_longopt_description (SYM_SUPPRESS_CALENDAR, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          Ausgabe des Kalenderblatts explizit unterdr%scken", UE);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
#  if USE_PAGER
      case SYM_PAGER:
        sprintf(s1, "%sp        %s", SWITCH, get_longopt_description (SYM_PAGER, TRUE));
        print_text (fp, s1);
#    ifdef GCAL_EPAGER
        if (ext_pager != (char *)NULL)
          sprintf(s1, "          Ausgabe durch externen `%s' Paginierer lenken",
                  (*ext_pager == *DIR_SEP) ? strrchr(ext_pager, *DIR_SEP)+1 : ext_pager);
        else
          strcpy(s1, "          Ausgabe durch einfachen internen Paginierer lenken");
#    else /* !GCAL_EPAGER */
        strcpy(s1, "          Ausgabe durch einfachen internen Paginierer lenken");
#    endif /* !GCAL_EPAGER */
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
#  endif /* USE_PAGER */
      case SYM_DISABLE_HIGHLIGHTING:
        sprintf(s1, "%sH no     %s", SWITCH, get_longopt_description (SYM_DISABLE_HIGHLIGHTING, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          %s=no",  get_longopt_description (SYM_HIGHLIGHTING, FALSE));
        print_text (fp, s1);
        strcpy(s1, "          Text, Feiertag bzw. aktuelles Datum ohne Hervorhebung ausgeben");
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_FORCE_HIGHLIGHTING:
        sprintf(s1, "%sH yes    %s", SWITCH, get_longopt_description (SYM_FORCE_HIGHLIGHTING, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          %s=yes",  get_longopt_description (SYM_HIGHLIGHTING, FALSE));
        print_text (fp, s1);
        sprintf(s1, "          Bel%s%st Hervorhebungssequenzen in Ausgabe bei Ausgabeumlenkung", AE, SZ);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_HIGHLIGHTING:
        sprintf(s1, "%sH %-3s    %s", SWITCH, larg_lit, get_longopt_description (SYM_HIGHLIGHTING, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          Setze Hervorhebungssequenz-Paare 1 (=aktueller Tag) und 2 (=Feiertag)");
        print_text (fp, s1);
        sprintf(s1, "   %-3s    = Durch `%s' Zeichen getrennte Hervorhebungssequenz-Paare",
                larg_lit, SEP);
        print_text (fp, s1);
        sprintf(s1, "            z.B.  %sH \\x2%s\\xAE   Benutze Hex-Wert 2 und AE f%sr Sequenz 1",
                SWITCH, SEP, UE);
        print_text (fp, s1);
        sprintf(s1, "            z.B.  %sH %s%s*%s*      Benutze Zeichen `*' und `*' f%sr Sequenz 2",
                SWITCH, SEP, SEP, SEP, UE);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_BLOCKS:
        sprintf(s1, "%sb %-3s    %s", SWITCH, larg_lit, get_longopt_description (SYM_BLOCKS, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          Format des Jahreskalenders festlegen");
        print_text (fp, s1);
        sprintf(s1, "   %-3s    = 1|2|3|4|6|12   Anzahl der Bl%scke", larg_lit, OE);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_CALENDAR_DATES:
        sprintf(s1, "%sj[MOD]   %s", SWITCH, get_longopt_description (SYM_CALENDAR_DATES, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          Fortlaufenden Tag des Jahres im Kalenderblatt verwenden");
        print_text (fp, s1);
        strcpy(s1, "  [MOD]   = b   Beide Datumnotationen (Tag des Monats+Jahres) verwenden");
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_HOLIDAY_DATES:
        sprintf(s1, "%sjn[MOD]  %s", SWITCH, get_longopt_description (SYM_HOLIDAY_DATES, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          Fortlaufenden Tag des Jahres in ewiger Feiertagsliste verwenden");
        print_text (fp, s1);
        strcpy(s1, "   [MOD]  = b   Beide Datumnotationen (Tag des Monats+Jahres) verwenden");
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
#if USE_RC
      case SYM_FIXED_DATES:
        sprintf(s1, "%sjc[MOD]  %s", SWITCH, get_longopt_description (SYM_FIXED_DATES, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          Fortlaufenden Tag des Jahres in Terminliste verwenden");
        print_text (fp, s1);
        strcpy(s1, "   [MOD]  = b   Beide Datumnotationen (Tag des Monats+Jahres) verwenden");
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
#endif
      case SYM_STARTING_DAY:
        sprintf(s1, "%ss %-3s    %s", SWITCH, larg_lit, get_longopt_description (SYM_STARTING_DAY, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          Starttag der Woche festlegen");
        print_text (fp, s1);
        if (use_short3_day_name)
          sprintf(s1, "   %-3s    = 0|today | %d|%s | %d|%s | ... | %d|%s",
                  larg_lit, DAY_MIN, short3_day_name (DAY_MIN),
                  DAY_MIN+1, short3_day_name (DAY_MIN+1),
                  DAY_MAX, short3_day_name (DAY_MAX));
        else
          sprintf(s1, "   %-3s    = 0|today | %d|%s | %d|%s | ... | %d|%s",
                  larg_lit, DAY_MIN, short_day_name (DAY_MIN),
                  DAY_MIN+1, short_day_name (DAY_MIN+1),
                  DAY_MAX, short_day_name (DAY_MAX));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
#  ifdef GCAL_EMAIL
      case SYM_MAIL:
        sprintf(s1, "          %s", get_longopt_description (SYM_MAIL, TRUE));
        print_text (fp, s1);
        ptr_env = MAIL_PRGR;
#    if !defined(AMIGA) || defined(__GNUC__)
        ptr_env = getenv(ENV_VAR_MAILPROG);
        if (ptr_env == (char *)NULL)
          ptr_env = MAIL_PRGR;
        else
          if (!*ptr_env)
            ptr_env = MAIL_PRGR;
#    endif /* AMIGA && !__GNUC__ */
        sprintf(s1, "          Versenden der Ausgabe via `%s' Programm an Benutzer", ptr_env);
        print_text (fp, s1);
        sprintf(s1, "   %-3s    = eMail Adresse", larg_lit);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
#  endif /* GCAL_EMAIL */
      case SYM_HOLIDAY_LIST:
      case SYM_DESC_HOLIDAY_LIST:
        sprintf(s1, "%sn|N[MOD] %s", SWITCH, get_longopt_description (SYM_HOLIDAY_LIST, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          %s", get_longopt_description (SYM_DESC_HOLIDAY_LIST, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          Ewige Feiertagsliste ausgeben (Jahr im Bereich: %d...%d)",
                EASTER_MIN, EASTER_MAX);
        print_text (fp, s1);
        strcpy(s1, " n        = Gesetzliche Feiertage und Gedenktage");
        print_text (fp, s1);
        strcpy(s1, "   N      = Nur gesetzliche Feiertage");
        print_text (fp, s1);
        sprintf(s1, "    [MOD] = %s   Absteigend sortiert", DES_LIT);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_EXCLUDE_HD_TITLE:
        sprintf(s1, "%sX        %s", SWITCH, get_longopt_description (SYM_EXCLUDE_HD_TITLE, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          Ewige Feiertagsliste ohne %sberschrift ausgeben", UUE);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_STANDARD_HDY:
        sprintf(s1, "          %s", get_longopt_description (SYM_STANDARD_HDY, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          Ewige Feiertagsliste ohne Standardfeiertage ausstatten");
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_CHRISTIAN_HDY:
        sprintf(s1, "          %s", get_longopt_description (SYM_CHRISTIAN_HDY, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          Ewige Feiertagsliste mit christlichen Feiertagen ausstatten");
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_CC_HDY:
        sprintf(s1, "%sq %-3s    %s", SWITCH, larg_lit, get_longopt_description (SYM_CC_HDY, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          Ewige Feiertagsliste mit landesspezifischen Feiertagen ausstatten");
        print_text (fp, s1);
        sprintf(s1, "   %-3s    = Einzelner Landeskode oder Liste von Landeskodes,", larg_lit);
        print_text (fp, s1);
        sprintf(s1, "            die durch `%s' Zeichen verbunden werden", CONNECT_SEP);
        print_text (fp, s1);
        do
         {
           sprintf(s1, "            %s = %s Feiertage", ptr_cc_holidays->id, ptr_cc_holidays->info);
           print_text (fp, s1);
         } while ((++ptr_cc_holidays)->info != (char *)NULL);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_GREG_REFORM:
        sprintf(s1, "          %s", get_longopt_description (SYM_GREG_REFORM, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          Setze Zeitraum der Gregorianischen Reformation");
        print_text (fp, s1);
        sprintf(s1, "          Aktuell ber%scksichtigter Zeitraum:  %02d-%02d %s %0*d", UE,
                greg->first_day, greg->last_day, short_month_name (greg->month),
                len_year_max, greg->year);
        print_text (fp, s1);
        do
         {
           sprintf(s1, "   %-*d%*s = Setze Zeitraum auf:  %02d-%02d %s %0*d",
                   len_year_max, ptr_greg->year, 6-len_year_max, "",
                   ptr_greg->first_day, ptr_greg->last_day,
                   short_month_name (ptr_greg->month), len_year_max, ptr_greg->year);
           print_text (fp, s1);
         } while ((++ptr_greg)->year);
        sprintf(s1, "   %-3s    = %s%s%s%s%s%s%s   Setze Zeitraum explizit",
                larg_lit, yyyy_lit, SPLIT_SEP, mm_lit, SPLIT_SEP, dd_lit, SPLIT_SEP, dd_lit);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_DATE_FORMAT:
        sprintf(s1, "          %s", get_longopt_description (SYM_DATE_FORMAT, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          Anordnung der Datumelemente mit dem Formattext %s festlegen", larg_lit);
        print_text (fp, s1);
        sprintf(s1, "          Aktuelles Format:  (%s) `%s' (%s)",
                (date_format->id==(char *)NULL) ? "Selbstdefiniert" : date_format->id,
                date_format->format, date_format->info);
        print_text (fp, s1);
        do
         {
           sprintf(s1, "   %-6s = Setze Format auf:  `%s' (%s)", ptr_date_format->id,
                   ptr_date_format->format, ptr_date_format->info);
           print_text (fp, s1);
         } while ((++ptr_date_format)->info != (char *)NULL);
        sprintf(s1, "   %-3s    = Eigenes Format definieren. Ber%scksichtigte Formatelemente sind:",
                larg_lit, UE);
        print_text (fp, s1);
        do
         {
           if (*ptr_info_date_format->format == '\0')
             sprintf(s1, "               %s", ptr_info_date_format->info);
           else
             sprintf(s1, "            %-2s = %s", ptr_info_date_format->format, ptr_info_date_format->info);
           print_text (fp, s1);
         } while ((++ptr_info_date_format)->info != (char *)NULL);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_EXIT_STAT_HELP_127:
        sprintf(s1, "          %s", get_longopt_description (SYM_EXIT_STAT_HELP_127, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          Setze Programm EXIT Status auf 127 bei --help, --version etc.");
        print_text (fp, s1);
      default:
        ;   /* Void */
    }
#  if USE_RC
   if (longopt_symbolic == SYM_NIL)
    {
      print_text (fp, s1);
      sprintf(s1, "+++ TERMINE +++");
      print_text (fp, s1);
      print_text (fp, s1);
    }
   switch (longopt_symbolic)
    {
      case SYM_NIL:
      case SYM_TODAY:
      case SYM_FIXED_DATES_LIST:
      case SYM_DESC_FIXED_DATES_LIST:
        sprintf(s1, "%sc|C[MOD] %s", SWITCH, get_longopt_description (SYM_FIXED_DATES_LIST, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          %s", get_longopt_description (SYM_DESC_FIXED_DATES_LIST, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          %s", get_longopt_description (SYM_TODAY, TRUE));
        print_text (fp, s1);
#    ifdef GCAL_SHELL
        sprintf(s1, "          Benutze Standard-Ressourcendatei `.%s%s' f%sr Terminliste",
                PRGR_NAME, RC_SUFFIX, UE);
#    else /* !GCAL_SHELL */
        sprintf(s1, "          Benutze Standard-Ressourcendatei `%s%s' f%sr Terminliste",
                PRGR_NAME, RC_SUFFIX, UE);
#    endif /* !GCAL_SHELL */
        print_text (fp, s1);
        strcpy(s1, "          Impliziert Zeitraum:  Heute");
        print_text (fp, s1);
        strcpy(s1, " c        = Nur die Daten ausgeben, zu denen Termine vorliegen");
        print_text (fp, s1);
        strcpy(s1, "   C      = Auch die Daten ausgeben, zu denen keine Termine vorliegen");
        print_text (fp, s1);
        sprintf(s1, "    [MOD] = %s   Absteigend sortiert", DES_LIT);
        print_text (fp, s1);
      default:
        ;   /* Void */
    }
   if (longopt_symbolic == SYM_NIL)
    {
      print_text (fp, s1);
      strcpy(s1, "    [MOD] = Einer oder mehr der folgenden Modifizierer; hier markiert mit #");
      print_text (fp, s1);
      print_text (fp, s1);
    }
   switch (longopt_symbolic)
    {
      case SYM_NIL:
      case SYM_GROUPING_TEXT:
        sprintf(s1, "          %s", get_longopt_description (SYM_GROUPING_TEXT, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          # g[MOD]       = Benutze Trenntext MOD f%sr tageweise Gruppierung", UE);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      default:
        ;   /* Void */
    }
   if (longopt_symbolic == SYM_NIL)
    {
      print_text (fp, s1);
      strcpy(s1, "          1. Darstellung des Textes");
      print_text (fp, s1);
    }
   switch (longopt_symbolic)
    {
      case SYM_NIL:
      case SYM_BIORHYTHM_AXIS_LEN:
        sprintf(s1, "          %s", get_longopt_description (SYM_BIORHYTHM_AXIS_LEN, TRUE));
        print_text (fp, s1);
        strcpy(s1, "                         = Breite der Biorhythmus-Textgrafik festlegen");
        print_text (fp, s1);
        sprintf(s1, "                           %-3s = %d...%d   L%snge einer einzelnen Achse",
                larg_lit, BIO_AXIS_MIN, BIO_AXIS_MAX, AE);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_MOONIMAGE_LINES:
        sprintf(s1, "          %s", get_longopt_description (SYM_MOONIMAGE_LINES, TRUE));
        print_text (fp, s1);
        sprintf(s1, "                         = H%she der Mondphasen-Textgrafik festlegen", OE);
        print_text (fp, s1);
        sprintf(s1, "                           %-3s = %d...%d   Gesamtanzahl der Zeilen",
                larg_lit, MOONIMAGE_MIN, MOONIMAGE_MAX);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_INCLUDE_FILENAME:
        sprintf(s1, "          %s", get_longopt_description (SYM_INCLUDE_FILENAME, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          # a            = Herkunft des Termins ausgeben");
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_ALTERNATIVE_FORMAT:
        sprintf(s1, "          %s", get_longopt_description (SYM_ALTERNATIVE_FORMAT, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          # A            = Alternatives Listenformat verwenden");
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_BYPASS_SHELL_CMD:
        sprintf(s1, "          %s", get_longopt_description (SYM_BYPASS_SHELL_CMD, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          # B            = `%c%c[%s]' Shell-Kommandos nicht ausf%shren",
                RC_SPECIAL_TEXT_CHAR, RC_SHELL_ESC_CHAR, larg_lit, UE);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_INCLUDE_HOLIDAY:
        sprintf(s1, "          %s", get_longopt_description (SYM_INCLUDE_HOLIDAY, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          # e            = Gesetzliche Feiertage und Gedenktage einf%sgen", UE);
        print_text (fp, s1);
        sprintf(s1, "          # E            = Nur gesetzliche Feiertage einf%sgen", UE);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_INCLUDE_WEEK_NO:
        sprintf(s1, "          %s", get_longopt_description (SYM_INCLUDE_WEEK_NO, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          # k            = ISO-Wochennummer ausgeben");
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_OMIT_DATE_PART:
        sprintf(s1, "          %s", get_longopt_description (SYM_OMIT_DATE_PART, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          # o            = Datumfeld nur einmal ausgeben");
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_SUPPRESS_DATE_PART:
        sprintf(s1, "          %s", get_longopt_description (SYM_SUPPRESS_DATE_PART, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          # U            = Datumfeld nicht ausgeben");
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_EXCLUDE_RC_TITLE:
        sprintf(s1, "          %s", get_longopt_description (SYM_EXCLUDE_RC_TITLE, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          # x            = Terminliste ohne %sberschrift ausgeben", UUE);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_INCLUDE_CONS_NO:
        sprintf(s1, "          %s", get_longopt_description (SYM_INCLUDE_CONS_NO, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          # z            = Fortlaufende Postennummer ausgeben");
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_ZERO_DATES:
        sprintf(s1, "          %s", get_longopt_description (SYM_ZERO_DATES, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          # Z            = Nur Daten ausgeben, f%sr die keine Termine vorliegen", UE);
        print_text (fp, s1);
      default:
        ;   /* Void */
    }
   if (longopt_symbolic == SYM_NIL)
    {
      print_text (fp, s1);
      sprintf(s1, "          2. Ber%scksichtigter Zeitraum", UE);
      print_text (fp, s1);
    }
   switch (longopt_symbolic)
    {
      case SYM_NIL:
      case SYM_LEAP_DAY:
        sprintf(s1, "          %s", get_longopt_description (SYM_LEAP_DAY, TRUE));
        print_text (fp, s1);
        strcpy(s1, "                         = Den `29-FEBRUAR' in Nicht-Schaltjahren handhaben");
        print_text (fp, s1);
        /*
           *** Translators, please don't translate the word `february',
           *** because it is used textually AS IS as an option argument.
        */
        sprintf(s1, "                           %-3s = february   Am `28-FEBRUAR' ber%scksichtigen",
                larg_lit, UE);
        print_text (fp, s1);
        /*
           *** Translators, please don't translate the word `march',
           *** because it is used textually AS IS as an option argument.
        */
        sprintf(s1, "                           %-3s = march      Am `01-M%sRZ' ber%scksichtigen",
                larg_lit, AAE, UE);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_INCLUDE_TODAY:
        sprintf(s1, "          %s", get_longopt_description (SYM_INCLUDE_TODAY, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          # d            = Aktuelles Datum einschliessen bei Datumliste");
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_LIST_MODE:
        sprintf(s1, "          %s", get_longopt_description (SYM_LIST_MODE, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          # l            = Datumliste anstelle Einzeldatum");
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      default:
        ;   /* Void */
    }
   switch (longopt_symbolic)
    {
      case SYM_NIL:
      case SYM_PERIOD:
        sprintf(s1, "          %s", get_longopt_description (SYM_PERIOD, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          # Nd           = Einzelner absoluter Tag N");
        print_text (fp, s1);
        sprintf(s1, "          # %cdN          = Einzelner absoluter Tag N", RC_NWD_CHAR);
        print_text (fp, s1);
        print_text (fp, s1);
        strcpy(s1, "          # NMOD         = Einzelner Tag N relativ zu heute");
        print_text (fp, s1);
        sprintf(s1, "             MOD           = %s   Vorw%srts", ASC_LIT, AE);
        print_text (fp, s1);
        sprintf(s1, "                           = %s   R%sckw%srts", DES_LIT, UE, AE);
        print_text (fp, s1);
        print_text (fp, s1);
        strcpy(s1, "          # Nw           = Gesamte Woche N");
        print_text (fp, s1);
        strcpy(s1, "                           N = 0        1'te Woche / letzte Woche im Vorjahr");
        print_text (fp, s1);
        strcpy(s1, "                           N = 1...52   1'te...52'te Woche (stets)");
        print_text (fp, s1);
        strcpy(s1, "                           N = 53       53'te Woche (manchmal)");
        print_text (fp, s1);
        strcpy(s1, "                           N = 99       Letzte Woche");
        print_text (fp, s1);
        print_text (fp, s1);
        sprintf(s1, "          # `%s%s'       = Einzelner Tag `%s' im Monat `%s'",
                mm_lit, dd_lit, dd_lit, mm_lit);
        print_text (fp, s1);
        print_text (fp, s1);
        sprintf(s1, "          # `%s%s'N     = Einzelner N'ter Wochentag `%s' im Monat `%s'",
                mm_lit, www_lit, www_lit, mm_lit);
        print_text (fp, s1);
        sprintf(s1, "                           N = 1...4   1'ter...4'ter Wochentag `%s' (stets)", www_lit);
        print_text (fp, s1);
        sprintf(s1, "                           N = 5       5'ter Wochentag `%s' (manchmal)", www_lit);
        print_text (fp, s1);
        sprintf(s1, "                           N = 9       Letzter Wochentag `%s'", www_lit);
        print_text (fp, s1);
        print_text (fp, s1);
        sprintf(s1, "          # %cdN`%s'     = Einzelner N'ter Wochentag `%s'",
                RC_NWD_CHAR, www_lit, www_lit);
        print_text (fp, s1);
        sprintf(s1, "                           N = 1...51   1'ter...51'ter Wochentag `%s' (stets)", www_lit);
        print_text (fp, s1);
        sprintf(s1, "                           N = 52|53    52|53'ter Wochentag `%s' (manchmal)", www_lit);
        print_text (fp, s1);
        sprintf(s1, "                           N = 99       Letzter Wochentag `%s'", www_lit);
        print_text (fp, s1);
        print_text (fp, s1);
        sprintf(s1, "          # %cwN`%s'     = Einzelner Wochentag `%s' der N'ten Woche",
                RC_NWD_CHAR, www_lit, www_lit);
        print_text (fp, s1);
        sprintf(s1, "                           N = 0        `%s' der nicht in der 1'ten Woche ist", www_lit);
        print_text (fp, s1);
        sprintf(s1, "                           N = 1...51   `%s' der 1'ten...51'ten Woche (stets)", www_lit);
        print_text (fp, s1);
        sprintf(s1, "                           N = 52|53    `%s' der 52|53'ten Woche (manchmal)", www_lit);
        print_text (fp, s1);
        sprintf(s1, "                           N = 99       `%s' der letzten Woche", www_lit);
        print_text (fp, s1);
        print_text (fp, s1);
        sprintf(s1, "          # %c%c[[%s|%s]N]   = Einzelner Tag N relativ zu Ostersonntag",
                RC_HDY_CHAR, RC_EASTER_CHAR, ASC_LIT, DES_LIT);
        print_text (fp, s1);
        print_text (fp, s1);
        sprintf(s1, "          # %c%c[%s|%s]N`%s'= Einzelner N'ter Wochentag relativ zu Ostersonntag",
                RC_HDY_CHAR, RC_EASTER_CHAR, ASC_LIT, DES_LIT, www_lit);
        print_text (fp, s1);
        print_text (fp, s1);
        sprintf(s1, "          # %c%c[[%s|%s]N]   = Einzelner Tag N relativ zu heute",
                RC_HDY_CHAR, RC_TODAY_CHAR, ASC_LIT, DES_LIT);
        print_text (fp, s1);
        print_text (fp, s1);
        sprintf(s1, "          # %c%c[%s|%s]N`%s'= Einzelner N'ter Wochentag relativ zu heute",
                RC_HDY_CHAR, RC_TODAY_CHAR, ASC_LIT, DES_LIT, www_lit);
        print_text (fp, s1);
        print_text (fp, s1);
        sprintf(s1, "          # %c?[[%s|%s]N]   = Einzelner Tag N relativ zur Datumvariablen",
                RC_HDY_CHAR, ASC_LIT, DES_LIT);
        print_text (fp, s1);
        print_text (fp, s1);
        sprintf(s1, "          # %c?[%s|%s]N`%s'= Einzelner N'ter Wochentag relativ zur Datumvariablen",
                RC_HDY_CHAR, ASC_LIT, DES_LIT, www_lit);
        print_text (fp, s1);
        print_text (fp, s1);
      case SYM_TOMORROW:
        sprintf(s1, "          %s", get_longopt_description (SYM_TOMORROW, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          # t|T          = Datumliste von morgen");
        print_text (fp, s1);
        if (longopt_symbolic == SYM_TOMORROW)
          break;
        print_text (fp, s1);
      case SYM_WEEK:
      case SYM_END_OF_WEEK:
      case SYM_START_OF_WEEK:
        sprintf(s1, "          %s", get_longopt_description (SYM_WEEK, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          %s", get_longopt_description (SYM_END_OF_WEEK, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          %s", get_longopt_description (SYM_START_OF_WEEK, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          # w|W[MOD]     = Datumliste der gesamten Woche");
        print_text (fp, s1);
        sprintf(s1, "               [MOD]       = %s   Liste von morgen bis Ende der Woche",
                ASC_LIT);
        print_text (fp, s1);
        sprintf(s1, "                           = %s   Liste von gestern bis Anfang der Woche",
                DES_LIT);
        print_text (fp, s1);
        if (   longopt_symbolic == SYM_WEEK
            || longopt_symbolic == SYM_END_OF_WEEK
            || longopt_symbolic == SYM_START_OF_WEEK)
          break;
        print_text (fp, s1);
      case SYM_MONTH:
      case SYM_END_OF_MONTH:
      case SYM_START_OF_MONTH:
        sprintf(s1, "          %s", get_longopt_description (SYM_MONTH, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          %s", get_longopt_description (SYM_END_OF_MONTH, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          %s", get_longopt_description (SYM_START_OF_MONTH, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          # m|M[MOD]     = Datumliste des gesamten Monats");
        print_text (fp, s1);
        sprintf(s1, "               [MOD]       = %s   Liste von morgen bis Ende des Monats",
                ASC_LIT);
        print_text (fp, s1);
        sprintf(s1, "                           = %s   Liste von gestern bis Anfang der Monats",
                DES_LIT);
        print_text (fp, s1);
        if (   longopt_symbolic == SYM_MONTH
            || longopt_symbolic == SYM_END_OF_MONTH
            || longopt_symbolic == SYM_START_OF_MONTH)
          break;
        print_text (fp, s1);
      case SYM_YEAR:
      case SYM_END_OF_YEAR:
      case SYM_START_OF_YEAR:
        sprintf(s1, "          %s", get_longopt_description (SYM_YEAR, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          %s", get_longopt_description (SYM_END_OF_YEAR, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          %s", get_longopt_description (SYM_START_OF_YEAR, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          # y|Y[MOD]     = Datumliste des gesamten Jahres");
        print_text (fp, s1);
        sprintf(s1, "               [MOD]       = %s   Liste von morgen bis Ende des Jahres",
                ASC_LIT);
        print_text (fp, s1);
        sprintf(s1, "                           = %s   Liste von gestern bis Anfang des Jahres",
                DES_LIT);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_RESOURCE_FILE:
        sprintf(s1, "%sf|F %-3s  %s", SWITCH, larg_lit, get_longopt_description (SYM_RESOURCE_FILE, TRUE));
        print_text (fp, s1);
        strcpy(s1, "          Benutze alternative Datei(en) anstelle Standard-Ressourcendatei");
        print_text (fp, s1);
        strcpy(s1, "          Impliziert Zeitraum:  Heute");
        print_text (fp, s1);
        strcpy(s1, " f        = Nur die Daten ausgeben, zu denen Termine vorliegen");
        print_text (fp, s1);
        strcpy(s1, "   F      = Auch die Daten ausgeben, zu denen keine Termine vorliegen");
        print_text (fp, s1);
        sprintf(s1, "     %-3s  = Einzelne Datei oder durch `%s' Zeichen verbundene Liste von Dateien",
                larg_lit, CONNECT_SEP);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_HERE_FILE:
        sprintf(s1, "%s# %-3s    %s", SWITCH, larg_lit, get_longopt_description (SYM_HERE_FILE, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          Zus%stzliche Ressourcendateizeile definieren", AE);
        print_text (fp, s1);
        sprintf(s1, "   %-3s    = Jede m%sgliche Zeile einer Ressourcendatei", larg_lit, OE);
        print_text (fp, s1);
      default:
          ;   /* Void */
    }
   if (longopt_symbolic == SYM_NIL)
    {
      print_text (fp, s1);
      print_text (fp, s1);
      if (print_hls)
        strcpy(s1, ehls2s.seq);
      sprintf(s2, "%cDATUM", RC_ADATE_CHAR);
      strcat(s1, s2);
      if (print_hls)
        strcat(s1, ehls2e.seq);
      print_text (fp, s1);
      strcpy(s1, "Vorgegebenes `DATUM' anstelle heutiges Datum verwenden");
      print_text (fp, s1);
      sprintf(s1, "Datumformat:  %s[%s[%s|%s[N]]], %s%c%c|%c|dvar[[%s|%s]N[%s]], %s%cdN[%s]",
              yyyy_lit, mm_lit, dd_lit, www_lit, yyyy_lit, RC_HDY_CHAR, RC_EASTER_CHAR, RC_TODAY_CHAR,
              ASC_LIT, DES_LIT, www_lit, yyyy_lit, RC_NWD_CHAR, www_lit);
      print_text (fp, s1);
      sprintf(s1, "              %s%cwN[%s], Monatsname[%s], Wochentagsname[N] oder %s",
              yyyy_lit, RC_NWD_CHAR, www_lit, dd_lit, dd_lit);
      print_text (fp, s1);
    }
#  endif /* USE_RC */
   if (longopt_symbolic == SYM_NIL)
    {
      print_text (fp, s1);
      print_text (fp, s1);
      if (print_hls)
        strcpy(s1, ehls2s.seq);
      sprintf(s2, "%cDATEI", RSP_CHAR);
      strcat(s1, s2);
      if (print_hls)
        strcat(s1, ehls2e.seq);
      print_text (fp, s1);
      sprintf(s1, "L%sdt Optionen und Kommandos aus `DATEI' vor", AE);
      print_text (fp, s1);
      print_text (fp, s1);
      print_text (fp, s1);
      if (print_hls)
        strcpy(s1, ehls2s.seq);
      strcat(s1, "KOMMANDO");
      if (print_hls)
        strcat(s1, ehls2e.seq);
      print_text (fp, s1);
      sprintf(s1, "%s       = Monat im Bereich:  %d...%d", mm_lit, MONTH_MIN, MONTH_MAX);
      print_text (fp, s1);
      sprintf(s1, "                       oder:  Monatsname | %s | %s%s | %s%s | %s%s",
              MONTH3_LIT, MONTH3_LIT, MONTH3_LIT, MONTH3_LIT, ASC_LIT, MONTH3_LIT, DES_LIT);
      print_text (fp, s1);
      print_text (fp, s1);
      sprintf(s1, "           Liste:    %s[%s%s]%s...%s%s[%s%s]",
              mm_lit, YEAR_SEP, yyyy_lit, MLIST_SEP, MLIST_SEP, mm_lit, YEAR_SEP, yyyy_lit);
      print_text (fp, s1);
      sprintf(s1, "           Bereich:  %s[%s%s]%s%s[%s%s] | %s%s%s%s%s | %s %s%s%s",
              mm_lit, YEAR_SEP, yyyy_lit, MRANGE_SEP, mm_lit, YEAR_SEP, yyyy_lit,
              mm_lit, YEAR_SEP, yyyy_lit, MRANGE_SEP, yyyy_lit,
              mm_lit, yyyy_lit, MRANGE_SEP, yyyy_lit);
      print_text (fp, s1);
      print_text (fp, s1);
      sprintf(s1, "%s%*s   = Jahr  im Bereich:  %d...%d",
              yyyy_lit, 6-len_year_max, "", YEAR_MIN, YEAR_MAX);
      print_text (fp, s1);
      print_text (fp, s1);
      sprintf(s1, "           Liste:    [%s%s]%s%s...%s[%s%s]%s",
              mm_lit, FYEAR_SEP, yyyy_lit, YLIST_SEP, YLIST_SEP, mm_lit, FYEAR_SEP, yyyy_lit);
      print_text (fp, s1);
      sprintf(s1, "           Bereich:  [%s%s]%s%s[%s%s]%s",
              mm_lit, FYEAR_SEP, yyyy_lit, YRANGE_SEP, mm_lit, FYEAR_SEP, yyyy_lit);
    }
#else /* !USE_DE */
   switch (longopt_symbolic)
    {
      case SYM_NIL:
      case SYM_HELP:
        sprintf(s1, "%s?        %s", SWITCH, get_longopt_description (SYM_HELP, TRUE));
        print_text (fp, s1);
        sprintf(s1, "%sh        %s", SWITCH, get_longopt_description (SYM_HELP, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Display help text and quit program"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_LONG_HELP1:
      case SYM_LONG_HELP2:
        sprintf(s1, "%s??       %s", SWITCH, get_longopt_description (SYM_LONG_HELP1, TRUE));
        print_text (fp, s1);
        sprintf(s1, "%shh       %s", SWITCH, get_longopt_description (SYM_LONG_HELP1, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          %s", get_longopt_description (SYM_LONG_HELP2, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Display extended help text and quit program"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_LICENSE1:
      case SYM_LICENSE2:
      case SYM_LICENSE3:
        sprintf(s1, "%sL        %s", SWITCH, get_longopt_description (SYM_LICENSE1, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          %s", get_longopt_description (SYM_LICENSE2, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          %s", get_longopt_description (SYM_LICENSE3, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Display software license and quit program"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_VERSION:
        sprintf(s1, "%sV        %s", SWITCH, get_longopt_description (SYM_VERSION, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Display version number and quit program"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_RESPONSE_FILE:
        sprintf(s1, "%sR %-3s    %s", SWITCH, larg_lit, get_longopt_description (SYM_RESPONSE_FILE, TRUE));
        print_text (fp, s1);
        sprintf(s1, _("          Create response file for the `%cFILE' option"), RSP_CHAR);
        print_text (fp, s1);
        sprintf(s1, _("   %-3s    = Store arguments of command line in file %s"), larg_lit, larg_lit);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
#  ifdef GCAL_SHELL
      case SYM_SCRIPT_FILE:
        sprintf(s1, "%sS %-3s    %s", SWITCH, larg_lit, get_longopt_description (SYM_SCRIPT_FILE, TRUE));
        print_text (fp, s1);
        sprintf(s1, _("          Create shell script which contains the arguments of command line"));
        print_text (fp, s1);
        sprintf(s1, _("   %-3s    = File name of the shell script"), larg_lit);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
#  endif
#  if USE_RC
      case SYM_DATE_VARIABLE:
        sprintf(s1, "%sv %-3s    %s", SWITCH, larg_lit, get_longopt_description (SYM_DATE_VARIABLE, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Define global date variable(s) \"dvar->a...d|f...s|u...|z\""));
        print_text (fp, s1);
        sprintf(s1, _("   %-3s    = Definition of \"dvar%s`%s%s'\" separated by `%s' characters"),
                larg_lit, RC_DVAR_ASSIGN, mm_lit, dd_lit, SEP);
        print_text (fp, s1);
        sprintf(s1, _("            e.g.  %sv a%s1127%sb%s054   Set `a' to Nov 27 and `b' to May 4"),
                SWITCH, RC_DVAR_ASSIGN, SEP, RC_DVAR_ASSIGN);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_EXPORT_LOCAL_DVARS:
        sprintf(s1, "          %s", get_longopt_description (SYM_EXPORT_LOCAL_DVARS, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Export local date variables from file to file"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_TEXT_VARIABLE:
        sprintf(s1, "%sr %-3s    %s", SWITCH, larg_lit, get_longopt_description (SYM_TEXT_VARIABLE, TRUE));
        print_text (fp, s1);
        sprintf(s1, _("          Define global text variable(s) \"tvar->%ca...%cz\""),
                RC_TVAR_CHAR, RC_TVAR_CHAR);
        print_text (fp, s1);
        sprintf(s1, _("   %-3s    = Definition of \"tvar%s`text'\" separated by `%s' characters"),
                larg_lit, RC_TVAR_ASSIGN, SEP);
        print_text (fp, s1);
        sprintf(s1, _("            e.g.  %sr %ca%sfoo%s%cb%sbar   Set `%ca' to `foo' and `%cb' to `bar'"),
                SWITCH, RC_TVAR_CHAR, RC_TVAR_ASSIGN, SEP, RC_TVAR_CHAR, RC_TVAR_ASSIGN,
                RC_TVAR_CHAR, RC_TVAR_CHAR);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_EXPORT_LOCAL_TVARS:
        sprintf(s1, "          %s", get_longopt_description (SYM_EXPORT_LOCAL_TVARS, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Export local text variables from file to file"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_FILTER_DAY:
        sprintf(s1, "%sD %-3s    %s", SWITCH, larg_lit, get_longopt_description (SYM_FILTER_DAY, TRUE));
        print_text (fp, s1);
        sprintf(s1, _("          Display only those fixed dates, whose date is not excluded by %s"), larg_lit);
        print_text (fp, s1);
        sprintf(s1, _("   %-3s    = One or more of the following characters.  If character"), larg_lit);
        print_text (fp, s1);
        strcpy(s1, _("            is a LOWER CASE LETTER, it means a `non'-exclusion!"));
        print_text (fp, s1);
        do
         {
           sprintf(s1, _("            %c = Exclusion of all %s"),
                   ptr_info_exclusive_day->id, _(ptr_info_exclusive_day->info));
           print_text (fp, s1);
         } while ((++ptr_info_exclusive_day)->info != (char *)NULL);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_FILTER_PERIOD:
        sprintf(s1, "%sP %-3s    %s", SWITCH, larg_lit, get_longopt_description (SYM_FILTER_PERIOD, TRUE));
        print_text (fp, s1);
        sprintf(s1, _("          Display only those fixed dates, whose date is not excluded by %s"), larg_lit);
        print_text (fp, s1);
        sprintf(s1, _("   %-3s    = List of `%c[DATE][%c[DATE]]' and/or `%c[DATE][%c[DATE]]'"),
                larg_lit, RC_IDATE_CHAR, RC_DRANGE_CHAR, RC_EDATE_CHAR, RC_DRANGE_CHAR);
        print_text (fp, s1);
        sprintf(s1, _("            expressions separated by `%s' characters"), SPLIT_SEP);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_FILTER_TEXT:
        sprintf(s1, "%sI %-3s    %s", SWITCH, larg_lit, get_longopt_description (SYM_FILTER_TEXT, TRUE));
        print_text (fp, s1);
        sprintf(s1, _("          Display only those fixed dates, whose text is matched by %s"), larg_lit);
        print_text (fp, s1);
#    if NO_REGEX
        sprintf(s1, _("   %-3s    = Any text you like, metacharacters are not respected"), larg_lit);
#    else /* !NO_REGEX */
        sprintf(s1, _("   %-3s    = Any text respectively \"regular expression\" you like"), larg_lit);
#    endif /* !NO_REGEX */
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
#  endif /* USE_RC */
      case SYM_DEBUG:
        sprintf(s1, "          %s", get_longopt_description (SYM_DEBUG, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Display some debug informations"));
        print_text (fp, s1);
        sprintf(s1, _("  [%-3s]   = internal    Informations if program internal maximums are reached"), larg_lit);
        print_text (fp, s1);
        strcpy(s1, _("          = handled     Like `internal' and file names which are handled"));
        print_text (fp, s1);
        strcpy(s1, _("          = unhandled   Like `internal' and file names which are unhandled"));
        print_text (fp, s1);
        strcpy(s1, _("          = all         Like `handled' and `unhandled' together"));
        print_text (fp, s1);
        strcpy(s1, _("          = abort       Like `all' and abort if file name can't be handled"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_TYPE_OF_CALENDAR:
        sprintf(s1, "%si[MOD]   %s", SWITCH, get_longopt_description (SYM_TYPE_OF_CALENDAR, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Use special format for calendar sheet"));
        print_text (fp, s1);
        strcat(s1, _("  [MOD]   = -   Use standard format for calendar sheet"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_ORTHODOX_CALENDAR:
        sprintf(s1, "%sO        %s", SWITCH, get_longopt_description (SYM_ORTHODOX_CALENDAR, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Use leap year rule of Eastern orthodox churches"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_CALENDAR_WITH_WEEKNO:
        sprintf(s1, "%sK        %s", SWITCH, get_longopt_description (SYM_CALENDAR_WITH_WEEKNO, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Provide calendar sheet with ISO week numbers"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_SUPPRESS_CALENDAR:
        sprintf(s1, "%su        %s", SWITCH, get_longopt_description (SYM_SUPPRESS_CALENDAR, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Suppress output of calendar sheet explicitly"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
#  if USE_PAGER
      case SYM_PAGER:
        sprintf(s1, "%sp        %s", SWITCH, get_longopt_description (SYM_PAGER, TRUE));
        print_text (fp, s1);
#    ifdef GCAL_EPAGER
        if (ext_pager != (char *)NULL)
          sprintf(s1, _("          Direct output through external `%s' pager"),
                  (*ext_pager == *DIR_SEP) ? strrchr(ext_pager, *DIR_SEP)+1 : ext_pager);
        else
          strcpy(s1, _("          Direct output through simple internal pager"));
#    else /* !GCAL_EPAGER */
        strcpy(s1, _("          Direct output through simple internal pager"));
#    endif /* !GCAL_EPAGER */
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
#  endif /* USE_PAGER */
      case SYM_DISABLE_HIGHLIGHTING:
        sprintf(s1, "%sH no     %s", SWITCH, get_longopt_description (SYM_DISABLE_HIGHLIGHTING, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          %s=no",  get_longopt_description (SYM_HIGHLIGHTING, FALSE));
        print_text (fp, s1);
        strcpy(s1, _("          Disable highlighting of text, holiday resp., actual day"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_FORCE_HIGHLIGHTING:
        sprintf(s1, "%sH yes    %s", SWITCH, get_longopt_description (SYM_FORCE_HIGHLIGHTING, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          %s=yes",  get_longopt_description (SYM_HIGHLIGHTING, FALSE));
        print_text (fp, s1);
        strcpy(s1, _("          Forces highlighting sequences if output is redirected/piped"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_HIGHLIGHTING:
        sprintf(s1, "%sH %-3s    %s", SWITCH, larg_lit, get_longopt_description (SYM_HIGHLIGHTING, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Set highlighting sequence pairs 1 (=actual day) and 2 (=holiday)"));
        print_text (fp, s1);
        sprintf(s1, _("   %-3s    = Highlighting sequence pairs separated by `%s' characters"),
                larg_lit, SEP);
        print_text (fp, s1);
        sprintf(s1, _("            e.g.  %sH \\x2%s\\xAE   Use hex values 2 and AE for sequence 1"),
                SWITCH, SEP);
        print_text (fp, s1);
        sprintf(s1, _("            e.g.  %sH %s%s*%s*      Use characters `*' and `*' for sequence 2"),
                SWITCH, SEP, SEP, SEP);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_BLOCKS:
        sprintf(s1, "%sb %-3s    %s", SWITCH, larg_lit, get_longopt_description (SYM_BLOCKS, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Modify format of year calendar"));
        print_text (fp, s1);
        sprintf(s1, _("   %-3s    = 1|2|3|4|6|12   Number of blocks"), larg_lit);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_CALENDAR_DATES:
        sprintf(s1, "%sj[MOD]   %s", SWITCH, get_longopt_description (SYM_CALENDAR_DATES, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Use consecutive day of year in calendar sheet"));
        print_text (fp, s1);
        strcpy(s1, _("  [MOD]   = b   Use both date notations (day of month+year)"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_HOLIDAY_DATES:
        sprintf(s1, "%sjn[MOD]  %s", SWITCH, get_longopt_description (SYM_HOLIDAY_DATES, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Use consecutive day of year in eternal holiday list"));
        print_text (fp, s1);
        strcpy(s1, _("   [MOD]  = b   Use both date notations (day of month+year)"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
#if USE_RC
      case SYM_FIXED_DATES:
        sprintf(s1, "%sjc[MOD]  %s", SWITCH, get_longopt_description (SYM_FIXED_DATES, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Use consecutive day of year in fixed date list"));
        print_text (fp, s1);
        strcpy(s1, _("   [MOD]  = b   Use both date notations (day of month+year)"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
#endif
      case SYM_STARTING_DAY:
        sprintf(s1, "%ss %-3s    %s", SWITCH, larg_lit, get_longopt_description (SYM_STARTING_DAY, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Set starting day of week"));
        print_text (fp, s1);
        if (use_short3_day_name)
          sprintf(s1, "   %-3s    = 0|today | %d|%s | %d|%s | ... | %d|%s",
                  larg_lit, DAY_MIN, short3_day_name (DAY_MIN),
                  DAY_MIN+1, short3_day_name (DAY_MIN+1),
                  DAY_MAX, short3_day_name (DAY_MAX));
        else
          sprintf(s1, "   %-3s    = 0|today | %d|%s | %d|%s | ... | %d|%s",
                  larg_lit, DAY_MIN, short_day_name (DAY_MIN),
                  DAY_MIN+1, short_day_name (DAY_MIN+1),
                  DAY_MAX, short_day_name (DAY_MAX));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
#  ifdef GCAL_EMAIL
      case SYM_MAIL:
        sprintf(s1, "          %s", get_longopt_description (SYM_MAIL, TRUE));
        print_text (fp, s1);
        ptr_env = MAIL_PRGR;
#    if !defined(AMIGA) || defined(__GNUC__)
        ptr_env = getenv(ENV_VAR_MAILPROG);
        if (ptr_env == (char *)NULL)
          ptr_env = MAIL_PRGR;
        else
          if (!*ptr_env)
            ptr_env = MAIL_PRGR;
#    endif /* AMIGA && !__GNUC__ */
        sprintf(s1, _("          Send output via `%s' program to user"), ptr_env);
        print_text (fp, s1);
        sprintf(s1, _("   %-3s    = Email address"), larg_lit);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
#  endif /* GCAL_EMAIL */
      case SYM_HOLIDAY_LIST:
      case SYM_DESC_HOLIDAY_LIST:
        sprintf(s1, "%sn|N[MOD] %s", SWITCH, get_longopt_description (SYM_HOLIDAY_LIST, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          %s", get_longopt_description (SYM_DESC_HOLIDAY_LIST, TRUE));
        print_text (fp, s1);
        sprintf(s1, _("          Display eternal holiday list (Year in range: %d...%d)"),
                EASTER_MIN, EASTER_MAX);
        print_text (fp, s1);
        strcpy(s1, _(" n        = Legal days and memorial days"));
        print_text (fp, s1);
        strcpy(s1, _("   N      = Legal days only"));
        print_text (fp, s1);
        sprintf(s1, _("    [MOD] = %s   Descending sort order"), DES_LIT);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_EXCLUDE_HD_TITLE:
        sprintf(s1, "%sX        %s", SWITCH, get_longopt_description (SYM_EXCLUDE_HD_TITLE, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Exclude title of eternal holiday list"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_STANDARD_HDY:
        sprintf(s1, "          %s", get_longopt_description (SYM_STANDARD_HDY, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Provide eternal holiday list without standard holidays"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_CHRISTIAN_HDY:
        sprintf(s1, "          %s", get_longopt_description (SYM_CHRISTIAN_HDY, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Provide eternal holiday list with Christian holidays"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_CC_HDY:
        sprintf(s1, "%sq %-3s    %s", SWITCH, larg_lit, get_longopt_description (SYM_CC_HDY, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Provide eternal holiday list with country specific holidays"));
        print_text (fp, s1);
        sprintf(s1, _("   %-3s    = Single country code or list of country codes"), larg_lit);
        print_text (fp, s1);
        sprintf(s1, _("            which are connected by `%s' characters"), CONNECT_SEP);
        print_text (fp, s1);
        do
         {
           sprintf(s1, _("            %s = %s holidays"), ptr_cc_holidays->id, _(ptr_cc_holidays->info));
           print_text (fp, s1);
         } while ((++ptr_cc_holidays)->info != (char *)NULL);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_GREG_REFORM:
        sprintf(s1, "          %s", get_longopt_description (SYM_GREG_REFORM, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Set period of Gregorian Reformation"));
        print_text (fp, s1);
        sprintf(s1, _("          Actual respected period:  %02d-%02d %s %0*d"),
                greg->first_day, greg->last_day, short_month_name (greg->month),
                len_year_max, greg->year);
        print_text (fp, s1);
        do
         {
           sprintf(s1, _("   %-*d%*s = Set period to:  %02d-%02d %s %0*d"),
                   len_year_max, ptr_greg->year, 6-len_year_max, "",
                   ptr_greg->first_day, ptr_greg->last_day,
                   short_month_name (ptr_greg->month), len_year_max, ptr_greg->year);
           print_text (fp, s1);
         } while ((++ptr_greg)->year);
        sprintf(s1, _("   %-3s    = %s%s%s%s%s%s%s   Set period explicitly"),
                larg_lit, yyyy_lit, SPLIT_SEP, mm_lit, SPLIT_SEP, dd_lit, SPLIT_SEP, dd_lit);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_DATE_FORMAT:
        sprintf(s1, "          %s", get_longopt_description (SYM_DATE_FORMAT, TRUE));
        print_text (fp, s1);
        sprintf(s1, _("          Set order of date elements using the %s format text"), larg_lit);
        print_text (fp, s1);
        sprintf(s1, _("          Actual format:  (%s) `%s' (%s)"),
                (date_format->id==(char *)NULL) ? _("self-defined") : date_format->id,
                date_format->format, _(date_format->info));
        print_text (fp, s1);
        do
         {
           sprintf(s1, _("   %-6s = Set format to:  `%s' (%s)"), ptr_date_format->id,
                   ptr_date_format->format, _(ptr_date_format->info));
           print_text (fp, s1);
         } while ((++ptr_date_format)->info != (char *)NULL);
        sprintf(s1, _("   %-3s    = Define personal format. Respected format elements are:"), larg_lit);
        print_text (fp, s1);
        do
         {
           if (*ptr_info_date_format->format == '\0')
             sprintf(s1, "               %s", _(ptr_info_date_format->info));
           else
             sprintf(s1, "            %-2s = %s", ptr_info_date_format->format, _(ptr_info_date_format->info));
           print_text (fp, s1);
         } while ((++ptr_info_date_format)->info != (char *)NULL);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_EXIT_STAT_HELP_127:
        sprintf(s1, "          %s", get_longopt_description (SYM_EXIT_STAT_HELP_127, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Set EXIT status of program to 127 on --help, --version etc."));
        print_text (fp, s1);
      default:
        ;   /* Void */
    }
#  if USE_RC
   if (longopt_symbolic == SYM_NIL)
    {
      print_text (fp, s1);
      sprintf(s1, _("+++ FIXED DATES +++"));
      print_text (fp, s1);
      print_text (fp, s1);
    }
   switch (longopt_symbolic)
    {
      case SYM_NIL:
      case SYM_TODAY:
      case SYM_FIXED_DATES_LIST:
      case SYM_DESC_FIXED_DATES_LIST:
        sprintf(s1, "%sc|C[MOD] %s", SWITCH, get_longopt_description (SYM_FIXED_DATES_LIST, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          %s", get_longopt_description (SYM_DESC_FIXED_DATES_LIST, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          %s", get_longopt_description (SYM_TODAY, TRUE));
        print_text (fp, s1);
#    ifdef GCAL_SHELL
        sprintf(s1, _("          Use standard resource file `.%s%s' for fixed date list"),
                PRGR_NAME, RC_SUFFIX);
#    else /* !GCAL_SHELL */
        sprintf(s1, _("          Use standard resource file `%s%s' for fixed date list"),
                PRGR_NAME, RC_SUFFIX);
#    endif /* !GCAL_SHELL */
        print_text (fp, s1);
        strcpy(s1, _("          Implies period:  Today"));
        print_text (fp, s1);
        strcpy(s1, _(" c        = Display only those dates, for which fixed dates exists"));
        print_text (fp, s1);
        strcpy(s1, _("   C      = Display those dates, for which fixed dates doesn't exist, too"));
        print_text (fp, s1);
        sprintf(s1, _("    [MOD] = %s   Descending sort order"), DES_LIT);
        print_text (fp, s1);
      default:
        ;   /* Void */
    }
   if (longopt_symbolic == SYM_NIL)
    {
      print_text (fp, s1);
      strcpy(s1, _("    [MOD] = One or more of the following modifiers which are marked by #"));
      print_text (fp, s1);
      print_text (fp, s1);
    }
   switch (longopt_symbolic)
    {
      case SYM_NIL:
      case SYM_GROUPING_TEXT:
        sprintf(s1, "          %s", get_longopt_description (SYM_GROUPING_TEXT, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          # g[MOD]       = Group fixed dates by day using text MOD"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      default:
        ;   /* Void */
    }
   if (longopt_symbolic == SYM_NIL)
    {
      print_text (fp, s1);
      strcpy(s1, _("          1. Representation of text"));
      print_text (fp, s1);
    }
   switch (longopt_symbolic)
    {
      case SYM_NIL:
      case SYM_BIORHYTHM_AXIS_LEN:
        sprintf(s1, "          %s", get_longopt_description (SYM_BIORHYTHM_AXIS_LEN, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("                         = Set width of the biorhythm text graphics"));
        print_text (fp, s1);
        sprintf(s1, _("                           %-3s = %d...%d   Length of a single axis"),
                larg_lit, BIO_AXIS_MIN, BIO_AXIS_MAX);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_MOONIMAGE_LINES:
        sprintf(s1, "          %s", get_longopt_description (SYM_MOONIMAGE_LINES, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("                         = Set height of the moonphase text graphics"));
        print_text (fp, s1);
        sprintf(s1, _("                           %-3s = %d...%d   Total number of lines"),
                larg_lit, MOONIMAGE_MIN, MOONIMAGE_MAX);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_INCLUDE_FILENAME:
        sprintf(s1, "          %s", get_longopt_description (SYM_INCLUDE_FILENAME, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          # a            = Display origin of fixed date"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_ALTERNATIVE_FORMAT:
        sprintf(s1, "          %s", get_longopt_description (SYM_ALTERNATIVE_FORMAT, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          # A            = Display using alternative list format"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_BYPASS_SHELL_CMD:
        sprintf(s1, "          %s", get_longopt_description (SYM_BYPASS_SHELL_CMD, TRUE));
        print_text (fp, s1);
        sprintf(s1, _("          # B            = Don't execute `%c%c[%s]' shell commands"),
                RC_SPECIAL_TEXT_CHAR, RC_SHELL_ESC_CHAR, larg_lit);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_INCLUDE_HOLIDAY:
        sprintf(s1, "          %s", get_longopt_description (SYM_INCLUDE_HOLIDAY, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          # e            = Include legal days and memorial days"));
        print_text (fp, s1);
        strcpy(s1, _("          # E            = Include legal days only"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_INCLUDE_WEEK_NO:
        sprintf(s1, "          %s", get_longopt_description (SYM_INCLUDE_WEEK_NO, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          # k            = Display ISO week number"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_OMIT_DATE_PART:
        sprintf(s1, "          %s", get_longopt_description (SYM_OMIT_DATE_PART, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          # o            = Omit repeating date part of fixed dates"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_SUPPRESS_DATE_PART:
        sprintf(s1, "          %s", get_longopt_description (SYM_SUPPRESS_DATE_PART, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          # U            = Suppress date part of fixed dates"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_EXCLUDE_RC_TITLE:
        sprintf(s1, "          %s", get_longopt_description (SYM_EXCLUDE_RC_TITLE, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          # x            = Exclude title of fixed date list"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_INCLUDE_CONS_NO:
        sprintf(s1, "          %s", get_longopt_description (SYM_INCLUDE_CONS_NO, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          # z            = Display consecutive number of fixed date"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_ZERO_DATES:
        sprintf(s1, "          %s", get_longopt_description (SYM_ZERO_DATES, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          # Z            = Display dates, for which fixed dates doesn't exist"));
        print_text (fp, s1);
      default:
        ;   /* Void */
    }
   if (longopt_symbolic == SYM_NIL)
    {
      print_text (fp, s1);
      strcpy(s1, _("          2. Respected period"));
      print_text (fp, s1);
    }
   switch (longopt_symbolic)
    {
      case SYM_NIL:
      case SYM_LEAP_DAY:
        sprintf(s1, "          %s", get_longopt_description (SYM_LEAP_DAY, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("                         = Manage `29-FEBRUARY' in non-leap years"));
        print_text (fp, s1);
        /*
           *** Translators, please don't translate the word `february',
           *** because it is used textually AS IS as an option argument.
        */
        sprintf(s1, _("                           %-3s = february   Respect at `28-FEBRUARY'"), larg_lit);
        print_text (fp, s1);
        /*
           *** Translators, please don't translate the word `march',
           *** because it is used textually AS IS as an option argument.
        */
        sprintf(s1, _("                           %-3s = march      Respect at `01-MARCH'"), larg_lit);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_INCLUDE_TODAY:
        sprintf(s1, "          %s", get_longopt_description (SYM_INCLUDE_TODAY, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          # d            = Include actual date if list of periods is generated"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_LIST_MODE:
        sprintf(s1, "          %s", get_longopt_description (SYM_LIST_MODE, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          # l            = Generate list of periods instead of a single period"));
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      default:
        ;   /* Void */
    }
   switch (longopt_symbolic)
    {
      case SYM_NIL:
      case SYM_PERIOD:
        sprintf(s1, "          %s", get_longopt_description (SYM_PERIOD, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          # Nd           = Single absolute day N"));
        print_text (fp, s1);
        sprintf(s1, _("          # %cdN          = Single absolute day N"), RC_NWD_CHAR);
        print_text (fp, s1);
        print_text (fp, s1);
        strcpy(s1, _("          # NMOD         = Single day N relative to today"));
        print_text (fp, s1);
        sprintf(s1, _("             MOD           = %s   Forwards"), ASC_LIT);
        print_text (fp, s1);
        sprintf(s1, _("                           = %s   Backwards"), DES_LIT);
        print_text (fp, s1);
        print_text (fp, s1);
        strcpy(s1, _("          # Nw           = Complete week N"));
        print_text (fp, s1);
        strcpy(s1, _("                           N = 0        1st week / last week of previous year"));
        print_text (fp, s1);
        strcpy(s1, _("                           N = 1...52   1st...52nd week (always)"));
        print_text (fp, s1);
        strcpy(s1, _("                           N = 53       53rd week (sometimes)"));
        print_text (fp, s1);
        strcpy(s1, _("                           N = 99       Last week"));
        print_text (fp, s1);
        print_text (fp, s1);
        sprintf(s1, _("          # `%s%s'       = Single day `%s' of month `%s'"),
                mm_lit, dd_lit, dd_lit, mm_lit);
        print_text (fp, s1);
        print_text (fp, s1);
        sprintf(s1, _("          # `%s%s'N     = Single N'th weekday `%s' of month `%s'"),
                mm_lit, www_lit, www_lit, mm_lit);
        print_text (fp, s1);
        sprintf(s1, _("                           N = 1...4   1st...4th weekday `%s' (always)"), www_lit);
        print_text (fp, s1);
        sprintf(s1, _("                           N = 5       5th weekday `%s' (sometimes)"), www_lit);
        print_text (fp, s1);
        sprintf(s1, _("                           N = 9       Last weekday `%s'"), www_lit);
        print_text (fp, s1);
        print_text (fp, s1);
        sprintf(s1, _("          # %cdN`%s'     = Single N'th weekday `%s'"),
                RC_NWD_CHAR, www_lit, www_lit);
        print_text (fp, s1);
        sprintf(s1, _("                           N = 1...51   1st...51st weekday `%s' (always)"), www_lit);
        print_text (fp, s1);
        sprintf(s1, _("                           N = 52|53    52|53rd weekday `%s' (sometimes)"), www_lit);
        print_text (fp, s1);
        sprintf(s1, _("                           N = 99       Last weekday `%s'"), www_lit);
        print_text (fp, s1);
        print_text (fp, s1);
        sprintf(s1, _("          # %cwN`%s'     = Single weekday `%s' of N'th week"),
                RC_NWD_CHAR, www_lit, www_lit);
        print_text (fp, s1);
        sprintf(s1, _("                           N = 0        `%s' which isn't located in 1st week"), www_lit);
        print_text (fp, s1);
        sprintf(s1, _("                           N = 1...51   `%s' of 1st...51st week (always)"), www_lit);
        print_text (fp, s1);
        sprintf(s1, _("                           N = 52|53    `%s' of 52|53rd week (sometimes)"), www_lit);
        print_text (fp, s1);
        sprintf(s1, _("                           N = 99       `%s' of last week"), www_lit);
        print_text (fp, s1);
        print_text (fp, s1);
        sprintf(s1, _("          # %c%c[[%s|%s]N]   = Single day N relative to Easter Sunday"),
                RC_HDY_CHAR, RC_EASTER_CHAR, ASC_LIT, DES_LIT);
        print_text (fp, s1);
        print_text (fp, s1);
        sprintf(s1, _("          # %c%c[%s|%s]N`%s'= Single N'th weekday relative to Easter Sunday"),
                RC_HDY_CHAR, RC_EASTER_CHAR, ASC_LIT, DES_LIT, www_lit);
        print_text (fp, s1);
        print_text (fp, s1);
        sprintf(s1, _("          # %c%c[[%s|%s]N]   = Single day N relative to today's date"),
                RC_HDY_CHAR, RC_TODAY_CHAR, ASC_LIT, DES_LIT);
        print_text (fp, s1);
        print_text (fp, s1);
        sprintf(s1, _("          # %c%c[%s|%s]N`%s'= Single N'th weekday relative to today's date"),
                RC_HDY_CHAR, RC_TODAY_CHAR, ASC_LIT, DES_LIT, www_lit);
        print_text (fp, s1);
        print_text (fp, s1);
        sprintf(s1, _("          # %c?[[%s|%s]N]   = Single day N relative to date variable"),
                RC_HDY_CHAR, ASC_LIT, DES_LIT);
        print_text (fp, s1);
        print_text (fp, s1);
        sprintf(s1, _("          # %c?[%s|%s]N`%s'= Single N'th weekday relative to date variable"),
                RC_HDY_CHAR, ASC_LIT, DES_LIT, www_lit);
        print_text (fp, s1);
        print_text (fp, s1);
      case SYM_TOMORROW:
        sprintf(s1, "          %s", get_longopt_description (SYM_TOMORROW, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          # t|T          = List tomorrow"));
        print_text (fp, s1);
        if (longopt_symbolic == SYM_TOMORROW)
          break;
        print_text (fp, s1);
      case SYM_WEEK:
      case SYM_END_OF_WEEK:
      case SYM_START_OF_WEEK:
        sprintf(s1, "          %s", get_longopt_description (SYM_WEEK, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          %s", get_longopt_description (SYM_END_OF_WEEK, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          %s", get_longopt_description (SYM_START_OF_WEEK, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          # w|W[MOD]     = List complete week"));
        print_text (fp, s1);
        sprintf(s1, _("               [MOD]       = %s   List tomorrow until ending day of week"),
                ASC_LIT);
        print_text (fp, s1);
        sprintf(s1, _("                           = %s   List yesterday until starting day of week"),
                DES_LIT);
        print_text (fp, s1);
        if (   longopt_symbolic == SYM_WEEK
            || longopt_symbolic == SYM_END_OF_WEEK
            || longopt_symbolic == SYM_START_OF_WEEK)
          break;
        print_text (fp, s1);
      case SYM_MONTH:
      case SYM_END_OF_MONTH:
      case SYM_START_OF_MONTH:
        sprintf(s1, "          %s", get_longopt_description (SYM_MONTH, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          %s", get_longopt_description (SYM_END_OF_MONTH, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          %s", get_longopt_description (SYM_START_OF_MONTH, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          # m|M[MOD]     = List complete month"));
        print_text (fp, s1);
        sprintf(s1, _("               [MOD]       = %s   List tomorrow until ending day of month"),
                ASC_LIT);
        print_text (fp, s1);
        sprintf(s1, _("                           = %s   List yesterday until starting day of month"),
                DES_LIT);
        print_text (fp, s1);
        if (   longopt_symbolic == SYM_MONTH
            || longopt_symbolic == SYM_END_OF_MONTH
            || longopt_symbolic == SYM_START_OF_MONTH)
          break;
        print_text (fp, s1);
      case SYM_YEAR:
      case SYM_END_OF_YEAR:
      case SYM_START_OF_YEAR:
        sprintf(s1, "          %s", get_longopt_description (SYM_YEAR, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          %s", get_longopt_description (SYM_END_OF_YEAR, TRUE));
        print_text (fp, s1);
        sprintf(s1, "          %s", get_longopt_description (SYM_START_OF_YEAR, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          # y|Y[MOD]     = List complete year"));
        print_text (fp, s1);
        sprintf(s1, _("               [MOD]       = %s   List tomorrow until ending day of year"),
                ASC_LIT);
        print_text (fp, s1);
        sprintf(s1, _("                           = %s   List yesterday until starting day of year"),
                DES_LIT);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_RESOURCE_FILE:
        sprintf(s1, "%sf|F %-3s  %s", SWITCH, larg_lit, get_longopt_description (SYM_RESOURCE_FILE, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Use alternative file(s) instead of standard resource file"));
        print_text (fp, s1);
        strcpy(s1, _("          Implies period:  Today"));
        print_text (fp, s1);
        strcpy(s1, _(" f        = Display only those dates, for which fixed dates exists"));
        print_text (fp, s1);
        strcpy(s1, _("   F      = Display those dates, for which fixed dates doesn't exist, too"));
        print_text (fp, s1);
        sprintf(s1, _("     %-3s  = Single file or list of files connected by `%s' characters"),
                larg_lit, CONNECT_SEP);
        print_text (fp, s1);
        if (longopt_symbolic != SYM_NIL)
          break;
        print_text (fp, s1);
      case SYM_HERE_FILE:
        sprintf(s1, "%s# %-3s    %s", SWITCH, larg_lit, get_longopt_description (SYM_HERE_FILE, TRUE));
        print_text (fp, s1);
        strcpy(s1, _("          Define additional resource file line"));
        print_text (fp, s1);
        sprintf(s1, _("   %-3s    = Any possible resource file line"), larg_lit);
        print_text (fp, s1);
      default:
          ;   /* Void */
    }
   if (longopt_symbolic == SYM_NIL)
    {
      print_text (fp, s1);
      print_text (fp, s1);
      if (print_hls)
        strcpy(s1, ehls2s.seq);
      sprintf(s2, _("%cDATE"), RC_ADATE_CHAR);
      strcat(s1, s2);
      if (print_hls)
        strcat(s1, ehls2e.seq);
      print_text (fp, s1);
      strcpy(s1, _("Use given `DATE' instead of today's date"));
      print_text (fp, s1);
      sprintf(s1, _("Date format:  %s[%s[%s|%s[N]]], %s%c%c|%c|dvar[[%s|%s]N[%s]], %s%cdN[%s]"),
              yyyy_lit, mm_lit, dd_lit, www_lit, yyyy_lit, RC_HDY_CHAR, RC_EASTER_CHAR, RC_TODAY_CHAR,
              ASC_LIT, DES_LIT, www_lit, yyyy_lit, RC_NWD_CHAR, www_lit);
      print_text (fp, s1);
      sprintf(s1, _("              %s%cwN[%s], month name[%s], weekday name[N] or %s"),
              yyyy_lit, RC_NWD_CHAR, www_lit, dd_lit, dd_lit);
      print_text (fp, s1);
    }
#  endif /* USE_RC */
   if (longopt_symbolic == SYM_NIL)
    {
      print_text (fp, s1);
      print_text (fp, s1);
      if (print_hls)
        strcpy(s1, ehls2s.seq);
      sprintf(s2, _("%cFILE"), RSP_CHAR);
      strcat(s1, s2);
      if (print_hls)
        strcat(s1, ehls2e.seq);
      print_text (fp, s1);
      strcpy(s1, _("Preload options and commands from `FILE'"));
      print_text (fp, s1);
      print_text (fp, s1);
      print_text (fp, s1);
      if (print_hls)
        strcpy(s1, ehls2s.seq);
      strcat(s1, _("COMMAND"));
      if (print_hls)
        strcat(s1, ehls2e.seq);
      print_text (fp, s1);
      sprintf(s1, _("%s        = Month in range:  %d...%d"), mm_lit, MONTH_MIN, MONTH_MAX);
      print_text (fp, s1);
      sprintf(s1, _("                        or:  month name | %s | %s%s | %s%s | %s%s"),
              MONTH3_LIT, MONTH3_LIT, MONTH3_LIT, MONTH3_LIT, ASC_LIT, MONTH3_LIT, DES_LIT);
      print_text (fp, s1);
      print_text (fp, s1);
      sprintf(s1, _("            List:   %s[%s%s]%s...%s%s[%s%s]"),
              mm_lit, YEAR_SEP, yyyy_lit, MLIST_SEP, MLIST_SEP, mm_lit, YEAR_SEP, yyyy_lit);
      print_text (fp, s1);
      sprintf(s1, _("            Range:  %s[%s%s]%s%s[%s%s] | %s%s%s%s%s | %s %s%s%s"),
              mm_lit, YEAR_SEP, yyyy_lit, MRANGE_SEP, mm_lit, YEAR_SEP, yyyy_lit,
              mm_lit, YEAR_SEP, yyyy_lit, MRANGE_SEP, yyyy_lit,
              mm_lit, yyyy_lit, MRANGE_SEP, yyyy_lit);
      print_text (fp, s1);
      print_text (fp, s1);
      sprintf(s1, _("%s%*s    = Year  in range:  %d...%d"),
              yyyy_lit, 6-len_year_max, "", YEAR_MIN, YEAR_MAX);
      print_text (fp, s1);
      print_text (fp, s1);
      sprintf(s1, _("            List:   [%s%s]%s%s...%s[%s%s]%s"),
              mm_lit, FYEAR_SEP, yyyy_lit, YLIST_SEP, YLIST_SEP, mm_lit, FYEAR_SEP, yyyy_lit);
      print_text (fp, s1);
      sprintf(s1, _("            Range:  [%s%s]%s%s[%s%s]%s"),
              mm_lit, FYEAR_SEP, yyyy_lit, YRANGE_SEP, mm_lit, FYEAR_SEP, yyyy_lit);
    }
#endif /* !USE_DE */
   if (longopt_symbolic == SYM_NIL)
    {
      print_text (fp, s1);
      print_text (fp, s1);
      my_help_tail_text (fp);
    }
}



   PUBLIC void
my_basic_help (fp)
   FILE *fp;
/*
   Prints the "basic help" text to file `fp' using the central
     output function `print_text()' and uses global text buffer
     `s1' and `s2' internally.
*/
{
   my_help_head_text (fp);
#if USE_DE
   sprintf(s1, "%sh|hh   = Hilfetext; %sL = Lizenz; %sV = Version; %su = Kalender unterdr%scken",
           SWITCH, SWITCH, SWITCH, SWITCH, UE);
   print_text (fp, s1);
   sprintf(s1, "%si[-]   = Spezial-Kalenderformat; ", SWITCH);
#  if USE_PAGER
   sprintf(s2, "%sp = Paginierer; ", SWITCH);
   strcat(s1, s2);
#  endif
   /*
      *** Translators, please don't translate the word `yes',
      *** because it is used textually AS IS as an option argument.
   */
   sprintf(s2, "%sH yes = Mit Hervorhebungen", SWITCH);
   strcat(s1, s2);
   print_text (fp, s1);
   /*
      *** Translators, please don't translate the word `no',
      *** because it is used textually AS IS as an option argument.
   */
   sprintf(s1, "%sb %-3s  = Jahreskalender in 1|2|3|4|6|12 Bl%scken; %sH no = Ohne Hervorhebungen",
           SWITCH, larg_lit, OE, SWITCH);
   print_text (fp, s1);
   sprintf(s1, "%sj[]    = Fortlaufenden Tag des Jahres ausgeben (Modifizierer: [b n nb",
           SWITCH);
#  if USE_RC
   strcat(s1, " c cb])");
#  else /* !USE_RC */
   strcat(s1, "])");
#  endif /* !USE_RC */
   print_text (fp, s1);
   if (use_short3_day_name)
     /*
        *** Translators, please don't translate the word `today',
        *** because it is used textually AS IS as an option argument.
     */
     sprintf(s1, "%ss %-3s  = Starttag der Woche (%s = 0|today | %d|%s | %d|%s | ... | %d|%s)",
             SWITCH, larg_lit, larg_lit, DAY_MIN, short3_day_name (DAY_MIN), DAY_MIN+1,
             short3_day_name (DAY_MIN+1), DAY_MAX, short3_day_name (DAY_MAX));
   else
     /*
        *** Translators, please don't translate the word `today',
        *** because it is used textually AS IS as an option argument.
     */
     sprintf(s1, "%ss %-3s  = Starttag der Woche (%s = 0|today | %d|%s | %d|%s | ... | %d|%s)",
             SWITCH, larg_lit, larg_lit, DAY_MIN, short_day_name (DAY_MIN), DAY_MIN+1,
             short_day_name (DAY_MIN+1), DAY_MAX, short_day_name (DAY_MAX));
   print_text (fp, s1);
   sprintf(s1, "%sn|N[%s] = Ewige Feiertagsliste ausgeben (Jahr im Bereich: %d...%d)",
           SWITCH, DES_LIT, EASTER_MIN, EASTER_MAX);
   print_text (fp, s1);
#  if USE_RC
#    ifdef GCAL_SHELL
   sprintf(s1, "%sc|C[]  = Terminliste aus Datei `.%s%s'; %sf|F %s[%s%s%s...]=Benutze Datei %s",
           SWITCH, PRGR_NAME, RC_SUFFIX, SWITCH, larg_lit, CONNECT_SEP, larg_lit, CONNECT_SEP, larg_lit);
#    else /* !GCAL_SHELL */
   sprintf(s1, "%sc|C[]  = Terminliste aus Datei `%s%s'; %sf|F %s[%s%s%s...]=Benutze Datei %s",
           SWITCH, PRGR_NAME, RC_SUFFIX, SWITCH, larg_lit, CONNECT_SEP, larg_lit, CONNECT_SEP, larg_lit);
#    endif /* !GCAL_SHELL */
   print_text (fp, s1);
   sprintf(s1, "    ^^--> [g[] | [%saABdeEkloUxzZ][N[d|w|%s|%s]|`%s%s'|`%s%s'N|t|[w|m|y[%s|%s]]]]",
           DES_LIT, ASC_LIT, DES_LIT, mm_lit, dd_lit, mm_lit, www_lit, ASC_LIT, DES_LIT);
   print_text (fp, s1);
#  endif
   sprintf(s1, "%s   = Monat im Bereich: %d...%d               Liste:   %s[%s%s]%s...%s%s[%s%s]",
           mm_lit, MONTH_MIN, MONTH_MAX, mm_lit, YEAR_SEP, yyyy_lit, MLIST_SEP, MLIST_SEP, mm_lit,
           YEAR_SEP, yyyy_lit);
   print_text (fp, s1);
   sprintf(s1, "                   oder: Monatsname|%s[%s|%s|%s]  Bereich: %s[%s%s]%s%s[%s%s]",
           MONTH3_LIT, MONTH3_LIT, ASC_LIT, DES_LIT, mm_lit, YEAR_SEP, yyyy_lit, MRANGE_SEP,
           mm_lit, YEAR_SEP, yyyy_lit);
   print_text (fp, s1);
   sprintf(s1, "%s = Jahr  im Bereich: %d...%d%*s%*s         Liste:   [%s%s]%s%s...%s[%s%s]%s",
           yyyy_lit, YEAR_MIN, YEAR_MAX, 6-len_year_max, "", 6-len_year_max, "", mm_lit,
           FYEAR_SEP, yyyy_lit, YLIST_SEP, YLIST_SEP, mm_lit, FYEAR_SEP, yyyy_lit);
   print_text (fp, s1);
   sprintf(s1, "                                              Bereich: [%s%s]%s%s[%s%s]%s",
           mm_lit, FYEAR_SEP, yyyy_lit, YRANGE_SEP, mm_lit, FYEAR_SEP, yyyy_lit);
#else /* !USE_DE */
   sprintf(s1, _("%sh|hh   = Help screen; %sL = License; %sV = Version; %su = Suppress calendar sheet"),
           SWITCH, SWITCH, SWITCH, SWITCH);
   print_text (fp, s1);
   sprintf(s1, _("%si[-]   = Special calendar format; "), SWITCH);
#  if USE_PAGER
   sprintf(s2, _("%sp = Simple pager; "), SWITCH);
   strcat(s1, s2);
#  endif
   /*
      *** Translators, please don't translate the word `yes',
      *** because it is used textually AS IS as an option argument.
   */
   sprintf(s2, _("%sH yes = Use highlighting"), SWITCH);
   strcat(s1, s2);
   print_text (fp, s1);
   /*
      *** Translators, please don't translate the word `no',
      *** because it is used textually AS IS as an option argument.
   */
   sprintf(s1, _("%sb %-3s  = Year calendar with 1|2|3|4|6|12 blocks; %sH no = Disable highlighting"),
           SWITCH, larg_lit, SWITCH);
   print_text (fp, s1);
   sprintf(s1, _("%sj[]    = Output of consecutive day of year (Modifiers: [b n nb"),
           SWITCH);
#  if USE_RC
   strcat(s1, " c cb])");
#  else /* !USE_RC */
   strcat(s1, "])");
#  endif /* !USE_RC */
   print_text (fp, s1);
   if (use_short3_day_name)
     /*
        *** Translators, please don't translate the word `today',
        *** because it is used textually AS IS as an option argument.
     */
     sprintf(s1, _("%ss %-3s  = Starting day of week (%s = 0|today | %d|%s | %d|%s | ... | %d|%s)"),
             SWITCH, larg_lit, larg_lit, DAY_MIN, short3_day_name (DAY_MIN), DAY_MIN+1,
             short3_day_name (DAY_MIN+1), DAY_MAX, short3_day_name (DAY_MAX));
   else
     /*
        *** Translators, please don't translate the word `today',
        *** because it is used textually AS IS as an option argument.
     */
     sprintf(s1, _("%ss %-3s  = Starting day of week (%s = 0|today | %d|%s | %d|%s | ... | %d|%s)"),
             SWITCH, larg_lit, larg_lit, DAY_MIN, short_day_name (DAY_MIN), DAY_MIN+1,
             short_day_name (DAY_MIN+1), DAY_MAX, short_day_name (DAY_MAX));
   print_text (fp, s1);
   sprintf(s1, _("%sn|N[%s] = Display eternal holiday list (Year in range: %d...%d)"),
           SWITCH, DES_LIT, EASTER_MIN, EASTER_MAX);
   print_text (fp, s1);
#  if USE_RC
#    ifdef GCAL_SHELL
   sprintf(s1, _("%sc|C[]  = Fixed date list of file `.%s%s'; %sf|F %s[%s%s%s...]=Use file %s"),
           SWITCH, PRGR_NAME, RC_SUFFIX, SWITCH, larg_lit, CONNECT_SEP, larg_lit, CONNECT_SEP, larg_lit);
#    else /* !GCAL_SHELL */
   sprintf(s1, _("%sc|C[]  = Fixed date list of file `%s%s'; %sf|F %s[%s%s%s...]=Use file %s"),
           SWITCH, PRGR_NAME, RC_SUFFIX, SWITCH, larg_lit, CONNECT_SEP, larg_lit, CONNECT_SEP, larg_lit);
#    endif /* !GCAL_SHELL */
   print_text (fp, s1);
   sprintf(s1, "    ^^--> [g[] | [%saABdeEkloUxzZ][N[d|w|%s|%s]|`%s%s'|`%s%s'N|t|[w|m|y[%s|%s]]]]",
           DES_LIT, ASC_LIT, DES_LIT, mm_lit, dd_lit, mm_lit, www_lit, ASC_LIT, DES_LIT);
   print_text (fp, s1);
#  endif
   sprintf(s1, _("%s    = Month in range: %d...%d                 List:  %s[%s%s]%s...%s%s[%s%s]"),
           mm_lit, MONTH_MIN, MONTH_MAX, mm_lit, YEAR_SEP, yyyy_lit, MLIST_SEP, MLIST_SEP, mm_lit,
           YEAR_SEP, yyyy_lit);
   print_text (fp, s1);
   sprintf(s1, _("                    or: month name|%s[%s|%s|%s]    Range: %s[%s%s]%s%s[%s%s]"),
           MONTH3_LIT, MONTH3_LIT, ASC_LIT, DES_LIT, mm_lit, YEAR_SEP, yyyy_lit, MRANGE_SEP,
           mm_lit, YEAR_SEP, yyyy_lit);
   print_text (fp, s1);
   sprintf(s1, _("%s  = Year  in range: %d...%d%*s%*s           List:  [%s%s]%s%s...%s[%s%s]%s"),
           yyyy_lit, YEAR_MIN, YEAR_MAX, 6-len_year_max, "", 6-len_year_max, "", mm_lit,
           FYEAR_SEP, yyyy_lit, YLIST_SEP, YLIST_SEP, mm_lit, FYEAR_SEP, yyyy_lit);
   print_text (fp, s1);
   sprintf(s1, _("                                               Range: [%s%s]%s%s[%s%s]%s"),
           mm_lit, FYEAR_SEP, yyyy_lit, YRANGE_SEP, mm_lit, FYEAR_SEP, yyyy_lit);
#endif /* !USE_DE */
   my_help_tail_text (fp);
}



   PUBLIC void
my_license (fp)
   FILE *fp;
/*
   Prints the program-id and the license text to file `fp' using the central
     output function `print_text()', uses global text buffer `s1' internally.
*/
{
   my_copyright (fp, FALSE);
#if USE_DE
   sprintf(s1, "Diese Software erhebt keinen Anspruch auf Vollst%sndigkeit,", AE);
   print_text (fp, s1);
   sprintf(s1, "Verwendungsf%shigkeit oder Korrektheit. F%sr JEDWEDE Sch%sden oder", AE, UE, AE);
   print_text (fp, s1);
   strcpy(s1, "Verluste (implizit oder explizit), die aus Nutzung oder Handhabung");
   print_text (fp, s1);
   sprintf(s1, "meiner Software hervorgehen, wird grunds%stzlich keine Haftung", AE);
   print_text (fp, s1);
   sprintf(s1, "%sbernommen. Bei Verwendung dieser Software erkl%srt sich der Benutzer", UE, AE);
   print_text (fp, s1);
   strcpy(s1, "mit dieser Vereinbarung in vollem Umfang einverstanden.");
   print_text (fp, s1);
   print_text (fp, s1);
   sprintf(s1, "Dieses Programm ist freie Software; Sie k%snnen es weitervertreiben", OE);
   print_text (fp, s1);
   sprintf(s1, "und/oder ver%sndern innerhalb der Bestimmungen der `GNU General Public", AE);
   print_text (fp, s1);
   strcpy(s1, "License'; herausgegeben von der `Free Software Foundation';");
   print_text (fp, s1);
   sprintf(s1, "in der Version 2 oder jeder sp%steren Version.", AE);
   print_text (fp, s1);
   print_text (fp, s1);
   strcpy(s1, "Sie sollten eine Kopie der `GNU General Public License' zusammen mit");
   print_text (fp, s1);
   strcpy(s1, "diesem Programm erhalten haben; falls nicht, schreiben Sie an die:");
#else /* !USE_DE */
   strcpy(s1, _("This software doesn't claim completeness, correctness or usability."));
   print_text (fp, s1);
   strcpy(s1, _("On principle I will not be liable for ANY damages or losses (implicit"));
   print_text (fp, s1);
   strcpy(s1, _("or explicit), which result from using or handling my software."));
   print_text (fp, s1);
   strcpy(s1, _("If you use this software, you agree without any exception to this"));
   print_text (fp, s1);
   strcpy(s1, _("agreement, which binds you LEGALLY !!"));
   print_text (fp, s1);
   print_text (fp, s1);
   strcpy(s1, _("This program is free software; you can redistribute it and/or modify"));
   print_text (fp, s1);
   strcpy(s1, _("it under the terms of the `GNU General Public License' as published by"));
   print_text (fp, s1);
   strcpy(s1, _("the `Free Software Foundation'; either version 2, or (at your option)"));
   print_text (fp, s1);
   strcpy(s1, _("any later version."));
   print_text (fp, s1);
   print_text (fp, s1);
   strcpy(s1, _("You should have received a copy of the `GNU General Public License'"));
   print_text (fp, s1);
   strcpy(s1, _("along with this program; if not, write to the:"));
#endif /* !USE_DE */
   print_text (fp, s1);
   print_text (fp, s1);
   strcpy(s1, "   Free Software Foundation, Inc.");
   print_text (fp, s1);
   strcpy(s1, "   59 Temple Place - Suite 330");
   print_text (fp, s1);
   strcpy(s1, "   Boston, MA 02111-1307,  USA");
   print_text (fp, s1);
}



   PUBLIC void
my_version (fp)
   FILE *fp;
/*
   Prints the program-id and all compilation flags to file `fp' using the central
     output function `print_text()', uses global text buffer `s1' internally.
*/
{
   register int    i=0;
#if USE_RC
   auto     char  *ptr_env;
   auto     char  *ptr_home;
#endif


   my_copyright (fp, TRUE);
   print_compiler_info (fp);
   print_text (fp, s1);
#if USE_DE
   strcpy(s1, "Kompilations-Optionen:");
#else /* !USE_DE */
   strcpy(s1, _("Compilation options:"));
#endif /* !USE_DE */
#ifdef GCAL_NLS
   strcat(s1, " NLS");
   i++;
#endif
#if USE_EASC
   strcat(s1, " USE_EASC");
   i++;
#endif
#if USE_DE
   strcat(s1, " USE_DE");
   i++;
#endif
#if USE_HLS
   strcat(s1, " USE_HLS");
   i++;
#endif
#if USE_PAGER
   strcat(s1, " USE_PAGER");
   i++;
#endif
#if USE_RC
   strcat(s1, " USE_RC");
   i++;
#  if HAVE_GNU_RE_COMPILE_PATTERN
   strcat(s1, " GNU-REGEX");
   i++;
#  endif
#  if HAVE_POSIX_REGCOMP
   strcat(s1, " POSIX-REGEX");
   i++;
#  endif
#  if HAVE_RE_COMP
   strcat(s1, " BSD-REGEX");
   i++;
#  endif
#  if HAVE_REGCMP
   strcat(s1, " SysV-REGEX");
   i++;
#  endif
#  if HAVE_V8_REGCOMP
   strcat(s1, " V8-REGEX");
   i++;
#  endif
#  if NO_REGEX
   strcat(s1, " NO-REGEX");
   i++;
#  endif
#endif /* USE_RC */
   if (i)
     print_text (fp, s1);
   print_text (fp, s1);
#if USE_RC
#  if !defined(AMIGA) || defined(__GNUC__)
   ptr_env = getenv(ENV_VAR_SYS_DATADIR);
   if (ptr_env == (char *)NULL)
     ptr_env = GCAL_SYS_DATADIR;
   else
     if (!*ptr_env)
       ptr_env = GCAL_SYS_DATADIR;
   sprintf(s1, "GCAL_SYS_DATADIR=%s", ptr_env);
#  else /* AMIGA && !__GNUC__ */
   sprintf(s1, "GCAL_SYS_DATADIR=%s", GCAL_SYS_DATADIR);
#  endif /* AMIGA && !__GNUC__ */
   print_text (fp, s1);
#  if !defined(AMIGA) || defined(__GNUC__)
   ptr_env = getenv(ENV_VAR_USR_DATADIR);
   if (ptr_env == (char *)NULL)
     ptr_env = GCAL_USR_DATADIR;
   else
     if (!*ptr_env)
       ptr_env = GCAL_USR_DATADIR;
   i = 1;
   ptr_home = getenv(ENV_VAR_HOME);
   if (ptr_home == (char *)NULL)
     i--;
   else
     if (!*ptr_home)
       i--;
   if (i)
    {
      if (*(ptr_home+(strlen(ptr_home)-1)) == *DIR_SEP)
        sprintf(s1, "GCAL_USR_DATADIR=%s%s", ptr_home, ptr_env);
      else
        sprintf(s1, "GCAL_USR_DATADIR=%s%s%s", ptr_home, DIR_SEP, ptr_env);
    }
   else
     sprintf(s1, "GCAL_USR_DATADIR=$%s%s%s", ENV_VAR_HOME, DIR_SEP, ptr_env);
#  else /* AMIGA && !__GNUC__ */
   sprintf(s1, "GCAL_USR_DATADIR=%s", GCAL_USR_DATADIR);
#  endif /* AMIGA && !__GNUC__ */
   print_text (fp, s1);
   print_text (fp, s1);
#endif /* USE_RC */
#if USE_DE
   sprintf(s1, "Gregorianische Reformation: %02d-%02d %s %0*d", greg->first_day,
           greg->last_day, short_month_name (greg->month), len_year_max, greg->year);
#else /* !USE_DE */
   sprintf(s1, _("Gregorian Reformation: %02d-%02d %s %0*d"), greg->first_day,
           greg->last_day, short_month_name (greg->month), len_year_max, greg->year);
#endif /* !USE_DE */
   print_text (fp, s1);
#if USE_DE
   sprintf(s1, "Datumformat: (%s) `%s' (%s)",
           (date_format->id==(char *)NULL) ? "Selbstdefiniert" : date_format->id,
           date_format->format, date_format->info);
#else /* !USE_DE */
   sprintf(s1, _("Date format: (%s) `%s' (%s)"),
           (date_format->id==(char *)NULL) ? _("self-defined") : date_format->id,
           date_format->format, _(date_format->info));
#endif /* !USE_DE */
   print_text (fp, s1);
   print_text (fp, s1);
#if USE_DE
   strcpy(s1, "Fehlerberichte via eMail an <esken@uni-muenster.de>");
   print_text (fp, s1);
   sprintf(s1, "oder (falls das fehlschl%sgt) an <bug-gnu-utils@prep.ai.mit.edu>.", AE);
#else /* !USE_DE */
   strcpy(s1, _("Email bug reports to <esken@uni-muenster.de>"));
   print_text (fp, s1);
   strcpy(s1, _("or (if this fails) to <bug-gnu-utils@prep.ai.mit.edu>."));
#endif /* !USE_DE */
   print_text (fp, s1);
}



   PUBLIC char *
usage_msg ()
/*
   Creates the usage text `usage_text' and includes the actual program name.
*/
{
   static char  *usg_text;
   static Bool   is_initialized=FALSE;


   if (!is_initialized)
    {
      usg_text = (char *)my_malloc (120, 124, __FILE__, ((long)__LINE__)-1, "usg_text", 0);
#if USE_DE
      sprintf(usg_text, "Aufruf: %s [%s|%s{[?|h|??|hh|L|V]|{%sHKNOR%sXb%sijn%sq%ssu%s}}] [[%s] [%s]]",
              prgr_name, SWITCH, SWITCH2, USAGE_RC1, USAGE_SHELL, USAGE_RC2, USAGE_PAGER,
              USAGE_RC3, USAGE_RC4, mm_lit, yyyy_lit);
#else /* !USE_DE */
      sprintf(usg_text, _("Usage:  %s [%s|%s{[?|h|??|hh|L|V]|{%sHKNOR%sXb%sijn%sq%ssu%s}}] [[%s] [%s]]"),
              prgr_name, SWITCH, SWITCH2, USAGE_RC1, USAGE_SHELL, USAGE_RC2, USAGE_PAGER,
              USAGE_RC3, USAGE_RC4, mm_lit, yyyy_lit);
#endif /* !USE_DE */
      is_initialized = TRUE;
    }

   return(usg_text);
}



   PUBLIC void
put_longopt_description (fp)
   FILE *fp;
/*
   Puts a description text of the long-style options to file `fp'.
*/
{
#if USE_DE
   fprintf(fp, "Benutze `%s %s' oder ",
           prgr_name, get_longopt_description (SYM_HELP, TRUE));
   fprintf(fp, "`%s %s|[%s?]' f%sr mehr Information.",
           prgr_name, get_longopt_description (SYM_LONG_HELP2, TRUE), LARG_SEP, UE);
#else /* !USE_DE */
   fprintf(fp, _("Use `%s %s' or "),
           prgr_name, get_longopt_description (SYM_HELP, TRUE));
   fprintf(fp, _("`%s %s|[%s?]' for more information."),
           prgr_name, get_longopt_description (SYM_LONG_HELP2, TRUE), LARG_SEP);
#endif /* !USE_DE */
}



   LOCAL const char *
get_longopt_description (longopt_symbolic, with_larglist)
   const int  longopt_symbolic;
   const Bool with_larglist;
/*
   Returns a description text of given `longopt_symbolic' long-style option
     in global text buffer `s2'.  If `with_larglist' is TRUE, the returned
     description text contains the argument list text (if any) too, otherwise
     the returned description text contains the complete name of the long option only!
*/
{
   auto     const Lopt_struct  *ptr_lopt=lopt;
   register       int           i;


   while (ptr_lopt->long_name != NULL)
    {
      if (ptr_lopt->symbolic_name == longopt_symbolic)
        break;
      ptr_lopt++;
    }
   if (ptr_lopt->long_name != NULL)
    {
      i = 0;
      sprintf(s2, "%s%s%s", SWITCH, SWITCH, ptr_lopt->long_name);
      if (   with_larglist
          && (ptr_lopt->larg_mode != LARG_NO))
       {
         if (ptr_lopt->larg_mode == LARG_NO_OR_ONE)
          {
            strcat(s2, "[");
            strcat(s2, LARG_SEP);
          }
         else
           strcat(s2, LARG_SEP);
         if (   (ptr_lopt->larg_mode == LARG_ONE)
             && (ptr_lopt->largs[0] == NULL))
           strcat(s2, larg_lit);
         else
          {
            if (ptr_lopt->largs[0] == NULL)
              strcat(s2, larg_lit);
            else
              while (ptr_lopt->largs[i] != NULL)
               {
                 strcat(s2, ptr_lopt->largs[i]);
                 if (ptr_lopt->largs[i+1] != NULL)
                   strcat(s2, "|");
                 i++;
               }
          }
         if (ptr_lopt->larg_mode == LARG_NO_OR_ONE)
           strcat(s2, "]");
       }
    }
   else
     *s2 = '\0';

   return(s2);
}



   LOCAL void
my_copyright (fp, with_short_license)
         FILE *fp;
   const Bool  with_short_license;
/*
   Prints the program-id and copyright text to file `fp' using the central
     output function `print_text()' and uses global text buffer `s1' internally.
*/
{
   sprintf(s1, "%s (GNU cal) %s", prgr_name, VERSION_NO);
   print_text (fp, s1);
   if (!with_short_license)
     print_text (fp, s1);
   strcpy(s1, COPYRIGHT_TXT);
   print_text (fp, s1);
   if (with_short_license)
    {
#if USE_DE
      strcpy(s1, "Dies ist freie Software; in den Quellen befindet sich die Lizenz-");
      print_text (fp, s1);
      sprintf(s1, "und Kopierbedingung.  Es gibt KEINERLEI Garantie, nicht einmal f%sr", UE);
      print_text (fp, s1);
      strcpy(s1, "die TAUGLICHKEIT oder die VERWENDBARKEIT ZU EINEM ANGEGEBENEN ZWECK.");
#else /* !USE_DE */
      strcpy(s1, _("This is free software; see the source for copying conditions."));
      print_text (fp, s1);
      strcpy(s1, _("There is NO warranty, without even the implied warranty of"));
      print_text (fp, s1);
      strcpy(s1, _("MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE."));
#endif /* !USE_DE */
      print_text (fp, s1);
    }
   print_text (fp, s1);
}



   LOCAL void
my_help_head_text (fp)
   FILE *fp;
/*
   Prints the help header text to file `fp' using the central
     output function `print_text()' and uses global text buffer
     `s1' and `s2' internally.
*/
{
   auto Bool  print_hls=(Bool)(is_tty&&highlight_flag&&!emu_hls);


   *s1 = '\0';
   if (print_hls)
     strcpy(s1, ehls2s.seq);
   strcat(s1, prgr_name);
   if (print_hls)
     strcat(s1, ehls2e.seq);
#if USE_DE
   sprintf(s2, ":%*s Das GNU Kalenderprogramm  (", 6-len_prgr_name, "");
#else /* !USE_DE */
   sprintf(s2, _(":%*s The GNU calendar program  ("), 6-len_prgr_name, "");
#endif /* !USE_DE */
   strcat(s1, s2);
   if (print_hls)
     strcat(s1, ehls2s.seq);
   strcat(s1, VERSION_NO);
   if (print_hls)
     strcat(s1, ehls2e.seq);
   strcat(s1, ")  ");
   strcat(s1, COPYRIGHT_TXT);
   print_text (fp, s1);
   print_text (fp, s1);
   strcpy(s1, usage_msg ());
   print_text (fp, s1);
   print_text (fp, s1);
}



   LOCAL void
my_help_tail_text (fp)
   FILE *fp;
/*
   Prints the help tail text to file `fp' using the central
     output function `print_text()' and uses global text buffer
     `s1' internally.
*/
{
   auto Bool  print_hls=(Bool)(is_tty&&highlight_flag&&!emu_hls);


   print_text (fp, s1);
#if USE_DE
   sprintf(s1, "   +++  Bei zweistelliger Jahresangabe wird NICHT von %2d%s ausgegangen  +++",
           act_year/100, yy_lit);
   print_text (fp, s1);
   sprintf(s1, "   +++  `%s' ist freie Software, viel Spa%s damit   =8^)  +++", prgr_name, SZ);
#else /* !USE_DE */
   sprintf(s1, _("   +++  If you specify two digits for the year I DON'T assume %2d%s  +++"),
           act_year/100, yy_lit);
   print_text (fp, s1);
   sprintf(s1, _("   +++  `%s' is free software, enjoy   =8^)  +++"), prgr_name);
#endif /* !USE_DE */
   print_text (fp, s1);
   print_text (fp, s1);
   if (print_hls)
     strcat(s1, ehls2s.seq);
   strcat(s1, "------------------------oOO      \\\\\\_''/      OOo---------------------------");
   if (print_hls)
     strcat(s1, ehls2e.seq);
   print_text (fp, s1);
   strcpy(s1, "Thomas Esken               ");
   if (print_hls)
     strcat(s1, ehls2s.seq);
   strcat(s1, "O     (/");
   if (print_hls)
    {
      strcat(s1, ehls2e.seq);
      strcat(s1, ehls1s.seq);
    }
   strcat(s1, "o");
   if (print_hls)
    {
      strcat(s1, ehls1e.seq);
      strcat(s1, ehls2s.seq);
    }
   strcat(s1, "-");
   if (print_hls)
    {
      strcat(s1, ehls2e.seq);
      strcat(s1, ehls1s.seq);
    }
   strcat(s1, "o");
   if (print_hls)
    {
      strcat(s1, ehls1e.seq);
      strcat(s1, ehls2s.seq);
    }
   strcat(s1, "\\)     O");
   if (print_hls)
     strcat(s1, ehls2e.seq);
   strcat(s1, "  eMail: esken@uni-muenster.de");
   print_text (fp, s1);
   strcpy(s1, "Im Hagenfeld 84             ");
   if (print_hls)
     strcat(s1, ehls2s.seq);
   strcat(s1, "    ((  ^  ))    ");
   if (print_hls)
     strcat(s1, ehls2e.seq);
   strcat(s1, "   Phone: +49 251 232585");
   print_text (fp, s1);
   strcpy(s1, "D-48147 Muenster; Germany    ");
   if (print_hls)
     strcat(s1, ehls2s.seq);
   strcat(s1, "\\____) ~ (____/");
   if (print_hls)
     strcat(s1, ehls2e.seq);
   strcat(s1, "    MotD : 2old2live, 2young2die");
   print_text (fp, s1);
}



/*
   Writes an informational text to "fp" using the central output function
     showing the date, compiler and operating system used.  Uses global
     text buffer `s1' internally.  Code taken from `zip21' source package
     and adjusted to gcal's needs.
   Special thanks to the Info-ZIP group (`http://quest.jpl.nasa.gov/Info-ZIP/'),
     which merely granted the permission to use their `version_local()' function
     of `zip-2.1' in a modified way for Gcal (the `print_compiler_info()' function
     is based on copyrighted code by the Info-ZIP group).
*/
#if defined(MSDOS)
   LOCAL void
print_compiler_info (fp)
   FILE *fp;
{
#if defined(__DJGPP__) || defined(__WATCOMC__) || \
    (defined(_MSC_VER) && (_MSC_VER != 800))
   char buf[80];
#endif


    sprintf(s1, _(*compiler_info),

#ifdef __GNUC__
#  ifdef __DJGPP__
   (sprintf(buf, "djgpp v%d / gcc ", __DJGPP__), buf),
#  else
#    ifdef __GO32__           /* __GO32__ is defined as "1" only (sigh) */
   "djgpp v1.x / gcc ",
#    else
#      ifdef __EMX__            /* ...so is __EMX__ (double sigh) */
   "emx+gcc ",
#      else
   "gcc ",
#      endif
#    endif
#  endif
   __VERSION__,
#else
#  ifdef __WATCOMC__
#    if (__WATCOMC__ % 10 > 0)
/* We do this silly test because __WATCOMC__ gives two digits for the  */
/* minor version, but Watcom packaging prefers to show only one digit. */
   (sprintf(buf, "Watcom C/C++ %d.%02d", __WATCOMC__ / 100,
            __WATCOMC__ % 100), buf), "",
#    else
   (sprintf(buf, "Watcom C/C++ %d.%d", __WATCOMC__ / 100,
            (__WATCOMC__ % 100) / 10), buf), "",
#    endif
#  else
#    ifdef __TURBOC__
#      ifdef __BORLANDC__
   "Borland C++",
#        if (__BORLANDC__ < 0x0200)
   " 1.0",
#        else
#          if (__BORLANDC__ == 0x0200)   /* James:  __TURBOC__ = 0x0297 */
   " 2.0",
#          else
#            if (__BORLANDC__ == 0x0400)
   " 3.0",
#            else
#              if (__BORLANDC__ == 0x0410)   /* __BCPLUSPLUS__ = 0x0310 */
   " 3.1",
#              else
#                if (__BORLANDC__ == 0x0452)   /* __BCPLUSPLUS__ = 0x0320 */
#                  if USE_DE
   " 4.0 oder 4.02",
#                  else /* !USE_DE */
   _(" 4.0 or 4.02"),
#                  endif /* !USE_DE */
#                else
#                  if (__BORLANDC__ == 0x0460)   /* __BCPLUSPLUS__ = 0x0340 */
   " 4.5",
#                  else
#                    if (__BORLANDC__ == 0x0500)   /* __TURBOC__ = 0x0500 */
   " 5.0",
#                    else
#                      if USE_DE
   " neuer als 5.0",
#                      else /* !USE_DE */
   _(" later than 5.0"),
#                      endif /* !USE_DE */
#                    endif
#                  endif
#                endif
#              endif
#            endif
#          endif
#        endif
#      else
   "Turbo C",
#        if (__TURBOC__ >= 0x0400)     /* Kevin:  3.0 -> 0x0401 */
#          if USE_DE
   "++ 3.0 oder neuer",
#          else /* !USE_DE */
   _("++ 3.0 or later"),
#          endif /* !USE_DE */
#        else
#          if (__TURBOC__ == 0x0295)     /* [661] vfy'd by Kevin */
   "++ 1.0",
#          else
#            if ((__TURBOC__ >= 0x018d) && (__TURBOC__ <= 0x0200))  /* James: 0x0200 */
   " 2.0",
#            else
#              if (__TURBOC__ > 0x0100)
   " 1.5",                    /* James:  0x0105? */
#              else
   " 1.0",                    /* James:  0x0100 */
#              endif
#            endif
#          endif
#        endif
#      endif
#    else
#      ifdef MSC
   "Microsoft C ",
#        ifdef _MSC_VER
#          if (_MSC_VER == 800)
   "8.0/8.0c (Visual C++ 1.0/1.5)",
#          else
   (sprintf(buf, "%d.%02d", _MSC_VER/100, _MSC_VER%100), buf),
#          endif
#        else
#          if USE_DE
   "5.1 oder aelter",
#          else /* !USE_DE */
   _("5.1 or earlier"),
#          endif /* !USE_DE */
#        endif
#      else
#        if USE_DE
   "unbekannter Kompilierer", "",
#        else /* !USE_DE */
   _("unknown compiler"), "",
#        endif /* !USE_DE */
#      endif /* MSC */
#    endif /* __TURBOC__ */
#  endif /* __WATCOMC__ */
#endif /* __GNUC__ */

   "MS-DOS",

#if (defined(__GNUC__) || (defined(__WATCOMC__) && defined(__386__)))
   " (32-bit)",
#else
#  if defined(M_I86HM) || defined(__HUGE__)
   " (16-bit, huge)",
#  else
#    if defined(M_I86LM) || defined(__LARGE__)
   " (16-bit, large)",
#    else
#      if defined(M_I86MM) || defined(__MEDIUM__)
   " (16-bit, medium)",
#      else
#        if defined(M_I86CM) || defined(__COMPACT__)
   " (16-bit, compact)",
#        else
#          if defined(M_I86SM) || defined(__SMALL__)
   " (16-bit, small)",
#          else
#            if defined(M_I86TM) || defined(__TINY__)
   " (16-bit, tiny)",
#            else
   " (16-bit)",
#            endif
#          endif
#        endif
#      endif
#    endif
#  endif
#endif

#ifdef __DATE__
#  if USE_DE
   " am ", __DATE__
#  else /* !USE_DE */
   _(" on "), __DATE__
#  endif /* !USE_DE */
#else
   "", ""
#endif
   );

   print_text (fp, s1);
}
#else /* !MSDOS */
#  if defined(WIN32)
   LOCAL void
print_compiler_info (fp)
   FILE *fp;
{
#if (defined(_MSC_VER) || defined(__WATCOMC__))
   char buf[80];
#endif


   sprintf(s1, _(*compiler_info),

#ifdef _MSC_VER  /* MSC == MSVC++, including the SDK compiler */
   (sprintf(buf, "Microsoft C %d.%02d ", _MSC_VER/100, _MSC_VER%100), buf),
#  if (_MSC_VER == 800)
   "(Visual C++ v1.1)",
#  else
#    if (_MSC_VER == 850)
   "(Windows NT v3.5 SDK)",
#    else
#      if (_MSC_VER == 900)
   "(Visual C++ v2.0/v2.1)",
#      else
#        if (_MSC_VER == 1000)
   "(Visual C++ v4.0)",
#        else
#          if (_MSC_VER == 1010)
   "(Visual C++ v4.1)",
#          else
#            if (_MSC_VER > 800)
   "(Visual C++)",
#            else
#              if USE_DE
   "(fehlerhafte Version)",
#              else /* !USE_DE */
   _("(bad version)"),
#              endif /* !USE_DE */
#            endif
#          endif
#        endif
#      endif
#    endif
#  endif
#endif /* _MSC_VER */

#ifdef __WATCOMC__
#  if (__WATCOMC__ % 10 > 0)
/* We do this silly test because __WATCOMC__ gives two digits for the  */
/* minor version, but Watcom packaging prefers to show only one digit. */
   (sprintf(buf, "Watcom C/C++ %d.%02d", __WATCOMC__ / 100,
            __WATCOMC__ % 100), buf), "",
#  else
   (sprintf(buf, "Watcom C/C++ %d.%d", __WATCOMC__ / 100,
            (__WATCOMC__ % 100) / 10), buf), "",
#  endif /* __WATCOMC__ % 10 > 0 */
#endif /* __WATCOMC__ */

#ifdef __TURBOC__
#  ifdef __BORLANDC__
   "Borland C++",
#    if (__BORLANDC__ == 0x0452)   /* __BCPLUSPLUS__ = 0x0320 */
#      if USE_DE
   " 4.0 oder 4.02",
#      else /* !USE_DE */
   _(" 4.0 or 4.02"),
#      endif /* !USE_DE */
#    else
#      if (__BORLANDC__ == 0x0460)   /* __BCPLUSPLUS__ = 0x0340 */
   " 4.5",
#      else
#        if (__BORLANDC__ == 0x0500)   /* __TURBOC__ = 0x0500 */
   " 5.0",
#        else
#          if USE_DE
   " neuer als 5.0",
#          else /* !USE_DE */
   _(" later than 5.0"),
#          endif /* !USE_DE */
#        endif
#      endif
#    endif
#  else /* !__BORLANDC__ */
   "Turbo C",
#    if (__TURBOC__ >= 0x0400)     /* Kevin:  3.0 -> 0x0401 */
#      if USE_DE
   "++ 3.0 oder neuer",
#      else /* !USE_DE */
   _("++ 3.0 or later"),
#      endif /* !USE_DE */
#    else
#      if (__TURBOC__ == 0x0295)     /* [661] vfy'd by Kevin */
   "++ 1.0",
#      endif
#    endif
#  endif /* __BORLANDC__ */
#endif /* __TURBOC__ */

#if !defined(__TURBOC__) && !defined(__WATCOMC__) && !defined(_MSC_VER)
#  if USE_DE
   "unbekannter Kompilierer (SDK?)", "",
#  else /* !USE_DE */
   _("unknown compiler (SDK?)"), "",
#  endif /* !USE_DE */
#endif

   "\n\tWindows 95 / Windows NT", " (32-bit)",

#ifdef __DATE__
#  if USE_DE
   " am ", __DATE__
#  else /* !USE_DE */
   _(" on "), __DATE__
#  endif /* !USE_DE */
#else
   "", ""
#endif
   );

   print_text (fp, s1);
}
#  else /* !WIN32 */
#    if defined(OS2)
   LOCAL void
print_compiler_info (fp)
   FILE *fp;
{
#if defined(__IBMC__) || defined(__WATCOMC__) || defined(_MSC_VER)
   char buf[80];
#endif


   sprintf(s1, _(*compiler_info),

#ifdef __GNUC__
#  ifdef __EMX__  /* __EMX__ is defined as "1" only (sigh) */
   "emx+gcc ", __VERSION__,
#  else
   "gcc/2 ", __VERSION__,
#  endif
#else
#  ifdef __IBMC__
   "IBM ",
#    if (__IBMC__ < 200)
   (sprintf(buf, "C Set/2 %d.%02d", __IBMC__/100,__IBMC__%100), buf),
#    else
#      if (__IBMC__ < 300)
   (sprintf(buf, "C Set++ %d.%02d", __IBMC__/100,__IBMC__%100), buf),
#      else
   (sprintf(buf, "Visual Age C++ %d.%02d", __IBMC__/100,__IBMC__%100), buf),
#      endif
#    endif
#  else
#    ifdef __WATCOMC__
   "Watcom C", (sprintf(buf, " (__WATCOMC__ = %d)", __WATCOMC__), buf),
#    else
#      ifdef __TURBOC__
#        ifdef __BORLANDC__
   "Borland C++",
#          if (__BORLANDC__ < 0x0200)
   " 1.0",
#          else
#            if (__BORLANDC__ == 0x0200)
   " 2.0",
#            else
#              if (__BORLANDC__ == 0x0400)
   " 3.0",
#              else
#                if (__BORLANDC__ == 0x0410)
   " 3.1",
#                else
#                  if (__BORLANDC__ == 0x0452)
   " 4.0",
#                  else                    /* these two are guesses based on DOS version */
#                    if (__BORLANDC__ == 0x0460)
   " 4.5",
#                    else
#                      if USE_DE
   " neuer als 4.5",
#                      else /* !USE_DE */
   _(" later than 4.5"),
#                      endif /* !USE_DE */
#                    endif
#                  endif
#                endif
#              endif
#            endif
#          endif
#        else
   "Turbo C",
#          if (__TURBOC__ >= 661)
#            if USE_DE
   "++ 1.0 oder neuer",
#            else /* USE_DE */
   _("++ 1.0 or later"),
#            endif /* USE_DE */
#          else
#            if (__TURBOC__ == 661)
   " 3.0?",
#            else
#              if (__TURBOC__ == 397)
   " 2.0",
#              else
#                if USE_DE
   " 1.0 oder 1.5?",
#                else /* !USE_DE */
   _(" 1.0 or 1.5?"),
#                endif /* !USE_DE */
#              endif
#            endif
#          endif
#        endif
#      else
#        ifdef MSC
   "Microsoft C ",
#          ifdef _MSC_VER
   (sprintf(buf, "%d.%02d", _MSC_VER/100, _MSC_VER%100), buf),
#          else
#            if USE_DE
   "5.1 oder aelter",
#            else /* !USE_DE */
   _("5.1 or earlier"),
#            endif /* !USE_DE */
#          endif
#        else
#          if USE_DE
   "unbekannter Kompilierer", "",
#          else /* !USE_DE */
   _("unknown compiler"), "",
#          endif /* !USE_DE */
#        endif /* MSC */
#      endif /* __TURBOC__ */
#    endif /* __WATCOMC__ */
#  endif /* __IBMC__ */
#endif /* __GNUC__ */

   "OS/2",

/* GRR:  does IBM C/2 identify itself as IBM rather than Microsoft? */
#if (defined(MSC) || (defined(__WATCOMC__) && !defined(__386__)))
#  if defined(M_I86HM) || defined(__HUGE__)
   " (16-bit, huge)",
#  else
#    if defined(M_I86LM) || defined(__LARGE__)
   " (16-bit, large)",
#    else
#      if defined(M_I86MM) || defined(__MEDIUM__)
   " (16-bit, medium)",
#      else
#        if defined(M_I86CM) || defined(__COMPACT__)
   " (16-bit, compact)",
#        else
#          if defined(M_I86SM) || defined(__SMALL__)
   " (16-bit, small)",
#          else
#            if defined(M_I86TM) || defined(__TINY__)
   " (16-bit, tiny)",
#            else
   " (16-bit)",
#            endif
#          endif
#        endif
#      endif
#    endif
#  endif
#else
   " 2.x (32-bit)",
#endif

#ifdef __DATE__
#  if USE_DE
   " am ", __DATE__
#  else /* !USE_DE */
   _(" on "), __DATE__
#  endif /* !USE_DE */
#else
   "", ""
#endif
   );

   print_text (fp, s1);
}
#    else /* !OS2 */
#      if defined(ACORN)
   LOCAL void
print_compiler_info (fp)
   FILE *fp;
{
   sprintf(s1, _(*compiler_info),

#ifdef __GNUC__
   "gcc ", __VERSION__,
#else
#  ifdef __CC_NORCROFT
   "Norcroft ", "cc",
#  else
   "cc", "",
#  endif
#endif

   "RISC OS",

   " (Acorn Computers Ltd)",

#ifdef __DATE__
#  if USE_DE
   " am ", __DATE__
#  else /* !USE_DE */
   _(" on "), __DATE__
#  endif /* !USE_DE */
#else
   "", ""
#endif
   );

   print_text (fp, s1);
}
#      else /* !ACORN */
#        if defined(AMIGA)

/* NOTE:  the following include depends upon the environment
 *        variable $Workbench to be set correctly.  (Set by
 *        default, by kickstart during startup)
 */
int WBversion = (int)
#include "ENV:Workbench"
;

   LOCAL void
print_compiler_info (fp)
   FILE *fp;
{
/* Define buffers. */

   char buf1[16];  /* compiler name */
   char buf2[16];  /* revstamp */
   char buf3[16];  /* OS */
   char buf4[16];  /* Date */

/* format "with" name strings */

#ifdef __SASC
   strcpy(buf1, "SAS/C ");
#else
#  ifdef LATTICE
   strcpy(buf1, "Lattice C ");
#  else
#    ifdef AZTEC_C
   strcpy(buf1, "Manx Aztec C ");
#    else
#      if USE_DE
   strcpy(buf1, "unbekannt ");
#      else /* !USE_DE */
   strcpy(buf1, _("unknown "));
#      endif /* !USE_DE */
#    endif
#  endif
#endif
/* "under" */
   sprintf(buf3, "AmigaDOS v%d", WBversion);

/* Define revision, date, and time strings.
 * NOTE:  Do not calculate run time, be sure to use time compiled.
 * Pass these strings via your makefile if undefined.
 */

#if defined(__VERSION__) && defined(__REVISION__)
#  if USE_DE
   sprintf(buf2, "Version %d.%d", __VERSION__, __REVISION__);
#  else /* !USE_DE */
   sprintf(buf2, _("version %d.%d"), __VERSION__, __REVISION__);
#  endif /* !USE_DE */
#else
#  ifdef __VERSION__
#    if USE_DE
   sprintf(buf2, "Version %d", __VERSION__);
#    else /* !USE_DE */
   sprintf(buf2, _("version %d"), __VERSION__);
#    endif /* !USE_DE */
#  else
#    if USE_DE
   sprintf(buf2, "unbekannte Version");
#    else /* !USE_DE */
   sprintf(buf2, _("unknown version"));
#    endif /* !USE_DE */
#  endif
#endif

#ifdef __DATE__
#  if USE_DE
   sprintf(buf4, " am %s", __DATE__);
#  else /* !USE_DE */
   sprintf(buf4, _(" on %s"), __DATE__);
#  endif /* !USE_DE */
#else
#  if USE_DE
   strcpy(buf4, " unbekanntes Datum");
#  else /* !USE_DE */
   strcpy(buf4, _(" unknown date"));
#  endif /* !USE_DE */
#endif

   sprintf(s1, _(*compiler_info), buf1, buf2, buf3, buf4, "", "");

   print_text (fp, s1);
}
#        else /* !AMIGA */
#          if defined(ATARI)
   LOCAL void
print_compiler_info (fp)
   FILE *fp;
{
#ifdef __TURBOC__
   char buf[40];
#endif

   sprintf(s1, _(*compiler_info),

#ifdef __GNUC__
   "gcc ", __VERSION__,
#else
#  if 0
#    if USE_DE
   "cc ", (sprintf(buf, "Version %d", _RELEASE), buf),
#    else /* !USE_DE */
   "cc ", (sprintf(buf, _("version %d"), _RELEASE), buf),
#    endif /* !USE_DE */
#  else
#    ifdef __TURBOC__
   "Turbo C", (sprintf(buf, " (0x%04x = %d)", __TURBOC__, __TURBOC__), buf),
#    else
#      if USE_DE
   "unbekannter Kompilierer", "",
#      else /* !USE_DE */
   _("unknown compiler"), "",
#      endif /* !USE_DE */
#    endif
#  endif
#endif

#ifdef __MINT__
   "Atari TOS/MiNT",
#else
   "Atari TOS",
#endif

   " (Atari ST/TT/Falcon030)",

#ifdef __DATE__
#  if USE_DE
   " am ", __DATE__
#  else /* !USE_DE */
   _(" on "), __DATE__
#  endif /* !USE_DE */
#else
   "", ""
#endif
   );

   print_text (fp, s1);
}
#          else /* !ATARI */
#            if defined(__50SERIES)   /* Prime/PRIMOS */
   LOCAL void
print_compiler_info (fp)
   FILE *fp;
{
   sprintf(s1, _(*compiler_info),

#ifdef __GNUC__
   "gcc ", __VERSION__,
#else
   "cc", "",
#endif

   "PRIMOS",

   " (Prime Computer Inc)",

#ifdef __DATE__
#  if USE_DE
   " am ", __DATE__
#  else /* !USE_DE */
   _(" on "), __DATE__
#  endif /* !USE_DE */
#else
   "", ""
#endif
   );

   print_text (fp, s1);
}
#            else /* !__50SERIES */
#              if defined(VAXC) || defined(VMS)   /* DEC Vax */
   LOCAL void
print_compiler_info (fp)
   FILE *fp;
{
#ifdef VMS_VERSION
   char buf[40];
#endif
#ifdef __DECC_VER
   char buf2[40];
   int  vtyp;
#endif

/*  DEC C in ANSI mode does not like "#ifdef MACRO" inside another
    macro when MACRO is equated to a value (by "#define MACRO 1").   */

   sprintf(s1, _(*compiler_info),

#ifdef __GNUC__
   "gcc ", __VERSION__,
#else
#  if defined(DECC) || defined(__DECC) || defined (__DECC__)
   "DEC C",
#    ifdef __DECC_VER
   (sprintf(buf2, " %c%d.%d-%03d",
            ((vtyp = (__DECC_VER / 10000) % 10) == 6 ? 'T' :
             (vtyp == 8 ? 'S' : 'V')),
            __DECC_VER / 10000000,
            (__DECC_VER % 10000000) / 100000, __DECC_VER % 1000), buf2),
#    else
   "",
#    endif
#  else
#    ifdef VAXC
   "VAX C", "",
#    else
#      if USE_DE
   "unbekannter Kompilierer", "",
#      else /* !USE_DE */
   _("unknown compiler"), "",
#      endif /* !USE_DE */
#    endif
#  endif
#endif

#ifdef VMS_VERSION
#  if defined(__alpha)
   "OpenVMS",   /* version has trailing spaces ("V6.1   "), so truncate: */
#    if USE_DE
   (sprintf(buf, " (%.4s f%sr Alpha)", VMS_VERSION), buf, UE),
#    else /* !USE_DE */
   (sprintf(buf, _(" (%.4s for Alpha)"), VMS_VERSION), buf),
#    endif /* !USE_DE */
#  else /* VAX */
   (VMS_VERSION[1] >= '6')? "OpenVMS" : "VMS",
#    if USE_DE
   (sprintf(buf, " (%.4s f%sr VAX)", VMS_VERSION), buf, UE),
#    else /* !USE_DE */
   (sprintf(buf, _(" (%.4s for VAX)"), VMS_VERSION), buf),
#    endif /* !USE_DE */
#  endif
#else
   "VMS", "",
#endif /* ?VMS_VERSION */

#ifdef __DATE__
#  if USE_DE
   " am ", __DATE__
#  else /* !USE_DE */
   _(" on "), __DATE__
#  endif /* !USE_DE */
#else
   "", ""
#endif
   );

   print_text (fp, s1);
}
#              else /* default: UNIX */
#if defined(__NetBSD__) || defined(__FreeBSD__) || defined(__386BSD__) || \
    defined(__bsdi__)
#include <sys/param.h> /* for the BSD4_4 define */
#endif

   LOCAL void
print_compiler_info (fp)
   FILE *fp;
{
#if defined(CRAY) || defined(NX_CURRENT_COMPILER_RELEASE) || defined(NetBSD)
   char buf1[40];
#  if defined(CRAY) || defined(NX_CURRENT_COMPILER_RELEASE)
   char buf2[40];
#  endif
#endif

    /* Pyramid, NeXT have problems with huge macro expansion, too:  no Info() */
    sprintf(s1, _(*compiler_info),

#ifdef __GNUC__
#  ifdef NX_CURRENT_COMPILER_RELEASE
   (sprintf(buf1, "NeXT DevKit %d.%02d ", NX_CURRENT_COMPILER_RELEASE/100,
    NX_CURRENT_COMPILER_RELEASE%100), buf1),
    (strlen(__VERSION__) > 8)? "(gcc)" :
     (sprintf(buf2, "(gcc %s)", __VERSION__), buf2),
#  else
   "gcc ", __VERSION__,
#  endif
#else
#  if defined(CRAY) && defined(_RELEASE)
#    if USE_DE
   "cc ", (sprintf(buf1, "Version %d", _RELEASE), buf1),
#    else /* !USE_DE */
   "cc ", (sprintf(buf1, _("version %d"), _RELEASE), buf1),
#    endif /* !USE_DE */
#  else
#    ifdef __VERSION__
   "cc ", __VERSION__,
#    else
   "cc", "",
#    endif
#  endif
#endif

   "Unix",

#if defined(sgi) || defined(__sgi)
   " (Silicon Graphics IRIX)",
#else
#  ifdef sun
#    ifdef sparc
#      ifdef __SVR4
   " (Sun Sparc/Solaris)",
#      else /* may or may not be SunOS */
   " (Sun Sparc)",
#      endif
#    else
#      if defined(sun386) || defined(i386)
   " (Sun 386i)",
#      else
#        if defined(mc68020) || defined(__mc68020__)
   " (Sun 3)",
#        else /* mc68010 or mc68000:  Sun 2 or earlier */
   " (Sun 2)",
#        endif
#      endif
#    endif
#  else
#    ifdef __hpux
   " (HP/UX)",
#    else
#      ifdef __osf__
   " (DEC OSF/1)",
#      else
#        ifdef _AIX
   " (IBM AIX)",
#        else
#          ifdef aiws
   " (IBM RT/AIX)",
#          else
#            if defined(CRAY) || defined(cray)
#              ifdef _UNICOS
   (sprintf(buf2, " (Cray UNICOS release %d)", _UNICOS), buf2),
#              else
   " (Cray UNICOS)",
#              endif
#            else
#              if defined(uts) || defined(UTS)
   " (Amdahl UTS)",
#              else
#                ifdef NeXT
#                  ifdef mc68000
   " (NeXTStep/black)",
#                  else
   " (NeXTStep for Intel)",
#                  endif
#                else              /* the next dozen or so are somewhat order-dependent */
#                  if defined(linux) || defined(__linux__)
#                    ifdef __ELF__
   " (Linux ELF)",
#                    else
   " (Linux a.out)",
#                    endif
#                  else
#                    ifdef MINIX
   " (Minix)",
#                    else
#                      ifdef M_UNIX
   " (SCO Unix)",
#                      else
#                        ifdef M_XENIX
   " (SCO Xenix)",
#                        else
#                          ifdef __NetBSD__
#                            ifdef NetBSD0_8
#                              if NetBSD0_8 == 1
   " (NetBSD 0.8)",
#                              else
   (sprintf(buf1, " (NetBSD 0.8%c)", (char) (NetBSD0_8 + '@')), buf1),
#                              endif
#                            else
#                              ifdef NetBSD0_9
#                                if NetBSD0_9 == 1
   " (NetBSD 0.9)",
#                                else
   (sprintf(buf1, " (NetBSD 0.9%c)", (char)(NetBSD0_9 + 'A' - 2)), buf1),
#                                endif
#                              else
#                                ifdef NetBSD1_0
#                                  if NetBSD1_0 == 1
   " (NetBSD 1.0)",
#                                  else
   (sprintf(buf1, " (NetBSD 1.0%c)", (char)(NetBSD1_0 + 'A' - 2)), buf1),
#                                  endif
#                                else
#                                  ifdef NetBSD1_1
#                                    if NetBSD1_1 == 1
   " (NetBSD 1.1)",
#                                    else
   (sprintf(buf1, " (NetBSD 1.1%c)", (char)(NetBSD1_1 + 'A' - 2)), buf1),
#                                    endif
#                                  else
#                                    if USE_DE
   (BSD4_4 == 0.5)? " (NetBSD vor 0.9)" : " (NetBSD 1.2 oder neuer)",
#                                    else /* !USE_DE */
   (BSD4_4 == 0.5)? _(" (NetBSD before 0.9)") : _(" (NetBSD 1.2 or later)"),
#                                    endif /* !USE_DE */
#                                  endif
#                                endif
#                              endif
#                            endif
#                          else
#                            ifdef __FreeBSD__
#                              if USE_DE
   (BSD4_4 == 0.5)? " (FreeBSD 1.x)" : " (FreeBSD 2.0 oder neuer)",
#                              else /* !USE_DE */
   (BSD4_4 == 0.5)? " (FreeBSD 1.x)" : _(" (FreeBSD 2.0 or later)"),
#                              endif /* !USE_DE */
#                            else
#                              ifdef __bsdi__
#                                if USE_DE
   (BSD4_4 == 0.5)? " (BSD/386 1.0)" : " (BSD/386 1.1 oder neuer)",
#                                else /* !USE_DE */
   (BSD4_4 == 0.5)? " (BSD/386 1.0)" : _(" (BSD/386 1.1 or later)"),
#                                endif /* !USE_DE */
#                              else
#                                ifdef __386BSD__
   (BSD4_4 == 1)? " (386BSD, post-4.4 release)" : " (386BSD)",
#                                else
#                                  if defined(i486) || defined(__i486) || defined(__i486__)
   " (Intel 486)",
#                                  else
#                                    if defined(i386) || defined(__i386) || defined(__i386__)
   " (Intel 386)",
#                                    else
#                                      ifdef pyr
   " (Pyramid)",
#                                      else
#                                        if defined(ultrix) || defined(__ultrix)
#                                          if defined(mips) || defined(__mips)
   " (DEC/MIPS)",
#                                          else
#                                            if defined(vax) || defined(__vax)
   " (DEC/VAX)",
#                                            else /* __alpha? */
   " (DEC/Alpha)",
#                                            endif
#                                          endif
#                                        else
#                                          ifdef gould
   " (Gould)",
#                                          else
#                                            ifdef MTS
   " (MTS)",
#                                            else
#                                              ifdef __convexc__
   " (Convex)",
#                                              else
   "",
#                                              endif /* Convex */
#                                            endif /* MTS */
#                                          endif /* Gould */
#                                        endif /* DEC */
#                                      endif /* Pyramid */
#                                    endif /* 386 */
#                                  endif /* 486 */
#                                endif /* 386BSD */
#                              endif /* BSDI BSD/386 */
#                            endif /* NetBSD */
#                          endif /* FreeBSD */
#                        endif /* SCO Xenix */
#                      endif /* SCO Unix */
#                    endif /* Minix */
#                  endif /* Linux */
#                endif /* NeXT */
#              endif /* Amdahl */
#            endif /* Cray */
#          endif /* RT/AIX */
#        endif /* AIX */
#      endif /* OSF/1 */
#    endif /* HP/UX */
#  endif /* Sun */
#endif /* SGI */

#ifdef __DATE__
#  if USE_DE
   " am ", __DATE__
#  else /* !USE_DE */
   _(" on "), __DATE__
#  endif /* !USE_DE */
#else
   "", ""
#endif
   );

   print_text (fp, s1);
}
#              endif /* default: UNIX */
#            endif /* !__50SERIES */
#          endif /* !ATARI */
#        endif /* !AMIGA */
#      endif /* !ACORN */
#    endif /* !OS2 */
#  endif /* !WIN32 */
#endif /* !MSDOS */
