/*
*  rc-insert.c:  Inserts a line (of a resource file) into `rc_elems_table[]'.
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
static char rcsid[]="$Id: rc-insert.c 2.40 1997/03/31 02:04:00 tom Exp $";
#  endif



/*
*  Include header files.
*/
#  if HAVE_CTYPE_H
#    include <ctype.h>
#  endif
#  include "gcal.h"
/*
*  The REGEX stuff.
*/
#  if HAVE_GNU_RE_COMPILE_PATTERN
#    if HAVE_SYS_TYPES_H
#      include <sys/types.h>
#    endif
#    if HAVE_REGEX_H
#      include <regex.h>
#    endif
#  endif
#  if HAVE_POSIX_REGCOMP
#    if HAVE_SYS_TYPES_H
#      include <sys/types.h>
#    endif
#    if HAVE_REGEX_H
#      include <regex.h>
#    endif
#  endif
#  if HAVE_RE_COMP
IMPORT int re_exec();
#  endif
#  if HAVE_REGCMP
IMPORT char *regex();
#  endif
#  if HAVE_V8_REGCOMP
#    include "regexp.h"
#  endif



/*
*  Function prototypes.
*/
#  if __cplusplus
extern "C"
{
#  endif
/*
************************************************** Defined in `rc-utils.c'.
*/
#  if HAVE_V8_REGCOMP
IMPORT void
regerror __P_((char *msg));
#  endif
IMPORT Bool
rc_valid_day __P_((const char *date_text,
                   const int   day,
                   const int   month,
                   const int   year));
IMPORT Bool
rc_valid_period __P_((      char *date_text,
                      const int   d,
                      const int   m,
                      const int   y,
                      const int   incr_year,
                      const int   decr_year));
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
IMPORT void
nth_weekday_of_month __P_((      int  *d,
                                 int  *m,
                                 int  *y,
                           const int  *n,
                                 Bool *is_weekday_mode));
IMPORT void
next_date __P_((int *day,
                int *month,
                int *year));
IMPORT void
num2date __P_((Ulint  julian_days,
               int   *day,
               int   *month,
               int   *year));
IMPORT Slint
d_between __P_((const int d1,
                const int m1,
                const int y1,
                const int d2,
                const int m2,
                const int y2));
IMPORT Slint
w_between __P_((const int d1,
                const int m1,
                const int y1,
                const int d2,
                const int m2,
                const int y2));
IMPORT Slint
m_between __P_((const int m1,
                const int y1,
                const int m2,
                const int y2));
IMPORT char *
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
IMPORT int
the_phase __P_((      Bool *is_full_new,
                const int   day,
                const int   month,
                const int   year,
                const int   hour,
                const int   min));
#  else /* !HAVE_LIBM */
IMPORT int
the_moonphase __P_((      Bool *is_full_new,
                          int   day,
                          int   month,
                          int   year,
                    const int   hour,
                    const int   min));
#  endif /* !HAVE_LIBM */
IMPORT void
draw_moon __P_((const int    age,
                const int    lines,
                      char **string));
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
#  if !HAVE_STRSTR
IMPORT char *
my_strstr __P_((const char *text,
                const char *pattern));
#  endif /* !HAVE_STRSTR */
IMPORT Bool
get_actual_date __P_((void));
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
IMPORT Ulint
date2num __P_((const int day,
               const int month,
               const int year));
IMPORT int
weekday_of_date __P_((const int day,
                      const int month,
                      const int year));
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
week_number __P_((const int day,
                  const int month,
                  const int year));
/*
************************************************** Defined in `rc-insert.c'.
*/
EXPORT void
insert_line_into_table __P_((      char *line_buffer,
                             const char *filename,
                             const long  line_number,
                                   int  *rc_elems,
                                   int   len_date,
                                   int   print_twice));
#  if __cplusplus
}
#  endif



/*
*  Declare public(extern) variables.
*/
IMPORT const int    dvec[];                  /* Amount of days in months */
IMPORT Hls_struct   ehls1s;                  /* Effective hls 1 start (current day) */
IMPORT Hls_struct   ehls1e;                  /* Effective hls 1 end (current day) */
IMPORT Hls_struct   ehls2s;                  /* Effective hls 2 start (holiday) */
IMPORT Hls_struct   ehls2e;                  /* Effective hls 2 end (holiday) */
IMPORT Dvar_struct  rc_dvar[];               /* Date variables a[=`mmdd']...z[] (`yyyy'@{a|b|...|z}[[-]<n>]) */
IMPORT Line_struct *lptrs2;                  /* Pointers to different parts of a (resource file) line */
#  ifdef DJG
IMPORT Usint        testval;                 /* Set to SHRT_MAX for checking the maximum table range */
#  else
IMPORT Uint         testval;                 /* Set to INT_MAX for checking the maximum table range */
#  endif
IMPORT Uint         maxlen_max;              /* Actual size of all string vectors */
IMPORT Uint         rc_elems_max;            /* Actual size of `rc_elems_table[]' */
IMPORT int          rc_bio_axis_len;         /* Length of a single axis of a biorhythm text graphics bar */
IMPORT int          rc_moonimage_lines;      /* Number of lines of a moonphase text graphics image */
IMPORT int          len_year_max;            /* String length of the maximum year able to compute */
IMPORT int          warning_level;           /* --debug[=0...WARN_LVL_MAX] */
IMPORT int          start_day;               /* -s1<0,1...7|day name> */
IMPORT int          year;                    /* Current year */
IMPORT int          act_min;                 /* Actual minute */
IMPORT int          act_hour;                /* Actual hour */
IMPORT int          act_day;                 /* Actual day */
IMPORT int          act_month;               /* Actual month */
IMPORT int          act_year;                /* Actual year */
IMPORT int          fiscal_month;            /* Starting month of a fiscal year */
IMPORT int          len_fil_wt;              /* Filler length of week number text */
IMPORT int          len_the_text;            /* Actual size of text buffer of "text-part" of a line */
IMPORT int          incr_year;               /* Indicates whether event appears in next year too */
IMPORT int          decr_year;               /* Indicates whether event appears in previous year too */
IMPORT int          d;                       /* Day of event found in line */
IMPORT int          m;                       /* Month of event found in line */
IMPORT int          y;                       /* Year of event found in line */
IMPORT int          d_buf;                   /* Buffered day of event */
IMPORT int          m_buf;                   /* Buffered month of event */
IMPORT int          hn;                      /* The `N'th weekday of month' displacement value */
IMPORT int          hwd;                     /* The weekday number of `N'th weekday of month'*/
IMPORT char         hd_ldays[];              /* Vector of holiday dates (legal days) */
IMPORT char        *the_text;                /* Text buffer of "text-part" of a line */
IMPORT char        *s1;                      /* General purpose text buffer */
IMPORT char        *s2;                      /* General purpose text buffer */
IMPORT char        *s6;                      /* General purpose text buffer */
IMPORT char         hc;                      /* The mode specifying character */
IMPORT char       **rc_elems_table;          /* Stores the valid fixed date texts */
IMPORT char        *rc_filter_day;           /* Argument used for filtering fixed date days */
IMPORT char        *rc_filter_period;        /* Argument used for filtering fixed date periods */
IMPORT char        *rc_filter_text;          /* REGEX used for filtering fixed date texts */
IMPORT char        *rc_bio_emo_lit;          /* The biorhythm's "Emo" text */
IMPORT char        *rc_bio_int_lit;          /* The biorhythm's "Int" text */
IMPORT char        *rc_bio_phy_lit;          /* The biorhythm's "Phy" text */
IMPORT Bool         rc_enable_fn_flag;       /* [-c]a */
IMPORT Bool         rc_bypass_shell_cmd;     /* [-c]B */
IMPORT Bool         rc_weekno_flag;          /* [-c]k */
IMPORT Bool         rc_tomorrow_flag;        /* [-c]t */
IMPORT Bool         rc_week_flag;            /* [-c]w */
IMPORT Bool         shell_escape_done;       /* Stores whether a %shell escape special text is run */
IMPORT Bool         highlight_flag;          /* -H<yes> or -H<no> */
IMPORT Bool         is_2dvar;                /* Reference to a date variable found in line */
IMPORT Bool         is_2easter;              /* Reference to Easter Sundays date found in line */
/*
*  The REGEX stuff.
*/
#  if HAVE_GNU_RE_COMPILE_PATTERN
IMPORT struct re_pattern_buffer   regpattern;
#  endif
#  if HAVE_POSIX_REGCOMP
IMPORT regex_t                    regpattern;
#  endif
#  if HAVE_RE_COMP
IMPORT int                        re_pattern;
#  endif
#  if HAVE_REGCMP
IMPORT char                      *cpattern;
#  endif
#  if HAVE_V8_REGCOMP
IMPORT struct regexp             *regpattern;
#  endif



   PUBLIC void
