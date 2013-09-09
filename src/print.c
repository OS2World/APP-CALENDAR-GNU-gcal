/*
*  print.c:  Create, construct and print the calendar sheet(s).
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
static char rcsid[]="$Id: print.c 2.40 1997/03/17 02:04:00 tom Exp $";
#endif



/*
*  Include header files.
*/
#include "tailor.h"
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
IMPORT void
print_all_holidays __P_((      Bool init_data,
                         const Bool detect));
#if USE_RC
/*
************************************************** Defined in `rc-use.c'.
*/
IMPORT void
rc_use __P_((void));
#endif /* USE_RC */
/*
************************************************** Defined in `tty.c'.
*/
IMPORT void
print_text __P_((FILE *fp,
                 char *text_line));
/*
************************************************** Defined in `utils.c'.
*/
#if USE_RC
IMPORT Bool
get_actual_date __P_((void));
#endif
IMPORT const char *
short3_day_name __P_((const int day));
IMPORT const char *
short_day_name __P_((const int day));
IMPORT const char *
day_name __P_((const int day));
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
#if USE_RC
IMPORT Bool
valid_date __P_((const int day,
                 const int month,
                 const int year));
#endif
IMPORT int
week_number __P_((const int day,
                  const int month,
                  const int year));
#ifdef USE_RC
IMPORT int
weekno2doy __P_((      int week,
                 const int year));
#endif
/*
************************************************** Defined in `print.c'.
*/
EXPORT void
print_calendar __P_((void));
LOCAL void
fill_year_vector __P_((int year));
LOCAL Bool
print_unhighlighted_date __P_((Bool marker_flag));
LOCAL Bool
print_highlighted_date __P_((const Bool  last_item,
                             const Bool  marker_flag,
                             const char *hls_start,
                             const int   hls_slen,
                             const char *hls_end,
                             const int   hls_elen));
#if __cplusplus
}
#endif



/*
*  Declare public(extern) variables.
*/
IMPORT const int    dvec[];                  /* Amount of days in months */
IMPORT Greg_struct *greg;                    /* Points to the used Gregorian Reformation date */
IMPORT Ml_struct   *month_list;              /* Used if a list/range of months/years is given */
IMPORT Hls_struct   ehls1s;                  /* Effective hls 1 start (current day) */
IMPORT Hls_struct   ehls1e;                  /* Effective hls 1 end (current day) */
IMPORT Hls_struct   ehls2s;                  /* Effective hls 2 start (holiday) */
IMPORT Hls_struct   ehls2e;                  /* Effective hls 2 end (holiday) */
IMPORT int          len_year_max;            /* String length of the maximum year able to compute */
IMPORT int          len_dayname_max;         /* Maximum string length of a textual day_name() */
IMPORT int          start_day;               /* -s<0,1...7|day name> */
IMPORT int          day;                     /* Current day */
IMPORT int          month;                   /* Current month */
IMPORT int          year;                    /* Current year */
IMPORT int          act_day;                 /* Actual day */
IMPORT int          act_month;               /* Actual month */
IMPORT int          act_year;                /* Actual year */
IMPORT int          fiscal_month;            /* Starting month of a fiscal year */
IMPORT int          out_rows;                /* Number of month rows of a year calendar */
IMPORT int          out_cols;                /* Number of month columns of ... */
IMPORT int          fmt_len;                 /* Format length of a standard/special/both day */
IMPORT int          is_leap_year;            /* Is current year a leap year? */
IMPORT char         hd_ldays[];              /* Vector of holiday dates (legal days) */
IMPORT char        *s1;                      /* General purpose text buffer */
IMPORT char        *s2;                      /* General purpose text buffer */
IMPORT char        *s3;                      /* General purpose text buffer */
IMPORT char        *s4;                      /* General purpose text buffer */
#if USE_RC
IMPORT int          rc_period;               /* Amount of period of fixed dates */
IMPORT Bool         rc_use_flag;             /* -c */
IMPORT Bool         rc_period_flag;          /* [-c]<<<<n>>[<d|w|+|-]>|[0]`mmdd'|[0]`mmww[w]'<n>> */
IMPORT Bool         rc_period_list;          /* [-c]l */
IMPORT Bool         rc_week_year_flag;       /* [-c<<n>>]w */
IMPORT Bool         rc_forwards_flag;        /* [-c<<n>|w|m|y>]+ */
IMPORT Bool         rc_backwards_flag;       /* [-c<<n>|w|m|y>]- */
IMPORT Bool         is_date_given;           /* Is a command (explicit date) given in the command line? */
IMPORT Bool         is_1month_mode;          /* [-c]<n>w and complete week is in month */
IMPORT Bool         is_2month_mode;          /* [-c]<n>w and only part of week is in month */
#endif
IMPORT Bool         use_short3_day_name;     /* 3 char day name format specifier given in date format? */
IMPORT Bool         use_day_zeroleaded;      /* Day leaded with zeroes format specifier given in date format? */
IMPORT Bool         use_year_zeroleaded;     /* Year leaded with zeroes format specifier given in date format? */
IMPORT Bool         special_calsheet_flag;   /* -i */
IMPORT Bool         suppr_cal_flag;          /* -u */
IMPORT Bool         highlight_flag;          /* -H<yes> or -H<no> */
IMPORT Bool         cal_with_weekno;         /* -K */
IMPORT Bool         cal_special_flag;        /* -j */
IMPORT Bool         cal_both_dates_flag;     /* -jb */
IMPORT Bool         holiday_flag;            /* -n|N */
IMPORT Bool         is_fiscal_year;          /* `:' char found in argument (`mm':`yyyy') */
IMPORT Bool         is_3month_mode;          /* Argument is "." or ".+" or ".-" */
IMPORT Bool         is_3month_mode2;         /* Argument is ".." -> current quarter of actual year */
IMPORT Bool         is_ext_year;             /* Is an extended list/range of years given? */
IMPORT Bool         is_ext_list;             /* Is an extended list of months/years given? */
IMPORT Bool         is_ext_range;            /* Is an extended range of months/years given? */
IMPORT Bool         is_special_range;        /* Is a special range of a selected month of years given? */



