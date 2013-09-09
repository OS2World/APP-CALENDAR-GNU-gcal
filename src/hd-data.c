/*
*  hd-data.c:  All data which is composed into the eternal holiday list.
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
static char rcsid[]="$Id: hd-data.c 2.40 1997/03/31 02:04:00 tom Exp $";
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
************************************************** Defined in `hd_use.c'.
*/
IMPORT int
eval_holiday __P_((      int  day,
                   const int  month,
                   const int  year,
                   const int  wd,
                   const Bool forwards));
IMPORT void
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
/*
************************************************** Defined in `utils.c'.
*/
IMPORT int
weekday_of_date __P_((const int day,
                      const int month,
                      const int year));
IMPORT int
day_of_year __P_((const int day,
                  const int month,
                  const int year));
IMPORT int
knuth_easter_formula __P_((const int year));
IMPORT int
julian_gregorian_diff __P_((const int day,
                            const int month,
                            const int year));
/*
************************************************** Defined in `hd_data.c'.
*/
EXPORT const Cc_struct *
binsearch_cc_id __P_((char *id));
EXPORT void
hdy_standard __P_((      Bool *init_data,
                   const Bool  detected,
                   const int   year,
                         int  *hd_elems,
                   const int   fday,
                   const int   count));
EXPORT void
hdy_christian __P_((      Bool *init_data,
                    const Bool  detected,
                    const int   easter,
                    const int   year,
                          int  *hd_elems,
                    const int   fday,
                    const int   count));
LOCAL void
hdy_at __P_((      Bool *init_data,
             const Bool  detected,
             const int   easter,
             const int   year,
                   int  *hd_elems,
             const int   fday,
             const int   count));
LOCAL void
hdy_au __P_((      Bool *init_data,
             const Bool  detected,
             const int   easter,
             const int   year,
                   int  *hd_elems,
             const int   fday,
             const int   count));
LOCAL void
hdy_be __P_((      Bool *init_data,
             const Bool  detected,
             const int   easter,
             const int   year,
                   int  *hd_elems,
             const int   fday,
             const int   count));
LOCAL void
hdy_ca __P_((      Bool *init_data,
             const Bool  detected,
             const int   easter,
             const int   year,
                   int  *hd_elems,
             const int   fday,
             const int   count));
LOCAL void
hdy_ch __P_((      Bool *init_data,
             const Bool  detected,
             const int   easter,
             const int   year,
                   int  *hd_elems,
             const int   fday,
             const int   count));
LOCAL void
hdy_cz __P_((      Bool *init_data,
             const Bool  detected,
             const int   easter,
             const int   year,
                   int  *hd_elems,
             const int   fday,
             const int   count));
LOCAL void
hdy_de __P_((      Bool *init_data,
             const Bool  detected,
             const int   easter,
             const int   year,
                   int  *hd_elems,
             const int   fday,
             const int   count));
LOCAL void
hdy_dk __P_((      Bool *init_data,
             const Bool  detected,
             const int   easter,
             const int   year,
                   int  *hd_elems,
             const int   fday,
             const int   count));
LOCAL void
hdy_es __P_((      Bool *init_data,
             const Bool  detected,
             const int   easter,
             const int   year,
                   int  *hd_elems,
             const int   fday,
             const int   count));
LOCAL void
hdy_fi __P_((      Bool *init_data,
             const Bool  detected,
             const int   easter,
             const int   year,
                   int  *hd_elems,
             const int   fday,
             const int   count));
LOCAL void
hdy_fr __P_((      Bool *init_data,
             const Bool  detected,
             const int   easter,
             const int   year,
                   int  *hd_elems,
             const int   fday,
             const int   count));
LOCAL void
hdy_gb __P_((      Bool *init_data,
             const Bool  detected,
             const int   easter,
             const int   year,
                   int  *hd_elems,
             const int   fday,
             const int   count));
LOCAL void
hdy_gr __P_((      Bool *init_data,
             const Bool  detected,
                   int   easter,
             const int   year,
                   int  *hd_elems,
             const int   fday,
             const int   count));
LOCAL void
hdy_hu __P_((      Bool *init_data,
             const Bool  detected,
             const int   easter,
             const int   year,
                   int  *hd_elems,
             const int   fday,
             const int   count));
LOCAL void
hdy_it __P_((      Bool *init_data,
             const Bool  detected,
             const int   easter,
             const int   year,
                   int  *hd_elems,
             const int   fday,
             const int   count));
LOCAL void
hdy_mx __P_((      Bool *init_data,
             const Bool  detected,
             const int   easter,
             const int   year,
                   int  *hd_elems,
             const int   fday,
             const int   count));
LOCAL void
hdy_nl __P_((      Bool *init_data,
             const Bool  detected,
             const int   easter,
             const int   year,
                   int  *hd_elems,
             const int   fday,
             const int   count));
LOCAL void
hdy_no __P_((      Bool *init_data,
             const Bool  detected,
             const int   easter,
             const int   year,
                   int  *hd_elems,
             const int   fday,
             const int   count));
LOCAL void
hdy_pl __P_((      Bool *init_data,
             const Bool  detected,
             const int   easter,
             const int   year,
                   int  *hd_elems,
             const int   fday,
             const int   count));
LOCAL void
hdy_pt __P_((      Bool *init_data,
             const Bool  detected,
             const int   easter,
             const int   year,
                   int  *hd_elems,
             const int   fday,
             const int   count));
LOCAL void
hdy_se __P_((      Bool *init_data,
             const Bool  detected,
             const int   easter,
             const int   year,
                   int  *hd_elems,
             const int   fday,
             const int   count));
LOCAL void
hdy_si __P_((      Bool *init_data,
             const Bool  detected,
             const int   easter,
             const int   year,
                   int  *hd_elems,
             const int   fday,
             const int   count));
LOCAL void
hdy_th __P_((      Bool *init_data,
             const Bool  detected,
             const int   easter,
             const int   year,
                   int  *hd_elems,
             const int   fday,
             const int   count));
LOCAL void
hdy_us __P_((      Bool *init_data,
             const Bool  detected,
             const int   easter,
             const int   year,
                   int  *hd_elems,
             const int   fday,
             const int   count));
LOCAL void
do_all_saints_day __P_((      Bool  init_data,
                        const Bool  detected,
                        const char *country_code,
                        const char *holiday_prefix,
                              int   day,
                              int   month,
                        const int   year,
                              int  *hd_elems,
                        const int   fday,
                        const int   count));
LOCAL void
do_all_souls_day __P_((      Bool  init_data,
                       const Bool  detected,
                       const char *country_code,
                       const char *holiday_prefix,
                             int   day,
                             int   month,
                       const int   year,
                             int  *hd_elems,
                       const int   fday,
                       const int   count));
LOCAL void
do_armistice_day __P_((      Bool  init_data,
                       const Bool  detected,
                       const char *country_code,
                       const char *holiday_prefix,
                             int   day,
                             int   month,
                       const int   year,
                             int  *hd_elems,
                       const int   fday,
                       const int   count));
LOCAL void
do_ash_monday __P_((      Bool  init_data,
                    const Bool  detected,
                    const char *country_code,
                    const char *holiday_prefix,
                          int   day,
                          int   month,
                    const int   year,
                          int  *hd_elems,
                    const int   fday,
                    const int   count));
LOCAL void
do_ash_wednesday __P_((      Bool  init_data,
                       const Bool  detected,
                       const char *country_code,
                       const char *holiday_prefix,
                             int   day,
                             int   month,
                       const int   year,
                             int  *hd_elems,
                       const int   fday,
                       const int   count));
LOCAL void
do_bank_holiday __P_((      Bool  init_data,
                      const Bool  detected,
                      const char *country_code,
                      const char *holiday_prefix,
                            int   day,
                            int   month,
                      const int   year,
                            int  *hd_elems,
                      const int   fday,
                      const int   count));
LOCAL void
do_boxing_day __P_((      Bool  init_data,
                    const Bool  detected,
                    const char *country_code,
                    const char *holiday_prefix,
                          int   day,
                          int   month,
                    const int   year,
                          int  *hd_elems,
                    const int   fday,
                    const int   count));
LOCAL void
do_childrens_day __P_((      Bool  init_data,
                       const Bool  detected,
                       const char *country_code,
                       const char *holiday_prefix,
                             int   day,
                             int   month,
                       const int   year,
                             int  *hd_elems,
                       const int   fday,
                       const int   count));
LOCAL void
do_christmas_day __P_((      Bool  init_data,
                       const Bool  detected,
                       const char *country_code,
                       const char *holiday_prefix,
                             int   day,
                             int   month,
                       const int   year,
                             int  *hd_elems,
                       const int   fday,
                       const int   count));
LOCAL void
do_christmas_eve __P_((      Bool  init_data,
                       const Bool  detected,
                       const char *country_code,
                       const char *holiday_prefix,
                             int   day,
                             int   month,
                       const int   year,
                             int  *hd_elems,
                       const int   fday,
                       const int   count));
LOCAL void
do_christs_ascension_day __P_((      Bool  init_data,
                               const Bool  detected,
                               const char *country_code,
                               const char *holiday_prefix,
                                     int   day,
                                     int   month,
                               const int   year,
                                     int  *hd_elems,
                               const int   fday,
                               const int   count));
LOCAL void
do_constitution_anniversary __P_((      Bool  init_data,
                                  const Bool  detected,
                                  const char *country_code,
                                  const char *holiday_prefix,
                                        int   day,
                                        int   month,
                                  const int   year,
                                        int  *hd_elems,
                                  const int   fday,
                                  const int   count));
LOCAL void
do_easter_sunday __P_((      Bool  init_data,
                       const Bool  detected,
                       const char *country_code,
                       const char *holiday_prefix,
                             int   day,
                             int   month,
                       const int   year,
                             int  *hd_elems,
                       const int   fday,
                       const int   count));
LOCAL void
do_easter_monday __P_((      Bool  init_data,
                       const Bool  detected,
                       const char *country_code,
                       const char *holiday_prefix,
                             int   day,
                             int   month,
                       const int   year,
                             int  *hd_elems,
                       const int   fday,
                       const int   count));
LOCAL void
do_fathers_day __P_((      Bool  init_data,
                     const Bool  detected,
                     const char *country_code,
                     const char *holiday_prefix,
                           int   day,
                           int   month,
                     const int   year,
                           int  *hd_elems,
                     const int   fday,
                     const int   count));
LOCAL void
do_feast_of_corpus_christi __P_((      Bool  init_data,
                                 const Bool  detected,
                                 const char *country_code,
                                 const char *holiday_prefix,
                                       int   day,
                                       int   month,
                                 const int   year,
                                       int  *hd_elems,
                                 const int   fday,
                                 const int   count));
LOCAL void
do_good_friday __P_((      Bool  init_data,
                     const Bool  detected,
                     const char *country_code,
                     const char *holiday_prefix,
                           int   day,
                           int   month,
                     const int   year,
                           int  *hd_elems,
                     const int   fday,
                     const int   count));
LOCAL void
do_good_saturday __P_((      Bool  init_data,
                       const Bool  detected,
                       const char *country_code,
                       const char *holiday_prefix,
                             int   day,
                             int   month,
                       const int   year,
                             int  *hd_elems,
                       const int   fday,
                       const int   count));
LOCAL void
do_independence_day __P_((      Bool  init_data,
                          const Bool  detected,
                          const char *country_code,
                          const char *holiday_prefix,
                                int   day,
                                int   month,
                          const int   year,
                                int  *hd_elems,
                          const int   fday,
                          const int   count));
LOCAL void
do_johns_day __P_((      Bool  init_data,
                   const Bool  detected,
                   const char *country_code,
                   const char *holiday_prefix,
                         int   day,
                         int   month,
                   const int   year,
                         int  *hd_elems,
                   const int   fday,
                   const int   count));
LOCAL void
do_johns_eve __P_((      Bool  init_data,
                   const Bool  detected,
                   const char *country_code,
                   const char *holiday_prefix,
                         int   day,
                         int   month,
                   const int   year,
                         int  *hd_elems,
                   const int   fday,
                   const int   count));
LOCAL void
do_kings_birthday __P_((      Bool  init_data,
                        const Bool  detected,
                        const char *country_code,
                        const char *holiday_prefix,
                              int   day,
                              int   month,
                        const int   year,
                              int  *hd_elems,
                        const int   fday,
                        const int   count));
LOCAL void
do_labour_day __P_((      Bool  init_data,
                    const Bool  detected,
                    const char *country_code,
                    const char *holiday_prefix,
                          int   day,
                          int   month,
                    const int   year,
                          int  *hd_elems,
                    const int   fday,
                    const int   count));
LOCAL void
do_liberation_day __P_((      Bool  init_data,
                        const Bool  detected,
                        const char *country_code,
                        const char *holiday_prefix,
                              int   day,
                              int   month,
                        const int   year,
                              int  *hd_elems,
                        const int   fday,
                        const int   count));
LOCAL void
do_martinimas __P_((      Bool  init_data,
                    const Bool  detected,
                    const char *country_code,
                    const char *holiday_prefix,
                          int   day,
                          int   month,
                    const int   year,
                          int  *hd_elems,
                    const int   fday,
                    const int   count));
LOCAL void
do_marys_annunciation_day __P_((      Bool  init_data,
                                const Bool  detected,
                                const char *country_code,
                                const char *holiday_prefix,
                                      int   day,
                                      int   month,
                                const int   year,
                                      int  *hd_elems,
                                const int   fday,
                                const int   count));
LOCAL void
do_marys_ascension_day __P_((      Bool  init_data,
                             const Bool  detected,
                             const char *country_code,
                             const char *holiday_prefix,
                                   int   day,
                                   int   month,
                             const int   year,
                                   int  *hd_elems,
                             const int   fday,
                             const int   count));
LOCAL void
do_marys_immaculate_conception __P_((      Bool  init_data,
                                     const Bool  detected,
                                     const char *country_code,
                                     const char *holiday_prefix,
                                           int   day,
                                           int   month,
                                     const int   year,
                                           int  *hd_elems,
                                     const int   fday,
                                     const int   count));
LOCAL void
do_maundy_thursday __P_((      Bool  init_data,
                         const Bool  detected,
                         const char *country_code,
                         const char *holiday_prefix,
                               int   day,
                               int   month,
                         const int   year,
                               int  *hd_elems,
                         const int   fday,
                         const int   count));
LOCAL void
do_may_day __P_((      Bool  init_data,
                 const Bool  detected,
                 const char *country_code,
                 const char *holiday_prefix,
                       int   day,
                       int   month,
                 const int   year,
                       int  *hd_elems,
                 const int   fday,
                 const int   count));
LOCAL void
do_midsummer_day __P_((      Bool  init_data,
                       const Bool  detected,
                       const char *country_code,
                       const char *holiday_prefix,
                             int   day,
                             int   month,
                       const int   year,
                             int  *hd_elems,
                       const int   fday,
                       const int   count));
LOCAL void
do_midsummer_eve __P_((      Bool  init_data,
                       const Bool  detected,
                       const char *country_code,
                       const char *holiday_prefix,
                             int   day,
                             int   month,
                       const int   year,
                             int  *hd_elems,
                       const int   fday,
                       const int   count));
LOCAL void
do_mothers_day __P_((      Bool  init_data,
                     const Bool  detected,
                     const char *country_code,
                     const char *holiday_prefix,
                           int   day,
                           int   month,
                     const int   year,
                           int  *hd_elems,
                     const int   fday,
                     const int   count));
LOCAL void
do_national_day __P_((      Bool  init_data,
                      const Bool  detected,
                      const char *country_code,
                      const char *holiday_prefix,
                            int   day,
                            int   month,
                      const int   year,
                            int  *hd_elems,
                      const int   fday,
                      const int   count));
LOCAL void
do_new_years_day __P_((      Bool  init_data,
                       const Bool  detected,
                       const char *country_code,
                       const char *holiday_prefix,
                             int   day,
                             int   month,
                       const int   year,
                             int  *hd_elems,
                       const int   fday,
                       const int   count));
LOCAL void
do_palm_sunday __P_((      Bool  init_data,
                     const Bool  detected,
                     const char *country_code,
                     const char *holiday_prefix,
                           int   day,
                           int   month,
                     const int   year,
                           int  *hd_elems,
                     const int   fday,
                     const int   count));
LOCAL void
do_pentecost __P_((      Bool  init_data,
                   const Bool  detected,
                   const char *country_code,
                   const char *holiday_prefix,
                         int   day,
                         int   month,
                   const int   year,
                         int  *hd_elems,
                   const int   fday,
                   const int   count));
LOCAL void
do_reformation_day __P_((      Bool  init_data,
                         const Bool  detected,
                         const char *country_code,
                         const char *holiday_prefix,
                               int   day,
                               int   month,
                         const int   year,
                               int  *hd_elems,
                         const int   fday,
                         const int   count));
LOCAL void
do_remembrance_day __P_((      Bool  init_data,
                         const Bool  detected,
                         const char *country_code,
                         const char *holiday_prefix,
                               int   day,
                               int   month,
                         const int   year,
                               int  *hd_elems,
                         const int   fday,
                         const int   count));
LOCAL void
do_republic_day __P_((      Bool  init_data,
                      const Bool  detected,
                      const char *country_code,
                      const char *holiday_prefix,
                            int   day,
                            int   month,
                      const int   year,
                            int  *hd_elems,
                      const int   fday,
                      const int   count));
LOCAL void
do_shrove_tuesday __P_((      Bool  init_data,
                        const Bool  detected,
                        const char *country_code,
                        const char *holiday_prefix,
                              int   day,
                              int   month,
                        const int   year,
                              int  *hd_elems,
                        const int   fday,
                        const int   count));
LOCAL void
do_sylvester __P_((      Bool  init_data,
                   const Bool  detected,
                   const char *country_code,
                   const char *holiday_prefix,
                         int   day,
                         int   month,
                   const int   year,
                         int  *hd_elems,
                   const int   fday,
                   const int   count));
