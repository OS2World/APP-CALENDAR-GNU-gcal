#ifdef __50SERIES

#  include <stdio.h>

char
*getenv(const char *env_var)
/*
   Emulate getenv()-function for PRIME/Primos machines
*/
{
  static char  buf[256];
  extern char *gvget();


  buf[0] = '.';
  strcpy(buf+1, env_var);

  return(gvget(buf));
}
#endif /* __50SERIES */
