#include <stdlib.h>
#include "fractal.h"


struct fractal *fractal_new(const char *name, int width, int height, double a, double b)
{
    struct fractal *f = malloc(sizeof(struct fractal));
    if (!f) {
        return NULL;
    }
    strcpy(f->name)=name;
    f->width = width;
    f->height = height;
    f->a = a;
    f->b = b;
    int tab[width][height];
    f->tabValue = tab;
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
    int val=0;
    val=fractal_compute_value(f,x,y);
    return val;
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
