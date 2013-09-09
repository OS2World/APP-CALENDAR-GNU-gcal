/*
*  rc-utils.c:  Pool of special functions necessary for managing the fixed dates.
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
*  Include definition header file to see whether USE_RC is defined there.
*    Compile this module only if USE_RC is defined, otherwise skip it.
*/
#include "tailor.h"



#if USE_RC
#  ifdef RCSID
static char rcsid[]="$Id: rc-utils.c 2.40 1997/04/19 02:04:00 tom Exp $";
#  endif



/*
*  Include header files.
*/
#  if HAVE_CTYPE_H
#    include <ctype.h>
#  endif
#  if HAVE_MATH_H && HAVE_LIBM
#    include <math.h>
#  endif
#  include "gcal.h"



/*
*  Function prototypes.
*/
#  if __cplusplus
extern "C"
{
#  endif
/*
************************************************** Defined in `help.c'.
*/
IMPORT char *
usage_msg __P_((void));
IMPORT void
put_longopt_description __P_((FILE *fp));
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
IMPORT void
resize_all_strings __P_((const int   amount,
                         const Bool  with_line_buffer,
                         const char *module_name,
                         const long  module_line));
IMPORT void
my_error __P_((const int   exit_status,
               const char *module_name,
               const long  module_line,
               const char *var_name,
               const int   var_contents));
IMPORT void
my_exit __P_((const int exit_status));
IMPORT int
my_atoi __P_((const char *s));
IMPORT int
compare_d_m_name __P_((const char       *s,
                       const Cmode_enum  mode));
IMPORT const char *
month_name __P_((const int month));
IMPORT Ulint
date2num __P_((const int day,
               const int month,
               const int year));
IMPORT Bool
doy2date __P_((      int  doy,
               const int  is_leap_year,
                     int *day,
                     int *month));
IMPORT int
weekday_of_date __P_((const int day,
                      const int month,
                      const int year));
IMPORT int
day_of_year __P_((const int day,
                  const int month,
                  const int year));
IMPORT int
days_of_february __P_((const int year));
IMPORT Bool
valid_date __P_((const int day,
                 const int month,
                 const int year));
IMPORT int
weekno2doy __P_((      int week,
                 const int year));
IMPORT int
knuth_easter_formula __P_((const int year));
/*
************************************************** Defined in `rc-utils.c'.
*/
#  if HAVE_V8_REGCOMP
EXPORT void
regerror __P_((char *msg));
#  endif
EXPORT Bool
rc_valid_day __P_((const char *date_text,
                   const int   day,
                   const int   month,
                   const int   year));
EXPORT Bool
rc_valid_period __P_((      char *date_text,
                      const int   d,
                      const int   m,
                      const int   y,
                      const int   incr_year,
                      const int   decr_year));
EXPORT void
rc_clean_flags __P_((void));
EXPORT Line_struct *
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
EXPORT Bool
precomp_nth_wd __P_((      int         diff,
                     const int         wd,
                           int        *n,
                           int        *day,
                           int        *month,
                           int        *year,
                     const Cmode_enum  mode));
EXPORT Bool
precomp_date __P_((      int         diff,
                   const int         wd,
                         int        *day,
                         int        *month,
                   const int         year,
                   const Cmode_enum  mode));
EXPORT Bool
set_dvar __P_((const char        *line_buffer,
                     Line_struct *lineptrs,
               const char        *filename,
               const long         line_number,
               const Dvar_enum    mode));
EXPORT Bool
set_tvar __P_((const char      *line_buffer,
               const Tvar_enum  mode));
EXPORT void
nth_weekday_of_month __P_((      int  *d,
                                 int  *m,
                                 int  *y,
                           const int  *n,
                                 Bool *is_weekday_mode));
EXPORT Bool
prev_date __P_((int *day,
                int *month,
                int *year));
EXPORT Bool
next_date __P_((int *day,
                int *month,
                int *year));
EXPORT void
num2date __P_((Ulint  julian_days,
               int   *day,
               int   *month,
               int   *year));
EXPORT Slint
d_between __P_((const int d1,
                const int m1,
                const int y1,
                const int d2,
                const int m2,
                const int y2));
EXPORT Slint
w_between __P_((const int d1,
                const int m1,
                const int y1,
                const int d2,
                const int m2,
                const int y2));
EXPORT Slint
m_between __P_((const int m1,
                const int y1,
                const int m2,
                const int y2));
EXPORT void
manage_leap_day __P_((      int  *day,
                            int  *month,
                            int   year,
                      const char *line_buffer,
                      const char *filename,
                      const long  line_number));
EXPORT char *
biorhythm __P_((const Bool  create_bar,
                      int   axis_len,
                      char *string,
                const int   day,
                const int   month,
                const int   year,
                const int   birth_day,
                const int   birth_month,
                const int   birth_year,
                const char *emo_text,
                      int  *emo_phase,
                      int  *emo_waxes,
                const char *int_text,
                      int  *int_phase,
                      int  *int_waxes,
                const char *phy_text,
                      int  *phy_phase,
                      int  *phy_waxes,
                      int  *critical_day,
                      int  *positive_day,
                      int  *negative_day));
#  if HAVE_LIBM
LOCAL double
kepler __P_((      double m,
             const double ecc));
LOCAL double
phase __P_((const double julian_date));
EXPORT int
the_phase __P_((      Bool *is_full_new,
                const int   day,
                const int   month,
                const int   year,
                const int   hour,
                const int   min));
#  else /* !HAVE_LIBM */
LOCAL double
moonphase __P_((const int day,
                const int month,
                const int year,
                const int hour,
                const int min));
EXPORT int
the_moonphase __P_((      Bool *is_full_new,
                          int   day,
                          int   month,
                          int   year,
                    const int   hour,
                    const int   min));
#  endif /* !HAVE_LIBM */
EXPORT void
draw_moon __P_((const int    age,
                const int    lines,
                      char **string));
LOCAL int
moon_charpos __P_((const double x,
                   const int    lines));
#  if !HAVE_LIBM
LOCAL Ulint
my_fac __P_((Slint n));
LOCAL double
my_pow __P_((double x,
             int    n));
LOCAL double
my_sqrt __P_((double x));
LOCAL double
my_sin __P_((double x));
LOCAL double
my_cos __P_((double x));
#  endif /* !HAVE_LIBM */
LOCAL void
dvar_warning __P_((const int   exit_status,
                   const int   dvar,
                   const char *line_buffer,
                   const char *filename,
                   const long  line_number));
#  if __cplusplus
}
#  endif



/*
*  Declare public(extern) variables.
*/
IMPORT const int    dvec[];                  /* Amount of days in months */
IMPORT const int    mvec[];                  /* Number of past days of month */
IMPORT Greg_struct *greg;                    /* Points to the used Gregorian Reformation date */
IMPORT Dvar_struct  rc_dvar[];               /* Date variables a[=`mmdd']...z[] (`yyyy'@{a|b|...|z}[[-]<n>]) */
IMPORT Tvar_struct  rc_tvar[];               /* Text variables $a[=TEXT]...$z[] */
IMPORT Line_struct *lptrs3;                  /* Pointers to different parts of a (resource file) line */
IMPORT Uint         maxlen_max;              /* Actual size of all string vectors */
IMPORT int          len_year_max;            /* String length of the maximum year able to compute */
IMPORT int          warning_level;           /* --debug[=0...WARN_LVL_MAX] */
IMPORT int          start_day;               /* -s<0,1...7|day name> */
IMPORT int          day;                     /* Current day */
IMPORT int          month;                   /* Current month */
IMPORT int          year;                    /* Current year */
IMPORT int          act_day;                 /* Actual day */
IMPORT int          act_month;               /* Actual month */
IMPORT int          fiscal_month;            /* Starting month of a fiscal year */
IMPORT int          is_leap_year;            /* Is current year a leap year? */
IMPORT char         hd_ldays[];              /* Vector of holiday dates (legal days) */
IMPORT char         hd_mdays[];              /* Vector of holiday dates (memorial days) */
IMPORT char        *prgr_name;               /* Stores the actual program name */
IMPORT char        *s5;                      /* General purpose text buffer */
IMPORT char        *rc_filter_text;          /* REGEX used for filtering fixed date */
IMPORT Bool         rc_period_flag;          /* [-c]<<<<n>>[<d|w|+|-]>|`mmdd'|`mmww[w]'<n>> */
IMPORT Bool         rc_period_list;          /* [-c]l */
IMPORT Bool         rc_tomorrow_flag;        /* [-c]t */
IMPORT Bool         rc_week_flag;            /* [-c]w */
IMPORT Bool         rc_month_flag;           /* [-c]m */
IMPORT Bool         rc_year_flag;            /* [-c]y */
IMPORT Bool         rc_week_year_flag;       /* [-c<<n>>]w */
IMPORT Bool         rc_forwards_flag;        /* [-c<<n>|w|m|y>]+ */
IMPORT Bool         rc_backwards_flag;       /* [-c<<n>|w|m|y>]- */
IMPORT Bool         rc_feb_29_to_feb_28;     /* `--leap-day=february' given */
IMPORT Bool         rc_feb_29_to_mar_01;     /* `--leap-day=march' given */
IMPORT Bool         is_3month_mode;          /* Argument is `.' or `.+' or `.-' */
IMPORT Bool         is_3month_mode2;         /* Argument is `..' -> current quarter of actual year */
IMPORT Bool         adate_set;               /* [-c]<n>w and actual date modified */



#  if HAVE_V8_REGCOMP
   PUBLIC void
regerror (msg)
   char *msg;
/*
   Enables terminating error message in case an illegal V8-REGEX pattern is specified.
*/
{
   my_error (104, msg, 0L, rc_filter_text, 0);
}
#  endif /* HAVE_V8_REGCOMP */



   PUBLIC Bool
rc_valid_day (date_text, day, month, year)
   const char *date_text;
   const int   day;
   const int   month;
   const int   year;
/*
   Checks the `date_text' for "%? special texts without argument", which
     disables a fixed dates and which is stored in `date_text' without a
     leading '%' character, and stores them into maps.  The `date_text'
     has to be proofed by the caller!  If the date given in `day', `month'
     and `year' is marked in the maps, we know that this date must be
     excluded so this function returns FALSE, otherwise TRUE.
*/
{
   register       int    i;
   register       int    wd=weekday_of_date (day, month, year);
   auto     const char  *ptr_date_text=date_text;
   auto           Bool   hd_found;
   static         Bool   inclusive_weekday_map[DAY_MAX+1];
   static         Bool   exclusive_weekday_map[DAY_MAX+1];


   for (i=1 ; i <= DAY_MAX ; i++)
     inclusive_weekday_map[i] = !(exclusive_weekday_map[i] = TRUE);
   *inclusive_weekday_map = *exclusive_weekday_map = FALSE;
   while (*ptr_date_text)
    {
      hd_found = FALSE;
      switch (*ptr_date_text)
       {
         case RC_EX_LHDY_CHAR:
         case RC_EX_AHDY_CHAR:
           /*
              %exclude_legal_holiday special text or
              %exclude_all_holiday special text found.
           */
           if (hd_ldays[((month-1)*MONTH_LAST)+(day-1)])
             hd_found = TRUE;
           if (   hd_found
               || *ptr_date_text == RC_EX_LHDY_CHAR)
            {
              if (hd_found)
               {
                 exclusive_weekday_map[wd] = FALSE;
                 *exclusive_weekday_map = TRUE;
               }
              break;
            }
           if (hd_mdays[((month-1)*MONTH_LAST)+(day-1)])
            {
              exclusive_weekday_map[wd] = FALSE;
              *exclusive_weekday_map = TRUE;
              break;
            }
           break;
         case RC_EX_NLHDY_CHAR:
         case RC_EX_NAHDY_CHAR:
           /*
              %exclude_no_legal_holiday special text or
              %exclude_no_all_holiday special text found.
           */
           if (hd_ldays[((month-1)*MONTH_LAST)+(day-1)])
            {
              hd_found = TRUE;
              break;
            }
           if (   hd_found
               || *ptr_date_text == RC_EX_NLHDY_CHAR)
            {
              if (hd_found)
                inclusive_weekday_map[wd] = TRUE;
              *inclusive_weekday_map = TRUE;
              break;
            }
           if (hd_mdays[((month-1)*MONTH_LAST)+(day-1)])
            {
              hd_found = TRUE;
              break;
            }
           if (hd_found)
             inclusive_weekday_map[wd] = TRUE;
           *inclusive_weekday_map = TRUE;
           break;
         case RC_EX_MON_CHAR:
           /*
              %exclude_monday special text found.
           */
           exclusive_weekday_map[1] = FALSE;
           *exclusive_weekday_map = TRUE;
           break;
         case RC_EX_NMON_CHAR:
           /*
              %exclude_no_monday special text found.
           */
           *inclusive_weekday_map=inclusive_weekday_map[1] = TRUE;
           break;
         case RC_EX_TUE_CHAR:
           /*
              %exclude_tuesday special text found.
           */
           exclusive_weekday_map[2] = FALSE;
           *exclusive_weekday_map = TRUE;
           break;
         case RC_EX_NTUE_CHAR:
           /*
              %exclude_no_tuesday special text found.
           */
           *inclusive_weekday_map=inclusive_weekday_map[2] = TRUE;
           break;
         case RC_EX_WED_CHAR:
           /*
              %exclude_wednesday special text found.
           */
           exclusive_weekday_map[3] = FALSE;
           *exclusive_weekday_map = TRUE;
           break;
         case RC_EX_NWED_CHAR:
           /*
              %exclude_no_wednesday special text found.
           */
           *inclusive_weekday_map=inclusive_weekday_map[3] = TRUE;
           break;
         case RC_EX_THU_CHAR:
           /*
              %exclude_thursday special text found.
           */
           exclusive_weekday_map[4] = FALSE;
           *exclusive_weekday_map = TRUE;
           break;
         case RC_EX_NTHU_CHAR:
           /*
              %exclude_no_thursday special text found.
           */
           *inclusive_weekday_map=inclusive_weekday_map[4] = TRUE;
           break;
         case RC_EX_FRI_CHAR:
           /*
              %exclude_friday special text found.
           */
           exclusive_weekday_map[5] = FALSE;
           *exclusive_weekday_map = TRUE;
           break;
         case RC_EX_NFRI_CHAR:
           /*
              %exclude_no_friday special text found.
           */
           *inclusive_weekday_map=inclusive_weekday_map[5] = TRUE;
           break;
         case RC_EX_SAT_CHAR:
           /*
              %exclude_saturday special text found.
           */
           exclusive_weekday_map[6] = FALSE;
           *exclusive_weekday_map = TRUE;
           break;
         case RC_EX_NSAT_CHAR:
           /*
              %exclude_no_saturday special text found.
           */
           *inclusive_weekday_map=inclusive_weekday_map[6] = TRUE;
           break;
         case RC_EX_SUN_CHAR:
           /*
              %exclude_sunday special text found.
           */
           exclusive_weekday_map[7] = FALSE;
           *exclusive_weekday_map = TRUE;
           break;
         case RC_EX_NSUN_CHAR:
           /*
              %exclude_no_sunday special text found.
           */
           *inclusive_weekday_map=inclusive_weekday_map[7] = TRUE;
           break;
         case RC_EX_MON_2_THU_CHAR:
           /*
              %exclude_monday_to_thursday special text found.
           */
           for (i=DAY_MIN ; i <= 4 ; i++)
             exclusive_weekday_map[i] = FALSE;
           *exclusive_weekday_map = TRUE;
           break;
         case RC_EX_NMON_2_THU_CHAR:
           /*
              %exclude_no_monday_to_thursday special text found.
           */
           for (i=0 ; i <= 4 ; i++)
             inclusive_weekday_map[i] = TRUE;
           break;
         case RC_EX_MON_2_FRI_CHAR:
           /*
              %exclude_monday_to_friday special text found.
           */
           for (i=DAY_MIN ; i <= 5 ; i++)
             exclusive_weekday_map[i] = FALSE;
           *exclusive_weekday_map = TRUE;
           break;
         case RC_EX_NMON_2_FRI_CHAR:
           /*
              %exclude_no_monday_to_friday special text found.
           */
           for (i=0 ; i <= 5 ; i++)
             inclusive_weekday_map[i] = TRUE;
           break;
         default:
           /*
              This case MUST be an internal error!
           */
           abort();
       }
      ptr_date_text++;
    }
   /*
      Check whether a weekday to exclude is marked in the maps and
        if so, avoid displaying the fixed date entry.
   */
   if (   *inclusive_weekday_map
       || *exclusive_weekday_map)
    {
      if (   *inclusive_weekday_map
          && *exclusive_weekday_map)
       {
         if (   !inclusive_weekday_map[wd]
             || !exclusive_weekday_map[wd])
           return(FALSE);
       }
      else
        if (*inclusive_weekday_map)
         {
           if (!inclusive_weekday_map[wd])
             return(FALSE);
         }
        else
          if (!exclusive_weekday_map[wd])
            return(FALSE);
    }

   return(TRUE);
}



   PUBLIC Bool