/*
   Define local(static) variables.
*/
LOCAL int  year_vector[VEC_ELEMS];      /* Standard or special year dates */
LOCAL int  special_vector[VEC_ELEMS];   /* Special year dates only */



   PUBLIC void
print_calendar ()
/*
   Prints one or more single month/year calendar(s).
*/
{
   auto     Slint  amount=0L;
   auto     Slint  count;
   register int    tmp_ad=act_day;
   register int    i;
   register int    j;
   register int    d;
   register int    m;
   register int    mm;
   register int    yy;
   register int    outer_end=(special_calsheet_flag) ? MONTH_COLS : DAY_MAX;
   register int    inner_end;
   register int    blanks_between=(special_calsheet_flag) ? 5 : 2;
   register int    hday;
   auto     int    hmonth;
   auto     int    this_day;
   auto     int    this_month;
   auto     Bool   is_marked=FALSE;
   auto     Bool   marker_flag=FALSE;
   auto     Bool   backwards=FALSE;


   if (cal_special_flag)
     act_day = day_of_year (tmp_ad, act_month, act_year);
   /*
      Evaluate the entries of `month_list':
        Set the actual list/range/fiscal year mode and
        initialize the controlling variables of the main loop.
   */
   if (is_special_range)
    {
      backwards = (Bool)((*month_list).year > month_list[1].year);
      if (backwards)
        amount = (*month_list).year - month_list[1].year + 1L;
      else
        amount = month_list[1].year - (*month_list).year + 1L;
      month = (*month_list).month;
      year = (*month_list).year;
    }
   else
     if (!is_ext_range)
       for (i=0 ; month_list[i].month ; i++)
        {
          if (!month_list[i].year)
           {
             if (year != act_year)
               month_list[i].year = year;
             else
               month_list[i].year = act_year;
           }
          amount++;
        }
     else
      {
        if (!is_ext_year)
         {
           if (!(*month_list).year)
             (*month_list).year = act_year;
           if (!month_list[1].year)
             month_list[1].year = act_year;
           backwards = (Bool)(   (*month_list).year > month_list[1].year
                              || (   ((*month_list).year == month_list[1].year)
                                  && ((*month_list).month > month_list[1].month)));
           if (backwards)
             amount = ((((*month_list).year - 1L) - month_list[1].year) * MONTH_MAX)
                        + (*month_list).month + ((MONTH_MAX - month_list[1].month) + 1L);
           else
             amount = (((month_list[1].year - 1L) - (*month_list).year) * MONTH_MAX)
                        + month_list[1].month + ((MONTH_MAX - (*month_list).month) + 1L);
           month = (*month_list).month;
           year = (*month_list).year;
         }
        else
         {
           if (is_fiscal_year)
            {
              backwards = (Bool)((*month_list).year > month_list[1].year);
              if (backwards)
                amount = ((*month_list).year - month_list[1].year) + 1L;
              else
                amount = (month_list[1].year - (*month_list).year) + 1L;
              year = (*month_list).year;
            }
           else
            {
              backwards = (Bool)((*month_list).month > month_list[1].month);
              if (backwards)
                amount = ((*month_list).month - month_list[1].month) + 1L;
              else
                amount = (month_list[1].month - (*month_list).month) + 1L;
              year = (*month_list).month;
            }
         }
      }
   if (is_fiscal_year)
     fiscal_month = (*month_list).month;
   if (!fiscal_month)
     fiscal_month = act_month;
   if (!amount)
     amount++;
#if USE_RC
   /*
      A [-c]<n>w option and no explicit date given:
        Set the correct month/year to display.
   */
   if (   rc_week_year_flag
       && !rc_period_list
       && !is_date_given)
    {
      i = weekno2doy (rc_period, act_year);
      if (i != -WEEK_MAX)
       {
         if (   i < DAY_MIN
             || i+DAY_MAX-1 > DAY_LAST+is_leap_year)
          {
            is_2month_mode=is_fiscal_year = TRUE;
            (*month_list).month=fiscal_month = MONTH_MAX;
            if (i < DAY_MIN)
              (*month_list).year = --year;
          }
         else
          {
            (void)doy2date (i, is_leap_year, &day, &month);
            m = month;
            (void)doy2date (i+DAY_MAX-1, is_leap_year, &day, &month);
            if (m != month)
             {
               is_2month_mode=is_fiscal_year = TRUE;
               month=fiscal_month = m;
             }
            else
              is_1month_mode = TRUE;
            (*month_list).month = month;
          }
         if (is_2month_mode)
          {
            if (cal_both_dates_flag)
             {
               out_rows = B2_OUT_ROWS;
               out_cols = B2_OUT_COLS;
             }
            else
             {
               out_rows = A2_OUT_ROWS;
               out_cols = A2_OUT_COLS;
             }
          }
       }
    }
#endif
   /*
      All necessary global initializations done, so let's enter the loop.
   */
   for (count=0L ; count < amount ; count++)
    {
      /*
         If the loop must be processed multiple,
           we have to re-initialize the affected variables according to actual mode.
      */
      mm=yy = 0;
      if (   !is_ext_list
          && !is_ext_range
          && !is_special_range)
       {
         /*
            If a month calendar of only the current year is wanted
              resp., a year calendar of only the current year is wanted:
              Initialize the touched variables.
         */
         month = month_list[(int)count].month;
         if (month_list[(int)count].year)
           year = month_list[(int)count].year;
         if (count)
           yy = year;
#if USE_RC
         /*
            Modify the actual date.
         */
         if (   rc_period_flag
             && (   rc_forwards_flag
                 || rc_backwards_flag)
             && !rc_period_list
             && !is_date_given)
          {
            if (cal_special_flag)
              i = act_day;
            else
              i = day_of_year (tmp_ad, act_month, act_year);
            if (rc_forwards_flag)
             {
               /*
                  Correction in case date occurs during Gregorian Reformation period.
               */
               if (   (act_year == greg->year)
                   && (i < day_of_year (greg->first_day, greg->month, greg->year))
                   && (i+rc_period >= day_of_year (greg->first_day, greg->month, greg->year)))
                 i += (greg->last_day - greg->first_day + 1);
               i += rc_period;
             }
            else
             {
               /*
                  Correction in case date occurs during Gregorian Reformation period.
               */
               if (   (act_year == greg->year)
                   && (i > day_of_year (greg->last_day, greg->month, greg->year))
                   && (i-rc_period <= day_of_year (greg->last_day, greg->month, greg->year)))
                 i -= (greg->last_day - greg->first_day + 1);
               i -= rc_period;
             }
            if (   (i > 0)
                && (i < DAY_LAST+is_leap_year+1))
             {
               (void)doy2date (i, is_leap_year, &day, &month);
               act_month = month;
               if (cal_special_flag)
                 act_day = day_of_year (day, month, act_year);
               else
                 act_day = day;
             }
          }
#endif /* USE_RC */
       }
      else
        if (is_ext_list)
         {
           if (!is_ext_year)
            {
              month = month_list[(int)count].month;
              if (   count
                  && (year == month_list[(int)count].year))
                yy = year;
              else
                year = month_list[(int)count].year;
            }
           else
            {
              month = 0;
              if (is_fiscal_year)
               {
                 if (   count
                     && (fiscal_month == month_list[(int)count].month)
                     && (year == month_list[(int)count].year))
                  {
                    mm = fiscal_month;
                    yy = year;
                  }
                 else
                  {
                    fiscal_month = month_list[(int)count].month;
                    year = month_list[(int)count].year;
                  }
               }
              else
               {
                 if (   count
                     && (year == month_list[(int)count].month))
                   yy = year;
                 else
                   year = month_list[(int)count].month;
               }
            }
         }
        else
          if (   (   is_ext_range
                  || is_special_range)
              && count)
           {
             if (   !is_special_range
                 && !is_ext_year)
              {
                yy = year;
                if (backwards)
                 {
                   month--;
                   if (month < MONTH_MIN)
                     month = MONTH_MAX, year--;
                 }
                else
                 {
                   month++;
                   if (month > MONTH_MAX)
                     month = MONTH_MIN, year++;
                 }
              }
             else
              {
                if (!is_special_range)
                  month = 0;
                if (backwards)
                  year--;
                else
                  year++;
              }
           }
          else
            if (   !is_special_range
                && is_ext_year)
              month = 0;
      if (   yy != year
          || (   is_fiscal_year
              && (mm != fiscal_month)))
       {
         is_leap_year = (days_of_february (year) == 29);
         if (count)
           for (i=0 ; i < VEC_ELEMS ; i++)
             year_vector[i]=special_vector[i] = 0;
         /*
            Build the year calendar data structure.
         */
         fill_year_vector (year);
         /*
            Get the dates of the eternal holidays; used for highlighting the calendar.
         */
         if (   (   (year == EASTER_MIN-1)
                 && (fiscal_month > MONTH_MIN))
             || (   (year >= EASTER_MIN)
                 && (year <= EASTER_MAX)))
           print_all_holidays (TRUE, TRUE);
         else
           if (count)
             for (i=0 ; i < HD_MAX ; i++)
               hd_ldays[i] = '\0';
       }
      else
        /*
           Get the actual dates of holidays; used for highlighting the calendar.
        */
        if (   (   (year == EASTER_MIN-1)
                && (fiscal_month > MONTH_MIN))
            || (   (year >= EASTER_MIN)
                && (year <= EASTER_MAX)))
          print_all_holidays (TRUE, TRUE);
      *s1 = '\0';
      /*
         All necessary initializations done.
           So print a month calendar / year calendar of the required year.
      */
      if (   !is_ext_year
          && !is_fiscal_year
#if USE_RC
          && !is_2month_mode
#endif
          && (   month
              || is_ext_list
              || is_ext_range
              || is_special_range))
       {
         /*
            If no explicit request for suppressing the calendar sheet is given
              in the command line (`-u' option), create it!  (What else is the
              expense in this source file good for?)
         */
         if (!suppr_cal_flag)
          {
            /*
               Print the month calendar sheet and if needed the fixed dates, which are
                 related to the month and the eternal holidays related to the month.
            */
            print_text (stdout, s1);
            if (special_calsheet_flag)
             {
               /*
                  Print the month calendar in the special format:
                    Initialize the terminating value of the inner loop.
               */
               inner_end = DAY_MAX;
               /*
                  Print the month header in a centered manner.
               */
               i = ((fmt_len * DAY_MAX) >> 1)
                   + ((strlen(month_name (month)) + len_year_max + 1) >> 1) - len_year_max;
               if (use_year_zeroleaded)
                {
                  i = ((fmt_len * DAY_MAX) >> 1)
                      + ((strlen(month_name (month)) + len_year_max + 1) >> 1) - len_year_max;
                  sprintf(s1, "%*s %0*d", i, month_name (month), len_year_max, year);
                }
               else
                {
                  sprintf(s1, "%d", year);
                  d = (int)strlen(s1);
                  i = ((fmt_len * DAY_MAX) >> 1)
                      + ((strlen(month_name (month)) + d + 1) >> 1) - d;
                  sprintf(s1, "%*s %d", i, month_name (month), year);
                }
               print_text (stdout, s1);
               /*
                  Print the day names.
               */
               for (i=DAY_MIN ; i <= DAY_MAX ; i++)
                {
                  if (   use_short3_day_name
                      && (fmt_len > FMT_LEN_MIN))
                    sprintf(s2, "%*s", fmt_len, short3_day_name (SDAY(i, start_day)));
                  else
                    sprintf(s2, "%*s", fmt_len, short_day_name (SDAY(i, start_day)));
                  strcat(s1, s2);
                }
               if (cal_with_weekno)
                {
                  /*
                     The short week text which must be a proper abbreviation of "week"
                       consists of 2 letters and is separated by 1 space character of
                       the month calendar.
                  */
#if USE_DE
                  sprintf(s2, " %2s", "KW");
#else /* !USE_DE */
                  sprintf(s2, " %2s", _("CW"));
#endif /* !USE_DE */
                  strcat(s1, s2);
                }
             }
            else
             {
               /*
                  Print the month calendar in the standard format:
                    Initialize the terminating value of the inner loop.
               */
               inner_end = MONTH_COLS;
               /*
                  Print the month header.
               */
               if (use_year_zeroleaded)
                 sprintf(s1, "%s %0*d", month_name (month), len_year_max, year);
               else
                 sprintf(s1, "%s %d", month_name (month), year);
               print_text (stdout, s1);
               if (cal_with_weekno)
#if USE_DE
                 sprintf(s3, "%-*s", len_dayname_max+blanks_between, "Woche");
#else /* !USE_DE */
                 sprintf(s3, "%-*s", len_dayname_max+blanks_between, _("Week"));
#endif /* !USE_DE */
             }
            print_text (stdout, s1);
            /*
               Print the days.
            */
            for (i=1 ; i <= outer_end ; i++)
             {
               if (!special_calsheet_flag)
                 /*
                    Print the day names.
                 */
                 sprintf(s1, "%-*s", len_dayname_max+blanks_between,
                         day_name (SDAY(i, start_day)));
               for (d=1 ; d <= inner_end ; d++)
                {
                  /*
                     Compute the days position in `year_vector'.
                  */
                  if (special_calsheet_flag)
                    day = (month - 1) * VEC_BLOCK - 1
                          + (i - 1) * DAY_MAX + d;
                  else
                    day = (month - 1) * VEC_BLOCK - 1
                          + d * DAY_MAX - MONTH_COLS + i - 1;
                  /*
                     Is `day' the actual day?
                  */
                  if (   highlight_flag
                      && (year_vector[day] == act_day)
                      && (month == act_month)
                      && (year == act_year))
                   {
                     if (   is_marked
                         && (ehls1s.len == 1))
                       marker_flag = print_highlighted_date (d==inner_end, is_marked,
                                                             "", 0,
                                                             ehls1e.seq, ehls1e.len);
                     else
                       is_marked=marker_flag = print_highlighted_date (d==inner_end, is_marked,
                                                                       ehls1s.seq, ehls1s.len,
                                                                       ehls1e.seq, ehls1e.len);
                   }
                  else
                   {
                     /*
                        Is `day' a legal holiday?
                     */
                     j = 0;
                     if (year_vector[day])
                      {
                        if (cal_special_flag)
                          (void)doy2date (year_vector[day], is_leap_year, &this_day, &month);
                        else
                          this_day = year_vector[day];
                        if (hd_ldays[((month-1)*MONTH_LAST)+(this_day-1)])
                          j = 1;
                      }
                     if (   j
                         && highlight_flag)
                      {
                        /*
                           `day' is a legal holiday!
                        */
                        if (   is_marked
                            && (ehls2s.len == 1))
                          marker_flag = print_highlighted_date (d==inner_end, is_marked,
                                                                "", 0,
                                                                ehls2e.seq, ehls2e.len);
                        else
                          is_marked=marker_flag = print_highlighted_date (d==inner_end, is_marked,
                                                                          ehls2s.seq, ehls2s.len,
                                                                          ehls2e.seq, ehls2e.len);
                      }
                     else
                      {
                        /*
                           `day' is no holiday!
                        */
                        marker_flag = print_unhighlighted_date (marker_flag);
                        is_marked = FALSE;
                      }
                   }
                  if (   cal_with_weekno
                      && (   (   special_calsheet_flag
                              && (d == inner_end))
                          || (   !special_calsheet_flag
                              && (i == 1))))
                   {
                     j = SPECIAL_VALUE;
                     if (special_calsheet_flag)
                      {
                        j = 0;
                        day -= (DAY_MAX - 1);
                        if (day >= 0)
                         {
                           while (   (j < DAY_MAX)
                                  && (day+j < VEC_ELEMS)
                                  && !year_vector[day+j])
                             j++;
                           if (   (j != DAY_MAX)
                               && (day+j < VEC_ELEMS))
                            {
                              if (cal_special_flag)
                               {
                                 (void)doy2date (year_vector[day+j], is_leap_year, &day, &month);
                                 j = week_number (day, month, year);
                               }
                              else
                                j = week_number (year_vector[day+j], month, year);
                            }
                           else
                             j = SPECIAL_VALUE;
                         }
                        else
                          j = SPECIAL_VALUE;
                      }
                     else
                      {
                        j = 0;
                        while (   (j < DAY_MAX)
                               && (day+j < VEC_ELEMS)
                               && !year_vector[day+j])
                          j++;
                        if (   (j != DAY_MAX)
                            && (day+j < VEC_ELEMS))
                         {
                           if (cal_special_flag)
                            {
                              (void)doy2date (year_vector[day+j], is_leap_year, &day, &month);
                               j = week_number (day, month, year);
                            }
                           else
                             j = week_number (year_vector[day+j], month, year);
                         }
                        else
                          j = SPECIAL_VALUE;
                      }
                     if (j != SPECIAL_VALUE)
                      {
                        /*
                           We convert the computed week number to a week number text
                             (that looks nicer in output).
                        */
                        if (j < 0)
                          /*
                             Week starts in previous year and the first days
                               of the actual year are not in its first week.
                          */
                          sprintf(s2, "%02d/0", -j);
                        else
                          if (!j)
                            /*
                               Week starts in previous year and the first days
                                 of the actual year are in its first week.
                            */
                            sprintf(s2, "%s", "53/1");
                          else
                            /*
                               Week starts in actual year.
                            */
                            sprintf(s2, "%02d", j);
                        if (   !special_calsheet_flag
                            && (strlen(s2) > 2)
                            && (d == 1))
                         {
                           j = (int)strlen(s3);
                           s3[j-2] = '\0';
                         }
                      }
                     else
                       if (   special_calsheet_flag
                           || (   !special_calsheet_flag
                               && (i == 1)))
                         strcpy(s2, "  ");
                     if (special_calsheet_flag)
                      {
                        if (   !is_marked
                            || (   is_marked
                                && (   (ehls1s.len > 1)
                                    || (ehls2s.len > 1))))
                          strcat(s1, " ");
                        strcat(s1, s2);
                      }
                     else
                      {
                        sprintf(s4, "%*s", fmt_len-2, "");
                        strcat(s3, s4);
                        strcat(s3, s2);
                      }
                   }
                }
               /*
                  Print the constructed line.
               */
               print_text (stdout, s1);
               is_marked=marker_flag = FALSE;
             }
            if (   cal_with_weekno
                && !special_calsheet_flag)
             {
               print_text (stdout, s1);
               print_text (stdout, s3);
             }
          }
#if USE_RC
         /*
            Print the fixed dates list.
         */
         if (   rc_use_flag
             && (   is_ext_list
                 || is_ext_range
                 || amount > 1))
           rc_use ();
#endif
         /*
            Print the eternal holiday list.
         */
         if (   holiday_flag
             && (   is_ext_list
                 || is_ext_range
                 || amount > 1)
             && (   (   (year == EASTER_MIN-1)
                     && (fiscal_month > MONTH_MIN))
                 || (   (year >= EASTER_MIN)
                     && (year <= EASTER_MAX))))
           print_all_holidays (FALSE, FALSE);
       }
      else
       {
         /*
            If no explicit request for suppressing the calendar sheet
              is given in command line (`-u' option), create it!
         */
         if (!suppr_cal_flag)
          {
            /*
               Print the year calendar sheet and if needed:
                 The fixed dates related to the year and
                 the eternal holidays related to the year.
            */
            print_text (stdout, s1);
            print_text (stdout, s1);
            if (special_calsheet_flag)
             {
               /*
                  Print the year calendar in the special format:
                    Initialize the terminating value of the inner loop.
               */
               inner_end = DAY_MAX * out_cols;
               /*
                  Compute the position of the year number.
               */
               i = ((out_cols - 1) * blanks_between + out_cols * fmt_len * DAY_MAX) >> 1;
             }
            else
             {
               /*
                  Print the year calendar in the standard format:
                    Initialize the terminating value of the inner loop.
               */
               inner_end = MONTH_COLS * out_cols;
               /*
                  Compute the position of the year number.
               */
               i = (out_cols * fmt_len * MONTH_COLS + 2 + blanks_between) >> 1;
             }
            /*
               Print the year header.
            */
            if (   (   !is_3month_mode
#if USE_RC
                    && !is_2month_mode
#endif
                    && !is_3month_mode2
                    && (fiscal_month > MONTH_MIN)
                    && (year+1 <= YEAR_MAX))
#if USE_RC
                || (   is_2month_mode
                    && (fiscal_month == MONTH_MAX))
#endif
                || (   is_3month_mode
                    && (fiscal_month >= MONTH_MAX-1)))
             {
               if (use_year_zeroleaded)
                 sprintf(s1, "%*s%0*d/%0*d", i-len_year_max, "",
                         len_year_max, year, len_year_max, year+1);
               else
                {
                  sprintf(s1, "%d", year);
                  d = (int)strlen(s1);
                  sprintf(s1, "%d", year+1);
                  d += (int)strlen(s1);
                  sprintf(s1, "%*s%d/%d", i-(d>>1), "", year, year+1);
                }
             }
            else
             {
               if (use_year_zeroleaded)
                 sprintf(s1, "%*s%0*d", i-(len_year_max>>1), "", len_year_max, year);
               else
                {
                  sprintf(s1, "%d", year);
                  d = (int)strlen(s1);
                  sprintf(s1, "%*s%d", i-(d>>1), "", year);
                }
             }
            print_text (stdout, s1);
            print_text (stdout, s1);
            print_text (stdout, s1);
            for (m=0 ; m < out_rows ; m++)
             {
               if (special_calsheet_flag)
                {
                  /*
                     Print the month header in a centered manner.
                  */
                  for (i=1 ; i <= out_cols ; i++)
                   {
                     d = ((fmt_len * DAY_MAX) >> 1)
                         + (strlen(month_name (SMONTH(m*out_cols+i, fiscal_month))) >> 1);
                     sprintf(s2, "%*s", d, month_name (SMONTH(m*out_cols+i, fiscal_month)));
                     strcat(s1, s2);
                     if (i != out_cols)
                      {
                        sprintf(s2, "%*s", (fmt_len*DAY_MAX)-(d-blanks_between), "");
                        strcat(s1, s2);
                      }
                   }
                  print_text (stdout, s1);
                  /*
                     Print the day names.
                  */
                  for (i=1 ; i <= out_cols ; i++)
                   {
                     for (d=DAY_MIN ; d <= DAY_MAX ; d++)
                      {
                        if (   use_short3_day_name
                            && (fmt_len > FMT_LEN_MIN))
                          sprintf(s2, "%*s", fmt_len, short3_day_name (SDAY(d, start_day)));
                        else
                          sprintf(s2, "%*s", fmt_len, short_day_name (SDAY(d, start_day)));
                        strcat(s1, s2);
                      }
                     if (cal_with_weekno)
                      {
                        /*
                           The short week text which must be a proper abbreviation of "week"
                             consists of 2 letters and is separated by 1 space character of
                             the month calendar.
                        */
#if USE_DE
                        sprintf(s2, " %2s", "KW");
#else /* !USE_DE */
                        sprintf(s2, " %2s", _("CW"));
#endif /* !USE_DE */
                        strcat(s1, s2);
                      }
                     if (i != out_cols)
                      {
                        sprintf(s2, "%*s", (cal_with_weekno) ? blanks_between-3 : blanks_between, "");
                        strcat(s1, s2);
                      }
                   }
                }
               else
                {
                  /*
                     Print the month header.
                  */
                  if (use_short3_day_name)
                    strcat(s1, " ");
                  for (i=1 ; i <= out_cols ; i++)
                   {
                     strcpy(s2, month_name (SMONTH(m*out_cols+i, fiscal_month)));
                     d = (int)strlen(s2);
                     sprintf(s2, "%*s%s", 2+blanks_between+1, "",
                             month_name (SMONTH(m*out_cols+i, fiscal_month)));
                     strcat(s1, s2);
                     if (i != out_cols)
                      {
                        sprintf(s2, "%*s", (fmt_len*MONTH_COLS)-(d+2+blanks_between+1), "");
                        strcat(s1, s2);
                      }
                   }
                  if (cal_with_weekno)
                   {
                     if (use_short3_day_name)
                       j = 3;
                     else
                       j = 2;
                     /*
                        The short week text which must be a proper abbreviation of "week"
                          consists of 2 letters and is separated by 1 space character of
                          the month calendar.
                     */
#if USE_DE
                     sprintf(s3, "%-*s", j+blanks_between, "KW");
#else /* !USE_DE */
                     sprintf(s3, "%-*s", j+blanks_between, _("CW"));
#endif /* !USE_DE */
                   }
                  print_text (stdout, s1);
                }
               print_text (stdout, s1);
               /*
                  Print the days.
               */
               for (i=1 ; i <= outer_end ; i++)
                {
                  if (!special_calsheet_flag)
                   {
                     /*
                        Print the day name.
                     */
                     if (use_short3_day_name)
                       sprintf(s1, "%-*s", 3+blanks_between,
                               short3_day_name (SDAY(i, start_day)));
                     else
                       sprintf(s1, "%-*s", 2+blanks_between,
                               short_day_name (SDAY(i, start_day)));
                   }
                  for (d=1 ; d <= inner_end ; d++)
                   {
                     /*
                        Compute the days position in `year_vector'.
                     */
                     if (special_calsheet_flag)
                       day = m * out_cols * VEC_BLOCK - 1
                             + (((d - 1) / DAY_MAX) * VEC_BLOCK)
                             + ((i - 1) * DAY_MAX) + ((d - 1) % DAY_MAX) + 1;
                     else
                       day = m * out_cols * VEC_BLOCK - 1
                             + (d * DAY_MAX - MONTH_COLS) + (i - 1);
                     hday = (m * out_cols) + ((d-1)
                            / ((special_calsheet_flag) ? DAY_MAX : MONTH_COLS)) + 1;
                     hmonth = SMONTH(hday, fiscal_month);
                     /*
                        Is `day' the actual day?
                     */
                     if (   highlight_flag
                         && (year_vector[day] == act_day)
                         && (hmonth == act_month)
                         && (act_year == ((hday>hmonth) ? year+1 : year)))
                      {
                        if (   is_marked
                            && (ehls1s.len == 1))
                          marker_flag = print_highlighted_date (TRUE, is_marked,
                                                                "", 0,
                                                                ehls1e.seq, ehls1e.len);
                        else
                          is_marked=marker_flag = print_highlighted_date ((ehls1s.len==1) ? TRUE : FALSE, is_marked,
                                                                          ehls1s.seq, ehls1s.len,
                                                                          ehls1e.seq, ehls1e.len);
                      }
                     else
                      {
                        /*
                           Is `day' a legal holiday?
                        */
                        j = 0;
                        if (year_vector[day])
                         {
                           if (cal_special_flag)
                             (void)doy2date (year_vector[day+j], (hday>hmonth)
                                               ? days_of_february (year+1) == 29 : is_leap_year,
                                               &this_day, &this_month);
                           else
                            {
                              this_day = year_vector[day];
                              this_month = hmonth;
                            }
                           if (hd_ldays[((this_month-1)*MONTH_LAST)+(this_day-1)])
                             j = 1;
                         }
                        if (   j
                            && highlight_flag)
                         {
                           /*
                              `day' is a legal holiday!
                           */
                           if (   is_marked
                               && (ehls2s.len == 1))
                             marker_flag = print_highlighted_date (TRUE, is_marked,
                                                                   "", 0,
                                                                   ehls2e.seq, ehls2e.len);
                           else
                             is_marked=marker_flag = print_highlighted_date ((ehls2s.len==1) ? TRUE : FALSE, is_marked,
                                                                             ehls2s.seq, ehls2s.len,
                                                                             ehls2e.seq, ehls2e.len);
                         }
                        else
                         {
                           /*
                              `day' is no holiday!
                           */
                           marker_flag = print_unhighlighted_date (marker_flag);
                           is_marked = FALSE;
                         }
                      }
                     if (   cal_with_weekno
                         && (   (   special_calsheet_flag
                                 && !(d % DAY_MAX))
                             || (   !special_calsheet_flag
                                 && (i == 1))))
                      {
                        j = SPECIAL_VALUE;
                        if (special_calsheet_flag)
                         {
                           j = 0;
                           day -= (DAY_MAX - 1);
                           if (day >= 0)
                            {
                              while (   (j < DAY_MAX)
                                     && (day+j < VEC_ELEMS)
                                     && !year_vector[day+j])
                                j++;
                              if (   (j != DAY_MAX)
                                  && (day+j < VEC_ELEMS))
                               {
                                 if (cal_special_flag)
                                  {
                                    (void)doy2date (year_vector[day+j], (hday>hmonth)
                                                      ? days_of_february (year+1) == 29 : is_leap_year,
                                                      &day, &hmonth);
                                    j = week_number (day, hmonth, (hday>hmonth) ? year+1 : year);
                                  }
                                 else
                                   j = week_number (year_vector[day+j], hmonth, (hday>hmonth) ? year+1 : year);
                               }
                              else
                                j = SPECIAL_VALUE;
                            }
                           else
                             j = SPECIAL_VALUE;
                         }
                        else
                         {
                           j = 0;
                           while (   (j < DAY_MAX)
                                  && (day+j < VEC_ELEMS)
                                  && !year_vector[day+j])
                             j++;
                           if (   (j != DAY_MAX)
                               && (day+j < VEC_ELEMS))
                            {
                              if (cal_special_flag)
                               {
                                 (void)doy2date (year_vector[day+j], (hday>hmonth)
                                                   ? days_of_february (year+1) == 29 : is_leap_year,
                                                   &day, &hmonth);
                                  j = week_number (day, hmonth, (hday>hmonth) ? year+1 : year);
                               }
                              else
                                j = week_number (year_vector[day+j], hmonth, (hday>hmonth) ? year+1 : year);
                            }
                           else
                             j = SPECIAL_VALUE;
                         }
                        if (j != SPECIAL_VALUE)
                         {
                           /*
                              We convert the computed week number to a week number text
                                (that looks nicer in output).
                           */
                           if (j < 0)
                             /*
                                Week starts in previous year and the first days
                                  of the actual year are not in its first week.
                             */
                             sprintf(s2, "%02d", -j);
                           else
                             if (!j)
                               /*
                                  Week starts in previous year and the first days
                                    of the actual year are in its first week.
                               */
                               sprintf(s2, "%02d", 1);
                             else
                               /*
                                  Week starts in actual year.
                               */
                               sprintf(s2, "%02d", j);
                         }
                        else
                          if (   special_calsheet_flag
                              || (   !special_calsheet_flag
                                  && (i == 1)))
                            strcpy(s2, "  ");
                        if (special_calsheet_flag)
                         {
                           if (!is_marked)
                             strcat(s1, " ");
                           strcat(s1, s2);
                         }
                        else
                         {
                           sprintf(s4, "%*s", fmt_len-2, "");
                           strcat(s3, s4);
                           strcat(s3, s2);
                         }
                      }
                     /*
                        If we have completed a day line of a month:
                          Concatenate some separating blanks between the months.
                     */
                     if (   special_calsheet_flag
                         && !(d % DAY_MAX)
                         && (d != DAY_MAX*out_cols))
                      {
                        sprintf(s2, "%*s", (cal_with_weekno) ? blanks_between-3 : blanks_between, "");
                        if (   cal_with_weekno
                            && is_marked)
                          strcat(s2, " ");
                        strcat(s1, s2);
                      }
                   }
                  /*
                     Print the constructed line.
                  */
                  print_text (stdout, s1);
                  marker_flag=is_marked = FALSE;
                }
               if (   cal_with_weekno
                   && !special_calsheet_flag)
                {
                  print_text (stdout, s1);
                  print_text (stdout, s3);
                }
               /*
                  Print two NEWLINE characters between the month rows.
               */
               if (m < out_rows-1)
                {
                  print_text (stdout, s1);
                  print_text (stdout, s1);
                }
             }
          }
#if USE_RC
         /*
            Print the fixed dates list.
         */
         if (   rc_use_flag
             && (   is_ext_list
                 || is_ext_range))
           rc_use ();
#endif
         /*
            Print the eternal holiday list.
         */
         if (   is_ext_year
             && holiday_flag
             && (   (   (year == EASTER_MIN-1)
                     && (fiscal_month > MONTH_MIN))
                 || (   (year >= EASTER_MIN)
                     && (year <= EASTER_MAX))))
           print_all_holidays (FALSE, FALSE);
       }
    }
#if USE_RC
   /*
      A [-c]<n>w option and no explicit date given:
        Reset some affected global variables.
   */
   if (   is_1month_mode
       || is_2month_mode)
    {
      is_fiscal_year = TRUE;
      fiscal_month = month;
      year = act_year;
    }
   else
     /*
        Reinitialize actual date.
     */
     if (   rc_period_flag
         && (   rc_forwards_flag
             || rc_backwards_flag)
         && !rc_period_list
         && !is_date_given)
       (void)get_actual_date ();
     else
#endif /* USE_RC */
       if (cal_special_flag)
         act_day = tmp_ad;
}



   LOCAL void
