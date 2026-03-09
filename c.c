#include <stdio.h>
#include <pthread.h>

int counter = 0; // shared between threads

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *increment(void *arg) {
    int local = 0;
    for (int i = 0; i < 1000000; i++) {
        local += 1;
    }
    pthread_mutex_lock(&lock);
    counter += local;
    pthread_mutex_unlock(&lock);
    return NULL;
}

int main() {
    pthread_t t1, t2;
    pthread_create(&t1, NULL, increment, NULL);
    pthread_create(&t2, NULL, increment, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    printf("Counter: %d\n", counter);
    return 0;
}