/*
*  gcal.c:  Main part which controls the extended calendar program.
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



static char rcsid[]="$Id: gcal.c 2.40 1997/05/23 02:04:00 tom Exp $";



/*
*  Include header files.
*/
#include "tailor.h"
#if HAVE_ASSERT_H
#  include <assert.h>
#endif
#if HAVE_CTYPE_H
#  include <ctype.h>
#endif
#if HAVE_LIMITS_H
#  include <limits.h>
#endif
#if HAVE_UNISTD_H
#  include <unistd.h>
#endif
#if (!HAVE_SIGNAL_H || !HAVE_SIGNAL) && HAVE_SYS_TYPES_H   /* Otherwise "gcal.h" includes <sys/types.h> */
#  include <sys/types.h>
#endif
#if HAVE_SYS_STAT_H
#  include <sys/stat.h>
#endif
#ifdef GCAL_EPAGER
#  if HAVE_FCNTL_H
#    include <fcntl.h>
#    if !HAVE_DUP
#      define dup(old)            (fcntl(old, F_DUPFD, 0))
#    endif
#    if !HAVE_DUP2
#      define dup2(old, new)      (close(new), fcntl(old, F_DUPFD, new))
#    endif
#  endif
#  if HAVE_SYS_WAIT_H
#    include <sys/wait.h>
#  endif
#  ifndef WEXITSTATUS
#    define WEXITSTATUS(stat_val) ((unsigned)(stat_val) >> 8)
#  endif
#  ifndef WIFEXITED
#    define WIFEXITED(stat_val)   (((stat_val) & 0xff) == 0)
#  endif
#endif /* GCAL_EPAGER */
#include "gcal.h"



/*
*  Set Borland/Turbo-c specific MSDOS stack length in case USE_RC is defined  ;<
*/
#if USE_RC
#  if defined(MSDOS) && defined(__TURBOC__)
#    include <dos.h>
PUBLIC Uint _stklen=0x4000;
#  endif
#endif



/*
*  Function prototypes.
*/
#if __cplusplus
extern "C"
{
#endif
/*
************************************************** Defined in `file-io.c'.
*/
IMPORT FILE *
file_open __P_((      char       **filename,
                const int          level,
                const Fmode_enum   mode,
                      Bool        *bad_sys_include));
IMPORT char **
insert_response_file __P_((      FILE *fp,
                                 char *filename,
                           const char *opt_list,
                                 Uint *my_argc_max,
                                 int  *my_argc,
                                 char *my_argv[]));
IMPORT void
write_log_file __P_((const char       *filename,
                     const Fmode_enum  mode,
                     const char       *mode_txt,
                     const char       *created_txt,
                     const int         argc,
                           char       *argv[]));
/*
************************************************** Defined in `holiday.c'.
*/
IMPORT const Cc_struct *
binsearch_cc_id __P_((char *id));
IMPORT int
eval_holiday __P_((      int  day,
                   const int  month,
                   const int  year,
                   const int  wd,
                   const Bool forwards));
IMPORT void
print_all_holidays __P_((      Bool init_data,
                         const Bool detect));
/*
************************************************** Defined in `help.c'.
*/
IMPORT void
my_help_on_help __P_((      FILE *fp,
                      const char *longopt,
                      const Bool  lopt_ambig,
                      const int   cols));
IMPORT void
my_extended_help __P_((      FILE *fp,
                       const int   longopt_symbolic));
IMPORT void
my_basic_help __P_((FILE *fp));
IMPORT void
my_license __P_((FILE *fp));
IMPORT void
my_version __P_((FILE *fp));
IMPORT char *
usage_msg __P_((void));
IMPORT void
put_longopt_description __P_((FILE *fp));
/*
************************************************** Defined in `print.c'.
*/
IMPORT void
print_calendar __P_((void));
#if USE_RC
/*
************************************************** Defined in `rc-utils.c'.
*/
IMPORT void
rc_clean_flags __P_((void));
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
set_dvar __P_((const char        *line_buffer,
                     Line_struct *lineptrs,
               const char        *filename,
               const long         line_number,
               const Dvar_enum    mode));
IMPORT Bool
set_tvar __P_((const char      *line_buffer,
               const Tvar_enum  mode));
/*
************************************************** Defined in `rc-use.c'.
*/
IMPORT void
rc_use __P_((void));
#endif /* USE_RC */
/*
************************************************** Defined in `tty.c'.
*/
#if USE_PAGER
IMPORT void
get_tty_scr_size __P_((int *rows,
                       int *cols));
#endif
IMPORT void
get_tty_hls __P_((const char *sequence_str));
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
allocate_all_strings __P_((const int   amount,
                           const char *module_name,
                           const long  module_line));
IMPORT void
resize_all_strings __P_((const int   amount,
                         const int   with_line_buffer,
                         const char *module_name,
                         const long  module_line));
IMPORT void
my_error __P_((const int   exit_status,
               const char *module_name,
               const long  module_line,
               const char *var_name,
               const int   var_contents));
#if HAVE_SIGNAL && (defined(SIGINT) || defined(SIGTERM) || defined(SIGHUP))
IMPORT RETSIGTYPE
handle_signal __P_((int the_signal));
#endif
IMPORT void
my_exit __P_((const int exit_status));
IMPORT int
my_atoi __P_((const char *string));
#if !HAVE_STRSTR
IMPORT char *
my_strstr __P_((const char *text,
                const char *pattern));
#endif /* !HAVE_STRSTR */
#if !HAVE_STRCASECMP
IMPORT int
my_strcasecmp __P_((const char *s1,
                    const char *s2));
#endif /* !HAVE_STRCASECMP */
#if !HAVE_STRNCASECMP
IMPORT int
my_strncasecmp __P_((const char *s1,
                     const char *s2,
                           int   len));
#endif /* !HAVE_STRNCASECMP */
IMPORT Bool
get_actual_date __P_((void));
IMPORT int
compare_d_m_name __P_((const char       *string,
                       const Cmode_enum  mode));
IMPORT const char *
day_suffix __P_((int day));
IMPORT const char *
day_name __P_((const int day));
IMPORT const char *
short_month_name __P_((const int month));
IMPORT const char *
month_name __P_((const int month));
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
/*
************************************************** Defined in `gcal.c'.
*/
EXPORT int
main __P_((int   argc,
           char *argv[]));
EXPORT int
eval_longopt __P_((char *longopt,
                   int  *longopt_symbolic));
LOCAL Bool
correct_date_format __P_((char *format_txt,
                          Bool *use_day_suffix,
                          Bool *use_short3_day_name,
                          Bool *use_day_zeroleaded,
                          Bool *use_year_zeroleaded));
LOCAL void
rearrange_argv __P_((const char *opt_list,
                           int  *argc,
                           char *argv[]));
LOCAL void
check_command_line __P_((int   argc,
                         char *argv[]));
LOCAL void
build_month_list __P_((char *argv[]));
LOCAL void
eliminate_invalid_data __P_((void));
#if __cplusplus
}
#endif



/*
*  Declare public(extern) variables.
*/
#if USE_RC
IMPORT Line_struct *lineptrs;                   /* Pointers to different parts of a (resource file) line */
IMPORT FILE        *rc_here_fp;                 /* Temporary file used for managing `--here=ARG' options */
IMPORT int          rc_period;                  /* Amount of period of fixed dates */
IMPORT int          rc_elems;                   /* Amount of resource file entries */
IMPORT int          rc_zero_pos;                /* Starting index of dummy resource file entries in table */
IMPORT int          rc_have_today_in_list;      /* [-c]d */
IMPORT int          rc_bio_axis_len;            /* Length of a single axis of a biorhythm text graphics bar */
IMPORT int          rc_moonimage_lines;         /* Number of lines of a moonphase text graphics image */
IMPORT char        *rc_adate;                   /* Text of modified actual date %... */
IMPORT char        *rc_filename;                /* Name of alternative resource file(s) -f|F<NAME[+...]> */
IMPORT char        *rc_here_fn;                 /* Name of tempfile used for managing `--here=ARG' options */
IMPORT char        *rc_filter_day;              /* Argument used for filtering fixed date days */
IMPORT char        *rc_filter_period;           /* Argument used for filtering fixed date periods */
IMPORT char        *rc_filter_text;             /* REGEX used for filtering fixed date texts */
IMPORT char        *rc_grp_sep;                 /* Fixed date list grouping separator [-c]g[text] */
IMPORT Bool         rc_special_flag;            /* -jc */
IMPORT Bool         rc_both_dates_flag;         /* -jcb */
IMPORT Bool         rc_use_flag;                /* -c */
IMPORT Bool         rc_all_dates_flag;          /* -C[] or -C[][T|W|M|Y] or -c[][T|W|M|Y] or -F<> */
IMPORT Bool         rc_sort_des_flag;           /* [-c]- */
IMPORT Bool         rc_enable_fn_flag;          /* [-c]a */
IMPORT Bool         rc_alternative_format_flag; /* [-c]A */
IMPORT Bool         rc_bypass_shell_cmd;        /* [-c]B */
IMPORT Bool         rc_enable_hda_flag;         /* [-c]e */
IMPORT Bool         rc_enable_hdl_flag;         /* [-c]E */
IMPORT Bool         rc_weekno_flag;             /* [-c]k */
IMPORT Bool         rc_period_list;             /* [-c]l */
IMPORT Bool         rc_omit_date_flag;          /* [-c]o */
IMPORT Bool         rc_suppr_date_part_flag;    /* [-c]U */
IMPORT Bool         rc_title_flag;              /* [-c]x */
IMPORT Bool         rc_count_flag;              /* [-c]z */
IMPORT Bool         rc_zero_dates_flag;         /* [-c]Z */
IMPORT Bool         rc_period_flag;             /* [-c]<<<<n>>[<d|w|+|-]>|`mmdd'|`mmww[w]'<n>> */
IMPORT Bool         rc_tomorrow_flag;           /* [-c]t */
IMPORT Bool         rc_week_flag;               /* [-c]w */
IMPORT Bool         rc_month_flag;              /* [-c]m */
IMPORT Bool         rc_year_flag;               /* [-c]y */
IMPORT Bool         rc_week_year_flag;          /* [-c<<n>>]w */
IMPORT Bool         rc_forwards_flag;           /* [-c<<n>|w|m|y>]+ */
IMPORT Bool         rc_fwdf_buffer;             /* Buffers the state of `rc_forwards_flag' */
IMPORT Bool         rc_backwards_flag;          /* [-c<<n>|w|m|y>]- */
IMPORT Bool         rc_bwdf_buffer;             /* Buffers the state of `rc_backwards_flag' */
IMPORT Bool         rc_feb_29_to_feb_28;        /* `--leap-day=february' given */
IMPORT Bool         rc_feb_29_to_mar_01;        /* `--leap-day=march' given */
IMPORT Bool         rc_export_ldvar_flag;       /* `--export-date-variables' given */
IMPORT Bool         rc_export_ltvar_flag;       /* `--export-text-variables' given */
IMPORT Bool         is_date_given;              /* Is a command (explicit date) given in the command line? */
IMPORT Bool         date_enables_year;          /* Does the command enables a year implicitly? */
IMPORT Bool         shell_escape_done;          /* Stores whether a %shell escape special text is run */
#endif /* USE_RC */



/*
*  Define public(extern) variables.
*/
/*
   Number of days in months.
*/
PUBLIC const int  dvec[]=
 {
   31, 28, 31, 30, 31, 30,
   31, 31, 30, 31, 30, 31
 };
/*
   Number of past days of month.
*/
PUBLIC const int  mvec[]=
 {
     0,  31,  59,  90, 120, 151,
   181, 212, 243, 273, 304, 334
 };