fill_year_vector (year)
   int year;
/*
   Builds the "standard" year vector and the "special" year vector
     according to the delivered `year'.
*/
{
#if !USE_RC
   register int  days_of_feb=days_of_february (year);
#endif
   register int  i;
   register int  j;
   register int  count;
   register int  d;
   register int  m=fiscal_month;


   /*
      First, detect starting day of the fiscal/standard year.
   */
   i = weekday_of_date (DAY_MIN, m, year);
   i=day = SYEAR(i, start_day);
   j=d = 0;
   if (fiscal_month > MONTH_MIN)
     count = day_of_year (DAY_MIN, m, year) - 1;
   else
     count = 0;
   /*
      Then, fill the year vector.
   */
   LOOP
    {
      d++;
      count++;
#if USE_RC
      if (!valid_date (d, m, year))
#else /* !USE_RC */
      if (   (   (m != 2)
              && (d > dvec[m-1]))
          || (   (m == 2)
              && (d > days_of_feb)))
#endif /* !USE_RC */
       {
         j++;
         if (   (m < MONTH_MAX)
             && (j < MONTH_MAX))
          {
            if (fiscal_month > MONTH_MIN)
              i = j * VEC_BLOCK + day;
            else
              i = m * VEC_BLOCK + day;
            d = DAY_MIN;
            m++;
          }
         else
          {
            if (fiscal_month > MONTH_MIN)
             {
               year++;
               if (   j == MONTH_MAX
                   || year > YEAR_MAX)
                 break;
               i = j * VEC_BLOCK + day;
               count=d = DAY_MIN;
               m = MONTH_MIN;
#if !USE_RC
               days_of_feb = days_of_february (year);
#endif
             }
            else
              break;
          }
       }
      if (   (year == greg->year)
          && (m == greg->month)
          && (   (d >= greg->first_day)
              && (d <= greg->last_day)))
        i--;
      else
       {
         day++;
         if (cal_special_flag)
           year_vector[i-1] = count;
         else
           year_vector[i-1] = d;
         special_vector[i-1] = count;
       }
      i++;
      if (day > DAY_MAX)
        day = DAY_MIN;
    }
}



   LOCAL Bool
