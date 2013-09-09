/*
*  hd-use.c:  Manage the eternal holiday list.
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
static char rcsid[]="$Id: hd-use.c 2.40 1997/03/17 02:04:00 tom Exp $";
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
************************************************** Defined in `hd_data.c'.
*/
IMPORT const Cc_struct *
binsearch_cc_id __P_((char *id));
IMPORT void
hdy_standard __P_((      Bool *init_data,
                   const Bool  detected,
                   const int   year,
                         int  *hd_elems,
                   const int   fday,
                   const int   count));
IMPORT void
hdy_christian __P_((      Bool *init_data,
                    const Bool  detected,
                    const int   easter,
                    const int   year,
                          int  *hd_elems,
                    const int   fday,
                    const int   count));
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
IMPORT void
resize_all_strings __P_((const int   amount,
                         const Bool  with_line_buffer,
                         const char *module_name,
                         const long  module_line));
IMPORT int
my_atoi __P_((const char *string));
IMPORT int
asc_sort __P_((const char **a,
               const char **b));
IMPORT int
des_sort __P_((const char **a,
               const char **b));
IMPORT Bool
is_presorted __P_((char **table,
                   int   elems));
IMPORT void
resort __P_((      char **table,
             const int    elems));
IMPORT const char *
day_suffix __P_((int day));
IMPORT const char *
short3_day_name __P_((const int day));
IMPORT const char *
short_day_name __P_((const int day));
IMPORT const char *
day_name __P_((const int day));
IMPORT const char *
short_month_name __P_((const int month));
IMPORT const char *
month_name __P_((const int month));
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
IMPORT int
knuth_easter_formula __P_((const int year));
/*
************************************************** Defined in `hd_use.c'.
*/
EXPORT int
eval_holiday __P_((      int  day,
                   const int  month,
                   const int  year,
                   const int  wd,
                   const Bool forwards));
EXPORT int
decode_date_format __P_((      char  *format_txt,
                               char **result_txt,
                               int    day,
                               int    month,
                         const int    year,
                         const int    doy,
                         const Bool   hls1_set,
                         const Bool   hls2_set,
                         const Bool   fixed_length_names));
EXPORT void
print_all_holidays __P_((      Bool init_data,
                         const Bool detected));
EXPORT void
print_single_holiday __P_((      Bool  init_data,
                           const Bool  detected,
                           const char *holiday_name,
                           const char *country_code,
                           const char *holiday_prefix,
                                 int   day,
                                 int   month,
                           const int   year,
                                 int  *hd_elems,
                           const int   fday,
                           const int   count));
#if __cplusplus
}
#endif