/*
   The long option table is a vector of `Lopt_struct' terminated by an element
     containing a `long_name' which is zero!
   All initialization strings of the structure member `long_name' CAN be entered
     case insensitive (it is recommended to use them in lower case letters only),
     but in an ascending lexical sort order, and the table MUST be initialized
     by distinguishable texts given to the structure member `long_name'.
   The table structure is as follows:
     `symbolic_name': The symbolic name of the long option (a distingushable number [int]).
     `long_name':     The full name of the long option (case insensitive.)
     `short_name':    A list of short options (maximum 5 (==LARG_MAX-1)) terminated
                        by NULL, its order corresponds to the order of the field
                        `largs' (case sensitive), means "a" is another option than "A".
     `larg_mode':     States whether an option requires an argument or not.
                        LARG_NO or 0 means:
                          Option may not have an argument, e.g. --foo
                        LARG_NO_OR_ONE or 1 means:
                          Option may have an argument,
                          which is either SPECIAL or GIVEN, e.g. --foo or --foo=BAR or --foo=ARG
                        LARG_ONE or 2 means:
                          Option must have an argument,
                          which must be either SPECIAL if one or more `largs' are defined,
                          or which must be GIVEN if no `largs' are defined,
                          e.g. --foo=BAR or --foo=ARG
                        LARG_ONE_OR_ARG or 3 means:
                          Option must have an argument,
                          which could be either SPECIAL if one or more `largs' are defined,
                          or which could be GIVEN if one ore more `largs' are defined,
                          but none of these `largs' match the GIVEN argument,
                          e.g. --foo=BAR or --foo=ARG
                        If this field is set to LARG_NO_OR_ONE and NO argument
                        trails the option, the `short_name[0]' contains
                        the default value of the argument and is returned!
                        It's an ILLEGAL case to set this field to LARG_ONE_OR_ARG and NO `largs'
                        are defined in the `lopt[]' table, but one or more `short_name' entries!
     `largs':         A list of possible SPECIAL arguments (maximum 5 (==LARG_MAX-1))
                        terminated by NULL, from which only one may be selected at a time.
                      If its first element is set to NULL, any GIVEN argument is allowed,
                        otherwise only one of the listed SPECIAL arguments.
                      The SPECIAL arguments may be abbreviated in case this is
                        done unambigously; and they can be given case insensitive.
*/
PUBLIC const Lopt_struct  lopt[]=
{
/*
  { int symbolic_name, char *long_name, char *short_name[LARG_MAX], int larg_mode, char *largs[LARG_MAX] }
*/

#if USE_RC
  {
    SYM_ALTERNATIVE_FORMAT,
    "alternative-format",
    {"A", NULL},
    LARG_NO,
    {NULL}
  },
  {
    SYM_BIORHYTHM_AXIS_LEN,
    "biorhythm-axis",
    {"$", NULL},
    LARG_ONE,
    {NULL}
  },
#endif
  {
    SYM_BLOCKS,
    "blocks",
    {"b", NULL},
    LARG_ONE,
    {NULL}
  },
#if USE_RC
  {
    SYM_BYPASS_SHELL_CMD,
    "bypass-shell-command",
    {"B", NULL},
    LARG_NO,
    {NULL}
  },
#endif
  {
    SYM_CALENDAR_DATES,
    "calendar-dates",
    {"j", "jb", NULL},
    LARG_ONE,
    {"special", "both", NULL}
  },
  {
    SYM_CC_HDY,
    "cc-holidays",
    {"q", NULL},
    LARG_ONE,
    {NULL}
  },
  {
    SYM_CHRISTIAN_HDY,
    "christian-holidays",
    {NULL},
    LARG_NO,
    {NULL}
  },
  {
    SYM_LICENSE2,
    "copyleft",
    {"L", NULL},
    LARG_NO,
    {NULL}
  },
  {
    SYM_LICENSE3,
    "copyright",
    {"L", NULL},
    LARG_NO,
    {NULL}
  },
  {
    SYM_DATE_FORMAT,
    "date-format",
    {"J\001", "J\002", "J\003", /* e.g.: "J\004", */ "J",      NULL},
    LARG_ONE_OR_ARG,
#if USE_DE
    {CC_DE,   CC_US,   CC_GB,   /* e.g.: "OTHER", */ LARG_TXT, NULL}
#else /* !USE_DE */
    {CC_DE,   CC_US,   CC_GB,   /* e.g.: "OTHER", */ N_("ARG"), NULL}
#endif /* !USE_DE */
  },
#if USE_RC
  {
    SYM_DATE_VARIABLE,
    "date-variable",
    {"v", NULL},
    LARG_ONE,
    {NULL}
  },
#endif
  {
    SYM_DEBUG,
    "debug",
    {"0", "1", "2", "3", "4", NULL},
    LARG_NO_OR_ONE,
    {"internal", "handled", "unhandled", "all", "abort", NULL}
  },
#if USE_RC
  {
    SYM_DESC_FIXED_DATES_LIST,
    "descending-fixed-dates",
    {"c-", "C-", NULL},
    LARG_NO_OR_ONE,
    {"short", "long", NULL}
  },
#endif
  {
    SYM_DESC_HOLIDAY_LIST,
    "descending-holiday-list",
    {"n-", "N-", NULL},
    LARG_NO_OR_ONE,
    {"long", "short", NULL}
  },
  {
    SYM_DISABLE_HIGHLIGHTING,
    "disable-highlighting",
    {"Hno", NULL},
    LARG_NO,
    {NULL}
  },
#if USE_RC
  {
    SYM_END_OF_MONTH,
    "end-of-month",
    {"m+", NULL},
    LARG_NO,
    {NULL}
  },
  {
    SYM_END_OF_WEEK,
    "end-of-week",
    {"w+", NULL},
    LARG_NO,
    {NULL}
  },
  {
    SYM_END_OF_YEAR,
    "end-of-year",
    {"y+", NULL},
    LARG_NO,
    {NULL}
  },
  {
    SYM_EXCLUDE_RC_TITLE,
    "exclude-fixed-dates-list-title",
    {"x", NULL},
    LARG_NO,
    {NULL}
  },
#endif
  {
    SYM_EXCLUDE_HD_TITLE,
    "exclude-holiday-list-title",
    {"X", NULL},
    LARG_NO,
    {NULL}
  },
  {
    SYM_EXIT_STAT_HELP_127,
    "exit-status-help-127",
    {NULL},
    LARG_NO,
    {NULL}
  },
#if USE_RC
  {
    SYM_EXPORT_LOCAL_DVARS,
    "export-date-variables",
    {NULL},
    LARG_NO,
    {NULL}
  },
  {
    SYM_EXPORT_LOCAL_TVARS,
    "export-text-variables",
    {NULL},
    LARG_NO,
    {NULL}
  },
  {
    SYM_FILTER_DAY,
    "filter-day",
    {"D", NULL},
    LARG_ONE,
    {NULL}
  },
  {
    SYM_FILTER_PERIOD,
    "filter-period",
    {"P", NULL},
    LARG_ONE,
    {NULL}
  },
  {
    SYM_FILTER_TEXT,
    "filter-text",
    {"I", NULL},
    LARG_ONE,
    {NULL}
  },
  {
    SYM_FIXED_DATES,
    "fixed-dates",
    {"jc", "jcb", NULL},
    LARG_ONE,
    {"special", "both", NULL}
  },
#endif
  {
    SYM_FORCE_HIGHLIGHTING,
    "force-highlighting",
    {"Hyes", NULL},
    LARG_NO,
    {NULL}
  },
  {
    SYM_GREG_REFORM,
    "gregorian-reform",
    {"G\001", "G\002", "G\003", "G\004", "G",      NULL},
    LARG_ONE_OR_ARG,
#if USE_DE
    {"1582",  "1700",  "1752",  "1753",  LARG_TXT, NULL}
#else /* !USE_DE */
    {"1582",  "1700",  "1752",  "1753",  N_("ARG"), NULL}
#endif /* !USE_DE */
  },
#if USE_RC
  {
    SYM_GROUPING_TEXT,
    "grouping-text",
    {"g", NULL},
    LARG_NO_OR_ONE,
    {NULL}
  },
#endif
  {
    SYM_HELP,
    "help",
    {"h", NULL},
    LARG_NO,
    {NULL}
  },
#if USE_RC
  {
    SYM_HERE_FILE,
    "here",
    {"#", NULL},
    LARG_ONE,
    {NULL}
  },
#endif
  {
    SYM_HIGHLIGHTING,
    "highlighting",
    {"H", NULL},
    LARG_ONE,
    {NULL}
  },
  {
    SYM_HOLIDAY_DATES,
    "holiday-dates",
    {"jn", "jnb", NULL},
    LARG_ONE,
    {"special", "both", NULL}
  },
  {
    SYM_HOLIDAY_LIST,
    "holiday-list",
    {"n", "N", NULL},
    LARG_NO_OR_ONE,
    {"long", "short", NULL}
  },
#if USE_RC
  {
    SYM_INCLUDE_CONS_NO,
    "include-consecutive-number",
    {"z", NULL},
    LARG_NO,
    {NULL}
  },
  {
    SYM_INCLUDE_HOLIDAY,
    "include-holidays",
    {"e", "E", NULL},
    LARG_NO_OR_ONE,
    {"long", "short", NULL}
  },
  {
    SYM_INCLUDE_FILENAME,
    "include-resource-file-name",
    {"a", NULL},
    LARG_NO,
    {NULL}
  },
  {
    SYM_INCLUDE_TODAY,
    "include-today",
    {"d", NULL},
    LARG_NO,
    {NULL}
  },
  {
    SYM_INCLUDE_WEEK_NO,
    "include-week-number",
    {"k", NULL},
    LARG_NO,
    {NULL}
  },
  {
    SYM_LEAP_DAY,
    "leap-day",
    {"f", "m", NULL},
    LARG_ONE,
    {"february", "march", NULL}
  },
#endif
  {
    SYM_LICENSE1,
    "license",
    {"L", NULL},
    LARG_NO,
    {NULL}
  },
#if USE_RC
  {
    SYM_LIST_MODE,
    "list-mode",
    {"l", NULL},
    LARG_NO,
    {NULL}
  },
  {
    SYM_FIXED_DATES_LIST,
    "list-of-fixed-dates",
    {"c", "C", NULL},
    LARG_NO_OR_ONE,
    {"short", "long", NULL}
  },
#endif
  {
    SYM_LONG_HELP1,
    "long-help",
    {"hh", NULL},
    LARG_NO_OR_ONE,
    {NULL}
  },
#ifdef GCAL_EMAIL
  {
    SYM_MAIL,
    "mail",
    {"!", NULL},
    LARG_ONE,
    {NULL}
  },
#endif
#if USE_RC
  {
    SYM_MONTH,
    "month",
    {"m", NULL},
    LARG_NO,
    {NULL}
  },
  {
    SYM_MOONIMAGE_LINES,
    "moonimage-lines",
    {"&", NULL},
    LARG_ONE,
    {NULL}
  },
  {
    SYM_OMIT_DATE_PART,
    "omit-multiple-date-part",
    {"o", NULL},
    LARG_NO,
    {NULL}
  },
#endif
  {
    SYM_ORTHODOX_CALENDAR,
    "orthodox-calendar",
    {"O", NULL},
    LARG_NO,
    {NULL}
  },
#if USE_PAGER
  {
    SYM_PAGER,
    "pager",
    {"p", NULL},
    LARG_NO,
    {NULL}
  },
#endif
#if USE_RC
  {
    SYM_PERIOD,
    "period-of-fixed-dates",
    {"c", NULL},
    LARG_ONE,
    {NULL}
  },
  {
    SYM_RESOURCE_FILE,
    "resource-file",
    {"f", NULL},
    LARG_ONE,
    {NULL}
  },
#endif
  {
    SYM_RESPONSE_FILE,
    "response-file",
    {"R", NULL},
    LARG_ONE,
    {NULL}
  },
#ifdef GCAL_SHELL
  {
    SYM_SCRIPT_FILE,
    "shell-script",
    {"S", NULL},
    LARG_ONE,
    {NULL}
  },
#endif
#if USE_RC
  {
    SYM_START_OF_MONTH,
    "start-of-month",
    {"m-", NULL},
    LARG_NO,
    {NULL}
  },
  {
    SYM_START_OF_WEEK,
    "start-of-week",
    {"w-", NULL},
    LARG_NO,
    {NULL}
  },
  {
    SYM_START_OF_YEAR,
    "start-of-year",
    {"y-", NULL},
    LARG_NO,
    {NULL}
  },
#endif
  {
    SYM_STARTING_DAY,
    "starting-day",
    {"s", NULL},
    LARG_ONE,
    {NULL}
  },
  {
    SYM_SUPPRESS_CALENDAR,
    "suppress-calendar",
    {"u", NULL},
    LARG_NO,
    {NULL}
  },
#if USE_RC
  {
    SYM_SUPPRESS_DATE_PART,
    "suppress-date-part",
    {"U", NULL},
    LARG_NO,
    {NULL}
  },
  {
    SYM_TEXT_VARIABLE,
    "text-variable",
    {"r", NULL},
    LARG_ONE,
    {NULL}
  },
  {
    SYM_TODAY,
    "today",
    {"c", NULL},
    LARG_NO,
    {NULL}
  },
  {
    SYM_TOMORROW,
    "tomorrow",
    {"t", NULL},
    LARG_NO,
    {NULL}
  },
#endif
  {
    SYM_TYPE_OF_CALENDAR,
    "type",
    {"i", "i-", NULL},
    LARG_ONE,
    {"special", "standard", NULL}
  },
  {
    SYM_LONG_HELP2,
    "usage",
    {"hh", NULL},
    LARG_NO_OR_ONE,
    {NULL}
  },
  {
    SYM_VERSION,
    "version",
    {"V", NULL},
    LARG_NO,
    {NULL}
  },
#if USE_RC
  {
    SYM_WEEK,
    "week",
    {"w", NULL},
    LARG_NO,
    {NULL}
  },
#endif
  {
    SYM_CALENDAR_WITH_WEEKNO,
    "with-week-number",
    {"K", NULL},
    LARG_NO,
    {NULL}
  },
  {
    SYM_STANDARD_HDY,
    "without-standard-holidays",
    {NULL},
    LARG_NO,
    {NULL}
  },
#if USE_RC
  {
    SYM_YEAR,
    "year",
    {"y", NULL},
    LARG_NO,
    {NULL}
  },
  {
    SYM_ZERO_DATES,
    "zero-dates-only",
    {"Z", NULL},
    LARG_NO,
    {NULL}
  },
#endif
  { SYM_NIL, NULL, {NULL}, LARG_NO, {NULL} }
};
#if USE_RC
/*
   The table used to inform about the characters used in the
     %? inclusive/exclusive day special texts is a vector of
     `Ed_struct' terminated by an element containing an `info' which is zero!
*/
PUBLIC const Ed_struct    info_exclusive_day[]=
{
/*
  { char id, char *info },
*/
#  if USE_DE
  { RC_EX_LHDY_CHAR,      "gesetzlichen Feiertage" },
  { RC_EX_AHDY_CHAR,      "Feiertage" },
  { RC_EX_MON_CHAR,       "Montage" },
  { RC_EX_TUE_CHAR,       "Dienstage" },
  { RC_EX_WED_CHAR,       "Mittwoche" },
  { RC_EX_THU_CHAR,       "Donnerstage" },
  { RC_EX_FRI_CHAR,       "Freitage" },
  { RC_EX_SAT_CHAR,       "Samstage" },
  { RC_EX_SUN_CHAR,       "Sonntage" },
  { RC_EX_MON_2_THU_CHAR, "Montage...Donnerstage" },
  { RC_EX_MON_2_FRI_CHAR, "Montage...Freitage" },
#  else /* !USE_DE */
  { RC_EX_LHDY_CHAR,      N_("legal holidays") },
  { RC_EX_AHDY_CHAR,      N_("holidays") },
  { RC_EX_MON_CHAR,       N_("Mondays") },
  { RC_EX_TUE_CHAR,       N_("Tuesdays") },
  { RC_EX_WED_CHAR,       N_("Wednesdays") },
  { RC_EX_THU_CHAR,       N_("Thursdays") },
  { RC_EX_FRI_CHAR,       N_("Fridays") },
  { RC_EX_SAT_CHAR,       N_("Saturdays") },
  { RC_EX_SUN_CHAR,       N_("Sundays") },
  { RC_EX_MON_2_THU_CHAR, N_("Mondays...Thursdays") },
  { RC_EX_MON_2_FRI_CHAR, N_("Mondays...Fridays") },
#  endif /* !USE_DE */
  { '\0', NULL }
};
#endif /* USE_RC */
/*
   The table used to inform about the supported date formats is a vector of
     `Df_struct' terminated by an element containing an `info' which is zero!
*/
PUBLIC const Df_struct    info_date_format[]=
{
/*
  { char *id, char *info, char *format },
*/
#if USE_DE
  { "", "Tag-Gruppe (genau ein Mitglied mu"SZ" definiert werden):",          "" },
  { "", "Tageszahl mit f"UE"hrenden Nullen",                                 "%d" },
  { "", "Tageszahl mit f"UE"hrenden Leerzeichen",                            "%D" },
  { "", "Tageszahl mit f"UE"hrenden Nullen und nachfolgendem Anhang",        "%u" },
  { "", "Tageszahl mit f"UE"hrenden Leerzeichen und nachfolgendem Anhang",   "%U" },
  { "", "Monat-Gruppe (genau ein Mitglied mu"SZ" definiert werden):",        "" },
  { "", "Vollst"AE"ndiger Monatsname",                                       "%B" },
  { "", "Abgek"UE"rzter Monatsname (3 Buchstaben)",                          "%b" },
  { "", "Monatszahl mit f"UE"hrender Null",                                  "%m" },
  { "", "Monatszahl mit f"UE"hrendem Leerzeichen",                           "%M" },
  { "", "Jahr-Gruppe (genau ein Mitglied mu"SZ" definiert werden):",         "" },
  { "", "Vollst"AE"ndige Jahreszahl mit f"UE"hrenden Nullen",                "%y" },
  { "", "Vollst"AE"ndige Jahreszahl mit f"UE"hrenden Leerzeichen",           "%Y" },
  { "", "Letzten zwei Ziffern der Jahreszahl mit f"UE"hrender Null",         "%z" },
  { "", "Letzten zwei Ziffern der Jahreszahl mit f"UE"hrendem Leerzeichen",  "%Z" },
  { "", "Wochentagsname-Gruppe (ein Mitglied darf definiert werden):",       "" },
  { "", "Vollst"AE"ndiger Wochentagsname",                                   "%A" },
  { "", "Abgek"UE"rzter Wochentagsname (3 Buchstaben)",                      "%W" },
  { "", "Abgek"UE"rzter Wochentagsname (2 Buchstaben)",                      "%w" },
  { "", "Hervorhebung-Gruppe (alle Mitglieder m"UE"ssen definiert werden):", "" },
  { "", "Start von Hervorhebungssequenz/Markierungszeichen",                 "%1" },
  { "", "Ende von Hervorhebungssequenz/Markierungszeichen",                  "%2" },
  { "", "Zeichenersetzung:",                                                 "" },
  { "", "Leerzeichen (' ')",                                                 "_" },
  { "", "Unterstrichzeichen ('_')",                                          "\\_" },
  { "", "Prozentzeichen ('%')",                                              "\\%" },
#else /* !USE_DE */
  { "", N_("Day group (exactly one member must be defined):"),    "" },
  { "", N_("Day number with leading zeroes"),                     "%d" },
  { "", N_("Day number with leading spaces"),                     "%D" },
  { "", N_("Day number with leading zeroes and trailing suffix"), "%u" },
  { "", N_("Day number with leading spaces and trailing suffix"), "%U" },
  { "", N_("Month group (exactly one member must be defined):"),  "" },
  { "", N_("Complete month name"),                                "%B" },
  { "", N_("Abbreviated month name (3 letters)"),                 "%b" },
  { "", N_("Month number with leading zero"),                     "%m" },
  { "", N_("Month number with leading space"),                    "%M" },
  { "", N_("Year group (exactly one member must be defined):"),   "" },
  { "", N_("Complete year number with leading zeroes"),           "%y" },
  { "", N_("Complete year number with leading spaces"),           "%Y" },
  { "", N_("Last two digits of year number with leading zero"),   "%z" },
  { "", N_("Last two digits of year number with leading space"),  "%Z" },
  { "", N_("Weekday name group (one member may be defined):"),    "" },
  { "", N_("Complete weekday name"),                              "%A" },
  { "", N_("Abbreviated weekday name (3 letters)"),               "%W" },
  { "", N_("Abbreviated weekday name (2 letters)"),               "%w" },
  { "", N_("Highlighting group (all members must be defined):"),  "" },
  { "", N_("Start of highlighting sequence/marking character"),   "%1" },
  { "", N_("End of highlighting sequence/marking character"),     "%2" },
  { "", N_("Character replacement:"),                             "" },
  { "", N_("Space/blank (' ') character"),                        "_" },
  { "", N_("Underscore ('_') character"),                         "\\_" },
  { "", N_("Percent ('%') character"),                            "\\%" },
#endif /* !USE_DE */
  { NULL, NULL, NULL }
};
/*
   The supported date formats table is a vector of `Df_struct'
     terminated by an element containing an `info' which is zero!
   If you extend this table by some more date formats, please extend the
     according `lopt[]' table entry for the `--date-format' long-style option, too!
     If you do so, remember that each new entry in the `lopt[]' table for
     SYM_DATE_FORMAT must have a new `short_name' text
     "J\NEXT-CONSECUTIVE-OCTAL-NUMBER-OF-THIS-NEW-INDEX" and a corresponding
     identifying text in `largs' (with a length of less than 6 characters).
     The simple "J" text in the `short_name' member and its corresponding
     LARG_TXT must always trail any new entry, which will be possibly added
     in future.
*/
PUBLIC Df_struct    supported_date_format[LARG_MAX-1]=
{
/*
  { char *id, char *info, char *format },
*/
#if USE_DE
  { CC_DE, "Deutschland",       "%w,  %1%D%2 %b %y" },
  { CC_US, "USA",               "%W, %b  %1%U%2 %y" },
  { CC_GB, "Gro"SZ"britannien", "%W,  %1%U%2 %b %y" },
/*
   The next table field can be provided with further data!
   If more "burned-in" data is needed, LARG_MAX must be increased accordingly,
   because it's necessary to respect the data of this fields in the functions,
   which manage the long-style arguments!
*/
  { NULL, NULL, NULL },
#else /* !USE_DE */
  { CC_DE, N_("Germany"),       "%w,  %1%D%2 %b %y" },
  { CC_US, N_("U.S.A."),        "%W, %b  %1%U%2 %y" },
  { CC_GB, N_("Great Britain"), "%W,  %1%U%2 %b %y" },
/*
   The next table field can be provided with further data!
   If more "burned-in" data is needed, LARG_MAX must be increased accordingly,
   because it's necessary to respect the data of this fields in the functions,
   which manage the long-style arguments!
*/
  { NULL, NULL, NULL },
#endif /* !USE_DE */
/*
   This next "empty" table field must kept UNTOUCHED, because it marks the end of the table!
*/
  { NULL, NULL, NULL }
};
PUBLIC Df_struct   *date_format=supported_date_format;   /* Points to the used date format */
/*
   The Gregorian Reformation dates table is a vector of `Greg_struct'
     terminated by an element containing a `year' which is zero!
   If you extend this table by some more dates, please extend the according
     `lopt[]' table entry for the `--gregorian-reform' long-style option, too!
     If you do so, remember that each new entry in the `lopt[]' table for
     SYM_GREG_REFORM must have a new `short_name' text
     "G\NEXT-CONSECUTIVE-OCTAL-NUMBER-OF-THIS-NEW-INDEX" and a corresponding
     year of Gregorian Reformation text in `largs'.  The simple "G" text in
     the `short_name' member and its corresponding LARG_TXT must always
     trail any new entry, which will be possibly added in future.
*/
PUBLIC Greg_struct  greg_reform_date[LARG_MAX-1]=
{
/*
  { int year, int month, int f_day, int l_day },
*/
  { 1582, 10,  5, 14 },
  { 1700,  2, 19, 28 },
  { 1752,  9,  3, 13 },
  { 1753,  2, 18, 28 },
/*
   This next "empty" table field must kept UNTOUCHED, because it marks the end of the table!
*/
  { 0, 0, 0, 0  }
};
PUBLIC Greg_struct *greg=greg_reform_date;         /* Points to the used Gregorian Reformation date */
PUBLIC Greg_struct  users_greg;                    /* User defined Gregorian Reformation date */
#ifdef GCAL_EMAIL
PUBLIC FILE        *tfp=(FILE *)NULL;              /* Temporary file which is send by the mailer */
#endif
PUBLIC Ml_struct   *month_list=(Ml_struct *)NULL;  /* Used if a list/range of months/years is given */
PUBLIC Hls_struct   ehls1s;                        /* Effective hls 1 start (current day) */
PUBLIC Hls_struct   ehls1e;                        /* Effective hls 1 end (current day) */
PUBLIC Hls_struct   ehls2s;                        /* Effective hls 2 start (holiday) */
PUBLIC Hls_struct   ehls2e;                        /* Effective hls 2 end (holiday) */
#ifdef DJG
PUBLIC Usint        testval;                       /* Set to SHRT_MAX for checking the maximum table range */
#else
PUBLIC Uint         testval;                       /* Set to INT_MAX for checking the maximum table range */
#endif
PUBLIC Uint         maxlen_max=MAXLEN_MAX;         /* Actual size of all string vectors */
PUBLIC int          len_year_max;                  /* String length of the maximum year able to compute */
PUBLIC int          len_dayname_max;               /* Maximum string length of a textual day_name() */
PUBLIC int          len_monthname_max;             /* Maximum string length of a textual month_name() */
PUBLIC int          len_suffix_max;                /* Maximum string length of a textual day_suffix() [if any] */
PUBLIC int          warning_level=SPECIAL_VALUE;   /* --debug[=0...WARN_LVL_MAX], SPECIAL VALUE at startup */
PUBLIC int          start_day;                     /* -s<0,1...7|day name> */
PUBLIC int          day;                           /* Current day */
PUBLIC int          month;                         /* Current month */
PUBLIC int          year;                          /* Current year */
PUBLIC int          act_sec;                       /* Actual second */
PUBLIC int          act_min;                       /* Actual minute */
PUBLIC int          act_hour;                      /* Actual hour */
PUBLIC int          act_day;                       /* Actual day */
PUBLIC int          act_month;                     /* Actual month */
PUBLIC int          act_year;                      /* Actual year */
PUBLIC int          buf_ad;                        /* Buffer of actual day */
PUBLIC int          buf_am;                        /* Buffer of actual month */
PUBLIC int          buf_ay;                        /* Buffer of actual year */
PUBLIC int          fiscal_month=MONTH_MIN;        /* Starting month of a fiscal year */
PUBLIC int          is_tty;                        /* Is output displayed on a terminal? */
PUBLIC int          is_tty1;                       /* Is output directed to channel 1? */
PUBLIC int          is_tty2;                       /* Is output directed to channel 2?*/
#if USE_PAGER
PUBLIC int          tty_rows=SPECIAL_VALUE;        /* Number of tty rows, SPECIAL_VALUE at startup */
PUBLIC int          tty_cols=SPECIAL_VALUE;        /* Number of tty columns, SPECIAL_VALUE at startup */
#endif
PUBLIC int          out_rows;                      /* Number of month rows of a year calendar */
PUBLIC int          out_cols;                      /* Number of month columns of ... */
PUBLIC int          fmt_len;                       /* Format length of a standard/special/both day */
PUBLIC int          is_leap_year;                  /* Is current year a leap year? */
PUBLIC int          len_prgr_name;                 /* Length of actual program name */
PUBLIC int          exit_stat_help=0;              /* Termination status on --help, --version etc. */
#ifdef GCAL_EMAIL
PUBLIC char        *tfn=(char *)NULL;              /* Name of tempfile used by the mailer */
#endif
PUBLIC char        *yy_lit=(char *)NULL;           /* The "yy" text */
PUBLIC char        *yyyy_lit=(char *)NULL;         /* The "yyyy" text */
PUBLIC char        *mm_lit=(char *)NULL;           /* The "mm" text */
PUBLIC char        *www_lit=(char *)NULL;          /* The "www" text */
PUBLIC char        *dd_lit=(char *)NULL;           /* The "dd" text */
PUBLIC char        *larg_lit=(char *)NULL;         /* The "ARG" text */
PUBLIC char        *s1=(char *)NULL;               /* General purpose text buffer */
PUBLIC char        *s2=(char *)NULL;               /* General purpose text buffer */
PUBLIC char        *s3=(char *)NULL;               /* General purpose text buffer */
PUBLIC char        *s4=(char *)NULL;               /* General purpose text buffer */
PUBLIC char        *prgr_name=(char *)NULL;        /* Stores the actual program name */
PUBLIC char        *tz=(char *)NULL;               /* Pointer to the $TZ (timezone) environment variable */
PUBLIC char        *cc=(char *)NULL;               /* The argument `--cc-holidays=CC[+CC+...]' received */
#ifdef GCAL_EPAGER
PUBLIC char        *ext_pager=(char *)NULL;        /* Name of external pager program */
#endif
PUBLIC Bool         use_day_suffix;                /* Day suffix format specifier given in date format? */
PUBLIC Bool         use_short3_day_name;           /* 3 char day name format specifier given in date format? */
PUBLIC Bool         use_day_zeroleaded;            /* Day leaded with zeroes format specifier given in date format? */
PUBLIC Bool         use_year_zeroleaded;           /* Year leaded with zeroes format specifier given in date format? */
PUBLIC Bool         standard_hdy=TRUE;             /* Use the standard holidays by default */
PUBLIC Bool         christian_hdy=FALSE;           /* Don't use Christian holidays by default */
PUBLIC Bool         orthodox_calendar=FALSE;       /* -O (compute leap years as done by Eastern churches) */
PUBLIC Bool         suppr_cal_flag=FALSE;          /* -u */
PUBLIC Bool         highlight_flag=TRUE;           /* -H<yes> or -H<no> */
PUBLIC Bool         cal_with_weekno=FALSE;         /* -K */
PUBLIC Bool         cal_special_flag=FALSE;        /* -j */
PUBLIC Bool         cal_both_dates_flag=FALSE;     /* -jb */
PUBLIC Bool         holiday_flag=FALSE;            /* -n|N */
PUBLIC Bool         hd_legal_days_only=FALSE;      /* -N */
PUBLIC Bool         hd_sort_des_flag=FALSE;        /* [-n|N]- */
PUBLIC Bool         hd_special_flag=FALSE;         /* -jn */
PUBLIC Bool         hd_both_dates_flag=FALSE;      /* -jnb */
PUBLIC Bool         hd_title_flag=TRUE;            /* -X */
PUBLIC Bool         is_fiscal_year=FALSE;          /* ':' char found in argument (`mm':`yyyy') */
PUBLIC Bool         is_3month_mode=FALSE;          /* Argument is "." or ".+" or "+-" */
PUBLIC Bool         is_3month_mode2=FALSE;         /* Argument is ".." -> current quarter of actual year */
PUBLIC Bool         is_ext_year=FALSE;             /* Is an extended list/range of years given? */
PUBLIC Bool         is_ext_list=FALSE;             /* Is an extended list of months/years given? */
PUBLIC Bool         is_ext_range=FALSE;            /* Is an extended range of months/years given? */
PUBLIC Bool         is_special_range=FALSE;        /* Is a special range of a selected month of years given? */
#ifdef GCAL_NLS
PUBLIC Bool         is_de=FALSE;                   /* Support of German language? */
PUBLIC Bool         is_en=FALSE;                   /* Support of English language? */
#endif
PUBLIC Bool         special_calsheet_flag;         /* -i */
#if USE_HLS
PUBLIC Bool         emu_hls=FALSE;                 /* Must we emulate the highlighting sequences? */
#else /* !USE_HLS */
PUBLIC Bool         emu_hls=TRUE;                  /* Must we emulate the highlighting sequences? */
#endif /* !USE_HLS */
#if USE_PAGER
PUBLIC Bool         pager_flag=FALSE;              /* -p */
#endif



/*
   Define local(static) variables.
*/
#ifdef GCAL_EPAGER
LOCAL pid_t        child_pid;                      /* Child process id of external pager */
LOCAL int          pipe_fd[2];                     /* Pipe file descriptors */
LOCAL int          sys_fd[2];                      /* Buffer of system file descriptors 0 and 1 */
#endif
LOCAL Df_struct    users_date_format;              /* User defined date format */
LOCAL Uint         month_list_max=MONTH_MAX+1;     /* Maximum number of `month_list[]' table elems */
LOCAL int          lopt_id;                        /* The index value of a long option */
#ifdef GCAL_EPAGER
/*
   The external pager program names table is a vector of char pointers elements,
     which must be terminated by a NULL element!
*/
LOCAL char        *pagers[]={PAGER1_PROG, PAGER2_PROG, PAGER3_PROG, NULL};
#endif
#ifdef GCAL_EMAIL
LOCAL char        *email_adr=(char *)NULL;         /* Email address gcal's output is send to */
#endif
#ifdef GCAL_SHELL
LOCAL char        *shl_filename=(char *)NULL;      /* File name of shell script to write -S<NAME> */
#endif
LOCAL char        *rsp_filename=(char *)NULL;      /* Name of response file to read (@file) or write (-R<NAME>) */
LOCAL char        *hl_seq=(char *)NULL;            /* Text containing user defined highlighting sequences -H<> */
LOCAL char        *errtxt_dformat=(char *)NULL;    /* Points to "date format error location" description text */
LOCAL Bool         year_flag=FALSE;                /* -b<1|2|3|4|6|12> */
LOCAL Bool         switch_back_flag=FALSE;         /* [-i]- */



   PUBLIC int
main (argc, argv)
   int   argc;
   char *argv[];
