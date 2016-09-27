header_dir=../../utils/cdata

all: rbtree_lib rbtree_spec

rbtree_lib: rbtree_static_lib rbtree_dynamic_lib

rbtree_static_lib: rbtree.h rbtree.c
	gcc -I${header_dir} -c -std=c11 rbtree.c -o rbtree.o
	ar rcs librbtree.a rbtree.o
	rm rbtree.o

rbtree_dynamic_lib:rbtree.h rbtree.c
	gcc -I${header_dir} -c -std=c11 -fPIC rbtree.c -o rbtree.o
	gcc -shared -o librbtree.so rbtree.o
	rm rbtree.o
	
rbtree_spec : rbtree_static_lib rbtree_spec.c
	gcc -I${header_dir} -c -std=c11 rbtree_spec.c -o rbtree_spec.o
	gcc -lpthread -o  rbtree_spec rbtree_spec.o librbtree.a
	rm rbtree_spec.o
