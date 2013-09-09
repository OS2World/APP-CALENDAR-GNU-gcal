#  $Id: 2accent.awk 0.07 1996/10/04 00:00:07 tom Exp $
#
#  2accent.awk:  Very simple and silly `AWK' script for converting
#                  "a "A "s ...  or  @"a @"A @ss{} ... character sequences
#                  found in the German `gcal.texi' file (or other files)
#                  to a few real ISO-8859-1 Umlaute and accent characters.
#
#
#  Copyright (C) 1995, 1996  Thomas Esken      <esken@uni-muenster.de>
#                            Im Hagenfeld 84
#                            D-48147 M"unster
#                            GERMANY
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
#
{
  the_line = $0
  isat = 0
  #
  # Check for character sequences starting with a  `@'  character.
  #
  if (the_line ~ /@"/)
    isat = 1
  else
   {
     if (the_line ~ /@`/)
       isat = 1
     else
      {
        if (the_line ~ /@'/)
          isat = 1
        else
         {
           if (the_line ~ /@\^/)
             isat = 1
           else
            {
              if (the_line ~ /@ss\{\}/)
                isat = 1
              else
               {
                 if (the_line ~ /@,\{[cC]\}/)
                   isat = 1
               }
            }
         }
      }
   }
  if (isat == 1)
   {
     #
     # Found a character sequence starting with a  `@'  character.
     #
     len = length(the_line)
     tmptxt = ""
     for (i=1; i <= len; i++)
      {
        tmpchr = substr(the_line, i, 1)
        if (tmpchr == "@")
         {
           if (i != len)
            {
              i++
              tmpchr = substr(the_line, i, 1)
              if (i != len)
               {
                 if (tmpchr == "\"")
                  {
                    #
                    # Check for  Umlaute  character sequences.
                    #
                    i++
                    tmpchr = substr(the_line, i, 1)
                    if (tmpchr == "a")
                      tmptxt = tmptxt "ä"
                    else
                     {
                       if (tmpchr == "e")
                         tmptxt = tmptxt "ë"
                       else
                        {
                          if (tmpchr == "i")
                            tmptxt = tmptxt "ï"
                          else
                           {
                             if (tmpchr == "o")
                               tmptxt = tmptxt "ö"
                             else
                              {
                                if (tmpchr == "u")
                                  tmptxt = tmptxt "ü"
                                else
                                 {
                                   if (tmpchr == "A")
                                     tmptxt = tmptxt "Ä"
                                   else
                                    {
                                      if (tmpchr == "O")
                                        tmptxt = tmptxt "Ö"
                                      else
                                       {
                                         if (tmpchr == "U")
                                           tmptxt = tmptxt "Ü"
                                         else
                                           tmptxt = tmptxt "\"" tmpchr
                                       }
                                    }
                                 }
                              }
                           }
                        }
                     }
                  }
                 else
                  {
                    if (tmpchr == "`")
                     {
                       #
                       # Check for  grave accent  character sequences.
                       #
                       i++
                       tmpchr = substr(the_line, i, 1)
                       if (tmpchr == "a")
                         tmptxt = tmptxt "à"
                       else
                        {
                          if (tmpchr == "e")
                            tmptxt = tmptxt "è"
                          else
                           {
                             if (tmpchr == "i")
                               tmptxt = tmptxt "ì"
                             else
                              {
                                if (tmpchr == "o")
                                  tmptxt = tmptxt "ò"
                                else
                                 {
                                   if (tmpchr == "u")
                                     tmptxt = tmptxt "ù"
                                   else
                                     tmptxt = tmptxt "@`" tmpchr
                                 }
                              }
                           }
                        }
                     }
                    else
                     {
                       if (tmpchr == "'")
                        {
                          #
                          # Check for  acute accent  character sequences.
                          #
                          i++
                          tmpchr = substr(the_line, i, 1)
                          if (tmpchr == "a")
                            tmptxt = tmptxt "à"
                          else
                           {
                             if (tmpchr == "e")
                               tmptxt = tmptxt "é"
                             else
                              {
                                if (tmpchr == "i")
                                  tmptxt = tmptxt "í"
                                else
                                 {
                                   if (tmpchr == "o")
                                     tmptxt = tmptxt "ó"
                                   else
                                    {
                                      if (tmpchr == "u")
                                        tmptxt = tmptxt "ú"
                                      else
                                       {
                                         if (tmpchr == "E")
                                           tmptxt = tmptxt "É"
                                         else
                                           tmptxt = tmptxt "@'" tmpchr
                                       }
                                    }
                                 }
                              }
                           }
                        }
                       else
                        {
                          if (tmpchr == "^")
                           {
                             #
                             # Check for  circumflex accent  character sequences.
                             #
                             i++
                             tmpchr = substr(the_line, i, 1)
                             if (tmpchr == "a")
                               tmptxt = tmptxt "â"
                             else
                              {
                                if (tmpchr == "e")
                                  tmptxt = tmptxt "ê"
                                else
                                 {
                                   if (tmpchr == "i")
                                     tmptxt = tmptxt "î"
                                   else
                                    {
                                      if (tmpchr == "o")
                                        tmptxt = tmptxt "ô"
                                      else
                                       {
                                         if (tmpchr == "u")
                                           tmptxt = tmptxt "û"
                                         else
                                           tmptxt = tmptxt "@^" tmpchr
                                       }
                                    }
                                 }
                              }
                           }
                          else
                           {
                             if (tmpchr == "s")
                              {
                                #
                                # Check for  es-zet  character sequences.
                                #
                                if (substr(the_line, i, 4) == "ss{}")
                                 {
                                   tmptxt = tmptxt "ß"
                                   i += 3
                                 }
                                else
                                  tmptxt = tmptxt "@" tmpchr 
                              }
                             else
                              {
                                if (tmpchr == ",")
                                 {
                                   #
                                   # Check for  cedilla accent  character sequences.
                                   #
                                   if (substr(the_line, i, 4) == ",{c}")
                                    {
                                      tmptxt = tmptxt "ç"
                                      i += 3
                                    }
                                   else
                                    {
                                      if (substr(the_line, i, 4) == ",{C}")
                                       {
                                         tmptxt = tmptxt "Ç"
                                         i += 3
                                       }
                                      else
                                        tmptxt = tmptxt "@" tmpchr
                                    }
                                 }
                                else
                                  tmptxt = tmptxt "@" tmpchr
                              }
                           }
                        }
                     }
                  }
               }
              else
                tmptxt = tmptxt "@" tmpchr
            }
           else
             tmptxt = tmptxt tmpchr
         }
        else
          tmptxt = tmptxt tmpchr
      }
     the_line = tmptxt
   }
  #
  # Check for character sequences starting with a  `"'  character.
  #
  if (the_line ~ /"/)
   {
     #
     # Possibly found a character sequence starting with a  `"'  character.
     #
     len = length(the_line)
     tmptxt = ""
     found = 0
     for (i=1; i <= len; i++)
      {
        tmpchr = substr(the_line, i, 1)
        if (tmpchr == "\"")
         {
           if (found == 1)
            {
              found = 0
              tmptxt = tmptxt tmpchr tmpchr
            }
           else
             found = 1
         }
        else
         {
           #
           # Check for  Umlaute  character sequences.
           #
           if (found == 1)
            {
              found = 0
              if (tmpchr == "a")
                tmptxt = tmptxt "ä"
              else
               {
                 if (tmpchr == "e")
                   tmptxt = tmptxt "ë"
                 else
                  {
                    if (tmpchr == "i")
                      tmptxt = tmptxt "ï"
                    else
                     {
                       if (tmpchr == "o")
                         tmptxt = tmptxt "ö"
                       else
                        {
                          if (tmpchr == "u")
                            tmptxt = tmptxt "ü"
                          else
                           {
                             if (tmpchr == "A")
                               tmptxt = tmptxt "Ä"
                             else
                              {
                                if (tmpchr == "O")
                                  tmptxt = tmptxt "Ö"
                                else
                                 {
                                   if (tmpchr == "U")
                                     tmptxt = tmptxt "Ü"
                                   else
                                    {
                                      if (tmpchr == "s")
                                        tmptxt = tmptxt "ß"
                                      else
                                        tmptxt = tmptxt "\"" tmpchr
                                    }
                                 }
                              }
                           }
                        }
                     }
                  }
               }
            }
           else
             tmptxt = tmptxt tmpchr
         }
      }
     if (found == 1)
       print tmptxt tmpchr
     else
       print tmptxt
   }
  else
    print the_line
}
