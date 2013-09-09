/*
 * Definitions etc. for regexp(3) routines.
 *
 * Caveat:  this is V8 regexp(3) [actually, a reimplementation thereof],
 * not the System V one.
 *
 * Modified by Thomas Esken <esken@uni-muenster.de>, 17-Sep-1996, for Gcal.
 */

#ifndef _REGEXP
#define _REGEXP 1

#define NSUBEXP  10
typedef struct regexp {
    char *startp[NSUBEXP];
    char *endp[NSUBEXP];
    char regstart;      /* Internal use only. */
    char reganch;       /* Internal use only. */
    char *regmust;      /* Internal use only. */
    int regmlen;        /* Internal use only. */
    char program[1];    /* Unwarranted chumminess with compiler. */
} regexp;

IMPORT regexp *regcomp __P_((char *exp));
IMPORT int regexec __P_((regexp *prog, char *string));
IMPORT void regsub __P_((regexp *prog, char *source, char *dest));
IMPORT void regerror __P_((char *msg));

#endif /* REGEXP */