/*
*  Declare public(extern) variables.
*/
IMPORT const int    dvec[];                  /* Amount of days in months */
IMPORT Df_struct   *date_format;             /* Points to the used format string */
IMPORT Greg_struct *greg;                    /* Points to the used Gregorian Reformation date */
IMPORT Hls_struct   ehls1s;                  /* Effective hls 1 start (current day) */
IMPORT Hls_struct   ehls1e;                  /* Effective hls 1 end (current day) */
IMPORT Hls_struct   ehls2s;                  /* Effective hls 2 start (holiday) */
IMPORT Hls_struct   ehls2e;                  /* Effective hls 2 end (holiday) */
#ifdef DJG
IMPORT Usint        testval;                 /* Set to SHRT_MAX for checking the maximum table range */
#else
IMPORT Uint         testval;                 /* Set to INT_MAX for checking the maximum table range */
#endif
IMPORT Uint         maxlen_max;              /* Actual size of all string vectors */
IMPORT int          len_year_max;            /* String length of the maximum year able to compute */
IMPORT int          len_dayname_max;         /* Maximum string length of a textual day_name() */
IMPORT int          len_monthname_max;       /* Maximum string length of a textual month_name() */
IMPORT int          len_suffix_max;          /* Maximum string length of a textual day_suffix() [if any] */
IMPORT int          warning_level;           /* --debug[=0...WARN_LVL_MAX] */
IMPORT int          month;                   /* Current month */
IMPORT int          year;                    /* Current year */
IMPORT int          act_day;                 /* Actual day */
IMPORT int          act_month;               /* Actual month */
IMPORT int          act_year;                /* Actual year */
IMPORT int          fiscal_month;            /* Starting month of a fiscal year */
IMPORT int          is_leap_year;            /* Is current year a leap year? */
IMPORT char        *s1;                      /* General purpose text buffer */
IMPORT char        *s2;                      /* General purpose text buffer */
IMPORT char        *s4;                      /* General purpose text buffer */
IMPORT char        *cc;                      /* The argument `--cc-holidays=CC[+CC+...]' received */
#if USE_RC
IMPORT Bool         rc_enable_hda_flag;      /* [-c]e */
IMPORT Bool         rc_enable_hdl_flag;      /* [-c]E */
IMPORT Bool         is_1month_mode;          /* [-c]<n>w and complete week is in month */
IMPORT Bool         is_2month_mode;          /* [-c]<n>w and only part of week is in month */
#endif
IMPORT Bool         use_year_zeroleaded;     /* Year leaded with zeroes format specifier given in date format? */
IMPORT Bool         standard_hdy;            /* Use the standard holidays */
IMPORT Bool         christian_hdy;           /* Use Christian holidays */
IMPORT Bool         highlight_flag;          /* -H<yes> or -H<no> */
IMPORT Bool         cal_special_flag;        /* -j */
IMPORT Bool         holiday_flag;            /* -n|N */
IMPORT Bool         hd_legal_days_only;      /* -N */
IMPORT Bool         hd_sort_des_flag;        /* [-n|N]- */
IMPORT Bool         hd_special_flag;         /* -jn */
IMPORT Bool         hd_both_dates_flag;      /* -jnb */
IMPORT Bool         hd_title_flag;           /* -X */
IMPORT Bool         is_fiscal_year;          /* `:' char found in argument (`mm':`yyyy') */
IMPORT Bool         is_3month_mode;          /* Argument is "." or ".+" or ".-" */
IMPORT Bool         is_3month_mode2;         /* Argument is ".." -> current quarter of actual year */



/*
   Define public(extern) variables.
*/
PUBLIC char  *hd_table[HD_ELEMS_MAX];   /* Stores the fixed eternal holiday list texts */
PUBLIC char   hd_ldays[HD_MAX];         /* Vector of holiday dates (legal days) */
PUBLIC char   hd_mdays[HD_MAX];         /* Vector of holiday dates (memorial days) */



   PUBLIC int
eval_holiday (day, month, year, wd, forwards)
         int  day;
   const int  month;
   const int  year;
   const int  wd;
   const Bool forwards;
/*
   Detects first (`forwards'==TRUE) or last (`forwards'==!TRUE) appearance
     of weekday `wd' in given date starting the search at `day'
     and returns the day number of `wd' in month.
*/
{
   LOOP
    {
      if (weekday_of_date (day, month, year) == wd)
        break;
      else
        (forwards) ? day++ : day--;
    }
   /*
      Correction for missing days during Gregorian Reformation.
   */
   if (   (year == greg->year)
       && (month == greg->month)
       && (day >= greg->first_day)
       && (day <= greg->last_day))
     day = (forwards) ? eval_holiday (greg->last_day+1, month, year, wd, forwards)
                      : eval_holiday (greg->first_day-1, month, year, wd, forwards);

   return(day);
}



   PUBLIC int
decode_date_format (format_txt, result_txt, day, month, year, doy,
                    hls1_set, hls2_set, fixed_length_names)
         char  *format_txt;
         char **result_txt;
         int    day;
         int    month;
   const int    year;
   const int    doy;
   const Bool   hls1_set;
   const Bool   hls2_set;
   const Bool   fixed_length_names;