rc_valid_period (date_text, d, m, y, incr_year, decr_year)
         char *date_text;
   const int   d;
   const int   m;
   const int   y;
   const int   incr_year;
   const int   decr_year;
/*
   Checks the `date_text' for "%? special texts with date argument", which
     disables a fixed dates and which is stored in `date_text' without a
     leading '%' character, but separated by ',' colon characters, and stores
     them into maps.  The `date_text' has to be proofed by the caller!
     If `the_day', `the_month' and `the_year' is marked there, we know
     that this date must be excluded so this function returns FALSE,
     otherwise TRUE.
*/
{
   auto     Slint  num;
   register int    i;
   static   Bool   inclusive_date_map[DAY_LAST+2];
   static   Bool   exclusive_date_map[DAY_LAST+2];
   auto     int    len;
   auto     int    rlen;
   auto     int    dd;
   auto     int    rdd;
   auto     int    mm;
   auto     int    rmm;
   auto     int    yy;
   auto     int    ryy;
   auto     int    nn;
   auto     int    rnn;
   auto     int    hhn;
   auto     int    rhn;
   auto     int    hhwd;
   auto     int    rhwd;
   auto     char  *ptr_date_text=date_text;
   auto     char  *ptr_char;
   auto     char   special_text_char;
   auto     char   ch;
   auto     char   hhc;
   auto     char   rhc;
   auto     Bool   is_weekday_mode;
   auto     Bool   ris_weekday_mode;
   auto     Bool   is_range;
   auto     Bool   dflt_yy_set;
   auto     Bool   dflt_ryy_set;


   /*
      Initialize the tables.
   */
   for (i=1 ; i < DAY_LAST+2 ; i++)
     inclusive_date_map[i] = !(exclusive_date_map[i] = TRUE);
   *inclusive_date_map = *exclusive_date_map = FALSE;
   while (*ptr_date_text)
    {
      /*
         Check if a range of dates is given.
      */
      rhc = '\0';
      rlen=rdd=rmm=ryy=rnn = 0;
      is_range=dflt_yy_set=dflt_ryy_set = FALSE;
      special_text_char = *ptr_date_text++;
      ptr_char = ptr_date_text;
      while (   *ptr_date_text
             && (*ptr_date_text != *SPLIT_SEP)
             && (*ptr_date_text != RC_DRANGE_CHAR))
        ptr_date_text++;
      if (*ptr_date_text == RC_DRANGE_CHAR)
        is_range = TRUE;
      ch = *ptr_date_text;
      *ptr_date_text = '\0';
#  if USE_DE
      (void)rc_get_date (ptr_char, lptrs3, FALSE, &is_weekday_mode, &dd, &mm, &yy, &nn,
                         &len, &hhc, &hhn, &hhwd, INTERNAL_TXT, -1L, date_text, FALSE);
#  else /* !USE_DE */
      (void)rc_get_date (ptr_char, lptrs3, FALSE, &is_weekday_mode, &dd, &mm, &yy, &nn,
                         &len, &hhc, &hhn, &hhwd, _("Internal"), -1L, date_text, FALSE);
#  endif /* !USE_DE */
      /*
         Error, invalid date encoded.
      */
      if (yy == SPECIAL_VALUE)
       {
#  if USE_DE
         fprintf(stderr, "%s: ung%sltiges Datum angegeben -- %s\n%s\n",
                 prgr_name, UE, date_text, usage_msg ());
#  else /* !USE_DE */
         fprintf(stderr, _("%s: invalid date given -- %s\n%s\n"),
                 prgr_name, date_text, usage_msg ());
#  endif /* !USE_DE */
         put_longopt_description (stderr);
         S_NEWLINE(stderr);
         my_exit (126);
       }
      *ptr_date_text = ch;
      if (is_range)
       {
         ptr_char = ++ptr_date_text;
         while (   *ptr_date_text
                && (*ptr_date_text != *SPLIT_SEP))
           ptr_date_text++;
         ch = *ptr_date_text;
         *ptr_date_text = '\0';
#  if USE_DE
         (void)rc_get_date (ptr_char, lptrs3, FALSE, &ris_weekday_mode, &rdd, &rmm, &ryy, &rnn,
                            &rlen, &rhc, &rhn, &rhwd, INTERNAL_TXT, -1L, date_text, FALSE);
#  else /* !USE_DE */
         (void)rc_get_date (ptr_char, lptrs3, FALSE, &ris_weekday_mode, &rdd, &rmm, &ryy, &rnn,
                            &rlen, &rhc, &rhn, &rhwd, _("Internal"), -1L, date_text, FALSE);
#  endif /* !USE_DE */
         if (ryy == SPECIAL_VALUE)
          {
#  if USE_DE
            fprintf(stderr, "%s: ung%sltiges Datum angegeben -- %s\n%s\n",
                    prgr_name, UE, date_text, usage_msg ());
#  else /* !USE_DE */
            fprintf(stderr, _("%s: invalid date given -- %s\n%s\n"),
                    prgr_name, date_text, usage_msg ());
#  endif /* !USE_DE */
            put_longopt_description (stderr);
            S_NEWLINE(stderr);
            my_exit (126);
          }
         *ptr_date_text = ch;
       }
      if (ch)
        ptr_date_text++;
      if (!len)
        dflt_yy_set = TRUE;
      if (!rlen)
        dflt_ryy_set = TRUE;
      if (!yy)
       {
         if (dflt_yy_set)
           yy = YEAR_MIN;
         else
          {
            yy = year;
            if (   y
                && (fiscal_month > MONTH_MIN))
              yy = y;
          }
       }
      if (!ryy)
       {
         if (dflt_ryy_set)
           ryy = YEAR_MAX;
         else
          {
            ryy = year;
            if (   y
                && (fiscal_month > MONTH_MIN))
              ryy = y;
          }
       }
      /*
         Respect possible fiscal year.
      */
      if (   !dflt_yy_set
          && (yy != SPECIAL_VALUE))
       {
         yy -= incr_year;
         yy += decr_year;
       }
      if (   !dflt_ryy_set
          && (ryy != SPECIAL_VALUE))
       {
         ryy -= incr_year;
         ryy += decr_year;
       }
      /*
         If @... "date"-part is given, compute the according date.
      */
      switch (hhc)
       {
         case RC_EASTER_CHAR:
         case RC_TODAY_CHAR:
           if (   !dflt_yy_set
               && (fiscal_month > MONTH_MIN))
            {
              if (!precomp_date (hhn, hhwd, &dd, &mm, yy+incr_year,
                                 (hhc == RC_EASTER_CHAR) ? EAster : TOday))
                yy = SPECIAL_VALUE;
            }
           else
             if (!precomp_date (hhn, hhwd, &dd, &mm, yy,
                                (hhc == RC_EASTER_CHAR) ? EAster : TOday))
               yy = SPECIAL_VALUE;
           break;
         case 'D':
         case 'W':
           if (   !dflt_yy_set
               && (fiscal_month > MONTH_MIN))
            {
              auto int  fiscal_year=yy+incr_year;


              if (!precomp_nth_wd (hhn, hhwd, &hhn, &dd, &mm, &fiscal_year,
                                   (hhc == 'D') ? DAy : WEek))
                yy = fiscal_year;
            }
           else
             (void)precomp_nth_wd (hhn, hhwd, &hhn, &dd, &mm, &yy,
                                   (hhc == 'D') ? DAy : WEek);
           break;
         default:
           if (islower(hhc))
            {
              if (rc_dvar[IDX(hhc)].l.month)
               {
                 mm = (int)rc_dvar[IDX(hhc)].l.month;
                 dd = (int)rc_dvar[IDX(hhc)].l.day;
               }
              else
                if (rc_dvar[IDX(hhc)].g.month)
                 {
                   mm = (int)rc_dvar[IDX(hhc)].g.month;
                   dd = (int)rc_dvar[IDX(hhc)].g.day;
                 }
              if (   !dflt_yy_set
                  && (fiscal_month > MONTH_MIN))
               {
                 if (!precomp_date (hhn, hhwd, &dd, &mm, yy+incr_year, DVar))
                   yy = SPECIAL_VALUE;
               }
              else
                if (!precomp_date (hhn, hhwd, &dd, &mm, yy, DVar))
                  yy = SPECIAL_VALUE;
            }
       }
      switch (rhc)
       {
         case RC_EASTER_CHAR:
         case RC_TODAY_CHAR:
           if (   !dflt_ryy_set
               && (fiscal_month > MONTH_MIN))
            {
              if (!precomp_date (rhn, rhwd, &rdd, &rmm, ryy+incr_year,
                                 (rhc == RC_EASTER_CHAR) ? EAster : TOday))
                ryy = SPECIAL_VALUE;
            }
           else
             if (!precomp_date (rhn, rhwd, &rdd, &rmm, ryy,
                                (rhc == RC_EASTER_CHAR) ? EAster : TOday))
               ryy = SPECIAL_VALUE;
           break;
         case 'D':
         case 'W':
           if (   !dflt_ryy_set
               && (fiscal_month > MONTH_MIN))
            {
              auto int  fiscal_year=ryy+incr_year;


              if (!precomp_nth_wd (rhn, rhwd, &rhn, &rdd, &rmm, &fiscal_year,
                                   (rhc == 'D') ? DAy : WEek))
                ryy = fiscal_year;
            }
           else
             (void)precomp_nth_wd (rhn, rhwd, &rhn, &rdd, &rmm, &ryy,
                                   (rhc == 'D') ? DAy : WEek);
           break;
         default:
           if (islower(rhc))
            {
              if (rc_dvar[IDX(rhc)].l.month)
               {
                 rmm = (int)rc_dvar[IDX(rhc)].l.month;
                 rdd = (int)rc_dvar[IDX(rhc)].l.day;
               }
              else
                if (rc_dvar[IDX(rhc)].g.month)
                 {
                   rmm = (int)rc_dvar[IDX(rhc)].g.month;
                   rdd = (int)rc_dvar[IDX(rhc)].g.day;
                 }
              if (   !dflt_ryy_set
                  && (fiscal_month > MONTH_MIN))
               {
                 if (!precomp_date (rhn, rhwd, &rdd, &rmm, ryy+incr_year, DVar))
                   ryy = SPECIAL_VALUE;
               }
              else
                if (!precomp_date (rhn, rhwd, &rdd, &rmm, ryy, DVar))
                  ryy = SPECIAL_VALUE;
            }
       }
      if (len > len_year_max)
        len -= len_year_max;
      else
        len = 0;
      if (rlen > len_year_max)
        rlen -= len_year_max;
      else
        rlen = 0;
      /*
         Assume current/first month of year.
      */
      if (!mm)
       {
         if (len >= 1)
          {
            mm = m;
            if (len == 2)
              len = 0;
            else
              len--;
          }
         else
           mm = MONTH_MIN;
       }
      else
       {
         if (len == 2)
           len = 0;
         else
           len--;
       }
      /*
         Assume current/first day of month.
      */
      if (!dd)
       {
         if (len >= 1)
           dd = d;
         else
           dd = DAY_MIN;
       }
      /*
         Assume current/last month of year.
      */
      if (!rmm)
       {
         if (rlen >= 1)
          {
            rmm = m;
            if (rlen == 2)
              rlen = 0;
            else
              rlen--;
          }
         else
           rmm = MONTH_MAX;
       }
      else
       {
         if (rlen == 2)
           rlen = 0;
         else
           rlen--;
       }
      /*
         Assume current/last day of month.
      */
      if (!rdd)
       {
         if (rlen >= 1)
           rdd = d;
         else
          {
            if (rmm == 2)
             {
               if (   !dflt_ryy_set
                   && (fiscal_month > MONTH_MIN))
                 rdd = days_of_february (ryy+incr_year);
               else
                 rdd = days_of_february (ryy);
             }
            else
              rdd = dvec[rmm-1];
          }
       }
      /*
         If special value "99" for day `dd' is given,
           set the day to last day of month.
      */
      if (dd == 99)
       {
         /*
            Assume the last day of month.
         */
         if (mm == 2)
           dd = days_of_february (yy);
         else
           dd = dvec[mm-1];
       }
      if (rdd == 99)
       {
         /*
            Assume the last day of month.
         */
         if (rmm == 2)
          {
            if (   !dflt_ryy_set
                && (fiscal_month > MONTH_MIN))
              rdd = days_of_february (ryy+incr_year);
            else
              rdd = days_of_february (ryy);
          }
         else
           rdd = dvec[rmm-1];
       }
      /*
         If "n'th weekday of month" entry set, compute the according date.
      */
      if (nn)
        nth_weekday_of_month (&dd, &mm, &yy, &nn, &is_weekday_mode);
      if (rnn)
        nth_weekday_of_month (&rdd, &rmm, &ryy, &rnn, &ris_weekday_mode);
      /*
         Proceed if (optionally specified) date is valid.
      */
      if (   (   !is_range
              && (yy != SPECIAL_VALUE))
          || (   is_range
              && (yy != SPECIAL_VALUE)
              && (ryy != SPECIAL_VALUE)))
       {
         register int  true_year=(y) ? y : year+incr_year;


         if (   !nn
             && !dflt_yy_set
             && (   fiscal_month > MONTH_MIN
                 || (   incr_year
                     && (   rc_tomorrow_flag
                         || rc_week_flag))))
           yy += incr_year;
         if (   !rnn
             && !dflt_ryy_set
             && (   fiscal_month > MONTH_MIN
                 || (   incr_year
                     && (   rc_tomorrow_flag
                         || rc_week_flag))))
           ryy += incr_year;
         /*
            If starting date of event not greater than ending
              date of event, mark the period in according map,
              otherwise ignore the %... special text completely.
         */
         num = d_between (dd, mm, yy, rdd, rmm, ryy);
         if (num >= 0L)
          {
            register int  s_doy=DAY_MIN;
            register int  e_doy=DAY_LAST+1;


            if (special_text_char == RC_IDATE_CHAR)
              *inclusive_date_map = TRUE;
            else
              *exclusive_date_map = TRUE;
            if (yy == true_year)
              s_doy = day_of_year (dd, mm, yy);
            else
              if (yy > true_year)
                s_doy = SPECIAL_VALUE;
            if (ryy == true_year)
              e_doy = day_of_year (rdd, rmm, ryy);
            else
              if (ryy < true_year)
                e_doy = SPECIAL_VALUE;
            if (   (s_doy != SPECIAL_VALUE)
                && (e_doy != SPECIAL_VALUE))
             {
               if (special_text_char == RC_IDATE_CHAR)
                 for (i=s_doy ; i <= e_doy ; i++)
                   inclusive_date_map[i] = TRUE;
               else
                 for (i=s_doy ; i <= e_doy ; i++)
                   exclusive_date_map[i] = FALSE;
             }
          }
       }
    }
   /*
      Check whether a period to exclude is marked in the maps and
        if so, avoid displaying the fixed date entry.
   */
   if (   *inclusive_date_map
       || *exclusive_date_map)
    {
      i = day_of_year (d, m, year+incr_year-decr_year);
      if (   *inclusive_date_map
          && *exclusive_date_map)
       {
         if (   !inclusive_date_map[i]
             || !exclusive_date_map[i])
           return(FALSE);
       }
      else
        if (*inclusive_date_map)
         {
           if (!inclusive_date_map[i])
             return(FALSE);
         }
        else
          if (!exclusive_date_map[i])
            return(FALSE);
    }

   return(TRUE);
}



   PUBLIC void