/*
   The Gcal program entry point   =8^)
*/
{
   auto         FILE   *fp=(FILE *)NULL;
   auto         Uint    my_argc_max=MY_ARGC_MAX;
   auto         int     my_argc=1;
   auto         int     i;
   auto         char  **my_argv=(char **)NULL;
#if USE_RC
   auto         char   *ptr_optarg;
#  ifdef GCAL_SHELL
   static const char   *these_short_opts_need_args="#DFHIPRSbfqrsv";
#  else /* !GCAL_SHELL */
   static const char   *these_short_opts_need_args="#DFHIPRbfqrsv";
#  endif /* !GCAL_SHELL */
#else /* !USE_RC */
#  ifdef GCAL_SHELL
   static const char   *these_short_opts_need_args="HRSbqs";
#  else /* !GCAL_SHELL */
   static const char   *these_short_opts_need_args="HRbqs";
#  endif /* !GCAL_SHELL */
#endif /* !USE_RC */
   auto         char   *ptr_char;
   auto         char   *y_txt;
   auto         char   *m_txt;
   auto         char   *w_txt;
   auto         char   *d_txt;
#ifdef GCAL_NLS
   auto         Bool    gnu_env_var_language_set=FALSE;
#endif
   auto         Bool    show_calendar=TRUE;
   auto         Bool    b_dummy;


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
#if HAVE_ASSERT_H
   /*
      To ensure safe program operations,
        MAXLEN_MAX must be 1024 minimum and `testval' maximum!
   */
   assert(MAXLEN_MAX>=1024);
   assert((Uint)MAXLEN_MAX<=testval);
#endif
   /*
      Then we have to allocate all string vectors we'll use.
   */
   allocate_all_strings (MAXLEN_MAX, __FILE__, (long)__LINE__);
   /*
      Compute the string length of the maximum year able to compute.
   */
   sprintf(s1, "%d", YEAR_MAX);
   len_year_max = (int)strlen(s1);
   /*
      Perform some more assertations for safe program operation.
   */
#if HAVE_ASSERT_H
   /*
      Check if value for maximum number of table entries
        fits to the positive range of a signed int (SHRT_MAX/INT_MAX)!
   */
   assert(my_argc_max>0);
   assert(my_argc_max<=testval);
   assert(len_year_max<11);
   assert(len_year_max>0);
   assert(YEAR_MAX>=YEAR_MIN);
   assert(MONTH_MAX==12);
   assert(CENTURY==1900);
   /*
      For safe program operation, BUF_LEN must be 1 minimum and `testval'-1 maximum!
   */
   assert(BUF_LEN>=1);
   assert((Uint)BUF_LEN<testval);
   assert(strlen(PRGR_NAME)<7);
   assert(strlen(PRGR_NAME)>0);
   assert(strlen(VERSION_NO)>0);
   assert(MY_ARGC_MAX>1);
   assert((Uint)MY_ARGC_MAX<=testval);
   assert(HD_ELEMS_MAX>0);
   assert((Uint)HD_ELEMS_MAX<=testval);
#endif /* HAVE_ASSERT_H */
#if USE_DE
   /*
      USE_DE means German texts and territory specifics by default.
   */
   special_calsheet_flag = FALSE;
#else /* !USE_DE */
#  ifdef GCAL_NLS
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
   /*
      Now check whether we use a native language message catalog
        or the internal default (==English) language texts!
   */
#    if !defined(AMIGA) || defined(__GNUC__)
   /*
      Detect whether the GNU specific $LANGUAGE environment variable is set.
   */
   ptr_char = getenv(ENV_VAR_LANGUAGE);
   if (ptr_char != (char *)NULL)
     gnu_env_var_language_set = TRUE;
   /*
      Detect whether the $LC_ALL, $LC_MESSAGES or the $LANG
        environment variable is set.
   */
   ptr_char = getenv(ENV_VAR_LC_ALL);
   if (ptr_char == (char *)NULL)
    {
#      if HAVE_LC_MESSAGES
      ptr_char = getenv(ENV_VAR_LC_MESSAGES);
      if (ptr_char == (char *)NULL)
#      endif
        ptr_char = getenv(ENV_VAR_LANG);
    }
   /*
      If no $LC_ALL, $LC_MESSAGES and $LANG environment variable is set
        and defined, but the GNU specific $LANGUAGE environment variable,
        use its value for detecting the native language message catalog.
   */
   if (   gnu_env_var_language_set
       && (   ptr_char == (char *)NULL
           || (   (ptr_char != (char *)NULL)
               && !*ptr_char)))
     ptr_char = getenv(ENV_VAR_LANGUAGE);
   /*
      Now check the kind of territory specifics we have to use!
   */
   if (ptr_char != (char *)NULL)
    {
      if (*ptr_char)
       {
         strncpy(s1, ptr_char, 6);
         s1[5] = '\0';
         if (!strncasecmp(s1, "de", 2))
           /*
              We have to use German texts and territory specifics!
           */
           is_de = TRUE;
         else
           if (!strncasecmp(s1, "en", 2))
             /*
                We have to use English texts, but we check for territory
                  specifics (either U.S.A. or Great Britain) later!
             */
             is_en = TRUE;
           else
             /*
                Hmm... We have to check whether the special settings "C" or
                  "POSIX" are given, if so, we have to use English texts
                  and U.S.A. territory by default!
             */
             if (   !strncasecmp(s1, "posix", 5)
                 || !strcasecmp(s1, "c"))
               is_en = TRUE;
       }
      else
        /*
           Environment variable defined but not set, hmm...
             We use English texts and U.S.A. territory specifics by default!
        */
        is_en = TRUE;
    }
   else
     /*
        No environment variable defined.
     */
#    endif /* !AMIGA || __GNUC__ */
     /*
        We use English texts and U.S.A. territory specifics by default!
     */
     is_en = TRUE;
   /*
      Set some variables according to the native language used.
   */
   if (is_en)
    {
      /*
         `is_en' means English texts and U.S.A. territory specifics by default for now.
      */
      special_calsheet_flag = TRUE;
      /*
         Set the date of Gregorian Reformation to 1752 (table index 2 !!)
      */
      greg += 2;
      /*
         Set the date format to U.S.A. style (table index 1 !!)
      */
      date_format++;
#    if !defined(AMIGA) || defined(__GNUC__)
      /*
         Now check whether if we have to use the British date format.
      */
      if (ptr_char != (char *)NULL)
        if (*ptr_char)
         {
           /*
              Skip the first three characters for detecting a possibly territory information.
           */
           for (i=0 ; i < 3 ; i++)
             if (*ptr_char)
               ptr_char++;
           if (*ptr_char)
            {
              strncpy(s1, ptr_char, 3);
              s1[2] = '\0';
              /*
                 We have to use the British date format!
              */
              if (!strcasecmp(s1, CC_GB))
                /*
                   Set the date format to Great Britain style (table index 2 !!)
                */
                date_format++;
            }
         }
#    endif /* !AMIGA || __GNUC__ */
    }
   else
     /*
        `!is_en' means German territory specifics (useful for most Europeans)
          by default and possibly another native language instead of English.
     */
     special_calsheet_flag = FALSE;
#  else /* !GCAL_NLS */
   /*
      !USE_DE means English texts and U.S.A. territory specifics by default.
   */
   special_calsheet_flag = TRUE;
   /*
      Set the date of Gregorian Reformation to 1752 (table index 1 !!)
   */
   greg += 2;
   /*
      Set the date format to U.S.A. style (table index 1 !!)
   */
   date_format++;
#  endif /* !GCAL_NLS */
#endif /* !USE_DE */
   /*
      Test if the default date format is valid.
   */
   if (!correct_date_format (date_format->format, &use_day_suffix, &use_short3_day_name,
                             &use_day_zeroleaded, &use_year_zeroleaded))
#if USE_DE
     errtxt_dformat = "Grundeinstellung";
#else /* !USE_DE */
     errtxt_dformat = _("default");
#endif /* !USE_DE */
   /*
      Test if the program output is redirected or piped.
   */
#if USE_PAGER
   is_tty1 = isatty(1);
   is_tty2 = isatty(2);
   is_tty = (   is_tty1
             && is_tty2);
#  ifdef GCAL_EPAGER
   if (is_tty)
    {
      /*
         Store the sys-stdin/sys-stdout file descriptors.
      */
      dup2(0, sys_fd[0]);
      dup2(1, sys_fd[1]);
    }
#  endif
#else /* !USE_PAGER */
   is_tty = (int)TRUE;
#endif /* !USE_PAGER */
   /*
      Detect the own program name.
   */
   i = (int)strlen(argv[0]);
   if ((Uint)i >= maxlen_max)
     resize_all_strings (i+1, FALSE, __FILE__, (long)__LINE__);
   if (!i)
     strcpy(s3, PRGR_NAME);
   else
     strcpy(s3, argv[0]);
#ifdef SUFFIX_SEP
   /*
      Eliminate version suffix under VMS.
   */
   ptr_char = strrchr(s3, *SUFFIX_SEP);
   if (ptr_char != (char *)NULL)
     *ptr_char = '\0';
#endif
   len_prgr_name = (int)strlen(s3);
#ifdef DJG
   ptr_char = strrchr(s3, *DIR2_SEP);
#else /* !DJG */
   ptr_char = strrchr(s3, *DIR_SEP);
#endif /* !DJG */
   if (ptr_char != (char *)NULL)
    {
      ptr_char++;
      len_prgr_name = (int)strlen(ptr_char);
    }
   else
     ptr_char = s3;
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
   if (   (len_prgr_name > 4)
       && !strcmp(ptr_char+len_prgr_name-4, ".exe"))
    {
      len_prgr_name -= 4;
      *(ptr_char + len_prgr_name) = '\0';
    }
   /*
      The maximum length of the actual program name is always truncated
        to 6 characters for displaying purposes (e.g. in the help screens).
   */
   if (len_prgr_name > 6)
    {
      len_prgr_name = 6;
      *(ptr_char + len_prgr_name) = '\0';
    }
   prgr_name = (char *)my_malloc (len_prgr_name+1,
                                  124, __FILE__, ((long)__LINE__)-1,
                                  "prgr_name", 0);
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
   /*
      Initialize the basic meta texts.
   */
#if USE_DE
   y_txt = "j";
   m_txt = "m";
   w_txt = "w";
   d_txt = "t";
#else /* !USE_DE */
   y_txt = _("y");
   m_txt = _("m");
   w_txt = _("w");
   d_txt = _("d");
#endif /* !USE_DE */
   /*
      Initial memory allocation and initialization of the variables
        which contain the "yy", "yyyy", "mm", "www" and "dd" literals.
   */
   yy_lit = (char *)my_malloc (3, 124, __FILE__, (long)__LINE__, "yy_lit", 0);
   for (i=0 ; i < 2 ; i++)
     yy_lit[i] = *y_txt;
   yy_lit[i] = '\0';
   yyyy_lit = (char *)my_malloc (len_year_max+1, 124, __FILE__, (long)__LINE__, "yyyy_lit", 0);
   for (i=0 ; i < len_year_max ; i++)
     yyyy_lit[i] = *y_txt;
   yyyy_lit[i] = '\0';
   mm_lit = (char *)my_malloc (3, 124, __FILE__, (long)__LINE__, "mm_lit", 0);
   for (i=0 ; i < 2 ; i++)
     mm_lit[i] = *m_txt;
   mm_lit[i] = '\0';
   www_lit = (char *)my_malloc (4, 124, __FILE__, (long)__LINE__, "www_lit", 0);
   for (i=0 ; i < 3 ; i++)
     www_lit[i] = *w_txt;
   www_lit[i] = '\0';
   dd_lit = (char *)my_malloc (3, 124, __FILE__, (long)__LINE__, "dd_lit", 0);
   for (i=0 ; i < 2 ; i++)
     dd_lit[i] = *d_txt;
   dd_lit[i] = '\0';
   /*
      Initial memory allocation and initialization of the variable which
        contains the 3 character "ARG" literal (a proper abbreviation of
        the word "ARGUMENT").
   */
   larg_lit = (char *)my_malloc (4, 124, __FILE__, (long)__LINE__, "larg_lit", 0);
#if USE_DE
   strncpy(larg_lit, LARG_TXT, 4);
#else /* !USE_DE */
   strncpy(larg_lit, _("ARG"), 4);
#endif /* !USE_DE */
   larg_lit[3] = '\0';
   /*
      Initial memory allocation for the `my_argv[]' table.
   */
   my_argv = (char **)my_malloc (MY_ARGC_MAX*sizeof(char *),
                                 124, __FILE__, ((long)__LINE__)-1,
                                 "my_argv[MY_ARGC_MAX]", MY_ARGC_MAX);
   /*
      Initial memory allocation for `month_list[]' structure,
        the number of vector elements must be set to MONTH_MAX+1 in this case,
        because the vector must always be terminated by an empty structure
        element (year==0 && month==0).
   */
   month_list = (Ml_struct *)my_malloc ((MONTH_MAX+1)*sizeof(Ml_struct),
                                        124, __FILE__, ((long)__LINE__)-1,
                                        "month_list[MONTH_MAX+1]", MONTH_MAX+1);
   /*
      Initialize `month_list[]' structure.
   */
   for (i=0 ; i <= MONTH_MAX; i++)
     month_list[i].year=month_list[i].month = 0;
#if USE_RC
   /*
      Initial memory allocation for an element of the `Line_struct' record.
   */
   lineptrs = (Line_struct *)my_malloc(sizeof(Line_struct),
                                       124, __FILE__, ((long)__LINE__)-1,
                                       "lineptrs", 0);
#endif
#if !defined(AMIGA) || defined(__GNUC__)
   /*
      Detect whether a $GCAL_DATE_FORMAT environment variable is set.
   */
   ptr_char = getenv(ENV_VAR_GCAL_DATE_FORMAT);
   if (ptr_char != (char *)NULL)
     if (*ptr_char)
      {
#if USE_DE
        users_date_format.info = "Umgebungsvariable";
#else /* !USE_DE */
        users_date_format.info = _("environment variable");
#endif /* !USE_DE */
        users_date_format.format = (char *)my_malloc (strlen(ptr_char)+1,
                                                      124, __FILE__, ((long)__LINE__)-1,
                                                      "users_date_format.format", 0);
        strcpy(users_date_format.format, ptr_char);
        date_format = &users_date_format;
        /*
           Test if the date format given in environment variable is valid.
        */
        if (!correct_date_format (date_format->format, &use_day_suffix, &use_short3_day_name,
                                  &use_day_zeroleaded, &use_year_zeroleaded))
#if USE_DE
          errtxt_dformat = "Umgebungsvariable";
#else /* !USE_DE */
          errtxt_dformat = _("environment variable");
#endif /* !USE_DE */
        else
          errtxt_dformat = (char *)NULL;
      }
   /*
      Detect whether a $TZ (timezone) environment variable is set.
   */
   tz = getenv(ENV_VAR_TZ);
   if (tz != (char *)NULL)
     if (!*tz)
       tz = (char *)NULL;
   /*
      Detect whether the $GCAL environment variable is set.
   */
   ptr_char = getenv(ENV_VAR_GCAL);
   if (ptr_char != (char *)NULL)
    {
      if (*ptr_char)
       {
         /*
            $GCAL environment variable defined and not empty:
              Copy the OPTIONS, expanded @file arguments or the actual date
              modifier %... argument only; found in environment variable $GCAL;
              into the private `my_argv[]' table.
         */
         while (isspace(*ptr_char))
           ptr_char++;
         i = 0;
         while (*ptr_char)
          {
            if ((Uint)i >= maxlen_max)
              resize_all_strings (maxlen_max<<1, FALSE, __FILE__, (long)__LINE__);
            s1[i++] = *ptr_char++;
            if (   !*ptr_char
                || isspace(*ptr_char))
             {
               s1[i] = '\0';
               if (   *s1 == *SWITCH
                   || *s1 == *SWITCH2
#  if USE_RC
                   || *s1 == RC_ADATE_CHAR
#  endif
                   || *s1 == RSP_CHAR)
                {
                  /*
                     Avoid one or two letter combinations of '-', '/', '%' or '@' characters only!
                  */
                  if (   i == 1
                      || (   (i == 2)
                          && (   s1[1] == *SWITCH
#  if USE_RC
                              || s1[1] == RC_ADATE_CHAR
#  endif
                              || s1[1] == *SWITCH2)))
                    ;   /* Void, don't allocate memory */
                  else
                   {
                     if (*s1 == RSP_CHAR)
                      {
                        /*
                           Try to manage a response file "@file" argument given
                             in the environment variable $GCAL.
                        */
                        rsp_filename = (char *)my_malloc (strlen(s1),
                                                          124, __FILE__, ((long)__LINE__)-1,
                                                          "rsp_filename", 0);
                        strcpy(rsp_filename, s1+1);
                        fp = file_open (&rsp_filename, 0, REsponse, &b_dummy);
                        if (fp != (FILE *)NULL)
                         {
                           my_argv = insert_response_file (fp, rsp_filename, these_short_opts_need_args,
                                                           &my_argc_max, &my_argc, my_argv);
                           fclose(fp);
                         }
                        free(rsp_filename);
                        rsp_filename = (char *)NULL;
                      }
                     else
                      {
                        /*
                           Ok, argument is an option or an actual date modifier %...
                        */
                        if (   (   *s1 == *SWITCH
                                || *s1 == *SWITCH2)
                            && (s1[1] != *SWITCH))
                         {
                           /*
                              Short-style option found.
                           */
                           if (   (strchr(these_short_opts_need_args, s1[1]) != (char *)NULL)
                               && !s1[2])
                            {
                              /*
                                 Short-style option requires an argument, which is separated by
                                   one or more whitespace characters from the option character;
                                   respect this!  This means all separating whitespace characters
                                   between the option character and argument will be eliminated.
                              */
                              while (isspace(*ptr_char))
                                /*
                                   Skip the separating whitespace characters.
                                */
                                ptr_char++;
                              /*
                                 Store needed argument of option adjacent to option character.
                              */
                              while (   *ptr_char
                                     && !isspace(*ptr_char))
                               {
                                 if ((Uint)i >= maxlen_max)
                                   resize_all_strings (maxlen_max<<1, FALSE, __FILE__, (long)__LINE__);
                                 s1[i++] = *ptr_char++;
                               }
                              s1[i] = '\0';
                            }
                         }
                        if ((Uint)my_argc >= my_argc_max)
                         {
                           /*
                              Resize the `my_argv[]' table.
                           */
                           my_argc_max <<= 1;
                           if (my_argc_max*sizeof(char *) > testval)
                             my_argc_max--;
                           my_argv = (char **)my_realloc ((VOID_PTR)my_argv, my_argc_max*sizeof(char *),
                                                          124, __FILE__, ((long)__LINE__)-1,
                                                          "my_argv[my_argc_max]", my_argc_max);
                         }
                        my_argv[my_argc] = (char *)my_malloc (i+1,
                                                              124, __FILE__, ((long)__LINE__)-1,
                                                              "my_argv[my_argc]", my_argc);
                        strcpy(my_argv[my_argc++], s1);
                      }
                   }
                  while (isspace(*ptr_char))
                    ptr_char++;
                  i = 0;
                }
               else
                 if (i)
                  {
                    /*
                       Error, argument is a command.
                    */
#  if USE_DE
                    fprintf(stderr, "%s: Kommando in Umgebungsvariable `%s' angegeben -- %s\n%s\n",
                            prgr_name, ENV_VAR_GCAL, s1, usage_msg ());
#  else /* !USE_DE */
                    fprintf(stderr, _("%s: command in environment variable `%s' found -- %s\n%s\n"),
                            prgr_name, ENV_VAR_GCAL, s1, usage_msg ());
#  endif /* !USE_DE */
                    put_longopt_description (stderr);
                    S_NEWLINE(stderr);
                    exit(126);
                  }
             }
          }
       }
    }
#endif /* !AMIGA || __GNUC__ */
   /*
      Now insert/append the "arguments" of command line
        into the private `my_argv[]' table.
   */
   while (argc > 1)
    {
      argv++;
      /*
         Preload the contents of response files and don't place this option
           text itself in private `my_argv[]' command line options table!
      */
      if (**argv == RSP_CHAR)
       {
         /*
            Try to manage a response file "@file" argument given in the command line.
         */
         rsp_filename = (char *)my_malloc (strlen(*argv),
                                           124, __FILE__, ((long)__LINE__)-1,
                                           "rsp_filename", 0);
         strcpy(rsp_filename, *argv+1);
         fp = file_open (&rsp_filename, 0, REsponse, &b_dummy);
         if (fp != (FILE *)NULL)
          {
            my_argv = insert_response_file (fp, rsp_filename, these_short_opts_need_args,
                                            &my_argc_max, &my_argc, my_argv);
            fclose(fp);
          }
         free(rsp_filename);
         rsp_filename = (char *)NULL;
       }
      else
       {
         register int  len=(int)strlen(*argv);


         /*
            Avoid one or two letter combinations of '-', '/' or '%' characters only!
         */
         if (   (   (len == 1)
                 && (   **argv == *SWITCH
#if USE_RC
                     || **argv == RC_ADATE_CHAR
#endif
                     || **argv == *SWITCH2))
             || (   (len == 2)
                 && (   **argv == *SWITCH
#if USE_RC
                     || **argv == RC_ADATE_CHAR
#endif
                     || **argv == *SWITCH2)
                 && (   *(*argv+1) == *SWITCH
#if USE_RC
                     || *(*argv+1) == RC_ADATE_CHAR
#endif
                     || *(*argv+1) == *SWITCH2)))
           ;   /* Void, don't allocate memory */
         else
          {
            if ((Uint)my_argc >= my_argc_max)
             {
               /*
                  Resize the `my_argv[]' table.
               */
               my_argc_max <<= 1;
               if (my_argc_max*sizeof(char *) > testval)
                 my_argc_max--;
               my_argv = (char **)my_realloc ((VOID_PTR)my_argv, my_argc_max*sizeof(char *),
                                              124, __FILE__, ((long)__LINE__)-1,
                                              "my_argv[my_argc_max]", my_argc_max);
             }
            my_argv[my_argc] = (char *)my_malloc (len+1,
                                                  124, __FILE__, ((long)__LINE__)-1,
                                                  "my_argv[my_argc]", my_argc);
            strcpy(my_argv[my_argc++], *argv);
          }
       }
      argc--;
    }
   /*
      Building of private `my_argv[]' table is done, so check first whether
        warning/debug option (--debug[=0...WARN_LVL_MAX]) is given (needed to
        set correct warning level in case global date variable definitions or
        operations follow) and then check whether global date variable
        definitions/operations -v<> are given, so we can reference them
        in the actual date modifier, e.g., %`yyyy'@`dvar'[[-]<n>[`ww[w]']].
   */
   if (my_argc > 1)
    {
      register int  j;


      /*
         Now it's time to rearrange the `my_argv[]' table, so all short-style
           options with needed argument like `-X foo' are concatenated to `-Xfoo'.
      */
      rearrange_argv (these_short_opts_need_args, &my_argc, my_argv);
      i = 1;
      while (i < my_argc)
       {
         ptr_char = my_argv[i];
         if (   *ptr_char == *SWITCH
             || *ptr_char == *SWITCH2)
          {
            ptr_char++;
            if (*ptr_char == *SWITCH)
             {
               /*
                  Long-style option given:
                    Convert it to an according short-style option.
               */
               ptr_char++;
               /*
                  Copy this long-style option into a buffer.
               */
               j = (int)strlen(ptr_char);
               if ((Uint)j >= maxlen_max)
                 resize_all_strings (j+1, FALSE, __FILE__, (long)__LINE__);
               strcpy(s3, ptr_char);
               /*
                  Check this long-style option.
               */
               if (!eval_longopt (s3, &lopt_id))
                 ptr_char = s3;
               else
                 /*
                    Error in given long-style option detected:
                      Skip this long-style option only!
                 */
                 ptr_char = s3 + strlen(s3);
             }
            else
              lopt_id = SYM_NIL;
            if (lopt_id == SYM_DEBUG)
               warning_level = my_atoi (ptr_char);
#if USE_RC
            else
              if (*ptr_char == 'r')
               {
                 ptr_char++;
                 /*
                    Global text variable definition found (e.g. -r$a=TEXT1:$b=TEXT2:$c=$a...):
                      So try to scan this argument and store found TEXT's into
                      the text variable table.
                 */
                 while (*ptr_char)
                  {
                    /*
                       Split the SEP (colon) separated list of text variables.
                    */
                    j = 0;
                    while (   *ptr_char
                           && (*ptr_char != *SEP))
                     {
                       if ((Uint)j >= maxlen_max)
                         resize_all_strings (maxlen_max<<1, FALSE, __FILE__, (long)__LINE__);
                       s1[j++] = *ptr_char++;
                       /*
                          Take a quoted SEP (colon) separator as is,
                            but remove the quote character.
                       */
                       if (   (*ptr_char == *SEP)
                           && (s1[j-1] == QUOTE_CHAR))
                         s1[j-1] = *ptr_char++;
                     }
                    s1[j] = '\0';
                    if (strchr(s1, PSEUDO_BLANK) != (char *)NULL)
                     {
                       /*
                          Manage quoted or unquoted PSEUDO_BLANK characters.
                       */
                       ptr_optarg=ptr_char = s1;
                       while (*ptr_char)
                        {
                          if (*ptr_char == PSEUDO_BLANK)
                           {
                             ptr_char++;
                             *ptr_optarg++ = ' ';
                           }
                          else
                            if (*ptr_char == QUOTE_CHAR)
                             {
                               ptr_char++;
                               if (*ptr_char == PSEUDO_BLANK)
                                 *ptr_optarg++ = *ptr_char++;
                               else
                                 *ptr_optarg++ = QUOTE_CHAR;
                             }
                            else
                              *ptr_optarg++ = *ptr_char++;
                        }
                       *ptr_optarg = '\0';
                     }
                    if (!set_tvar (s1, GLobal))
                      /*
                         Error, unable to store text variable.
                      */
                      my_error (105, "", 0L, s1, 0);
                    if (*ptr_char)
                      ptr_char++;
                  }
               }
              else
                if (*ptr_char == 'v')
                 {
                   ptr_char++;
                   /*
                      Global text variable definition found (e.g. -va=1227:b=0514:c=a...):
                        So try to scan this argument and store found `mmdd' dates into
                        the date variable table.
                   */
                   while (*ptr_char)
                    {
                      /*
                         Split the SEP (colon) separated list of date variables.
                      */
                      j = 0;
                      while (   *ptr_char
                             && (*ptr_char != *SEP))
                       {
                         if ((Uint)j >= maxlen_max)
                           resize_all_strings (maxlen_max<<1, FALSE, __FILE__, (long)__LINE__);
                         s1[j++] = *ptr_char++;
                       }
                      s1[j] = '\0';
#  if USE_DE
                      if (!set_dvar (s1, lineptrs, INTERNAL_TXT, 0L, GLobal))
#  else /* !USE_DE */
                      if (!set_dvar (s1, lineptrs, _("Internal"), 0L, GLobal))
#  endif /* !USE_DE */
                        /*
                           Error, unable to store date variable.
                        */
                        my_error (114, "", 0L, s1, 0);
                      if (*ptr_char)
                        ptr_char++;
                    }
                 }
#endif /* USE_RC */
          }
         /*
            Stop processing if the first command is found.
         */
         if (   (*my_argv[i] != *SWITCH)
             && (*my_argv[i] != *SWITCH2)
#if USE_RC
             && (*my_argv[i] != RC_ADATE_CHAR)
#endif
             && (*my_argv[i] != RSP_CHAR))
           break;
         i++;
       }
    }
#if USE_RC
   /*
      Check whether an actual date modifier %... is given.
   */
   if (my_argc > 1)
    {
      i = 1;
      while (i < my_argc)
       {
         if (*my_argv[i] == RC_ADATE_CHAR)
           rc_adate = my_argv[i] + 1;
         /*
            Stop processing if the first command is found.
         */
         if (   (*my_argv[i] != *SWITCH)
             && (*my_argv[i] != *SWITCH2)
             && (*my_argv[i] != RC_ADATE_CHAR))
           break;
         i++;
       }
    }
   /*
      Read the internal system date.
   */
   if (!get_actual_date ())
    {
      /*
         Error, invalid actual date modifier %... given.
      */
#  if USE_DE
      fprintf(stderr, "%s: ung%sltiges Datum angegeben -- %c%s\n%s\n",
              prgr_name, UE, RC_ADATE_CHAR, rc_adate, usage_msg ());
#  else /* !USE_DE */
      fprintf(stderr, _("%s: invalid date given -- %c%s\n%s\n"),
              prgr_name, RC_ADATE_CHAR, rc_adate, usage_msg ());
#  endif /* !USE_DE */
      put_longopt_description (stderr);
      S_NEWLINE(stderr);
      exit(126);
    }
#else /* !USE_RC */
   (void)get_actual_date ();
#endif /* !USE_RC */
   is_leap_year = (days_of_february (act_year) == 29);
#ifdef GCAL_EPAGER
   if (is_tty)
    {
      /*
         Try to detect the external pager program in a safe way.  This means first
           detect whether a $PAGER environment variable is set, if so, try to use this pager
           program, otherwise try to use one of the burned-in pager names of `ext_pager[]' table.
      */
      ptr_char = getenv(ENV_VAR_PAGER);
      if (ptr_char != (char *)NULL)
       {
         if (*ptr_char)
          {
            while (isspace(*ptr_char))
              ptr_char++;
            i = (int)strlen(ptr_char);
            if ((Uint)i >= maxlen_max)
              resize_all_strings (i+1, FALSE, __FILE__, (long)__LINE__);
            strcpy(s3, ptr_char);
            if (*ptr_char != *DIR_SEP)
             {
               ptr_char = strrchr(s3, *DIR_SEP);
               if (ptr_char != (char *)NULL)
                 ptr_char++;
               else
                 ptr_char = s3;
             }
            ext_pager = (char *)my_malloc (strlen(ptr_char)+1,
                                           124, __FILE__, ((long)__LINE__)-1,
                                           "ext_pager", 0);
            strcpy(ext_pager, ptr_char);
            i = 0;
            while (   ext_pager[i]
                   && !isspace(ext_pager[i]))
              i++;
            ext_pager[i] = '\0';
            if (*ext_pager == *DIR_SEP)
             {
               /*
                  Absolute pager name given, try to get this executable directly.
               */
#  if HAVE_SYS_STAT_H && defined(S_IFMT) && defined(S_IFREG)
               auto struct stat  statbuf;


               /*
                  Test if the file is a regular file, if not, ignore it!
               */
               if (!stat(ext_pager, &statbuf))
                 if ((statbuf.st_mode & S_IFMT) == S_IFREG)
                   fp = fopen(ext_pager, "r");
#  else  /* !HAVE_SYS_STAT_H || !S_IFMT || !S_IFREG */
               fp = fopen(ext_pager, "r");
#  endif  /* !HAVE_SYS_STAT_H || !S_IFMT || !S_IFREG */
             }
            else
              /*
                 Relative pager name given, search this executable
                   using the $PATH environment variable.
              */
              fp = file_open (&ext_pager, 0, COmmon, &b_dummy);
            if (fp == (FILE *)NULL)
             {
               free(ext_pager);
               ext_pager = (char *)NULL;
             }
            else
              fclose(fp);
          }
       }
      if (ext_pager == (char *)NULL)
       {
         i = 0;
         while (pagers[i] != (char *)NULL)
          {
            ext_pager = (char *)my_malloc (strlen(pagers[i])+1,
                                           124, __FILE__, ((long)__LINE__)-1,
                                           "ext_pager", 0);
            strcpy(ext_pager, pagers[i]);
            fp = file_open (&ext_pager, 0, COmmon, &b_dummy);
            if (fp != (FILE *)NULL)
             {
               fclose(fp);
               break;
             }
            free(ext_pager);
            ext_pager = (char *)NULL;
            i++;
          }
       }
      /*
         If we have found an external pager program, we'll use it!
      */
      if (ext_pager != (char *)NULL)
       {
         /*
            We work unbuffered in this case so we have to flush the stdout buffer first.
         */
         fflush(stdout);
         /*
            Open an unnamed pipe.
         */
         if (pipe(pipe_fd) < 0)
           /*
              Error, the `pipe()' function have failed.
           */
           my_error (110, __FILE__, ((long)__LINE__)-4, "pipe()<", 0);
#  if HAVE_SIGNAL && defined(SIGPIPE)
         /*
            Ignore the SIGPIPE signal.
         */
         (void)signal(SIGPIPE, SIG_IGN);
#  endif
       }
    }
#endif /* GCAL_EPAGER */
   /*
      Check and evaluate the command line arguments and detect,
        whether a list/range of extended years or other lists/ranges of
        months/years are wanted.
   */
   check_command_line (my_argc, my_argv);
   /*
      If an invalid date format is found, exit the program with error message!
   */
   if (errtxt_dformat != (char *)NULL)
     my_error (106, errtxt_dformat, 0, date_format->format, 0);
#ifdef GCAL_EMAIL
   /*
      In case Gcal's output must be send to a user via eMail:
        Create and open a temporary file.
   */
   if (email_adr != (char *)NULL)
    {
      tfn = tmpnam(NULL);
      if (tfn == (char *)NULL)
        my_error (111, __FILE__, ((long)__LINE__)-2, email_adr, 0);
      tfp = fopen(tfn, "w");
      if (tfp == (FILE *)NULL)
        my_error (111, __FILE__, ((long)__LINE__)-2, email_adr, 0);
      is_tty = (int)FALSE;
/*
#  if USE_DE
      fprintf(tfp, "Betreff: Post von \\`%s' (%02d-%s-%04d %02d%s%02d%s%02d",
              prgr_name, buf_ad, short_month_name (buf_am), buf_ad,
              act_hour, TIME_SEP, act_min, TIME_SEP, act_sec);
#  else
      fprintf(tfp, _("Subject: Mail from \\`%s' (%02d-%s-%04d %02d%s%02d%s%02d"),
              prgr_name, buf_ad, short_month_name (buf_am), buf_ay,
              act_hour, _(":"), act_min, _(":"), act_sec);
#  endif
      if (tz != (char *)NULL)
        fprintf(tfp, " %s", tz);
      fputs(")\n\n", tfp);
*/
    }
#endif /* GCAL_EMAIL */
   /*
      Check whether month calendar output is wanted
        in case no explicit date is given in command line.
   */
   if (switch_back_flag)
     special_calsheet_flag = (Bool)!special_calsheet_flag;
   /*
      Do we have to suppress the output of a month calendar in case only
        the -n|N[-] option (display eternal holiday list) is found and/or
        only the -c[] option (display fixed dates list) is found?
   */
   if (   !year
       && !month
       && (   holiday_flag
#if USE_RC
           || rc_use_flag
#endif
          )
#if USE_DE
       && !special_calsheet_flag
#else /* !USE_DE */
#  ifdef GCAL_NLS
       && (   (   is_en
               && special_calsheet_flag)
           || (   !is_en
               && !special_calsheet_flag))
#  else /* !GCAL_NLS */
       && special_calsheet_flag
#  endif /* !GCAL_NLS */
#endif /* !USE_DE */
       && (   !start_day
           || (   (start_day < DAY_MIN)
#if USE_RC
               && rc_use_flag
#endif
          ))
       && !switch_back_flag
       && !year_flag)
     show_calendar = FALSE;
   else
     /*
        It's sure that we have to suppress a month/year calendar sheet,
          because the `-u' option is given in the command line!
          If NO eternal holiday list option (-n|N[-]) AND NO fixed dates
          list option (-c|C[]) is given, we have to avoid the call
          of the calendar sheet creating function `print_calendar()';
          this case is marked by setting `show_calendar' to FALSE!
     */
     if (   suppr_cal_flag
#if USE_RC
         && !rc_use_flag
#endif
         && !holiday_flag)
       show_calendar = FALSE;
#if USE_RC
   /*
      Check whether an explicit date is given in the command line.
   */
   is_date_given = (Bool)(   month
                          || year
                          || is_fiscal_year);
   if (   rc_use_flag
       && (   (   !month
               && year)
           || is_fiscal_year))
     date_enables_year = TRUE;
   /*
      Manage quoted or unquoted PSEUDO_BLANK characters of some option arguments.
   */
   if (rc_filename != (char *)NULL)
     if (strchr(rc_filename, PSEUDO_BLANK) != (char *)NULL)
      {
        ptr_optarg=ptr_char = rc_filename;
        while (*ptr_char)
         {
           if (*ptr_char == PSEUDO_BLANK)
            {
              ptr_char++;
              *ptr_optarg++ = ' ';
            }
           else
             if (*ptr_char == QUOTE_CHAR)
              {
                ptr_char++;
                if (*ptr_char == PSEUDO_BLANK)
                  *ptr_optarg++ = *ptr_char++;
                else
                  *ptr_optarg++ = QUOTE_CHAR;
              }
             else
               *ptr_optarg++ = *ptr_char++;
         }
        *ptr_optarg = '\0';
      }
   if (rc_filter_text != (char *)NULL)
     if (strchr(rc_filter_text, PSEUDO_BLANK) != (char *)NULL)
      {
        ptr_optarg=ptr_char = rc_filter_text;
        while (*ptr_char)
         {
           if (*ptr_char == PSEUDO_BLANK)
            {
              ptr_char++;
              *ptr_optarg++ = ' ';
            }
           else
             if (*ptr_char == QUOTE_CHAR)
              {
                ptr_char++;
                if (*ptr_char == PSEUDO_BLANK)
                  *ptr_optarg++ = *ptr_char++;
                else
                  *ptr_optarg++ = QUOTE_CHAR;
              }
             else
               *ptr_optarg++ = *ptr_char++;
         }
        *ptr_optarg = '\0';
      }
#endif /* USE_RC */
   /*
      Check whether the arguments of command line are valid.
   */
   eliminate_invalid_data ();
   /*
      Get the highlighting sequences of the terminal.
   */
   get_tty_hls (hl_seq);
#if USE_PAGER
   /*
      No program output yet, so if the paging option is wanted:
        Set the standard stream buffers to unbuffered mode
        and get actual settings of the tty.
   */
   if (   is_tty1
       && is_tty2
#  ifdef GCAL_EPAGER
       && (ext_pager == (char *)NULL)
#  endif
       && pager_flag)
    {
      setbuf(stdout, (char *)NULL);
      setbuf(stdin, (char *)NULL);
      get_tty_scr_size (&tty_rows, &tty_cols);
    }
#endif /* USE_PAGER */
#if USE_RC
   /*
      If any `-# ARG' resp., `--here=ARG' options are found:
        Close temporary file which is used for managing all "here" options
          (it contains all "here" lines now) so it can be used at the end
          of each resource file processing step/cycle.
   */
   if (rc_here_fn != (char *)NULL)
     if (fclose(rc_here_fp) == EOF)
       my_error (115, __FILE__, ((long)__LINE__)-1, rc_here_fn, 0);
#endif
   /*
      Log contents of command line:
        I.e. check whether a response file must be written.
   */
#if USE_DE
   if (rsp_filename != (char *)NULL)
     write_log_file (rsp_filename, REsponse, RESPONSE_TXT, CREATED_TXT, my_argc, my_argv);
#else /* !USE_DE */
   if (rsp_filename != (char *)NULL)
     write_log_file (rsp_filename, REsponse, _("response file"), _("Created"), my_argc, my_argv);
#endif /* !USE_DE */
#ifdef GCAL_SHELL
   /*
      Log contents of command line:
        i.e. check whether a shell script must be written
   */
#  if USE_DE
   if (shl_filename != (char *)NULL)
     write_log_file (shl_filename, SCript, SCRIPT_TXT, CREATED_TXT, my_argc, my_argv);
#  else /* !USE_DE */
   if (shl_filename != (char *)NULL)
     write_log_file (shl_filename, SCript, _("shell script"), _("Created"), my_argc, my_argv);
#  endif /* !USE_DE */
#endif /* GCAL_SHELL */
#ifdef GCAL_EPAGER
   if (   is_tty1
       && is_tty2
       && pager_flag
       && (ext_pager != (char *)NULL)
       && (email_adr == (char *)NULL))
    {
      child_pid = fork();
      switch (child_pid)
       {
         case -1:
           /*
              Error, the `fork()' function have failed.
           */
           my_error (110, __FILE__, ((long)__LINE__)-7, "fork() child_pid=", child_pid);
         case 0:
           /*
              Child process (read from pipe):
                Connect PIPE-stdin to SYS-stdin in a safe way
                and launch the external pager program.
           */
           close(pipe_fd[1]);
           if (pipe_fd[0])
            {
              close(0);
              dup(pipe_fd[0]);
              close(pipe_fd[0]);
            }
           i = execlp(ext_pager, ext_pager, (char *)NULL);
           /*
              Error, the `execlp()' function have failed (this line should never be executed)!
           */
           sprintf(s1, "execlp(%s)=", ext_pager);
           my_error (110, __FILE__, ((long)__LINE__)-5, s1, i);
         default:
           /*
              Parent process (write to pipe):
                Connect PIPE-stdout to SYS-stdout in a safe way
                and initiate the action.
           */
           close(pipe_fd[0]);
           if (pipe_fd[1] != 1)
            {
              close(1);
              if (!dup(pipe_fd[1]))
               {
                 dup(pipe_fd[1]);
                 close(0);
               }
              close(pipe_fd[1]);
            }
       }
    }
#endif /* GCAL_EPAGER */
   /*
      And now display the calendar sheet(s)!
   */
   if (show_calendar)
     print_calendar ();
   else
     /*
        Only the -n|N[-] option (display eternal holiday list) is found and/or
        only the -c[] option (display fixed dates) is found, display
        these list(s) without/omitting a leading month calendar.
     */
     if (   holiday_flag
#if USE_RC
         || rc_use_flag
#endif
        )
      {
        register int  tmp_ad=act_day;


        is_leap_year = (days_of_february (year) == 29);
        if (is_fiscal_year)
          fiscal_month = (*month_list).month;
        if (cal_special_flag)
          act_day = day_of_year (tmp_ad, act_month, act_year);
        if (   (   (year == EASTER_MIN-1)
                && (fiscal_month > MONTH_MIN))
            || (   (year >= EASTER_MIN)
                && (year <= EASTER_MAX)))
          print_all_holidays (FALSE, TRUE);
        if (cal_special_flag)
          act_day = tmp_ad;
      }
   /*
      If the simple month/year mode is active...
   */
   if (!month_list[1].month)
    {
      if (   is_fiscal_year
          || (   holiday_flag
              && !show_calendar))
        month = 0;
#if USE_RC
      if (rc_use_flag)
        rc_use ();
#endif
      if (holiday_flag)
       {
         if (   (   (year == EASTER_MIN-1)
                 && (fiscal_month == 1))
             || year < EASTER_MIN-1
             || year > EASTER_MAX)
           /*
              Error, invalid year given for computing Easter Sunday's date.
           */
           my_error (125, "", 0L, "", 0);
         print_all_holidays (FALSE, FALSE);
       }
    }
#ifdef GCAL_EPAGER
   if (   is_tty1
       && is_tty2
       && pager_flag
       && (ext_pager != (char *)NULL)
       && (email_adr == (char *)NULL))
    {
      /*
         We work unbuffered in this case so we have to flush the stdout buffer
           for showing all contents of it.
      */
      fflush(stdout);
      /*
         And reconnect the SYS-stdin/SYS-stdout file descriptors.
      */
      close(0);
      dup(sys_fd[0]);
      close(1);
      dup(sys_fd[1]);
      /*
         And wait until the child has performed all action.
      */
      while (wait((int *)NULL) != child_pid)
        ;
#  if HAVE_SIGNAL && defined(SIGPIPE)
      /*
         Reset the SIGPIPE signal.
      */
      (void)signal(SIGPIPE, SIG_DFL);
#  endif
    }
#endif /* GCAL_EPAGER */
#ifdef GCAL_EMAIL
   /*
      If the mailing option is selected:
        Close temporary file, redirect it to mailer and then erase it.
   */
   if (email_adr != (char *)NULL)
    {
      if (fclose(tfp) == EOF)
        my_error (111, __FILE__, ((long)__LINE__)-1, email_adr, 0);
#  if USE_DE
      sprintf(s2, "Post von \\`%s' (%02d-%s-%04d %02d%s%02d%s%02d",
              prgr_name, buf_ad, short_month_name (buf_am), buf_ay,
              act_hour, TIME_SEP, act_min, TIME_SEP, act_sec);
#  else /* !USE_DE */
      sprintf(s2, _("Mail from \\`%s' (%02d-%s-%04d %02d%s%02d%s%02d"),
              prgr_name, buf_ad, short_month_name (buf_am), buf_ay,
              act_hour, _(":"), act_min, _(":"), act_sec);
#  endif /* !USE_DE */
      if (tz != (char *)NULL)
       {
         sprintf(s1, " %s", tz);
         strcat(s2, s1);
       }
      strcat(s2, ")");
      ptr_char = (char *)NULL;
#    if !defined(AMIGA) || defined(__GNUC__)
      /*
         Detect whether a $MAILPROG environment variable is set.
      */
      ptr_char = getenv(ENV_VAR_MAILPROG);
      if (ptr_char != (char *)NULL)
        if (!*ptr_char)
          ptr_char = (char *)NULL;
#    endif /* !AMIGA || __GNUC__ */
      if (ptr_char == (char *)NULL)
        ptr_char = MAIL_PRGR;
      i = (int)strlen(ptr_char) + strlen(s2) + strlen(email_adr) + strlen(REDIRECT) + 9;
      if ((Uint)i >= maxlen_max)
        resize_all_strings (i+1, FALSE, __FILE__, (long)__LINE__);
      sprintf(s1, "%s -s \"%s\" %s %s %s", ptr_char, s2, email_adr, REDIRECT, tfn);
      if (system(s1))
       {
         /*
            If the "shell" reports a MAIL exit status not equal 0, first try
              to erase the temporary file (if this results to an error, this
              error has a higher preference than pointing out only that the
              mail program has failed, see the next comment to this a few lines
              below), and only then, terminate Gcal with error message.
         */
         i = unlink(tfn);
         if (i)
           my_error (110, __FILE__, ((long)__LINE__)-2, "unlink(tfn)=", i);
         my_error (111, __FILE__, ((long)__LINE__)-12, email_adr, 0);
       }
      i = unlink(tfn);
      if (i)
        /*
           Don't ignore any errors, because NOT being able to erase a non-write
             protected regular file we have created just some [milli]seconds before
             is a serious error that DOES produce trouble in future, because this
             is a detection of a bad block of the storage media in most cases!
        */
        my_error (110, __FILE__, ((long)__LINE__)-8, "unlink(tfn)=", i);
    }
#endif
#if USE_RC
   /*
      Erase temporary file which is used for managing all `-# ARG' resp.,
        `--here=ARG' options if all processing is done.
   */
   if (rc_here_fn != (char *)NULL)
    {
      i = unlink(rc_here_fn);
      if (i)
        /*
           Don't ignore any errors, because NOT being able to erase a non-write
             protected regular file we have created some time before is a serious
             error that DOES produce trouble in future, because this is a detection
             of a bad block of the storage media in most cases!
        */
        my_error (110, __FILE__, ((long)__LINE__)-8, "unlink(rc_here_fn)=", i);
    }
   if (   rc_use_flag
       && !shell_escape_done
       && (   !rc_elems
           || (   rc_zero_dates_flag
               && !(rc_elems-rc_zero_pos))))
     return(1);
#endif

   return(0);
}



   PUBLIC int
