#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdint.h>
#include <immintrin.h> //clflush

// lock flag for atomicity
atomic_flag lock_flag = ATOMIC_FLAG_INIT;

void acquire_lock(atomic_flag* lock) {
    while (atomic_flag_test_and_set(lock)) { }
}

void release_lock(atomic_flag* lock) {
    atomic_flag_clear(lock);
}

void* worker(void* arg) {
    uintptr_t accessed_address = *(uintptr_t*)arg;

    printf("Accessed address: 0x%lX\n", accessed_address);

    for (int i = 0; i < 100000; i++) {
        acquire_lock(&lock_flag);

        volatile int* target_address = (volatile int*)accessed_address;

        *target_address = *target_address + 1;

        _mm_clflush((void*)target_address);

        //enforce memory ordering
        _mm_mfence();

        release_lock(&lock_flag);
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;

    // memory allocation
    int* safe_memory = (int*)malloc(sizeof(int));
    *safe_memory = 0;

    uintptr_t allocated_address = (uintptr_t)safe_memory;

    printf("Allocated Address: 0x%lX\n", allocated_address);
    printf("Initialized value: %d\n\n", *safe_memory);

    // thread init, excuting
    pthread_create(&t1, NULL, worker, &allocated_address);
    pthread_create(&t2, NULL, worker, &allocated_address);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("\nFinal value(0x%lX): %d\n", allocated_address, *(volatile int*)allocated_address);

    free(safe_memory);
    return 0;
}