#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

int total_sum = 0;
sem_t sem;

typedef struct{
  int ini;
  int fin;
}Range;

void * partial_sum(void * arg) {
  Range * range = (Range *) arg ;
  int part_sum = 0;
  int ni,nf;

  ni = range->ini;
  nf = range->fin;

  printf("Thread responsible of : %d , %d\n",ni,nf);

  for (int j = ni; j <= nf; j++)
    part_sum = part_sum + j;


  if (sem_wait(&sem) == -1) perror("sem_wait");
  // Critical Region
  total_sum+=part_sum;
  // End of Critical Region
  if (sem_post(&sem) == -1) perror("sem_post");

  pthread_exit(0);
}

void distribut_job(pthread_t * threads, int n, int tam  ){

  int num_values;
  Range * args;
  
  if( n < 1){
      perror("number of threads shoud be > 1");
      exit(-1);
  }

  num_values = tam / n;

  args = malloc(sizeof(Range) * n);

  for(int i = 0; i < n ; i++){

    args[i].ini = num_values * i + 1;
    args[i].fin = args[i].ini + num_values - 1;
  }

  args[n-1].fin += tam%n;
  
  /* create threads*/
  for(int i = 0; i < n ; i++)
    pthread_create(&threads[i], NULL, partial_sum, (void*)&args[i]);
  
    
  /* the main thread waits until both threads complete */
  for(int i = 0; i < n ; i++)
    pthread_join(threads[i], NULL);

  free(args);

}

int main(int argc, char * argv[]) {
  pthread_t * threads = NULL;
  int num_threads, tam;

  if (argc!=3){
		fprintf(stderr, "Usage: %s <num_threads> <tam> \n", argv[0]);
		exit(1);
	}

	num_threads = atoi( argv[1] );
	tam = atoi( argv[2] );

  printf("###\nWorking with %d threads.\n",num_threads);

  threads = (pthread_t  *) malloc(num_threads * sizeof(pthread_t));

  if ( -1 == sem_init(&sem, 0, 1)) {
    perror("sem_init");
    exit(-1);
  }

  distribut_job(threads, num_threads, tam);

  free(threads);
  sem_destroy(&sem);

  printf("total_sum = %d \n", total_sum);

  printf("###\n");

  return 0;
}
