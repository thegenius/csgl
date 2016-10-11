all: itrie_lib itrie_spec itrie_bnch

itrie_lib: itrie_static_lib itrie_dynamic_lib

itrie_static_lib: itrie.h itrie.c
	gcc -I../../utils/cdata -c -std=c11 itrie.c -o itrie.o
	ar rcs libitrie.a itrie.o
	rm itrie.o

itrie_dynamic_lib:itrie.h itrie.c
	gcc -I../../utils/cdata -c -std=c11 -fPIC itrie.c -o itrie.o
	gcc -shared -o libitrie.so itrie.o
	rm itrie.o
	
itrie_spec : itrie_static_lib itrie_spec.c
	gcc -I../../utils/cdata -c -std=c11 itrie_spec.c -o itrie_spec.o
	gcc -lpthread -o  itrie_spec itrie_spec.o libitrie.a
	rm itrie_spec.o

itrie_bnch : itrie_static_lib itrie_bnch.c
	gcc -I../../utils/cdata -c -std=c11 itrie_bnch.c -o itrie_bnch.o
	gcc -lpthread -o  itrie_bnch itrie_bnch.o libitrie.a
	rm itrie_bnch.o