/*
   Concatenates the elements of a date (inclusive highlighting) in formatted
     manner to `result_txt' according to the order of format specifiers, which
     are given in the `format_txt.  The core area of the string vector
     `result_txt' (likewise `s4') must be allocated and "filled" properly by
     the caller.  "Filled" either means `result_txt' contains a ZERO terminated
     text at function startup or a previous "*result_txt = '\0';" statement was
     performed by the caller before entering this function.
     If `day' is not zero but `doy' is zero:
       Use day of month and all other GROUPS.
     If `day' is zero but `doy' is not zero:
       Use day of year, insert a "day" text, use all other GROUPS except the month GROUP.
     If `day' is not zero and `doy' is not zero:
       Use both day of month and day of year and all other GROUPS.
     If `day' is zero and `doy' is zero:
       This results to an unmanaged internal error, because this
       case is unchecked due to faster internal processing speed.
     If `hls1_set' is TRUE and `hls2_set' is FALSE:
       Use `ehls1' highlighting sequences/marking characters.
     If `hls1_set' is FALSE and `hls2_set' is TRUE:
       Use `ehls2' highlighting sequences/marking characters.
     If `hls1_set' is FALSE and `hls2_set' is FALSE:
       Use no highlighting sequences/marking characters.
     If `hls1_set' is TRUE and `hls2_set' is TRUE:
       This case is unchecked due to faster internal processing speed
       but doesn't result to an unmanaged internal error, because
       the `ehls1' sequence is used by default.
     If `fixed_length_names' is set to TRUE, the texts returned by the
       `day_name()' and `month_name()' functions are set to a fixed length
       of `len_???name_max' characters (means trailing spaces), otherwise
       these texts are used as is.
     The `format_txt' must have exactly ONE format specifier from the day,
       month and year GROUPS, BOTH format specifiers from the highlighting
       GROUP (%1 must be given before %2), and may have ONE format specifier
       from the weekday name GROUP:
     Day GROUP:
       %d - Day number with leading zeroes
       %D - Day number with leading spaces
       %u - Day number with leading zeroes with trailing day suffix
       %U - Day number with leading spaces with trailing day suffix
     Month GROUP:
       %B - Complete month name
       %b - Abbreviated month name (3 letters)
       %m - Month number with leading zeroes (2
       %M - Month number with leading spaces
     Year GROUP:
       %y - Year number with leading zeroes
       %Y - Year number with leading spaces
       %z - Last two digits of year with leading zeroes
       %Z - Last two digits of year with leading spaces
     Weekday name GROUP:
       %A - Complete weekday name
       %W - Abbreviated weekday name `www' (3 letters)
       %w - Abbreviated weekday name `ww' (2 letters)
     Highlighting GROUP:
       %1 - Starting highlighting sequence
       %2 - Ending highlighting sequence
     Other (no group):
       _  - Blank (' ') character
       \_ - Underscore ('_') character
       \% - Percent ('%') character
   The function return value is the text length of the "real" highlighting
   sequence pair if "real" highlighting must be performed, otherwise zero.
   Uses the global text buffer `s4' internally.
*/
{
   register int   skip_space=0;
   register int   i=(int)strlen(format_txt);
   register int   hls_len=0;
   register int   len=(int)strlen(*result_txt);
   auto     int   d=day;
   auto     Bool  is_marking_char=(Bool)((ehls1s.len==1) ? TRUE : FALSE);
   auto     Bool  is_special_date=FALSE;


   if ((Uint)i+len >= maxlen_max)
     resize_all_strings (i+len+1, FALSE, __FILE__, (long)__LINE__);
   if (   !day
       && doy)
     is_special_date = TRUE;
   if (   (   hls1_set
           || hls2_set)
       && !is_marking_char)
    {
      if (hls1_set)
        hls_len = ehls1s.len + ehls1e.len;
      else
        hls_len = ehls2s.len + ehls2e.len;
    }
   if (is_special_date)
     (void)doy2date (doy, (days_of_february (year)==29), &day, &month);
   do
    {
      i = 0;
      while (   *format_txt
             && (*format_txt != DFORMAT_CHAR)
             && (*format_txt != QUOTE_CHAR))
       {
         if (   !skip_space
             || (   (*format_txt != ' ')
                 && (*format_txt != PSEUDO_BLANK)))
          {
            if (*format_txt == PSEUDO_BLANK)
              s4[i++] = ' ';
            else
              s4[i++] = *format_txt;
          }
         format_txt++;
         if (skip_space)
           skip_space--;
       }
      if (i)
       {
         s4[i] = '\0';
         strcat(*result_txt, s4);
         len += i;
       }
      if (*format_txt)
       {
         if (*format_txt == DFORMAT_CHAR)
           /*
              Format specifier found.
           */
           switch (*++format_txt)
            {
              case '1':
                if (highlight_flag)
                 {
                   if (   !hls1_set
                       && !hls2_set)
                    {
                      if (   !len
                          && is_marking_char)
                       {
                         strcat(*result_txt, NO_HLS);
                         len++;
                       }
                    }
                   else
                     if (len)
                       if (   is_marking_char
                           && (*(*result_txt+(len-1)) == ' '))
                        {
                          *(*result_txt + (len - 1)) = '\0';
                          len--;
                        }
                   if (hls1_set)
                    {
                      strcat(*result_txt, ehls1s.seq);
                      len += ehls1s.len;
                    }
                   else
                     if (hls2_set)
                      {
                        strcat(*result_txt, ehls2s.seq);
                        len += ehls2s.len;
                      }
                 }
                break;
              case '2':
                if (highlight_flag)
                 {
                   if (   is_marking_char
                       && (*(format_txt+1) != ' ')
                       && (*(format_txt+1) != PSEUDO_BLANK)
                       && !hls1_set
                       && !hls2_set)
                    {
                      strcat(*result_txt, NO_HLS);
                      len++;
                    }
                   if (hls1_set)
                    {
                      strcat(*result_txt, ehls1e.seq);
                      len += ehls1e.len;
                      if (is_marking_char)
                        skip_space++;
                    }
                   else
                     if (hls2_set)
                      {
                        strcat(*result_txt, ehls2e.seq);
                        len += ehls2e.len;
                        if (is_marking_char)
                          skip_space++;
                      }
                 }
                break;
              case 'd':
              case 'D':
              case 'u':
              case 'U':
                if (is_special_date)
                 {
                   if (islower(*format_txt))
                     sprintf(s4, "%03d", doy);
                   else
                     sprintf(s4, "%3d", doy);
                   len += 3;
                 }
                else
                 {
                   if (islower(*format_txt))
                     sprintf(s4, "%02d", day);
                   else
                     sprintf(s4, "%2d", day);
                   len += 2;
                 }
                strcat(*result_txt, s4);
                if (tolower(*format_txt) == 'u')
                 {
                   if (is_special_date)
                     sprintf(s4, "%-*s", len_suffix_max, day_suffix (doy));
                   else
                     sprintf(s4, "%-*s", len_suffix_max, day_suffix (day));
                   strcat(*result_txt, s4);
                   len += len_suffix_max;
                 }
                if (doy)
                 {
                   if (!d)
                    {
#if USE_DE
                      strcat(*result_txt, " Tag");
                      len += (int)strlen(" Tag");
#else /* !USE_DE */
                      strcat(*result_txt, _(" day"));
                      len += (int)strlen(_(" day"));
#endif /* !USE_DE */
                    }
                   else
                    {
                      if (islower(*format_txt))
                        sprintf(s4, "(%03d)", doy);
                      else
                        sprintf(s4, "(%3d)", doy);
                      strcat(*result_txt, s4);
                      len += 5;
                    }
                 }
                break;
              case 'A':
                if (fixed_length_names)
                 {
                   sprintf(s4, "%-*s", len_dayname_max, day_name (weekday_of_date (day, month, year)));
                   len += len_dayname_max;
                 }
                else
                 {
                   sprintf(s4, "%s", day_name (weekday_of_date (day, month, year)));
                   len += (int)strlen(s4);
                 }
                strcat(*result_txt, s4);
                break;
              case 'w':
              case 'W':
                if (islower(*format_txt))
                 {
                   strcat(*result_txt, short_day_name (weekday_of_date (day, month, year)));
                   len += 2;
                 }
                else
                 {
                   strcat(*result_txt, short3_day_name (weekday_of_date (day, month, year)));
                   len += 3;
                 }
                break;
              case 'B':
              case 'b':
                if (!is_special_date)
                 {
                   if (islower(*format_txt))
                    {
                      strcat(*result_txt, short_month_name (month));
                      len += 3;
                    }
                   else
                    {
                      if (fixed_length_names)
                       {
                         sprintf(s4, "%-*s", len_monthname_max, month_name (month));
                         len += len_monthname_max;
                       }
                      else
                       {
                         sprintf(s4, "%s", month_name (month));
                         len += (int)strlen(s4);
                       }
                      strcat(*result_txt, s4);
                    }
                 }
                else
                  if (len)
                    if (   (   *(format_txt+1) == ' '
                            || *(format_txt+1) == PSEUDO_BLANK)
                        && (*(*result_txt+(len-1)) == ' '))
                      skip_space++;
                break;
              case 'm':
              case 'M':
                if (!is_special_date)
                 {
                   if (islower(*format_txt))
                     sprintf(s4, "%02d", month);
                   else
                     sprintf(s4, "%2d", month);
                   strcat(*result_txt, s4);
                   len += 2;
                 }
                else
                  if (len)
                    if (   (   *(format_txt+1) == ' '
                            || *(format_txt+1) == PSEUDO_BLANK)
                        && (*(*result_txt+(len-1)) == ' '))
                      skip_space++;
                break;
              case 'y':
              case 'Y':
                if (islower(*format_txt))
                  sprintf(s4, "%0*d", len_year_max, year);
                else
                  sprintf(s4, "%*d", len_year_max, year);
                strcat(*result_txt, s4);
                len += len_year_max;
                break;
              case 'z':
              case 'Z':
                sprintf(s4, "%0*d", len_year_max, year);
                s4[0] = s4[len_year_max-2];
                s4[1] = s4[len_year_max-1];
                s4[2] = '\0';
                if (   !islower(*format_txt)
                    && (s4[0] == '0'))
                  s4[0] = ' ';
                strcat(*result_txt, s4);
                len += 2;
                break;
              default:
                /*
                   This case must be an internal error!
                */
                abort();
            }
         else
           /*
              Quote character found.
           */
           switch (*++format_txt)
            {
              case PSEUDO_BLANK:
              case DFORMAT_CHAR:
                sprintf(s4, "%c", *format_txt);
                strcat(*result_txt, s4);
                len++;
                break;
              default:
                sprintf(s4, "%c%c", QUOTE_CHAR, *format_txt);
                strcat(*result_txt, s4);
                len +=2;
            }
       }
      else
        break;
    } while (*++format_txt);

   return(hls_len);
}



   PUBLIC void
