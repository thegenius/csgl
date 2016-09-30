header_dir=../../utils/cdata
opt_flag=-O3

all: rbtree_lib rbtree_spec rbtree_bnch stlmap_bnch

rbtree_lib: rbtree_static_lib 

rbtree_static_lib: rbtree.h rbtree.c
	gcc -I${header_dir} -c ${opt_flag} -std=c11 rbtree.c -o rbtree.o
	ar rcs librbtree.a rbtree.o

rbtree_dynamic_lib:rbtree.h rbtree.c
	gcc -I${header_dir} -c ${opt_flag} -std=c11 -fPIC rbtree.c -o rbtree.o
	gcc -shared -o librbtree.so rbtree.o
	rm rbtree.o
	
rbtree_spec : rbtree_static_lib rbtree_spec.c
	gcc -I${header_dir} -c ${opt_flag} -std=c11 rbtree_spec.c -o rbtree_spec.o
	gcc -lpthread -o  rbtree_spec rbtree_spec.o librbtree.a 
	rm rbtree_spec.o
rbtree_bnch : rbtree_static_lib rbtree_bnch.c
	gcc -I${header_dir} -c ${opt_flag} -std=c11 rbtree_bnch.c -o rbtree_bnch.o
	gcc -lpthread -o  rbtree_bnch rbtree_bnch.o librbtree.a
	rm rbtree_bnch.o
stlmap_bnch : stlmap_bnch.cpp
	g++ -o stlmap_bnch ${opt_flag} stlmap_bnch.cpp 