eval_longopt (longopt, longopt_symbolic)
   char *longopt;
   int  *longopt_symbolic;
/*
   Evaluates a long option and returns -2...0 if success, +1...+6 if fails:
     -2 == Given `longopt' successfully parsed and completed `long_name' with argument returned.
     -1 == Given `longopt' successfully parsed and completed `long_name' returned only.
      0 == Given `longopt' successfully parsed and corresponding `short_name' returned.
     +1 == Given `longopt' is ambiguous (not distinguishable).
     +2 == Given `longopt' is unknown.
     +3 == Given `longopt' requires no argument.
     +4 == Given `longopt' requires an argument.
     +5 == Given `longopt' with invalid argument.
     +6 == Given `longopt' with ambiguous argument.
   This function `abort()'s internally if no SPECIAL longoption argument
     is defined in the `lopt[]' table for a LARG_ONE_OR_ARG mode, but one
     or some `short_name' entries are in it!  Such a case is illegal!
   If success and `short_name[0]' isn't set to NULL, the delivered `longopt'
     is converted to an according `short_name' (UN*X-style) option and returned,
     otherwise `longopt' is returned unmodified.
   If success and `short_name[0]' is set to NULL, return the completed
     `long_name' (with possibly trailing arguments) instead.
   `longopt_symbolic' is either set to SYM_NIL if given `longopt' isn't member
     of field `long_name' in `lopt[]' table or to the according SYM_???.
*/
{
   auto const Lopt_struct  *ptr_lopt=lopt;


   *longopt_symbolic = SYM_NIL;
   if (tolower(*longopt) >= tolower(*ptr_lopt->long_name))
    {
      /*
         Check whether first character of `longopt' can be found
           in `lopt[]' tables structure member `long_name'.
      */
      while (tolower(*ptr_lopt->long_name) < tolower(*longopt))
       {
         ptr_lopt++;
         if (ptr_lopt->long_name == NULL)
           break;
       }
      if (ptr_lopt->long_name != NULL)
       {
         if (tolower(*ptr_lopt->long_name) == tolower(*longopt))
          {
            auto     const Lopt_struct  *ptr2_lopt=ptr_lopt+1;
            register       int           len_longopt=(int)strlen(longopt);
            auto           Bool          larg_sep_found;
            auto           Bool          ok=FALSE;


            if (strchr(longopt, *LARG_SEP) != NULL)
              len_longopt -= (int)strlen(strchr(longopt, *LARG_SEP));
            /*
               First character of `longopt' is found in `lopt[]' tables
                 structure member `long_name'; now check for the complete
                 long name or a significant abbreviation of it.
            */
            while (   !ok
                   && (ptr_lopt->long_name != NULL))
             {
               if (ptr2_lopt->long_name != NULL)
                 if (   !strncasecmp(ptr_lopt->long_name, longopt, len_longopt)
                     && !strncasecmp(ptr2_lopt->long_name, longopt, len_longopt))
                   /*
                      Error, `longopt' is ambiguous (not distinguishable).
                   */
                   return(1);
               if (strncasecmp(ptr_lopt->long_name, longopt, len_longopt))
                {
                  ptr_lopt++;
                  if (ptr_lopt->long_name != NULL)
                    ptr2_lopt++;
                  if (   (ptr_lopt->long_name != NULL)
                      && (ptr2_lopt->long_name == NULL))
                    if (!strncasecmp(ptr_lopt->long_name, longopt, len_longopt))
                      ok = TRUE;
                }
               else
                 ok = TRUE;
             }
            if (!ok)
              /*
                 Error, `longopt' not found (contains a spelling mistake).
              */
              return(2);
            larg_sep_found = (Bool)(((int)strlen(longopt)-len_longopt) ? TRUE : FALSE);
            if (   larg_sep_found
                && (ptr_lopt->larg_mode == LARG_NO))
             {
               *longopt_symbolic = ptr_lopt->symbolic_name;
               /*
                  Error, `longopt' requires NO argument.
               */
               return(3);
             }
            if (   !larg_sep_found
                && (ptr_lopt->larg_mode > LARG_NO_OR_ONE))
             {
               *longopt_symbolic = ptr_lopt->symbolic_name;
               /*
                  Error, `longopt' requires AN argument.
               */
               return(4);
             }
            /*
               Valid `longopt' found.
            */
            if (   ptr_lopt->larg_mode == LARG_NO
                || (   (ptr_lopt->larg_mode == LARG_NO_OR_ONE)
                    && !larg_sep_found))
             {
               /*
                  `longopt' requires NO argument (e.g. foo);
                    return the FIRST `short_name' of `lopt[]' table if its
                    not set to NULL, otherwise return the completed `long_name'.
               */
               if (ptr_lopt->short_name[0] == NULL)
                {
                  strcpy(longopt, ptr_lopt->long_name);
                  *longopt_symbolic = ptr_lopt->symbolic_name;
                  return(-1);
                }
               strcpy(longopt, ptr_lopt->short_name[0]);
             }
            else
             {
               auto char  *ptr_char=longopt+len_longopt+1;
               auto char  *larg_longopt;


               /*
                  `longopt' must have ONE trailing argument (e.g. foo=BAR);
                    return the CORRESPONDING `short_name' stored in `lopt[]' table.
               */
               if (!*ptr_char)
                {
                  *longopt_symbolic = ptr_lopt->symbolic_name;
                  /*
                     Error, `longopt' isn't trailed by an argument.
                  */
                  return(4);
                }
               larg_longopt = (char *)my_malloc (strlen(longopt)-len_longopt,
                                                 124, __FILE__, ((long)__LINE__)-1,
                                                 "larg_longopt", 0);
               strcpy(larg_longopt, ptr_char);
               if (ptr_lopt->largs[0] == NULL)
                {
                  /*
                     Process GIVEN argument.
                  */
                  if (ptr_lopt->short_name[0] == NULL)
                   {
                     /*
                        Return the completed `long_name' with GIVEN argument;
                          the format of returned `longopt' is like:  foo=ARG
                     */
                     strcpy(longopt, ptr_lopt->long_name);
                     strcat(longopt, LARG_SEP);
                     strcat(longopt, larg_longopt);
                     *longopt_symbolic = ptr_lopt->symbolic_name;
                     return(-2);
                   }
                  if (ptr_lopt->larg_mode == LARG_ONE_OR_ARG)
                    /*
                       Internal error, such a mode MUST HAVE one
                         SPECIAL longoption argument minimum in the table!
                    */
                    abort();
                  /*
                     Return the corresponding `short_name' with GIVEN argument;
                       the format of returned `longopt' is like:  fooARG
                  */
                  strcpy(longopt, ptr_lopt->short_name[0]);
                  strcat(longopt, larg_longopt);
                }
               else
                {
                  register int  i=0;


                  /*
                     Process SPECIAL argument given.
                  */
                  ok = FALSE;
                  i = 0;
                  ptr_char = ptr_lopt->largs[i++];
                  while (   !ok
                         && (ptr_char != NULL))
                   {
                     if (tolower(*ptr_char) == tolower(*larg_longopt))
                      {
                        register int  j=i-1;
                        register int  len_larg=(int)strlen(larg_longopt);


                        if (!strncasecmp(ptr_lopt->largs[j], larg_longopt, len_larg))
                         {
                           if (len_larg == (int)strlen(ptr_lopt->largs[j]))
                             /*
                                Exact match happened!
                             */
                             ok = TRUE;
                           else
                            {
                              while (   !ok
                                     && (ptr_lopt->largs[i] != NULL))
                               {
                                 if (   !strncasecmp(ptr_lopt->largs[i], larg_longopt, len_larg)
                                     && (len_larg == (int)strlen(ptr_lopt->largs[i])))
                                   /*
                                      Exact match happened!
                                   */
                                   ok = TRUE;
                                 i++;
                               }
                            }
                           if (!ok)
                            {
                              auto Bool  ambig=FALSE;


                              /*
                                 The given SPECIAL argument doesn't match exactly,
                                   so try to detect whether it's ambiguous.
                              */
                              i = j + 1;
                              while (   !ambig
                                     && (ptr_lopt->largs[i] != NULL))
                               {
                                 if (!strncasecmp(ptr_lopt->largs[i], larg_longopt, len_larg))
                                   ambig = TRUE;
                                 i++;
                               }
                              if (ambig)
                               {
                                 *longopt_symbolic = ptr_lopt->symbolic_name;
                                 /*
                                    Error, the given SPECIAL argument is ambiguous.
                                 */
                                 return(6);
                               }
                              else
                               {
                                 /*
                                    The given SPECIAL argument is valid!
                                 */
                                 ok = TRUE;
                                 i = j + 1;
                               }
                            }
                         }
                        else
                          ptr_char = ptr_lopt->largs[i++];
                      }
                     else
                       ptr_char = ptr_lopt->largs[i++];
                   }
                  if (   !ok
                      && (ptr_lopt->larg_mode != LARG_ONE_OR_ARG))
                   {
                     *longopt_symbolic = ptr_lopt->symbolic_name;
                     /*
                        Error, the given SPECIAL argument not found
                          in `lopt[]' tables structure member `largs'.
                     */
                     return(5);
                   }
                  i--;
                  if (ptr_lopt->short_name[0] == NULL)
                   {
                     /*
                        Return the completed `long_name' with a completed SPECIAL argument
                          or with a GIVEN argument;
                          the format of returned `longopt' is like:  foo=BAR  or  foo=ARG
                     */
                     strcpy(longopt, ptr_lopt->long_name);
                     strcat(longopt, LARG_SEP);
                     if (ptr_lopt->larg_mode == LARG_ONE_OR_ARG)
                       /*
                          Return with the GIVEN argument.
                       */
                       strcat(longopt, larg_longopt);
                     else
                       /*
                          Return with the completed SPECIAL argument.
                       */
                       strcat(longopt, ptr_lopt->largs[i]);
                     *longopt_symbolic = ptr_lopt->symbolic_name;
                     return(-2);
                   }
                  if (   !ok
                      && (ptr_lopt->larg_mode == LARG_ONE_OR_ARG))
                   {
                     /*
                        Return the corresponding `short_name' with a GIVEN argument;
                          the format of returned `longopt' is like:  fooBAR
                     */
                     strcpy(larg_longopt, longopt+len_longopt+1);
                     strcpy(longopt, ptr_lopt->short_name[--i]);
                     strcat(longopt, larg_longopt);
                   }
                  else
                    /*
                       Return the corresponding `short_name' with a converted SPECIAL argument;
                         the format of returned `longopt' is like:  fooBAR
                    */
                    strcpy(longopt, ptr_lopt->short_name[i]);
                }
               free(larg_longopt);
             }
          }
         else
           /*
              Error, `longopt' not member of field `long_name' in `lopt[]' table.
           */
           return(2);
       }
      else
        /*
           Error, `longopt' not member of field `long_name' in `lopt[]' table.
        */
        return(2);
    }
   else
     /*
        Error, first entry in `lopt[]' tables structure member `long_name'
          is greater than the delivered `longopt'.
     */
     return(2);
   *longopt_symbolic = ptr_lopt->symbolic_name;

   return(0);
}



   LOCAL Bool
