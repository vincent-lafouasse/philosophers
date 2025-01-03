#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

typedef uint32_t u32;

u32 ms_to_us(u32 ms) { return 1000 * ms; }
void sleep_ms(u32 ms) { usleep(1000 * ms); }

void* thread_routine1(void* args) {
	(void)args;
	printf("hello from thread1");
	sleep_ms(1000);
	printf("thread 1 not done");
	sleep_ms(500);
	printf("thread 1 done");
	return NULL;
}

void* thread_routine2(void* args) {
	(void)args;
	printf("hello from thread2");
	sleep_ms(1000);
	printf("thread 2 done");
	return NULL;
}

int main(void) {
}
