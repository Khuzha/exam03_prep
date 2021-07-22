#include "mp.h"

int fill_and_check_canvas(FILE *file, t_canvas *canvas)
{
  int read = fscanf(file, "%d %d %c\n", &canvas->w, &canvas->h, &canvas->c);

  if (read != 3)
    return 0;
  if (
    canvas->w < 0 || canvas->w > 300 ||
    canvas->h < 0 || canvas->h > 300
  ) {
    return 0;
  }

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

int check_target(t_target target)
{
  // printf("target.w = %f, target.h = %f, target.ty")
  if (
    target.w > 0.00000000 && target.h > 0.00000000 &&
    (target.type == 'r' || target.type == 'R')
  )
    return 1;
  else 
    return 0;
}

int   in_target(t_target target, int y, int x)
{
  if (
    y < target.y || y > target.y + target.h ||
    x < target.x || x > target.x + target.w
  )
    return 0;
  if (
    x - target.x < 1.00000000 || target.x + target.w - x < 1.00000000 ||
    y - target.y < 1.00000000 || target.y + target.h - y < 1.00000000
  )
    return 2;
  return 1;
}

void  add_target(char *bg, t_target target, t_canvas *canvas)
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
      if (
        (response && target.type == 'R') ||
        (response == 2 && target.type == 'r')
      )
        bg[i * canvas->w + j] = target.c;
      j++;
    }
    i++;
  }
}

int add_targets(char *bg, FILE *file, t_canvas *canvas)
{
  t_target tmp;
  int response;

  while ((response = fscanf(file, "%c %f %f %f %f %c\n", &tmp.type, &tmp.x, &tmp.y, &tmp.w, &tmp.h, &tmp.c)) == 6)
  {
    // printf("bef check_target\n");
    if (!check_target(tmp))
      return 0;
    // printf("bef add_target\n");
    add_target(bg, tmp, canvas);
  }
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
  t_canvas *canvas = calloc(1, sizeof(t_canvas));
  FILE     *file;
  char     *bg;

  if (argc != 2)
  {
    ft_error("Error: argument");
    return (1);
  }
  // printf("bef fopen\n");
  if (!(file = fopen(argv[1], "r")))
  {
    ft_error("Error: Operation file corrupted");
    return (1);
  }
  // printf("bef fill_and_check_canvas\n");
  if (!fill_and_check_canvas(file, canvas))
  {
    ft_error("Error: Operation file corrupted");
    return (1);
  }
  if (!(bg = fill_bg(canvas)))
  {
    ft_error("Error");
    return (1);
  }
  // printf("bg = %s\n", bg);
  // printf("bef add_targets\n");
  if (!add_targets(bg, file, canvas))
  {
    ft_error("Error: Operation file corrupted");
    return (1);
  }
  print_result(bg, canvas);
}