rc_clean_flags ()
/*
   Cleans all global flags (except `rc_period_list')
     which are related to the fixed date period.
*/
{
   rc_tomorrow_flag=rc_week_flag=rc_month_flag=rc_year_flag
   =rc_week_year_flag=rc_forwards_flag=rc_backwards_flag=rc_period_flag = FALSE;
}



   PUBLIC Line_struct *
rc_get_date (the_line, lineptrs, is_rc_file, is_weekday_mode, d, m, y, n, len, hc, hn, hwd,
             filename, line_number, line_buffer, on_error_exit)
         char        *the_line;
         Line_struct *lineptrs;
   const Bool         is_rc_file;
         Bool        *is_weekday_mode;
         int         *d;
         int         *m;
         int         *y;
         int         *n;
         int         *len;
         char        *hc;
         int         *hn;
         int         *hwd;
   const char        *filename;
   const long         line_number;
   const char        *line_buffer;
   const Bool         on_error_exit;
/*
   Converts the textual/string `date' of a RC-file line to a numerical date
     and returns a pointer struct to the "day"-part and the "text"-part of the
     line indicating whether the "day"-part contains a list or a range of days;
     a char pointer to the "repeat"-field and to the "appears"-field if these
     exists, and/or if a @... or *... day is encoded in "date"-part and year
     is set to zero in the line, then this function returns holiday_mode_char
     (==date variable) or upper case characters 'D' or 'W' in &hc, the day
     displacement in &hn and a possible weekday name (mo...su) converted to a
     number (1...7) in &hwd for further managing of such a line.  If any invalid
     date is given in line, then this function either returns SPECIAL_VALUE in &y or leaves
     the complete program with an error message (depending on mode of operation
     resp., contents of `on_error_exit' variable).
*/
{
   register int    num_of_range_chars=0;
   register int    num_of_repeat_chars=0;
   register int    num_of_appears_chars=0;
   register int    i;
   static   char   str7[8];   /* For "date"-parts, length of 7 chars+'\0' maximum! */
   auto     char  *ptr_char;
   auto     Bool   is_hdy_mode=FALSE;


   *hc = '\0';
   lineptrs->day_list=lineptrs->day_range = FALSE;
   lineptrs->repeat_part=lineptrs->appears_part = (char *)NULL;
   (*len)=(*hn)=(*hwd)=(*n)=i = 0;
   /*
      Get the year from the year field of the line.
   */
   while (   *the_line
          && !isspace(*the_line)
          && isdigit(*the_line)
          && (i < len_year_max))
     str7[i++] = *the_line++;
   str7[i] = '\0';
   *y = my_atoi (str7);
   *len = i;
   /*
      Get the month from the month field of the line.
   */
   i = 0;
   while (   *the_line
          && !isspace(*the_line)
          && (i < 2))
     str7[i++] = *the_line++;
   if (i)
     /*
        Try to get a short (3 character) textual month name.
     */
     if (   isalpha(*the_line)
         && (   isupper(str7[i-1])
#  if USE_EASC
             || str7[i-1] == *AE
             || str7[i-1] == *OE
             || str7[i-1] == *UE
             || str7[i-1] == *AAE
             || str7[i-1] == *OOE
             || str7[i-1] == *UUE
#  else /* !USE_EASC */
             || str7[i-1] == '"'
#  endif /* !USE_EASC */
             || islower(str7[i-1])))
       str7[i++] = *the_line++;
   str7[i] = '\0';
   *m = my_atoi (str7);
   if (!*m)
     /*
        Check for short (3 character) textual month name.
     */
     *m = compare_d_m_name (str7, MOnth);
   else
     if (   i == 3
         || (   (i == 2)
             && (!isdigit(str7[1]))))
      {
        /*
           Error, invalid month field.
        */
        if (on_error_exit)
          my_error (122, filename, line_number, line_buffer, *m);
        *y = SPECIAL_VALUE;
      }
   /*
      Check if @... date variable statement or *... statement is given.
   */
   if (i)
    {
      *len += i;
      if (*str7 == RC_HDY_CHAR)
       {
         is_hdy_mode = TRUE;
         if (i == 2)
           *hc = (char)tolower(str7[1]);
       }
      else
        if (*str7 == RC_NWD_CHAR)
         {
           is_hdy_mode = TRUE;
           if (   (i == 2)
               && (   toupper(str7[1]) == 'D'
                   || toupper(str7[1]) == 'W'))
             *hc = (char)toupper(str7[1]);
           else
            {
              if (i == 2)
                /*
                   Error, invalid mode specifying character given.
                */
                *hc = (char)toupper(str7[1]);
              else
                /*
                   Error, no mode specifying character given.
                */
                *hc = *str7;
            }
         }
    }
   /*
      If the special value "99" for a month `m' is given,
        set the month to 12 (december).
   */
   if (*m == 99)
     *m = MONTH_MAX;
   if (   !is_hdy_mode
       && (   *m > MONTH_MAX
           || (   !*m
               && (   (   (i == 1)
                       && !isdigit(*str7))
                   || (   (i == 2)
                       && (   !isdigit(*str7)
                           || !isdigit(str7[1])))
                   || (   (i == 3)
                       && (   !isdigit(*str7)
                           || !isdigit(str7[1])
                           || !isdigit(str7[2])))))))
    {
      /*
         Error, invalid month field given.
      */
      if (on_error_exit)
        my_error (122, filename, line_number, line_buffer, *m);
      *y = SPECIAL_VALUE;
    }
   /*
      Get the day (maximum 3 characters in this case, template is either `dd', `ww'  or `www')
        resp., @... date variable or *... statement (maximum 7 characters in this case,
        template is: [+|-]nnn`www').
   */
   ptr_char=lineptrs->day_part = the_line;
   i = 0;
   while (   *the_line
          && !isspace(*the_line)
          && (i < ((is_hdy_mode) ? 7 : 3)))
     str7[i++] = *the_line++;
   str7[i] = '\0';
   *d = atoi(str7);
   *len += i;
   *is_weekday_mode = FALSE;
   the_line--;
   if (   isalpha(*the_line)
       || (   (i < 3)
           && !is_hdy_mode))
     the_line++;
   /*
      Check for a list/range of days/textual day names,
        if such a list is found, let `lineptrs->day_part' point to it
        and return to caller for further managing this list/range.
   */
   while (   *ptr_char
          && !isspace(*ptr_char))
    {
      if (*ptr_char == RC_DLIST_CHAR)
        lineptrs->day_list = TRUE;
      else
        if (*ptr_char == RC_DRANGE_CHAR)
         {
           num_of_range_chars++;
           lineptrs->day_range = TRUE;
         }
        else
          if (*ptr_char == RC_REPEAT_CHAR)
           {
             num_of_repeat_chars++;
             lineptrs->repeat_part = ptr_char;
           }
          else
            if (*ptr_char == RC_APPEARS_CHAR)
             {
               num_of_appears_chars++;
               lineptrs->appears_part = ptr_char;
             }
      ptr_char++;
    }
   if (   lineptrs->day_list
       || lineptrs->day_range)
    {
      if (is_rc_file)
       {
         if (   (   num_of_range_chars > 1
                 || *ptr_char == RC_DLIST_CHAR
                 || *ptr_char == RC_DRANGE_CHAR
                 || (   lineptrs->day_list
                     && lineptrs->day_range)
                 || (   !lineptrs->day_list
                     && !lineptrs->day_range
                     && (   num_of_repeat_chars > 1
                         || num_of_appears_chars > 1)))
             && on_error_exit)
           /*
              Error, invalid list/range of days.
           */
           my_error (123, filename, line_number, line_buffer, 0);
         /*
            Check if a day variable is referenced.
         */
         if (   islower(*hc)
             && (*hc != RC_EASTER_CHAR)
             && (*hc != RC_TODAY_CHAR))
          {
            /*
               Try to assign a local date variable if there is set any,
                 else try to assign a global date variable if there is set any,
                 otherwise we have to skip this part.
            */
            if (   rc_dvar[IDX(*hc)].l.month
                || rc_dvar[IDX(*hc)].g.month)
             {
               if (rc_dvar[IDX(*hc)].l.month)
                {
                  *m = (int)rc_dvar[IDX(*hc)].l.month;
                  *d = (int)rc_dvar[IDX(*hc)].l.day;
                }
               else
                {
                  *m = (int)rc_dvar[IDX(*hc)].g.month;
                  *d = (int)rc_dvar[IDX(*hc)].g.day;
                }
             }
            else
             {
               /*
                  Error, no such date variable defined.
               */
               if (   (warning_level >= 0)
                   && on_error_exit)
                 dvar_warning (113, (int)*hc, line_buffer, filename, line_number);
               *y = SPECIAL_VALUE;
             }
          }
         if (!isalpha(str7[i-1]))
           (*len)--;
         i = 0;
         while (   *the_line
                && !isspace(*the_line))
          {
            the_line++;
            i++;
          }
         *len += i;
       }
      else
       {
         /*
            Error, list/range of days is given in an expression it may not occur.
         */
         if (on_error_exit)
           my_error (123, filename, line_number, line_buffer, 0);
         *y = SPECIAL_VALUE;
       }
    }
   else
    {
      if (   !is_rc_file
          && (   num_of_repeat_chars
              || num_of_appears_chars))
       {
         /*
            Error, day "repeat" or "appears" coding is given in an expression
              it may not occur.
         */
         if (on_error_exit)
           my_error (123, filename, line_number, line_buffer, 0);
         *y = SPECIAL_VALUE;
       }
      else
        if (   num_of_repeat_chars > 1
            || num_of_appears_chars > 1)
         {
           /*
              Error, "repeat" or "appears" coding given twice or more.
           */
           if (on_error_exit)
             my_error (123, filename, line_number, line_buffer, 0);
           *y = SPECIAL_VALUE;
         }
      lineptrs->day_part = (char *)NULL;
    }
   /*
      If no list/range of days is given, try to precompute the according date.
   */
   if (lineptrs->day_part == (char *)NULL)
    {
      if (!is_hdy_mode)
       {
         /*
            Check for simple textual day name (either two or three characters),
              template `ww' or `www'.
         */
         if (!*d)
          {
            if (*str7)
              *d = compare_d_m_name (str7, DAy);
            if (*d)
             {
               *is_weekday_mode = TRUE;
               if (isdigit(str7[i-1]))
                 (*len)--;
             }
            else
             {
               i = 0;
               while (isdigit(str7[i]))
                 i++;
               if (str7[i])
                {
                  /*
                     Error, invalid day field.
                  */
                  if (on_error_exit)
                    my_error (121, filename, line_number, line_buffer, *d);
                  *y = SPECIAL_VALUE;
                }
             }
          }
         else
           if (   (i > 1)
               && !isdigit(str7[1]))
            {
              /*
                 Error, invalid day field.
              */
              if (on_error_exit)
                my_error (121, filename, line_number, line_buffer, *d);
              *y = SPECIAL_VALUE;
            }
         /*
            Check whether a "n'th weekday of month" field exists.
         */
         if (   *the_line
             && !isspace(*the_line))
          {
            if (isdigit(*the_line))
             {
               *n = CHR2DIG(*the_line);
               if (*n)
                {
                  if (   (*n > 5)
                      && (*n < 9))
                   {
                     /*
                        Error, invalid "n'th weekday of month" field.
                     */
                     if (on_error_exit)
                       my_error (117, filename, line_number, line_buffer, *n);
                     *y = SPECIAL_VALUE;
                   }
                }
             }
            else
              if (   (lineptrs->repeat_part == (char *)NULL)
                  && (lineptrs->appears_part == (char *)NULL))
               {
                 /*
                    Error, missing separator between "date"-part and "text"-part.
                 */
                 if (on_error_exit)
                   my_error (116, filename, line_number, line_buffer, 0);
                 *y = SPECIAL_VALUE;
               }
            if (*the_line)
              the_line++;
            if (   *the_line
                && !isspace(*the_line)
                && (lineptrs->repeat_part == (char *)NULL)
                && (lineptrs->appears_part == (char *)NULL))
             {
               /*
                  Error, missing separator between "date"-part and "text"-part.
               */
               if (on_error_exit)
                 my_error (116, filename, line_number, line_buffer, 0);
               *y = SPECIAL_VALUE;
             }
            if (   *n
                && (   *d < DAY_MIN
                    || *d > DAY_MAX))
             {
               /*
                  Error, "n'th weekday of month" entry set but invalid day encoded.
               */
               if (on_error_exit)
                 my_error (121, filename, line_number, line_buffer, *d);
               *y = SPECIAL_VALUE;
             }
            (*len)++;
            if (   lineptrs->repeat_part != (char *)NULL
                || lineptrs->appears_part != (char *)NULL)
              while (   *the_line
                     && !isspace(*the_line))
               {
                 the_line++;
                 (*len)++;
               }
          }
       }
      else
       {
         if (isdigit(*the_line))
           the_line++;
         if (   *the_line
             && !isspace(*the_line)
             && (lineptrs->repeat_part == (char *)NULL)
             && (lineptrs->appears_part == (char *)NULL))
          {
            /*
               Error, missing separator character between "date"-part and "text"-part.
            */
            if (on_error_exit)
              my_error (116, filename, line_number, line_buffer, 0);
            *y = SPECIAL_VALUE;
          }
         /*
            Compute the base date of '@' date variable "date"-part of line
              or '*' n'th weekday of year/weekday `ww[w]' of n'th week
              in case an explicit year `yyyy' is given in "date"-part.
         */
         i = atoi(str7);
         ptr_char = str7;
         if (islower(*hc))
          {
            if (   *ptr_char == *ASC_LIT
                || *ptr_char == *DES_LIT)
              ptr_char++;
            if (   *ptr_char == *ASC_LIT
                || *ptr_char == *DES_LIT
                || isalpha(*ptr_char))
             {
               /*
                  Error, simple weekday name or invalid sign given.
               */
               if (on_error_exit)
                 my_error (123, filename, line_number, line_buffer, 0);
               *hc = '\0';
               *d = 0;
               *y = SPECIAL_VALUE;
             }
          }
         else
           if (   *ptr_char == *ASC_LIT
               || *ptr_char == *DES_LIT)
            {
              /*
                 Error, invalid sign given.
              */
              if (on_error_exit)
                my_error (123, filename, line_number, line_buffer, 0);
              *hc = '\0';
              *d = 0;
              *y = SPECIAL_VALUE;
            }
         /*
            Now eat all digits.
         */
         while (isdigit(*ptr_char))
           ptr_char++;
         if (   *ptr_char
             && (*ptr_char != RC_REPEAT_CHAR)
             && (*ptr_char != RC_APPEARS_CHAR))
          {
            *hwd = compare_d_m_name (ptr_char, DAy);
            if (!*hwd)
             {
               /*
                  Error, invalid textual short day name given.
               */
               if (on_error_exit)
                 my_error (123, filename, line_number, line_buffer, 0);
               *hc = '\0';
               *d = 0;
               *y = SPECIAL_VALUE;
             }
          }
         if (*y >= 0)
          {
            if (   *hc == RC_EASTER_CHAR
                || *hc == RC_TODAY_CHAR)
             {
               if (!precomp_date (i, *hwd, d, m, *y,
                                  (*hc == RC_EASTER_CHAR) ? EAster : TOday))
                {
                  if (!*y)
                   {
                     /*
                        No explicit year `yyyy' given in "date"-part of line.
                     */
                     *hn = i;
                     if (*hc == RC_EASTER_CHAR)
                      {
                        *d = 0;
                        *m = 0;
                      }
                   }
                  else
                   {
                     /*
                        Invalid relative date given.
                     */
                     *hc = '\0';
                     *d = 0;
                     *y = SPECIAL_VALUE;
                   }
                }
               else
                 *hc = '\0';
             }
            else
              if (islower(*hc))
               {
                 /*
                    Try to assign a local date variable if there is set any,
                      else try to assign a global date variable if there is set any,
                      otherwise we have to skip this part.
                 */
                 if (   rc_dvar[IDX(*hc)].l.month
                     || rc_dvar[IDX(*hc)].g.month)
                  {
                    if (rc_dvar[IDX(*hc)].l.month)
                     {
                       *m = (int)rc_dvar[IDX(*hc)].l.month;
                       *d = (int)rc_dvar[IDX(*hc)].l.day;
                     }
                    else
                     {
                       *m = (int)rc_dvar[IDX(*hc)].g.month;
                       *d = (int)rc_dvar[IDX(*hc)].g.day;
                     }
                    if (!precomp_date (i, *hwd, d, m, *y, DVar))
                     {
                       if (!*y)
                         /*
                            No explicit year `yyyy' given in "date"-part of line.
                         */
                         *hn = i;
                       else
                        {
                          /*
                             Invalid relative date given.
                          */
                          *hc = '\0';
                          *d = 0;
                          *y = SPECIAL_VALUE;
                        }
                     }
                    else
                      *hc = '\0';
                  }
                 else
                  {
                    /*
                       Error, no such date variable defined.
                    */
                    if (   (warning_level >= 0)
                        && on_error_exit)
                      dvar_warning (113, (int)*hc, line_buffer, filename, line_number);
                    *hc = '\0';
                    *d = 0;
                    *y = SPECIAL_VALUE;
                  }
               }
              else
                if (   *hc == 'D'
                    || *hc == 'W')
                 {
                   /*
                      Try to compute the '*' n'th weekday of year resp.,
                        weekday `ww[w]' of n'th week statement.
                   */
                   if (*y == 0)
                    {
                      /*
                         No explicit year `yyyy' given in "date"-part of line.
                      */
                      *hn = i;
                      *d = 0;
                      *m = 0;
                    }
                   else
                     if (precomp_nth_wd (i, *hwd, hn, d, m, y,
                                         (*hc == 'D') ? DAy : WEek))
                       *hc = '\0';
                 }
                else
                  /*
                     Error, either an invalid date variable character trails the holiday
                       mode character '@', or an invalid character trails the "n'th weekday
                       of year" resp., weekday `ww[w]' of "n'th week mode" character '*'.
                  */
                  if (on_error_exit)
                    my_error (123, filename, line_number, line_buffer, 0);
          }
         if (   lineptrs->repeat_part != (char *)NULL
             || lineptrs->appears_part != (char *)NULL)
           while (   *the_line
                  && !isspace(*the_line))
            {
              the_line++;
              (*len)++;
            }
         if (*the_line)
           the_line++;
       }
    }
   /*
      Now let's allocate memory for all pointers to texts of the `lineptrs' structure
        if we work on a resource/include file (except `text_part').  That's absolutely
        necessary because after a potential resizing of "all strings" elsewhere in a later
        part of the program, these pointers could get lost otherwise.  The caller has to
        free this memory!
   */
   if (is_rc_file)
    {
      /*
         ONLY IF DETECTED!
      */
      if (lineptrs->day_part != (char *)NULL)
       {
         ptr_char = lineptrs->day_part;
         i = 0;
         LOOP
          {
            if (   !*ptr_char
                || isspace(*ptr_char))
              break;
            i++;
            ptr_char++;
          }
         ptr_char = lineptrs->day_part;
         lineptrs->day_part = (char *)my_malloc (i+1,
                                                 124, __FILE__, ((long)__LINE__)-1,
                                                 "lineptrs->day_part", 0);
         strncpy(lineptrs->day_part, ptr_char, i);
         lineptrs->day_part[i] = '\0';
       }
      /*
         ONLY IF DETECTED!
      */
      if (lineptrs->repeat_part != (char *)NULL)
       {
         ptr_char = lineptrs->repeat_part;
         i = 0;
         LOOP
          {
            if (   !*ptr_char
                || isspace(*ptr_char))
              break;
            i++;
            ptr_char++;
          }
         ptr_char = lineptrs->repeat_part;
         lineptrs->repeat_part = (char *)my_malloc (i+1,
                                                    124, __FILE__, ((long)__LINE__)-1,
                                                    "lineptrs->repeat_part", 0);
         strncpy(lineptrs->repeat_part, ptr_char, i);
         lineptrs->repeat_part[i] = '\0';
       }
      /*
         ONLY IF DETECTED!
      */
      if (lineptrs->appears_part != (char *)NULL)
       {
         ptr_char = lineptrs->appears_part;
         i = 0;
         LOOP
          {
            if (   !*ptr_char
                || isspace(*ptr_char))
              break;
            i++;
            ptr_char++;
          }
         ptr_char = lineptrs->appears_part;
         lineptrs->appears_part = (char *)my_malloc (i+1,
                                                     124, __FILE__, ((long)__LINE__)-1,
                                                     "lineptrs->appears_part", 0);
         strncpy(lineptrs->appears_part, ptr_char, i);
         lineptrs->appears_part[i] = '\0';
       }
      if (   (   lineptrs->repeat_part != (char *)NULL
              || lineptrs->appears_part != (char *)NULL)
          && !is_hdy_mode
          && !*is_weekday_mode
          && !lineptrs->day_list
          && !lineptrs->day_range)
        (*len)--;
    }
   /*
      ALWAYS!
   */
   lineptrs->text_part = the_line;

   return(lineptrs);
}



   PUBLIC Bool