LOCAL void
do_thanksgiving __P_((      Bool  init_data,
                      const Bool  detected,
                      const char *country_code,
                      const char *holiday_prefix,
                            int   day,
                            int   month,
                      const int   year,
                            int  *hd_elems,
                      const int   fday,
                      const int   count));
LOCAL void
do_three_kings_day __P_((      Bool  init_data,
                         const Bool  detected,
                         const char *country_code,
                         const char *holiday_prefix,
                               int   day,
                               int   month,
                         const int   year,
                               int  *hd_elems,
                         const int   fday,
                         const int   count));
LOCAL void
do_queens_birthday __P_((      Bool  init_data,
                         const Bool  detected,
                         const char *country_code,
                         const char *holiday_prefix,
                               int   day,
                               int   month,
                         const int   year,
                               int  *hd_elems,
                         const int   fday,
                         const int   count));
LOCAL void
do_valentines_day __P_((      Bool  init_data,
                        const Bool  detected,
                        const char *country_code,
                        const char *holiday_prefix,
                              int   day,
                              int   month,
                        const int   year,
                              int  *hd_elems,
                        const int   fday,
                        const int   count));
LOCAL void
do_walpurgis_night __P_((      Bool  init_data,
                         const Bool  detected,
                         const char *country_code,
                         const char *holiday_prefix,
                               int   day,
                               int   month,
                         const int   year,
                               int  *hd_elems,
                         const int   fday,
                         const int   count));
LOCAL void
do_whit_monday __P_((      Bool  init_data,
                     const Bool  detected,
                     const char *country_code,
                     const char *holiday_prefix,
                           int   day,
                           int   month,
                     const int   year,
                           int  *hd_elems,
                     const int   fday,
                     const int   count));