correct_date_format (format_txt, use_day_suffix, use_short3_day_name,
                     use_day_zeroleaded, use_year_zeroleaded)
   char *format_txt;
   Bool *use_day_suffix;
   Bool *use_short3_day_name;
   Bool *use_day_zeroleaded;
   Bool *use_year_zeroleaded;
/*
   Checks whether the delivered `format_txt' contains valid format directives
     (see the `decode_date_format()' function for a more brief description of
     all used format specifiers).  Returns FALSE in case any invalid format
     specifiers are found, otherwise TRUE.  Boolean `&use_day_suffix' is set
     to TRUE and is returned in case a day GROUP format character defining the
     use of a trailing day suffix is found in `format_txt', otherwise to FALSE.
     Boolean `&use_short3_day_name' is set to TRUE and is returned in case a
     weekday GROUP format character defining the use of a day name truncated to
     three characters `www' is found in `format_txt', otherwise to FALSE.
     Boolean `&use_day_zeroleaded' is set to TRUE and is returned in case a day
     GROUP format character defining the use of a day with leading zeroes is
     found in `format_txt', otherwise to FALSE.  Boolean `&use_year_zeroleaded'
     is set to TRUE and is returned in case a year GROUP format character
     defining the use of a year with leading zeroes is found in `format_txt',
     otherwise to FALSE.
*/
{
   register int  start_highlighting=2;
   register int  end_highlighting=2;
   register int  weekday_name_group=2;
   register int  day_group=2;
   register int  month_group=2;
   register int  year_group=2;


   *use_day_suffix = *use_short3_day_name = *use_day_zeroleaded = *use_year_zeroleaded = FALSE;
   if (!*format_txt)
     /*
        Error, no `format_txt' given.
     */
     return(FALSE);
   do
    {
      while (   *format_txt
             && (*format_txt != DFORMAT_CHAR)
             && (*format_txt != QUOTE_CHAR))
        format_txt++;
      if (*format_txt)
       {
         if (*format_txt == DFORMAT_CHAR)
           /*
              Format specifier found.
           */
           switch (*++format_txt)
            {
              case '1':
                if (end_highlighting == 2)
                  if (start_highlighting)
                    start_highlighting--;
                break;
              case '2':
                if (start_highlighting == 1)
                  if (end_highlighting)
                    end_highlighting--;
                break;
              case 'u':
                *use_day_zeroleaded = TRUE;
              case 'U':
                *use_day_suffix = TRUE;
                if (day_group)
                  day_group--;
                break;
              case 'd':
                *use_day_zeroleaded = TRUE;
              case 'D':
                if (day_group)
                  day_group--;
                break;
              case 'W':
                *use_short3_day_name = TRUE;
              case 'w':
              case 'A':
                if (weekday_name_group)
                  weekday_name_group--;
                break;
              case 'b':
              case 'B':
              case 'm':
              case 'M':
                if (month_group)
                  month_group--;
                break;
              case 'y':
              case 'z':
                *use_year_zeroleaded = TRUE;
              case 'Y':
              case 'Z':
                if (year_group)
                  year_group--;
                break;
              default:
                /*
                   Error, invalid date format character specified.
                */
                return(FALSE);
            }
         else
           /*
              Quote character found.
           */
           format_txt++;
         format_txt++;
       }
      else
        break;
    } while (*format_txt);
   if (   start_highlighting != 1
       || end_highlighting != 1
       || day_group != 1
       || month_group != 1
       || year_group != 1)
     /*
        Error, either required date format element not given
          or it occurred more than once.
     */
     return(FALSE);
   if (weekday_name_group < 1)
     /*
        Error, optional format element occurred more than once.
     */
     return(FALSE);

   return(TRUE);
}



   LOCAL void
rearrange_argv (opt_list, argc, argv)
   const char *opt_list;
         int  *argc;
         char *argv[];
/*
   Rearranges `argv[]' internally.
     This means all short-style options which need an argument,
     that is separated by a whitespace character in the command line
     from the short option character, e.g. `-x foo', is concatenated
     to `-xfoo'.  The short option characters which need an argument
     are given in `*opt_list'.  This function sets given `*argc' to
     the "new" rearranged amount of arguments stored in `argv[]'.
*/
{
   register int     i=(*argc-1);
   register int     n=1;
   auto     char  **ptr_argv=argv;
   auto     char  **ptr2_argv;
   auto     Bool    is_modified;


   while (i)
    {
      /*
         Skip `argv[0]' generally!
      */
      ptr_argv++;
      is_modified = FALSE;
      if (   **ptr_argv == *SWITCH
          || **ptr_argv == *SWITCH2)
       {
         /*
            Found an option in `argv[]'
              (either a long-style option or a short-style option).
         */
         (*ptr_argv)++;
         if (   **ptr_argv
             && (**ptr_argv != *SWITCH))
          {
            /*
               Short-style option given, e.g. `-x' or `-Xfoo' or `-x bar'.
            */
            if (strchr(opt_list, **ptr_argv) != (char *)NULL)
             {
               /*
                  The option character is found in `opt_list',
                    this means this option needs an argument.
               */
               if (i > 1)
                {
                  /*
                     We are not working on the final option stored in `argv[]'.
                  */
                  (*ptr_argv)++;
                  /*
                     Check if the option doesn't have its argument already
                       unseparated by a whitespace character in the command line,
                       e.g. is not like this `-Xfoo' option.
                  */
                  if (!**ptr_argv)
                   {
                     /*
                        Check if the option is trailed by a needed argument,
                          which is the next argument in `argv[]', e.g. `-x bar'.
                     */
                     (*ptr_argv) -= 2;
                     ptr2_argv = ptr_argv;
                     ptr2_argv++;
                     /*
                        Check whether this next argument in `argv[]' is an option.
                     */
                     if (   (**ptr2_argv != RSP_CHAR)
#if USE_RC
                         && (**ptr2_argv != RC_ADATE_CHAR)
#endif
                        )
                      {
                        if (   (   **ptr2_argv == *SWITCH
                                || **ptr2_argv == *SWITCH2)
                            && (strlen(*ptr2_argv) <= 2))
                          /*
                             Next argument is definitely a simple short-style option
                               without a given modifier, e.g. `-x' or `/x'.
                          */
                          (*ptr_argv)++;
                        else
                         {
                           /*
                              Next argument is no option, so let's concatenate the
                                short-style option character and its needed argument.
                           */
                           i--;
                           argv[n] = (char *)my_realloc ((VOID_PTR)(argv[n]), strlen(*ptr_argv)+strlen(*ptr2_argv)+1,
                                                         124, __FILE__, ((long)__LINE__)-1,
                                                         "argv[n]", n);
                           strcpy(argv[n], *ptr_argv);
                           strcat(argv[n], *ptr2_argv);
                           ptr_argv++;
                           is_modified = TRUE;
                         }
                      }
                     else
                       (*ptr_argv)++;
                   }
                  else
                    (*ptr_argv)--;
                }
             }
          }
       }
      else
        (*ptr_argv)++;
      if (!is_modified)
       {
         /*
            Unmodified argument found in `argv[]'.
              This means, the argument we work on here is:
              either a short-style option, which needs an argument, which isn't given,
              or     a short-style option, which already contains its argument like `-Xfoo',
              or     a simple short-style option like `-x',
              or     a long-style option like `--foo',
              or     a command (an argument not leaded by a '-', '/', '@' or '%' character.
         */
         (*ptr_argv)--;
         argv[n] = (char *)my_realloc ((VOID_PTR)(argv[n]), strlen(*ptr_argv)+1,
                                       124, __FILE__, ((long)__LINE__)-1,
                                       "argv[n]", n);
         strcpy(argv[n], *ptr_argv);
       }
      n++;
      i--;
    }
   *argc = n;
}



   LOCAL void
check_command_line (argc, argv)
   int   argc;
   char *argv[];
