#include <stdlib.h>
#include "/etinfo/users/2018/jhouyoux/Desktop/project_2018_template/libfractal/fractal.h"
#include <string.h>


struct fractal *fractal_new(const char *name, int width, int height, double a, double b)
{
    struct fractal *f = (struct fractal *) malloc(sizeof(struct fractal));
    if (!f)
    {
        return NULL;
    }
    double moyenne=0.0;
    char *n = (char*) malloc(strlen(name)*sizeof(char));
    strcpy(n,name);
    f->name=n;
    f->width = width;
    f->height = height;
    f->a = a;
    f->b = b;
    int **tab;
    tab=(int **)malloc(width*sizeof(int *));
    if(!tab)
    {
      return NULL;
    }
    int i;
    for(i=0;i<width;i++)
    {
      tab[i]=(int *)malloc(height*sizeof(int));
      if(!tab)
      {
        return NULL;
      }
    }
    f->tabValue = tab;
    f->moyenne = moyenne;
    return f;
}

void fractal_free(struct fractal *f)
{
    free(f);
}

const char *fractal_get_name(const struct fractal *f)
{
    return f->name;
}

int fractal_get_value(const struct fractal *f, int x, int y)
{
    return f->tabValue[x][y];
}

void fractal_set_value(struct fractal *f, int x, int y, int val)
{
  f->tabValue[x][y] = val;
}

int fractal_get_width(const struct fractal *f)
{
    return f->width;
}

int fractal_get_height(const struct fractal *f)
{
    return f->height;
}

double fractal_get_a(const struct fractal *f)
{
    return f->a;
}

double fractal_get_b(const struct fractal *f)
{
   return f->b;
}

void fractal_set_moyenne(struct fractal *f, double val)
{
  f->moyenne=val;
}

double fractal_get_moyenne(struct fractal *f)
{
  return f->moyenne;
}
