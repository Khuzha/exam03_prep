#include "mp.h"



int  check_and_fill_canvas(t_canvas *canvas, FILE *file)
{
  int response = fscanf(file, "%d %d %c\n", &canvas->w, &canvas->h, &canvas->c);

  // printf("bef resp = %d\n", response);
  if (response != 3)
    return 0;

  // printf("bef vals\n");

  if (
    canvas->w < 0 || canvas->w > 300 ||
    canvas->h < 0 || canvas->h > 300
  )
    return 0;

  return 1;
}

char *fill_bg(t_canvas *canvas)
{
  size_t  size = canvas->w * canvas->h;
  char    *res = calloc(1, sizeof(char) * size);
  size_t  i = 0;
  while (res && i < size)
  {
    res[i] = canvas->c;
    i++;
  }
  // printf("res = %s\n", res);

  return res;
}

int check_target(t_target tmp)
{
  // printf("tmp.r = %f, tmp.type = %c\n", tmp.r, tmp.type);
  if (
    tmp.r < 0.00000000 ||
    (tmp.type != 'c' && tmp.type != 'C')
  )
    return 0;
  else
    return 1;
}

int in_target(t_target target, int y, int x)
{
  float dist = sqrtf(powf(x - target.x, 2.) + powf(y - target.y, 2.));

  if (dist <= target.r)
  {
    if (target.r - dist < 1.00000000)
      return 2;
    return 1;
  }

  return 0;
}

void  add_target(t_canvas *canvas, char *bg, t_target target)
{
  int i = 0;
  int j;
  int response;

  while (i < canvas->h)
  {
    j = 0;
    while (j < canvas->w)
    {
      response = in_target(target, i, j);
      printf("response = %d\n", response);
      if (
        (response && target.type == 'C') ||
        (response == 2 && target.type == 'c')
      )
        bg[i * canvas->w + j] = target.c;
      j++;
    }
    i++;
  }
}

int add_targets(t_canvas *canvas, char *bg, FILE *file)
{
  t_target tmp;
  int response;

  while ((response = fscanf(file, "%c %f %f %f %c\n", &tmp.type, &tmp.x, &tmp.y, &tmp.r, &tmp.c)) == 5)
  {
    printf("bef check_target\n");
    if (!check_target(tmp))
      return 0;
    printf("bef add_target\n");
    add_target(canvas, bg, tmp);
  }
  printf("aft loop, response = %d\n", response);
  if (response != -1)
    return 0;

  return 1;
}

void  print_result(char *bg, t_canvas *canvas)
{
  int i = 0;

  while (i < canvas->h)
  {
    write(1, bg + i * canvas->w, canvas->w);
    write(1, "\n", 1);
    i++;
  }
}

int main(int argc, char **argv)
{
  char *bg;
  FILE *file;
  t_canvas *canvas;

  if (argc != 2)
  {
    ft_error("Error: argument");
    return (1);
  }
  if (!(canvas = calloc(1, sizeof(t_canvas))))
  {
    ft_error("Error");
    return (1);
  }
  if (!(file = fopen(argv[1], "r")))
  {
    printf("err in fopen\n");
    ft_error("Error: Operation file corrupted");
    return (1);
  }
  if (!check_and_fill_canvas(canvas, file))
  {
    printf("err in check_and_fill_canvas\n");
    ft_error("Error: Operation file corrupted");
    return (1);
  }
  if (!(bg = fill_bg(canvas)))
  {
    printf("err in fill_bg\n");
    ft_error("Error");
    return (1);
  }
  if (!add_targets(canvas, bg, file))
  {
    printf("err in add_targets\n");
    ft_error("Error: Operation file corrupted");
    return (1);
  }
  print_result(bg, canvas);
}