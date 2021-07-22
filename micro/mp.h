#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

typedef struct  s_canvas {
  int   w;
  int   h;
  char  c;
}               t_canvas;

typedef struct  s_target {
  char  type;
  float x;
  float y;
  float w;
  float h;
  char  c;
}               t_target;

void  ft_error(char *str);