precomp_nth_wd (diff, wd, n, day, month, year, mode)
         int         diff;
   const int         wd;
         int        *n;
         int        *day;
         int        *month;
         int        *year;
   const Cmode_enum  mode;
/*
   Precomputes the date of the "n'th absolute weekday" `wd' of the year
     or the date on weekday `wd' of the "n'th absolute week" of the year,
     and return TRUE in case date exits in year, otherwise FALSE.
*/
{
   register int  j=0;
   auto     int  i=0;


   if (*year)
    {
      if (mode == DAy)
       {
         *day = DAY_MIN;
         *month = MONTH_MIN;
         if (wd)
          {
            if (   diff == WEEK_MAX+1
                || diff == 99)
             {
               i = diff;
               diff = WEEK_MAX;
             }
          }
         else
          {
            /*
               If a special value "999" for `diff' is given,
                 set it to last day of year (365|366).
            */
            if (diff == 999)
              diff = DAY_LAST + (days_of_february (*year) == 29);
            i = diff--;
          }
       }
      else
       {
         /*
            `mode' == WEek.
         */
         j = diff;
         diff=i = weekno2doy (diff, *year);
         if (diff > DAY_MIN)
           diff--;
         else
           diff = 1;
         if (doy2date (diff, (days_of_february (*year)==29), day, month))
           diff = 1;
       }
    }
   if (!precomp_date (diff, wd, day, month, *year, DVar))
    {
      if (!*year)
       {
         /*
            No explicit year `yyyy' given in "date"-part of line.
         */
         *day = 0;
         *month = 0;
         *n = diff;
       }
      else
       {
         /*
            Invalid relative date given.
         */
         *day = 0;
         *month = 0;
         *year = SPECIAL_VALUE;
       }
      return(FALSE);
    }
   else
    {
      if (   wd
          && (mode == DAy))
       {
         register int  year_old=(*year);


         if (i)
           for (diff=DAY_MIN ; diff <= DAY_MAX ; diff++)
             next_date (day, month, year);
         if (   (   (*day <= DAY_MAX)
                 && (*year != year_old))
             || weekday_of_date (DAY_MIN, MONTH_MIN, *year) == wd)
           for (diff=DAY_MIN ; diff <= DAY_MAX ; diff++)
             prev_date (day, month, year);
         if (i == WEEK_MAX+1)
          {
            i = DAY_MIN;
            *month = MONTH_MIN;
            (void)precomp_date (WEEK_MAX, wd, &i, month, *year, DVar);
            if (   (*day == i)
                && (weekday_of_date (DAY_MIN, MONTH_MIN, *year) != wd))
             {
               /*
                  Error, no such 53rd weekday `ww[w]' of year.
               */
               *day = 0;
               *month = 0;
               *year = SPECIAL_VALUE;
               return(FALSE);
             }
          }
       }
      else
        /*
           `mode' == WEek.
        */
        if (   !wd
            || i < DAY_MIN
            || (   (j <= 1)
                && (*day == DAY_MAX+1)
                && (wd == DAY_MIN)))
         {
           if (*day >= DAY_MAX+i)
             *day -= DAY_MAX;
           else
             if (   !wd
                 && (   i < DAY_MIN
                     || (   (*day == DAY_MIN+1)
                         && (i == DAY_MIN))))
               (*day)--;
           if (*day < DAY_MIN)
            {
              /*
                 Error, n'th week doesn't contain such a weekday `ww[w]'.
              */
              *day = 0;
              *month = 0;
              *year = SPECIAL_VALUE;
              return(FALSE);
            }
         }
    }

   return(TRUE);
}



   PUBLIC Bool
precomp_date (diff, wd, day, month, year, mode)
         int         diff;
   const int         wd;
         int        *day;
         int        *month;
   const int         year;
   const Cmode_enum  mode;
/*
   Precomputes the date relative to Easter Sunday's date (mode==EAster),
     relative to Today's date (mode==TOday) or relative to date variables
     date (mode==DVar) plus displacement `diff' or displacement `diff' `wd'
     and return TRUE in case date exits in year, otherwise FALSE.
*/
{
   register int  i;


   if (   (   (mode == EAster)
           && (year >= EASTER_MIN)
           && (year <= EASTER_MAX))
       || (   (   mode == TOday
               || mode == DVar)
           && (year >= YEAR_MIN)
           && (year <= YEAR_MAX)))
    {
      switch (mode)
       {
         case EAster:
           i = knuth_easter_formula (year);
           break;
         case TOday:
           *day = act_day;
           *month = act_month;
         default:
           if (!valid_date (*day, *month, year))
             /*
                Error, invalid date given (e.g. 19010229).
             */
             return(FALSE);
           i = day_of_year (*day, *month, year);
       }
      if (wd)
       {
         /*
            Calculate date like:  3rd(`diff') Friday(`wd') before Easter Sunday's date.
         */
         if (   wd < DAY_MIN
             || wd > DAY_MAX)
           /*
              Error, invalid weekday specified.
           */
           return(FALSE);
         else
           if (!diff)
             /*
                Error, a weekday but no difference specified.
             */
             return(FALSE);
           else
             if (diff == -99)
              {
                /*
                   Detect first weekday `wd' of year.
                */
                *month = MONTH_MIN;
                *day = eval_holiday (DAY_MIN, *month, year, wd, TRUE);
                return(TRUE);
              }
             else
               if (diff == 99)
                {
                  /*
                     Detect last weekday `wd' of year.
                  */
                  *month = MONTH_MAX;
                  *day = eval_holiday (dvec[MONTH_MAX-1], *month, year, wd, FALSE);
                  return(TRUE);
                }
               else
                {
                  register int  act_wd;
                  auto     int  d;
                  auto     int  m;
                  auto     int  y=year;


                  doy2date (i, (days_of_february (y)==29), &d, &m);
                  act_wd = weekday_of_date (d, m, y);
                  if (act_wd != wd)
                   {
                     if (diff < 0)
                      {
                        /*
                           Try to detect first weekday `wd' before actual date.
                        */
                        while (act_wd != wd)
                         {
                           prev_date (&d, &m, &y);
                           act_wd = weekday_of_date (d, m, y);
                           i--;
                         }
                        diff++;
                      }
                     else
                      {
                        /*
                           Try to detect first weekday `wd' after actual date.
                        */
                        while (act_wd != wd)
                         {
                           next_date (&d, &m, &y);
                           act_wd = weekday_of_date (d, m, y);
                           i++;
                         }
                        diff--;
                      }
                   }
                  if (y != year)
                    /*
                       Error, we have left the year bounds.
                    */
                    return(FALSE);
                  /*
                     Calculate the difference.
                  */
                  i += diff * DAY_MAX;
                }
       }
      else
       {
         /*
           Calculate the difference.
         */
         if (diff == -999)
           i = 1;
         else
           if (diff == 999)
             i = DAY_LAST + (days_of_february (year) == 29);
           else
             i += diff;
       }
      if (doy2date (i, (days_of_february (year)==29), day, month))
        return(TRUE);
    }

   return(FALSE);
}



   PUBLIC Bool
set_dvar (line_buffer, lineptrs, filename, line_number, mode)
   const char        *line_buffer;
         Line_struct *lineptrs;
   const char        *filename;
   const long         line_number;
   const Dvar_enum    mode;