/*
   Gets and manages the arguments from the command line.
*/
{
   register int    opt_error=0;
   register int    len;
   auto     int    i;
   auto     int    lopt_help;
   auto     char  *ptr_char;
   auto     char  *option=(char *)NULL;
   auto     Bool   is_longopt;
   auto     Bool   license_flag=FALSE;        /* -L */
   auto     Bool   version_flag=FALSE;        /* -V */
   auto     Bool   help_flag=FALSE;           /* -? | -h */
   auto     Bool   ext_help_flag=FALSE;       /* -?? | -hh */
   auto     Bool   help_on_help_flag=FALSE;   /* --long-help=? */
   auto     Bool   lopt_ambig=FALSE;
   auto     Bool   skip_option;
#if USE_RC
   auto     Bool   further_check;
#endif


   /*
      Work on the arguments that have been found in the command line.
   */
   while (argc > 1)
    {
      option = *++argv;
      /*
         If a leading switch character is found, check the command line for options.
      */
      if (   *option == *SWITCH
          || *option == *SWITCH2)
       {
         is_longopt = FALSE;
         /*
            Check the command line for options.
         */
         for (option++ ; *option ; option++)
          {
            skip_option = FALSE;
#if USE_RC
            further_check = FALSE;
#endif
            /*
               Check for long-style options, e.g. --help ...
            */
            if (*option == *SWITCH)
             {
               option--;
               if (*option == *SWITCH)
                {
                  is_longopt = TRUE;
                  option += 2;
                }
               if (   *option
                   && is_longopt)
                {
                  /*
                     Copy this long-style option into a buffer.
                  */
                  len = (int)strlen(option);
                  if ((Uint)len >= maxlen_max)
                    resize_all_strings (len+1, FALSE, __FILE__, (long)__LINE__);
                  strcpy(s2, option);
                  /*
                     Check this long-style option.
                  */
                  opt_error = eval_longopt (s2, &lopt_id);
                  if (opt_error < 1)
                   {
                     opt_error = 0;
                     switch (lopt_id)
                      {
#if USE_RC
                        case SYM_EXPORT_LOCAL_DVARS:
                          rc_export_ldvar_flag = TRUE;
                          break;
                        case SYM_EXPORT_LOCAL_TVARS:
                          rc_export_ltvar_flag = TRUE;
                          break;
                        case SYM_LEAP_DAY:
                          rc_feb_29_to_feb_28=rc_feb_29_to_mar_01 = FALSE;
                          /*
                             Hmmm... I think it's enough to check the first
                               character of the option argument...
                               (I know users could specify `f*ck' or other NICE arguments
                                but it doesn't bother/matter me in any case...)
                          */
                          if (tolower(*s2) == 'f')
                            rc_feb_29_to_feb_28 = TRUE;
                          else
                            rc_feb_29_to_mar_01 = TRUE;
                          break;
#endif
                        case SYM_DEBUG:
                          break;   /* Void, --debug[=0...WARN_LVL_MAX] is already managed in main() */
                        case SYM_EXIT_STAT_HELP_127:
                          exit_stat_help = 127;
                          break;
                        /*
                           The holidays.
                        */
                        case SYM_STANDARD_HDY:
                          standard_hdy = FALSE;
                          break;
                        case SYM_CHRISTIAN_HDY:
                          christian_hdy = TRUE;
                          break;
                        default:
                          option = s2;
                          /*
                             Now we have decoded a long-style option into the according
                             short-style option, let's JUMP to that part where the short
                             options are decoded/evaluated/processed (to JUMP isn't best
                             coding style, I know [Hi Mr.Dijkstra], but it's the easiest
                             now to do so  =8^)
                          */
                          goto LABEL_short_option;
                      }
                   }
                }
               else
                 /*
                    Hmm, seems that a just processed short-style option having
                      permitted modifiers has an invalid switch character within
                      its modifiers.
                 */
                 opt_error = 5;
               skip_option = TRUE;
             }
            else
              /*
                 Check for short options...
              */
LABEL_short_option:
              switch (*option)
               {
                 case '?':
                 case 'h':
                   help_flag = TRUE;
                   option++;
                   if (*option)
                    {
                      if (   *option == '?'
                          || *option == 'h')
                       {
                         ext_help_flag = TRUE;
                         lopt_help = SYM_NIL;
                         option++;
                         if (   *option
                             && is_longopt)
                          {
                            /*
                               Copy the long-style option argument into a buffer.
                            */
                            len = (int)strlen(option);
                            if ((Uint)len >= maxlen_max)
                              resize_all_strings (len+1, FALSE, __FILE__, (long)__LINE__);
                            strcpy(s3, option);
                            /*
                               Get the reference number of the long-style option
                                 argument which is a long-style option itself.
                            */
                            lopt_ambig = (Bool)(eval_longopt (s3, &lopt_help) == 1);
                            if (lopt_help == SYM_NIL)
                              /*
                                 Error, no help related to this argument found,
                                   so we display the "help on help" screen now.
                              */
                              help_on_help_flag = TRUE;
                          }
                         else
                           if (*option)
                             /*
                                Error, invalid option modifier given.
                             */
                             opt_error = 2;
                       }
                      else
                        /*
                           Error, no semi-long option name -?? or -hh given.
                        */
                        opt_error = 2;
                    }
                   skip_option = TRUE;
                   break;
                 case 'K':
                 case 'L':
                 case 'O':
                 case 'V':
                 case 'X':
#if USE_PAGER
                 case 'p':
#endif
                 case 'u':
                   option++;
                   if (!*option)
                    {
                      option--;
                      switch (*option)
                       {
                         case 'K':
                           cal_with_weekno = TRUE;
                           break;
                         case 'L':
                           license_flag = TRUE;
                           break;
                         case 'O':
                           orthodox_calendar = TRUE;
                           break;
                         case 'V':
                           version_flag = TRUE;
                           break;
                         case 'X':
                           hd_title_flag = FALSE;
                           break;
#if USE_PAGER
                         case 'p':
                           pager_flag = TRUE;
                           break;
#endif
                         case 'u':
                           suppr_cal_flag = TRUE;
                           break;
                         default:
                           /*
                              This case MUST be an internal error!
                           */
                           abort();
                       }
                      skip_option = TRUE;
                    }
                   else
                     /*
                        Error, invalid option modifier given.
                     */
                     opt_error = 2;
                   break;
                 case 'R':
                   option++;
                   if (!*option)
                     /*
                        Error, option character is not trailed by an argument.
                     */
                     opt_error = 4;
                   else
                    {
                      if (rsp_filename == (char *)NULL)
                        rsp_filename = (char *)my_malloc (strlen(option)+1,
                                                          124, __FILE__, ((long)__LINE__)-1,
                                                          "rsp_filename", 0);
                      else
                        rsp_filename = (char *)my_realloc ((VOID_PTR)rsp_filename, strlen(option)+1,
                                                           124, __FILE__, ((long)__LINE__)-1,
                                                           "rsp_filename", 0);
                      strcpy(rsp_filename, option);
                      skip_option = TRUE;
                    }
                   break;
#ifdef GCAL_SHELL
                 case 'S':
                   option++;
                   if (!*option)
                     /*
                        Error, option character is not trailed by an argument.
                     */
                     opt_error = 4;
                   else
                    {
                      if (shl_filename == (char *)NULL)
                        shl_filename = (char *)my_malloc (strlen(option)+1,
                                                          124, __FILE__, ((long)__LINE__)-1,
                                                          "shl_filename", 0);
                      else
                        shl_filename = (char *)my_realloc ((VOID_PTR)shl_filename, strlen(option)+1,
                                                           124, __FILE__, ((long)__LINE__)-1,
                                                           "shl_filename", 0);
                      strcpy(shl_filename, option);
                      skip_option = TRUE;
                    }
                   break;
#endif
                 case 'H':
                   option++;
                   if (!*option)
                     /*
                        Error, option character is not trailed by an argument.
                     */
                     opt_error = 4;
                   else
                    {
                      len = (int)strlen(option);
                      /*
                         Check if the special argument `no' is given:
                           If only `long-option=NO' instead of both long-style
                           and short-style option; like `short-optionNO'; should
                           be valid, extend following if() statement like this:
                             if (is_longopt && ...
                      */
                      if (   (len == 2)
                          && (tolower(*option) == 'n')
                          && (tolower(*(option+1)) == 'o'))
                        highlight_flag = FALSE;
                      else
                        /*
                           Check if the special argument `yes' is given:
                             If only `long-option=YES' instead of both long-style
                             and short-style option; like `short-optionYES'; should
                             be valid, extend following if() statement like this:
                               if (is_longopt && ...
                        */
                        if (   (len == 3)
                            && (tolower(*option) == 'y')
                            && (tolower(*(option+1)) == 'e')
                            && (tolower(*(option+2)) == 's'))
                         {
                           highlight_flag = TRUE;
                           is_tty = (int)TRUE;
                         }
                        else
                         {
                           if (hl_seq == (char *)NULL)
                             hl_seq = (char *)my_malloc (strlen(option)+1,
                                                         124, __FILE__, ((long)__LINE__)-1,
                                                         "hl_seq", 0);
                           else
                             hl_seq = (char *)my_realloc ((VOID_PTR)hl_seq, strlen(option)+1,
                                                          124, __FILE__, ((long)__LINE__)-1,
                                                          "hl_seq", 0);
                           strcpy(hl_seq, option);
                           highlight_flag = TRUE;
                         }
                    }
                   skip_option = TRUE;
                   break;
                 case 'i':
                   suppr_cal_flag = FALSE;
                   option++;
#if USE_DE
                   special_calsheet_flag = TRUE;
#else /* !USE_DE */
#  ifdef GCAL_NLS
                   if (is_en)
                     special_calsheet_flag = FALSE;
                   else
                     special_calsheet_flag = TRUE;
#  else /* !GCAL_NLS */
                   special_calsheet_flag = FALSE;
#  endif /* !GCAL_NLS */
#endif /* !USE_DE */
                   if (*option)
                    {
                      if (*option == '-')
                       {
                         switch_back_flag = TRUE;
                         option++;
                         if (*option)
                           /*
                              Error, option character is trailed by invalid argument.
                           */
                           opt_error = 5;
                       }
                      else
                        /*
                           Error, option character is trailed by invalid modifier.
                        */
                        opt_error = 5;
                    }
                   else
                     switch_back_flag = FALSE;
                   option--;
                   break;
                 case 'n':
                 case 'N':
                   holiday_flag = TRUE;
                   hd_sort_des_flag = FALSE;
                   hd_legal_days_only = (Bool)((*option == 'N') ? TRUE : FALSE);
                   option++;
                   if (*option)
                    {
                      if (*option == *DES_LIT)
                        hd_sort_des_flag = TRUE;
                      else
                        /*
                           Error, option character is trailed by invalid modifier.
                        */
                        opt_error = 5;
                    }
                   skip_option = TRUE;
                   break;
                 case 'q':
                   option++;
                   if (!*option)
                     /*
                        Error, option character is not trailed by an argument.
                     */
                     opt_error = 4;
                   else
                    /*
                       Check for country code id's which are listed in the
                         `cc_holidays' table.  If more than one country code
                         is specified, it must be connected by a CONNECT_SEP.
                    */
                    {
                      auto const Cc_struct  *ptr_cc;


                      *s1 = '\0';
                      ptr_char = option;
                      while (   *ptr_char
                             && !opt_error)
                        do
                         {
                           /*
                              Copy a single, given country code into `s2'.
                           */
                           for (i=0 ; i < 2 ; i++)
                             s2[i] = (char)toupper(*ptr_char++);
                           s2[i] = '\0';
                           /*
                              Now skip a possibly trailing CONNECT_SEP.
                           */
                           if (   !*ptr_char
                               || *ptr_char == *CONNECT_SEP)
                            {
                              if (*ptr_char)
                                ptr_char++;
                            }
                           else
                            {
                              /*
                                 Error, invalid country code given.
                              */
                              opt_error = 5;
                              break;
                            }
                           ptr_cc = binsearch_cc_id (s2);
                           if (ptr_cc == (Cc_struct *)NULL)
                            {
                              /*
                                 Error, country code not found.
                              */
                              opt_error = 5;
                              break;
                            }
                           else
                             /*
                                Country code found, but only insert it into
                                  `s1' in case it is not already listed there.
                             */
                             if (strstr(s1, s2) == (char *)NULL)
                              {
                                strcat(s1, s2);
                                if (*ptr_char)
                                  strcat(s1, CONNECT_SEP);
                              }
                         } while (*ptr_char);
                      if (!opt_error)
                       {
                         if (cc == (char *)NULL)
                           cc = (char *)my_malloc (strlen(s1)+1,
                                                   124, __FILE__, ((long)__LINE__)-1,
                                                   "cc", 0);
                         else
                           cc = (char *)my_realloc ((VOID_PTR)cc, strlen(s1)+1,
                                                    124, __FILE__, ((long)__LINE__)-1,
                                                    "cc", 0);
                         strcpy(cc, s1);
                       }
                    }
                   skip_option = TRUE;
                   break;
                 case 'J':
                   if (!is_longopt)
                     /*
                        Error, invalid short-style option character given.
                     */
                     opt_error = 2;
                   else
                    {
                      /*
                         Get the argument of the --date-format=PRESET_VALUE|ARG long-style option.
                      */
                      option++;
                      if ((Uint)*option < LARG_MAX-1)
                       {
                         date_format = supported_date_format;
                         date_format += ((Uint)*option - 1);
                         if (date_format->id == (char *)NULL)
                           /*
                              Error, index of an "empty" table element referenced.
                           */
                           opt_error = 5;
                       }
                      else
                       {
                         /*
                            Respect this given argument now.
                         */
#if USE_DE
                         users_date_format.info = "Kommandozeile";
#else /* !USE_DE */
                         users_date_format.info = _("command line");
#endif /* !USE_DE */
                         if (users_date_format.format == (char *)NULL)
                           users_date_format.format = (char *)my_malloc (strlen(option)+1,
                                                                         124, __FILE__, ((long)__LINE__)-1,
                                                                         "users_date_format.format", 0);
                         else
                           users_date_format.format = (char *)my_realloc ((VOID_PTR)users_date_format.format,
                                                                          strlen(option)+1,
                                                                          124, __FILE__, ((long)__LINE__)-2,
                                                                          "users_date_format.format", 0);
                         strcpy(users_date_format.format, option);
                         date_format = &users_date_format;
                       }
                      if (!opt_error)
                       {
                         /*
                            Test if the date format given in command line is valid.
                         */
                         if (!correct_date_format (date_format->format, &use_day_suffix, &use_short3_day_name,
                                                   &use_day_zeroleaded, &use_year_zeroleaded))
#if USE_DE
                           errtxt_dformat = "Kommandozeile";
#else /* !USE_DE */
                           errtxt_dformat = _("command line");
#endif /* !USE_DE */
                         else
                           errtxt_dformat = (char *)NULL;
                       }
                      skip_option = TRUE;
                    }
                   break;
                 case 'G':
                   if (!is_longopt)
                     /*
                        Error, invalid short-style option character given.
                     */
                     opt_error = 2;
                   else
                    {
                      /*
                         Get the argument of the --gregorian-reform=PRESET_VALUE|ARG long-style option.
                      */
                      option++;
                      if ((Uint)*option < LARG_MAX-1)
                       {
                         greg = greg_reform_date;
                         greg += ((Uint)*option - 1);
                         if (!greg->year)
                           /*
                              Error, index of an "empty" table element referenced.
                           */
                           opt_error = 5;
                       }
                      else
                       {
                         auto Bool  ok=FALSE;


                         /*
                            Check whether the argument is a valid Gregorian Reformation date,
                              which must consist of the year, the month, the first day and the
                              last day (all these date elements must be numbers and separated
                              by a SPLIT_SEP) the Gregorian Reformation have occurred,
                              like:  yyyy,mm,dd,dd
                         */
                         users_greg.year=users_greg.month=users_greg.first_day=users_greg.last_day = 0;
                         ptr_char = option;
                         do
                          {
                            i = 0;
                            while (   !ok
                                   && !opt_error)
                             {
                               if (   isdigit(*ptr_char)
                                   && (i < len_year_max))
                                 s1[i++] = *ptr_char;
                               else
                                {
                                  if (isdigit(*ptr_char))
                                    /*
                                       Error, single date element too long.
                                    */
                                    opt_error = 5;
                                  else
                                    if (   i
                                        && (*ptr_char == *SPLIT_SEP))
                                     {
                                       ptr_char++;
                                       if (*ptr_char == *SPLIT_SEP)
                                         /*
                                            Error, argument contains 2 successive SPLIT_SEPs.
                                         */
                                         opt_error = 5;
                                       else
                                         /*
                                            Scanning of single date element completed.
                                         */
                                         break;
                                     }
                                    else
                                      if (   !i
                                          && *ptr_char)
                                        /*
                                           Error, either argument starts with a SPLIT_SEP
                                             or contains any invalid characters.
                                        */
                                        opt_error = 5;
                                      else
                                        /*
                                           Scanning of all date elements completed.
                                        */
                                        ok = TRUE;
                                }
                               if (*ptr_char)
                                 ptr_char++;
                             }
                            if (!opt_error)
                             {
                               s1[i] = '\0';
                               i = atoi(s1);
                               if (i)
                                {
                                  if (!users_greg.year)
                                    users_greg.year = i;
                                  else
                                    if (!users_greg.month)
                                      users_greg.month = i;
                                    else
                                      if (!users_greg.first_day)
                                        users_greg.first_day = i;
                                      else
                                        if (!users_greg.last_day)
                                          users_greg.last_day = i;
                                        else
                                          /*
                                             Error, argument contains too many date elements.
                                          */
                                          opt_error = 5;
                                }
                               else
                                 /*
                                    Error, argument contains invalid date element.
                                 */
                                 opt_error = 5;
                             }
                          } while (   !ok
                                   && !opt_error);
                         /*
                            Now we have decoded the argument,
                              so let's check if the single date elements are valid.
                         */
                         if (!opt_error)
                          {
                            if (   users_greg.month
                                && users_greg.first_day
                                && users_greg.last_day
                                && (users_greg.year <= YEAR_MAX)
                                && (users_greg.month <= MONTH_MAX)
                                && (users_greg.last_day >= users_greg.first_day)
                                && (   (   (users_greg.month == 2)
                                        && (users_greg.last_day <= ((users_greg.year & 3) ? 28 : 29)))
                                    || (   (users_greg.month != 2)
                                        && (users_greg.last_day <=  dvec[users_greg.month-1]))))
                              /*
                                 The given argument is valid,
                                   so respect this given date of Gregorian Reformation.
                              */
                              greg = &users_greg;
                            else
                              /*
                                 Error, invalid date element found.
                              */
                              opt_error = 5;
                          }
                       }
                      skip_option = TRUE;
                    }
                   break;
                 case 'j':
                   option++;
                   if (!*option)
                    {
                      cal_special_flag = TRUE;
                      cal_both_dates_flag = FALSE;
                    }
                   else
                    {
                      if (*option == 'b')
                       {
                         option++;
                         if (!*option)
                          {
                            cal_special_flag = FALSE;
                            cal_both_dates_flag = TRUE;
                          }
                         else
                           /*
                              Error, option character is trailed by an invalid modifier.
                           */
                           opt_error = 5;
                       }
                      else
                       {
                         switch (*option)
                          {
                            case 'n':
                              option++;
                              if (!*option)
                               {
                                 hd_special_flag = TRUE;
                                 hd_both_dates_flag = FALSE;
                               }
                              else
                                if (*option == 'b')
                                 {
                                   hd_special_flag = FALSE;
                                   hd_both_dates_flag = TRUE;
                                 }
                                else
                                  /*
                                     Error, option character is trailed by an invalid modifier.
                                  */
                                  opt_error = 5;
                              break;
#if USE_RC
                            case 'c':
                              option++;
                              if (!*option)
                               {
                                 rc_special_flag = TRUE;
                                 rc_both_dates_flag = FALSE;
                               }
                              else
                                if (*option == 'b')
                                 {
                                   rc_special_flag = FALSE;
                                   rc_both_dates_flag = TRUE;
                                 }
                                else
                                  /*
                                     Error, option character is trailed by an invalid modifier.
                                  */
                                  opt_error = 5;
                              break;
#endif
                            default:
                              /*
                                 Error, option character is trailed by an invalid modifier.
                              */
                              opt_error = 5;
                          }
                       }
                    }
                   if (!*option)
                     option--;
                   break;
                 case 'b':
                   year_flag = TRUE;
                   option++;
                   if (*option == '0')
                    {
                      while (*option == '0')
                        option++;
                      if (!*option)
                        option--;
                    }
                   out_rows = my_atoi (option);
                   if (   out_rows > MONTH_MAX
                       || out_rows < MONTH_MIN
                       || out_rows == 5
                       || (   (out_rows > 6)
                           && (out_rows < MONTH_MAX)))
                    {
                      if (!*option)
                        /*
                           Error, option character is not trailed by an argument.
                        */
                        opt_error = 4;
                      else
                        /*
                           Error, option character is trailed by an invalid argument.
                        */
                        opt_error = 5;
                    }
                   else
                     skip_option = TRUE;
                   suppr_cal_flag = FALSE;
                   break;
                 case 's':
                   option++;
                   if (!*option)
                     /*
                        Error, option character is not trailed by an argument.
                     */
                     opt_error = 4;
                   else
                    {
                      start_day = my_atoi (option);
                      /*
                         Check for delivered weekday name.
                      */
                      if (!start_day)
                        start_day = compare_d_m_name (option, DAy);
                      if (!start_day)
                       {
                         /*
                            Check if the 'today' text is given.
                         */
                         len = (int)strlen(option);
                         if ((Uint)len >= maxlen_max)
                           resize_all_strings (len+1, FALSE, __FILE__, (long)__LINE__);
                         strcpy(s2, option);
                         if (strlen(s2) > strlen("today"))
                           /*
                              Error, option character is trailed by an invalid argument.
                           */
                           opt_error = 5;
                         else
                           if (   isdigit(*option)
                               || !strncasecmp(s2, "today", len))
                             /*
                                Set a special value for a given `-s0' argument.
                             */
                             start_day = SPECIAL_VALUE;
                           else
                             /*
                                Error, option character is trailed by an invalid argument.
                             */
                             opt_error = 5;
                       }
                      else
                        if (   start_day > DAY_MAX
                            || start_day < DAY_MIN)
                          /*
                             Error, option character is trailed by an invalid argument.
                          */
                          opt_error = 5;
                      suppr_cal_flag = FALSE;
                    }
                   skip_option = TRUE;
                   break;
#if USE_RC
                 case '$':
                   option++;
                   if (*option == '0')
                    {
                      while (*option == '0')
                        option++;
                      if (!*option)
                        option--;
                    }
                   rc_bio_axis_len = my_atoi (option);
                   if (   rc_bio_axis_len > BIO_AXIS_MAX
                       || rc_bio_axis_len < BIO_AXIS_MIN)
                    {
                      if (!*option)
                        /*
                           Error, option character is not trailed by an argument.
                        */
                        opt_error = 4;
                      else
                        /*
                           Error, option character is trailed by an invalid argument.
                        */
                        opt_error = 5;
                    }
                   else
                    {
                      /*
                         Decrease `rc_bio_axis_len' by 1 until it divides 100 without a remainder.
                      */
                      while (100 % rc_bio_axis_len)
                        rc_bio_axis_len--;
                      skip_option = TRUE;
                    }
                   break;
                 case '&':
                   option++;
                   if (*option == '0')
                    {
                      while (*option == '0')
                        option++;
                      if (!*option)
                        option--;
                    }
                   rc_moonimage_lines = my_atoi (option);
                   if (   rc_moonimage_lines > MOONIMAGE_MAX
                       || rc_moonimage_lines < MOONIMAGE_MIN)
                    {
                      if (!*option)
                        /*
                           Error, option character is not trailed by an argument.
                        */
                        opt_error = 4;
                      else
                        /*
                           Error, option character is trailed by an invalid argument.
                        */
                        opt_error = 5;
                    }
                   else
                     skip_option = TRUE;
                   break;
#  ifdef GCAL_EMAIL
                 case '!':
                   option++;
                   if (!*option)
                     /*
                        Error, option character is not trailed by an argument.
                     */
                     opt_error = 4;
                   else
                    {
                      if (email_adr == (char *)NULL)
                        email_adr = (char *)my_malloc (strlen(option)+1,
                                                       124, __FILE__, ((long)__LINE__)-1,
                                                       "email_adr", 0);
                      else
                        email_adr = (char *)my_realloc ((VOID_PTR)email_adr, strlen(option)+1,
                                                        124, __FILE__, ((long)__LINE__)-1,
                                                        "email_adr", 0);
                      strcpy(email_adr, option);
                      skip_option = TRUE;
                    }
                   break;
#  endif
                 case 'r':
                 case 'v':
                   /*
                     If the define_global_text_variable option -r<def:def...>
                       or the define_global_date_variable option -v<def:def...> is given,
                       skip/ignore that option because it's already managed by main().
                       Only check if NO definitions are given because this is an error!
                   */
                   option++;
                   if (!*option)
                     /*
                        Error, option character is not trailed by an argument.
                     */
                     opt_error = 4;
                   skip_option = TRUE;
                   break;
                 case 'f':
                 case 'F':
                   if (isupper(*option))
                     rc_all_dates_flag = TRUE;
                   option++;
                   if (!*option)
                     /*
                        Error, option character is not trailed by an argument.
                     */
                     opt_error = 4;
                   else
                    {
                      len = (int)strlen(option);
                      /*
                         Check if a file name argument with a leading
                           CONNECT_SEP like '+foo' is given.
                      */
                      if (*option == *CONNECT_SEP)
                        /*
                           Error, option character is trailed by an invalid argument.
                        */
                        opt_error = 5;
                      else
                       {
                         if (len > 1)
                          {
                            /*
                               Check if a file name argument with a trailing
                                 CONNECT_SEP like 'foo+' is given.
                            */
                            if (   (*(option + (len-1)) == *CONNECT_SEP)
                                && (*(option + (len-2)) != QUOTE_CHAR))
                              /*
                                 Error, option character is trailed by an invalid argument.
                              */
                              opt_error = 5;
                            else
                             {
                               /*
                                  Check if a file name list argument containing attached
                                    unquoted CONNECT_SEP like 'foo+bar+++file' is given.
                               */
                               ptr_char = strchr(option, *CONNECT_SEP);
                               if (ptr_char != (char *)NULL)
                                {
                                  ptr_char--;
                                  if (*ptr_char == QUOTE_CHAR)
                                    ptr_char++;
                                  ptr_char++;
                                  while (   *ptr_char
                                         && !opt_error)
                                   {
                                     if (*ptr_char == QUOTE_CHAR)
                                       ptr_char++;
                                     else
                                       if (*ptr_char == *CONNECT_SEP)
                                        {
                                          ptr_char++;
                                          if (*ptr_char == QUOTE_CHAR)
                                            ptr_char++;
                                          else
                                            if (*ptr_char == *CONNECT_SEP)
                                              /*
                                                 Error, option character is trailed by an invalid argument.
                                              */
                                              opt_error = 5;
                                        }
                                     if (*ptr_char)
                                       ptr_char++;
                                   }
                                }
                             }
                          }
                         if (!opt_error)
                          {
                            if (rc_filename == (char *)NULL)
                              rc_filename = (char *)my_malloc (len+1,
                                                               124, __FILE__, ((long)__LINE__)-1,
                                                               "rc_filename", 0);
                            else
                              rc_filename = (char *)my_realloc ((VOID_PTR)rc_filename, len+1,
                                                                124, __FILE__, ((long)__LINE__)-1,
                                                                "rc_filename", 0);
                            strcpy(rc_filename, option);
                            rc_use_flag=skip_option = TRUE;
                          }
                       }
                    }
                   break;
                 case '#':
                   option++;
                   if (!*option)
                     /*
                        Error, option character is not trailed by an argument.
                     */
                     opt_error = 4;
                   else
                    {
                      /*
                         If the first `-# ARG' resp., `--here=ARG' option is found:
                           Create and open a temporary file for storing it.
                      */
                      if (rc_here_fn == (char *)NULL)
                       {
                         ptr_char = tmpnam(NULL);
                         len = (int)strlen(ptr_char);
                         rc_here_fn = (char *)my_malloc (len+1,
                                                         124, __FILE__, ((long)__LINE__)-1,
                                                         "rc_here_fn", 0);
                         strcpy(rc_here_fn, ptr_char);
                         if (rc_here_fn == (char *)NULL)
                           my_error (110, __FILE__, ((long)__LINE__)-2, "tmpnam(NULL)=", 0);
                         rc_here_fp = fopen(rc_here_fn, "w");
                         if (rc_here_fp == (FILE *)NULL)
                           my_error (115, __FILE__, (long)__LINE__, rc_here_fn, 0);
                       }
                      /*
                         Copy this long-style option into a buffer.
                      */
                      len = (int)strlen(option);
                      if ((Uint)len >= maxlen_max)
                        resize_all_strings (len+1, FALSE, __FILE__, (long)__LINE__);
                      strcpy(s2, option);
                      /*
                         Manage quoted or unquoted PSEUDO_BLANK characters of the option argument.
                      */
                      if (strchr(s2, PSEUDO_BLANK) != (char *)NULL)
                       {
                         auto char  *ptr_optarg;


                         ptr_optarg=ptr_char = s2;
                         while (*ptr_char)
                          {
                            if (*ptr_char == PSEUDO_BLANK)
                             {
                               ptr_char++;
                               *ptr_optarg++ = ' ';
                             }
                            else
                              if (*ptr_char == QUOTE_CHAR)
                               {
                                 ptr_char++;
                                 if (*ptr_char == PSEUDO_BLANK)
                                   *ptr_optarg++ = *ptr_char++;
                                 else
                                   *ptr_optarg++ = QUOTE_CHAR;
                               }
                              else
                                *ptr_optarg++ = *ptr_char++;
                          }
                         *ptr_optarg = '\0';
                       }
                      /*
                         And write the option argument into the temporary file.
                      */
                      i = fprintf(rc_here_fp, "%s\n", s2);
                      if (i == EOF)
                        my_error (115, __FILE__, ((long)__LINE__)-2, rc_here_fn, 0);
                    }
                   rc_use_flag=skip_option = TRUE;
                   break;
                 case 'D':
                   option++;
                   if (!*option)
                     /*
                        Error, option character is not trailed by an argument.
                     */
                     opt_error = 4;
                   else
                    {
                      /*
                         Check for %? special_texts (without the leading
                           `%' character) that disable a fixed date.
                      */
                      ptr_char = option;
                      while (   *ptr_char
                             && !opt_error)
                        if (   *ptr_char == RC_EX_LHDY_CHAR
                            || *ptr_char == RC_EX_NLHDY_CHAR
                            || *ptr_char == RC_EX_AHDY_CHAR
                            || *ptr_char == RC_EX_NAHDY_CHAR
                            || *ptr_char == RC_EX_MON_CHAR
                            || *ptr_char == RC_EX_NMON_CHAR
                            || *ptr_char == RC_EX_TUE_CHAR
                            || *ptr_char == RC_EX_NTUE_CHAR
                            || *ptr_char == RC_EX_WED_CHAR
                            || *ptr_char == RC_EX_NWED_CHAR
                            || *ptr_char == RC_EX_THU_CHAR
                            || *ptr_char == RC_EX_NTHU_CHAR
                            || *ptr_char == RC_EX_FRI_CHAR
                            || *ptr_char == RC_EX_NFRI_CHAR
                            || *ptr_char == RC_EX_SAT_CHAR
                            || *ptr_char == RC_EX_NSAT_CHAR
                            || *ptr_char == RC_EX_SUN_CHAR
                            || *ptr_char == RC_EX_NSUN_CHAR
                            || *ptr_char == RC_EX_MON_2_THU_CHAR
                            || *ptr_char == RC_EX_NMON_2_THU_CHAR
                            || *ptr_char == RC_EX_MON_2_FRI_CHAR
                            || *ptr_char == RC_EX_NMON_2_FRI_CHAR)
                          ptr_char++;
                        else
                          /*
                             Error, invalid %? special text character given.
                          */
                          opt_error = 5;
                      if (!opt_error)
                       {
                         if (rc_filter_day == (char *)NULL)
                           rc_filter_day = (char *)my_malloc (strlen(option)+1,
                                                              124, __FILE__, ((long)__LINE__)-1,
                                                              "rc_filter_day", 0);
                         else
                           rc_filter_day = (char *)my_realloc ((VOID_PTR)rc_filter_day, strlen(option)+1,
                                                               124, __FILE__, ((long)__LINE__)-1,
                                                               "rc_filter_day", 0);
                         strcpy(rc_filter_day, option);
                       }
                    }
                   skip_option = TRUE;
                   break;
                 case 'P':
                   option++;
                   if (!*option)
                     /*
                        Error, option character is not trailed by an argument.
                     */
                     opt_error = 4;
                   else
                    {
                      /*
                         Check for %?[DATE][#[DATE]] special_texts (without the
                           leading `%' character) that disable a fixed date.
                           If more than one special text is specified, it must
                           be separated by a SPLIT_SEP.
                      */
                      ptr_char = option;
                      while (   *ptr_char
                             && !opt_error)
                        if (   *ptr_char == RC_IDATE_CHAR
                            || *ptr_char == RC_EDATE_CHAR)
                         {
                           ptr_char++;
                           while (   *ptr_char
                                  && (*ptr_char != *SPLIT_SEP))
                             ptr_char++;
                           if (   !*ptr_char
                               || *ptr_char == *SPLIT_SEP)
                            {
                              if (*ptr_char)
                                ptr_char++;
                            }
                           else
                             /*
                                Error, invalid %?[DATE][#[DATE]] special text given.
                             */
                             opt_error = 5;
                         }
                        else
                          /*
                             Error, invalid %?[DATE][#[DATE]] special text given.
                          */
                          opt_error = 5;
                      if (!opt_error)
                       {
                         if (rc_filter_period == (char *)NULL)
                           rc_filter_period = (char *)my_malloc (strlen(option)+1,
                                                                 124, __FILE__, ((long)__LINE__)-1,
                                                                 "rc_filter_period", 0);
                         else
                           rc_filter_period = (char *)my_realloc ((VOID_PTR)rc_filter_period, strlen(option)+1,
                                                                124, __FILE__, ((long)__LINE__)-1,
                                                                "rc_filter_period", 0);
                         strcpy(rc_filter_period, option);
                       }
                    }
                   skip_option = TRUE;
                   break;
                 case 'I':
                   option++;
                   if (!*option)
                     /*
                        Error, option character is not trailed by an argument.
                     */
                     opt_error = 4;
                   else
                    {
                      if (rc_filter_text == (char *)NULL)
                        rc_filter_text = (char *)my_malloc (strlen(option)+1,
                                                            124, __FILE__, ((long)__LINE__)-1,
                                                            "rc_filter_text", 0);
                      else
                        rc_filter_text = (char *)my_realloc ((VOID_PTR)rc_filter_text, strlen(option)+1,
                                                             124, __FILE__, ((long)__LINE__)-1,
                                                             "rc_filter_text", 0);
                      strcpy(rc_filter_text, option);
                    }
                   skip_option = TRUE;
                   break;
                 case 'g':
                   option++;
                   if (*option)
                    {
                      if (rc_grp_sep == (char *)NULL)
                        rc_grp_sep = (char *)my_malloc (strlen(option)+1,
                                                        124, __FILE__, ((long)__LINE__)-1,
                                                        "rc_grp_sep", 0);
                      else
                        rc_grp_sep = (char *)my_realloc ((VOID_PTR)rc_grp_sep, strlen(option)+1,
                                                         124, __FILE__, ((long)__LINE__)-1,
                                                         "rc_grp_sep", 0);
                      strcpy(rc_grp_sep, option);
                    }
                   else
                    {
                      if (rc_grp_sep == (char *)NULL)
                        rc_grp_sep = (char *)my_malloc (strlen(RC_GROUP_SEP)+1,
                                                        124, __FILE__, ((long)__LINE__)-1,
                                                        "rc_grp_sep", 0);
                      else
                        rc_grp_sep = (char *)my_realloc ((VOID_PTR)rc_grp_sep, strlen(RC_GROUP_SEP)+1,
                                                         124, __FILE__, ((long)__LINE__)-1,
                                                         "rc_grp_sep", 0);
                      strcpy(rc_grp_sep, RC_GROUP_SEP);
                    }
                   skip_option = TRUE;
                   break;
                 case 'a':
                   rc_enable_fn_flag = TRUE;
                   break;
                 case 'A':
                   rc_alternative_format_flag = TRUE;
                   break;
                 case 'B':
                   rc_bypass_shell_cmd = TRUE;
                   break;
                 case 'd':
                   rc_have_today_in_list = 1;
                   break;
                 case 'e':
                 case 'E':
                   if (isupper(*option))
                    {
                      rc_enable_hdl_flag = TRUE;
                      rc_enable_hda_flag = FALSE;
                    }
                   else
                    {
                      rc_enable_hdl_flag = FALSE;
                      rc_enable_hda_flag = TRUE;
                    }
                   break;
                 case 'o':
                   rc_omit_date_flag = TRUE;
                   break;
                 case 'l':
                   /*
                      Assume the user wants to have a list
                        of periods instead of a single period!
                   */
                   rc_period_list = TRUE;
                   break;
                 case 'k':
                   rc_weekno_flag = TRUE;
                   break;
                 case 'U':
                   rc_suppr_date_part_flag = TRUE;
                   break;
                 case 'x':
                   rc_title_flag = FALSE;
                   break;
                 case 'z':
                   rc_count_flag = TRUE;
                   break;
                 case 'Z':
                   rc_zero_dates_flag = TRUE;
                   break;
                 case 'c':
                 case 'C':
                   rc_use_flag = TRUE;
                   if (isupper(*option))
                     rc_all_dates_flag = TRUE;
                   option++;
                   if (*option == *DES_LIT)
                    {
                      rc_sort_des_flag = TRUE;
                      while (*option == *DES_LIT)
                        option++;
                    }
                   option--;
                   break;
                 case '0':
                 case '1':
                 case '2':
                 case '3':
                 case '4':
                 case '5':
                 case '6':
                 case '7':
                 case '8':
                 case '9':
                 case RC_HDY_CHAR:
                 case RC_NWD_CHAR:
                   {
                     register int   act_is_leap_year=(days_of_february (act_year)==29);
                     auto     int   wmax=WEEK_MAX;
                     auto     char  dvar='\0';
                     auto     Bool  nth_day_of_year_flag=FALSE;


                     i = 0;
                     if (act_year == greg->year)
                       wmax = ((DAY_LAST + (days_of_february (greg->year) == 29)
                              - (greg->last_day - greg->first_day + 1)) / DAY_MAX) + 1;
                     rc_clean_flags ();
                     rc_period = 0;
                     rc_period_flag = TRUE;
                     /*
                        Before I forget... All mode specifying characters
                          ('d' or 'w') may be given in upper or lower
                          case letters, i.e. they are managed case insensitive!
                     */
                     /*
                        Check if @<e|t|`dvar'>[[-]<n>[`ww[w]']] or
                          *d|w<n>[`ww[w]'] is given.
                     */
                     if (   *option == RC_HDY_CHAR
                         || *option == RC_NWD_CHAR)
                       dvar = *option;
                     else
                       /*
                          Compute the period for the `<n>d' modifier.
                       */
                       rc_period = atoi(option);
                     if (!dvar)
                      {
                        /*
                           Check if a `<n>+' modifier is given.
                        */
                        rc_fwdf_buffer=rc_forwards_flag = (Bool)(strchr(option, *ASC_LIT) != (char *)NULL);
                        /*
                           Check if a `<n>-' modifier is given.
                        */
                        rc_bwdf_buffer=rc_backwards_flag = (Bool)(strchr(option, *DES_LIT) != (char *)NULL);
                        /*
                           Check if a `<n>w' modifier is given.
                        */
                        ptr_char = strrchr(option, 'w');
                        if (ptr_char == (char *)NULL)
                          ptr_char = strrchr(option, 'W');
                        if (ptr_char != (char *)NULL)
                         {
                           ptr_char++;
                           if (!*ptr_char)
                             rc_week_year_flag = TRUE;
                         }
                        /*
                           Check if a `<n>d' modifier is given.
                        */
                        ptr_char = strrchr(option, 'd');
                        if (ptr_char == (char *)NULL)
                          ptr_char = strrchr(option, 'D');
                        if (ptr_char != (char *)NULL)
                         {
                           ptr_char++;
                           if (!*ptr_char)
                             nth_day_of_year_flag = TRUE;
                         }
                        i = (int)rc_forwards_flag + rc_backwards_flag
                            + rc_week_year_flag + nth_day_of_year_flag;
                      }
                     /*
                        Compute the period for a `<n>d' modifier.
                     */
                     if (rc_period == 999)
                       rc_period = DAY_LAST + act_is_leap_year;
                     if (   !dvar
                         && (   i > 1
                             || (   (rc_period > DAY_LAST+act_is_leap_year)
                                 && (   rc_forwards_flag
                                     || rc_backwards_flag
                                     || nth_day_of_year_flag))
                             || (   rc_week_year_flag
                                 && (rc_period > wmax+1)
                                 && (rc_period != 99))))
                       /*
                          Error, illegal fixed date period modifier given.
                       */
                       opt_error = 5;
                     else
                      {
                        /*
                           Found one of these fixed date period modifiers:
                             @<e|t|`dvar'>[[-]<n>[`ww[w]']]
                             *d|w<n>[`ww[w]']
                             `mmdd'
                             `mmww[w]'<n>
                        */
                        if (   !rc_week_year_flag
                            && !rc_forwards_flag
                            && !rc_backwards_flag)
                         {
                           auto int   y=act_year;
                           auto int   n;
                           auto char  hc;
                           auto Bool  is_weekday_mode;


                           if (!nth_day_of_year_flag)
                            {
                              len = (int)strlen(option) + len_year_max;
                              if ((Uint)len >= maxlen_max)
                                resize_all_strings (len+1, FALSE, __FILE__, (long)__LINE__);
                              sprintf(s2, "%0*d%s", len_year_max, y, option);
                              /*
                                 The `rc_get_date()' arguments `wmax', `hc' and `i' are only
                                   dummys and must be given.  They are not respected!
                              */
#  if USE_DE
                              (void)rc_get_date (s2, lineptrs, FALSE, &is_weekday_mode, &day, &month,
                                                 &y, &n, &wmax, &hc, &i, &i, INTERNAL_TXT, -1L, s2, FALSE);
#  else /* !USE_DE */
                              (void)rc_get_date (s2, lineptrs, FALSE, &is_weekday_mode, &day, &month,
                                                 &y, &n, &wmax, &hc, &i, &i, _("Internal"), -1L, s2, FALSE);
#  endif /* !USE_DE */
                              if (y != SPECIAL_VALUE)
                               {
                                 if (!dvar)
                                  {
                                    /*
                                       `mmww[w]'<n> or `mmdd' given.
                                    */
                                    if (!month)
                                      month = act_month;
                                    if (   month < MONTH_MIN
                                        || month > MONTH_MAX)
                                      /*
                                         Error, invalid month given.
                                      */
                                      opt_error = 5;
                                    else
                                     {
                                       i = dvec[month-1];
                                       if (month == 2)
                                         i += act_is_leap_year;
                                       if (is_weekday_mode)
                                        {
                                          /*
                                             `mmww[w]'<n> given.
                                          */
                                          if (n == 9)
                                            day = eval_holiday (i, month, act_year, day, FALSE);
                                          else
                                           {
                                             day = eval_holiday (DAY_MIN, month, act_year, day, TRUE);
                                             day += (DAY_MAX * (n - 1));
                                             if (day > i)
                                               /*
                                                  Error, month contains no such "n'th weekday of month".
                                               */
                                               opt_error = 5;
                                             else
                                              {
                                                /*
                                                   Now check if the given date is valid.
                                                */
                                                if (   !day
                                                    || !valid_date (day, month, act_year))
                                                  /*
                                                     Error, invalid date given.
                                                  */
                                                  opt_error = 5;
                                              }
                                           }
                                        }
                                       else
                                        {
                                          /*
                                             `mmdd' given.
                                          */
                                          if (day == 99)
                                            day = i;
                                          if (!day)
                                            day = act_day;
                                          if (   day < DAY_MIN
                                              || day > i)
                                            /*
                                               Error, invalid day given.
                                            */
                                            opt_error = 5;
                                        }
                                     }
                                  }
                                 else
                                   if (   !day
                                       || !month)
                                     /*
                                        Error, either invalid date variable
                                          or invalid mode specifier given (not d|w).
                                     */
                                     opt_error = 5;
                                 if (!opt_error)
                                   rc_period = day_of_year (day, month, act_year);
                               }
                              else
                                /*
                                   Error, invalid date given.
                                */
                                opt_error = 5;
                            }
                           else
                             if (!rc_period)
                               /*
                                  Error, zero date is invalid.
                               */
                               opt_error = 5;
                           if (!opt_error)
                            {
                              i = day_of_year (act_day, act_month, act_year);
                              if (rc_period >= i)
                               {
                                 rc_period -= i;
                                 /*
                                    Set `rc_forwards_flag' only if the recomputed
                                      `rc_period' is not zero (it is in case @t
                                      is given and the actual date is the last day
                                      of the year, so we have to manage this like
                                      simple `-c' is given in command line.
                                 */
                                 if (!rc_period)
                                   rc_period_flag = FALSE;
                                 else
                                   rc_forwards_flag = TRUE;
                               }
                              else
                               {
                                 rc_period = i - rc_period;
                                 rc_backwards_flag = TRUE;
                               }
                            }
                         }
                        else
                         {
                           /*
                              <n>w|+|- given.
                           */
                           i = 0;
                           while (isdigit(*option))
                            {
                              i++;
                              option++;
                            }
                           option--;
                           if (   !rc_period
                               && !rc_week_year_flag)
                             /*
                                Error, zero length date of fixed date period given.
                             */
                             opt_error = 5;
                           else
                             further_check = TRUE;
                         }
                      }
                   }
                   skip_option = TRUE;
                   break;
                 case 't':
                 case 'T':
                   rc_clean_flags ();
                   rc_use_flag=rc_tomorrow_flag = TRUE;
                   if (isupper(*option))
                     rc_all_dates_flag = TRUE;
                   option++;
                   if (*option)
                     /*
                        Error, invalid option modifier given.
                     */
                     opt_error = 2;
                   skip_option = TRUE;
                   break;
                 case 'w':
                 case 'W':
                   rc_clean_flags ();
                   rc_use_flag=rc_week_flag = TRUE;
                   if (isupper(*option))
                     rc_all_dates_flag = TRUE;
                   further_check = TRUE;
                   break;
                 case 'm':
                 case 'M':
                   rc_clean_flags ();
                   rc_use_flag=rc_month_flag = TRUE;
                   if (isupper(*option))
                     rc_all_dates_flag = TRUE;
                   further_check = TRUE;
                   break;
                 case 'y':
                 case 'Y':
                   rc_clean_flags ();
                   rc_use_flag=rc_year_flag = TRUE;
                   if (isupper(*option))
                     rc_all_dates_flag = TRUE;
                   further_check = TRUE;
                   break;
#endif /* USE_RC */
                 default:
                   /*
                      Error, any unmanaged options are always invalid.
                   */
                   opt_error = 2;
               }
#if USE_RC
            if (   further_check
                && !opt_error)
             {
               option++;
               if (*option)
                {
                  rc_week_year_flag = (Bool)(tolower(*option) == 'w');
                  rc_forwards_flag = (Bool)(*option == *ASC_LIT);
                  rc_backwards_flag = (Bool)(*option == *DES_LIT);
                  option++;
                  if (   *option
                      || (   !rc_week_year_flag
                          && !rc_forwards_flag
                          && !rc_backwards_flag))
                    /*
                       Error, either argument is trailed by an illegal character
                         or no mode specifying character (w|+|-) is given.
                    */
                    opt_error = 2;
                }
               skip_option = TRUE;
             }
#endif /* USE_RC */
            if (opt_error)
             {
               switch (opt_error)
                {
#if USE_DE
                  case 1:
                    len = (int)strlen(*argv) + 100;
                    if ((Uint)len >= maxlen_max)
                      resize_all_strings (len+1, FALSE, __FILE__, (long)__LINE__);
                    sprintf(s1, "%s: Option `%s' ist nicht eindeutig",
                            prgr_name, *argv);
                    break;
                  case 2:
                    if (is_longopt)
                     {
                       len = (int)strlen(*argv) + 100;
                       if ((Uint)len >= maxlen_max)
                         resize_all_strings (len+1, FALSE, __FILE__, (long)__LINE__);
                       sprintf(s1, "%s: unbekannte Option `%s'",
                               prgr_name, *argv);
                     }
                    else
                     {
                       len = (int)strlen(*argv+1) + 100;
                       if ((Uint)len >= maxlen_max)
                         resize_all_strings (len+1, FALSE, __FILE__, (long)__LINE__);
                       sprintf(s1, "%s: ung%sltige Option -- %s",
                               prgr_name, UE, *argv+1);
                     }
                    break;
                  case 3:
                    len = (int)strlen(*argv) + 100;
                    if ((Uint)len >= maxlen_max)
                      resize_all_strings (len+1, FALSE, __FILE__, (long)__LINE__);
                    sprintf(s1, "%s: Option `%s' erlaubt kein Argument",
                            prgr_name, *argv);
                    break;
                  case 4:
                    if (is_longopt)
                     {
                       len = (int)strlen(*argv) + 100;
                       if ((Uint)len >= maxlen_max)
                         resize_all_strings (len+1, FALSE, __FILE__, (long)__LINE__);
                       sprintf(s1, "%s: Option `%s' ben%stigt ein Argument",
                               prgr_name, *argv, OE);
                     }
                    else
                     {
                       len = (int)strlen(*argv+1) + 100;
                       if ((Uint)len >= maxlen_max)
                         resize_all_strings (len+1, FALSE, __FILE__, (long)__LINE__);
                       sprintf(s1, "%s: Option ben%stigt ein Argument -- %s",
                               prgr_name, OE, *argv+1);
                     }
                    break;
                  case 5:
                    len = (int)strlen(*argv) + 100;
                    if ((Uint)len >= maxlen_max)
                      resize_all_strings (len+1, FALSE, __FILE__, (long)__LINE__);
                    sprintf(s1, "%s: Option mit unzul%sssigem Argument -- %s",
                            prgr_name, AE, *argv);
                    break;
                  case 6:
                    len = (int)strlen(*argv) + 100;
                    if ((Uint)len >= maxlen_max)
                      resize_all_strings (len+1, FALSE, __FILE__, (long)__LINE__);
                    sprintf(s1, "%s: Option mit nicht eindeutigem Argument -- %s",
                            prgr_name, *argv);
#else /* !USE_DE */
                  case 1:
                    len = (int)strlen(*argv) + 100;
                    if ((Uint)len >= maxlen_max)
                      resize_all_strings (len+1, FALSE, __FILE__, (long)__LINE__);
                    sprintf(s1, _("%s: option `%s' is ambiguous"),
                            prgr_name, *argv);
                    break;
                  case 2:
                    if (is_longopt)
                     {
                       len = (int)strlen(*argv) + 100;
                       if ((Uint)len >= maxlen_max)
                         resize_all_strings (len+1, FALSE, __FILE__, (long)__LINE__);
                       sprintf(s1, _("%s: unrecognized option `%s'"),
                               prgr_name, *argv);
                     }
                    else
                     {
                       len = (int)strlen(*argv+1) + 100;
                       if ((Uint)len >= maxlen_max)
                         resize_all_strings (len+1, FALSE, __FILE__, (long)__LINE__);
                       sprintf(s1, _("%s: invalid option -- %s"),
                               prgr_name, *argv+1);
                     }
                    break;
                  case 3:
                    len = (int)strlen(*argv) + 100;
                    if ((Uint)len >= maxlen_max)
                      resize_all_strings (len+1, FALSE, __FILE__, (long)__LINE__);
                    sprintf(s1, _("%s: option `%s' doesn't allow an argument"),
                            prgr_name, *argv);
                    break;
                  case 4:
                    if (is_longopt)
                     {
                       len = (int)strlen(*argv) + 100;
                       if ((Uint)len >= maxlen_max)
                         resize_all_strings (len+1, FALSE, __FILE__, (long)__LINE__);
                       sprintf(s1, _("%s: option `%s' requires an argument"),
                               prgr_name, *argv);
                     }
                    else
                     {
                       len = (int)strlen(*argv+1) + 100;
                       if ((Uint)len >= maxlen_max)
                         resize_all_strings (len+1, FALSE, __FILE__, (long)__LINE__);
                       sprintf(s1, _("%s: option requires an argument -- %s"),
                               prgr_name, *argv+1);
                     }
                    break;
                  case 5:
                    len = (int)strlen(*argv) + 100;
                    if ((Uint)len >= maxlen_max)
                      resize_all_strings (len+1, FALSE, __FILE__, (long)__LINE__);
                    sprintf(s1, _("%s: option with invalid argument -- %s"),
                            prgr_name, *argv);
                    break;
                  case 6:
                    len = (int)strlen(*argv) + 100;
                    if ((Uint)len >= maxlen_max)
                      resize_all_strings (len+1, FALSE, __FILE__, (long)__LINE__);
                    sprintf(s1, _("%s: option with ambiguous argument -- %s"),
                            prgr_name, *argv);
#endif /* !USE_DE */
                    break;
                  default:
                    /*
                       This case must be an internal error
                    */
                    my_error (opt_error, __FILE__, ((long)__LINE__)-4, "", 0);
                }
               fprintf(stderr, "%s\n%s\n", s1, usage_msg ());
               put_longopt_description (stderr);
               S_NEWLINE(stderr);
               my_exit (126);
             }
            if (skip_option)
             {
               while (*option)
                 option++;
               option--;
             }
          }
         argc--;
       }
      else
       {
#if USE_RC
         if (**argv == RC_ADATE_CHAR)
           /*
              If an actual date modifier %... is given, skip/ignore this option
                because it's already managed by the main() function.
           */
           argc--;
         else
#endif
           if (**argv == RSP_CHAR)
             /*
                If a response file @file is given, skip/ignore this option,
                  because we don't manage nested response files!
             */
             argc--;
           else
             /*
                If a command is given, skip processing the options.
             */
             break;
       }
    }
   if (   help_flag
       || license_flag
       || version_flag)
    {
#if USE_PAGER
      /*
         No program output yet and output goes to a tty:
           Set the standard stream buffers to unbuffered mode,
           then get actual settings of the tty (needed if paging is wanted).
      */
      if (   is_tty1
#  ifdef GCAL_EPAGER
          && (ext_pager == (char *)NULL)
#  endif
          && is_tty2)
       {
         setbuf(stdout, (char *)NULL);
         setbuf(stdin, (char *)NULL);
         get_tty_scr_size (&tty_rows, &tty_cols);
       }
      else
        if (help_on_help_flag)
          /*
             We have to use the actual screen size in this case!
          */
          get_tty_scr_size (&tty_rows, &tty_cols);
#endif /* USE_PAGER */
      /*
         Get the highlighting sequences of the terminal.
      */
      get_tty_hls (hl_seq);
#ifdef GCAL_EPAGER
      if (   is_tty1
          && is_tty2
          && pager_flag
          && (ext_pager != (char *)NULL)
          && (email_adr == (char *)NULL))
       {
         child_pid = fork();
         switch (child_pid)
          {
            case -1:
              /*
                 Error, the `fork()' function have failed.
              */
              my_error (110, __FILE__, ((long)__LINE__)-7, "fork() child_pid=", child_pid);
            case 0:
              /*
                 Child process (read from pipe):
                   Connect PIPE-stdin to SYS-stdin in a safe way
                   and launch external pager program.
              */
              close(pipe_fd[1]);
              if (pipe_fd[0])
               {
                 close(0);
                 dup(pipe_fd[0]);
                 close(pipe_fd[0]);
               }
              i = execlp(ext_pager, ext_pager, (char *)NULL);
              /*
                 Error, the `execlp()' function have failed (this line should never be executed).
              */
              len = (int)strlen(ext_pager) + 100;
              if ((Uint)len >= maxlen_max)
                resize_all_strings (len+1, FALSE, __FILE__, (long)__LINE__);
              sprintf(s1, "execlp(%s)=", ext_pager);
              my_error (110, __FILE__, ((long)__LINE__)-8, s1, i);
            default:
              /*
                 Parent process (write to pipe):
                   Connect PIPE-stdout to SYS-stdout in a safe way
                   and initiate the action.
              */
              close(pipe_fd[0]);
              if (pipe_fd[1] != 1)
               {
                 close(1);
                 if (!dup(pipe_fd[1]))
                  {
                    dup(pipe_fd[1]);
                    close(0);
                  }
                 close(pipe_fd[1]);
               }
          }
       }
#endif /* GCAL_EPAGER */
      if (help_flag)
       {
         if (help_on_help_flag)
           /*
              "Help on help" page wanted (--long-help=?).
           */
#if USE_PAGER
           my_help_on_help (stdout, s3, lopt_ambig, tty_cols);
#else /* !USE_PAGER */
           my_help_on_help (stdout, s3, lopt_ambig, 80);
#endif /* !USE_PAGER */
         else
            if (ext_help_flag)
              /*
                 "Extended" help pages wanted (--long-help[=ARG]|[=?] or -hh or -?? or -h? or -?h).
              */
              my_extended_help (stdout, lopt_help);
            else
              /*
                 "Basic" help page wanted (--help or -h or -?).
              */
              my_basic_help (stdout);
       }
      else
       {
         if (license_flag)
           /*
              License text wanted (--license or -L).
           */
           my_license (stdout);
         else
           if (version_flag)
             /*
                Version text wanted (--version or -V).
             */
             my_version (stdout);
       }
#ifdef GCAL_EPAGER
      if (   is_tty1
          && is_tty2
          && pager_flag
          && (ext_pager != (char *)NULL)
          && (email_adr == (char *)NULL))
       {
         /*
            We work unbuffered in this case so we have to flush the
              stdout buffer for showing all contents of it.
         */
         fflush(stdout);
         /*
            And reconnect the SYS-stdin/SYS-stdout file descriptors.
         */
         close(0);
         dup(sys_fd[0]);
         close(1);
         dup(sys_fd[1]);
         /*
            And wait until the child has performed all action.
         */
         while (wait((int *)NULL) != child_pid)
           ;
#  if HAVE_SIGNAL && defined(SIGPIPE)
         /*
            Reset the SIGPIPE signal.
         */
         (void)signal(SIGPIPE, SIG_DFL);
#  endif
       }
#endif /* GCAL_EPAGER */
      my_exit (exit_stat_help);
    }
   month = 0;
   if (argc > 1)
    {
      auto Bool  is_list=FALSE;
      auto Bool  is_range=FALSE;
      auto Bool  month_set=FALSE;
      auto Bool  year_sep_found=FALSE;


      for ( ; argc > 1 ; argc--, argv++)
       {
         if (!month)
          {
            option = *argv;
            if (*option == *MONTH3_LIT)
              break;
            else
              while (*option)
               {
                 if (   *option == *MLIST_SEP
                     || *option == *YLIST_SEP)
                   is_list = TRUE;
                 else
                   if (   *option == *MRANGE_SEP
                       || *option == *YRANGE_SEP)
                     is_range = TRUE;
                   else
                     if (*option == *YEAR_SEP)
                       year_sep_found = TRUE;
                     else
                       if (*option == *FYEAR_SEP)
                         is_fiscal_year = TRUE;
                 option++;
               }
            option = *argv;
            if (   !is_list
                && !is_range
                && !year_sep_found
                && !is_fiscal_year
                && !my_atoi (option))
             {
               month = compare_d_m_name (option, MOnth);
               if (month)
                {
                  month_set = TRUE;
                  continue;
                }
             }
          }
         if (!month)
          {
            build_month_list (argv);
            if (!(*month_list).month)
              month = atoi(*argv);
            else
              month = (*month_list).month;
            if (   month < MONTH_MIN
                || month > MONTH_MAX)
             {
               if (!is_fiscal_year)
                 year = month;
               month = 0;
               break;
             }
          }
         else
           if (!year)
            {
              if (   (*month_list).year
                  && (   is_list
                      || is_range))
                year = (*month_list).year;
              else
                year = my_atoi (*argv);
              /*
                 Check if a `mm yyyy-yyyy' command is given
                   (mm is already managed), if so, evaluate the year range part.
              */
              if (   month
                  && isdigit(**argv)
                  && (strchr(*argv, *MRANGE_SEP) != (char *)NULL))
               {
                 i = 0;
                 while (isdigit(*(*(argv)+i)))
                  {
                    s2[i] = *(*(argv) + i);
                    i++;
                  }
                 if (*(*(argv)+i) == *MRANGE_SEP)
                  {
                    s2[i++] = '\0';
                    year = my_atoi (s2);
                    if (!year)
                      year = act_year;
                    (*month_list).year = year;
                    year = my_atoi (*(argv)+i);
                    if (!year)
                      year = act_year;
                    month_list[1].year = year;
                    (*month_list).month=month_list[1].month = month;
                    is_special_range = TRUE;
                  }
                 else
                  {
                    year = month;
                    month = 0;
                  }
                 break;
               }
            }
       }
      /*
         "." or ".." or ".+" or ".-" argument found:
           3 month mode wanted
      */
      if (*option == *MONTH3_LIT)
       {
         len = (int)strlen(option);
         if (len <= 2)
          {
            if (*(option + 1) == *MONTH3_LIT)
              is_3month_mode2 = TRUE;
            else
              is_3month_mode = TRUE;
            is_fiscal_year = TRUE;
            year_flag = FALSE;
            month = act_month;
            /*
               Check if an explicit year trails the 3 month mode command.
            */
            if (argc > 2)
             {
               argv++;
               year = atoi(*argv);
               if (!year)
                 year = act_year;
             }
            else
              year = act_year;
            if (is_3month_mode)
             {
               /*
                  Either "." or ".+" or ".-" 3 month mode command found.
               */
               if (!*(option + 1))
                {
                  /*
                     "." 3 month mode command found.
                  */
                  month--;
                  if (!month)
                   {
                     month = MONTH_MAX;
                     year--;
                   }
                }
               else
                 if (*(option + 1) == *DES_LIT)
                  {
                    /*
                       ".-" 3 month mode command found.
                    */
                    month -= 2;
                    if (month < MONTH_MIN)
                     {
                       month = MONTH_MAX + month;
                       year--;
                     }
                  }
                 else
                   if (*(option + 1) == *ASC_LIT)
                     /*
                        ".+" 3 month mode command found.
                     */
                     ;   /* Void, nothing to do! */
                   else
                    {
                      /*
                         Error, invalid modifier trails the dot character:
                           Print default month calendar only.
                      */
                      is_fiscal_year = FALSE;
                      year = act_year;
                      month = act_month;
                    }
               if (year < YEAR_MIN)
                {
                  year = YEAR_MIN;
                  month = act_month;
                }
             }
            else
              /*
                 ".." 3 month mode command found.
              */
              month = ((month + 1) - ((month - 1) % 3)) - 1;
          }
       }
      else
        if (   !month
            && !year
            && !month_set
            && !(*month_list).month
            && !is_fiscal_year)
         {
           year = month;
           month = 0;
         }
    }
}



   LOCAL void