insert_line_into_table (line_buffer, filename, line_number, rc_elems, len_date, print_twice)
         char *line_buffer;
   const char *filename;
   const long  line_number;
         int  *rc_elems;
         int   len_date;
         int   print_twice;
/*
   Inserts a line into `rc_elems_table[]',
     but before this the line is checked and evaluated first
     for $... text variables and then for %... special texts.
*/
{
   auto     Slint  num;
   register int    i;
   register int    j;
   register int    k;
   register int    kpos=0;
   register int    len_fn=len_year_max+5;   /* Position of the `(' of the "(FILENAME)" text */
   register int    tmp_year=year;
   auto     int    len;
   auto     int    rlen=0;
   auto     int    dd;
   auto     int    rdd=0;
   auto     int    mm;
   auto     int    rmm=0;
   auto     int    yy;
   auto     int    ryy=0;
   auto     int    nn;
   auto     int    rnn=0;
   auto     int    hhn;
   auto     int    rhn;
   auto     int    hhwd;
   auto     int    rhwd;
   auto     int    bio_emo_phase;
   auto     int    bio_emo_waxes;
   auto     int    bio_int_phase;
   auto     int    bio_int_waxes;
   auto     int    bio_phy_phase;
   auto     int    bio_phy_waxes;
   auto     int    bio_critical_day;
   auto     int    bio_positive_day;
   auto     int    bio_negative_day;
   static   char   date_text[23];
   static   char   bio_emo_ptext[7];
   static   char   bio_int_ptext[7];
   static   char   bio_phy_ptext[7];
   auto     char  *ptr_date_text=date_text;
   auto     char  *ptr_char=line_buffer+len_date;
   auto     char   hhc;
   auto     char   rhc='\0';
   static   Bool   inclusive_date_map[DAY_LAST+2];
   static   Bool   exclusive_date_map[DAY_LAST+2];
   auto     Bool   ie_date_maps_set=FALSE;
   auto     Bool   hls1_set;
   auto     Bool   hls2_set;
   auto     Bool   is_weekday_mode;
   auto     Bool   ris_weekday_mode;
   auto     Bool   moon_min_max;
   auto     Bool   ok;
   auto     Bool   got_shell_cmd=FALSE;
   auto     Bool   print_line=TRUE;
   auto     Bool   is_obsolete_whitespace=FALSE;


   if (rc_enable_fn_flag)
     len_fn += (int)strlen(filename) + 3;   /* +3 due to the "(THE_REAL_FILENAME_ITSELF) " text */
   /*
      Skip the delimiting whitespace characters between the "date"-part and the "text"-part.
   */
   while (isspace(*ptr_char))
     ptr_char++;
   if (*ptr_char)
    {
      /*
         If the "text"-part starts with a QUOTEd whitespace character, exchange
           the QUOTE character by a PSEUDO_QUOTE character ('\001') to keep the
           internal sorting order intact.
      */
      if (   (*ptr_char == QUOTE_CHAR)
          && isspace(*(ptr_char + 1)))
        *ptr_char = PSEUDO_QUOTE;
      if ((Uint)len_the_text < maxlen_max)
       {
         len_the_text = (int)maxlen_max;
         the_text = (char *)my_realloc ((VOID_PTR)the_text, len_the_text,
                                        124, __FILE__, ((long)__LINE__)-1,
                                        "the_text", len_the_text);
       }
      strcpy(the_text, ptr_char);
    }
   else
     /*
        No "text"-part given.
     */
     *the_text = '\0';
   do
    {
      /*
         If `--filter-period=ARG' is given and ARG matches the
           date of the fixed date, suppress this fixed date!
      */
      if (rc_filter_period != (char *)NULL)
        print_line = rc_valid_period (rc_filter_period, d, m, y, incr_year, decr_year);
      /*
         If `--filter-day=ARG' is given and ARG matches the
           date of the fixed date, suppress this fixed date!
      */
      if (   print_line
          && (rc_filter_day != (char *)NULL))
        print_line = rc_valid_day (rc_filter_day, d, m, year+incr_year-decr_year);
      if (!print_line)
        ok = TRUE;
      else
       {
         /*
            Check line for %... special texts and quoted $... text variables.
         */
         j=k = 0;
         ok=hls1_set=hls2_set = FALSE;
         *date_text = '\0';
       }
      while (!ok)
       {
         while (*(the_text + j))
          {
            if ((Uint)k >= maxlen_max-(Uint)len_fn)
              resize_all_strings (maxlen_max<<1, FALSE, __FILE__, (long)__LINE__);
            if (   *(the_text + j) == RC_SPECIAL_TEXT_CHAR
                || *(the_text + j) == RC_TVAR_CHAR)
             {
               if (j)
                {
                  /*
                     Check if the %... special text or the $... text variable is quoted.
                  */
                  if (*(the_text + (j - 1)) == QUOTE_CHAR)
                    k--;
                  else
                    if (*(the_text + j) == RC_SPECIAL_TEXT_CHAR)
                      break;
                }
               else
                 if (*(the_text + j) == RC_SPECIAL_TEXT_CHAR)
                   break;
             }
            if (   is_obsolete_whitespace
                && isspace(*(the_text + j)))
             {
               is_obsolete_whitespace = FALSE;
               j++;
             }
            else
              s1[k++] = *(the_text + j++);
          }
         if ((Uint)k >= maxlen_max-(Uint)len_fn)
           resize_all_strings (maxlen_max<<1, FALSE, __FILE__, (long)__LINE__);
         if (*(the_text + j))
          {
            auto char  special_text_char;


            /*
               A '%' character (special text prefix) found!
            */
            j++;
            special_text_char = *(the_text + j);
            if (  (   !got_shell_cmd
                   && (   special_text_char == RC_IDATE_CHAR
                       || special_text_char == RC_EDATE_CHAR))
                || special_text_char == RC_TDATE_CHAR
                || special_text_char == RC_WDNAME_CHAR
                || special_text_char == RC_WDNAME3_CHAR
                || special_text_char == RC_WDNAME2_CHAR
                || special_text_char == RC_WDNR_M1_2_S7_CHAR
                || special_text_char == RC_WDNR_M0_2_S6_CHAR
                || special_text_char == RC_WDNR_M1_2_S7S_CHAR
                || special_text_char == RC_WDNR_M0_2_S6S_CHAR
                || special_text_char == RC_WDNR_S1_2_S7_CHAR
                || special_text_char == RC_WDNR_S0_2_S6_CHAR
                || special_text_char == RC_WDNR_S1_2_S7S_CHAR
                || special_text_char == RC_WDNR_S0_2_S6S_CHAR
                || special_text_char == RC_WDNR_X1_2_X7_CHAR
                || special_text_char == RC_WDNR_X0_2_X6_CHAR
                || special_text_char == RC_WDNR_X1_2_X7S_CHAR
                || special_text_char == RC_WDNR_X0_2_X6S_CHAR
                || special_text_char == RC_DOYNR_NZ_CHAR
                || special_text_char == RC_DOYNR_LZ_CHAR
                || special_text_char == RC_DOYNR_NZS_CHAR
                || special_text_char == RC_DOYNR_LZS_CHAR
                || special_text_char == RC_DAYNR_NZ_CHAR
                || special_text_char == RC_DAYNR_LZ_CHAR
                || special_text_char == RC_DAYNR_NZS_CHAR
                || special_text_char == RC_DAYNR_LZS_CHAR
                || special_text_char == RC_MONTHNAME_CHAR
                || special_text_char == RC_MONTHNAME3_CHAR
                || special_text_char == RC_MONTHNR_NZ_CHAR
                || special_text_char == RC_MONTHNR_LZ_CHAR
                || special_text_char == RC_MONTHNR_NZS_CHAR
                || special_text_char == RC_MONTHNR_LZS_CHAR
                || special_text_char == RC_YEARNR_NZ_CHAR
                || special_text_char == RC_YEARNR_LZ_CHAR
                || special_text_char == RC_WEEKNR_CHAR
                || special_text_char == RC_BYEAR_CHAR
                || special_text_char == RC_BYEAR_S_CHAR
                || (   (special_text_char == RC_YEAR_DIFF_CHAR)
                    && *(the_text + j + 1)
                    && !isspace(*(the_text + j + 1)))
                || special_text_char == RC_MOON_NZ_CHAR
                || special_text_char == RC_MOON_LZ_CHAR
                || special_text_char == RC_MOON_IMAGE_CHAR
                || special_text_char == RC_BIO_CHAR
                || special_text_char == RC_BIO_BAR_CHAR)
             {
               j++;
               if (special_text_char == RC_TDATE_CHAR)
                {
                  auto Slint  jdate;


                  /*
                     %textual_date[[+|-]<n>] special text found.
                  */
                  i = 0;
                  if (   *(the_text + j) == *ASC_LIT
                      || *(the_text + j) == *DES_LIT)
                    s2[i++] = *(the_text + j++);
                  while (isdigit(*(the_text + j)))
                   {
                     if ((Uint)i >= maxlen_max)
                       resize_all_strings (maxlen_max<<1, FALSE, __FILE__, (long)__LINE__);
                     s2[i++] = *(the_text + j++);
                   }
                  s2[i] = '\0';
                  num = atol(s2);
                  jdate = (Slint)date2num (d, m, year+incr_year-decr_year);
                  if (   (jdate+num <= (Slint)date2num (dvec[MONTH_MAX-1], MONTH_MAX, YEAR_MAX))
                      && (jdate+num >= 1L))
                   {
                     num2date (jdate+num, &dd, &mm, &yy);
                     sprintf(s2, "%02d-%s-%0*d", dd,
                             short_month_name (mm), len_year_max, yy);
                   }
                  else
                    *s2 = '\0';
                  /*
                     And copy the expanded special text into the target string.
                  */
                  i = 0;
                  while (s2[i])
                   {
                     if ((Uint)k >= maxlen_max-(Uint)len_fn)
                       resize_all_strings (maxlen_max<<1, FALSE, __FILE__, (long)__LINE__);
                     s1[k++] = s2[i++];
                   }
                }
               else
                {
                  auto Bool  is_ie=FALSE;
                  auto Bool  is_range=FALSE;
                  auto Bool  dflt_yy_set=FALSE;
                  auto Bool  dflt_ryy_set=FALSE;


                  /*
                     Check if a range of dates is given.
                  */
                  if (   special_text_char == RC_IDATE_CHAR
                      || special_text_char == RC_EDATE_CHAR)
                   {
                     is_ie = TRUE;
                     if (!ie_date_maps_set)
                      {
                        for (i=1 ; i < DAY_LAST+2 ; i++)
                          inclusive_date_map[i] = !(exclusive_date_map[i] = TRUE);
                        ie_date_maps_set = !(*inclusive_date_map = *exclusive_date_map = FALSE);
                      }
                     rhc = '\0';
                     rlen=rdd=rmm=ryy=rnn = 0;
                     ptr_char = the_text + j;
                     while (   *ptr_char
                            && !isspace(*ptr_char)
                            && (*ptr_char != RC_DRANGE_CHAR))
                       ptr_char++;
                     if (*ptr_char == RC_DRANGE_CHAR)
                       is_range = TRUE;
                   }
                  if (is_range)
                    *ptr_char = '\0';
                  (void)rc_get_date (the_text+j, lptrs2, FALSE, &is_weekday_mode, &dd, &mm, &yy, &nn,
                                     &len, &hhc, &hhn, &hhwd, filename, line_number, the_text+j-2, TRUE);
                  j += len;
                  if (is_range)
                   {
                     j++;
                     (void)rc_get_date (the_text+j, lptrs2, FALSE, &ris_weekday_mode, &rdd, &rmm, &ryy, &rnn,
                                        &rlen, &rhc, &rhn, &rhwd, filename, line_number, the_text+j, TRUE);
                     j += rlen;
                   }
                  if (is_ie)
                   {
                     if (!len)
                       dflt_yy_set = TRUE;
                     if (!rlen)
                       dflt_ryy_set = TRUE;
                   }
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
                     All %?[STARTING_DATE][#[ENDING_DATE]] special texts.
                  */
                  if (is_ie)
                   {
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
                   }
                  else
                   /*
                      All other %?[DATE] special texts.
                   */
                   {
                     /*
                        Assume current month/day.
                     */
                     if (!mm)
                       mm = m;
                     if (!dd)
                       dd = d;
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
                        Manage the %?[STARTING_DATE[#ENDING_DATE]] special texts.
                     */
                     if (is_ie)
                      {
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
                     else
                      /*
                         Manage the %?[DATE] special texts.
                      */
                      {
                        if (   (special_text_char != RC_BYEAR_CHAR)
                            && (special_text_char != RC_BYEAR_S_CHAR)
                            && (special_text_char != RC_YEAR_DIFF_CHAR))
                          yy += incr_year;
                        i = weekday_of_date (dd, mm, yy);
                        switch (special_text_char)
                         {
                           case RC_WDNAME_CHAR:
                             /*
                                %complete_weekday_name special text.
                             */
                             sprintf(s2, "%s", day_name (i));
                             break;
                           case RC_WDNAME3_CHAR:
                             /*
                                %3_letter_weekday_name special text.
                             */
                             sprintf(s2, "%s", short3_day_name (i));
                             break;
                           case RC_WDNAME2_CHAR:
                             /*
                                %2_letter_weekday_name special text.
                             */
                             sprintf(s2, "%s", short_day_name (i));
                             break;
                           case RC_WDNR_M1_2_S7_CHAR:
                             /*
                                %weekday_number special text.
                             */
                             sprintf(s2, "%d", i);
                             break;
                           case RC_WDNR_M0_2_S6_CHAR:
                             /*
                                %weekday_number special text.
                             */
                             sprintf(s2, "%d", i-1);
                             break;
                           case RC_WDNR_M1_2_S7S_CHAR:
                             /*
                                %weekday_number_with_ordinal_number_suffix special text.
                             */
                             sprintf(s2, "%d%s", i, day_suffix (i));
                             break;
                           case RC_WDNR_M0_2_S6S_CHAR:
                             /*
                                %weekday_number_with_ordinal_number_suffix special text.
                             */
                             sprintf(s2, "%d%s", i-1, day_suffix (i-1));
                             break;
                           case RC_WDNR_S1_2_S7_CHAR:
                             /*
                                %weekday_number special text.
                             */
                             sprintf(s2, "%d", (i==DAY_MAX) ? DAY_MIN : i+1);
                             break;
                           case RC_WDNR_S0_2_S6_CHAR:
                             /*
                                %weekday_number special text.
                             */
                             sprintf(s2, "%d", (i==DAY_MAX) ? DAY_MIN-1 : i);
                             break;
                           case RC_WDNR_S1_2_S7S_CHAR:
                             /*
                                %weekday_number_with_ordinal_number_suffix special text.
                             */
                             if (i == DAY_MAX)
                               i = DAY_MIN;
                             else
                               i++;
                             sprintf(s2, "%d%s", i, day_suffix (i));
                             break;
                           case RC_WDNR_S0_2_S6S_CHAR:
                             /*
                                %weekday_number_with_ordinal_number_suffix special text.
                             */
                             if (i == DAY_MAX)
                               i = DAY_MIN - 1;
                             sprintf(s2, "%d%s", i, day_suffix (i));
                             break;
                           case RC_WDNR_X1_2_X7_CHAR:
                             /*
                                %weekday_number special text.
                             */
                             sprintf(s2, "%d", SYEAR(i, start_day));
                             break;
                           case RC_WDNR_X0_2_X6_CHAR:
                             /*
                                %weekday_number special text.
                             */
                             sprintf(s2, "%d", (SYEAR(i, start_day))-1);
                             break;
                           case RC_WDNR_X1_2_X7S_CHAR:
                             /*
                                %weekday_number_with_ordinal_number_suffix special text.
                             */
                             sprintf(s2, "%d%s", SYEAR(i, start_day), day_suffix (SYEAR(i, start_day)));
                             break;
                           case RC_WDNR_X0_2_X6S_CHAR:
                             /*
                                %weekday_number_with_ordinal_number_suffix special text.
                             */
                             sprintf(s2, "%d%s", (SYEAR(i, start_day))-1, day_suffix ((SYEAR(i, start_day))-1));
                             break;
                           case RC_DOYNR_NZ_CHAR:
                             /*
                                %day_of_year_number_without_leading_zero special text.
                             */
                             sprintf(s2, "%d", day_of_year (dd, mm, yy));
                             break;
                           case RC_DOYNR_LZ_CHAR:
                             /*
                                %day_of_year_number_with_leading_zero special text.
                             */
                             sprintf(s2, "%02d", day_of_year (dd, mm, yy));
                             break;
                           case RC_DOYNR_NZS_CHAR:
                             /*
                                %day_of_year_number_without_leading_zero_and_ordinal_number_suffix special text.
                             */
                             i = day_of_year (dd, mm, yy);
                             sprintf(s2, "%d%s", i, day_suffix (i));
                             break;
                           case RC_DOYNR_LZS_CHAR:
                             /*
                                %day_of_year_number_with_leading_zero_and_ordinal_number_suffix special text.
                             */
                             i = day_of_year (dd, mm, yy);
                             sprintf(s2, "%02d%s", i, day_suffix (i));
                             break;
                           case RC_DAYNR_NZ_CHAR:
                             /*
                                %day_number_without_leading_zero special text.
                             */
                             sprintf(s2, "%d", dd);
                             break;
                           case RC_DAYNR_LZ_CHAR:
                             /*
                                %day_number_with_leading_zero special text.
                             */
                             sprintf(s2, "%02d", dd);
                             break;
                           case RC_DAYNR_NZS_CHAR:
                             /*
                                %day_number_without_leading_zero_and_ordinal_number_suffix special text.
                             */
                             sprintf(s2, "%d%s", dd, day_suffix (dd));
                             break;
                           case RC_DAYNR_LZS_CHAR:
                             /*
                                %day_number_with_leading_zero_and_ordinal_number_suffix special text.
                             */
                             sprintf(s2, "%02d%s", dd, day_suffix (dd));
                             break;
                           case RC_MONTHNAME_CHAR:
                             /*
                                %complete_month_name special text.
                             */
                             sprintf(s2, "%s", month_name (mm));
                             break;
                           case RC_MONTHNAME3_CHAR:
                             /*
                                %3_letter_month_name special text.
                             */
                             sprintf(s2, "%s", short_month_name (mm));
                             break;
                           case RC_MONTHNR_NZ_CHAR:
                             /*
                                %month_number_without_leading_zero special text.
                             */
                             sprintf(s2, "%d", mm);
                             break;
                           case RC_MONTHNR_LZ_CHAR:
                             /*
                                %month_number_with_leading_zero special text.
                             */
                             sprintf(s2, "%02d", mm);
                             break;
                           case RC_MONTHNR_NZS_CHAR:
                             /*
                                %month_number_without_leading_zero_and_trailing_ordinal_number_suffix special text.
                             */
                             sprintf(s2, "%d%s", mm, day_suffix (mm));
                             break;
                           case RC_MONTHNR_LZS_CHAR:
                             /*
                                %month_number_with_leading_zero_and_trailing_ordinal_number_suffix special text.
                             */
                             sprintf(s2, "%02d%s", mm, day_suffix (mm));
                             break;
                           case RC_YEARNR_NZ_CHAR:
                             /*
                                %year_number_without_leading_zero special text.
                             */
                             sprintf(s2, "%d", yy);
                             break;
                           case RC_YEARNR_LZ_CHAR:
                             /*
                                %year_number_with_leading_zero special text.
                             */
                             sprintf(s2, "%0*d", len_year_max, yy);
                             break;
                           case RC_WEEKNR_CHAR:
                             /*
                                %iso_week_number special text.
                             */
                             i = week_number (dd, mm, yy);
                             /*
                                We convert the computed week number to a week number text
                                (that looks nicer in output).
                             */
                             if (i < 0)
                               /*
                                  Week starts in previous year and the first days
                                    of the actual year are not in its first week.
                               */
                               sprintf(s2, "%02d/0", -i);
                             else
                               if (!i)
                                 /*
                                    Week starts in previous year and the first days
                                      of the actual year are in its first week.
                                 */
                                 sprintf(s2, "%s", "53/1");
                               else
                                 /*
                                    Week starts in actual year.
                                 */
                                 sprintf(s2, "%02d", i);
                             break;
                           case RC_YEAR_DIFF_CHAR:
                             /*
                                %year_difference special text.
                             */
                             yy = yy - true_year;
                             if (   m < mm
                                 || (   (m == mm)
                                     && (d < dd)))
                              {
                                if (yy < 1)
                                  yy++;
                                else
                                  if (yy > 1)
                                    yy--;
                              }
                             sprintf(s2, "%d", yy);
                             break;
                           case RC_BYEAR_CHAR:
                           case RC_BYEAR_S_CHAR:
                             /*
                                %birth_age_number special text.
                             */
                             yy = true_year - yy;
                             if (   m < mm
                                 || (   (m == mm)
                                     && (d < dd)))
                               yy--;
                             if (yy >= 1)
                              {
                                sprintf(s2, "%d", yy);
                                /*
                                   %birth_age_number_with_trailing_ordinal_number_suffix special text.
                                */
                                if (special_text_char == RC_BYEAR_S_CHAR)
                                  strcat(s2, day_suffix (yy));
                              }
                             else
                               *s2 = '\0';
                             break;
                           case RC_MOON_NZ_CHAR:
                           case RC_MOON_LZ_CHAR:
#  if HAVE_LIBM
                             i = the_phase (&moon_min_max, dd, mm, yy, 0, 0);
#  else /* !HAVE_LIBM */
                             i = the_moonphase (&moon_min_max, dd, mm, yy, 0, 0);
#  endif /* !HAVE_LIBM */
                             if (special_text_char == RC_MOON_NZ_CHAR)
                                /*
                                   %moonphase_without_leading_zero special text.
                                */
                                sprintf(s2, "%d%%", abs(i));
                             else
                               /*
                                  %moonphase_with_leading_zero special text.
                               */
                               sprintf(s2, "%03d%%", abs(i));
                             if (moon_min_max)
                              {
                                if (abs(i) < 50)
                                  strcat(s2, MOON_MIN);
                                else
                                  strcat(s2, MOON_MAX);
                              }
                             else
                               if (i < 0)
                                 strcat(s2, MOON_WANES);
                               else
                                 strcat(s2, MOON_WAXES);
                             break;
                           case RC_MOON_IMAGE_CHAR:
                             /*
                                %moonphase_text_graphics_image special text.
                             */
#  if HAVE_LIBM
                             draw_moon (the_phase (&moon_min_max, dd, mm, yy, 0, 0), rc_moonimage_lines, &s2);
#  else /* !HAVE_LIBM */
                             draw_moon (the_moonphase (&moon_min_max, dd, mm, yy, 0, 0), rc_moonimage_lines, &s2);
#  endif /* !HAVE_LIBM */
                             /*
                                Exchange all RC_NL_CHAR=='~' by RC_NL2_CHAR=='^'
                                  in `s2' in case the moon text graphics special
                                  text is lead by a RC_NL2_CHAR.
                             */
                             if (k)
                               if (s1[k-1] == RC_NL2_CHAR)
                                 for (i=0 ; s2[i] ; i++)
                                   if (s2[i] == RC_NL_CHAR)
                                     s2[i] = RC_NL2_CHAR;
                             break;
                           case RC_BIO_CHAR:
                             /*
                                %biorhythm special text.
                             */
                             (void)biorhythm (FALSE, rc_bio_axis_len, s2, d, m, y, dd, mm, yy,
                                              rc_bio_emo_lit, &bio_emo_phase, &bio_emo_waxes,
                                              rc_bio_int_lit, &bio_int_phase, &bio_int_waxes,
                                              rc_bio_phy_lit, &bio_phy_phase, &bio_phy_waxes,
                                              &bio_critical_day, &bio_positive_day, &bio_negative_day);
                             /*
                                Convert the "emotional" phase value to a string of template `+NNN%+',
                                  where `+' indicates a sign and `N' indicates a digit (use '0' if none).
                             */
                             sprintf(bio_emo_ptext, "%4d%%%c", abs(bio_emo_phase), (abs(bio_emo_phase) == 100)
                                     ? *BIO_STABLE : ((bio_emo_waxes) ? *BIO_WAXES : *BIO_WANES));
                             ptr_char = bio_emo_ptext + 1;
                             while (isspace(*ptr_char))
                               *ptr_char++ = '0';
                             if (bio_emo_phase < 0)
                               *bio_emo_ptext = *BIO_WANES;
                             else
                               *bio_emo_ptext = *BIO_WAXES;
                             /*
                                Convert the "intellectual" phase value to a string of template `+NNN%+',
                                  where `+' indicates a sign and `N' indicates a digit (use '0' if none).
                             */
                             sprintf(bio_int_ptext, "%4d%%%c", abs(bio_int_phase), (abs(bio_int_phase) == 100)
                                     ? *BIO_STABLE : ((bio_int_waxes) ? *BIO_WAXES : *BIO_WANES));
                             ptr_char = bio_int_ptext + 1;
                             while (isspace(*ptr_char))
                               *ptr_char++ = '0';
                             if (bio_int_phase < 0)
                               *bio_int_ptext = *BIO_WANES;
                             else
                               *bio_int_ptext = *BIO_WAXES;
                             /*
                                Convert the "physical" phase value to a string of template `+NNN%+',
                                  where `+' indicates a sign and `N' indicates a digit (use '0' if none).
                             */
                             sprintf(bio_phy_ptext, "%4d%%%c", abs(bio_phy_phase), (abs(bio_phy_phase) == 100)
                                     ? *BIO_STABLE : ((bio_phy_waxes) ? *BIO_WAXES : *BIO_WANES));
                             ptr_char = bio_phy_ptext + 1;
                             while (isspace(*ptr_char))
                               *ptr_char++ = '0';
                             if (bio_phy_phase < 0)
                               *bio_phy_ptext = *BIO_WANES;
                             else
                               *bio_phy_ptext = *BIO_WAXES;
                             /*
                                Create the resulting biorhythm values line.
                             */
                             sprintf(s2, "%d%c %d%c %d%c , %s=%s %s=%s %s=%s",
                                     bio_critical_day, *BIO_CRITICAL,
                                     bio_positive_day, *BIO_WAXES,
                                     bio_negative_day, *BIO_WANES,
                                     rc_bio_emo_lit, bio_emo_ptext,
                                     rc_bio_int_lit, bio_int_ptext,
                                     rc_bio_phy_lit, bio_phy_ptext);
                             break;
                           case RC_BIO_BAR_CHAR:
                             /*
                                %biorhythm_text_graphics_bar special text.
                             */
                             s2 = biorhythm (TRUE, rc_bio_axis_len, s2, d, m, y, dd, mm, yy,
                                             rc_bio_emo_lit, &bio_emo_phase, &bio_emo_waxes,
                                             rc_bio_int_lit, &bio_int_phase, &bio_int_waxes,
                                             rc_bio_phy_lit, &bio_phy_phase, &bio_phy_waxes,
                                             &bio_critical_day, &bio_positive_day, &bio_negative_day);
                             break;
                           default:
                             /*
                                This case MUST be an internal error!
                             */
                             abort();
                         }
                        /*
                           And copy the expanded special text into the target string.
                        */
                        i = 0;
                        /*
                           A moon text graphics special text leads the text part of
                             a resource file line so it is good now to remove its
                             leading newline character, which is represented by
                             a RC_NL_CHAR=='~' in `s2[0]', to avoid a redundant
                             line break-up, but only in this definite case and in
                             the case, all RC_NL_CHAR's of the moon text graphics
                             string are already exchanged by RC_NL2_CHAR's=='^'!
                        */
                        if (   (   !k
                                && (*s2 == RC_NL_CHAR))
                            || (*s2 == RC_NL2_CHAR))
                          i++;
                        while (s2[i])
                         {
                           if ((Uint)k >= maxlen_max-(Uint)len_fn)
                             resize_all_strings (maxlen_max<<1, FALSE, __FILE__, (long)__LINE__);
                           s1[k++] = s2[i++];
                         }
                      }
                     if (!*(the_text + j))
                       ok = TRUE;
                   }
                  is_obsolete_whitespace = TRUE;
                }
             }
            else
             {
               if (   special_text_char == RC_TIME_CHAR
                   || special_text_char == RC_DAY_DIFF_CHAR
                   || special_text_char == RC_WEEK_DIFF_CHAR
                   || special_text_char == RC_MONTH_DIFF_CHAR
                   || special_text_char == RC_YEAR_DIFF_CHAR
                   || special_text_char == RC_JDAYS_CHAR
                   || special_text_char == RC_HLS1S_CHAR
                   || special_text_char == RC_HLS1E_CHAR
                   || special_text_char == RC_HLS2S_CHAR
                   || special_text_char == RC_HLS2E_CHAR
                   || special_text_char == RC_HLS3S_CHAR
                   || special_text_char == RC_HLS3E_CHAR
                   || special_text_char == RC_HLS4S_CHAR
                   || special_text_char == RC_HLS4E_CHAR
                   || special_text_char == RC_HLS5S_CHAR
                   || special_text_char == RC_HLS5E_CHAR)
                {
                  if (special_text_char == RC_TIME_CHAR)
                   {
                     register int   tmp_ah=act_hour;
                     static   char  time_suffix[3];
                     auto     Bool  is_12_hours_format=FALSE;


                     /*
                        %time[12|24] special text found:
                          Replace it by the current system time hh:mm[am|pm].
                     */
                     j++;
                     if (isdigit(*(the_text + j)))
                      {
                        if (*(the_text + j) == '1')
                          is_12_hours_format = TRUE;
                        while (isdigit(*(the_text + j)))
                          j++;
                      }
                     j--;
                     if (is_12_hours_format)
                      {
                        /*
                           Copy a trailing "am" resp., "pm" suffix to text.
                        */
                        if (tmp_ah >= 12)
                         {
#  if USE_DE
                           strcpy(time_suffix, RC_PM_TXT);
#  else /* !USE_DE */
                           strcpy(time_suffix, _("pm"));
#  endif /* !USE_DE */
                           if (tmp_ah > 12)
                             tmp_ah -= 12;
                         }
                        else
#  if USE_DE
                          strcpy(time_suffix, RC_AM_TXT);
#  else /* !USE_DE */
                          strcpy(time_suffix, _("am"));
#  endif /* !USE_DE */
                      }
                     else
                       *time_suffix = '\0';
#  if USE_DE
                     sprintf(s2, "%02d%s%02d%s", tmp_ah, TIME_SEP, act_min, time_suffix);
#  else /* !USE_DE */
                     sprintf(s2, "%02d%s%02d%s", tmp_ah, _(":"), act_min, time_suffix);
#  endif /* !USE_DE */
                   }
                  else
                    if (   special_text_char == RC_HLS1S_CHAR
                        || special_text_char == RC_HLS1E_CHAR
                        || special_text_char == RC_HLS2S_CHAR
                        || special_text_char == RC_HLS2E_CHAR
                        || special_text_char == RC_HLS3S_CHAR
                        || special_text_char == RC_HLS3E_CHAR
                        || special_text_char == RC_HLS4S_CHAR
                        || special_text_char == RC_HLS4E_CHAR
                        || special_text_char == RC_HLS5S_CHAR
                        || special_text_char == RC_HLS5E_CHAR)
                     {
                       /*
                          One of the %highlighting special texts found:
                            Replace it by the real hightlighting sequence.
                       */
                       *s2 = '\0';
                       if (highlight_flag)
                        {
                          if (   (special_text_char == RC_HLS1S_CHAR)
                              && !hls1_set)
                           {
                             sprintf(s2, "%s", ehls1s.seq);
                             hls1_set = TRUE;
                             hls2_set = FALSE;
                           }
                          else
                            if (   (special_text_char == RC_HLS1E_CHAR)
                                && hls1_set)
                             {
                               sprintf(s2, "%s", ehls1e.seq);
                               hls1_set = FALSE;
                             }
                            else
                              if (   (special_text_char == RC_HLS2S_CHAR)
                                  && !hls2_set)
                               {
                                 sprintf(s2, "%s", ehls2s.seq);
                                 hls2_set = TRUE;
                                 hls1_set = FALSE;
                               }
                              else
                                if (   (special_text_char == RC_HLS2E_CHAR)
                                    && hls2_set)
                                 {
                                   sprintf(s2, "%s", ehls2e.seq);
                                   hls2_set = FALSE;
                                 }
                               else
                                 /*
                                    Set highlighting sequence 1 only if the
                                      fixed date is on today's date!
                                 */
                                 if (   (   special_text_char == RC_HLS3S_CHAR
                                         || special_text_char == RC_HLS5S_CHAR)
                                     && (year+incr_year-decr_year == act_year)
                                     && (m == act_month)
                                     && (d == act_day)
                                     && !hls1_set)
                                  {
                                    sprintf(s2, "%s", ehls1s.seq);
                                    hls1_set = TRUE;
                                    hls2_set = FALSE;
                                  }
                                 else
                                   if (   (   special_text_char == RC_HLS3E_CHAR
                                           || special_text_char == RC_HLS5E_CHAR)
                                       && hls1_set)
                                    {
                                      sprintf(s2, "%s", ehls1e.seq);
                                      hls1_set = FALSE;
                                    }
                                   else
                                     /*
                                        Set highlighting sequence 2 only if the
                                          fixed date is on a legal holiday date!
                                     */
                                     if (   (   special_text_char == RC_HLS4S_CHAR
                                             || special_text_char == RC_HLS5S_CHAR)
                                         && hd_ldays[((m-1)*MONTH_LAST)+(d-1)]
                                         && !hls2_set)
                                      {
                                        sprintf(s2, "%s", ehls2s.seq);
                                        hls2_set = TRUE;
                                        hls1_set = FALSE;
                                      }
                                     else
                                       if (   (   special_text_char == RC_HLS4E_CHAR
                                               || special_text_char == RC_HLS5E_CHAR)
                                           && hls2_set)
                                        {
                                          sprintf(s2, "%s", ehls2e.seq);
                                          hls2_set = FALSE;
                                        }
                                }
                     }
                    else
                     {
                       auto Slint  diff;


                       dd = act_day;
                       mm = act_month;
                       yy = act_year;
                       (void)get_actual_date ();
                       y = year + incr_year - decr_year;
                       /*
                          Now compute the differences:
                            date1 == actual (base)date (act_day, act_month, act_year)
                            date2 == reported date (d, m, y)
                       */
                       if (special_text_char == RC_JDAYS_CHAR)
                        {
                          /*
                             A %julian_days[[-]<n>] special text found
                               (we always suppress the day fraction part of 0.5).
                          */
                          diff = d_between (1, 1, 1, d, m, y);
                          j++;
                          if (*(the_text + j) == *DES_LIT)
                            j++;
                          i = 0;
                          while (isdigit(*(the_text + j)))
                           {
                             if ((Uint)i >= maxlen_max)
                               resize_all_strings (maxlen_max<<1, FALSE, __FILE__, (long)__LINE__);
                             s2[i++] = *(the_text + j++);
                           }
                          s2[i] = '\0';
                          j--;
                          num = atol(s2);
                          diff += (RC_MIN_BCE_TO_1_CE - num);
                        }
                       else
                         if (special_text_char == RC_DAY_DIFF_CHAR)
                           /*
                              %day_difference special text found.
                           */
                           diff = d_between (act_day, act_month, act_year, d, m, y);
                         else
                           if (special_text_char == RC_WEEK_DIFF_CHAR)
                             /*
                                %week_difference special text found.
                             */
                             diff = w_between (act_day, act_month, act_year, d, m, y);
                           else
                             if (special_text_char == RC_MONTH_DIFF_CHAR)
                               /*
                                  %month_difference special text found.
                               */
                               diff = m_between (act_month, act_year, m, y);
                             else
                               /*
                                  A simple %year_difference special text found.
                               */
                               diff = (Slint)y - act_year;
                       sprintf(s2, "%ld", diff);
                       act_day = dd;
                       act_month = mm;
                       act_year = yy;
                     }
                  /*
                     And copy the expanded special text into the target string.
                  */
                  i = 0;
                  while (s2[i])
                   {
                     if ((Uint)k >= maxlen_max-(Uint)len_fn)
                       resize_all_strings (maxlen_max<<1, FALSE, __FILE__, (long)__LINE__);
                     s1[k++] = s2[i++];
                   }
                  j++;
                }
               else
                 /*
                    Check for %... special_texts which disable a fixed date.
                 */
                 if (   !got_shell_cmd
                     && (   special_text_char == RC_EX_LHDY_CHAR
                         || special_text_char == RC_EX_NLHDY_CHAR
                         || special_text_char == RC_EX_AHDY_CHAR
                         || special_text_char == RC_EX_NAHDY_CHAR
                         || special_text_char == RC_EX_MON_CHAR
                         || special_text_char == RC_EX_NMON_CHAR
                         || special_text_char == RC_EX_TUE_CHAR
                         || special_text_char == RC_EX_NTUE_CHAR
                         || special_text_char == RC_EX_WED_CHAR
                         || special_text_char == RC_EX_NWED_CHAR
                         || special_text_char == RC_EX_THU_CHAR
                         || special_text_char == RC_EX_NTHU_CHAR
                         || special_text_char == RC_EX_FRI_CHAR
                         || special_text_char == RC_EX_NFRI_CHAR
                         || special_text_char == RC_EX_SAT_CHAR
                         || special_text_char == RC_EX_NSAT_CHAR
                         || special_text_char == RC_EX_SUN_CHAR
                         || special_text_char == RC_EX_NSUN_CHAR
                         || special_text_char == RC_EX_MON_2_THU_CHAR
                         || special_text_char == RC_EX_NMON_2_THU_CHAR
                         || special_text_char == RC_EX_MON_2_FRI_CHAR
                         || special_text_char == RC_EX_NMON_2_FRI_CHAR))
                  {
                    if (strchr(date_text, special_text_char) == (char *)NULL)
                     {
                       /*
                          Insert the `special_text_char' into `date_text'
                            only in cases it is not already stored in there!
                       */
                       *ptr_date_text++ = special_text_char;
                       *ptr_date_text = '\0';
                     }
                    j++;
                  }
                 else
                   /*
                      Check for %shell_escape[TEXT-TILL-EOL] special text.
                   */
                   if (   !rc_bypass_shell_cmd
                       && !got_shell_cmd
                       && (special_text_char == RC_SHELL_ESC_CHAR))
                    {
                      /*
                         Buffer the starting position of the %shell_escape[TEXT-TILL-EOL]
                           special text and process it as usual, so other special texts
                           possibly contained are expanded!
                      */
                      kpos = k;
                      got_shell_cmd = TRUE;
                      s1[k++] = *(the_text + (j - 1));
                    }
                   else
                     s1[k++] = *(the_text + (j - 1));
             }
            if (!print_line)
              ok = TRUE;
          }
         else
           ok = TRUE;
         if (ok)
          {
            if (print_line)
             {
               /*
                  Check whether a period to exclude is marked in the maps and
                    if so, avoid displaying the fixed date entry.
               */
               if (   ie_date_maps_set
                   && (   *inclusive_date_map
                       || *exclusive_date_map))
                {
                  i = day_of_year (d, m, year+incr_year-decr_year);
                  if (   *inclusive_date_map
                      && *exclusive_date_map)
                   {
                     if (   !inclusive_date_map[i]
                         || !exclusive_date_map[i])
                       print_line = FALSE;
                   }
                  else
                    if (*inclusive_date_map)
                     {
                       if (!inclusive_date_map[i])
                         print_line = FALSE;
                     }
                    else
                      if (!exclusive_date_map[i])
                        print_line = FALSE;
                }
               /*
                  Check whether a weekday to exclude is marked in the maps and
                    if so, avoid displaying the fixed date entry.
               */
               if (   print_line
                   && *date_text)
                 print_line = rc_valid_day (date_text, d, m, year+incr_year-decr_year);
               if (print_line)
                {
                  if (   highlight_flag
                      && (   hls1_set
                          || hls2_set))
                   {
                     /*
                        Hmm, seems the user forgot to give the right %highlighting
                          special text for disabling a highlighting sequence already
                          enabled, let's do this for him/her/it/per/or or or...  hehehe!
                     */
                     if (hls1_set)
                      {
                        if (ehls1e.len == 1)
                         {
                           if ((Uint)k >= maxlen_max-(Uint)len_fn-1)
                             resize_all_strings (maxlen_max<<1, FALSE, __FILE__, (long)__LINE__);
                           s1[k++] = *ehls1e.seq;
                         }
                        else
                         {
                           while ((Uint)k >= maxlen_max-(Uint)len_fn-(ehls1e.len+1))
                             resize_all_strings (maxlen_max<<1, FALSE, __FILE__, (long)__LINE__);
                           s1[k] = '\0';
                           strcat(s1, ehls1e.seq);
                           k += (ehls1e.len + 1);
                         }
                      }
                     else
                      {
                        if (ehls2e.len == 1)
                         {
                           if ((Uint)k >= maxlen_max-(Uint)len_fn-1)
                             resize_all_strings (maxlen_max<<1, FALSE, __FILE__, (long)__LINE__);
                           s1[k++] = *ehls2e.seq;
                         }
                        else
                         {
                           while ((Uint)k >= maxlen_max-(Uint)len_fn-(ehls2e.len+1))
                             resize_all_strings (maxlen_max<<1, FALSE, __FILE__, (long)__LINE__);
                           s1[k] = '\0';
                           strcat(s1, ehls2e.seq);
                           k += (ehls2e.len + 1);
                         }
                      }
                   }
                  s1[k] = '\0';
                  /*
                     Is it necessary to execute a %!shell_command[TEXT-TILL-EOL] ?
                  */
                  if (got_shell_cmd)
                   {
                     /*
                        Execute the command.
                     */
                     i = system(s1+kpos+2);
                     if (warning_level >= 0)
                      {
                        if (i == -1)
                         {
                           /*
                              Error, the `system()' function have failed.
                           */
#  if USE_DE
                           sprintf(s2, "Kommando kann nicht ausf%shrt werden in Datei `%s'\nZeile %ld: %s",
                                   UE, filename, line_number, the_text+kpos+2);
#  else /* !USE_DE */
                           sprintf(s2, _("Cannot execute command in file `%s'\nLine: %ld %s"),
                                   filename, line_number, the_text+kpos+2);
#  endif /* !USE_DE */
                           print_text (stderr, s2);
                           if (warning_level >= WARN_LVL_MAX)
                            {
                              k = (int)strlen(s1+kpos+2);
                              if ((Uint)k >= maxlen_max-9)
                                resize_all_strings (k+9, FALSE, __FILE__, (long)__LINE__);
                              sprintf(s2, "system(%s)=", s1+kpos+2);
                              my_error (110, __FILE__, ((long)__LINE__)-22, s2, i);
                            }
                         }
                        else
                         {
                           /*
                              Report the exit code of command executed by the `system()' function.
                           */
#  if USE_DE
                           sprintf(s2, "Kommando ausgef%shrt (Status=%d) in Datei `%s'\nZeile %ld: %s",
                                   UE, i, filename, line_number, s1+kpos+2);
#  else /* !USE_DE */
                           sprintf(s2, _("Command executed (exit code=%d) in file `%s'\nLine %ld: %s"),
                                   i, filename, line_number, s1+kpos+2);
#  endif /* !USE_DE */
                           print_text (stderr, s2);
                           /*
                              Abort, the command executed by the `system()' function
                                returned a value not equal zero so we terminate all
                                further processing now with exit code 2.
                           */
                           if (   i
                               && (warning_level >= WARN_LVL_MAX))
                             my_exit (2);
                         }
                      }
                     if (!kpos)
                       print_line = FALSE;
                     else
                       s1[kpos] = '\0';
                     shell_escape_done = TRUE;
                   }
                }
             }
          }
       }
      if (   print_line
          && (rc_filter_text != (char *)NULL))
       {
         /*
            If `--filter-text=PATTERN' is given and REGEX doesn't match
              the text of the fixed date, suppress this fixed date!
         */
#  if HAVE_GNU_RE_COMPILE_PATTERN
         print_line = (Bool)(re_search(&regpattern, s1, k, 0, k, (struct re_registers *)NULL) >= 0);
#  endif
#  if HAVE_POSIX_REGCOMP
         static regmatch_t  rm_dummy;


         print_line = (Bool)!regexec(&regpattern, s1, 1, &rm_dummy, 0);
#  endif
#  if HAVE_RE_COMP
         print_line = (Bool)(re_exec(s1) == 1);
#  endif
#  if HAVE_REGCMP
         print_line = (Bool)(regex(cpattern, s1) != (char *)NULL);
#  endif
#  if HAVE_V8_REGCOMP
         print_line = (Bool)regexec(regpattern, s1);
#  endif
#  if NO_REGEX
         print_line = (Bool)(strstr(s1, rc_filter_text) != (char *)NULL);
#  endif
       }
      if (print_line)
       {
         if (rc_enable_fn_flag)
          {
            if (*s1)
              sprintf(s6, "%0*d%02d%02d (%s) %s",
                      len_year_max, year+incr_year-decr_year, m, d, filename, s1);
            else
              sprintf(s6, "%0*d%02d%02d (%s)",
                      len_year_max, year+incr_year-decr_year, m, d, filename);
          }
         else
           sprintf(s6, "%0*d%02d%02d %s",
                   len_year_max, year+incr_year-decr_year, m, d, s1);
         /*
            Now compute whether a filler text for a week entry is needed:
              Week 51/0   == 7 chars text "|51/0| "  -> no filler text.
              Week 52/0   == 7 chars text "|52/1| "  -> no filler text.
              Week 53/0   == 7 chars text "|53/0| "  -> no filler text.
              Week 53/1   == 7 chars text "|53/1| "  -> no filler text.
              Week 1...52 == 5 chars text "|nn| "    -> 2 chars filler text.
         */
         if (rc_weekno_flag)
           if (week_number (d, m, year+incr_year-decr_year) <= 0)
             len_fil_wt = 2;
         /*
            Now place the fixed date into the table:
              Only if the previous fixed date is equal the actual fixed date,
              avoid insertation of actual fixed date!
         */
         i = 1;
         if (*rc_elems)
           i = strcmp(s6, rc_elems_table[*rc_elems-1]);
         /*
            Store the constructed "raw" line in `rc_elems_table[]'.
         */
         if (i)
          {
            if ((Uint)*rc_elems >= rc_elems_max)
             {
               /*
                  Resize the `rc_elems_table[]' table.
               */
               rc_elems_max <<= 1;
               if (rc_elems_max*sizeof(char *) > testval)
                 rc_elems_max--;
               rc_elems_table = (char **)my_realloc ((VOID_PTR)rc_elems_table, rc_elems_max*sizeof(char *),
                                                     124, __FILE__, ((long)__LINE__)-1,
                                                     "rc_elems_table[rc_elems_max]", rc_elems_max);
             }
            rc_elems_table[*rc_elems] = (char *)my_malloc (strlen(s6)+1,
                                                           124, __FILE__, ((long)__LINE__)-1,
                                                           "rc_elems_table[rc_elems]", *rc_elems);
            strcpy(rc_elems_table[(*rc_elems)++], s6);
          }
       }
      /*
         Do we have to construct a 1x2 production of the line?
      */
      if (print_twice > 1)
       {
         if (is_2easter)
          {
            /*
               Precalculate the next date relative to Easter Sunday's date.
            */
            incr_year = 1;
            precomp_date (hn, hwd, &d, &m, year+incr_year, EAster);
          }
         else
           if (is_2dvar)
            {
              /*
                 Precalculate the next date relative to date variable's date
                   and use original date of `dvar', i.e. use buffer of day an month.
              */
              incr_year = 1;
              if (islower(hc))
                (void)precomp_date (hn, hwd, &d_buf, &m_buf, year+incr_year,
                                    (hc == RC_TODAY_CHAR) ? TOday : DVar);
              else
               {
                 y = year + incr_year;
                 (void)precomp_nth_wd (hn, hwd, &hn, &d_buf, &m_buf, &y,
                                       (hc == 'D') ? DAy : WEek);
               }
              d = d_buf;
              m = m_buf;
            }
           else
             /*
                Compute tomorrow's date.
             */
             next_date (&d, &m, &year);
         /*
            If the "new" precomputed date has left the year bounds, we have to exit the loop!
         */
         if (year+incr_year > YEAR_MAX)
           print_twice--;
         else
           print_line = TRUE;
       }
    } while (--print_twice);
   year = tmp_year;
}
#endif /* USE_RC */
