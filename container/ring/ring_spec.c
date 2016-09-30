#include <stdio.h>
#include <stdatomic.h>
#include <assert.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include "cdata.h"
#include "ring.h"

ring_t global_queue_node;
_Atomic int node_begin_flag = ATOMIC_VAR_INIT(0);

_Atomic int queue_begin_flag = ATOMIC_VAR_INIT(0);

typedef void* (*pthread_func_t) (void*);

void* benchmark_node(void *arg) {
	while( !atomic_load(&node_begin_flag)) {
	}

	cdata_t data;

	// warm up
	for (int i=0; i<1000; ++i) {
		ring_push(global_queue_node, 23);
		ring_pull(global_queue_node, &data);
	}

	clock_t b = clock();
	size_t ntimes = 10000000;
	for (int i=0; i<ntimes; ++i) {
		ring_push(global_queue_node, 23);
		ring_pull(global_queue_node, &data);
	}
	clock_t e = clock();
	printf("[%llu] node_bench push %d  pull %d\n", pthread_self(), ntimes, ntimes);
	printf("[%llu] node_bench begin time: %d us\n", pthread_self(), b);
	printf("[%llu] node_bench end   time: %d us\n", pthread_self(), e);
	printf("[%llu] node_bench uesd  time: %d us\n", pthread_self(), e - b);
	printf("[%llu] node_bench speed %.2f op/sec\n", pthread_self(), ntimes*((double)CLOCKS_PER_SEC/(double)(e-b)));
	return (void*)0;
}

void create_thread(int num, pthread_func_t func, _Atomic int *begin_flag) {
	pthread_t *tid = (pthread_t*)malloc(num*sizeof(pthread_t));
	for(int i=0; i<num; ++i) {
		pthread_create(&tid[i], NULL, func, NULL);
	}
	if (begin_flag) {
		atomic_store(begin_flag, 1);
	}
	for(int i=0; i<num; ++i) {
		pthread_join(tid[i], NULL);
	}
}


void verify_cqueue_node() {
	ring_t queue_node = NULL;
	ring_create(&queue_node, 128);
	assert(queue_node != NULL);
	printf("addr: %llu\n", queue_node);
	ring_push(queue_node, 5);
	ring_push(queue_node, 4);
	ring_push(queue_node, 3);

	cdata_t data;
	ring_pull(queue_node, &data);
	printf("data :%d\n", data.i);
	ring_pull(queue_node, &data);
	printf("data :%d\n", data.i);
	ring_pull(queue_node, &data);
	printf("data :%d\n", data.i);

	printf("CLOCKS_PER_SEC:%d\n", CLOCKS_PER_SEC);
	//ring_delete(&queue_node);

}




void prepare_env() {
	srand((unsigned int)time(NULL));
	ring_create(&global_queue_node, 128);
}

int main(int argc, char* argv[]) {
	prepare_env();
	create_thread(1, benchmark_node, &node_begin_flag);
	return 0;
}