/*
   Scans given string `line_buffer' and tries to detect a valid date variable
     reference, which can be:
       1) `dvar'=           (NOTHING)  --> Undefine local `dvar' so we are able to use
                                             its global value.  If `mode' is set to "GLobal",
                                             this "empty" assignment results an error.
       2) `dvar'=`mmdd'                --> Assignment of a constant date expression `mmdd'.
       3) `dvar'=`mmww[w]'<n>          --> Assignment of a dynamic date expression.
                                             n'th weekday `ww[w]' of month `mm'.
       4) `dvar'=*d<n>[`ww[w]']        --> Assignment of a dynamic date expression
                                             n'th weekday `ww[w]' of year.
       5) `dvar'=*w<n>[`ww[w]']        --> Assignment of a dynamic date expression
                                             weekday `ww[w]' of n'th week of year.
       6) `dvar'=`dvar'                --> Assignment of a date variable `dvar',
                                             which must be already defined.
       7) `dvar'=`dvar'[+|-]<n>        --> Assignment of a date variable `dvar', which must be already
                                             defined, advanced by plus/minus <n> days.
       8) `dvar'=`dvar'[+|-]<n>`ww[w]' --> Assignment of a date variable `dvar', which must be already
                                             defined, advanced by plus/minus <n> weekdays `ww[w]'.
       9) `dvar'++                     --> Simple incrementation.
      10) `dvar'--                     --> Simple decrementation.
      11) `dvar'+=<n>                  --> Addition of a constant numeric factor <n>.
      12) `dvar'-=<n>                  --> Subtraction of a constant numeric factor <n>.
      13) `dvar'+=<n>`ww[w]'           --> Addition of <n> weekdays `ww[w]'.
      14) `dvar'-=<n>`ww[w]'           --> Subtraction of <n> weekdays `ww[w]'.
     A date variable name is valid from a...d, f...s and u...z (24 variables totally,
     case-insensitive), because the `e' variable is always reserved for the current
     Easter Sunday's date and the `t' variable is always reserved for today's date,
     so we must skip any reference to these variables.
     No whitespace characters may occur between the date variable, operator
     and value.  Stores assignment (1)...(8) at position `date variable'
     into global date variable vector `rc_dvar[]' (either the local or the
     global ones, depending on given `mode', which can be either "GLobal"
     or "LOcal".  Assignments (1), (3)...(5), (7), (8) and operations (9)...(14)
     may be used ONLY in local date variables.  Returns FALSE if an error
     occurs, otherwise TRUE.
*/
{
   auto         int    i;
   auto         int    len;
   auto         int    d=0;
   auto         int    m=0;
   auto         int    y=year;
   auto         int    n;
   auto         char   op;
   auto         char   op2;
   auto   const char  *ptr_char=line_buffer;
   static       char   str20[21];
   auto         Bool   is_error=FALSE;
   auto         Bool   is_weekday_mode;
   auto         Bool   dvar_with_displacement=FALSE;
   auto         Bool   dvar_inc_dec=FALSE;
   auto         Bool   dvar_add_sub=FALSE;
   auto         Bool   skip_dvar_assign=FALSE;


   /*
      Skip and return error if invalid date variable name is given.
   */
   if (   isalpha(*ptr_char)
       && (tolower(*ptr_char) != RC_EASTER_CHAR)
       && (tolower(*ptr_char) != RC_TODAY_CHAR))
    {
      ptr_char++;
      /*
         Check if assignment (1)...(8) is given.
      */
      if (*ptr_char != *RC_DVAR_ASSIGN)
       {
         if (   (*ptr_char != *RC_DVAR_ADD)
             && (*ptr_char != *RC_DVAR_SUB))
           /*
              Error, invalid first operator character found (no '+' or '-' given).
           */
           is_error = TRUE;
         else
          {
            /*
               Check if operation (9)...(14) is given.
            */
            op = *ptr_char++;
            if (*ptr_char)
             {
               op2 = *ptr_char++;
               if (   op2 == op
                   || op2 == *RC_DVAR_ASSIGN)
                {
                  if (mode == GLobal)
                    m = (int)rc_dvar[IDX(*line_buffer)].g.month;
                  else
                    m = (int)rc_dvar[IDX(*line_buffer)].l.month;
                  if (m)
                   {
                     if (op == op2)
                      {
                        while (isspace(*ptr_char))
                          ptr_char++;
                        if (*ptr_char)
                          /*
                             Error, found invalid trailing characters.
                          */
                          is_error = TRUE;
                        else
                          /*
                             Either 'dvar++' or 'dvar--' found.
                          */
                          dvar_inc_dec = TRUE;
                      }
                     else
                       /*
                          Either 'dvar+=...' or 'dvar-=...' found.
                       */
                       dvar_add_sub = TRUE;
                     if (!is_error)
                       goto LABEL_compute_dvar;
                   }
                  else
                   {
                     /*
                        Error, date variable undefined.
                     */
                     skip_dvar_assign = TRUE;
                     if (warning_level >= 0)
                       dvar_warning (113, (int)*line_buffer, line_buffer, filename, line_number);
                   }
                }
               else
                 /*
                    Error, invalid second operator character found
                      (no '=', '+' or '-' given resp., illegal combination of '+' and '-').
                 */
                 is_error = TRUE;
             }
            else
              /*
                 Error, incomplete operator found (no '+=', '-=', '++' or '--' given).
              */
              is_error = TRUE;
          }
       }
      else
       {
         /*
            Assignment (1)...(8) to date variable found (simple '=' given),
              scan expression part of date variable definition.  Assignments
              (1), (3)...(5), (7), (8) are ONLY allowed for local date variables.
         */
         i = 0;
         ptr_char++;
         if (!*ptr_char)
          {
            /*
               No date assigned ("empty" assignment), set the date variable slot to
                 zero so we are able to use its possibly setted global value if this
                 variable is referenced again at a later place within the sequence.
                 This kind of assignment is allowed for local date variables only;
                 for global date variables, we have to report an error instead.
            */
            if (mode == GLobal)
              /*
                 Error, empty assignment to a global date variable given.
              */
              is_error = TRUE;
          }
         else
          {
            if (   isalpha(*ptr_char)
                && !isalpha(*(ptr_char+1)))
             {
               op = *ptr_char;
               ptr_char++;
               if (   !*ptr_char
                   || isspace(*ptr_char))
                {
                  if (   tolower(op) == RC_EASTER_CHAR
                      || tolower(op) == RC_TODAY_CHAR)
                   {
                     /*
                        Error, date variable is invalid.
                     */
                     skip_dvar_assign = TRUE;
                     if (warning_level >= 0)
                       dvar_warning (112, (int)*line_buffer, line_buffer, filename, line_number);
                   }
                  else
                   {
                     /*
                        If the character after '=' is alphabetic and is not trailed
                          by digits, assume assignment (6) is given.
                     */
                     if (mode == GLobal)
                      {
                        m = (int)rc_dvar[IDX(op)].g.month;
                        d = (int)rc_dvar[IDX(op)].g.day;
                      }
                     else
                      {
                        m = (int)rc_dvar[IDX(op)].l.month;
                        d = (int)rc_dvar[IDX(op)].l.day;
                      }
                   }
                }
               else
                {
                  /*
                     Check if assignments (7)...(8) are given.
                  */
                  if (   *ptr_char == *ASC_LIT
                      || *ptr_char == *DES_LIT
                      || isdigit(*ptr_char ))
                   {
                     ptr_char--;
                     dvar_with_displacement = TRUE;
                     goto LABEL_compute_dvar;
                   }
                  else
                    /*
                       Error, invalid date variable name given.
                    */
                    is_error = TRUE;
                }
             }
            else
             {
LABEL_compute_dvar:
               /*
                  Assuming the string vectors have a minimum length of 1024
                    Bytes and the maximum text length of a date variable
                    assignment/operation in a line may not be longer than
                    20 Bytes, let's use these 20 Bytes of the line only.
               */
               strncpy(str20, ptr_char, 20);
               str20[20] = '\0';
               if (dvar_with_displacement)
                 sprintf(s5, "%0*d%c%s", len_year_max, y, RC_HDY_CHAR, str20);
               else
                 if (dvar_add_sub)
                   sprintf(s5, "%0*d%c%c%c%s", len_year_max, y, RC_HDY_CHAR, *line_buffer, op, str20);
                 else
                   if (dvar_inc_dec)
                     sprintf(s5, "%0*d%c%c%c1", len_year_max, y, RC_HDY_CHAR, *line_buffer, op);
                   else
                     sprintf(s5, "%0*d%s", len_year_max, y, str20);
               /*
                  `rc_get_date()' arguments `len' and `i' are dummys
                    only and must be given.  They are not respected!
               */
               (void)rc_get_date (s5, lineptrs, FALSE, &is_weekday_mode, &d, &m, &y, &n, &len,
                                  &op, &i, &i, filename, line_number, line_buffer, TRUE);
               if (y != SPECIAL_VALUE)
                {
                  /*
                     Check if assignments (3)...(5) are given.
                  */
                  if (   (mode == GLobal)
                      && (   op
                          || is_weekday_mode))
                    is_error = TRUE;
                  else
                   {
                     /*
                        Assignments (2)...(3) are given.
                     */
                     if (   m < MONTH_MIN
                         || m > MONTH_MAX)
                       /*
                          Error, invalid month given.
                       */
                       is_error = TRUE;
                     else
                      {
                        i = dvec[m-1];
                        if (m == 2)
                          i += is_leap_year;
                        /*
                           Check for assignment (3) `dvar'=`mmww[w]'<n> (`ww'=mo...su,
                             `www'=mon...sun, n=1...5|9), e.g.:
                             x=03mo3  sets `x' to date of 3rd Monday in March
                             x=03mon3  sets `x' to date of 3rd Monday in March, too.
                        */
                        if (is_weekday_mode)
                         {
                           if (n == 9)
                             d = eval_holiday (i, m, year, d, FALSE);
                           else
                            {
                              d = eval_holiday (DAY_MIN, m, year, d, TRUE);
                              d += (DAY_MAX * (n - 1));
                              if (d > i)
                               {
                                 /*
                                    Month contains no such "n'th weekday of month",
                                      ignore the assignment but produce NO error!!
                                 */
                                 skip_dvar_assign = TRUE;
                                 if (warning_level >= 0)
                                   dvar_warning (112, (int)*line_buffer, line_buffer, filename, line_number);
                               }
                            }
                         }
                        else
                         {
                           /*
                              Assume assignment (1) is given.
                           */
                           if (d == 99)
                             d = i;
                           /*
                              We must avoid an assigment like `dvar'=0229
                                if we are in fiscal year mode and the next
                                year is no leap year and no "--leap-day=ARG"
                                option is given!
                           */
                           if (   (fiscal_month > MONTH_MIN+1)
                               && (days_of_february (year+1) == 28)
                               && !rc_feb_29_to_feb_28
                               && !rc_feb_29_to_mar_01
                               && (m == 2)
                               && (d == 29))
                            {
                              /*
                                 Year contains no such date,
                                   ignore the assignment but produce NO error!!
                              */
                              skip_dvar_assign = TRUE;
                              if (warning_level >= 0)
                                dvar_warning (112, (int)*line_buffer, line_buffer, filename, line_number);
                            }
                           else
                            {
                              if (d > i)
                               {
                                 manage_leap_day (&d, &m, year, line_buffer, filename, line_number);
                                 i = d;
                               }
                              if (   d < DAY_MIN
                                  || d > i)
                               {
                                 /*
                                    Error, invalid day given.
                                 */
                                 is_error = TRUE;
                               }
                            }
                         }
                      }
                   }
                }
               else
                {
                  /*
                     Year contains no such date,
                       ignore the assignment but produce NO error!!
                  */
                  skip_dvar_assign = TRUE;
                  if (warning_level >= 0)
                    dvar_warning (112, (int)*line_buffer, line_buffer, filename, line_number);
                }
             }
          }
       }
      if (   !is_error
          && !skip_dvar_assign)
       {
         /*
            Store the assigned/calculated date.
         */
         if (mode == GLobal)
          {
            rc_dvar[IDX(*line_buffer)].g.month = (char)m;
            rc_dvar[IDX(*line_buffer)].g.day = (char)d;
          }
         else
          {
            rc_dvar[IDX(*line_buffer)].l.month = (char)m;
            rc_dvar[IDX(*line_buffer)].l.day = (char)d;
          }
       }
    }
   else
     /*
        Error, invalid date variable name given (not a...d, f...s, u...z).
     */
     is_error = TRUE;

   return((Bool)!is_error);
}



   PUBLIC Bool
set_tvar (line_buffer, mode)
   const char      *line_buffer;
   const Tvar_enum  mode;
/*
   Scans given string `line_buffer' and tries to detect a valid text variable
     reference, which is:
       $`tvar'=[TEXT] --> Assignment of a constant text expression TEXT to `tvar'
                          (TEXT may contain references to other `tvar's, which
                          are always expanded recursively before the assignment
                          is performed)!
     A text variable name is valid from $a...$z (totally 26 variables,
     case-insensitve).  No whitespace characters may occur between the text
     variable prefix character '$' and the text variable letter itself, the
     assignment operator '=' and the TEXT value.  Stores the assigned TEXT at
     position `text variable' into global text variable vector `rc_tvar[]' (either
     the local or the global ones, depending on given `mode', which can be either
     "GLobal" or "LOcal".  Returns FALSE if an error occurs, otherwise TRUE.
*/
{
   register       int    len;
   auto           char   tvar;
   auto     const char  *ptr_char=line_buffer;
   auto           Bool   is_error=FALSE;


   if (*ptr_char == RC_TVAR_CHAR)
    {
      /*
         Skip the trailing '$' character of a text variable by default.
      */
      ptr_char++;
      /*
         Skip and return error if invalid text variable name is given.
      */
      if (isalpha(*ptr_char))
       {
         tvar = *ptr_char++;
         /*
            Check if an assignment is given.
         */
         if (*ptr_char == *RC_TVAR_ASSIGN)
          {
            register       int    i=0;
            auto     const char  *ptr_tvar;
            auto           Bool   is_quoted=FALSE;


            ptr_char++;
            /*
               Check if the assigned TEXT contains any references
                 to other `tvar' variables, if so, insert their TEXTs.
            */
            ptr_tvar = strchr(ptr_char, RC_TVAR_CHAR);
            if (ptr_tvar != (char *)NULL)
             {
               do
                {
                  len = (int)(ptr_tvar - ptr_char);
                  if (len)
                   {
                     while ((Uint)len+i >= maxlen_max)
                       resize_all_strings (maxlen_max<<1, TRUE, __FILE__, (long)__LINE__);
                     strncpy(s5+i, ptr_char, len);
                     i += len;
                   }
                  s5[i] = '\0';
                  if (i)
                    if (s5[i-1] == QUOTE_CHAR)
                      is_quoted = TRUE;
                  ptr_tvar++;
                  if (   !is_quoted
                      && isalpha(*ptr_tvar))
                   {
                     register int  j=0;


                     /*
                        Try to insert the value of this `tvar' (this is its TEXT).
                     */
                     if (   (mode == GLobal)
                         && (rc_tvar[IDX(*ptr_tvar)].g.text != (char *)NULL))
                      {
                        j = (int)strlen(rc_tvar[IDX(*ptr_tvar)].g.text);
                        if (j)
                         {
                           while ((Uint)i+j >= maxlen_max)
                             resize_all_strings (maxlen_max<<1, TRUE, __FILE__, (long)__LINE__);
                           strcat(s5, rc_tvar[IDX(*ptr_tvar)].g.text);
                         }
                      }
                     else
                       if (   (mode == LOcal)
                           && (rc_tvar[IDX(*ptr_tvar)].l.text != (char *)NULL))
                        {
                          j = (int)strlen(rc_tvar[IDX(*ptr_tvar)].l.text);
                          if (j)
                           {
                             while ((Uint)i+j >= maxlen_max)
                               resize_all_strings (maxlen_max<<1, TRUE, __FILE__, (long)__LINE__);
                             strcat(s5, rc_tvar[IDX(*ptr_tvar)].l.text);
                           }
                        }
                     if (   (   (mode == GLobal)
                             && (rc_tvar[IDX(*ptr_tvar)].g.text != (char *)NULL))
                         || (   (mode == LOcal)
                             && (rc_tvar[IDX(*ptr_tvar)].l.text != (char *)NULL))
                         || (   (tvar == *ptr_tvar)
                             && (   (   (mode == GLobal)
                                     && (rc_tvar[IDX(*ptr_tvar)].g.text == (char *)NULL))
                                 || (   (mode == LOcal)
                                     && (rc_tvar[IDX(*ptr_tvar)].l.text == (char *)NULL)))))
                      {
                        /*
                           Skip `tvar' name.
                        */
                        len += 2;
                        if (j)
                          i += j;
                        else
                          /*
                             If `tvar' is "empty", remove a possibly obsolete whitespace.
                          */
                          if (i)
                            if (   isspace(s5[i-1])
                                && isspace(*(ptr_tvar + 1)))
                              s5[--i] = '\0';
                      }
                     else
                      {
                        /*
                           If `tvar' isn't defined, don't touch its name.
                        */
                        if ((Uint)i+2 >= maxlen_max)
                          resize_all_strings (maxlen_max<<1, TRUE, __FILE__, (long)__LINE__);
                        s5[i++] = RC_TVAR_CHAR;
                        s5[i++] = *ptr_tvar;
                        s5[i] = '\0';
                        len += 2;
                      }
                   }
                  else
                   {
                     /*
                        If a quoted or an invalid `tvar' name is found, don't touch it.
                     */
                     if ((Uint)i+1 >= maxlen_max)
                       resize_all_strings (maxlen_max<<1, TRUE, __FILE__, (long)__LINE__);
                     s5[i++] = RC_TVAR_CHAR;
                     s5[i] = '\0';
                     len++;
                     if (*ptr_tvar)
                      {
                        if ((Uint)i+1 >= maxlen_max)
                          resize_all_strings (maxlen_max<<1, TRUE, __FILE__, (long)__LINE__);
                        s5[i++] = *ptr_tvar;
                        s5[i] = '\0';
                        len++;
                      }
                   }
                  ptr_char += len;
                  ptr_tvar = strchr(ptr_char, RC_TVAR_CHAR);
                  is_quoted = FALSE;
                } while (ptr_tvar != (char *)NULL);
               /*
                  Add possibly trailing ordinary text.
               */
               if (*ptr_char)
                {
                  i += (int)strlen(ptr_char);
                  while ((Uint)i >= maxlen_max)
                    resize_all_strings (maxlen_max<<1, TRUE, __FILE__, (long)__LINE__);
                  strcat(s5, ptr_char);
                }
               i++;
               ptr_char = s5;
             }
            else
              i = (int)strlen(ptr_char) + 1;
            /*
               Store TEXT to the according `tvar' text variable.
            */
            if (mode == GLobal)
             {
               if (rc_tvar[IDX(tvar)].g.text == (char *)NULL)
                 rc_tvar[IDX(tvar)].g.text = (char *)my_malloc (i,
                                                                124, __FILE__, ((long)__LINE__)-1,
                                                                "rc_tvar[IDX(tvar)].g.text", IDX(tvar));
               else
                 rc_tvar[IDX(tvar)].g.text = (char *)my_realloc ((VOID_PTR)(rc_tvar[IDX(tvar)].g.text), i,
                                                                 124, __FILE__, ((long)__LINE__)-1,
                                                                 "rc_tvar[IDX(tvar)].g.text", IDX(tvar));
               strcpy(rc_tvar[IDX(tvar)].g.text, ptr_char);
             }
            else
             {
               if (i > 1)
                {
                  /*
                     We have to store the assigned text.
                  */
                  if (rc_tvar[IDX(tvar)].l.text == (char *)NULL)
                    rc_tvar[IDX(tvar)].l.text = (char *)my_malloc (i,
                                                                   124, __FILE__, ((long)__LINE__)-1,
                                                                   "rc_tvar[IDX(tvar)].l.text", IDX(tvar));
                  else
                    rc_tvar[IDX(tvar)].l.text = (char *)my_realloc ((VOID_PTR)(rc_tvar[IDX(tvar)].l.text), i,
                                                                    124, __FILE__, ((long)__LINE__)-1,
                                                                    "rc_tvar[IDX(tvar)].l.text", IDX(tvar));
                  strcpy(rc_tvar[IDX(tvar)].l.text, ptr_char);
                }
               else
                 /*
                    No text assigned ("empty" assignment), set the text variable slot to
                      NULL so we are able to use its possibly setted global value if
                      this variable is referenced again at a later place within the sequence.
                 */
                 if (rc_tvar[IDX(tvar)].l.text != (char *)NULL)
                  {
                    free(rc_tvar[IDX(tvar)].l.text);
                    rc_tvar[IDX(tvar)].l.text = (char *)NULL;
                  }
             }
          }
         else
           /*
              Error, invalid operator character found (not '=' given).
           */
           is_error = TRUE;
       }
      else
        /*
           Error, invalid text variable name given (not a...z).
        */
        is_error = TRUE;
    }
   else
     /*
        Error, no leading '$' character (text variable prefix) given.
     */
     is_error = TRUE;

   return((Bool)!is_error);
}



   PUBLIC void
