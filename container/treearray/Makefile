all: treearray_lib treearray_spec

treearray_lib: treearray_static_lib treearray_dynamic_lib

treearray_static_lib:
	gcc -c -std=c11 treearray.c -o treearray.o
	ar rcs libtreearray.a treearray.o
	rm treearray.o

treearray_dynamic_lib:treearray.h treearray.c
	gcc -c -std=c11 -fPIC treearray.c -o treearray.o
	gcc -shared -o libtreearray.so treearray.o
	rm treearray.o

treearray_spec : treearray_static_lib treearray_spec.c
	gcc -c -std=c11 treearray_spec.c -o treearray_spec.o
	gcc -lpthread -o  treearray_spec treearray_spec.o libtreearray.a
	rm treearray_spec.o