build_month_list (argv)
   char *argv[];
/*
   If more than a single month/year is wanted (means list or ranges of months
     or years), fill the global data structure `month_list[]'; which is used in
     the print_calendar() function; according the values found in the command line.
*/
{
   register       int    i;
   register       int    j;
   register       int    k;
   auto     const char  *option;
   auto           Bool   is_list=FALSE;
   auto           Bool   is_range=FALSE;
   auto           Bool   year_sep_found=FALSE;
   auto           Bool   buf_is_ext_year;


   option = *argv;
   for ( ; *option && !is_list && !is_range ; option++)
     if (   *option == *MLIST_SEP
         || *option == *YLIST_SEP)
      {
        is_list = TRUE;
        if (*option == *YLIST_SEP)
          is_ext_list=is_ext_year = TRUE;
      }
     else
       if (   *option == *MRANGE_SEP
           || *option == *YRANGE_SEP)
        {
          is_range = TRUE;
          if (*option == *YRANGE_SEP)
            is_ext_range=is_ext_year = TRUE;
        }
       else
         if (*option == *YEAR_SEP)
           year_sep_found = TRUE;
   buf_is_ext_year = is_ext_year;
   if (   year_sep_found
       || (   is_fiscal_year
           && (   is_ext_list
               || is_ext_year
               || (   !is_list
                   && !is_range)))
       || (   !is_fiscal_year
           && (   is_list
               || is_range)))
    {
      i = 0;
      option = *argv;
      while (*option)
       {
         year_sep_found = FALSE;
         j = 0;
         while (   *option
                && !year_sep_found
                && (   (   !is_list
                        && !is_range)
                    || (   (   is_list
                            && (   (*option != *MLIST_SEP)
                                && (*option != *YLIST_SEP)))
                        || (   is_range
                            && (   (*option != *MRANGE_SEP)
                                && (*option != *YRANGE_SEP))))))
          {
            year_sep_found = (Bool)(   *option == *YEAR_SEP
                                    || *option == *FYEAR_SEP);
            s1[j++] = *option++;
          }
         if (year_sep_found)
           s1[j-1] = '\0';
         else
           s1[j] = '\0';
         if (!*s1)
          {
            if (is_fiscal_year)
             {
               j = atoi(option);
               if (j)
                {
                  is_ext_year = FALSE;
                  month_list[i++].month = act_month;
                }
             }
            else
             {
               j = 0;
               while (*option)
                 option++;
             }
          }
         else
           j = my_atoi (s1);
         if (is_ext_year)
          {
            if (is_fiscal_year)
             {
               if (   (j >= MONTH_MIN)
                   && (j <= MONTH_MAX))
                 month_list[i++].month = j;
               else
                {
                  /*
                     Is the argument a valid month name?
                  */
                  j = compare_d_m_name (s1, MOnth);
                  if (j)
                    month_list[i++].month = j;
                  else
                    /*
                       Are we at the end of the argument?
                    */
                    if (!*option)
                     {
                       if (   is_ext_list
                           || is_ext_range)
                        {
                          if (!is_ext_list)
                            month_list[i].month = month_list[i-1].month;
                          else
                            month_list[i].month = MONTH_MIN;
                          j = my_atoi (s1);
                          if (j)
                            month_list[i++].year = j;
                          else
                            month_list[i].month = 0;
                        }
                     }
                    else
                      /*
                         Not at the end of the argument:
                           Fiscal year list wanted and standard year given.
                      */
                      if (!year_sep_found)
                       {
                         j = my_atoi (s1);
                         if (j)
                           month_list[i++].month = j;
                       }
                      else
                        /*
                           Otherwise ignore given argument.
                        */
                        year_sep_found = FALSE;
                }
             }
            else
             {
               if (year_sep_found)
                {
                  /*
                     Error, list/range of extended years given like
                       5/1999;may/7777... or 5/1999+may/7777
                       --> respect the year part of the date only.
                  */
                  j = 0;
                  while (   *option
                         && (   (   is_list
                                 && (*option != *YLIST_SEP))
                             || (   is_range
                                 && (*option != *YRANGE_SEP))))
                    s1[j++] = *option++;
                  s1[j] = '\0';
                  j = my_atoi (s1);
                }
               if (j)
                 month_list[i++].month = j;
               else
                 if (is_list)
                   month_list[i++].month = act_year;
             }
            if (year_sep_found)
             {
               while (   *option == *YEAR_SEP
                      || *option == *FYEAR_SEP)
                 option++;
               if (!is_fiscal_year)
                {
                  year_sep_found = FALSE;
                  while (   *option
                         && (   (   is_list
                                 && (*option != *YLIST_SEP))
                             || (   is_range
                                 && (*option != *YRANGE_SEP))))
                    option++;
                }
             }
          }
         else
           if (*s1)
            {
              if (   (j >= MONTH_MIN)
                  && (j <= MONTH_MAX))
                month_list[i++].month = j;
              else
               {
                 j = compare_d_m_name (s1, MOnth);
                 if (j)
                   month_list[i++].month = j;
               }
              /*
                 Check if range of a selected month of years is given like:
                   mm/yyyy-yyyy ; e.g.: 5/1999-2011
                 which means all May months starting in 1999 and ending in 2011.
              */
              if (   i
                  && is_range
                  && !year_sep_found
                  && (*month_list).year
                  && !*option)
               {
                 if (j)
                   i--;
                 else
                   j = my_atoi (s1);
                 if (j)
                  {
                    month_list[i].month = month_list[i-1].month;
                    month_list[i++].year = j;
                    is_ext_range = FALSE;
                    is_special_range = TRUE;
                  }
               }
            }
         if (year_sep_found)
          {
            if (j)
             {
               j = 0;
               while (   *option == *YEAR_SEP
                      || *option == *FYEAR_SEP)
                 option++;
               if (   !is_list
                   && !is_range)
                 while (*option)
                   s1[j++] = *option++;
               else
                 if (   is_fiscal_year
                     && (   is_ext_list
                         || is_ext_range))
                   while (   *option
                          && (   (   is_list
                                  && (*option != *YLIST_SEP))
                              || (   is_range
                                  && (*option != *YRANGE_SEP))))
                     s1[j++] = *option++;
                 else
                   while (   *option
                          && (   (   is_list
                                  && (*option != *MLIST_SEP))
                              || (   is_range
                                  && (*option != *MRANGE_SEP))))
                     s1[j++] = *option++;
               s1[j] = '\0';
               j = my_atoi (s1);
               if (j)
                {
                  month_list[i-1].year = j;
                  if (   is_list
                      && !is_ext_range)
                    is_ext_list = TRUE;
                  else
                    if (   is_range
                        && !is_ext_list)
                      is_ext_range = TRUE;
                }
               else
                 if (is_fiscal_year)
                   month_list[i-1].year = act_year;
             }
            else
             {
               if (is_fiscal_year)
                 while (   *option
                        && (   (   is_list
                                && (*option != *YLIST_SEP))
                            || (   is_range
                                && (*option != *YRANGE_SEP))))
                   option++;
               else
                 while (   *option
                        && (   (   is_list
                                && (*option != *MLIST_SEP))
                            || (   is_range
                                && (*option != *MRANGE_SEP))))
                   option++;
             }
          }
         while (   *option == *MLIST_SEP
                || *option == *MRANGE_SEP
                || *option == *YLIST_SEP
                || *option == *YRANGE_SEP)
           option++;
         /*
            A list/range of fiscal years is wanted and the specified
              list/range of years is incomplete --> avoid error.
         */
         if (   i
             && is_fiscal_year
             && !month_list[i-1].year)
          {
            month_list[i-1].year = month_list[i-1].month;
            month_list[i-1].month = MONTH_MIN;
          }
         is_ext_year = buf_is_ext_year;
         if ((Uint)i >= month_list_max)
          {
            /*
               Resize the `month_list[]' table.
            */
            month_list_max <<= 1;
            if (month_list_max*sizeof(Ml_struct) > testval)
              month_list_max--;
            month_list = (Ml_struct *)my_realloc ((VOID_PTR)month_list, month_list_max*sizeof(Ml_struct),
                                                  124, __FILE__, ((long)__LINE__)-1,
                                                  "month_list[month_list_max]", month_list_max);
            /*
               Initialize the `month_list[]' structure elements.
            */
            for (k=i ; k < (int)month_list_max ; k++)
              month_list[k].year=month_list[k].month = 0;
          }
       }
      if (   is_range
          && !is_ext_range
          && !is_special_range)
       {
         /*
            Fiscal year wanted and a range of months is specified only
              --> avoid error.
         */
         if (is_fiscal_year)
           is_ext_year = TRUE;
         else
          {
            /*
               Ok, compute the range of months and put it in the `month_list[]' table.
            */
            j = (*month_list).month;
            k = month_list[1].month;
            i = 0;
            while (month_list[i].month)
              month_list[i++].month = 0;
            if (   !j
                && !k)
              (*month_list).month = act_month;
            else
             {
               if (!j)
                 j = MONTH_MIN;
               if (!k)
                 k = MONTH_MAX;
               i = 0;
               if (j > k)
                 for ( ; j >= k ; i++, j--)
                   month_list[i].month = j;
               else
                 for ( ; j <= k ; i++, j++)
                   month_list[i].month = j;
             }
          }
       }
      /*
         Fiscal/standard year wanted and an invalid range is specified
           --> avoid error.
      */
      if (   is_ext_range
          && month_list[2].month)
        month_list[1].month = 0;
      /*
         Standard year wanted and the list/range of years is incomplete
           --> avoid error.
      */
      if (   !month_list[1].month
          && (   is_ext_list
              || is_ext_range
              || is_special_range
              || is_ext_year))
       {
         is_ext_list=is_ext_range=is_special_range=is_ext_year = FALSE;
         (*month_list).month = act_month;
         (*month_list).year = act_year;
       }
    }
}



   LOCAL void
