#include <stdio.h>
#include <stdlib.h>

void type_safety() {
    printf("Type Safety Violation\n");

    int (*cmp)(char*, char*);
    int *p = (int*)malloc(sizeof(int));
    *p = 1;
    cmp = (int(*)(char*, char*))p;
    cmp("hello", "bye");
}