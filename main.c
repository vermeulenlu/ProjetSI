#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "./libfractal/fractal.h"
#include <pthread.h>
#include <semaphore.h>

//////////////////////////////////////////////////
//////////////////Déclarations////////////////////
//////////////////////////////////////////////////

void *LireFichier(void *item);
void *Consom_fractal(void *item);

struct fractal **buffer1;
struct fractal *MM=NULL;

int isEmpty=0;
int doAll=0;
int isReading=0;
int nbrFract=0;

pthread_mutex_t mutex;
pthread_mutex_t mutex2;
pthread_mutex_t MoyenneX;
pthread_mutex_t Compteur;
sem_t empty;
sem_t full;
pthread_t *producteur;
pthread_t *consommateur;

//////////////////////////////////////////////////
//////////////Fonctions Utiles////////////////////
//////////////////////////////////////////////////

 double Calcul_fractale(struct fractal *f)
{
  int width=0;
  int height=0;
  int val=0;
  double moyenne=0.0;
  width = fractal_get_width(f);
  height = fractal_get_height(f);
  int i;
  int j;
  for(i=0;i<width;i++)
  {
    for(j=0;j<height;j++)
    {
      val=fractal_compute_value(f,i,j);
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

void *LireFichier(void * item)
{
  struct fractal *Runner=NULL;
  char* fichier = (char*) item;
  char chaine[100];
  char chaine2[100];
  int count=0;
  int max_length = 100;
  FILE* texte=NULL;
  texte = fopen(fichier,"r");
  if(texte!=NULL)
  {
    while(fgets(chaine,100,texte) != NULL)
    {
      if(chaine[0]!='#' && chaine[0]!='\n')
      {
        count++;
      }
    }
    pthread_mutex_lock(&Compteur);
    nbrFract=nbrFract+count;
    pthread_mutex_unlock(&Compteur);
    char tabName[count][64];
    int tabWidth[count];
    int tabHeight[count];
    double taba[count];
    double tabb[count];
    const char N1[64]={0};
    int N2=0;
    int N3=0;
    double N4=0.0;
    double N5=0.0;
    int j=0;
    int i=0;
    rewind(texte);
    while(fgets(chaine2,max_length,texte)!=NULL)
    {
      if(chaine2[0]!='#' && chaine2[0]!='\n')
      {
        sscanf(chaine2,"%s %d %d %lf %lf",N1,&N2,&N3,&N4,&N5);
        strcpy(tabName[i],N1);
        tabWidth[i]=N2;
        tabHeight[i]=N3;
        taba[i]=N4;
        tabb[i]=N5;
        i++;
      }
    }
   for(i=0;i<count;i++)
   {
     Runner=fractal_new(tabName[i],tabWidth[i],tabHeight[i],taba[i],tabb[i]);
     sem_wait(&empty);
     pthread_mutex_lock(&mutex);
     int t=1;
     int j;
     for(j=0;j<t;j++)
     {
     if(buffer1[j]==NULL)
       {
         buffer1[j]=Runner;
         isEmpty++;
         printf("Insertion de %s dans le buffer\n",fractal_get_name(Runner));
         Runner=NULL;
       }
      else
      {
        t++;
      }
     }
     pthread_mutex_unlock(&mutex);
     sem_post(&full);
   }
 }
 int err= fclose(texte);
 if(err!=0)
 {
   fprintf(stderr, "Impossible de fermer le fichier");
 }
 if(texte==NULL)
 {
   fprintf(stderr, "impossible d'ouvrir le fichier");
 }
 pthread_mutex_lock(&mutex2);
 if(isReading>0)
 {
   isReading--;
 }
 pthread_mutex_unlock(&mutex2);
}

/////////////////////////////////////////
///////////////CONSOMMATEUR//////////////
/////////////////////////////////////////

void *Consom_fractal(void *item)
{
  struct fractal *Runner=NULL;

  while(isEmpty!=0 || isReading!=0)
  {
    if(isEmpty !=0)
    {
      sem_wait(&full);
      pthread_mutex_lock(&mutex);
      int t=1;
      int i;

      for(i=0;i<t;i++)
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
      printf("%s est calculée \n",fractal_get_name(Runner));
      if(doAll==1)
      {
        const char *lefichier;
        lefichier = fractal_get_name(Runner);
        write_bitmap_sdl(Runner,lefichier);
      }
      else
      {
        if(MM==NULL)
        {
          MM=Runner;
        }
        else
        {
          pthread_mutex_lock(&MoyenneX);
          if(fractal_get_moyenne(MM)<fractal_get_moyenne(Runner))
          {
            fractal_free(MM);
            MM=Runner;
          }
          else
          {
            fractal_free(Runner);
          }
          pthread_mutex_unlock(&MoyenneX);
        }
      }
    }
 }
  return (void*) MM;
}

///////////////////////////////////////////////////////////
///////////////////////Main////////////////////////////////
///////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{
  int j=0;
  int nbrFichier = 0;
  int maxThread = 1;
  char * OutPut = argv[argc-1];
  char* NomFichier[argc];

  int i=1;
  for(i=1;i<argc-1;i++)
  {
    if(strcmp(argv[i],"-d")==0)
    {
      doAll=1;
    }
    else if(strcmp(argv[i],"--maxthreads")==0)
    {
      maxThread=atoi(argv[i+1]);
      i++;
    }
    else
    {
      NomFichier[nbrFichier]=argv[i];
      nbrFichier++;
      isReading++;
    }
  }

  int err;

  //Initialisation des sémaphores
  sem_init(&empty,0,maxThread);
  sem_init(&full,0,0);

  //Initaialistion du mutex
  err=pthread_mutex_init(&mutex,NULL);
  if(err!=0)
  {
    fprintf(stderr, "Erreur dans l'initialisation du mutex\n");
  }
  err=pthread_mutex_init(&mutex2,NULL);
  if(err!=0)
  {
    fprintf(stderr, "Erreur dans l'initialisation du mutex2\n");
  }

  err=pthread_mutex_init(&MoyenneX,NULL);
  if(err!=0)
  {
    fprintf(stderr, "Erreur dans l'initialisation du Moyenne\n");
  }

  err=pthread_mutex_init(&Compteur,NULL);
  if(err!=0)
  {
    fprintf(stderr, "Erreur dans l'initialisation du mutex Compteur\n");
  }

  //Création du buffer
  buffer1=(struct fractal **) malloc(100*sizeof(struct fractal *));

  //tableux de threads en fonction du nombre de fichier et maxthreads
  pthread_t* producteur = (pthread_t*)malloc(nbrFichier*sizeof(pthread_t));
  pthread_t* consommateur = (pthread_t*)malloc(maxThread*sizeof(pthread_t));

  //Création des threads de lecture
  for(j=0; j<nbrFichier; j++)
  {
    err=pthread_create(&(producteur[j]),NULL,LireFichier,(void *) NomFichier[j]);
    if(err!=0)
    {
      fprintf(stderr, "Erreur dans la création du thread de lecture\n");
    }
  }
  //Création des threads de calcul
  for(j=0; j<maxThread; j++)
  {
    err=pthread_create(&consommateur[j],NULL,Consom_fractal,NULL);
    if(err!=0)
    {
      fprintf(stderr, "Erreur dans la création du thread de calcul\n");
    }
  }
  //Attente des calculs de fractales et sauvegarde de la moyenne la plus hauteur
   int err2;
   for(j=0;j<maxThread;j++)
   {
     err2=pthread_join(consommateur[j],NULL);
     printf("Valeur du thread n°%d retournée\n",j+1);
     if(err2!=0)
     {
       fprintf(stderr, "Erreur dans le join\n");
     }
   }
   if(doAll==0)
   {
     write_bitmap_sdl(MM,OutPut);
     printf("On imprime %s car il possède la meilleure moyenne\n",fractal_get_name(MM));
     pthread_mutex_destroy(&mutex);
   }
   else
   {
     printf("Toutes les fractales ont été affichées car vous avez passez l'argument -d\n");
   }
   free(buffer1);
   free(consommateur);
   free(producteur);
   pthread_mutex_destroy(&mutex);
   pthread_mutex_destroy(&mutex2);
   pthread_mutex_destroy(&MoyenneX);
   pthread_mutex_destroy(&Compteur);
   sem_destroy(&full);
   sem_destroy(&empty);
return 0;

}
