#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <pthread.h>

typedef uint32_t u32;

void sleep_ms(u32 ms) { usleep(1000 * ms); }

void* thread_routine1(void* args) {
	(void)args;
	printf("hello from thread1\n");
	sleep_ms(2000);
	printf("thread 1 not done\n");
	sleep_ms(1000);
	printf("thread 1 done\n");
	return NULL;
}

void* thread_routine2(void* args) {
	(void)args;
	printf("hello from thread2\n");
	sleep_ms(2000);
	printf("thread 2 done\n");
	return NULL;
}

int main(void) {
	pthread_t thread1;
	pthread_t thread2;

	pthread_create(&thread1, NULL, &thread_routine1, NULL);
	pthread_create(&thread2, NULL, &thread_routine2, NULL);

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
}