print_unhighlighted_date (marker_flag)
   Bool marker_flag;
/*
   Prints a date in NON-highlighted manner.
     If the MARKER_FLAG is set to TRUE when entering this function, this indicates,
     that a highlighting  sequence/marking character pair was already displayed
     previously (or better adjacent) in the same row of the line, so it can be set
     to FALSE now and returned by this function, otherwise it will be returned
     untouched (means it was FALSE).  The respection of this MARKER_FLAG is absolutely
     necessary to format the line properly.
*/
{
   if (cal_both_dates_flag)
    {
      if (year_vector[day])
       {
         if (marker_flag)
          {
            if (use_day_zeroleaded)
              sprintf(s2, "%02d(%03d)", year_vector[day], special_vector[day]);
            else
              sprintf(s2, "%2d(%3d)", year_vector[day], special_vector[day]);
            marker_flag = FALSE;
          }
         else
           if (use_day_zeroleaded)
             sprintf(s2, " %02d(%03d)", year_vector[day], special_vector[day]);
           else
             sprintf(s2, " %2d(%3d)", year_vector[day], special_vector[day]);
       }
      else
       {
         if (marker_flag)
          {
            sprintf(s2, "%*s", fmt_len-1, "");
            marker_flag = FALSE;
          }
         else
           sprintf(s2, "%*s", fmt_len, "");
       }
    }
   else
    {
      if (year_vector[day])
       {
         if (marker_flag)
          {
            if (use_day_zeroleaded)
              sprintf(s2, "%0*d", fmt_len-1, year_vector[day]);
            else
              sprintf(s2, "%*d", fmt_len-1, year_vector[day]);
            marker_flag = FALSE;
          }
         else
           if (use_day_zeroleaded)
             sprintf(s2, " %0*d", fmt_len-1, year_vector[day]);
           else
             sprintf(s2, "%*d", fmt_len, year_vector[day]);
       }
      else
       {
         if (marker_flag)
          {
            sprintf(s2, "%*s", fmt_len-1, "");
            marker_flag = FALSE;
          }
         else
           sprintf(s2, "%*s", fmt_len, "");
       }
    }
   strcat(s1, s2);

   return(marker_flag);
}



   LOCAL Bool
