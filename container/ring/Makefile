all: ring_lib ring_spec

ring_lib: ring_static_lib ring_dynamic_lib

ring_static_lib: ring.h ring.c
	gcc -I../../utils/cdata -c -std=c11 ring.c -o ring.o
	ar rcs libring.a ring.o
	rm ring.o

ring_dynamic_lib:ring.h ring.c
	gcc -I../../utils/cdata -c -std=c11 -fPIC ring.c -o ring.o
	gcc -shared -o libring.so ring.o
	rm ring.o
	
ring_spec : ring_static_lib ring_spec.c
	gcc -I../../utils/cdata -c -std=c11 ring_spec.c -o ring_spec.o
	gcc -lpthread -o  ring_spec ring_spec.o libring.a
	rm ring_spec.o
