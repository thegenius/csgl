header_dir=../utils/cdata
header_dir2=../container/ring
opt_flag=-O3 -g

all: allocator_lib allocator_spec allocator_bnch
 
allocator_lib: allocator_static_lib 

allocator_static_lib: allocator.h allocator.c
	gcc -I${header_dir} -I${header_dir2} -c ${opt_flag} -std=c11 allocator.c -o allocator.o
	ar rcs liballocator.a allocator.o

allocator_dynamic_lib:allocator.h allocator.c
	gcc -I${header_dir} -I${header_dir2} -c ${opt_flag} -std=c11 -fPIC allocator.c -o allocator.o
	gcc -shared -o liballocator.so allocator.o -g
	rm allocator.o
	
allocator_spec : allocator_static_lib allocator_spec.c
	gcc -I${header_dir} -I${header_dir2} -c ${opt_flag} -std=c11 allocator_spec.c -o allocator_spec.o
	gcc -lpthread -o  allocator_spec allocator_spec.o liballocator.a libring.a -g
	rm allocator_spec.o
allocator_bnch : allocator_static_lib allocator_bnch.c
	gcc -I${header_dir} -I${header_dir2} -c ${opt_flag} -std=c11 allocator_bnch.c -o allocator_bnch.o
	gcc -lpthread -o  allocator_bnch allocator_bnch.o liballocator.a libring.a -g
	rm allocator_bnch.o
