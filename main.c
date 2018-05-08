#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "./libfractal/fractal.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "./libfractal/fractal.h"
#include <pthread.h>
#include <semaphore.h>


void *LireFichier(void *item);
void *CalculFractale(void *item);

struct fractal **buffer1;

int isEmpty=0;
int doAll=0;

pthread_mutex_t mutex;
sem_t empty;
sem_t full;


//////////////////////////////////////////////////
//////////////Fonctions Utiles////////////////////
//////////////////////////////////////////////////

/*Calcul du tableau de pixel du tableau*/
double Calcul_fractale(const struct fractal *f)
{
  int width=0;
  int height=0;
  int val=0;
  double moyenne=0.0;
  width = fractal_get_width(f);
  height = fractal_get_height(f);
  for(int i=0;i<width;i++)
  {
    for(int j=0;j<height;j++)
    {
      val=fractal_get_value(f,i,j);
      fractal_set_value(f,i,j,val);
      moyenne=moyenne+val;
    }
  }
  moyenne=moyenne/(width*height);
  return moyenne;
}


/////////////////////////////////////////
///////////////PRODUCTEUR////////////////
/////////////////////////////////////////
void


/////////////////////////////////////////
///////////////CONSOMMATEUR//////////////
/////////////////////////////////////////
void *CalculFractale(void *item)
{
  const struct fractal *Runner=NULL;

  while(isEmpty!=0)
  {
    sem_wait(&full);
    pthread_mutex_lock(&mutex);
    int t=1;
    for(int i=0;i<t;i++)
    {
      if(buffer1[i]!=NULL)
      {
        Runner=buffer1[i];
        isEmpty--;
        buffer1[i]=NULL;
      }
      else
      {
        t++;
      }
    }
    pthread_mutex_unlock(&mutex);
    sem_post(&empty);
    double moyenne = Calcul_fractale(Runner);
    fractal_set_moyenne(Runner,moyenne);
    if(doAll==1)
    {
      char *name = "";
      strcpy(name,fractal_get_name(Runner));
      write_bitmap_sdl(Runner,name);
    }
  }
  return (void*) Runner;
}


int main(int argc, char **argv)
{

}


int main(int argc, char **argv)
{
  int nbrFichier = 0;
  int doAll = 0;
  int maxThread = 1;
  char NomFichier[argc-1];


  int i=1;
  for(i=1;i<argc;i++){
    if(strcmp(argv[i],"-d")==0){
      doAll=1;
    }
    else if(strcmp(argv[i],"--maxthreads")==0){
      maxThread=atoi(argv[i+1]);
      i++;
    }
    else{
        //c'est un fichier
        strcpy(&NomFichier[nbrFichier],argv[i]);
        nbrFichier++;

    }
  }
  printf("%s\n",NomFichier[0]);

  //tableux de threads en fonction du nombre de fichier
  //pthread_t* producteur = (pthread_t*)malloc(nbrFichier*sizeof(pthread_t));
  //char* NomFichier[nbrFichier];
  //int j=0;
  /*for(j=0; j<nbrFichier; j++){
    pthread_create(&(producteur[j]),NULL,LireFichier,(void *) &NomFichier[j])
  }*/
}
