/*
 * ProductofconsecutiveFibnumbers
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef unsigned long long ull;
typedef struct dyn_array {
    int size;
    ull *membs;
} darray;


ull fibn(int n, darray *table);

int
main
        (int argc, char *argv[])
{
    darray *table = calloc(1, sizeof(darray));
    for (int i = 0; i < 10; i++){
        int n = rand() % 55;
        printf("fib(%d) : %llu", n, fibn(n, table));
    }

    return EXIT_SUCCESS;
}
void darray_init(darray *da){
    da->size = 2;
    da->membs = calloc(2, sizeof(ull));
    da->membs[1] = 1;
}

void darray_push(darray *da, int idx, ull val){
    if (idx <= da->size)
        return;
    if (da->size <= idx){
        da->size = idx + 1;
        da->membs = realloc(da->membs, sizeof(ull) * da->size);
    }
    da->membs[idx] = val;
}

ull fibn(int n, darray *table){
    if (n < 0)
        return -1;
    if (table->size > n)
        return table->membs[n];
    darray_push(table, n, fibn(n - 2, table) + fibn(n - 1, table));
    return table->membs[n];
}

unsigned long long* productFib(ull prod) {
    static darray table;
    darray_init(&table);
    return 0;
}
