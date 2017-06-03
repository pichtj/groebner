#define _GNU_SOURCE
 
#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <time.h>

void log_memory_status() {
    FILE *f = fopen("/proc/self/statm", "r");
    if (!f) return;
    unsigned long size,resident,share,text,lib,data,dt;
    if (7 != fscanf(f,"%ld %ld %ld %ld %ld %ld %ld", &size, &resident, &share, &text, &lib, &data, &dt)) return;
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    fprintf(stderr, "\n%ld.%06ld %ld %ld %ld %ld %ld %ld %ld", ts.tv_sec, ts.tv_nsec, size, resident, share, text, lib, data, dt);
    fclose(f);
}

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
        log_memory_status();
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
        fprintf(stream, "\n%ld.%06ld ", ts.tv_sec, ts.tv_nsec);
    }
    return real_fflush(stream);
}
