#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

#include "cdata.h"
#include "ring.h"
#include "ring_struct.h"

ring_t global_queue_node;
int node_begin_flag = 0;

typedef void* (*pthread_func_t) (void*);

void* benchmark_node(void *arg) {

	cdata_t data;

	// warm up
	for (int i=0; i<1000; ++i) {
		ring_push(global_queue_node, 23);
		ring_pull(global_queue_node, &data);
	}

    ring_realloc(global_queue_node, 13);
    ring_realloc(global_queue_node, 132);
    ring_realloc(global_queue_node, 313);
    ring_realloc(global_queue_node, 513);
    ring_realloc(global_queue_node, 613);
    ring_realloc(global_queue_node, 1113);

	clock_t b = clock();
	size_t ntimes = 2000000;
	for (int i=0; i<ntimes; ++i) {
		ring_push(global_queue_node, i);
	}
	for (int i=0; i<ntimes; ++i) {
		ring_pull(global_queue_node, &data);
        printf("data : %d\n", data.i);
        assert(data.i == i);
	}
	clock_t e = clock();

	printf("[%llu] node_bench push %d  pull %d\n", pthread_self(), ntimes, ntimes);
	printf("[%llu] node_bench begin time: %d us\n", pthread_self(), b);
	printf("[%llu] node_bench end   time: %d us\n", pthread_self(), e);
	printf("[%llu] node_bench uesd  time: %d us\n", pthread_self(), e - b);
	printf("[%llu] node_bench speed %.2f op/sec\n", pthread_self(), ntimes*((double)CLOCKS_PER_SEC/(double)(e-b)));
	return (void*)0;
}

void create_thread(int num, pthread_func_t func, int *begin_flag) {
	pthread_t *tid = (pthread_t*)malloc(num*sizeof(pthread_t));
	for(int i=0; i<num; ++i) {
		pthread_create(&tid[i], NULL, func, NULL);
	}
	if (begin_flag) {
        *begin_flag = 1;
	}
	for(int i=0; i<num; ++i) {
		pthread_join(tid[i], NULL);
	}
}





void prepare_env() {
	srand((unsigned int)time(NULL));
	ring_create(&global_queue_node, 28);
}

int verify_ring_push_pull() {
    puts("verify ring push pull ....!");
    cdata_t data;
    ring_t ring;
    ring_create(&ring, 28);
    assert(ring != NULL);

    ring_push(ring, 5);
    ring_pull(ring, &data);
    assert(data.i == 5);

    ring_push(ring, 5);
    ring_push(ring, 4);
    ring_push(ring, 3);
    ring_pull(ring, &data);
    assert(data.i == 5);
    ring_pull(ring, &data);
    assert(data.i == 4);
    ring_pull(ring, &data);
    assert(data.i == 3);
    puts("verify ring push pull pass!");
    return 1;
}

int verify_ring_realloc() {
    puts("verify ring realloc ....!");
    cdata_t data;
    ring_t ring;
    ring_create(&ring, 5);
    assert(ring != NULL);
    assert(ring->capc == 7);

    puts("verify ring realloc case[1] ....!");
    ring_push(ring, 23);
    ring_push(ring, 24);
    ring_push(ring, 25);
    ring_realloc(ring, 51);
    assert(ring->capc == 63);
    ring_pull(ring, &data);
    assert(data.i == 23);
    ring_pull(ring, &data);
    assert(data.i == 24);
    ring_pull(ring, &data);
    assert(data.i == 25);
    puts("verify ring realloc case[1] pass!");

    puts("verify ring realloc case[2] ....!");
    ring_push(ring, 23);
    ring_pull(ring, &data);
    assert(data.i == 23);

    ring_push(ring, 24);
    ring_push(ring, 25);
    ring_realloc(ring, 110);
    assert(ring->capc == 127);
    ring_pull(ring, &data);
    assert(data.i == 24);
    ring_pull(ring, &data);
    assert(data.i == 25);
    ring_delete(&ring);
    puts("verify ring realloc case[2] pass!");

    puts("verify ring realloc case[3] ....!");
    ring_create(&ring, 7);
    assert(ring->capc == 7);
    for (int i=0; i<3; ++i) {
        ring_push(ring, i);
        ring_pull(ring, &data);
    }
    for (int i=0; i<3; ++i) {
        ring_push(ring, i);
        ring_push(ring, i+1);
        ring_push(ring, i+2);
        ring_pull(ring, &data);
    }
    assert(ring->size == 6);
    assert(ring->tail < ring->head);
    assert(ring->head == 6);
    assert(ring->data[ring->head].i == 1);
    assert(ring->data[ring->head+1].i == 2);

    ring_realloc(ring, 15);
    assert(ring->size == 6);
    assert(ring->capc == 15);
    assert(ring->tail < ring->head);
    assert(ring->head == 14);
    assert(ring->data[ring->head].i == 1);
    assert(ring->data[ring->head+1].i == 2);
    puts("verify ring realloc case[3] pass!");

    puts("verify ring realloc pass!");
    return 1;
}

int main(int argc, char* argv[]) {
    assert(verify_ring_push_pull());
    assert(verify_ring_realloc());
    return 0;
}

