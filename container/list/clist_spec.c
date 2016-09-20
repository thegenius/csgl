#include <stdio.h>
#include <assert.h>
#include "clist.h"


int clist_create_spec(){
	clist_t *clist = clist_create();
	int pass = 0;
	pass = (clist == clist->prev);
	pass = (clist == clist->next);
	if (pass) {
		printf("clist_create() specification pass!\n");
	} else {
		printf("clist_create() specification fail!\n");
	}
	return pass;
}

int clist_push_back_spec() {
	clist_t *clist = clist_create();
	clist_push_back(clist, 10);
	clist_push_back(clist, 20);
	clist_push_back(clist, 30);
	int pass = 0;
	pass = (clist->data.sz == 3);
	printf("clist_push_back(): size :%d\n", clist->data.sz);
	pass = (clist->next->data.i == 10);
	printf("clist_push_back(): elem[0] :%d\n", clist->next->data.i);
	pass = (clist->next->next->data.i == 20);
	printf("clist_push_back(): elem[1] :%d\n", clist->next->next->data.i);
	pass = (clist->next->next->next->data.i == 30);
	printf("clist_push_back(): elem[2] :%d\n", clist->next->next->next->data.i);

	if (pass) {
		printf("clist_push_back() specification pass!\n");
	} else {
		printf("clist_push_back(): clist :%llu\n", clist);
		printf("clist_push_back() specification fail!\n");
	}
	return 0;
}

int clist_pop_front_spec() {
	clist_t *clist = clist_create();
	clist_push_back(clist, 10);
	clist_push_back(clist, 20);
	clist_push_back(clist, 30);
	int pass = 0;
	pass = (clist->data.sz == 3);
	pass = (clist_pop_front(clist).i == 10);
	pass = (clist_pop_front(clist).i == 20);
	pass = (clist_pop_front(clist).i == 30);

	if (pass) {
		printf("clist_pop_front() specification pass!\n");
	} else {
		printf("clist_pop_front() specification fail!\n");
	}
	return 0;
}
int clist_destroy_spec(){
	clist_t *clist = clist_create();
	clist_destroy(&clist);
	int pass = 0;
	pass = (clist == NULL);

	if (pass) {
		printf("clist_destroy() specification pass!\n");
	} else {
		printf("clist_destroy(): clist :%llu\n", clist);
		printf("clist_create() specification fail!\n");
	}
	return pass;
}


int main(int argc, const char* argv[]) {
	clist_create_spec();
	clist_push_back_spec();
	clist_pop_front_spec();
	clist_destroy_spec();
	return 0;
}
