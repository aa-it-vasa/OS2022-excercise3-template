#include <pthread.h>
#include <stdio.h>
#include <stdint.h>

void *print_hello(void *p)
{
	intptr_t thread_n;
	thread_n = (intptr_t)p;
	printf("Hello! I'm thread #%ti\n", thread_n);
	return NULL;
}

int main()
{
	pthread_t thread_ids[N_PTHREADS];
	int ret;
	intptr_t i;
	for (i = 0; i < N_PTHREADS; i++) {
		printf("Creating thread #%ti\n", i);
		ret = pthread_create(&thread_ids[i], NULL, print_hello, (void *)i);
		if (ret != 0) {
			printf("ERROR: pthread_create() returned %i\n", ret);
			return -1;
		}
	}

	return 0;
}