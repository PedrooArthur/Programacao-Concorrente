
#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <time.h>

int *create_vector(int tam);
int verify_vector(int *vector, int *vector_quad, int tam);
int *quad_vector(int *vector, int tam);
//cria a estrutura de dados para armazenar os argumentos da thread
typedef struct {
   int idThread, nThreads, passo, tam;
   int *vector;
} t_Args;

//funcao executada pelas threads
void *Calcula_Quad (void *arg) {
  t_Args args = *(t_Args*) arg;
  int i;
  int *vector = args.vector;


   int pos = args.idThread;
   int lim = ((pos +1)* args.passo -1);
   if(args.idThread +1 != args.nThreads){ 
    for(i = lim - (args.passo -1) ; i < lim+1 ; i++){
        vector[i] = vector[i]*vector[i];
     } 
   }
   else{ 
    for(i = lim - (args.passo -1) ; i < args.tam ; i++){
        vector[i] = vector[i]*vector[i];
     }  
   }
   

  pthread_exit(NULL);
}

//funcao principal do programa
int main(int argc, char *argv[]) {
  int NTHREADS, passo , tam;
  int *vector;
  int *vector_quad;
  
  NTHREADS = atoi(argv[1]);
  pthread_t tid_sistema[NTHREADS]; //identificadores das threads no sistema
  t_Args *args; //receberá os argumentos para a thread
  
  tam = atoi(argv[2]);
  vector = create_vector(tam);
  vector_quad = quad_vector(vector, tam);
  if( vector == NULL)
    return 1;
   
  printf("O vetor original é: ");  
  for(int i = 0; i < tam; i++){
    printf("%d , ", vector[i]);
  }
  printf("\n");
  
  passo =  tam % NTHREADS;

  for(int i=0; i<NTHREADS; i++) {
    printf("--Aloca e preenche argumentos para thread %d\n", i);
    args = malloc(sizeof(t_Args));
    if (args == NULL) {
      printf("--ERRO: malloc()\n"); exit(-1);
    }
    args->idThread = i; 
    args->nThreads = NTHREADS; 
    args->vector = vector;
    args->passo = passo;
    args->tam = tam;
    
    printf("--Cria a thread %d\n", i);
    if (pthread_create(&tid_sistema[i], NULL, Calcula_Quad, (void*) args)) {
      printf("--ERRO: pthread_create()\n"); exit(-1);
    }
  }
  
    for (int i = 0; i < NTHREADS; i++) {
    if (pthread_join(tid_sistema[i], NULL)) {
      printf("Erro ao aguardar término da thread.\n");
      return 1;
    }
  }
  
  verify_vector(vector,vector_quad, tam);
  
  printf("O quadrado do vetor original é : ");
  for(int i = 0; i<tam; i++){
   printf("%d , " , vector[i]);
  }
  printf("--Thread principal terminou\n");
  free(vector);
  free(vector_quad);
  pthread_exit(NULL);
  return 0;
}



int *create_vector(int tam){
    int *vector;
    
    vector = malloc(sizeof(int)*tam);
    if( vector == NULL){
     return NULL;
    }
    srand((unsigned) time(NULL));
    for(int i= 0; i <tam; i++){
    
        vector[i] = (rand())%100;
    
    }
    
    return vector;
}

int *quad_vector(int *vector, int tama){
    int *vector_quad;
    int tam;
    tam = tama;
    vector_quad = malloc(sizeof(int)*tam);
    
    for(int i = 0; i < tam; i++){
      vector_quad[i] = vector[i]*vector[i];
    }

   return vector_quad;  
}

int verify_vector(int *vector_ori, int *vector_quad_ori, int tam){
       int *vector;
       int *vector_quad;
       
       vector = vector_ori;
       vector_quad = vector_quad_ori;
       
       for(int i = 0; i < tam; i++){
          if(vector[i] != vector_quad[i]){
          printf("Houve um erro");
             return 1; 
          }
       }
     printf("A operação foi realizada com sucesso");      
     printf("\n");
    return 0;
}