LOCAL void
do_womens_day __P_((      Bool  init_data,
                    const Bool  detected,
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
IMPORT const int           dvec[];   /* Amount of days in months */
IMPORT       Greg_struct  *greg;     /* Points to the used Gregorian Reformation date */



/*
   Define public(extern) variables.
*/
/*
   The table used to manage the country specific holidays is a vector of
     `Df_struct' terminated by an element containing an `info' which is zero!
     The `id' entries in this table *MUST* be listed in ASCENDING SORT ORDER !!
*/
PUBLIC const Cc_struct  cc_holidays[]=
{
/*
  The A2 (2-letters) country codes are taken from ISO-3166.

  { char *id, char *info, void (*Cc_hdy_handler)() },
*/
#if USE_DE
  { CC_AT, OOE"stereichische",   hdy_at },
  { CC_AU, "Australische",       hdy_au },
  { CC_BE, "Belgische",          hdy_be },
  { CC_CA, "Kanadische",         hdy_ca },
  { CC_CH, "Schweizerische",     hdy_ch },
  { CC_CZ, "Tschechische",       hdy_cz },
  { CC_DE, "Deutsche",           hdy_de },
  { CC_DK, "D"AE"nische",        hdy_dk },
  { CC_ES, "Spanische",          hdy_es },
  { CC_FI, "Finnische",          hdy_fi },
  { CC_FR, "Franz"OE"sische",    hdy_fr },
  { CC_GB, "Britische",          hdy_gb },
  { CC_GR, "Griechische",        hdy_gr },
  { CC_HU, "Ungarische",         hdy_hu },
  { CC_IT, "Italienische",       hdy_it },
  { CC_MX, "Mexikanische",       hdy_mx },
  { CC_NL, "Niederl"AE"ndische", hdy_nl },
  { CC_NO, "Norwegische",        hdy_no },
  { CC_PL, "Polnische",          hdy_pl },
  { CC_PT, "Portugiesische",     hdy_pt },
  { CC_SE, "Schwedische",        hdy_se },
  { CC_SI, "Slowenische",        hdy_si },
  { CC_TH, "Thail"AE"ndische",   hdy_th },
  { CC_US, "US amerikanische",   hdy_us },
#else /* !USE_DE */
  { CC_AT, N_("Austrian"),       hdy_at },
  { CC_AU, N_("Australian"),     hdy_au },
  { CC_BE, N_("Belgian"),        hdy_be },
  { CC_CA, N_("Canadian"),       hdy_ca },
  { CC_CH, N_("Swiss"),          hdy_ch },
  { CC_CZ, N_("Czech"),          hdy_cz },
  { CC_DE, N_("German"),         hdy_de },
  { CC_DK, N_("Danish"),         hdy_dk },
  { CC_ES, N_("Spanish"),        hdy_es },
  { CC_FI, N_("Finnish"),        hdy_fi },
  { CC_FR, N_("French"),         hdy_fr },
  { CC_GB, N_("British"),        hdy_gb },
  { CC_GR, N_("Greek"),          hdy_gr },
  { CC_HU, N_("Hungarian"),      hdy_hu },
  { CC_IT, N_("Italian"),        hdy_it },
  { CC_MX, N_("Mexican"),        hdy_mx },
  { CC_NL, N_("Dutch"),          hdy_nl },
  { CC_NO, N_("Norwegian"),      hdy_no },
  { CC_PL, N_("Polish"),         hdy_pl },
  { CC_PT, N_("Portuguese"),     hdy_pt },
  { CC_SE, N_("Swedish"),        hdy_se },
  { CC_SI, N_("Slovenian"),      hdy_si },
  { CC_TH, N_("Thai"),           hdy_th },
  { CC_US, N_("U.S. American"),  hdy_us },
#endif /* !USE_DE */
  { NULL, NULL, NULL }
};



/*
   Define local(static) variables.
*/
LOCAL char  *ptr_cc_id;   /* The ISO-3166 character code used in the `hdy_CC()' functions. */



   PUBLIC const Cc_struct *
binsearch_cc_id (id)
   char *id;
/*
   Simple binary search the delivered `id' if it is listed in the global
     `cc_holidays' table.  The `id' is an ISO-3166 A2 (2-letter) country
     code.  If `id' is found in the table, this function returns a pointer
     to its complete record, otherwise NULL.
     *** This function *MUST* be placed in the same .c source file
     *** where the `cc_holidays' table is defined!!
*/
{
   auto     const Cc_struct  *ptr_cc;
   register       int         i;
   register       int         low=0;
   register       int         mid;
   register       int         high=(sizeof cc_holidays/sizeof(Cc_struct))-2;


   while (low <= high)
    {
      mid = ((low + high) >> 1);
      ptr_cc = cc_holidays + mid;
      i = strcmp(id, ptr_cc->id);
      if (i < 0)
        high = mid - 1;
      else
        if (i > 0)
          low = mid + 1;
        else
          return(ptr_cc);
    }

   return(NULL);
}



/*
*  All global holiday management functions.
*/



   PUBLIC void
hdy_standard (init_data, detected, year, hd_elems, fday, count)
         Bool *init_data;
   const Bool  detected;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Manages all standard holidays.
*/
{
#if USE_DE
   ptr_cc_id = "Std";
   print_single_holiday (*init_data, detected, "1'ter April", ptr_cc_id,
                         DIS_HLS_PREF, DAY_MIN, 4, year, hd_elems, fday, count);
#else /* !USE_DE */
   ptr_cc_id = _("Std");
   print_single_holiday (*init_data, detected, _("All Fool's Day"), ptr_cc_id,
                         DIS_HLS_PREF, DAY_MIN, 4, year, hd_elems, fday, count);
#endif /* !USE_DE */
   if (init_data)
     *init_data = FALSE;
   do_christmas_eve (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                     24, MONTH_MAX, year, hd_elems, fday, count);
   do_new_years_day (*init_data, detected, ptr_cc_id, "+",
                     DAY_MIN, MONTH_MIN, year, hd_elems, fday, count);
   do_sylvester (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                 dvec[MONTH_MAX-1], MONTH_MAX, year, hd_elems, fday, count);
}




   PUBLIC void
hdy_christian (init_data, detected, easter, year, hd_elems, fday, count)
         Bool *init_data;
   const Bool  detected;
   const int   easter;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Manages all Christian holidays.
*/
{
   register int  day;
#if USE_DE
   ptr_cc_id = "Chr";
   print_single_holiday (*init_data, detected, "Dreifaltigkeitsfest", ptr_cc_id,
                         DIS_HLS_PREF, easter+56, 0, year, hd_elems, fday, count);
   if (*init_data)
     *init_data = FALSE;
/*
   print_single_holiday (*init_data, detected, "Mari"AE" Lichtmess", ptr_cc_id,
                         DIS_HLS_PREF, 2, 2, year, hd_elems, fday, count);
   print_single_holiday (*init_data, detected, "Mari"AE" Heimsuchung", ptr_cc_id,
                         DIS_HLS_PREF, 2, 7, year, hd_elems, fday, count);
   print_single_holiday (*init_data, detected, "Mari"AE" Geburt", ptr_cc_id,
                         DIS_HLS_PREF, 8, 9, year, hd_elems, fday, count);
   print_single_holiday (*init_data, detected, "Mari"AE" Namen", ptr_cc_id,
                         DIS_HLS_PREF, 12, 9, year, hd_elems, fday, count);
   print_single_holiday (*init_data, detected, "Mari"AE" Mutterschaft", ptr_cc_id,
                         DIS_HLS_PREF, 11, 10, year, hd_elems, fday, count);
   print_single_holiday (*init_data, detected, "Mari"AE" Opferung", ptr_cc_id,
                         DIS_HLS_PREF, 21, 11, year, hd_elems, fday, count);
   print_single_holiday (*init_data, detected, "Mari"AE" Erwartung", ptr_cc_id,
                         DIS_HLS_PREF, 18, 12, year, hd_elems, fday, count);
*/
   print_single_holiday (*init_data, detected, "St. Nikolaustag", ptr_cc_id,
                         DIS_HLS_PREF, 6, MONTH_MAX, year, hd_elems, fday, count);
   day = eval_holiday (24, MONTH_MAX, year, DAY_MAX, FALSE);
   print_single_holiday (*init_data, detected, "4'ter Advent", ptr_cc_id,
                         DIS_HLS_PREF, day, MONTH_MAX, year, hd_elems, fday, count);
   day -= DAY_MAX;
   print_single_holiday (*init_data, detected, "3'ter Advent", ptr_cc_id,
                         DIS_HLS_PREF, day, MONTH_MAX, year, hd_elems, fday, count);
   day -= DAY_MAX;
   print_single_holiday (*init_data, detected, "2'ter Advent", ptr_cc_id,
                         DIS_HLS_PREF, day, MONTH_MAX, year, hd_elems, fday, count);
   day -= DAY_MAX;
   if (day > 0)
     print_single_holiday (*init_data, detected, "1'ter Advent", ptr_cc_id,
                           DIS_HLS_PREF, day, MONTH_MAX, year, hd_elems, fday, count);
   else
     print_single_holiday (*init_data, detected, "1'ter Advent", ptr_cc_id,
                           DIS_HLS_PREF, dvec[11-1]+day, 11, year, hd_elems, fday, count);
#else /* !USE_DE */
   ptr_cc_id = _("Chr");
   print_single_holiday (*init_data, detected, _("Trinity Sunday"), ptr_cc_id,
                         DIS_HLS_PREF, easter+56, 0, year, hd_elems, fday, count);
   if (*init_data)
     *init_data = FALSE;
   print_single_holiday (*init_data, detected, _("St. Nicholas' Day"), ptr_cc_id,
                         DIS_HLS_PREF, 6, MONTH_MAX, year, hd_elems, fday, count);
   day = eval_holiday (24, MONTH_MAX, year, DAY_MAX, FALSE);
   print_single_holiday (*init_data, detected, _("4th Advent"), ptr_cc_id,
                         DIS_HLS_PREF, day, MONTH_MAX, year, hd_elems, fday, count);
   day -= DAY_MAX;
   print_single_holiday (*init_data, detected, _("3rd Advent"), ptr_cc_id,
                         DIS_HLS_PREF, day, MONTH_MAX, year, hd_elems, fday, count);
   day -= DAY_MAX;
   print_single_holiday (*init_data, detected, _("2nd Advent"), ptr_cc_id,
                         DIS_HLS_PREF, day, MONTH_MAX, year, hd_elems, fday, count);
   day -= DAY_MAX;
   if (day > 0)
     print_single_holiday (*init_data, detected, _("1st Advent"), ptr_cc_id,
                           DIS_HLS_PREF, day, MONTH_MAX, year, hd_elems, fday, count);
   else
     print_single_holiday (*init_data, detected, _("1st Advent"), ptr_cc_id,
                           DIS_HLS_PREF, dvec[11-1]+day, 11, year, hd_elems, fday, count);
#endif /* !USE_DE */
   do_all_saints_day (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                      DAY_MIN, 11, year, hd_elems, fday, count);
   do_all_souls_day (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                     2, 11, year, hd_elems, fday, count);
   do_ash_wednesday (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                     easter-46, 0, year, hd_elems, fday, count);
   do_boxing_day (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                  26, MONTH_MAX, year, hd_elems, fday, count);
   do_christmas_day (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                     25, MONTH_MAX, year, hd_elems, fday, count);
   do_christmas_eve (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                     24, MONTH_MAX, year, hd_elems, fday, count);
   do_christs_ascension_day (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                             easter+39, 0, year, hd_elems, fday, count);
   do_easter_monday (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                     easter+1, 0, year, hd_elems, fday, count);
   do_easter_sunday (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                     easter, 0, year, hd_elems, fday, count);
   do_feast_of_corpus_christi (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                               easter+60, 0, year, hd_elems, fday, count);
   do_good_friday (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                   easter-2, 0, year, hd_elems, fday, count);
   do_good_saturday (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                     easter-1, 0, year, hd_elems, fday, count);
   do_martinimas (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                  11, 11, year, hd_elems, fday, count);
   do_marys_annunciation_day (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                              25, 3, year, hd_elems, fday, count);
   do_marys_ascension_day (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                           15, 8, year, hd_elems, fday, count);
   do_marys_immaculate_conception (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                                   8, MONTH_MAX, year, hd_elems, fday, count);
   do_maundy_thursday (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                       easter-3, 0, year, hd_elems, fday, count);
   do_palm_sunday (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                   easter-DAY_MAX, 0, year, hd_elems, fday, count);
   do_pentecost (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                 easter+49, 0, year, hd_elems, fday, count);
   do_reformation_day (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                       dvec[10-1], 10, year, hd_elems, fday, count);
   do_sylvester (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                 dvec[MONTH_MAX-1], MONTH_MAX, year, hd_elems, fday, count);
   do_three_kings_day (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                       6, MONTH_MIN, year, hd_elems, fday, count);
   do_valentines_day (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                      14, 2, year, hd_elems, fday, count);
   do_whit_monday (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                   easter+50, 0, year, hd_elems, fday, count);
}



/*
*  All country specific holiday management functions.
*/



   LOCAL void
hdy_at (init_data, detected, easter, year, hd_elems, fday, count)
         Bool *init_data;
   const Bool  detected;
   const int   easter;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Manages all specific holidays celebrated in Austria.
*/
{
   ptr_cc_id = CC_AT;
   do_all_saints_day (*init_data, detected, ptr_cc_id, "+",
                      DAY_MIN, 11, year, hd_elems, fday, count);
   if (*init_data)
     *init_data = FALSE;
   do_all_souls_day (*init_data, detected, ptr_cc_id, DIS_HLS_PREF2,
                     2, 11, year, hd_elems, fday, count);
   do_boxing_day (*init_data, detected, ptr_cc_id, "+",
                  26, MONTH_MAX, year, hd_elems, fday, count);
   do_christmas_day (*init_data, detected, ptr_cc_id, "+",
                     25, MONTH_MAX, year, hd_elems, fday, count);
   do_christs_ascension_day (*init_data, detected, ptr_cc_id, "+",
                             easter+39, 0, year, hd_elems, fday, count);
   do_easter_monday (*init_data, detected, ptr_cc_id, "+",
                     easter+1, 0, year, hd_elems, fday, count);
   do_easter_sunday (*init_data, detected, ptr_cc_id, "+",
                     easter, 0, year, hd_elems, fday, count);
   do_feast_of_corpus_christi (*init_data, detected, ptr_cc_id, "+",
                               easter+60, 0, year, hd_elems, fday, count);
   do_labour_day (*init_data, detected, ptr_cc_id, "+",
                  DAY_MIN, 5, year, hd_elems, fday, count);
   do_marys_ascension_day (*init_data, detected, ptr_cc_id, "+",
                           15, 8, year, hd_elems, fday, count);
   do_marys_immaculate_conception (*init_data, detected, ptr_cc_id, "+",
                                   8, MONTH_MAX, year, hd_elems, fday, count);
   do_national_day (*init_data, detected, ptr_cc_id, "+",
                    26, 10, year, hd_elems, fday, count);
   do_pentecost (*init_data, detected, ptr_cc_id, "+",
                 easter+49, 0, year, hd_elems, fday, count);
   do_three_kings_day (*init_data, detected, ptr_cc_id, "+",
                       6, MONTH_MIN, year, hd_elems, fday, count);
   do_whit_monday (*init_data, detected, ptr_cc_id, "+",
                   easter+50, 0, year, hd_elems, fday, count);
}



   LOCAL void
hdy_au (init_data, detected, easter, year, hd_elems, fday, count)
         Bool *init_data;
   const Bool  detected;
   const int   easter;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Manages all specific holidays celebrated in Australia.
*/
{
   register int  day;


   ptr_cc_id = CC_AU;
#if USE_DE
   print_single_holiday (*init_data, detected, "Anzac Tag", ptr_cc_id,
                         "+", 25, 4, year, hd_elems, fday, count);
#else /* !USE_DE */
   print_single_holiday (*init_data, detected, _("Anzac Day"), ptr_cc_id,
                         "+", 25, 4, year, hd_elems, fday, count);
#endif /* !USE_DE */
   if (*init_data)
     *init_data = FALSE;
   do_bank_holiday (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                    6, 8, year, hd_elems, fday, count);
   do_boxing_day (*init_data, detected, ptr_cc_id, "+",
                  26, MONTH_MAX, year, hd_elems, fday, count);
   do_christmas_day (*init_data, detected, ptr_cc_id, "+",
                     25, MONTH_MAX, year, hd_elems, fday, count);
   do_easter_monday (*init_data, detected, ptr_cc_id, "+",
                     easter+1, 0, year, hd_elems, fday, count);
   do_easter_sunday (*init_data, detected, ptr_cc_id, "+",
                     easter, 0, year, hd_elems, fday, count);
   do_good_friday (*init_data, detected, ptr_cc_id, "+",
                   easter-2, 0, year, hd_elems, fday, count);
   do_good_saturday (*init_data, detected, ptr_cc_id, "+",
                     easter-1, 0, year, hd_elems, fday, count);
   day = eval_holiday (DAY_MIN, 10, year, DAY_MIN, TRUE);
   do_labour_day (*init_data, detected, ptr_cc_id, "+",
                  day, 10, year, hd_elems, fday, count);
   day = eval_holiday (DAY_MIN, 5, year, DAY_MIN, TRUE);
   do_may_day (*init_data, detected, ptr_cc_id, "*",
               day, 5, year, hd_elems, fday, count);
   do_national_day (*init_data, detected, ptr_cc_id, "+",
                    26, MONTH_MIN, year, hd_elems, fday, count);
   do_queens_birthday (*init_data, detected, ptr_cc_id, "+",
                       10, 6, year, hd_elems, fday, count);
}



   LOCAL void
hdy_be (init_data, detected, easter, year, hd_elems, fday, count)
         Bool *init_data;
   const Bool  detected;
   const int   easter;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Manages all specific holidays celebrated in Belgium.
*/
{
   ptr_cc_id = CC_BE;
#if USE_DE
   print_single_holiday (*init_data, detected, "Fl"AE"mischer Kulturtag", ptr_cc_id,
                         "+", 11, 7, year, hd_elems, fday, count);
   if (*init_data)
     *init_data = FALSE;
   print_single_holiday (*init_data, detected, "Franz"OE"sischer Kulturtag", ptr_cc_id,
                         "+", 27, 9, year, hd_elems, fday, count);
   print_single_holiday (*init_data, detected, "Tag des Herrscherhauses", ptr_cc_id,
                         DIS_HLS_PREF, 15, 11, year, hd_elems, fday, count);
#else /* !USE_DE */
   print_single_holiday (*init_data, detected, _("Flemish Culture Day"), ptr_cc_id,
                         "+", 11, 7, year, hd_elems, fday, count);
   if (*init_data)
     *init_data = FALSE;
   print_single_holiday (*init_data, detected, _("French Culture Day"), ptr_cc_id,
                         "+", 27, 9, year, hd_elems, fday, count);
   print_single_holiday (*init_data, detected, _("Dynasty Day"), ptr_cc_id,
                         DIS_HLS_PREF, 15, 11, year, hd_elems, fday, count);
#endif /* !USE_DE */
   do_all_saints_day (*init_data, detected, ptr_cc_id, "+",
                      DAY_MIN, 11, year, hd_elems, fday, count);
   if (year > 1917)
     do_armistice_day (*init_data, detected, ptr_cc_id, "+",
                       11, 11, year, hd_elems, fday, count);
   do_christmas_day (*init_data, detected, ptr_cc_id, "+",
                     25, MONTH_MAX, year, hd_elems, fday, count);
   do_christs_ascension_day (*init_data, detected, ptr_cc_id, "+",
                             easter+39, 0, year, hd_elems, fday, count);
   do_easter_monday (*init_data, detected, ptr_cc_id, "+",
                     easter+1, 0, year, hd_elems, fday, count);
   do_easter_sunday (*init_data, detected, ptr_cc_id, "+",
                     easter, 0, year, hd_elems, fday, count);
   do_labour_day (*init_data, detected, ptr_cc_id, "+",
                  DAY_MIN, 5, year, hd_elems, fday, count);
   do_marys_ascension_day (*init_data, detected, ptr_cc_id, "+",
                           15, 8, year, hd_elems, fday, count);
   do_national_day (*init_data, detected, ptr_cc_id, "+",
                    21, 7, year, hd_elems, fday, count);
   do_pentecost (*init_data, detected, ptr_cc_id, "+",
                 easter+49, 0, year, hd_elems, fday, count);
   do_whit_monday (*init_data, detected, ptr_cc_id, "+",
                   easter+50, 0, year, hd_elems, fday, count);
}



   LOCAL void
hdy_ca (init_data, detected, easter, year, hd_elems, fday, count)
         Bool *init_data;
   const Bool  detected;
   const int   easter;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Manages all specific holidays celebrated in Canada.
*/
{
   register int  day;


   ptr_cc_id = CC_CA;
#if USE_DE
   day = eval_holiday (DAY_MIN, 5, year, DAY_MIN, TRUE);
   print_single_holiday (*init_data, detected, "Victoriatag", ptr_cc_id,
                         "+", day+(2*DAY_MAX), 5, year, hd_elems, fday, count);
   if (*init_data)
     *init_data = FALSE;
   print_single_holiday (*init_data, detected, "St. Jean Baptiste Tag", ptr_cc_id,
                         "+", 24, 6, year, hd_elems, fday, count);
#else /* !USE_DE */
   day = eval_holiday (DAY_MIN, 5, year, DAY_MIN, TRUE);
   print_single_holiday (*init_data, detected, _("Victoria Day"), ptr_cc_id,
                         "+", day+(2*DAY_MAX), 5, year, hd_elems, fday, count);
   if (*init_data)
     *init_data = FALSE;
   print_single_holiday (*init_data, detected, _("St. Jean Baptiste Day"), ptr_cc_id,
                         "+", 24, 6, year, hd_elems, fday, count);
#endif /* !USE_DE */
   do_boxing_day (*init_data, detected, ptr_cc_id, "+",
                  26, MONTH_MAX, year, hd_elems, fday, count);
   do_christmas_day (*init_data, detected, ptr_cc_id, "+",
                     25, MONTH_MAX, year, hd_elems, fday, count);
   do_easter_monday (*init_data, detected, ptr_cc_id, "+",
                     easter+1, 0, year, hd_elems, fday, count);
   do_easter_sunday (*init_data, detected, ptr_cc_id, "+",
                     easter, 0, year, hd_elems, fday, count);
   day = eval_holiday (DAY_MIN, 6, year, DAY_MAX, TRUE);
   do_fathers_day (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                   day+(2*DAY_MAX), 6, year, hd_elems, fday, count);
   do_good_friday (*init_data, detected, ptr_cc_id, "+",
                   easter-2, 0, year, hd_elems, fday, count);
   day = eval_holiday (DAY_MIN, 9, year, DAY_MIN, TRUE);
   do_labour_day (*init_data, detected, ptr_cc_id, "+",
                  day, 9, year, hd_elems, fday, count);
   day = eval_holiday (DAY_MIN, 5, year, DAY_MAX, TRUE);
   do_mothers_day (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                   day+DAY_MAX, 5, year, hd_elems, fday, count);
   day = weekday_of_date (DAY_MIN, 7, year);
   if (day > 5)
     day = (DAY_MAX - day) + 2;
   else
     day = 1;
   do_national_day (*init_data, detected, ptr_cc_id, "+",
                    day, 7, year, hd_elems, fday, count);
   do_remembrance_day (*init_data, detected, ptr_cc_id, "+",
                       11, 11, year, hd_elems, fday, count);
   day = eval_holiday (DAY_MIN, 10, year, DAY_MIN, TRUE);
   do_thanksgiving (*init_data, detected, ptr_cc_id, "+",
                    day+DAY_MAX, 10, year, hd_elems, fday, count);
}



   LOCAL void
hdy_ch (init_data, detected, easter, year, hd_elems, fday, count)
         Bool *init_data;
   const Bool  detected;
   const int   easter;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Manages all specific holidays celebrated in Switzerland.
*/
{
   ptr_cc_id = CC_CH;
#if USE_DE
   print_single_holiday (*init_data, detected, "St. Berchtoldstag", ptr_cc_id,
                         DIS_HLS_PREF, 2, MONTH_MIN, year, hd_elems, fday, count);
#else /* !USE_DE */
   print_single_holiday (*init_data, detected, _("St. Berchtold's Day"), ptr_cc_id,
                         DIS_HLS_PREF, 2, MONTH_MIN, year, hd_elems, fday, count);
#endif /* !USE_DE */
   if (*init_data)
     *init_data = FALSE;
   do_all_saints_day (*init_data, detected, ptr_cc_id, "+",
                      DAY_MIN, 11, year, hd_elems, fday, count);
   do_christs_ascension_day (*init_data, detected, ptr_cc_id, "+",
                             easter+39, 0, year, hd_elems, fday, count);
   do_christmas_day (*init_data, detected, ptr_cc_id, "+",
                     25, MONTH_MAX, year, hd_elems, fday, count);
   do_easter_monday (*init_data, detected, ptr_cc_id, "+",
                     easter+1, 0, year, hd_elems, fday, count);
   do_easter_sunday (*init_data, detected, ptr_cc_id, "+",
                     easter, 0, year, hd_elems, fday, count);
   do_feast_of_corpus_christi (*init_data, detected, ptr_cc_id, "+",
                               easter+60, 0, year, hd_elems, fday, count);
   do_good_friday (*init_data, detected, ptr_cc_id, "+",
                   easter-2, 0, year, hd_elems, fday, count);
   do_marys_ascension_day (*init_data, detected, ptr_cc_id, "+",
                           15, 8, year, hd_elems, fday, count);
   if (year > 1992)
     do_national_day (*init_data, detected, ptr_cc_id, "+",
                      DAY_MIN, 8, year, hd_elems, fday, count);
   do_pentecost (*init_data, detected, ptr_cc_id, "+",
                 easter+49, 0, year, hd_elems, fday, count);
   do_whit_monday (*init_data, detected, ptr_cc_id, "+",
                   easter+50, 0, year, hd_elems, fday, count);
}



   LOCAL void
hdy_cz (init_data, detected, easter, year, hd_elems, fday, count)
         Bool *init_data;
   const Bool  detected;
   const int   easter;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Manages all specific holidays celebrated in Czech Republic.
*/
{
   register int  day;


   ptr_cc_id = CC_CZ;
#if USE_DE
   if (year > 1944)
    {
      print_single_holiday (*init_data, detected, "Maiaufstand der Tschechen", ptr_cc_id,
                            DIS_HLS_PREF, 5, 5, year, hd_elems, fday, count);
      if (*init_data)
        *init_data = FALSE;
    }
   print_single_holiday (*init_data, detected, "Tag der Slavischen Missionierung", ptr_cc_id,
                         "+", 5, 7, year, hd_elems, fday, count);
   if (*init_data)
     *init_data = FALSE;
   if (year > 1414)
     print_single_holiday (*init_data, detected, "Verbrennung des Jan Hus", ptr_cc_id,
                           "+", 6, 7, year, hd_elems, fday, count);
   print_single_holiday (*init_data, detected, "Tag der Studentenrevolte", ptr_cc_id,
                         DIS_HLS_PREF, 17, 11, year, hd_elems, fday, count);
#else /* !USE_DE */
   if (year > 1944)
    {
      print_single_holiday (*init_data, detected, _("Czech People's May Uprising"), ptr_cc_id,
                            DIS_HLS_PREF, 5, 5, year, hd_elems, fday, count);
      if (*init_data)
        *init_data = FALSE;
    }
   print_single_holiday (*init_data, detected, _("Slav Missionaries Method's Day"), ptr_cc_id,
                         "+", 5, 7, year, hd_elems, fday, count);
   if (*init_data)
     *init_data = FALSE;
   if (year > 1414)
     print_single_holiday (*init_data, detected, _("Burning of Jan Hus"), ptr_cc_id,
                           "+", 5, 7, year, hd_elems, fday, count);
   print_single_holiday (*init_data, detected, _("Day of Students Revolt"), ptr_cc_id,
                         DIS_HLS_PREF, 17, 11, year, hd_elems, fday, count);
#endif /* !USE_DE */
   do_boxing_day (*init_data, detected, ptr_cc_id, "+",
                  26, MONTH_MAX, year, hd_elems, fday, count);
   if (*init_data)
     *init_data = FALSE;
   do_christmas_day (*init_data, detected, ptr_cc_id, "+",
                     25, MONTH_MAX, year, hd_elems, fday, count);
   do_christmas_eve (*init_data, detected, ptr_cc_id, "+",
                     24, MONTH_MAX, year, hd_elems, fday, count);
   do_easter_monday (*init_data, detected, ptr_cc_id, "+",
                     easter+1, 0, year, hd_elems, fday, count);
   do_easter_sunday (*init_data, detected, ptr_cc_id, "+",
                     easter, 0, year, hd_elems, fday, count);
   if (year > 1917)
     do_independence_day (*init_data, detected, ptr_cc_id, "+",
                          28, 10, year, hd_elems, fday, count);
   do_labour_day (*init_data, detected, ptr_cc_id, "+",
                  DAY_MIN, 5, year, hd_elems, fday, count);
   if (year > 1944)
     do_liberation_day (*init_data, detected, ptr_cc_id, "+",
                        8, 5, year, hd_elems, fday, count);
   day = eval_holiday (DAY_MIN, 5, year, DAY_MAX, TRUE);
   do_mothers_day (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                   day+DAY_MAX, 5, year, hd_elems, fday, count);
}



   LOCAL void
hdy_de (init_data, detected, easter, year, hd_elems, fday, count)
         Bool *init_data;
   const Bool  detected;
   const int   easter;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Manages all specific holidays celebrated in Germany.
*/
{
   register int  day;


   ptr_cc_id = CC_DE;
#if USE_DE
   print_single_holiday (*init_data, detected, "Weiberfastnacht", ptr_cc_id,
                         DIS_HLS_PREF, easter-52, 0, year, hd_elems, fday, count);
   if (*init_data)
     *init_data = FALSE;
   if (year >= 1990)
     print_single_holiday (*init_data, detected, "Tag der Deutschen Einheit", ptr_cc_id,
                           "+", 3, 10, year, hd_elems, fday, count);
   else
     if (year > 1953)
       print_single_holiday (*init_data, detected, "Tag der Deutschen Einheit", ptr_cc_id,
                             "+", 17, 6, year, hd_elems, fday, count);
   day = weekday_of_date (DAY_MIN, 11, year);
   day = (day<3) ? 18-day : 25-day;
   print_single_holiday (*init_data, detected, "Bu"SZ"- und Bettag", ptr_cc_id,
                         (year>1994) ? DIS_HLS_PREF: "+", day, 11, year, hd_elems, fday, count);
   day = eval_holiday (24, MONTH_MAX, year, DAY_MAX, FALSE);
   day -= (DAY_MAX * 4);
   print_single_holiday (*init_data, detected, "Totensonntag", ptr_cc_id,
                         DIS_HLS_PREF, dvec[11-1]+day, 11, year, hd_elems, fday, count);
   day -= DAY_MAX;
   print_single_holiday (*init_data, detected, "Volkstrauertag", ptr_cc_id,
                         DIS_HLS_PREF, dvec[11-1]+day, 11, year, hd_elems, fday, count);
#else /* !USE_DE */
   print_single_holiday (*init_data, detected, _("Women's shrove day"), ptr_cc_id,
                         DIS_HLS_PREF, easter-52, 0, year, hd_elems, fday, count);
   if (*init_data)
     *init_data = FALSE;
   if (year >= 1990)
     print_single_holiday (*init_data, detected, _("German Unity Day"), ptr_cc_id,
                           "+", 3, 10, year, hd_elems, fday, count);
   else
     if (year > 1953)
       print_single_holiday (*init_data, detected, _("German Unity Day"), ptr_cc_id,
                             "+", 17, 6, year, hd_elems, fday, count);
   day = weekday_of_date (DAY_MIN, 11, year);
   day = (day<3) ? 18-day : 25-day;
   print_single_holiday (*init_data, detected, _("Day of Prayer and Repentance"), ptr_cc_id,
                         (year>1994) ? DIS_HLS_PREF: "+", day, 11, year, hd_elems, fday, count);
   day = eval_holiday (24, MONTH_MAX, year, DAY_MAX, FALSE);
   day -= (DAY_MAX * 4);
   print_single_holiday (*init_data, detected, _("Death Sunday"), ptr_cc_id,
                         DIS_HLS_PREF, dvec[11-1]+day, 11, year, hd_elems, fday, count);
   day -= DAY_MAX;
   print_single_holiday (*init_data, detected, _("National Affliction Day"), ptr_cc_id,
                            DIS_HLS_PREF, dvec[11-1]+day, 11, year, hd_elems, fday, count);
#endif /* !USE_DE */
   do_all_saints_day (*init_data, detected, ptr_cc_id, "#",
                      DAY_MIN, 11, year, hd_elems, fday, count);
   do_all_souls_day (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                     2, 11, year, hd_elems, fday, count);
   do_ash_monday (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                  easter-48, 0, year, hd_elems, fday, count);
   do_ash_wednesday (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                     easter-46, 0, year, hd_elems, fday, count);
   do_boxing_day (*init_data, detected, ptr_cc_id, "+",
                  26, MONTH_MAX, year, hd_elems, fday, count);
   do_christs_ascension_day (*init_data, detected, ptr_cc_id, "#",
                             easter+39, 0, year, hd_elems, fday, count);
   do_christmas_day (*init_data, detected, ptr_cc_id, "+",
                     25, MONTH_MAX, year, hd_elems, fday, count);
   do_easter_monday (*init_data, detected, ptr_cc_id, "+",
                     easter+1, 0, year, hd_elems, fday, count);
   do_easter_sunday (*init_data, detected, ptr_cc_id, "+",
                     easter, 0, year, hd_elems, fday, count);
   do_fathers_day (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                   easter+39, 0, year, hd_elems, fday, count);
   do_feast_of_corpus_christi (*init_data, detected, ptr_cc_id, "#",
                               easter+60, 0, year, hd_elems, fday, count);
   do_good_friday (*init_data, detected, ptr_cc_id, "+",
                   easter-2, 0, year, hd_elems, fday, count);
   do_labour_day (*init_data, detected, ptr_cc_id, "+",
                  DAY_MIN, 5, year, hd_elems, fday, count);
   do_marys_ascension_day (*init_data, detected, ptr_cc_id, DIS_HLS_PREF2,
                           15, 8, year, hd_elems, fday, count);
   day = eval_holiday (DAY_MIN, 5, year, DAY_MAX, TRUE);
   if (day_of_year (day, 5, year) != easter+49)
     day += DAY_MAX;
   do_mothers_day (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                   day, 5, year, hd_elems, fday, count);
   do_pentecost (*init_data, detected, ptr_cc_id, "+",
                 easter+49, 0, year, hd_elems, fday, count);
   do_shrove_tuesday (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                      easter-47, 0, year, hd_elems, fday, count);
   day = eval_holiday (DAY_MIN, 10, year, DAY_MAX, TRUE);
   do_thanksgiving (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                    day, 10, year, hd_elems, fday, count);
   do_three_kings_day (*init_data, detected, ptr_cc_id, DIS_HLS_PREF2,
                       6, MONTH_MIN, year, hd_elems, fday, count);
   do_valentines_day (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                      14, 2, year, hd_elems, fday, count);
   do_walpurgis_night (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                       dvec[4-1], 4, year, hd_elems, fday, count);
   do_whit_monday (*init_data, detected, ptr_cc_id, "+",
                   easter+50, 0, year, hd_elems, fday, count);
}



   LOCAL void
hdy_dk (init_data, detected, easter, year, hd_elems, fday, count)
         Bool *init_data;
   const Bool  detected;
   const int   easter;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Manages all specific holidays celebrated in Denmark.
*/
{
   register int  day;


   ptr_cc_id = CC_DK;
#if USE_DE
   print_single_holiday (*init_data, detected, "Bettag", ptr_cc_id,
                         "+", easter+26, 0, year, hd_elems, fday, count);
#else /* !USE_DE */
   print_single_holiday (*init_data, detected, _("Prayer Day"), ptr_cc_id,
                         "+", easter+26, 0, year, hd_elems, fday, count);
#endif /* !USE_DE */
   if (*init_data)
     *init_data = FALSE;
   do_boxing_day (*init_data, detected, ptr_cc_id, "+",
                  26, MONTH_MAX, year, hd_elems, fday, count);
   do_christs_ascension_day (*init_data, detected, ptr_cc_id, "+",
                             easter+39, 0, year, hd_elems, fday, count);
   do_christmas_day (*init_data, detected, ptr_cc_id, "+",
                     25, MONTH_MAX, year, hd_elems, fday, count);
   do_constitution_anniversary (*init_data, detected, ptr_cc_id, "+",
                                5, 6, year, hd_elems, fday, count);
   do_easter_monday (*init_data, detected, ptr_cc_id, "+",
                     easter+1, 0, year, hd_elems, fday, count);
   do_easter_sunday (*init_data, detected, ptr_cc_id, "+",
                     easter, 0, year, hd_elems, fday, count);
   if (year > 1844)
     do_fathers_day (*init_data, detected, ptr_cc_id, "#",
                     5, 6, year, hd_elems, fday, count);
   do_feast_of_corpus_christi (*init_data, detected, ptr_cc_id, "+",
                               easter+60, 0, year, hd_elems, fday, count);
   do_good_friday (*init_data, detected, ptr_cc_id, "+",
                   easter-2, 0, year, hd_elems, fday, count);
   do_johns_day (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                 24, 6, year, hd_elems, fday, count);
   do_johns_eve (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                 23, 6, year, hd_elems, fday, count);
   do_labour_day (*init_data, detected, ptr_cc_id, "+",
                  DAY_MIN, 5, year, hd_elems, fday, count);
   do_martinimas (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                  11, 11, year, hd_elems, fday, count);
   do_maundy_thursday (*init_data, detected, ptr_cc_id, "+",
                       easter-3, 0, year, hd_elems, fday, count);
   day = eval_holiday (DAY_MIN, 5, year, DAY_MAX, TRUE);
   if (day_of_year (day, 5, year) != easter+49)
     day += DAY_MAX;
   do_mothers_day (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                   day, 5, year, hd_elems, fday, count);
   do_palm_sunday (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                   easter-DAY_MAX, 0, year, hd_elems, fday, count);
   do_pentecost (*init_data, detected, ptr_cc_id, "+",
                 easter+49, 0, year, hd_elems, fday, count);
   do_three_kings_day (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                       6, MONTH_MIN, year, hd_elems, fday, count);
   do_whit_monday (*init_data, detected, ptr_cc_id, "+",
                   easter+50, 0, year, hd_elems, fday, count);
}



   LOCAL void
hdy_es (init_data, detected, easter, year, hd_elems, fday, count)
         Bool *init_data;
   const Bool  detected;
   const int   easter;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Manages all specific holidays celebrated in Spain.
*/
{
   ptr_cc_id = CC_ES;
#if USE_DE
   print_single_holiday (*init_data, detected, "St. Josefstag", ptr_cc_id,
                         DIS_HLS_PREF, 19, 3, year, hd_elems, fday, count);
   if (*init_data)
     *init_data = FALSE;
   print_single_holiday (*init_data, detected, "St. Jamestag", ptr_cc_id,
                         DIS_HLS_PREF, 25, 7, year, hd_elems, fday, count);
#else /* !USE_DE */
   print_single_holiday (*init_data, detected, _("St. Josef's Day"), ptr_cc_id,
                         DIS_HLS_PREF, 19, 3, year, hd_elems, fday, count);
   if (*init_data)
     *init_data = FALSE;
   print_single_holiday (*init_data, detected, _("St. James' Day"), ptr_cc_id,
                         DIS_HLS_PREF, 25, 7, year, hd_elems, fday, count);
#endif /* !USE_DE */
   do_all_saints_day (*init_data, detected, ptr_cc_id, "+",
                      DAY_MIN, 11, year, hd_elems, fday, count);
   do_boxing_day (*init_data, detected, ptr_cc_id, "+",
                  26, MONTH_MAX, year, hd_elems, fday, count);
   do_christmas_day (*init_data, detected, ptr_cc_id, "+",
                     25, MONTH_MAX, year, hd_elems, fday, count);
   do_constitution_anniversary (*init_data, detected, ptr_cc_id, "+",
                                6, MONTH_MAX, year, hd_elems, fday, count);
   do_easter_sunday (*init_data, detected, ptr_cc_id, "+",
                     easter, 0, year, hd_elems, fday, count);
   do_good_friday (*init_data, detected, ptr_cc_id, "+",
                   easter-2, 0, year, hd_elems, fday, count);
   do_kings_birthday (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                      24, 6, year, hd_elems, fday, count);
   do_labour_day (*init_data, detected, ptr_cc_id, "+",
                  DAY_MIN, 5, year, hd_elems, fday, count);
   do_marys_ascension_day (*init_data, detected, ptr_cc_id, "+",
                           15, 8, year, hd_elems, fday, count);
   do_marys_immaculate_conception (*init_data, detected, ptr_cc_id, "+",
                                   8, MONTH_MAX, year, hd_elems, fday, count);
   do_national_day (*init_data, detected, ptr_cc_id, "+",
                    12, 10, year, hd_elems, fday, count);
   do_three_kings_day (*init_data, detected, ptr_cc_id, "+",
                       6, MONTH_MIN, year, hd_elems, fday, count);
}



   LOCAL void
hdy_fi (init_data, detected, easter, year, hd_elems, fday, count)
         Bool *init_data;
   const Bool  detected;
   const int   easter;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Manages all specific holidays celebrated in Finland.
*/
{
   register int  day;


   ptr_cc_id = CC_FI;
   do_boxing_day (*init_data, detected, ptr_cc_id, "+",
                  26, MONTH_MAX, year, hd_elems, fday, count);
   if (*init_data)
     *init_data = FALSE;
   do_christmas_day (*init_data, detected, ptr_cc_id, "+",
                     25, MONTH_MAX, year, hd_elems, fday, count);
   do_easter_monday (*init_data, detected, ptr_cc_id, "+",
                     easter+1, 0, year, hd_elems, fday, count);
   do_easter_sunday (*init_data, detected, ptr_cc_id, "+",
                     easter, 0, year, hd_elems, fday, count);
   do_good_friday (*init_data, detected, ptr_cc_id, "+",
                   easter-2, 0, year, hd_elems, fday, count);
   do_independence_day (*init_data, detected, ptr_cc_id, "+",
                        6, MONTH_MAX, year, hd_elems, fday, count);
   do_labour_day (*init_data, detected, ptr_cc_id, "+",
                  DAY_MIN, 5, year, hd_elems, fday, count);
   do_marys_ascension_day (*init_data, detected, ptr_cc_id, "+",
                           15, 8, year, hd_elems, fday, count);
   do_maundy_thursday (*init_data, detected, ptr_cc_id, "+",
                       easter-3, 0, year, hd_elems, fday, count);
   day = eval_holiday (dvec[6-1], 6, year, 6, FALSE);
   while (day > 26)
     day -= DAY_MAX;
   do_midsummer_day (*init_data, detected, ptr_cc_id, "+",
                     day, 6, year, hd_elems, fday, count);
   do_midsummer_eve (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                     day-1, 6, year, hd_elems, fday, count);
   do_national_day (*init_data, detected, ptr_cc_id, "+",
                    14, 7, year, hd_elems, fday, count);
   do_pentecost (*init_data, detected, ptr_cc_id, "+",
                 easter+49, 0, year, hd_elems, fday, count);
   do_three_kings_day (*init_data, detected, ptr_cc_id, "+",
                       6, MONTH_MIN, year, hd_elems, fday, count);
}



   LOCAL void
hdy_fr (init_data, detected, easter, year, hd_elems, fday, count)
         Bool *init_data;
   const Bool  detected;
   const int   easter;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Manages all specific holidays celebrated in France.
*/
{
   ptr_cc_id = CC_FR;
   do_all_saints_day (*init_data, detected, ptr_cc_id, "+",
                      DAY_MIN, 11, year, hd_elems, fday, count);
   if (*init_data)
     *init_data = FALSE;
   if (year > 1944)
     do_armistice_day (*init_data, detected, ptr_cc_id, "+",
                       8, 5, year, hd_elems, fday, count);
   if (year > 1917)
     do_armistice_day (*init_data, detected, ptr_cc_id, "+",
                       11, 11, year, hd_elems, fday, count);
   do_boxing_day (*init_data, detected, ptr_cc_id, "+",
                  26, MONTH_MAX, year, hd_elems, fday, count);
   do_christmas_day (*init_data, detected, ptr_cc_id, "+",
                     25, MONTH_MAX, year, hd_elems, fday, count);
   do_easter_monday (*init_data, detected, ptr_cc_id, "+",
                     easter+1, 0, year, hd_elems, fday, count);
   do_easter_sunday (*init_data, detected, ptr_cc_id, "+",
                     easter, 0, year, hd_elems, fday, count);
   do_labour_day (*init_data, detected, ptr_cc_id, "+",
                  DAY_MIN, 5, year, hd_elems, fday, count);
   do_marys_ascension_day (*init_data, detected, ptr_cc_id, "+",
                           15, 8, year, hd_elems, fday, count);
   do_national_day (*init_data, detected, ptr_cc_id, "+",
                    14, 7, year, hd_elems, fday, count);
   do_pentecost (*init_data, detected, ptr_cc_id, "+",
                 easter+49, 0, year, hd_elems, fday, count);
   do_whit_monday (*init_data, detected, ptr_cc_id, "+",
                   easter+50, 0, year, hd_elems, fday, count);
}



   LOCAL void
hdy_gb (init_data, detected, easter, year, hd_elems, fday, count)
         Bool *init_data;
   const Bool  detected;
   const int   easter;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Manages all specific holidays celebrated in Great Britain.
*/
{
   register int  day;


   ptr_cc_id = CC_GB;
#if USE_DE
   print_single_holiday (*init_data, detected, "Brandnacht", ptr_cc_id,
                         DIS_HLS_PREF2, 25, MONTH_MIN, year, hd_elems, fday, count);
   if (*init_data)
     *init_data = FALSE;
   print_single_holiday (*init_data, detected, "St. Davidstag", ptr_cc_id,
                         DIS_HLS_PREF, DAY_MIN, 3, year, hd_elems, fday, count);
   print_single_holiday (*init_data, detected, "Schwalbentag", ptr_cc_id,
                         DIS_HLS_PREF, 15, 4, year, hd_elems, fday, count);
   print_single_holiday (*init_data, detected, "St. Georgstag", ptr_cc_id,
                         DIS_HLS_PREF, 23, 4, year, hd_elems, fday, count);
   print_single_holiday (*init_data, detected, "Schlacht von Boyne", ptr_cc_id,
                         DIS_HLS_PREF2, 12, 7, year, hd_elems, fday, count);
   print_single_holiday (*init_data, detected, "Grottentag", ptr_cc_id,
                         DIS_HLS_PREF, 25, 7, year, hd_elems, fday, count);
   print_single_holiday (*init_data, detected, "St. Edwardstag", ptr_cc_id,
                         DIS_HLS_PREF, 13, 10, year, hd_elems, fday, count);
   if (year > 1604)
     print_single_holiday (*init_data, detected, "Guy Fawke's Tag", ptr_cc_id,
                           DIS_HLS_PREF, 5, 11, year, hd_elems, fday, count);
#else /* !USE_DE */
   print_single_holiday (*init_data, detected, _("Burns Night"), ptr_cc_id,
                         DIS_HLS_PREF2, 25, MONTH_MIN, year, hd_elems, fday, count);
   if (*init_data)
     *init_data = FALSE;
   print_single_holiday (*init_data, detected, _("St. David's Day"), ptr_cc_id,
                         DIS_HLS_PREF, DAY_MIN, 3, year, hd_elems, fday, count);
   print_single_holiday (*init_data, detected, _("Swallow Day"), ptr_cc_id,
                         DIS_HLS_PREF, 15, 4, year, hd_elems, fday, count);
   print_single_holiday (*init_data, detected, _("St. George's Day"), ptr_cc_id,
                         DIS_HLS_PREF, 23, 4, year, hd_elems, fday, count);
   print_single_holiday (*init_data, detected, _("Battle of Boyne"), ptr_cc_id,
                         DIS_HLS_PREF2, 12, 7, year, hd_elems, fday, count);
   print_single_holiday (*init_data, detected, _("Grotto Day"), ptr_cc_id,
                         DIS_HLS_PREF, 25, 7, year, hd_elems, fday, count);
   print_single_holiday (*init_data, detected, _("St. Edward's Day"), ptr_cc_id,
                         DIS_HLS_PREF, 13, 10, year, hd_elems, fday, count);
   if (year > 1604)
     print_single_holiday (*init_data, detected, _("Guy Fawke's Day"), ptr_cc_id,
                           DIS_HLS_PREF, 5, 11, year, hd_elems, fday, count);
#endif /* !USE_DE */
   day = eval_holiday (dvec[5-1], 5, year, DAY_MIN, FALSE);
   do_bank_holiday (*init_data, detected, ptr_cc_id, "+",
                    day, 5, year, hd_elems, fday, count);
   day = eval_holiday (DAY_MIN, 8, year, DAY_MIN, TRUE);
   do_bank_holiday (*init_data, detected, ptr_cc_id, DIS_HLS_PREF2,
                    day, 8, year, hd_elems, fday, count);
   day = eval_holiday (dvec[8-1], 8, year, DAY_MIN, FALSE);
   do_bank_holiday (*init_data, detected, ptr_cc_id, "+",
                    day, 8, year, hd_elems, fday, count);
   do_boxing_day (*init_data, detected, ptr_cc_id, "+",
                  26, MONTH_MAX, year, hd_elems, fday, count);
   do_christmas_day (*init_data, detected, ptr_cc_id, "+",
                     25, MONTH_MAX, year, hd_elems, fday, count);
   do_easter_monday (*init_data, detected, ptr_cc_id, "+",
                     easter+1, 0, year, hd_elems, fday, count);
   do_easter_sunday (*init_data, detected, ptr_cc_id, "+",
                     easter, 0, year, hd_elems, fday, count);
   day = eval_holiday (DAY_MIN, 6, year, DAY_MAX, TRUE);
   do_fathers_day (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                   day+(2*DAY_MAX), 6, year, hd_elems, fday, count);
   do_good_friday (*init_data, detected, ptr_cc_id, "+",
                   easter-2, 0, year, hd_elems, fday, count);
   do_marys_annunciation_day (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                              25, 3, year, hd_elems, fday, count);
   day = eval_holiday (DAY_MIN, 5, year, DAY_MIN, TRUE);
   do_may_day (*init_data, detected, ptr_cc_id, "+",
               day, 5, year, hd_elems, fday, count);
   do_mothers_day (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                   easter-21, 0, year, hd_elems, fday, count);
   do_new_years_day (*init_data, detected, ptr_cc_id, DIS_HLS_PREF2,
                     3, MONTH_MIN, year, hd_elems, fday, count);
   do_queens_birthday (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                       11, 6, year, hd_elems, fday, count);
}



   LOCAL void
hdy_gr (init_data, detected, easter, year, hd_elems, fday, count)
         Bool *init_data;
   const Bool  detected;
         int   easter;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Manages all specific holidays celebrated in Greece.
     The Greeks compute all holiday dates which are depending on Easter
     Sunday's date in the way done by the Eastern Orthodox churches.
     We have to compute the date of Easter by using Julian dates for a
     definite year, means, we have to ignore any missing days of a Gregorian
     Reformation for this year.  Thereafter, we simply add the day difference
     between the given Gregorian calendar date and the according Julian
     calendar date --- i.e. the amount of days, the Julian calendar is
     past the Gregorian calendar --- to this date!
*/
{
   auto Greg_struct  tmp_greg;


   ptr_cc_id = CC_GR;
   /*
      So we first need to buffer the Gregorian Reformation period actually used.
   */
   tmp_greg.first_day = greg->first_day;
   tmp_greg.last_day = greg->last_day;
   tmp_greg.month = greg->month;
   tmp_greg.year = greg->year;
   /*
      Then we set the Gregorian Reformation period to the first date
        which is out of the range of dates Gcal is able to manage,
        for computing the Easter Sunday of a Julian date, means
        we behave as if no Gregorian Reformation have ever happened.
   */
   greg->last_day=greg->first_day = DAY_MIN;
   greg->month = MONTH_MIN;
   greg->year = YEAR_MAX+1;
   /*
      Now we compute Easter Sunday's date in the Julian style.
   */
   easter = knuth_easter_formula (year);
   /*
      Now we set the Gregorian Reformation period to the date used by the Greeks.
   */
   greg->first_day = 10;
   greg->last_day = 22;
   greg->month = 3;
   greg->year = 1924;
   /*
      The computation is done for the next date after the Greek Gregorian
        Reformation Period (March 23rd 1924), but for the current year
        (Gregorian date).  If this is not proper, I would like to hear
        *your* solution how to compute it in the right way!
   */
   easter += julian_gregorian_diff (DAY_MIN, greg->month+1, year);
   /*
      And restore the buffered the Gregorian Reformation date actually used.
   */
   greg->year = tmp_greg.year;
   greg->month = tmp_greg.month;
   greg->last_day = tmp_greg.last_day;
   greg->first_day = tmp_greg.first_day;
#if USE_DE
   print_single_holiday (*init_data, detected, "Fest des Theotokos", ptr_cc_id,
                         "+", 26, MONTH_MAX, year, hd_elems, fday, count);
#else /* !USE_DE */
   print_single_holiday (*init_data, detected, _("Feast of Our Theotokos"), ptr_cc_id,
                         "+", 26, MONTH_MAX, year, hd_elems, fday, count);
#endif /* !USE_DE */
   if (*init_data)
     *init_data = FALSE;
   do_ash_monday (*init_data, detected, ptr_cc_id, "+",
                  easter-48, 0, year, hd_elems, fday, count);
   do_christmas_day (*init_data, detected, ptr_cc_id, "+",
                     25, MONTH_MAX, year, hd_elems, fday, count);
   do_easter_monday (*init_data, detected, ptr_cc_id, "+",
                     easter+1, 0, year, hd_elems, fday, count);
   do_easter_sunday (*init_data, detected, ptr_cc_id, "+",
                     easter, 0, year, hd_elems, fday, count);
   do_good_friday (*init_data, detected, ptr_cc_id, "+",
                   easter-2, 0, year, hd_elems, fday, count);
   do_good_saturday (*init_data, detected, ptr_cc_id, "+",
                     easter-1, 0, year, hd_elems, fday, count);
   do_labour_day (*init_data, detected, ptr_cc_id, "+",
                  DAY_MIN, 5, year, hd_elems, fday, count);
   do_marys_ascension_day (*init_data, detected, ptr_cc_id, "+",
                           15, 8, year, hd_elems, fday, count);
   do_national_day (*init_data, detected, ptr_cc_id, "+",
                    25, 3, year, hd_elems, fday, count);
   do_national_day (*init_data, detected, ptr_cc_id, "+",
                    28, 10, year, hd_elems, fday, count);
   do_pentecost (*init_data, detected, ptr_cc_id, "+",
                 easter+49, 0, year, hd_elems, fday, count);
   do_three_kings_day (*init_data, detected, ptr_cc_id, "+",
                       6, MONTH_MIN, year, hd_elems, fday, count);
   do_whit_monday (*init_data, detected, ptr_cc_id, "+",
                   easter+50, 0, year, hd_elems, fday, count);
}



   LOCAL void
hdy_hu (init_data, detected, easter, year, hd_elems, fday, count)
         Bool *init_data;
   const Bool  detected;
   const int   easter;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Manages all specific holidays celebrated in Hungary.
*/
{
   ptr_cc_id = CC_HU;
   do_boxing_day (*init_data, detected, ptr_cc_id, "+",
                  26, MONTH_MAX, year, hd_elems, fday, count);
   if (*init_data)
     *init_data = FALSE;
   do_christmas_day (*init_data, detected, ptr_cc_id, "+",
                     25, MONTH_MAX, year, hd_elems, fday, count);
   do_christmas_eve (*init_data, detected, ptr_cc_id, "+",
                     24, MONTH_MAX, year, hd_elems, fday, count);
   do_constitution_anniversary (*init_data, detected, ptr_cc_id, "+",
                                20, 8, year, hd_elems, fday, count);
   do_easter_monday (*init_data, detected, ptr_cc_id, "+",
                     easter+1, 0, year, hd_elems, fday, count);
   do_easter_sunday (*init_data, detected, ptr_cc_id, "+",
                     easter, 0, year, hd_elems, fday, count);
   do_labour_day (*init_data, detected, ptr_cc_id, "+",
                  DAY_MIN, 5, year, hd_elems, fday, count);
   if (   (year > 1944)
       && (year < 1989))
     do_liberation_day (*init_data, detected, ptr_cc_id, "+",
                        4, 4, year, hd_elems, fday, count);
   do_national_day (*init_data, detected, ptr_cc_id, "+",
                    15, 3, year, hd_elems, fday, count);
   do_national_day (*init_data, detected, ptr_cc_id, "+",
                    23, 10, year, hd_elems, fday, count);
   do_pentecost (*init_data, detected, ptr_cc_id, "+",
                 easter+49, 0, year, hd_elems, fday, count);
   do_whit_monday (*init_data, detected, ptr_cc_id, "+",
                   easter+50, 0, year, hd_elems, fday, count);
}



   LOCAL void
hdy_it (init_data, detected, easter, year, hd_elems, fday, count)
         Bool *init_data;
   const Bool  detected;
   const int   easter;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Manages all specific holidays celebrated in Italy.
*/
{
   ptr_cc_id = CC_IT;
#if USE_DE
   print_single_holiday (*init_data, detected, "Ferragosto", ptr_cc_id,
                         "+", 15, 8, year, hd_elems, fday, count);
   if (*init_data)
     *init_data = FALSE;
   print_single_holiday (*init_data, detected, "St. Stefanstag", ptr_cc_id,
                         "+", 26, MONTH_MAX, year, hd_elems, fday, count);
#else /* !USE_DE */
   print_single_holiday (*init_data, detected, _("Ferragosto"), ptr_cc_id,
                         "+", 15, 8, year, hd_elems, fday, count);
   if (*init_data)
     *init_data = FALSE;
   print_single_holiday (*init_data, detected, _("St. Stephen's Day"), ptr_cc_id,
                         "+", 26, MONTH_MAX, year, hd_elems, fday, count);
#endif /* !USE_DE */
   do_all_saints_day (*init_data, detected, ptr_cc_id, "+",
                      DAY_MIN, 11, year, hd_elems, fday, count);
   do_all_souls_day (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                     2, 11, year, hd_elems, fday, count);
   do_ash_wednesday (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                     easter-46, 0, year, hd_elems, fday, count);
   do_christs_ascension_day (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                             easter+39, 0, year, hd_elems, fday, count);
   do_christmas_day (*init_data, detected, ptr_cc_id, "+",
                     25, MONTH_MAX, year, hd_elems, fday, count);
   do_easter_monday (*init_data, detected, ptr_cc_id, "+",
                     easter+1, 0, year, hd_elems, fday, count);
   do_easter_sunday (*init_data, detected, ptr_cc_id, "+",
                     easter, 0, year, hd_elems, fday, count);
   do_feast_of_corpus_christi (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                               easter+60, 0, year, hd_elems, fday, count);
   do_labour_day (*init_data, detected, ptr_cc_id, "+",
                  DAY_MIN, 5, year, hd_elems, fday, count);
   if (year > 1944)
     do_liberation_day (*init_data, detected, ptr_cc_id, "+",
                        25, 4, year, hd_elems, fday, count);
   do_marys_immaculate_conception (*init_data, detected, ptr_cc_id, "+",
                                   8, MONTH_MAX, year, hd_elems, fday, count);
   do_palm_sunday (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                   easter-DAY_MAX, 0, year, hd_elems, fday, count);
   do_pentecost (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                 easter+49, 0, year, hd_elems, fday, count);
   do_republic_day (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                    2, 6, year, hd_elems, fday, count);
   do_three_kings_day (*init_data, detected, ptr_cc_id, "+",
                       6, MONTH_MIN, year, hd_elems, fday, count);
   do_valentines_day (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                      14, 2, year, hd_elems, fday, count);
}



   LOCAL void
hdy_mx (init_data, detected, easter, year, hd_elems, fday, count)
         Bool *init_data;
   const Bool  detected;
   const int   easter;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Manages all specific holidays celebrated in Mexico.
*/
{
   register int  day;


   ptr_cc_id = CC_MX;
#if USE_DE
   print_single_holiday (*init_data, detected, "Benito Juarez Tag", ptr_cc_id,
                         "+", 21, 3, year, hd_elems, fday, count);
   if (*init_data)
     *init_data = FALSE;
   print_single_holiday (*init_data, detected, "Schlacht von Puebla", ptr_cc_id,
                         "*", 5, 5, year, hd_elems, fday, count);
   print_single_holiday (*init_data, detected, "Tag der Lehrer", ptr_cc_id,
                         DIS_HLS_PREF, 15, 5, year, hd_elems, fday, count);
   print_single_holiday (*init_data, detected, "Berichtstag des Pr"AE"sidenten", ptr_cc_id,
                         "+", DAY_MIN, 9, year, hd_elems, fday, count);
   print_single_holiday (*init_data, detected, "Proklamation der Unabh"AE"ngigkeit", ptr_cc_id,
                         DIS_HLS_PREF, 15, 9, year, hd_elems, fday, count);
   print_single_holiday (*init_data, detected, "Tag der Rassenverschmelzung", ptr_cc_id,
                         DIS_HLS_PREF2, 12, 10, year, hd_elems, fday, count);
   print_single_holiday (*init_data, detected, "Tag der Revolution", ptr_cc_id,
                         "+", 20, 11, year, hd_elems, fday, count);
   print_single_holiday (*init_data, detected, "Tag der Jungfrau von Guadalupe", ptr_cc_id,
                         DIS_HLS_PREF, 12, MONTH_MAX, year, hd_elems, fday, count);
#else /* !USE_DE */
   print_single_holiday (*init_data, detected, _("Benito Juarez Day"), ptr_cc_id,
                         "+", 21, 3, year, hd_elems, fday, count);
   if (*init_data)
     *init_data = FALSE;
   print_single_holiday (*init_data, detected, _("Puebla's Battle"), ptr_cc_id,
                         "*", 5, 5, year, hd_elems, fday, count);
   print_single_holiday (*init_data, detected, _("Teacher's Day"), ptr_cc_id,
                         DIS_HLS_PREF, 15, 5, year, hd_elems, fday, count);
   print_single_holiday (*init_data, detected, _("President's Inform"), ptr_cc_id,
                         "+", DAY_MIN, 9, year, hd_elems, fday, count);
   print_single_holiday (*init_data, detected, _("Independence proclamation"), ptr_cc_id,
                         DIS_HLS_PREF, 15, 9, year, hd_elems, fday, count);
   print_single_holiday (*init_data, detected, _("Breed fusion Day"), ptr_cc_id,
                         DIS_HLS_PREF2, 12, 10, year, hd_elems, fday, count);
   print_single_holiday (*init_data, detected, _("Revolution Day"), ptr_cc_id,
                         "+", 20, 11, year, hd_elems, fday, count);
   print_single_holiday (*init_data, detected, _("Guadalupe Virgin's Day"), ptr_cc_id,
                         DIS_HLS_PREF, 12, MONTH_MAX, year, hd_elems, fday, count);
#endif /* !USE_DE */
   do_all_saints_day (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                      DAY_MIN, 11, year, hd_elems, fday, count);
   do_all_souls_day (*init_data, detected, ptr_cc_id, "+",
                     2, 11, year, hd_elems, fday, count);
   do_childrens_day (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                     dvec[4-1], 4, year, hd_elems, fday, count);
   do_christmas_day (*init_data, detected, ptr_cc_id, "+",
                     25, MONTH_MAX, year, hd_elems, fday, count);
   do_constitution_anniversary (*init_data, detected, ptr_cc_id, "+",
                                5, 2, year, hd_elems, fday, count);
   day = eval_holiday (DAY_MIN, 6, year, DAY_MAX, TRUE);
   do_fathers_day (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                   day+(2*DAY_MAX), 6, year, hd_elems, fday, count);
   do_good_friday (*init_data, detected, ptr_cc_id, "+",
                   easter-2, 0, year, hd_elems, fday, count);
   do_labour_day (*init_data, detected, ptr_cc_id, "+",
                  DAY_MIN, 5, year, hd_elems, fday, count);
   do_maundy_thursday (*init_data, detected, ptr_cc_id, "+",
                       easter-3, 0, year, hd_elems, fday, count);
   do_mothers_day (*init_data, detected, ptr_cc_id, "*",
                   10, 5, year, hd_elems, fday, count);
   do_independence_day (*init_data, detected, ptr_cc_id, "+",
                        16, 9, year, hd_elems, fday, count);
   do_sylvester (*init_data, detected, ptr_cc_id, "+",
                 dvec[MONTH_MAX-1], MONTH_MAX, year, hd_elems, fday, count);
}



   LOCAL void
hdy_nl (init_data, detected, easter, year, hd_elems, fday, count)
         Bool *init_data;
   const Bool  detected;
   const int   easter;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Manages all specific holidays celebrated in Netherlands.
*/
{
   register int  day;


   ptr_cc_id = CC_NL;
#if USE_DE
   print_single_holiday (*init_data, detected, "Einhoven's Befreiung", ptr_cc_id,
                         DIS_HLS_PREF2, 18, 9, year, hd_elems, fday, count);
   if (*init_data)
     *init_data = FALSE;
   day = eval_holiday (DAY_MIN, 9, year, 2, TRUE);
   print_single_holiday (*init_data, detected, "Prinzessin' Tag", ptr_cc_id,
                         DIS_HLS_PREF, day+(2*DAY_MAX), 9, year, hd_elems, fday, count);
   print_single_holiday (*init_data, detected, "Welt-Tierschutztag", ptr_cc_id,
                         DIS_HLS_PREF, 4, 10, year, hd_elems, fday, count);
   print_single_holiday (*init_data, detected, "St. Nikolaus Vorabend", ptr_cc_id,
                         DIS_HLS_PREF, 5, MONTH_MAX, year, hd_elems, fday, count);
#else /* !USE_DE */
   print_single_holiday (*init_data, detected, _("Eindhoven's Liberation"), ptr_cc_id,
                         DIS_HLS_PREF2, 18, 9, year, hd_elems, fday, count);
   if (*init_data)
     *init_data = FALSE;
   day = eval_holiday (DAY_MIN, 9, year, 2, TRUE);
   print_single_holiday (*init_data, detected, _("Princess' Day"), ptr_cc_id,
                         DIS_HLS_PREF, day+(2*DAY_MAX), 9, year, hd_elems, fday, count);
   print_single_holiday (*init_data, detected, _("World Animal Day"), ptr_cc_id,
                         DIS_HLS_PREF, 4, 10, year, hd_elems, fday, count);
   print_single_holiday (*init_data, detected, _("St. Nicholas' Eve"), ptr_cc_id,
                         DIS_HLS_PREF, 5, MONTH_MAX, year, hd_elems, fday, count);
#endif /* !USE_DE */
   do_boxing_day (*init_data, detected, ptr_cc_id, "+",
                  26, MONTH_MAX, year, hd_elems, fday, count);
   do_christs_ascension_day (*init_data, detected, ptr_cc_id, "+",
                             easter+39, 0, year, hd_elems, fday, count);
   do_christmas_day (*init_data, detected, ptr_cc_id, "+",
                     25, MONTH_MAX, year, hd_elems, fday, count);
   do_easter_monday (*init_data, detected, ptr_cc_id, "+",
                     easter+1, 0, year, hd_elems, fday, count);
   do_easter_sunday (*init_data, detected, ptr_cc_id, "+",
                     easter, 0, year, hd_elems, fday, count);
   day = eval_holiday (DAY_MIN, 6, year, DAY_MAX, TRUE);
   do_fathers_day (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                   day+(2*DAY_MAX), 6, year, hd_elems, fday, count);
   do_good_friday (*init_data, detected, ptr_cc_id, DIS_HLS_PREF2,
                   easter-2, 0, year, hd_elems, fday, count);
   do_maundy_thursday (*init_data, detected, ptr_cc_id, DIS_HLS_PREF2,
                       easter-3, 0, year, hd_elems, fday, count);
   day = eval_holiday (DAY_MIN, 5, year, DAY_MAX, TRUE);
   if (day_of_year (day, 5, year) != easter+49)
     day += DAY_MAX;
   do_mothers_day (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                   day, 5, year, hd_elems, fday, count);
   do_labour_day (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                  DAY_MIN, 5, year, hd_elems, fday, count);
   if (year > 1944)
     do_liberation_day (*init_data, detected, ptr_cc_id, "+",
                        5, 5, year, hd_elems, fday, count);
   do_pentecost (*init_data, detected, ptr_cc_id, "+",
                 easter+49, 0, year, hd_elems, fday, count);
   do_queens_birthday (*init_data, detected, ptr_cc_id, "+",
                       dvec[4-1], 4, year, hd_elems, fday, count);
   do_whit_monday (*init_data, detected, ptr_cc_id, "+",
                   easter+50, 0, year, hd_elems, fday, count);
}



   LOCAL void
hdy_no (init_data, detected, easter, year, hd_elems, fday, count)
         Bool *init_data;
   const Bool  detected;
   const int   easter;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Manages all specific holidays celebrated in Norway.
*/
{
   ptr_cc_id = CC_NO;
#if USE_DE
   print_single_holiday (*init_data, detected, "Nationalfeiertag der Lappen", ptr_cc_id,
                         DIS_HLS_PREF, 6, 2, year, hd_elems, fday, count);
   if (*init_data)
     *init_data = FALSE;
   print_single_holiday (*init_data, detected, "Union mit Schweden aufgel"OE"st", ptr_cc_id,
                         DIS_HLS_PREF, 7, 6, year, hd_elems, fday, count);
   print_single_holiday (*init_data, detected, "Olsok Vorabend", ptr_cc_id,
                         DIS_HLS_PREF, 29, 7, year, hd_elems, fday, count);
   print_single_holiday (*init_data, detected, "Tag der Vereinten Nationen", ptr_cc_id,
                         DIS_HLS_PREF, 24, 10, year, hd_elems, fday, count);
#else /* !USE_DE */
   print_single_holiday (*init_data, detected, _("Lapp National Holiday"), ptr_cc_id,
                         DIS_HLS_PREF, 6, 2, year, hd_elems, fday, count);
   if (*init_data)
     *init_data = FALSE;
   print_single_holiday (*init_data, detected, _("Union with Sweden dissolved"), ptr_cc_id,
                         DIS_HLS_PREF, 7, 6, year, hd_elems, fday, count);
   print_single_holiday (*init_data, detected, _("Olsok Eve"), ptr_cc_id,
                         DIS_HLS_PREF, 29, 7, year, hd_elems, fday, count);
   print_single_holiday (*init_data, detected, _("United Nations Day"), ptr_cc_id,
                         DIS_HLS_PREF, 24, 10, year, hd_elems, fday, count);
#endif /* !USE_DE */
   do_ash_wednesday (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                     easter-46, 0, year, hd_elems, fday, count);
   do_boxing_day (*init_data, detected, ptr_cc_id, "+",
                  26, MONTH_MAX, year, hd_elems, fday, count);
   do_christs_ascension_day (*init_data, detected, ptr_cc_id, "+",
                             easter+39, 0, year, hd_elems, fday, count);
   do_christmas_day (*init_data, detected, ptr_cc_id, "+",
                     25, MONTH_MAX, year, hd_elems, fday, count);
   do_easter_monday (*init_data, detected, ptr_cc_id, "+",
                     easter+1, 0, year, hd_elems, fday, count);
   do_easter_sunday (*init_data, detected, ptr_cc_id, "+",
                     easter, 0, year, hd_elems, fday, count);
   do_good_friday (*init_data, detected, ptr_cc_id, "+",
                   easter-2, 0, year, hd_elems, fday, count);
   do_johns_day (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                 24, 6, year, hd_elems, fday, count);
   do_johns_eve (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                 23, 6, year, hd_elems, fday, count);
   do_kings_birthday (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                      21, 2, year, hd_elems, fday, count);
   do_labour_day (*init_data, detected, ptr_cc_id, "+",
                  DAY_MIN, 5, year, hd_elems, fday, count);
   if (year > 1944)
     do_liberation_day (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                        8, 5, year, hd_elems, fday, count);
   do_maundy_thursday (*init_data, detected, ptr_cc_id, "+",
                       easter-3, 0, year, hd_elems, fday, count);
   do_national_day (*init_data, detected, ptr_cc_id, "+",
                    17, 5, year, hd_elems, fday, count);
   do_palm_sunday (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                   easter-DAY_MAX, 0, year, hd_elems, fday, count);
   do_pentecost (*init_data, detected, ptr_cc_id, "+",
                 easter+49, 0, year, hd_elems, fday, count);
   do_queens_birthday (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                       4, 7, year, hd_elems, fday, count);
   do_whit_monday (*init_data, detected, ptr_cc_id, "+",
                   easter+50, 0, year, hd_elems, fday, count);
   do_womens_day (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                  8, 3, year, hd_elems, fday, count);
}



   LOCAL void
hdy_pl (init_data, detected, easter, year, hd_elems, fday, count)
         Bool *init_data;
   const Bool  detected;
   const int   easter;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Manages all specific holidays celebrated in Poland.
*/
{
   ptr_cc_id = CC_PL;
   if (   (year > 1944)
       && (year < 1989))
    {
#if USE_DE
      print_single_holiday (*init_data, detected, "Siegestag", ptr_cc_id,
                           "+", 9, 5, year, hd_elems, fday, count);
#else /* !USE_DE */
      print_single_holiday (*init_data, detected, _("Victory Day"), ptr_cc_id,
                            "+", 9, 5, year, hd_elems, fday, count);
#endif /* !USE_DE */
      if (*init_data)
        *init_data = FALSE;
    }
   do_all_saints_day (*init_data, detected, ptr_cc_id, "+",
                      DAY_MIN, 11, year, hd_elems, fday, count);
   if (*init_data)
     *init_data = FALSE;
   do_boxing_day (*init_data, detected, ptr_cc_id, "+",
                  26, MONTH_MAX, year, hd_elems, fday, count);
   do_christmas_day (*init_data, detected, ptr_cc_id, "+",
                     25, MONTH_MAX, year, hd_elems, fday, count);
   do_christs_ascension_day (*init_data, detected, ptr_cc_id, "+",
                             easter+39, 0, year, hd_elems, fday, count);
   do_constitution_anniversary (*init_data, detected, ptr_cc_id, "+",
                                3, 5, year, hd_elems, fday, count);
   do_easter_monday (*init_data, detected, ptr_cc_id, "+",
                     easter+1, 0, year, hd_elems, fday, count);
   do_easter_sunday (*init_data, detected, ptr_cc_id, "+",
                     easter, 0, year, hd_elems, fday, count);
   do_fathers_day (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                   23, 6, year, hd_elems, fday, count);
   do_feast_of_corpus_christi (*init_data, detected, ptr_cc_id, "+",
                               easter+60, 0, year, hd_elems, fday, count);
   if (year > 1988)
     do_independence_day (*init_data, detected, ptr_cc_id, "+",
                          11, 11, year, hd_elems, fday, count);
   do_labour_day (*init_data, detected, ptr_cc_id, "+",
                  DAY_MIN, 5, year, hd_elems, fday, count);
   do_marys_ascension_day (*init_data, detected, ptr_cc_id, "+",
                           15, 8, year, hd_elems, fday, count);
   do_mothers_day (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                   26, 5, year, hd_elems, fday, count);
   if (   (year > 1944)
       && (year < 1989))
     do_national_day (*init_data, detected, ptr_cc_id, "+",
                      22, 7, year, hd_elems, fday, count);
   do_three_kings_day (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                       6, MONTH_MIN, year, hd_elems, fday, count);
   do_valentines_day (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                      14, 2, year, hd_elems, fday, count);
   do_womens_day (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                  8, 3, year, hd_elems, fday, count);
}



   LOCAL void
hdy_pt (init_data, detected, easter, year, hd_elems, fday, count)
         Bool *init_data;
   const Bool  detected;
   const int   easter;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Manages all specific holidays celebrated in Portugal.
*/
{
   ptr_cc_id = CC_PT;
   do_all_saints_day (*init_data, detected, ptr_cc_id, "+",
                      DAY_MIN, 11, year, hd_elems, fday, count);
   if (*init_data)
     *init_data = FALSE;
   do_christmas_day (*init_data, detected, ptr_cc_id, "+",
                     25, MONTH_MAX, year, hd_elems, fday, count);
   do_easter_sunday (*init_data, detected, ptr_cc_id, "+",
                     easter, 0, year, hd_elems, fday, count);
   do_feast_of_corpus_christi (*init_data, detected, ptr_cc_id, "+",
                               easter+60, 0, year, hd_elems, fday, count);
   do_good_friday (*init_data, detected, ptr_cc_id, "+",
                   easter-2, 0, year, hd_elems, fday, count);
   do_independence_day (*init_data, detected, ptr_cc_id, "+",
                        DAY_MIN, MONTH_MAX, year, hd_elems, fday, count);
   do_labour_day (*init_data, detected, ptr_cc_id, "+",
                  DAY_MIN, 5, year, hd_elems, fday, count);
   if (year > 1944)
     do_liberation_day (*init_data, detected, ptr_cc_id, "+",
                        25, 4, year, hd_elems, fday, count);
   do_marys_ascension_day (*init_data, detected, ptr_cc_id, "+",
                           15, 8, year, hd_elems, fday, count);
   do_marys_immaculate_conception (*init_data, detected, ptr_cc_id, "+",
                                   8, MONTH_MAX, year, hd_elems, fday, count);
   do_national_day (*init_data, detected, ptr_cc_id, "+",
                    10, 6, year, hd_elems, fday, count);
   do_republic_day (*init_data, detected, ptr_cc_id, "+",
                    5, 10, year, hd_elems, fday, count);
   do_shrove_tuesday (*init_data, detected, ptr_cc_id, "+",
                      easter-47, 0, year, hd_elems, fday, count);
}



   LOCAL void
hdy_se (init_data, detected, easter, year, hd_elems, fday, count)
         Bool *init_data;
   const Bool  detected;
   const int   easter;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Manages all specific holidays celebrated in Sweden.
*/
{
   register int  day;
   register int  month;


   ptr_cc_id = CC_SE;
#if USE_DE
   print_single_holiday (*init_data, detected, "St. Knutstag", ptr_cc_id,
                         DIS_HLS_PREF, 13, MONTH_MIN, year, hd_elems, fday, count);
   if (*init_data)
     *init_data = FALSE;
   print_single_holiday (*init_data, detected, "Des K"OE"nigs Namenstag", ptr_cc_id,
                         DIS_HLS_PREF, 28, MONTH_MIN, year, hd_elems, fday, count);
   print_single_holiday (*init_data, detected, "Der Kronprinzessin Namenstag", ptr_cc_id,
                         DIS_HLS_PREF, 12, 3, year, hd_elems, fday, count);
   print_single_holiday (*init_data, detected, "Pfingstvorabend", ptr_cc_id,
                         DIS_HLS_PREF, easter+48, 0, year, hd_elems, fday, count);
   print_single_holiday (*init_data, detected, "Der Kronprinzessin Geburtstag", ptr_cc_id,
                         DIS_HLS_PREF, 14, 7, year, hd_elems, fday, count);
   print_single_holiday (*init_data, detected, "Der K"OE"nigin Namenstag", ptr_cc_id,
                         DIS_HLS_PREF, 8, 8, year, hd_elems, fday, count);
   print_single_holiday (*init_data, detected, "Gustav Adolph Tag", ptr_cc_id,
                         DIS_HLS_PREF, 6, 11, year, hd_elems, fday, count);
   print_single_holiday (*init_data, detected, "Nobel Tag", ptr_cc_id,
                         DIS_HLS_PREF, 10, MONTH_MAX, year, hd_elems, fday, count);
   print_single_holiday (*init_data, detected, "St. Luciastag", ptr_cc_id,
                         DIS_HLS_PREF, 13, MONTH_MAX, year, hd_elems, fday, count);
   print_single_holiday (*init_data, detected, "Tag der heiligen Unschuldigen", ptr_cc_id,
                         DIS_HLS_PREF, 28, MONTH_MAX, year, hd_elems, fday, count);
#else /* !USE_DE */
   print_single_holiday (*init_data, detected, _("St. Canute's Day"), ptr_cc_id,
                         DIS_HLS_PREF, 13, MONTH_MIN, year, hd_elems, fday, count);
   if (*init_data)
     *init_data = FALSE;
   print_single_holiday (*init_data, detected, _("The King's Name-Day"), ptr_cc_id,
                         DIS_HLS_PREF, 28, MONTH_MIN, year, hd_elems, fday, count);
   print_single_holiday (*init_data, detected, _("The Crown Princesse's Name-Day"), ptr_cc_id,
                         DIS_HLS_PREF, 12, 3, year, hd_elems, fday, count);
   print_single_holiday (*init_data, detected, _("Whitsun Eve"), ptr_cc_id,
                         DIS_HLS_PREF, easter+48, 0, year, hd_elems, fday, count);
   print_single_holiday (*init_data, detected, _("The Crown Princesse's Birthday"), ptr_cc_id,
                         DIS_HLS_PREF, 14, 7, year, hd_elems, fday, count);
   print_single_holiday (*init_data, detected, _("The Queen's Name-Day"), ptr_cc_id,
                         DIS_HLS_PREF, 8, 8, year, hd_elems, fday, count);
   print_single_holiday (*init_data, detected, _("Gustavus Adolphus' Day"), ptr_cc_id,
                         DIS_HLS_PREF, 6, 11, year, hd_elems, fday, count);
   print_single_holiday (*init_data, detected, _("Nobel Day"), ptr_cc_id,
                         DIS_HLS_PREF, 10, MONTH_MAX, year, hd_elems, fday, count);
   print_single_holiday (*init_data, detected, _("St. Lucia's Day"), ptr_cc_id,
                         DIS_HLS_PREF, 13, MONTH_MAX, year, hd_elems, fday, count);
   print_single_holiday (*init_data, detected, _("Holy Innocent's Day"), ptr_cc_id,
                         DIS_HLS_PREF, 28, MONTH_MAX, year, hd_elems, fday, count);
#endif /* !USE_DE */
   do_boxing_day (*init_data, detected, ptr_cc_id, "+",
                  26, MONTH_MAX, year, hd_elems, fday, count);
   do_christs_ascension_day (*init_data, detected, ptr_cc_id, "+",
                             easter+39, 0, year, hd_elems, fday, count);
   do_christmas_day (*init_data, detected, ptr_cc_id, "+",
                     25, MONTH_MAX, year, hd_elems, fday, count);
   do_easter_monday (*init_data, detected, ptr_cc_id, "+",
                     easter+1, 0, year, hd_elems, fday, count);
   do_easter_sunday (*init_data, detected, ptr_cc_id, "+",
                     easter, 0, year, hd_elems, fday, count);
   day = eval_holiday (DAY_MIN, 11, year, DAY_MAX, TRUE);
   do_fathers_day (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                   day+DAY_MAX, 11, year, hd_elems, fday, count);
   do_good_friday (*init_data, detected, ptr_cc_id, "+",
                   easter-2, 0, year, hd_elems, fday, count);
   do_good_saturday (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                     easter-1, 0, year, hd_elems, fday, count);
   do_kings_birthday (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                      dvec[4-1], 4, year, hd_elems, fday, count);
   do_labour_day (*init_data, detected, ptr_cc_id, "+",
                  DAY_MIN, 5, year, hd_elems, fday, count);
   do_marys_annunciation_day (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                              25, 3, year, hd_elems, fday, count);
   day = eval_holiday (dvec[6-1], 6, year, 6, FALSE);
   while (day > 26)
     day -= DAY_MAX;
   do_midsummer_day (*init_data, detected, ptr_cc_id, "+",
                     day, 6, year, hd_elems, fday, count);
   do_midsummer_eve (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                     day-1, 6, year, hd_elems, fday, count);
   day = eval_holiday (dvec[5-1], 5, year, DAY_MAX, FALSE);
   do_mothers_day (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                   day, 5, year, hd_elems, fday, count);
   do_national_day (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                    6, 6, year, hd_elems, fday, count);
   do_pentecost (*init_data, detected, ptr_cc_id, "+",
                 easter+49, 0, year, hd_elems, fday, count);
   do_queens_birthday (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                       23, MONTH_MAX, year, hd_elems, fday, count);
   do_shrove_tuesday (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                      easter-47, 0, year, hd_elems, fday, count);
   month = 10;
   day = weekday_of_date (dvec[month-1], month, year);
   if (day != 6)
    {
      month++;
      day = eval_holiday (DAY_MIN, month, year, 6, TRUE);
    }
   do_thanksgiving (*init_data, detected, ptr_cc_id, "+",
                    day, month, year, hd_elems, fday, count);
   do_three_kings_day (*init_data, detected, ptr_cc_id, "+",
                       6, MONTH_MIN, year, hd_elems, fday, count);
   do_walpurgis_night (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                       dvec[4-1], 4, year, hd_elems, fday, count);
   do_whit_monday (*init_data, detected, ptr_cc_id, "+",
                   easter+50, 0, year, hd_elems, fday, count);
}



   LOCAL void
hdy_si (init_data, detected, easter, year, hd_elems, fday, count)
         Bool *init_data;
   const Bool  detected;
   const int   easter;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Manages all specific holidays celebrated in Slovenia.
*/
{
   ptr_cc_id = CC_SI;
#if USE_DE
   print_single_holiday (*init_data, detected, "Kulturtag", ptr_cc_id,
                         "+", 8, 2, year, hd_elems, fday, count);
   if (*init_data)
     *init_data = FALSE;
   print_single_holiday (*init_data, detected, "Tag des nationalen Widerstandes", ptr_cc_id,
                         "+", 27, 4, year, hd_elems, fday, count);
#else /* !USE_DE */
   print_single_holiday (*init_data, detected, _("Culture Day"), ptr_cc_id,
                         "+", 8, 2, year, hd_elems, fday, count);
   if (*init_data)
     *init_data = FALSE;
   print_single_holiday (*init_data, detected, _("National Resistance Day"), ptr_cc_id,
                         "+", 27, 4, year, hd_elems, fday, count);
#endif /* !USE_DE */
   do_christmas_day (*init_data, detected, ptr_cc_id, "+",
                     25, MONTH_MAX, year, hd_elems, fday, count);
   do_easter_monday (*init_data, detected, ptr_cc_id, "+",
                     easter+1, 0, year, hd_elems, fday, count);
   do_easter_sunday (*init_data, detected, ptr_cc_id, "+",
                     easter, 0, year, hd_elems, fday, count);
   do_independence_day (*init_data, detected, ptr_cc_id, "+",
                        26, MONTH_MAX, year, hd_elems, fday, count);
   do_labour_day (*init_data, detected, ptr_cc_id, "+",
                  DAY_MIN, 5, year, hd_elems, fday, count);
   do_labour_day (*init_data, detected, ptr_cc_id, "+",
                  2, 5, year, hd_elems, fday, count);
   do_marys_ascension_day (*init_data, detected, ptr_cc_id, "+",
                           15, 8, year, hd_elems, fday, count);
   do_new_years_day (*init_data, detected, ptr_cc_id, "+",
                     2, MONTH_MIN, year, hd_elems, fday, count);
   do_national_day (*init_data, detected, ptr_cc_id, "+",
                    25, 6, year, hd_elems, fday, count);
   do_pentecost (*init_data, detected, ptr_cc_id, "+",
                 easter+49, 0, year, hd_elems, fday, count);
   do_reformation_day (*init_data, detected, ptr_cc_id, "+",
                       dvec[10-1], 10, year, hd_elems, fday, count);
   do_remembrance_day (*init_data, detected, ptr_cc_id, "+",
                       DAY_MIN, 11, year, hd_elems, fday, count);
}



   LOCAL void
hdy_th (init_data, detected, easter, year, hd_elems, fday, count)
         Bool *init_data;
   const Bool  detected;
   const int   easter;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Manages all specific holidays celebrated in Thailand.
*/
{
   register int  day;


   ptr_cc_id = CC_TH;
#if USE_DE
   print_single_holiday (*init_data, detected, "Kr"OE"nungstag", ptr_cc_id,
                         "+", 5, 5, year, hd_elems, fday, count);
#else /* !USE_DE */
   print_single_holiday (*init_data, detected, _("Coronation Day"), ptr_cc_id,
                         "+", 5, 5, year, hd_elems, fday, count);
#endif /* !USE_DE */
   if (*init_data)
     *init_data = FALSE;
   day = eval_holiday (DAY_MIN, MONTH_MIN, year, 6, TRUE);
   do_childrens_day (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                     day+DAY_MAX, MONTH_MIN, year, hd_elems, fday, count);
   do_constitution_anniversary (*init_data, detected, ptr_cc_id, "+",
                                10, 12, year, hd_elems, fday, count);
   do_kings_birthday (*init_data, detected, ptr_cc_id, "+",
                      5, MONTH_MAX, year, hd_elems, fday, count);
   do_labour_day (*init_data, detected, ptr_cc_id, "+",
                  DAY_MIN, 5, year, hd_elems, fday, count);
   do_queens_birthday (*init_data, detected, ptr_cc_id, "+",
                       12, 8, year, hd_elems, fday, count);
   do_sylvester (*init_data, detected, ptr_cc_id, "+",
                 dvec[MONTH_MAX-1], MONTH_MAX, year, hd_elems, fday, count);
}



   LOCAL void
hdy_us (init_data, detected, easter, year, hd_elems, fday, count)
         Bool *init_data;
   const Bool  detected;
   const int   easter;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Manages all specific holidays celebrated in U.S.A.
*/
{
   register int  day;


   ptr_cc_id = CC_US;
#if USE_DE
   day = eval_holiday (DAY_MIN, MONTH_MIN, year, DAY_MIN, TRUE);
   print_single_holiday (*init_data, detected, "Martin L. King's Tag", ptr_cc_id,
                         DIS_HLS_PREF, day+(2*DAY_MAX), MONTH_MIN, year, hd_elems, fday, count);
   if (*init_data)
     *init_data = FALSE;
   day = eval_holiday (DAY_MIN, 2, year, DAY_MIN, TRUE);
   print_single_holiday (*init_data, detected, "Pr"AE"sidenten Tag", ptr_cc_id,
                         DIS_HLS_PREF, day+(2*DAY_MAX), 2, year, hd_elems, fday, count);
   print_single_holiday (*init_data, detected, "St. Patrickstag", ptr_cc_id,
                         DIS_HLS_PREF, 17, 3, year, hd_elems, fday, count);
   day = eval_holiday (DAY_MIN, 5, year, 6, TRUE);
   print_single_holiday (*init_data, detected, "Tag der Streitkr"AE"fte", ptr_cc_id,
                         DIS_HLS_PREF, day+(2*DAY_MAX), 5, year, hd_elems, fday, count);
   print_single_holiday (*init_data, detected, "Tag der Flagge", ptr_cc_id,
                         DIS_HLS_PREF, 14, 6, year, hd_elems, fday, count);
   day = eval_holiday (DAY_MIN, 10, year, DAY_MIN, TRUE);
   print_single_holiday (*init_data, detected, "Kolumbustag", ptr_cc_id,
                         "#", day+DAY_MAX, 10, year, hd_elems, fday, count);
   print_single_holiday (*init_data, detected, "Halloween", ptr_cc_id,
                         DIS_HLS_PREF, dvec[10-1], 10, year, hd_elems, fday, count);
   if (year > 1844)
    {
      day = eval_holiday (DAY_MIN, 11, year, DAY_MIN, TRUE);
      print_single_holiday (*init_data, detected, "Wahltag", ptr_cc_id,
                            DIS_HLS_PREF, day+1, 11, year, hd_elems, fday, count);
    }
   print_single_holiday (*init_data, detected, "Tag der Veteranen", ptr_cc_id,
                         "+", 11, 11, year, hd_elems, fday, count);
#else /* !USE_DE */
   day = eval_holiday (DAY_MIN, MONTH_MIN, year, DAY_MIN, TRUE);
   print_single_holiday (*init_data, detected, _("Martin L. King's Day"), ptr_cc_id,
                         DIS_HLS_PREF, day+(2*DAY_MAX), MONTH_MIN, year, hd_elems, fday, count);
   if (*init_data)
     *init_data = FALSE;
   day = eval_holiday (DAY_MIN, 2, year, DAY_MIN, TRUE);
   print_single_holiday (*init_data, detected, _("President's Day"), ptr_cc_id,
                         DIS_HLS_PREF, day+(2*DAY_MAX), 2, year, hd_elems, fday, count);
   print_single_holiday (*init_data, detected, _("St. Patrick's Day"), ptr_cc_id,
                         DIS_HLS_PREF, 17, 3, year, hd_elems, fday, count);
   day = eval_holiday (DAY_MIN, 5, year, 6, TRUE);
   print_single_holiday (*init_data, detected, _("Armed Forces' Day"), ptr_cc_id,
                         DIS_HLS_PREF, day+(2*DAY_MAX), 5, year, hd_elems, fday, count);
   print_single_holiday (*init_data, detected, _("Flag Day"), ptr_cc_id,
                         DIS_HLS_PREF, 14, 6, year, hd_elems, fday, count);
   day = eval_holiday (DAY_MIN, 10, year, DAY_MIN, TRUE);
   print_single_holiday (*init_data, detected, _("Columbus Day"), ptr_cc_id,
                         "#", day+DAY_MAX, 10, year, hd_elems, fday, count);
   print_single_holiday (*init_data, detected, _("Halloween"), ptr_cc_id,
                         DIS_HLS_PREF, dvec[10-1], 10, year, hd_elems, fday, count);
   if (year > 1844)
    {
      day = eval_holiday (DAY_MIN, 11, year, DAY_MIN, TRUE);
      print_single_holiday (*init_data, detected, _("Election Day"), ptr_cc_id,
                            DIS_HLS_PREF, day+1, 11, year, hd_elems, fday, count);
    }
   print_single_holiday (*init_data, detected, _("Veteran's Day"), ptr_cc_id,
                         "+", 11, 11, year, hd_elems, fday, count);
#endif /* !USE_DE */
   do_ash_wednesday (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                     easter-46, 0, year, hd_elems, fday, count);
   do_christmas_day (*init_data, detected, ptr_cc_id, "+",
                     25, MONTH_MAX, year, hd_elems, fday, count);
   day = eval_holiday (DAY_MIN, 6, year, DAY_MAX, TRUE);
   do_fathers_day (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                   day+(2*DAY_MAX), 6, year, hd_elems, fday, count);
   do_good_friday (*init_data, detected, ptr_cc_id, DIS_HLS_PREF2,
                   easter-2, 0, year, hd_elems, fday, count);
   day = weekday_of_date (4, 7, year);
   if (day == 6)
     day = 3;
   else
     if (day == 7)
       day = 5;
     else
       day = 4;
   do_independence_day (*init_data, detected, ptr_cc_id, "+",
                        day, 7, year, hd_elems, fday, count);
   day = eval_holiday (DAY_MIN, 9, year, DAY_MIN, TRUE);
   do_labour_day (*init_data, detected, ptr_cc_id, "+",
                  day, 9, year, hd_elems, fday, count);
   day = eval_holiday (DAY_MIN, 5, year, DAY_MAX, TRUE);
   if (day_of_year (day, 5, year) != easter+49)
     day += DAY_MAX;
   do_mothers_day (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                   day, 5, year, hd_elems, fday, count);
   day = eval_holiday (dvec[5-1], 5, year, DAY_MIN, FALSE);
   do_remembrance_day (*init_data, detected, ptr_cc_id, "+",
                       day, 5, year, hd_elems, fday, count);
   day = eval_holiday (DAY_MIN, 11, year, 4, TRUE);
   do_thanksgiving (*init_data, detected, ptr_cc_id, "+",
                    day+(3*DAY_MAX), 11, year, hd_elems, fday, count);
   do_valentines_day (*init_data, detected, ptr_cc_id, DIS_HLS_PREF,
                      14, 2, year, hd_elems, fday, count);
}



/*
*  Manage all holidays which occur in more than ONE country!
*/



   LOCAL void
do_all_saints_day (init_data, detected, country_code, holiday_prefix,
                   day, month, year, hd_elems, fday, count)
         Bool  init_data;
   const Bool  detected;
   const char *country_code;
   const char *holiday_prefix;
         int   day;
         int   month;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Inserts this holiday into the eternal holiday list.
*/
{
#if USE_DE
   print_single_holiday (init_data, detected, "Allerheiligen", country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#else /* !USE_DE */
   print_single_holiday (init_data, detected, _("All Saints' Day"), country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#endif /* !USE_DE */
}



   LOCAL void
do_all_souls_day (init_data, detected, country_code, holiday_prefix,
                  day, month, year, hd_elems, fday, count)
         Bool  init_data;
   const Bool  detected;
   const char *country_code;
   const char *holiday_prefix;
         int   day;
         int   month;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Inserts this holiday into the eternal holiday list.
*/
{
#if USE_DE
   print_single_holiday (init_data, detected, "Allerseelen", country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#else /* !USE_DE */
   print_single_holiday (init_data, detected, _("All Souls' Day"), country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#endif /* !USE_DE */
}



   LOCAL void
do_ash_monday (init_data, detected, country_code, holiday_prefix,
               day, month, year, hd_elems, fday, count)
         Bool  init_data;
   const Bool  detected;
   const char *country_code;
   const char *holiday_prefix;
         int   day;
         int   month;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Inserts this holiday into the eternal holiday list.
*/
{
#if USE_DE
   print_single_holiday (init_data, detected, "Rosenmontag", country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#else /* !USE_DE */
   print_single_holiday (init_data, detected, _("Ash Monday"), country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#endif /* !USE_DE */
}



   LOCAL void
do_ash_wednesday (init_data, detected, country_code, holiday_prefix,
                  day, month, year, hd_elems, fday, count)
         Bool  init_data;
   const Bool  detected;
   const char *country_code;
   const char *holiday_prefix;
         int   day;
         int   month;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Inserts this holiday into the eternal holiday list.
*/
{
#if USE_DE
   print_single_holiday (init_data, detected, "Aschermittwoch", country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#else /* !USE_DE */
   print_single_holiday (init_data, detected, _("Ash Wednesday"), country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#endif /* !USE_DE */
}



   LOCAL void
do_armistice_day (init_data, detected, country_code, holiday_prefix,
                  day, month, year, hd_elems, fday, count)
         Bool  init_data;
   const Bool  detected;
   const char *country_code;
   const char *holiday_prefix;
         int   day;
         int   month;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Inserts this holiday into the eternal holiday list.
*/
{
#if USE_DE
   print_single_holiday (init_data, detected, "Waffenstillstandstag", country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#else /* !USE_DE */
   print_single_holiday (init_data, detected, _("Armistice Day"), country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#endif /* !USE_DE */
}



   LOCAL void
do_bank_holiday (init_data, detected, country_code, holiday_prefix,
                 day, month, year, hd_elems, fday, count)
         Bool  init_data;
   const Bool  detected;
   const char *country_code;
   const char *holiday_prefix;
         int   day;
         int   month;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Inserts this holiday into the eternal holiday list.
*/
{
#if USE_DE
   print_single_holiday (init_data, detected, "Feiertag", country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#else /* !USE_DE */
   print_single_holiday (init_data, detected, _("Bank Holiday"), country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#endif /* !USE_DE */
}



   LOCAL void
do_boxing_day (init_data, detected, country_code, holiday_prefix,
               day, month, year, hd_elems, fday, count)
         Bool  init_data;
   const Bool  detected;
   const char *country_code;
   const char *holiday_prefix;
         int   day;
         int   month;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Inserts this holiday into the eternal holiday list.
*/
{
#if USE_DE
   print_single_holiday (init_data, detected, "2'ter Weihnachtstag", country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#else /* !USE_DE */
   print_single_holiday (init_data, detected, _("Boxing Day"), country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#endif /* !USE_DE */
}



   LOCAL void
do_childrens_day (init_data, detected, country_code, holiday_prefix,
                  day, month, year, hd_elems, fday, count)
         Bool  init_data;
   const Bool  detected;
   const char *country_code;
   const char *holiday_prefix;
         int   day;
         int   month;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Inserts this holiday into the eternal holiday list.
*/
{
#if USE_DE
   print_single_holiday (init_data, detected, "Tag der Kinder", country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#else /* !USE_DE */
   print_single_holiday (init_data, detected, _("Children's Day"), country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#endif /* !USE_DE */
}



   LOCAL void
do_christmas_day (init_data, detected, country_code, holiday_prefix,
                  day, month, year, hd_elems, fday, count)
         Bool  init_data;
   const Bool  detected;
   const char *country_code;
   const char *holiday_prefix;
         int   day;
         int   month;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Inserts this holiday into the eternal holiday list.
*/
{
#if USE_DE
   print_single_holiday (init_data, detected, "1'ter Weihnachtstag", country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#else /* !USE_DE */
   print_single_holiday (init_data, detected, _("Christmas Day"), country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#endif /* !USE_DE */
}



   LOCAL void
do_christmas_eve (init_data, detected, country_code, holiday_prefix,
                  day, month, year, hd_elems, fday, count)
         Bool  init_data;
   const Bool  detected;
   const char *country_code;
   const char *holiday_prefix;
         int   day;
         int   month;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Inserts this holiday into the eternal holiday list.
*/
{
#if USE_DE
   print_single_holiday (init_data, detected, "Heiligabend", country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#else /* !USE_DE */
   print_single_holiday (init_data, detected, _("Christmas Eve"), country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#endif /* !USE_DE */
}



   LOCAL void
do_christs_ascension_day (init_data, detected, country_code, holiday_prefix,
                          day, month, year, hd_elems, fday, count)
         Bool  init_data;
   const Bool  detected;
   const char *country_code;
   const char *holiday_prefix;
         int   day;
         int   month;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Inserts this holiday into the eternal holiday list.
*/
{
#if USE_DE
   print_single_holiday (init_data, detected, "Christi Himmelfahrt", country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#else /* !USE_DE */
   print_single_holiday (init_data, detected, _("Christ's Ascension Day"), country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#endif /* !USE_DE */
}



   LOCAL void
do_constitution_anniversary (init_data, detected, country_code, holiday_prefix,
                             day, month, year, hd_elems, fday, count)
         Bool  init_data;
   const Bool  detected;
   const char *country_code;
   const char *holiday_prefix;
         int   day;
         int   month;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Inserts this holiday into the eternal holiday list.
*/
{
#if USE_DE
   print_single_holiday (init_data, detected, "Jahrestag der Verfassung", country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#else /* !USE_DE */
   print_single_holiday (init_data, detected, _("Constitution Anniversary"), country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#endif /* !USE_DE */
}



   LOCAL void
do_easter_sunday (init_data, detected, country_code, holiday_prefix,
                  day, month, year, hd_elems, fday, count)
         Bool  init_data;
   const Bool  detected;
   const char *country_code;
   const char *holiday_prefix;
         int   day;
         int   month;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Inserts this holiday into the eternal holiday list.
*/
{
#if USE_DE
   print_single_holiday (init_data, detected, "Ostersonntag", country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#else /* !USE_DE */
   print_single_holiday (init_data, detected, _("Easter Sunday"), country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#endif /* !USE_DE */
}



   LOCAL void
do_easter_monday (init_data, detected, country_code, holiday_prefix,
                  day, month, year, hd_elems, fday, count)
         Bool  init_data;
   const Bool  detected;
   const char *country_code;
   const char *holiday_prefix;
         int   day;
         int   month;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Inserts this holiday into the eternal holiday list.
*/
{
#if USE_DE
   print_single_holiday (init_data, detected, "Ostermontag", country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#else /* !USE_DE */
   print_single_holiday (init_data, detected, _("Easter Monday"), country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#endif /* !USE_DE */
}



   LOCAL void
do_fathers_day (init_data, detected, country_code, holiday_prefix,
                day, month, year, hd_elems, fday, count)
         Bool  init_data;
   const Bool  detected;
   const char *country_code;
   const char *holiday_prefix;
         int   day;
         int   month;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Inserts this holiday into the eternal holiday list.
*/
{
#if USE_DE
   print_single_holiday (init_data, detected, "Vatertag", country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#else /* !USE_DE */
   print_single_holiday (init_data, detected, _("Father's Day"), country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#endif /* !USE_DE */
}



   LOCAL void
do_feast_of_corpus_christi (init_data, detected, country_code, holiday_prefix,
                            day, month, year, hd_elems, fday, count)
         Bool  init_data;
   const Bool  detected;
   const char *country_code;
   const char *holiday_prefix;
         int   day;
         int   month;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Inserts this holiday into the eternal holiday list.
*/
{
#if USE_DE
   print_single_holiday (init_data, detected, "Fronleichnam", country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#else /* !USE_DE */
   print_single_holiday (init_data, detected, _("Feast of Corpus Christi"), country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#endif /* !USE_DE */
}



   LOCAL void
do_good_friday (init_data, detected, country_code, holiday_prefix,
                day, month, year, hd_elems, fday, count)
         Bool  init_data;
   const Bool  detected;
   const char *country_code;
   const char *holiday_prefix;
         int   day;
         int   month;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Inserts this holiday into the eternal holiday list.
*/
{
#if USE_DE
   print_single_holiday (init_data, detected, "Karfreitag", country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#else /* !USE_DE */
   print_single_holiday (init_data, detected, _("Good Friday"), country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#endif /* !USE_DE */
}



   LOCAL void
do_good_saturday (init_data, detected, country_code, holiday_prefix,
                  day, month, year, hd_elems, fday, count)
         Bool  init_data;
   const Bool  detected;
   const char *country_code;
   const char *holiday_prefix;
         int   day;
         int   month;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Inserts this holiday into the eternal holiday list.
*/
{
#if USE_DE
   print_single_holiday (init_data, detected, "Karsamstag/Ostervorabend", country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#else /* !USE_DE */
   print_single_holiday (init_data, detected, _("Good Saturday/Easter Eve"), country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#endif /* !USE_DE */
}



   LOCAL void
do_independence_day (init_data, detected, country_code, holiday_prefix,
                     day, month, year, hd_elems, fday, count)
         Bool  init_data;
   const Bool  detected;
   const char *country_code;
   const char *holiday_prefix;
         int   day;
         int   month;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Inserts this holiday into the eternal holiday list.
*/
{
#if USE_DE
   print_single_holiday (init_data, detected, "Unabh"AE"ngigkeitstag", country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#else /* !USE_DE */
   print_single_holiday (init_data, detected, _("Independence Day"), country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#endif /* !USE_DE */
}



   LOCAL void
do_johns_day (init_data, detected, country_code, holiday_prefix,
              day, month, year, hd_elems, fday, count)
         Bool  init_data;
   const Bool  detected;
   const char *country_code;
   const char *holiday_prefix;
         int   day;
         int   month;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Inserts this holiday into the eternal holiday list.
*/
{
#if USE_DE
   print_single_holiday (init_data, detected, "St. Hanstag", country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#else /* !USE_DE */
   print_single_holiday (init_data, detected, _("St. John's Day"), country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#endif /* !USE_DE */
}



   LOCAL void
do_johns_eve (init_data, detected, country_code, holiday_prefix,
              day, month, year, hd_elems, fday, count)
         Bool  init_data;
   const Bool  detected;
   const char *country_code;
   const char *holiday_prefix;
         int   day;
         int   month;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Inserts this holiday into the eternal holiday list.
*/
{
#if USE_DE
   print_single_holiday (init_data, detected, "St. Hans Vorabend", country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#else /* !USE_DE */
   print_single_holiday (init_data, detected, _("St. John's Eve"), country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#endif /* !USE_DE */
}



   LOCAL void
do_kings_birthday (init_data, detected, country_code, holiday_prefix,
                   day, month, year, hd_elems, fday, count)
         Bool  init_data;
   const Bool  detected;
   const char *country_code;
   const char *holiday_prefix;
         int   day;
         int   month;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Inserts this holiday into the eternal holiday list.
*/
{
#if USE_DE
   print_single_holiday (init_data, detected, "Des K"OE"nigs Geburtstag", country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#else /* !USE_DE */
   print_single_holiday (init_data, detected, _("The King's Birthday"), country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#endif /* !USE_DE */
}



   LOCAL void
do_labour_day (init_data, detected, country_code, holiday_prefix,
               day, month, year, hd_elems, fday, count)
         Bool  init_data;
   const Bool  detected;
   const char *country_code;
   const char *holiday_prefix;
         int   day;
         int   month;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Inserts this holiday into the eternal holiday list.
*/
{
#if USE_DE
   print_single_holiday (init_data, detected, "Tag der Arbeit", country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#else /* !USE_DE */
   print_single_holiday (init_data, detected, _("Labour Day"), country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#endif /* !USE_DE */
}



   LOCAL void
do_liberation_day (init_data, detected, country_code, holiday_prefix,
                   day, month, year, hd_elems, fday, count)
         Bool  init_data;
   const Bool  detected;
   const char *country_code;
   const char *holiday_prefix;
         int   day;
         int   month;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Inserts this holiday into the eternal holiday list.
*/
{
#if USE_DE
   print_single_holiday (init_data, detected, "Tag der Befreiung", country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#else /* !USE_DE */
   print_single_holiday (init_data, detected, _("Liberation Day"), country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#endif /* !USE_DE */
}



   LOCAL void
do_martinimas (init_data, detected, country_code, holiday_prefix,
               day, month, year, hd_elems, fday, count)
         Bool  init_data;
   const Bool  detected;
   const char *country_code;
   const char *holiday_prefix;
         int   day;
         int   month;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Inserts this holiday into the eternal holiday list.
*/
{
#if USE_DE
   print_single_holiday (init_data, detected, "Martinstag", country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#else /* !USE_DE */
   print_single_holiday (init_data, detected, _("Martinimas"), country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#endif /* !USE_DE */
}



   LOCAL void
do_marys_annunciation_day (init_data, detected, country_code, holiday_prefix,
                           day, month, year, hd_elems, fday, count)
         Bool  init_data;
   const Bool  detected;
   const char *country_code;
   const char *holiday_prefix;
         int   day;
         int   month;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Inserts this holiday into the eternal holiday list.
*/
{
#if USE_DE
   print_single_holiday (init_data, detected, "Mari"AE" Verk"UE"ndigung", country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#else /* !USE_DE */
   print_single_holiday (init_data, detected, _("Mary's Annunciation Day"), country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#endif /* !USE_DE */
}



   LOCAL void
do_marys_ascension_day (init_data, detected, country_code, holiday_prefix,
                        day, month, year, hd_elems, fday, count)
         Bool  init_data;
   const Bool  detected;
   const char *country_code;
   const char *holiday_prefix;
         int   day;
         int   month;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Inserts this holiday into the eternal holiday list.
*/
{
#if USE_DE
   print_single_holiday (init_data, detected, "Mari"AE" Himmelfahrt", country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#else /* !USE_DE */
   print_single_holiday (init_data, detected, _("Mary's Ascension Day"), country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#endif /* !USE_DE */
}

   LOCAL void
do_marys_immaculate_conception (init_data, detected, country_code, holiday_prefix,
                                day, month, year, hd_elems, fday, count)
         Bool  init_data;
   const Bool  detected;
   const char *country_code;
   const char *holiday_prefix;
         int   day;
         int   month;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Inserts this holiday into the eternal holiday list.
*/
{
#if USE_DE
   print_single_holiday (init_data, detected, "Mari"AE" Empf"AE"ngnis", country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#else /* !USE_DE */
   print_single_holiday (init_data, detected, _("Mary's Immaculate Conception"), country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#endif /* !USE_DE */
}



   LOCAL void
do_maundy_thursday (init_data, detected, country_code, holiday_prefix,
                    day, month, year, hd_elems, fday, count)
         Bool  init_data;
   const Bool  detected;
   const char *country_code;
   const char *holiday_prefix;
         int   day;
         int   month;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Inserts this holiday into the eternal holiday list.
*/
{
#if USE_DE
   print_single_holiday (init_data, detected, "Gr"UE"ndonnerstag", country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#else /* !USE_DE */
   print_single_holiday (init_data, detected, _("Maundy Thursday"), country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#endif /* !USE_DE */
}



   LOCAL void
do_may_day (init_data, detected, country_code, holiday_prefix,
            day, month, year, hd_elems, fday, count)
         Bool  init_data;
   const Bool  detected;
   const char *country_code;
   const char *holiday_prefix;
         int   day;
         int   month;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Inserts this holiday into the eternal holiday list.
*/
{
#if USE_DE
   print_single_holiday (init_data, detected, "Maifeiertag", country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#else /* !USE_DE */
   print_single_holiday (init_data, detected, _("May Day"), country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#endif /* !USE_DE */
}



   LOCAL void
do_midsummer_day (init_data, detected, country_code, holiday_prefix,
                  day, month, year, hd_elems, fday, count)
         Bool  init_data;
   const Bool  detected;
   const char *country_code;
   const char *holiday_prefix;
         int   day;
         int   month;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Inserts this holiday into the eternal holiday list.
*/
{
#if USE_DE
   print_single_holiday (init_data, detected, "Johannistag", country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#else /* !USE_DE */
   print_single_holiday (init_data, detected, _("Midsummer's Day"), country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#endif /* !USE_DE */
}



   LOCAL void
do_midsummer_eve (init_data, detected, country_code, holiday_prefix,
                  day, month, year, hd_elems, fday, count)
         Bool  init_data;
   const Bool  detected;
   const char *country_code;
   const char *holiday_prefix;
         int   day;
         int   month;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Inserts this holiday into the eternal holiday list.
*/
{
#if USE_DE
   print_single_holiday (init_data, detected, "Johannisvorabend", country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#else /* !USE_DE */
   print_single_holiday (init_data, detected, _("Midsummer's Eve"), country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#endif /* !USE_DE */
}



   LOCAL void
do_mothers_day (init_data, detected, country_code, holiday_prefix,
                day, month, year, hd_elems, fday, count)
         Bool  init_data;
   const Bool  detected;
   const char *country_code;
   const char *holiday_prefix;
         int   day;
         int   month;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Inserts this holiday into the eternal holiday list.
*/
{
#if USE_DE
   print_single_holiday (init_data, detected, "Muttertag", country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#else /* !USE_DE */
   print_single_holiday (init_data, detected, _("Mother's Day"), country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#endif /* !USE_DE */
}



   LOCAL void
do_national_day (init_data, detected, country_code, holiday_prefix,
                 day, month, year, hd_elems, fday, count)
         Bool  init_data;
   const Bool  detected;
   const char *country_code;
   const char *holiday_prefix;
         int   day;
         int   month;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Inserts this holiday into the eternal holiday list.
*/
{
#if USE_DE
   print_single_holiday (init_data, detected, "Nationalfeiertag", country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#else /* !USE_DE */
   print_single_holiday (init_data, detected, _("National Holiday"), country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#endif /* !USE_DE */
}



   LOCAL void
do_new_years_day (init_data, detected, country_code, holiday_prefix,
                  day, month, year, hd_elems, fday, count)
         Bool  init_data;
   const Bool  detected;
   const char *country_code;
   const char *holiday_prefix;
         int   day;
         int   month;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Inserts this holiday into the eternal holiday list.
*/
{
#if USE_DE
   print_single_holiday (init_data, detected, "Neujahr", country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#else /* !USE_DE */
   print_single_holiday (init_data, detected, _("New Year's Day"), country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#endif /* !USE_DE */
}



   LOCAL void
do_pentecost (init_data, detected, country_code, holiday_prefix,
              day, month, year, hd_elems, fday, count)
         Bool  init_data;
   const Bool  detected;
   const char *country_code;
   const char *holiday_prefix;
         int   day;
         int   month;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Inserts this holiday into the eternal holiday list.
*/
{
#if USE_DE
   print_single_holiday (init_data, detected, "Pfingstsonntag", country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#else /* !USE_DE */
   print_single_holiday (init_data, detected, _("Whitsunday/Pentecost"), country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#endif /* !USE_DE */
}



   LOCAL void
do_palm_sunday (init_data, detected, country_code, holiday_prefix,
                day, month, year, hd_elems, fday, count)
         Bool  init_data;
   const Bool  detected;
   const char *country_code;
   const char *holiday_prefix;
         int   day;
         int   month;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Inserts this holiday into the eternal holiday list.
*/
{
#if USE_DE
   print_single_holiday (init_data, detected, "Palmsonntag", country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#else /* !USE_DE */
   print_single_holiday (init_data, detected, _("Palm Sunday"), country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#endif /* !USE_DE */
}



   LOCAL void
do_reformation_day (init_data, detected, country_code, holiday_prefix,
                    day, month, year, hd_elems, fday, count)
         Bool  init_data;
   const Bool  detected;
   const char *country_code;
   const char *holiday_prefix;
         int   day;
         int   month;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Inserts this holiday into the eternal holiday list.
*/
{
#if USE_DE
   print_single_holiday (init_data, detected, "Reformationstag", country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#else /* !USE_DE */
   print_single_holiday (init_data, detected, _("Reformation Day"), country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#endif /* !USE_DE */
}



   LOCAL void
do_remembrance_day (init_data, detected, country_code, holiday_prefix,
                    day, month, year, hd_elems, fday, count)
         Bool  init_data;
   const Bool  detected;
   const char *country_code;
   const char *holiday_prefix;
         int   day;
         int   month;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Inserts this holiday into the eternal holiday list.
*/
{
#if USE_DE
   print_single_holiday (init_data, detected, "Gedenktag", country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#else /* !USE_DE */
   print_single_holiday (init_data, detected, _("Remembrance/Memorial Day"), country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#endif /* !USE_DE */
}



   LOCAL void
do_republic_day (init_data, detected, country_code, holiday_prefix,
                 day, month, year, hd_elems, fday, count)
         Bool  init_data;
   const Bool  detected;
   const char *country_code;
   const char *holiday_prefix;
         int   day;
         int   month;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Inserts this holiday into the eternal holiday list.
*/
{
#if USE_DE
   print_single_holiday (init_data, detected, "Tag der Republik", country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#else /* !USE_DE */
   print_single_holiday (init_data, detected, _("Republic Day"), country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#endif /* !USE_DE */
}



   LOCAL void
do_shrove_tuesday (init_data, detected, country_code, holiday_prefix,
                   day, month, year, hd_elems, fday, count)
         Bool  init_data;
   const Bool  detected;
   const char *country_code;
   const char *holiday_prefix;
         int   day;
         int   month;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Inserts this holiday into the eternal holiday list.
*/
{
#if USE_DE
   print_single_holiday (init_data, detected, "Fasching/Fastnacht", country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#else /* !USE_DE */
   print_single_holiday (init_data, detected, _("Shrove Tuesday/Mardi Gras"), country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#endif /* !USE_DE */
}



   LOCAL void
do_sylvester (init_data, detected, country_code, holiday_prefix,
              day, month, year, hd_elems, fday, count)
         Bool  init_data;
   const Bool  detected;
   const char *country_code;
   const char *holiday_prefix;
         int   day;
         int   month;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Inserts this holiday into the eternal holiday list.
*/
{
#if USE_DE
   print_single_holiday (init_data, detected, "Silvester", country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#else /* !USE_DE */
   print_single_holiday (init_data, detected, _("Sylvester"), country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#endif /* !USE_DE */
}



   LOCAL void
do_thanksgiving (init_data, detected, country_code, holiday_prefix,
                   day, month, year, hd_elems, fday, count)
         Bool  init_data;
   const Bool  detected;
   const char *country_code;
   const char *holiday_prefix;
         int   day;
         int   month;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Inserts this holiday into the eternal holiday list.
*/
{
#if USE_DE
   print_single_holiday (init_data, detected, "Erntedankfest", country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#else /* !USE_DE */
   print_single_holiday (init_data, detected, _("Thanksgiving"), country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#endif /* !USE_DE */
}



   LOCAL void
do_three_kings_day (init_data, detected, country_code, holiday_prefix,
                    day, month, year, hd_elems, fday, count)
         Bool  init_data;
   const Bool  detected;
   const char *country_code;
   const char *holiday_prefix;
         int   day;
         int   month;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Inserts this holiday into the eternal holiday list.
*/
{
#if USE_DE
   print_single_holiday (init_data, detected, "Epiphanias/Heilige Drei K"OE"nige", country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#else /* !USE_DE */
   print_single_holiday (init_data, detected, _("Epiphany/Three King's Day"), country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#endif /* !USE_DE */
}



   LOCAL void
do_queens_birthday (init_data, detected, country_code, holiday_prefix,
                    day, month, year, hd_elems, fday, count)
         Bool  init_data;
   const Bool  detected;
   const char *country_code;
   const char *holiday_prefix;
         int   day;
         int   month;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Inserts this holiday into the eternal holiday list.
*/
{
#if USE_DE
   print_single_holiday (init_data, detected, "Der K"OE"nigin Geburtstag", country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#else /* !USE_DE */
   print_single_holiday (init_data, detected, _("The Queen's Birthday"), country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#endif /* !USE_DE */
}



   LOCAL void
do_valentines_day (init_data, detected, country_code, holiday_prefix,
                   day, month, year, hd_elems, fday, count)
         Bool  init_data;
   const Bool  detected;
   const char *country_code;
   const char *holiday_prefix;
         int   day;
         int   month;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Inserts this holiday into the eternal holiday list.
*/
{
#if USE_DE
   print_single_holiday (init_data, detected, "St. Valentinstag", country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#else /* !USE_DE */
   print_single_holiday (init_data, detected, _("St. Valentine's Day"), country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#endif /* !USE_DE */
}



   LOCAL void
do_walpurgis_night (init_data, detected, country_code, holiday_prefix,
                    day, month, year, hd_elems, fday, count)
         Bool  init_data;
   const Bool  detected;
   const char *country_code;
   const char *holiday_prefix;
         int   day;
         int   month;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Inserts this holiday into the eternal holiday list.
*/
{
#if USE_DE
   print_single_holiday (init_data, detected, "Walpurgisnacht", country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#else /* !USE_DE */
   print_single_holiday (init_data, detected, _("Walpurgis Night"), country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#endif /* !USE_DE */
}



   LOCAL void
do_whit_monday (init_data, detected, country_code, holiday_prefix,
                day, month, year, hd_elems, fday, count)
         Bool  init_data;
   const Bool  detected;
   const char *country_code;
   const char *holiday_prefix;
         int   day;
         int   month;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Inserts this holiday into the eternal holiday list.
*/
{
#if USE_DE
   print_single_holiday (init_data, detected, "Pfingstmontag", country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#else /* !USE_DE */
   print_single_holiday (init_data, detected, _("Whit Monday"), country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#endif /* !USE_DE */
}



   LOCAL void
do_womens_day (init_data, detected, country_code, holiday_prefix,
               day, month, year, hd_elems, fday, count)
         Bool  init_data;
   const Bool  detected;
   const char *country_code;
   const char *holiday_prefix;
         int   day;
         int   month;
   const int   year;
         int  *hd_elems;
   const int   fday;
   const int   count;
/*
   Inserts this holiday into the eternal holiday list.
*/
{
#if USE_DE
   print_single_holiday (init_data, detected, "Internationaler Frauentag", country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#else /* !USE_DE */
   print_single_holiday (init_data, detected, _("International Women's Day"), country_code,
                         holiday_prefix, day, month, year, hd_elems, fday, count);
#endif /* !USE_DE */
}
