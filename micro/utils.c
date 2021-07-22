#include "mp.h"

size_t ft_strlen(char *str)
{
  size_t i = 0;

  while (str && str[i])
    i++;

  return (i);
}

void  ft_error(char *str)
{
  write(1, str, ft_strlen(str));
  write(1, "\n", 1);
}