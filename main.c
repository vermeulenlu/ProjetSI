#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "./libfractal/fractal.h"


///////////////////////////////////////////////////////////////////////
/////// Fonction utiles pour la créations des buffers /////////////////
///////////////////////////////////////////////////////////////////////

typedef struct {
  fractal *buffer1;
  int n;
  int front;
  int back;
  sem_t mutex;
  sem_t places;
  sem_t items;
} sbuf_t;

void initialisation_buffer(sbuf_t *b, int n)
{
  b->buffer1 = calloc(n, sizeof(fractal));
  b->n=n;
  b->front=0;
  b->back=0;
  sem_init(&b->mutex,0,1);
  sem_init(&b->places,0,n);
  sem_init(&b->items,0,0);
}

void insert(sbuf_t *b, const struct fractal *f)
{
  sem_wait(&(b->places));
  sem_wait(&(b->mutex));
  b->back=((b->back)+1)%(b->n);
  b->buffer1[b->rear]=f;
  sem_post(&(b->mutex));
  sem_post(&(b->items));
}

fractal remove(sbuf_t *b)
{
  sem_wait(&(b->items));
  sem_wait(&(b->mutex));
  b->front=((b->front)+1)%(b->n);
  fractal *f = b-> buffer1[b->front];
  sem_post(&(b->mutex));
  sem_post(&(b->places));
  return f;
}

//////////////////////////////////////////////////////
//////////////Calcul des fractales////////////////////
//////////////////////////////////////////////////////

void Calcul_fractale(const struct fractal *f)
{
  int width=0;
  int height=0;
  int val=0;
  width = fractal_get_width(f);
  height = fractal_get_height(f);
  for(int i=0;i<width;i++)
  {
    for(int j=0;j<height;j++)
    {
      val=fractal_get_value(f,x,y);
      fractal_set_value(f,x,y,val);
    }
  }
}

void BuffCalcul(sbuf_t *b1 , sbuf_t *b2)
{
  fractal *f = remove(b1);
  Calcul_fractale(f);
  insert(b2,f);
}



int main(int argc, char **argv)
{

}
