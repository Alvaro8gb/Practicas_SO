#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int total_sum = 0;

void * partial_sum(void * arg) {
  int ni=((int*)arg)[0];
  int nf=((int*)arg)[1];
  int part_sum = 0;

  for (int j = ni; j <= nf; j++)
    part_sum = part_sum + j;

  total_sum+=part_sum;

  pthread_exit(0);
}

void distribut_job(pthread_t * threads,int tam, int n  ){

  int num_valores;
  int vals[2];
  
  num_valores = tam / n;

  for(int i = 0; i<n ; i++){
    vals[0] = num_valores * i;
    vals[1] = vals[0] + num_valores;

    pthread_create((pthread_t  *)threads[i], NULL, partial_sum, (void*)vals);
  }
    
  /* the main thread waits until both threads complete */
  for(int i = 0; i < n ; i++)
      pthread_join(threads[i], NULL);

}

int main(int argc, char ** argv) {
  pthread_t * threads = NULL;
  int num_threads, expec_result;

  if (argc!=3){
		fprintf(stderr, "Usage: %s <num_threads> <expected result> \n", argv[0]);
		exit(1);
	}

	num_threads = atoi( argv[1] );
	expec_result = atoi( argv[2] );

  threads = malloc(num_threads * sizeof(pthread_t));

  distribut_job(threads,500000,num_threads);

  free(threads);

  printf("total_sum=%d and it should be %d\n", total_sum, expec_result);

  return 0;
}
