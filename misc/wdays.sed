#  $Id: wdays.sed 0.02 1996/09/10 00:00:02 tom Exp $
#
#  wdays.sed:  This simple sed script converts `gcal %mon -Cdl7+ -H::: ...'
#                output so only all working days of the complete week, except
#                holidays, and the succeeding Monday are displayed for the
#                Anglo-American version of `gcal'.
#
#  Any but default configuration could confuse this script.
#  It comes along with a UN*X script `wdays' and a DOS batch `wdays.bat'
#  which supports the correct usage.
#
#  Uses marking characters in output so it has to be extended accordingly
#  in case real highlighting sequences are required!  Needs also to be
#  modified accordingly in case the default `--date-format' is changed.
#
#  It is *not* guaranteed that this script works for any other call than
#  the one given above but it could easily be modified and extended for
#  using other special modes of operation.
#
#  If you modify this script you have to rename the modified version.
#
#  If you make any improvements I would like to hear from you.
#  But I do not promise any support.
#
#  Copyright (C) 1996  Thomas Esken      <esken@uni-muenster.de>
#                      Im Hagenfeld 84
#                      D-48147 M"unster
#                      GERMANY
#
#  This software doesn't claim completeness, correctness or usability.
#  On principle I will not be liable for ANY damages or losses (implicit
#  or explicit), which result from using or handling my software.
#  If you use this software, you agree without any exception to this
#  agreement, which binds you LEGALLY !!
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the `GNU General Public License' as published by
#  the `Free Software Foundation'; either version 2, or (at your option)
#  any later version.
#
#  You should have received a copy of the `GNU General Public License'
#  along with this program; if not, write to the:
#
#    Free Software Foundation, Inc.
#    59 Temple Place - Suite 330
#    Boston, MA 02111-1307,  USA
#
/Sat, /d
/Sun, /d
/ [0-9][a-z][a-z]/d
/[0-9][0-9][a-z][a-z]/d
s/[]//g
#
#  Alternatively replace the `s/[]//g' command in above line
#  by the next lines for producing a more detailed output in case you
#  use %3 and %4 special texts in the text part of a Gcal resource file line...
#
#s//{/g
#s//}/g
#s/ }/} /g