eliminate_invalid_data ()
/*
   Eliminates invalid command line argument values
     and sets some internal variables according to the command line arguments.
*/
{
   register int    i;
   register int    len;


   /*
      Correction of month and year.
   */
   if (   !month
       && !year)
    {
      month = act_month;
      year = act_year;
    }
   else
     if (   year < YEAR_MIN
         || year > YEAR_MAX)
      {
        if (   month
            && year)
          month = act_month;
        year = act_year;
      }
   if (   year_flag
       && month)
     month = 0;
   if (   month
       && !(*month_list).month)
     (*month_list).month = month;
   if (   year
       && !(*month_list).year)
     (*month_list).year = year;
   /*
      Set default amount of month rows and columns according to calendar format.
   */
   if (   is_3month_mode
       || is_3month_mode2)
    {
      /*
         Set fixed amount of month rows and columns for 3 month mode;
           the `-b<>' option is ignored.
      */
      if (special_calsheet_flag)
       {
         if (cal_special_flag)
          {
            out_rows = JI3_OUT_ROWS;
            out_cols = JI3_OUT_COLS;
          }
         else
           if (cal_both_dates_flag)
            {
              out_rows = BI3_OUT_ROWS;
              out_cols = BI3_OUT_COLS;
            }
           else
            {
              out_rows = SI3_OUT_ROWS;
              out_cols = SI3_OUT_COLS;
            }
       }
      else
       {
         if (cal_special_flag)
          {
            out_rows = J3_OUT_ROWS;
            out_cols = J3_OUT_COLS;
          }
         else
           if (cal_both_dates_flag)
            {
              out_rows = B3_OUT_ROWS;
              out_cols = B3_OUT_COLS;
            }
           else
            {
              out_rows = S3_OUT_ROWS;
              out_cols = S3_OUT_COLS;
            }
       }
    }
   else
    {
      /*
         Set the default amount of month columns according to calendar format.
      */
      if (special_calsheet_flag)
       {
         if (cal_special_flag)
           out_cols = JI_OUT_COLS;
         else
           if (cal_both_dates_flag)
             out_cols = BI_OUT_COLS;
           else
             out_cols = SI_OUT_COLS;
       }
      else
       {
         if (cal_special_flag)
           out_cols = J_OUT_COLS;
         else
           if (cal_both_dates_flag)
             out_cols = B_OUT_COLS;
           else
             out_cols = S_OUT_COLS;
       }
      /*
         Set the amount of month columns according to calendar format.
      */
      switch (out_rows)
       {
         case 1:
           out_cols = MONTH_MAX;
           break;
         case 2:
           out_cols = 6;
           break;
         case 3:
           out_cols = 4;
           break;
         case 4:
           out_cols = 3;
           break;
         case 6:
           out_cols = 2;
           break;
         case 12:
           out_cols = MONTH_MIN;
           break;
         default:
           /*
              Set default amounts of month rows according to calendar format.
           */
           if (special_calsheet_flag)
            {
              if (cal_special_flag)
                out_rows = JI_OUT_ROWS;
              else
                if (cal_both_dates_flag)
                  out_rows = BI_OUT_ROWS;
                else
                  out_rows = SI_OUT_ROWS;
            }
           else
            {
              if (cal_special_flag)
                out_rows = J_OUT_ROWS;
              else
                if (cal_both_dates_flag)
                  out_rows = B_OUT_ROWS;
                else
                  out_rows = S_OUT_ROWS;
            }
       }
    }
   /*
      Set the length of a single day "cell" used within the calendar sheets according
        to the selected mode (means either NO -j option, -j or the -jb option).
   */
   if (cal_special_flag)
     /*
         +1 because the day of year consists of 3 digits maximum
           plus a separating space (==> 4).
     */
     fmt_len = FMT_LEN_MIN + 1;
   else
     if (cal_both_dates_flag)
       /*
           +5 because such a date consists of the day of month (2 digits maximum)
             plus the day of year (3 digits maximum) plus two textual parantheses
             plus a separating space (==> 8).
       */
       fmt_len = FMT_LEN_MIN + 5;
     else
       /*
          The day of month consists of 2 digits maximum
            plus a separating space (==> 3).
       */
       fmt_len = FMT_LEN_MIN;
   /*
      Set starting day of week if not set by a command line argument;
        this means the special value of SPECIAL_VALUE is found now!
   */
   if (start_day < 0)
     start_day = weekday_of_date (act_day, act_month, act_year);
   else
     if (!start_day)
#if USE_DE
       start_day = DAY_MIN;
#else /* !USE_DE */
#  ifdef GCAL_NLS
       if (!is_en)
         start_day = DAY_MIN;
       else
         start_day = DAY_MAX;
#  else /* !GCAL_NLS */
       start_day = DAY_MAX;
#  endif /* !GCAL_NLS */
#endif /* !USE_DE */
   /*
      Now get the maximum length of a textual day name, which is returned
        by the function `day_name()'.
   */
   len_dayname_max = 0;
   for (i=DAY_MIN ; i <= DAY_MAX ; i++)
    {
      len = (int)strlen(day_name (i));
      if (len > len_dayname_max)
        len_dayname_max = len;
    }
   /*
      Now get the maximum length of a textual month name, which is returned
        by the function `month_name()'.
   */
   len_monthname_max = 0;
   for (i=MONTH_MIN ; i <= MONTH_MAX ; i++)
    {
      len = (int)strlen(month_name (i));
      if (len > len_monthname_max)
        len_monthname_max = len;
    }
   if (use_day_suffix)
    {
      /*
         Now get the length of a textual day suffix, which is returned
           by the function `day_suffix()'.  We check the length of the
           ordinal number suffixes:  1st 2nd 3rd 4th.
      */
      len_suffix_max = 0;
      for (i=DAY_MIN ; i <= 4 ; i++)
       {
         len = (int)strlen(day_suffix (i));
         if (len > len_suffix_max)
           len_suffix_max = len;
       }
    }
}