nth_weekday_of_month (d, m, y, n, is_weekday_mode)
         int  *d;
         int  *m;
         int  *y;
   const int  *n;
         Bool *is_weekday_mode;
/*
   If "n'th weekday of month" field is encoded:
     Compute the according date and return it in &d, &m, &y.
     If conversion error occurs, return &y==SPECIAL_VALUE (special value).
*/
{
   register int   i;
   register int   j=0;
   auto     int   dd=0;
   auto     int   mm=0;
   auto     Bool  year_set=FALSE;
   auto     Bool  year_modified=FALSE;


   if (   *n
       && (   !rc_year_flag
           || (   *m
               && rc_year_flag))
       && (   !rc_period_list
           || (   *m
               && rc_period_list)))
    {
      if (   !*m
          && (   is_3month_mode
              || is_3month_mode2
              || fiscal_month > MONTH_MIN))
        /*
           If fiscal year resp., 3 month mode and no month encoded, skip evaluation.
        */
        ;
      else
       {
         *is_weekday_mode = FALSE;
         if (!*y)
          {
            year_set = TRUE;
            *y = year;
          }
         if (!*m)
          {
            *m = month;
            /*
               A [-c][<n>]w or [-c]t option set:
                 Lookahead whether the week ends in the month it started.
            */
            if (   rc_week_flag
                || rc_tomorrow_flag)
             {
               /*
                  <0000|`yyyy'>00`ww[w]'<n> event is in last week of last month of previous year.
               */
               if (   (*n > 3)
                   && (day < DAY_MIN))
                {
                  i = (days_of_february (year-1) == 29);
                  j = day + DAY_LAST + i;
                  (void)doy2date (j, i, &dd, &mm);
                }
               else
                 if (*n == 1)
                  {
                    /*
                       <0000|`yyyy'>00`ww[w]'<n> event is in first week of next month of actual year.
                    */
                    if (   (day+DAY_MAX-1 > 0)
                        && (day+DAY_MAX-1 < DAY_LAST+is_leap_year+1))
                      (void)doy2date (day+DAY_MAX-1, is_leap_year, &dd, &mm);
                    else
                     {
                       /*
                          <0000|`yyyy'>00`ww[w]'<n> event is in first week of first month of next year.
                       */
                       i = (days_of_february (year+1) == 29);
                       j = (day + DAY_MAX - 1) - (DAY_LAST + is_leap_year);
                       (void)doy2date (j, i, &dd, &mm);
                     }
                  }
               dd = *d;
             }
          }
         else
           if (   year_set
               && (   rc_week_flag
                   || rc_tomorrow_flag))
            {
              if (   (*n == 9)
                  && (*m == MONTH_MAX)
                  && (*y > YEAR_MIN)
                  && (day < DAY_MIN))
               {
                 year_modified = TRUE;
                 (*y)--;
               }
              else
                if (   (*n == 1)
                    && (*m == MONTH_MIN)
                    && (*y < YEAR_MAX)
                    && (day+DAY_MAX >= DAY_LAST+is_leap_year))
                 {
                   year_modified = TRUE;
                   (*y)++;
                 }
            }
         if (   year_set
             && (*y < YEAR_MAX)
             && (   (fiscal_month > MONTH_MIN)
                 && (*m < fiscal_month)))
           if (!year_modified)
             (*y)++;
         if (*m == 2)
           i = days_of_february (*y);
         else
           i = dvec[*m-1];
         if (*n == 9)
           *d = eval_holiday (i, *m, *y, *d, FALSE);
         else
          {
            *d = eval_holiday (DAY_MIN, *m, *y, *d, TRUE);
            *d += (DAY_MAX * (*n - 1));
            /*
               The "n'th weekday of month" doesn't occur in month:
                 Skip it.
            */
            if (*d > i)
              *y = SPECIAL_VALUE;
          }
         /*
            A [-c][<n>]w or [-c]t option set:
              Correction for lookahead.
         */
         if (   mm
             && (   rc_week_flag
                 || rc_tomorrow_flag))
          {
            if (   (*n == 1)
                && (mm != *m))
             {
               *m = mm;
               if (   (day+DAY_MAX-1 > 0)
                   && (day+DAY_MAX-1 < DAY_LAST+is_leap_year+1))
                 /*
                    Void, don't change the year of event.
                 */
                 ;
               else
                 if (   year_set
                     && (year < YEAR_MAX))
                   *y = year + 1;
               *d = eval_holiday (DAY_MIN, *m, *y, dd, TRUE);
             }
            else
              if (   (*n > 3)
                  && (   (   adate_set
                          && (mm == *m))
                      || (   !adate_set
                          && (mm != *m))))
               {
                 if (!adate_set)
                   *m = mm;
                 if (   year_set
                     && (year > YEAR_MIN))
                   *y = year - 1;
                 if (*n == 9)
                   *d = eval_holiday (dvec[MONTH_MAX-1], *m, *y, dd, FALSE);
                 else
                  {
                    *d = eval_holiday (DAY_MIN, *m, *y, dd, TRUE);
                    *d += (DAY_MAX * (*n - 1));
                    /*
                       The "n'th weekday of month" doesn't occur in month:
                         Skip it
                    */
                    if (*d > dvec[MONTH_MAX-1])
                      *y = SPECIAL_VALUE;
                  }
               }
          }
       }
    }
}



   PUBLIC Bool
prev_date (day, month, year)
   int *day;
   int *month;
   int *year;
/*
   Sets a delivered date back by one day (to yesterday's date)
     respecting the missing period of the Gregorian Reformation.
     Returns FALSE in case a date was within the missing period
       of Gregorian Reformation, otherwise TRUE;
*/
{
   auto Bool  no_missing_date=TRUE;


   if (   (*day <= greg->last_day+1)
       && (*day >= greg->first_day)
       && (*month == greg->month)
       && (*year == greg->year))
    {
      no_missing_date = FALSE;
      *day = greg->first_day - 1;
    }
   else
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

   return(no_missing_date);
}



   PUBLIC Bool
next_date (day, month, year)
   int *day;
   int *month;
   int *year;
/*
   Sets the delivered date forwards by one day (to tomorrow's date)
     respecting the missing period of the Gregorian Reformation.
     Returns FALSE in case a date was in the missing period
       of Gregorian Reformation, otherwise TRUE.
*/
{
   auto Bool  no_missing_date=TRUE;


   if (   (*day >= greg->first_day-1)
       && (*day <= greg->last_day)
       && (*month == greg->month)
       && (*year == greg->year))
    {
      no_missing_date = FALSE;
      *day = greg->last_day + 1;
    }
   else
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

   return(no_missing_date);
}



   PUBLIC void
num2date (julian_days, day, month, year)
   Ulint  julian_days;
   int   *day;
   int   *month;
   int   *year;
/*
   Converts a delivered absolute number of days `julian_days' to a standard date
     (since 00010101(==`yyyymmdd'), returned in &day, &month and &year)
     respecting the missing period of the Gregorian Reformation.
*/
{
   auto     double  x;
   auto     Ulint   jdays=date2num (greg->first_day-1, greg->month, greg->year);
   register int     i;


   if (julian_days > jdays)
     julian_days += (Ulint)(greg->last_day - greg->first_day + 1);
   x = (double)julian_days / (DAY_LAST + 0.25);
   i = (int)x;
   if ((double)i != x)
     *year = i + 1;
   else
    {
      *year = i;
      i--;
    }
   if (julian_days > jdays)
    {
      /*
         Correction for Gregorian years.
      */
      julian_days -= (Ulint)((*year / 400) - (greg->year / 400));
      julian_days += (Ulint)((*year / 100) - (greg->year / 100));
      x = (double)julian_days / (DAY_LAST + 0.25);
      i = (int)x;
      if ((double)i != x)
        *year = i + 1;
      else
       {
         *year = i;
         i--;
       }
      if (   (*year % 400)
          && !(*year % 100))
        julian_days--;
    }
   i = (int)(julian_days - (Ulint)(i * (DAY_LAST + 0.25)));
   /*
      Correction for Gregorian centuries.
   */
   if (   (*year > greg->year)
       && (*year % 400)
       && !(*year % 100)
       && (i < ((*year/100)-(greg->year/100))-((*year/400)-(greg->year/400))))
     i++;
   (void)doy2date (i, (days_of_february (*year)==29), day, month);
}



   PUBLIC Slint
d_between (d1, m1, y1, d2, m2, y2)
   const int d1;
   const int m1;
   const int y1;
   const int d2;
   const int m2;
   const int y2;
/*
   Computes the amount of days between date1(base date) and date2
     exclusive date1 and date2, and adds 1 to result.
*/
{
   return(date2num (d2, m2, y2)-date2num (d1, m1, y1));
}



   PUBLIC Slint
w_between (d1, m1, y1, d2, m2, y2)
   const int d1;
   const int m1;
   const int y1;
   const int d2;
   const int m2;
   const int y2;
/*
   Computes the amount of weeks between date1(base date) and date2
     exclusive date1 and date2, and adds 1 to result.
*/
{
   auto     Ulint  date1=date2num (d1, m1, y1);
   auto     Ulint  date2=date2num (d2, m2, y2);
   auto     Slint  diff;
   auto     Slint  result;


   diff = (Slint)date2 - (date1 - (SYEAR(weekday_of_date (d1, m1, y1), start_day)) + 1);
   result = diff / DAY_MAX;
   if (   (diff % DAY_MAX)
       && (diff < 0L))
     result--;

   return(result);
}



   PUBLIC Slint
m_between (m1, y1, m2, y2)
   const int m1;
   const int y1;
   const int m2;
   const int y2;
/*
   Computes the amount of months between date1(base date) and date2
     exclusive date1 and date2, and adds 1 to result.
*/
{
   return(((y2 - y1)*MONTH_MAX)+(m2 - m1));
}



   PUBLIC void
manage_leap_day (day, month, year, line_buffer, filename, line_number)
         int  *day;
         int  *month;
         int   year;
   const char *line_buffer;
   const char *filename;
   const long  line_number;
/*
   Tries to set the leap day (29-Feb) either to "28-Feb" or "1-Mar"
     and prints a informational message in case this date modification is
     performed successfully (only if --debug[=ARG] is given).
*/
{
   register int  action=0;


   if (   (*month == 2)
       && (*day == 29)
       && (   rc_feb_29_to_feb_28
           || rc_feb_29_to_mar_01))
    {
      if (   (fiscal_month > MONTH_MIN+1)
          && (year < YEAR_MAX))
       {
         if (days_of_february (year+1) == 28)
          {
            if (rc_feb_29_to_feb_28)
              *day=action = 28;
            else
             {
               *day=action = DAY_MIN;
               (*month)++;
             }
          }
       }
      else
        if (days_of_february (year) == 28)
         {
           if (rc_feb_29_to_feb_28)
             *day=action = 28;
           else
            {
              *day=action = DAY_MIN;
              (*month)++;
            }
         }
      if (   (warning_level >= 0)
          && action)
       {
         *s5 = '\0';
         print_text (stderr, s5);
         action = (int)strlen(filename) + 100;
         if ((Uint)action >= maxlen_max)
           resize_all_strings (action+1, FALSE, __FILE__, (long)__LINE__);
#  if USE_DE
         sprintf(s5, "Schalttag auf `%02d-%s' gesetzt in Datei `%s'.",
                 *day, month_name (*month), filename);
#  else /* !USE_DE */
         sprintf(s5, _("Leap-day set to `%02d-%s' in file `%s'."),
                 *day, month_name (*month), filename);
#  endif /* !USE_DE */
         print_text (stderr, s5);
#  if USE_DE
         sprintf(s5, "Zeile %ld: %s", line_number, line_buffer);
#  else /* !USE_DE */
         sprintf(s5, _("Line %ld: %s"), line_number, line_buffer);
#  endif /* !USE_DE */
         print_text (stderr, s5);
       }
    }
}



   PUBLIC char *
biorhythm (create_bar, axis_len, string,
           day, month, year, birth_day, birth_month, birth_year,
           emo_text, emo_phase, emo_waxes,
           int_text, int_phase, int_waxes,
           phy_text, phy_phase, phy_waxes,
           critical_day, positive_day, negative_day)
   const Bool  create_bar;
         int   axis_len;
         char *string;
   const int   day;
   const int   month;
   const int   year;
   const int   birth_day;
   const int   birth_month;
   const int   birth_year;
   const char *emo_text;
         int  *emo_phase;
         int  *emo_waxes;
   const char *int_text;
         int  *int_phase;
         int  *int_waxes;
   const char *phy_text;
         int  *phy_phase;
         int  *phy_waxes;
         int  *critical_day;
         int  *positive_day;
         int  *negative_day;