print_highlighted_date (last_item, marker_flag, hls_start, hls_slen, hls_end, hls_elen)
   const Bool  last_item;
   const Bool  marker_flag;
   const char *hls_start;
   const int   hls_slen;
   const char *hls_end;
   const int   hls_elen;
/*
   Prints a date in highlighted manner using the given highlighting sequence,
     which consists of the "starting" highlighting sequence HLS_START and the
     "ending/final" highlighting sequence HLS_END, for distinguishing whether
     a real highlighting sequence is given.  The lengths of such a highlighting
     sequence/marking character pair are delivered, too.  If these lengths are
     greater ONE, it's assumed that a "real" (Termcap/ANSI) highlighting sequence
     pair is given, otherwise a marking character pair.  If the MARKER_FLAG is
     set to TRUE when entering this function, this indicates, that a highlighting
     sequence/marking character pair was already displayed previously (or better
     adjacent) in the same row of the line, so only an ending marking character
     may be produced in this case (this doesn't matter to "real" highlighting
     sequences, because their width is zero on the display [if interpreted properly
     by the screen device driver], but a marking character has the width of one),
     otherwise, all components of the marking character pair must be produced.
     The respection of this MARKER_FLAG is absolutely necessary to format the
     line properly.  If `last_item' is set to TRUE, this function does not
     produce a trailing blank character after an ending highlighting sequence
     respectively marking character.  Returns always TRUE to indicate, that a
     marking character/highlighting sequence pair is set by it!
*/
{
   if (cal_both_dates_flag)
    {
      if (use_day_zeroleaded)
        sprintf(s2, "%s%s%02d(%03d)%s%s",
                (((hls_slen>1)&&!marker_flag) ? NO_HLS : ""), hls_start,
                year_vector[day], special_vector[day],
                hls_end, (((hls_elen>1)&&!last_item) ? NO_HLS : ""));
      else
        sprintf(s2, "%s%s%2d(%3d)%s%s",
                (((hls_slen>1)&&!marker_flag) ? NO_HLS : ""), hls_start,
                year_vector[day], special_vector[day],
                hls_end, (((hls_elen>1)&&!last_item) ? NO_HLS : ""));
    }
   else
    {
      if (use_day_zeroleaded)
        sprintf(s2, "%s%s%0*d%s%s",
                (((hls_slen>1)&&!marker_flag) ? NO_HLS : ""), hls_start,
                fmt_len-1, year_vector[day],
                hls_end, (((hls_elen>1)&&!last_item) ? NO_HLS : ""));
      else
        sprintf(s2, "%s%s%*d%s%s",
                (((hls_slen>1)&&!marker_flag) ? NO_HLS : ""), hls_start,
                fmt_len-1, year_vector[day],
                hls_end, (((hls_elen>1)&&!last_item) ? NO_HLS : ""));
    }
   strcat(s1, s2);

   return(TRUE);
}
