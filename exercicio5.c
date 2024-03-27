
#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>


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
   int lim = ((pos +1)* args.passo -1);  // aqui temos que fazer algumas manipulações pois, primeiramente, o valor de IdThread começa em 0 e não em 1. Tal lim é o limitante do for, é aonde cada Thread deverá parar
   if(args.idThread +1 != args.nThreads){ // verificamos se a já é última Thread. Caso não executa o if.
    for(i = lim - (args.passo -1) ; i < lim+1 ; i++){
        vector[i] = vector[i]*vector[i];
     } 
   }
   else{                                   // Dado que é a última Thread, ela ficará com o restante do que não ficou pras outras Threads, no caso aonde a divisão do vetor não foi inteira. 
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
  
  if(atoi(argv[1]) < atoi(argv[2])){  
   NTHREADS = atoi(argv[1]);  // Aqui queremos verificar se o número de Threads é menor ou igual o número de elementos no vetor, para sabermos se precisamos criar todas as Threads pedidas 
  }                           // evitamos assim criar Threads sem uso
  else{
     NTHREADS = atoi(argv[2]);   // Neste condicional estamos tratando o caso aonde temos um número de Threads maior que o número de elementos no vetor
  }    
  pthread_t tid_sistema[NTHREADS]; //identificadores das threads no sistema
  t_Args *args; //receberá os argumentos para a thread
  
  tam = atoi(argv[2]);
  vector = create_vector(tam); // foi criada uma função para inicializar um vetor com base nos dados preenchidos pelo usuário
  vector_quad = quad_vector(vector, tam);  // função utilizada para calcular previamente o valor esperado do quadrado do vetor, porém de forma sequencial. Utilizaremos tal valor para conferir se a respota gerada pelas Threads foi a correta.
  if( vector == NULL)
    return 1;
   
  printf("O vetor original é: ");  
  for(int i = 0; i < tam; i++){
    printf("%d , ", vector[i]);
  }
  printf("\n");
  
  passo =  tam % NTHREADS;  // variável para conseguirmos calcular o pedaçõ do vetor separado para cada Thread

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
  
  verify_vector(vector,vector_quad, tam); // função de verificação utilizada para sabermos se as Threads fizeram um trabalho correto. Para tal verificação utilizamos o resultado calculado previamente pela função quad_vector, função sequancial, e verificamos o mesmo com a resposta gerada pelas Thread, com a programação concorrente.
  
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
    
    printf("Entre com os elementos do seu vetor de tamanho %d : ", tam);
    
    for(int i= 0;i <tam ; i++){
       scanf("%d", &vector[i]);
    
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
