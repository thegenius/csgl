#include <stdlib.h>


int main() {
    void *ptr = malloc(1200);
    ptr = realloc(ptr, 2000);
    return 0;
}