/*
   Computes the biorhythm for a date and creates a text graphics bar line
     according to the computed values in case `create_bar' is set to TRUE.
     Uses the delivered `string' for storing such a line and returns it.
     The caller has to guarantuee that enough `string' space is allocated.
     When used within Gcal, the maximum number of 100 that a single `axis_len'
     may have (100*2+6=>206) fits properly into the string vectors, which
     have 1024 Bytes by default.
     Moreover, this function optionally uses the emulation functions for
     computing sin() and cos()... OK, that's slow, I know.  The sin() and
     cos() functions themselves needs pow() and fac(), which are likewise
     replaced.  By reason of this, there is no real need for linking the
     mathematical library (libm.a) to this source, in this sense, it is
     optionally working stand-alone, and this is wanted for portability
     purposes.
*/
{
   auto     Slint  diff=d_between (birth_day, birth_month, birth_year, day, month, year);
   register int    yes_phase;
   register int    yes_waxes;
   register int    i;


   (*critical_day)=(*positive_day)=(*negative_day)=(*emo_waxes)=(*int_waxes)=(*phy_waxes) = 0;
   if (create_bar)
    {
      auto char  *ptr_string;


#if 0
      /*
         Decrease `axis_len' by 1 until it divides 100 without a remainder.
      */
      if (axis_len < 1)
        axis_len = -axis_len;
      if (axis_len > 100)
        axis_len = 100;
      else
        while (100 % axis_len)
          axis_len--;
#endif
      /*
         Initialize the biorhythm text graphics bar.
      */
      for (i=BIO_AXIS_TOTAL(axis_len),ptr_string=string ; --i ; )
        *ptr_string++ = ' ';
      string[BIO_AXIS_TOTAL(axis_len)-1] = '\0';
      string[BIO_AXIS_EXTRA-1] = *BIO_WANES;
      string[BIO_AXIS_TOTAL(axis_len)-BIO_AXIS_EXTRA-1] = *BIO_WAXES;
    }
   /*
      Reduce the day difference by multiples of 21252, which is the number
        of days at which each biorhythm cycle restarts from the birthdate.
   */
   if (diff < 1L)
     diff = 21252L - (-diff % 21252L);
   else
     diff %= 21252L;
   /*
      Manage the "emotional" phase value (cycle of 28 days).
   */
#  if HAVE_LIBM
   yes_phase = (int)ROUND(100.0 * sin(MY_TWO_PI*(double)(diff-1L)/28.0));
   yes_waxes = SGN((int)ROUND(100.0 * cos(MY_TWO_PI*(double)(diff-1L)/28.0)));
   *emo_phase = (int)ROUND(100.0 * sin(MY_TWO_PI*(double)diff/28.0));
   *emo_waxes = SGN((int)ROUND(100.0 * cos(MY_TWO_PI*(double)diff/28.0)));
#  else /* !HAVE_LIBM */
   yes_phase = (int)ROUND(100.0 * my_sin (MY_TWO_PI*(double)(diff-1L)/28.0));
   yes_waxes = SGN((int)ROUND(100.0 * my_cos (MY_TWO_PI*(double)(diff-1L)/28.0)));
   *emo_phase = (int)ROUND(100.0 * my_sin (MY_TWO_PI*(double)diff/28.0));
   *emo_waxes = SGN((int)ROUND(100.0 * my_cos (MY_TWO_PI*(double)diff/28.0)));
#  endif /* !HAVE_LIBM */
   if (*emo_phase == 100)
     (*positive_day)++;
   else
     if (   (yes_waxes == 1)
         && (yes_phase != 100)
         && (*emo_waxes <= 0))
       (*positive_day)++;
   if (*emo_phase == -100)
     (*negative_day)++;
   else
     if (   (yes_waxes == -1)
         && (yes_phase != -100)
         && (*emo_waxes >= 0))
       (*negative_day)++;
   *emo_waxes = (*emo_waxes >= 0);
   if (   (   (SGN(yes_phase) == 1)
           && (SGN(*emo_phase) <= 0))
       || (   (SGN(yes_phase) == -1)
           && (SGN(*emo_phase) >= 0)))
     (*critical_day)++;
   /*
      Manage the "intellectual" phase value (cycle of 33 days).
   */
#  if HAVE_LIBM
   yes_phase = (int)ROUND(100.0 * sin(MY_TWO_PI*(double)(diff-1L)/33.0));
   yes_waxes = SGN((int)ROUND(100.0 * cos(MY_TWO_PI*(double)(diff-1L)/33.0)));
   *int_phase = (int)ROUND(100.0 * sin(MY_TWO_PI*(double)diff/33.0));
   *int_waxes = SGN((int)ROUND(100.0 * cos(MY_TWO_PI*(double)diff/33.0)));
#  else /* !HAVE_LIBM */
   yes_phase = (int)ROUND(100.0 * my_sin (MY_TWO_PI*(double)(diff-1L)/33.0));
   yes_waxes = SGN((int)ROUND(100.0 * my_cos (MY_TWO_PI*(double)(diff-1L)/33.0)));
   *int_phase = (int)ROUND(100.0 * my_sin (MY_TWO_PI*(double)diff/33.0));
   *int_waxes = SGN((int)ROUND(100.0 * my_cos (MY_TWO_PI*(double)diff/33.0)));
#  endif /* !HAVE_LIBM */
   if (*int_phase == 100)
     (*positive_day)++;
   else
     if (   (yes_waxes == 1)
         && (yes_phase != 100)
         && (*int_waxes <= 0))
       (*positive_day)++;
   if (*int_phase == -100)
     (*negative_day)++;
   else
     if (   (yes_waxes == -1)
         && (yes_phase != -100)
         && (*int_waxes >= 0))
       (*negative_day)++;
   *int_waxes = (*int_waxes >= 0);
   if (   (   (SGN(yes_phase) == 1)
           && (SGN(*int_phase) <= 0))
       || (   (SGN(yes_phase) == -1)
           && (SGN(*int_phase) >= 0)))
     (*critical_day)++;
   /*
      Manage the "physical" phase value (cycle of 23 days).
   */
#  if HAVE_LIBM
   yes_phase = (int)ROUND(100.0 * sin(MY_TWO_PI*(double)(diff-1L)/23.0));
   yes_waxes = SGN((int)ROUND(100.0 * cos(MY_TWO_PI*(double)(diff-1L)/23.0)));
   *phy_phase = (int)ROUND(100.0 * sin(MY_TWO_PI*(double)diff/23.0));
   *phy_waxes = SGN((int)ROUND(100.0 * cos(MY_TWO_PI*(double)diff/23.0)));
#  else /* !HAVE_LIBM */
   yes_phase = (int)ROUND(100.0 * my_sin (MY_TWO_PI*(double)(diff-1L)/23.0));
   yes_waxes = SGN((int)ROUND(100.0 * my_cos (MY_TWO_PI*(double)(diff-1L)/23.0)));
   *phy_phase = (int)ROUND(100.0 * my_sin (MY_TWO_PI*(double)diff/23.0));
   *phy_waxes = SGN((int)ROUND(100.0 * my_cos (MY_TWO_PI*(double)diff/23.0)));
#  endif /* !HAVE_LIBM */
   if (*phy_phase == 100)
     (*positive_day)++;
   else
     if (   (yes_waxes == 1)
         && (yes_phase != 100)
         && (*phy_waxes <= 0))
       (*positive_day)++;
   if (*phy_phase == -100)
     (*negative_day)++;
   else
     if (   (yes_waxes == -1)
         && (yes_phase != -100)
         && (*phy_waxes >= 0))
       (*negative_day)++;
   *phy_waxes = (*phy_waxes >= 0);
   if (   (   (SGN(yes_phase) == 1)
           && (SGN(*phy_phase) <= 0))
       || (   (SGN(yes_phase) == -1)
           && (SGN(*phy_phase) >= 0)))
     (*critical_day)++;
   if (create_bar)
    {
      /*
         Place the "emotional", "intellectual" and "physical"
           marker on the text graphics bar.
      */
      i = *emo_phase / BIO_AXIS_SCALE(axis_len);
      if (SGN(*emo_phase) >= 0)
        i += BIO_AXIS_ZERO(axis_len);
      else
        i = BIO_AXIS_ZERO(axis_len) + i;
      if (string[i] == ' ')
        string[i] = *emo_text;
      else
        string[i] = *BIO_OVERLAPS;
      i = *int_phase / BIO_AXIS_SCALE(axis_len);
      if (SGN(*int_phase) >= 0)
        i += BIO_AXIS_ZERO(axis_len);
      else
        i = BIO_AXIS_ZERO(axis_len) + i;
      if (string[i] == ' ')
        string[i] = *int_text;
      else
        string[i] = *BIO_OVERLAPS;
      i = *phy_phase / BIO_AXIS_SCALE(axis_len);
      if (SGN(*phy_phase) >= 0)
        i += BIO_AXIS_ZERO(axis_len);
      else
        i = BIO_AXIS_ZERO(axis_len) + i;
      if (string[i] == ' ')
        string[i] = *phy_text;
      else
        string[i] = *BIO_OVERLAPS;
      /*
         Place the accumulated "negative", "positive" and "critical"
           day counters on the text graphics bar.
      */
      *string = DIG2CHR(*negative_day);
      string[BIO_AXIS_TOTAL(axis_len)-BIO_AXIS_EXTRA] = DIG2CHR(*positive_day);
      string[BIO_AXIS_ZERO(axis_len)] = DIG2CHR(*critical_day);
    }

   return(string);
}



#  if HAVE_LIBM
   LOCAL double
kepler (m, ecc)
         double m;
   const double ecc;
/*
   Solves the equation of Kepler.
*/
{
   auto double  e;
   auto double  delta;


   e=m = TORAD(m);
   do
    {
      delta = e - ecc * sin(e) - m;
      e -= delta / (1.0 - ecc * cos(e));
    } while (abs(delta)-KEPLER_EPSILON > 0.0);

   return(e);
}



   LOCAL double
phase (julian_date)
   const double julian_date;
/*
   Calculates the phase of the Moon and returns the illuminated fraction of
     the Moon's disc as a value within the range of -99.9~...0.0...+99.9~,
     which has a negative sign in case the Moon wanes, otherwise the sign
     is positive.  The New Moon phase is around the 0.0 value and the Full
     Moon phase is around the +/-99.9~ value.  The argument is the time for
     which the phase is requested, expressed as a Julian date and fraction.
   This function is taken from the program "moontool" by John Walker,
     February 1988, which is in the public domain.  So see it for more
     information!  It is adapted (crippled) and `pretty-printed' to the
     requirements of Gcal, which means it is lacking all the other useful
     computations of astronomical values of the original code.

   Here is the blurb from "moontool":
   ... The algorithms used in this program to calculate the positions Sun
   and Moon as seen from the Earth are given in the book "Practical Astronomy
   With Your Calculator" by Peter Duffett-Smith, Second Edition,
   Cambridge University Press, 1981. Ignore the word "Calculator" in the
   title; this is an essential reference if you're interested in
   developing software which calculates planetary positions, orbits,
   eclipses, and the like. If you're interested in pursuing such
   programming, you should also obtain:

   "Astronomical Formulae for Calculators" by Jean Meeus, Third Edition,
   Willmann-Bell, 1985. A must-have.

   "Planetary Programs and Tables from -4000 to +2800" by Pierre
   Bretagnon and Jean-Louis Simon, Willmann-Bell, 1986. If you want the
   utmost (outside of JPL) accuracy for the planets, it's here.

   "Celestial BASIC" by Eric Burgess, Revised Edition, Sybex, 1985. Very
   cookbook oriented, and many of the algorithms are hard to dig out of
   the turgid BASIC code, but you'll probably want it anyway.

   Many of these references can be obtained from Willmann-Bell, P.O. Box
   35025, Richmond, VA 23235, USA. Phone: (804) 320-7016. In addition
   to their own publications, they stock most of the standard references
   for mathematical and positional astronomy.

   This program was written by:

      John Walker
      Autodesk, Inc.
      2320 Marinship Way
      Sausalito, CA 94965
      (415) 332-2344 Ext. 829

      Usenet: {sun!well}!acad!kelvin

   This program is in the public domain: "Do what thou wilt shall be the
   whole of the law". I'd appreciate receiving any bug fixes and/or
   enhancements, which I'll incorporate in future versions of the
   program. Please leave the original attribution information intact so
   that credit and blame may be properly apportioned.
*/
{
   auto double  date_within_epoch;
   auto double  sun_eccent;
   auto double  sun_mean_anomaly;
   auto double  sun_perigree_co_ordinates_to_epoch;
   auto double  sun_geocentric_elong;
   auto double  moon_evection;
   auto double  moon_variation;
   auto double  moon_mean_anomaly;
   auto double  moon_mean_longitude;
   auto double  moon_annual_equation;
   auto double  moon_correction_term1;
   auto double  moon_correction_term2;
   auto double  moon_correction_equation_of_center;
   auto double  moon_corrected_anomaly;
   auto double  moon_corrected_longitude;
   auto double  moon_present_age;
   auto double  moon_present_phase;
   auto double  moon_present_longitude;


   /*
      Calculation of the Sun's position.
   */
   date_within_epoch = julian_date - EPOCH;
   sun_mean_anomaly = FIXANGLE((360.0/365.2422)*date_within_epoch);
   sun_perigree_co_ordinates_to_epoch = FIXANGLE(sun_mean_anomaly+SUN_ELONG_EPOCH-SUN_ELONG_PERIGEE);
   sun_eccent = kepler (sun_perigree_co_ordinates_to_epoch, ECCENT_EARTH_ORBIT);
   sun_eccent = sqrt((1.0+ECCENT_EARTH_ORBIT)/(1.0-ECCENT_EARTH_ORBIT))*tan(sun_eccent/2.0);
   sun_eccent = 2.0 * TODEG(atan(sun_eccent));
   sun_geocentric_elong = FIXANGLE(sun_eccent+SUN_ELONG_PERIGEE);
   /*
      Calculation of the Moon's position.
   */
   moon_mean_longitude = FIXANGLE(13.1763966*date_within_epoch+MOON_MEAN_LONGITUDE_EPOCH);
   moon_mean_anomaly = FIXANGLE(moon_mean_longitude-0.1114041*date_within_epoch-MOON_MEAN_LONGITUDE_PERIGREE);
   moon_evection = 1.2739 * sin(TORAD(2.0*(moon_mean_longitude-sun_geocentric_elong)-moon_mean_anomaly));
   moon_annual_equation = 0.1858 * sin(TORAD(sun_perigree_co_ordinates_to_epoch));
   moon_correction_term1 = 0.37 * sin(TORAD(sun_perigree_co_ordinates_to_epoch));
   moon_corrected_anomaly = moon_mean_anomaly + moon_evection - moon_annual_equation - moon_correction_term1;
   moon_correction_equation_of_center = 6.2886 * sin(TORAD(moon_corrected_anomaly));
   moon_correction_term2 = 0.214 * sin(TORAD(2.0*moon_corrected_anomaly));
   moon_corrected_longitude =   moon_mean_longitude + moon_evection + moon_correction_equation_of_center
                              - moon_annual_equation + moon_correction_term2;
   moon_variation = 0.6583 * sin(TORAD(2.0*(moon_corrected_longitude-sun_geocentric_elong)));
   moon_present_longitude = moon_corrected_longitude + moon_variation;
   moon_present_age = moon_present_longitude - sun_geocentric_elong;
   moon_present_phase = 100.0 * ((1.0 - cos(TORAD(moon_present_age))) / 2.0);
   if (FIXANGLE(moon_present_age)-180.0 > 0.0)
     moon_present_phase = -moon_present_phase;

   return(moon_present_phase);
}



   PUBLIC int
the_phase (is_full_new, day, month, year, hour, min)
         Bool *is_full_new;
   const int   day;
   const int   month;
   const int   year;
   const int   hour;
   const int   min;