print_all_holidays (init_data, detected)
         Bool init_data;
   const Bool detected;
/*
   Generate and display all holidays (global `hd_table') in a sorted manner.
*/
{
   auto     const Cc_struct  *ptr_cc;
   register       int         easter;
   register       int         i=0;
   register       int         j=0;
   register       int         k;
   register       int         fday=0;
   register       int         count=(fiscal_month>MONTH_MIN) ? 2 : 1;
   auto           int         hd_elems=0;
   auto           char       *ptr_char;


   if (   init_data
       || detected)
    {
      if (init_data)
        for (i=0 ; i < HD_MAX ; i++)
          hd_ldays[i]=hd_mdays[i] = '\0';
      if (fiscal_month > MONTH_MIN)
       {
         fday = day_of_year (DAY_MIN, fiscal_month, year);
         j = is_leap_year;
         i = year;
       }
      do
       {
         count--;
         easter = knuth_easter_formula (year);
         /*
            Use the global holiday management functions.
         */
         if (standard_hdy)
           hdy_standard (&init_data, detected, year, &hd_elems, fday, count);
         if (christian_hdy)
           hdy_christian (&init_data, detected, easter, year, &hd_elems, fday, count);
         if (cc != (char *)NULL)
          {
            /*
               Use the country specific holiday management functions.
            */
            ptr_char = cc;
            do
             {
               /*
                  Copy a single country code id into `s1'.
               */
               for (k=0 ; k < 2 ; k++)
                 s1[k] = *ptr_char++;
               s1[k] = '\0';
               ptr_cc = binsearch_cc_id (s1);
               if (ptr_cc == (Cc_struct *)NULL)
                 abort();   /* This case MUST be an internal error (`cc_holidays[]' corrupt')! */
               else
                 (*ptr_cc->Cc_hdy_handler)(&init_data, detected, easter, year, &hd_elems, fday, count);
               /*
                  Now skip a possibly trailing CONNECT_SEP.
               */
               if (*ptr_char)
                 ptr_char++;
             } while (*ptr_char);
          }
         if (fiscal_month > MONTH_MIN)
          {
            year++;
            is_leap_year = (days_of_february (year) == 29);
          }
       } while (count);
      if (fiscal_month > MONTH_MIN)
       {
         is_leap_year = j;
         year = i;
       }
    }
   if (   hd_elems
       && holiday_flag)
    {
      /*
         At last, sort the eternal holiday dates only if they are not presorted.
      */
      if (hd_elems > 1)
       {
         if (!is_presorted (hd_table, hd_elems))
          {
            if (hd_sort_des_flag)
              qsort((VOID_PTR)hd_table, hd_elems, sizeof *hd_table, (Cmp_func)des_sort);
            else
              qsort((VOID_PTR)hd_table, hd_elems, sizeof *hd_table, (Cmp_func)asc_sort);
          }
         else
           if (hd_sort_des_flag)
             /*
                `hd_table[]' is presorted and must be shown in descending sort order,
                  rearrange its internal sort order from ascending to descending sort order.
             */
             resort (hd_table, hd_elems);
       }
    }
   else
     if (!detected)
      {
        register int   m2=fiscal_month+1;
        register int   m3=fiscal_month+2;
        register int   tmp_month=month;
        auto     Bool  hd_title_shown=FALSE;
        auto     Bool  tmp_ify=is_fiscal_year;


        /*
           Display leading title text for a current year.
        */
        if (   hd_title_flag
#if USE_RC
            && !is_1month_mode
            && !is_2month_mode
#endif
            && (   (   !is_3month_mode
                    && !is_3month_mode2
                    && (   (   !is_fiscal_year
                            && !month)
                        || (   is_fiscal_year
                            && (fiscal_month == MONTH_MIN))))
                || is_3month_mode2
                || (   is_3month_mode
                    && (fiscal_month < MONTH_MAX-1))))
         {
           /*
              Now display a leading NEWLINE character before the title text.
           */
           *s1 = '\0';
           print_text (stdout, s1);
           if (hd_title_flag)
            {
              /*
                 Now display the leading title text only if not suppressed
                   by an explicitly given -X command line option.
              */
#if USE_DE
              i = (int)strlen(HD_LIST_TITLE);
              if (use_year_zeroleaded)
                sprintf(s1, "%s:%*sDas Jahr %0*d ist %s Schaltjahr",
                        HD_LIST_TITLE, LEN_HD_NAME-i+2, "",
                        len_year_max, year, (is_leap_year) ? "EIN" : "KEIN");
              else
                sprintf(s1, "%s:%*sDas Jahr %d ist %s Schaltjahr",
                        HD_LIST_TITLE, LEN_HD_NAME-i+2, "",
                        year, (is_leap_year) ? "EIN" : "KEIN");
#else /* !USE_DE */
              i = (int)strlen(_("Eternal holiday list"));
              if (use_year_zeroleaded)
               {
                 if (is_leap_year)
                   sprintf(s1, _("%s:%*sThe year %0*d is A leap year"),
                           _("Eternal holiday list"), LEN_HD_NAME-i+2, "", len_year_max, year);
                 else
                   sprintf(s1, _("%s:%*sThe year %0*d is NO leap year"),
                           _("Eternal holiday list"), LEN_HD_NAME-i+2, "", len_year_max, year);
               }
              else
               {
                 if (is_leap_year)
                   sprintf(s1, _("%s:%*sThe year %d is A leap year"),
                           _("Eternal holiday list"), LEN_HD_NAME-i+2, "", year);
                 else
                   sprintf(s1, _("%s:%*sThe year %d is NO leap year"),
                           _("Eternal holiday list"), LEN_HD_NAME-i+2, "", year);
               }
#endif /* !USE_DE */
              print_text (stdout, s1);
              if (hd_table[0] != (char *)NULL)
                print_text (stdout, s1);
            }
           hd_title_shown = TRUE;
         }
        /*
           Now display the (complete) eternal holiday list.
        */
        if (   is_3month_mode
#if USE_RC
            || is_1month_mode
            || is_2month_mode
#endif
            || is_3month_mode2)
         {
           /*
              Set the range of the 2|3 months', which have to be respected.
           */
           if (fiscal_month >= MONTH_MAX-1)
            {
              m3 = MONTH_MIN;
              if (fiscal_month == MONTH_MAX)
              {
                m2 = MONTH_MIN;
                m3++;
              }
            }
           is_fiscal_year = FALSE;
           month = fiscal_month;
         }
        i=j = 0;
        while (   (hd_table[i] != (char *)NULL)
               && (i < HD_ELEMS_MAX))
         {
           /*
              One month resp., 2|3-month calendar mode:
                Get holiday month of holiday text.
           */
           if (   is_3month_mode
#if USE_RC
               || is_1month_mode
               || is_2month_mode
#endif
               || is_3month_mode2
               || (   month
                   && !is_fiscal_year))
            {
              s1[0] = hd_table[i][len_year_max];
              s1[1] = hd_table[i][len_year_max+1];
              s1[2] = '\0';
              j = my_atoi (s1);
            }
           /*
              Display dates of eternal holiday list:
                Year calendar  --> Display all eternal holidays.
                Month calendar --> Display only those eternal holidays
                                     which refer to current month.
           */
           if (   is_fiscal_year
               || !month
               || month == j
#if USE_RC
               || (   is_1month_mode
                   && (month == j))
               || (   is_2month_mode
                   && (j == m2))
#endif
               || (   (   is_3month_mode
                       || is_3month_mode2)
                   && (   j == m2
                       || j == m3)))
            {
              if (   !hd_legal_days_only
                  || (   hd_legal_days_only
                      && (*(*(hd_table+i)+LEN_HD_NAME+len_year_max+4+2) != *DIS_HLS_PREF)
                      && (*(*(hd_table+i)+LEN_HD_NAME+len_year_max+4+2) != *DIS_HLS_PREF2)))
               {
                 /*
                    Month calendar mode:
                      Display a leading NEWLINE character only in case
                        an eternal holiday refers to the current month.
                 */
                 if (!hd_title_shown)
                  {
                    /*
                       Now display a leading NEWLINE character before the title text.
                    */
                    *s1 = '\0';
                    print_text (stdout, s1);
                    if (hd_title_flag)
                     {
                       /*
                          Now display the leading title text only if not
                            explicitly suppressed by a given -X command line option.
                       */
#if USE_DE
                       sprintf(s1, "%s:", HD_LIST_TITLE);
#else /* !USE_DE */
                       sprintf(s1, "%s:", _("Eternal holiday list"));
#endif /* !USE_DE */
                       print_text (stdout, s1);
                       print_text (stdout, s1);
                     }
                    hd_title_shown = TRUE;
                  }
                 /*
                    Display the eternal holiday text.
                 */
                 sprintf(s1, "%s", *(hd_table+i)+len_year_max+4+1);
                 print_text (stdout, s1);
               }
            }
           free(hd_table[i]);
           hd_table[i++] = (char *)NULL;
         }
        if (   is_3month_mode
#if USE_RC
            || is_1month_mode
            || is_2month_mode
#endif
            || is_3month_mode2)
         {
           month = tmp_month;
           is_fiscal_year = tmp_ify;
         }
      }
}



   PUBLIC void
