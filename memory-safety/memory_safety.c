#include <stdio.h>
#include <stdlib.h>

// Accessing freed pointers
void access_freed_pointer() {
    int *p = malloc(sizeof(int));
    *p = 5;
    free(p);

    printf("De-referenced memory no longer belongs to pointer p\n");
    printf("%d\n", *p);
}

// Accessing un-initialized pointers
void access_uninit_pointer() {
    int *p;
    *p = 5;
}

void temporal_safety() {
    access_freed_pointer();
    access_uninit_pointer();
}

int spatial_safety() {
    return 0;
}