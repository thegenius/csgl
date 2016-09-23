all: bsearch_lib bsearch_spec

bsearch_lib: bsearch_static_lib bsearch_dynamic_lib

bsearch_static_lib: bsearch.h bsearch.c
	gcc -c -std=c11 bsearch.c -o bsearch.o
	ar rcs libbsearch.a bsearch.o
	rm bsearch.o

bsearch_dynamic_lib:bsearch.h bsearch.c
	gcc -c -std=c11 -fPIC bsearch.c -o bsearch.o
	gcc -shared -o libbsearch.so bsearch.o
	rm bsearch.o
	
bsearch_spec : bsearch_static_lib bsearch_spec.c
	gcc -c -std=c11 bsearch_spec.c -o bsearch_spec.o
	gcc -lpthread -o  bsearch_spec bsearch_spec.o libbsearch.a
	rm bsearch_spec.o