print_single_holiday (init_data, detected, holiday_name, country_code,
                      holiday_prefix, day, month, year, hd_elems, fday, count)
         Bool  init_data;
   const Bool  detected;
   const char *holiday_name;
   const char *country_code;
   const char *holiday_prefix;
         int   day;
         int   month;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Generates a single holiday date text in formatted manner and stores the
     result into global `hd_table[]'.
     ONLY if `holiday_prefix' is a '-' (DIS_HLS_PREF) or '*' (DIS_HLS_PREF2)
     character, this holiday is not stored into global `hd_ldays' and
     will not be highlighted in the generated list, but is stored into
     global `hd_mdays'!
     If `holiday_prefix' is a '#' character, this marks the case the
     holiday is only valid in some parts of the country, but is stored
     into global `hd_ldays' and will be highlighted in the generated list.
     If `holiday_prefix' is a '\0' or any other character, the same rules
     concering to '#' character applies.
     If `month' is set to zero, it's assumed that the delivered date is a
     date relative to the Easter Sunday's date (`day' contains the according
     day number of year), otherwise the date is an absolute date of the `year'.
*/
{
   register int  i=day;
   register int  hd;
   register int  hd2;


   if (!month)
     doy2date (i, is_leap_year, &day, &month);
   hd=hd2 = day_of_year (day, month, year);
   if (   (year >= EASTER_MIN)
       && (year <= EASTER_MAX)
       && (day >= DAY_MIN)
       && (day <= dvec[month-1])
       && (   !fday
           || (   count
               && (hd >= fday))
           || (   !count
               && (hd < fday-(days_of_february (year-1)==29)))))
    {
      if (   holiday_flag
#if USE_RC
          || rc_enable_hda_flag
          || rc_enable_hdl_flag
#endif
         )
       {
         register int  d=day;


         sprintf(s1, " (%s)", country_code);
         strncpy(s2, holiday_name, LEN_HD_NAME);
         s2[LEN_HD_NAME-1] = '\0';
         strcat(s2, s1);
         s2[LEN_HD_NAME-1] = '\0';
         sprintf(s1, "%0*d%02d%02d %-*s  %1s ",
                 len_year_max, year, month, day, LEN_HD_NAME-1, s2, holiday_prefix);
         strcpy(s2, s1);
         i = hd - ((cal_special_flag) ? act_day : day_of_year (act_day, act_month, act_year));
         if (!hd_both_dates_flag)
          {
            if (!hd_special_flag)
              hd2 = 0;
            else
              d = 0;
          }
         if (   !i
             && (year == act_year))
           (void)decode_date_format (date_format->format, &s2, d, month, year, hd2,
                                     TRUE, FALSE, TRUE);
         else
           if (   (*holiday_prefix != *DIS_HLS_PREF)
               && (*holiday_prefix != *DIS_HLS_PREF2))
             (void)decode_date_format (date_format->format, &s2, d, month, year, hd2,
                                       FALSE, TRUE, TRUE);
           else
             (void)decode_date_format (date_format->format, &s2, d, month, year, hd2,
                                       FALSE, FALSE, TRUE);
         /*
            Add the day displacement, which is relative to the "actual date"
              to the eternal holiday text.
         */
         if (   (   fday
                 && (   (   (year == act_year-1)
                         && (month >= fiscal_month))
                     || (   (year == act_year+1)
                         && (month < fiscal_month))))
             || year == act_year)
          {
            if (   fday
                && (year != act_year))
             {
               if (year == act_year-1)
                 /*
                    Eternal holiday to compute is in "actual year - 1".
                 */
                 i = hd - (DAY_LAST + is_leap_year
                     + ((cal_special_flag) ? act_day : day_of_year (act_day, act_month, act_year)));
               else
                 /*
                    Eternal holiday to compute is in "actual year + 1".
                 */
                 i = hd + (DAY_LAST + (days_of_february (act_year) == 29)
                     - ((cal_special_flag) ? act_day : day_of_year (act_day, act_month, act_year)));
             }
            if (i)
             {
#if USE_DE
               sprintf(s1, " = %+4d Tag%s", i, (abs(i)==1) ? "" : "e");
#else /* !USE_DE */
               if (abs(i) == 1)
                 sprintf(s1, _(" = %+4d day"), i);
               else
                 sprintf(s1, _(" = %+4d days"), i);
#endif /* !USE_DE */
               strcat(s2, s1);
             }
          }
         /*
            Store the constructed "raw" line in `hd_table[]'.
         */
         if (*hd_elems < HD_ELEMS_MAX)
          {
            hd_table[*hd_elems] = (char *)my_malloc (strlen(s2)+1,
                                                     124, __FILE__, ((long)__LINE__)-1,
                                                     "hd_table[hd_elems]", *hd_elems);
            strcpy(hd_table[(*hd_elems)++], s2);
          }
       }
      /*
         This case can occur only, if the code was extended by more
           eternal holidays and the table size for `hd_table[]', namely
           HD_ELEMS_MAX, isn't increased/adjusted correctly so this
           warning/informational is for maintainers only!!
      */
      if (   (warning_level >= 0)
          && (*hd_elems >= HD_ELEMS_MAX))
       {
#  if USE_DE
         sprintf(s1, "%s%s hat nun %d Maximaleintr%sge!%s",
                 ((ehls1s.len!=1) ? ehls1s.seq : ""), HD_LIST_TITLE, HD_ELEMS_MAX, AE,
                 ((ehls1s.len!=1) ? ehls1e.seq : ""));
#  else /* !USE_DE */
         sprintf(s1, _("%s%s contains %d maximum entries now!%s"),
                 ((ehls1s.len!=1) ? ehls1s.seq : ""), _("Eternal holiday list"), HD_ELEMS_MAX,
                 ((ehls1s.len!=1) ? ehls1e.seq : ""));
#  endif /* !USE_DE */
         print_text (stderr, s1);
       }
      if (init_data)
        for (i=0 ; i < HD_MAX ; i++)
          hd_ldays[i]=hd_mdays[i] = '\0';
      if (detected)
       {
         if (   *holiday_prefix == *DIS_HLS_PREF
             || *holiday_prefix == *DIS_HLS_PREF2)
           hd_mdays[((month-1)*MONTH_LAST)+(day-1)] = '@';
         else
           hd_ldays[((month-1)*MONTH_LAST)+(day-1)] = '@';
       }
    }
   else
     if (init_data)
       for (i=0 ; i < HD_MAX ; i++)
         hd_ldays[i]=hd_mdays[i] = '\0';
}
