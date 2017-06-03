#define _GNU_SOURCE
 
#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <time.h>
 
static int (*real_fflush)(FILE*) = NULL;

void load_real_fflush() {
    real_fflush = dlsym(RTLD_NEXT, "fflush");
    if (NULL == real_fflush) {
        fprintf(stderr, "Error in 'dlsym': %s\n", dlerror());
        exit(1);
    }
}
 
int fflush(FILE* stream) {
    if (NULL == real_fflush) {
        load_real_fflush();
    }
    if (stream == stderr) {
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
        fprintf(stream, "\n%ld.%06ld ", ts.tv_sec, ts.tv_nsec);
    }
    return real_fflush(stream);
}