/*
   Computes yesterday's, today's and tomorrow's moonphase by calling the
     `phase()' function for each day and sets `&is_full_moon' to TRUE
     in case today's Moon is in the "Full" or in the "New" phase, otherwise
     to FALSE.  Returns the illuminated fraction of the Moon's disc as an
     integer value within the range of -100...0...+100, which has a negative
     sign in case the Moon wanes, otherwise the sign is positive.
*/
{
   auto double  the_date;
   auto double  the_time=(double)hour*60.0+(double)min;
   auto double  yes;
   auto double  tod;
   auto double  tom;


   *is_full_new = FALSE;
   the_date = (double)(date2num (day, month, year) + RC_MIN_BCE_TO_1_CE - (Ulint)2);
   if (the_time > 0.0)
     the_date += (1.0 / (1440.0 / the_time));
   yes = phase (the_date);
   the_date += 1.0;
   tod = phase (the_date);
   the_date += 1.0;
   tom = phase (the_date);
   if (   SGN(yes) != SGN(tod)
       || SGN(tod) != SGN(tom))
    {
      if (   (SGN(tod) == -1)
          && (SGN(tom) >= 0))
       {
         if (abs(tod)-abs(tom) <= 0.0)
           *is_full_new = TRUE;
       }
      else
        if (   (SGN(yes) == -1)
            && (SGN(tom) >= 0))
         {
           if (abs(tod)-abs(yes) <= 0.0)
             *is_full_new = TRUE;
         }
        else
          if (   (SGN(tod) >= 0)
              && (SGN(tom) == -1))
           {
             if ((100.0-tod)-(100.0+tom) <= 0.0)
               *is_full_new = TRUE;
           }
          else
            if (   (SGN(yes) >= 0)
                && (SGN(tod) == -1))
             {
               if ((100.0+tod)-(100.0-yes) <= 0.0)
                 *is_full_new = TRUE;
             }
      if (   !*is_full_new
          && (   abs(floor(tod)) == 0.0
              || abs(floor(tod)) == 100.0))
        tod = ceil(tod);
    }
   tod = floor(tod);
   if (   !*is_full_new
       && (   abs(tod) == 0.0
           || abs(tod) == 100.0))
     tod += 1.0;

   return((int)tod);
}
#  else /* !HAVE_LIBM */
   LOCAL double
moonphase (day, month, year, hour, min)
   const int day;
   const int month;
   const int year;
   const int hour;
   const int min;
/*
   Calculates the phase of the Moon and returns the illuminated fraction of
     the Moon's disc as a value within the range of -99.9~...0.0...+99.9~,
     which has a negative sign in case the Moon wanes, otherwise the sign
     is positive.  The New Moon phase is around the 0.0 value and the Full
     Moon phase is around the +/-99.9~ value.  This function computes the
     Moon phases only approximately, but it almost works fine when compared
     with the functions of John Walker "moontool" program.
*/
{
   auto       double  result;
   auto       Ulint   delta;
   auto       Ulint   offset;
   auto const Ulint   length=(Ulint)(2551.0/60.0*1000.0+443.0/60.0);
   auto const Ulint   diff=(Ulint)d_between (1, 1, 1978, day, month, year)+(Ulint)1;


   offset = (diff * (Ulint)24 + hour) * (Ulint)60 + min;
   delta = offset - ((Ulint)273 * (Ulint)24 + (Ulint)13) * (Ulint)60 + (Ulint)23;
   offset = delta - (delta / length) * length;
   result = (double)offset / length * 100.0;
   result -= (((double)year / 1000.0) + 4.0);
   if (result-50.0 > 0.0)
     return (-(100.0-((result-50.0)*2.0)));

   return(result*2.0);
}



   PUBLIC int
the_moonphase (is_full_new, day, month, year, hour, min)
         Bool *is_full_new;
         int   day;
         int   month;
         int   year;
   const int   hour;
   const int   min;
/*
   Computes yesterday's, today's and tomorrow's moonphase by calling the
     `moonphase()' function for each day and sets `&is_full_moon' to TRUE
     in case today's Moon is in the "Full" or in the "New" phase, otherwise
     to FALSE.  Returns the illuminated fraction of the Moon's disc as an
     integer value within the range of -100...0...+100, which has a negative
     sign in case the Moon wanes, otherwise the sign is positive.
*/
{
   auto double  yes;
   auto double  tod;
   auto double  tom;


   *is_full_new = FALSE;
   (void)prev_date (&day, &month, &year);
   yes = moonphase (day, month, year, hour, min);
   (void)next_date (&day, &month, &year);
   tod = moonphase (day, month, year, hour, min);
   (void)next_date (&day, &month, &year);
   tom = moonphase (day, month, year, hour, min);
   if (   SGN(yes) != SGN(tod)
       || SGN(tod) != SGN(tom))
    {
      if (   (SGN(tod) == -1)
          && (SGN(tom) >= 0))
       {
         if (abs(tod)-abs(tom) <= 0.0)
           *is_full_new = TRUE;
       }
      else
        if (   (SGN(yes) == -1)
            && (SGN(tom) >= 0))
         {
           if (abs(tod)-abs(yes) <= 0.0)
             *is_full_new = TRUE;
         }
        else
          if (   (SGN(tod) >= 0)
              && (SGN(tom) == -1))
           {
             if ((100.0-tod)-(100.0+tom) <= 0.0)
               *is_full_new = TRUE;
           }
          else
            if (   (SGN(yes) >= 0)
                && (SGN(tod) == -1))
             {
               if ((100.0+tod)-(100.0-yes) <= 0.0)
                 *is_full_new = TRUE;
             }
    }

   return((int)ROUND(tod));
}
#  endif /* !HAVE_LIBM */



   PUBLIC void
draw_moon (age, lines, string)
   const int    age;
   const int    lines;
         char **string;
/*
   Creates a text graphics image of the moon according to its `age', which is
     expressed as the illuminated fraction of the Moon's disc as a value within
     the range of -100...0...+100, which has a negative sign in case the Moon
     wanes, otherwise the sign is positive.  Uses the delivered `string' for
     storing this image (each line with a leading newline character represented
     by Gcal RC_NL_CHAR=='~') and returns it.  The caller has to guarantuee
     that enough `string' space is allocated, which means there must be two
     Bytes allocated minimum in this case, because this function increases
     the `string' properly by reallocating it internally when used within Gcal.
     Moreover, this function optionally uses the emulation functions for
     computing sqrt() and cos()... OK, that's slow, I know.  The cos() function
     itself needs pow() and fac(), which are likewise replaced.  By reason of
     this, there is no real need for linking the mathematical library (libm.a)
     to this source, in this sense, it is optionally working stand-alone, and
     this is wanted for portability purposes.
*/
{
   register       Uint     slen=0;
   register       int      i;
   register       int      j;
   register       int      k;
   register       int      end;
   auto           double   counter=(double)age;
   auto     const double   step=2.0/(double)lines;
   auto           double   squisher;
   auto           double   horizon;
   auto           double   terminator;
   static         char     buffer[80];   /* Proper if [MOONIMAGE_MIN]6<=lines<=30[MOONIMAGE_MAX] */
   auto           char    *ptr_buffer;


   **string = '\0';
   if (counter < 0.0)
     counter = -counter;
   if (age < 0)
     counter = (100.0 - (double)(((Uint)counter >> 1) + 50)) / 100.0;
   else
     counter /= 200.0;
#  if !HAVE_LIBM
   squisher = my_cos (counter*MY_TWO_PI);
#  else /* HAVE_LIBM */
   squisher = cos(counter*MY_TWO_PI);
#  endif /* HAVE_LIBM */
   for (counter=0.93 ; counter > -1.0 ; counter-=step)
    {
      sprintf(buffer, "%c", RC_NL_CHAR);
      strcat(*string, buffer);
      slen++;
#  if !HAVE_LIBM
      horizon = my_sqrt (1.0-counter*counter);
#  else /* HAVE_LIBM */
      horizon = sqrt(1.0-counter*counter);
#  endif /* HAVE_LIBM */
      i=j = moon_charpos (horizon, lines+6);
      for (ptr_buffer=buffer ; i-- ; )
        *ptr_buffer++ = ' ';
      i = j;
      slen += (i + 1);
      buffer[i] = *MOONIMAGE_REDGE;
      buffer[i+1] = '\0';
      j = moon_charpos (-horizon, lines+6);
      buffer[j] = *MOONIMAGE_LEDGE;
      terminator = horizon * squisher;
      if (abs(age) > 6)
       {
         k = moon_charpos (terminator, lines+6);
         if (age > 0)
          {
            end = i;
            i = k;
          }
         else
          {
            i = j;
            end = k;
          }
         while (i <= end)
           buffer[i++] = *MOONIMAGE_BRIGHT;
       }
      if (slen+1 >= maxlen_max)
        resize_all_strings (maxlen_max<<1, FALSE, __FILE__, (long)__LINE__);
      strcat(*string, buffer);
    }
}



   LOCAL int
moon_charpos (x, lines)
   const double x;
   const int    lines;
/*
   Computes the position where to place the next character
     of the moon text graphics image.
*/
{
   register int  i;


   i = (int)(x * (double)lines - 0.5);
   if (i+lines < 1)
     i = 0;
   else
     i += lines;

   return(i);
}



#  if !HAVE_LIBM
   LOCAL Ulint
my_fac (n)
   Slint n;
/*
   Computes the factorial of a positive `n' recursively.

             |  1        for  n = 0
      n! :=  |
             |  n*(n-1)  for  n >= 1
*/
{
   if (n < 0L)
     n = -n;

   return((n <= 1L) ? 1L : n*my_fac (n-1L));
}



   LOCAL double
my_pow (x, n)
   double x;
   int    n;
/*
                      ` n'
   Computes the power  x   iteratively.
*/
{
   auto     double  y;
   register int     is_neg=n<0;


   if (!n)
     return(1.0);
   if (is_neg)
     n = -n;
   while (!(n & 1))
    {
      x *= x;
      n >>= 1;
    }
   y = x;
   while (n >>= 1)
    {
      x *= x;
      if (n & 1)
        y *= x;
    }

   return((double)(is_neg) ? 1.0/y : y);
}



   LOCAL double
my_sqrt (x)
   double x;
/*
   Computes the "square root" of a given `x' by using Newton's iteration formula.

   sqrt x ==  x    = x - ( f(x ) / f'(x ) ) , n=0,1,2,... , abs( (f(x) * f''(x)) / (f'(x))^2 ) < 1
               n+1    n       n        n
*/
{
   auto double  old_val;
   auto double  new_val;



   if (x == 0.0)
     return(x);
   if (x < 0.0)
     x = -x;
   old_val = x;
   new_val = 1.0;
   LOOP
    {
      new_val = (new_val + x / new_val) / 2.0;
      if (new_val == old_val)
        /*
           Result does not change anymore, we have computed it with success.
        */
        break;
      old_val = new_val;
    }

   return(new_val);
}



   LOCAL double
my_sin (x)
   double x;
/*
   Computes the "sine" of a given `x' iteratively by using a power-sequence
     (Taylor) up to its fifth computed term (... + x^11/11!) and gives a good
     approximation of it with an error less than 1E-3 in case this function
     is used for computing `x' values within the range -1000.0<=x<=1000.0.

     sin x == x^1/1! - x^3/3! + x^5/5! - + ... + (-1)^n * ((x^2n+1) / (2n+1)!) + ... , r=inf
*/
{
   auto     double  new_val=x;
   register int     i;
   register int     sign=0;
   auto     Bool    is_neg=x<0.0;


   /*
      Reduce the given `x' by multiples of Pi till it's less than Pi.
   */
   if (is_neg)
    {
      new_val = -new_val;
      sign++;
    }
   x = MY_PI * 1000.0;
   while (new_val-x >= 0.0)
     new_val -= x;
   x = MY_PI * 100.0;
   while (new_val-x >= 0.0)
     new_val -= x;
   x = MY_PI * 10.0;
   while (new_val-x >= 0.0)
     new_val -= x;
   while (new_val-MY_PI > 0.0)
    {
      new_val -= MY_PI;
      sign++;
    }
   /*
      Let's compute the sine now.
   */
   if (sign & 1)
     new_val = -new_val;
   x = new_val;
   for (i=1 ; i <= 5 ; i++)
    {
      if (i & 1)
        new_val -= (my_pow (x, (i<<1)+1) / my_fac ((Slint)(i<<1)+1));
      else
        new_val += (my_pow (x, (i<<1)+1) / my_fac ((Slint)(i<<1)+1));
    }

   return(new_val);
}



   LOCAL double
my_cos (x)
   double x;
/*
   Computes the "cosine" of a given `x' iteratively by using a power-sequence
     (Taylor) up to its sixth computed term (... + x^12/12!) and gives a good
     approximation of it with an error less than 1E-8 in case this function
     is used for computing `x' values within the range -1000.0<=x<=1000.0.

     cos x == 1 - x^2/2! + x^4/4! - + ... + (-1)^n * ((x^2n) / (2n)!) + ... , r=inf
*/
{
   auto     double  new_val=x;
   register int     i;
   register int     sign=0;
   auto     Bool    is_neg=x<0.0;


   /*
      Reduce the given `x' by multiples of Pi till it's less than Pi/2.
   */
   if (is_neg)
    {
      new_val = -new_val;
      sign++;
    }
   x = MY_PI * 1000.0;
   while (new_val-x >= 0.0)
     new_val -= x;
   x = MY_PI * 100.0;
   while (new_val-x >= 0.0)
     new_val -= x;
   x = MY_PI * 10.0;
   while (new_val-x >= 0.0)
     new_val -= x;
   while (new_val-(MY_PI/2.0) > 0.0)
    {
      new_val -= MY_PI;
      sign++;
    }
   /*
      Let's compute the cosine now.
   */
   x = new_val;
   new_val = 1.0;
   for (i=1 ; i <= 6 ; i++)
    {
      /*
         Manage each single term.
      */
      if (i & 1)
        new_val -= (my_pow (x, i<<1) / my_fac ((Slint)i<<1));
      else
        new_val += (my_pow (x, i<<1) / my_fac ((Slint)i<<1));
    }
   /*
      Adjust the sign of the result.  (Not the smartest solution, I know, but it works!)
   */
   if (   (   is_neg
           && !(sign & 1))
       || (   !is_neg
           && (sign & 1)))
     new_val = -new_val;

   return(new_val);
}
#  endif /* !HAVE_LIBM */



   LOCAL void
dvar_warning (exit_status, dvar, line_buffer, filename, line_number)
   const int   exit_status;
   const int   dvar;
   const char *line_buffer;
   const char *filename;
   const long  line_number;
/*
   Prints an informational message in case an operation on a date variable
     is invalid and terminates program if `warning_level' is set to "MAX" amount.
*/
{
   register int  i;


   *s5 = '\0';
   print_text (stderr, s5);
   i = (int)strlen(filename) + 100;
   if ((Uint)i >= maxlen_max)
     resize_all_strings (i+1, FALSE, __FILE__, (long)__LINE__);
   switch (exit_status)
    {
      case 113:
#  if USE_DE
        sprintf(s5, "Datumvariable `%c' undefiniert in Datei `%s'.", (char)dvar, filename);
#  else /* !USE_DE */
        sprintf(s5, _("Date variable `%c' undefined in file `%s'."), (char)dvar, filename);
#  endif /* !USE_DE */
        break;
      case 112:
#  if USE_DE
        sprintf(s5, "Datumwert ung%sltig (Variable `%c') in Datei `%s'.", UE, (char)dvar, filename);
#  else /* !USE_DE */
        sprintf(s5, _("Invalid date value (variable `%c') in file `%s'."), (char)dvar, filename);
#  endif /* !USE_DE */
        break;
      default:
        /*
           This case MUST be an internal error!
        */
        abort();
    }
   print_text (stderr, s5);
   i = (int)strlen(line_buffer) + 100;
   if ((Uint)i >= maxlen_max)
     resize_all_strings (i+1, FALSE, __FILE__, (long)__LINE__);
#  if USE_DE
   if (!line_number)
     sprintf(s5, "Argument `%s' in Kommandozeile ignoriert", line_buffer);
   else
     sprintf(s5, "Zeile %ld ignoriert: %s", line_number, line_buffer);
#  else /* !USE_DE */
   if (!line_number)
     sprintf(s5, _("Argument `%s' of command line ignored"), line_buffer);
   else
     sprintf(s5, _("Line %ld ignored: %s"), line_number, line_buffer);
#  endif /* !USE_DE */
   print_text (stderr, s5);
   if (warning_level >= WARN_LVL_MAX)
    {
#  if USE_DE
      strcpy(s5, "Abbruch!");
#  else /* !USE_DE */
      strcpy(s5, _("Abort!"));
#  endif /* !USE_DE */
      print_text (stderr, s5);
      my_exit (exit_status);
    }
}
#endif /* USE_RC */
