#include <pthread.h>
#include <string.h>


int main()
{
    pthread_t a;
    pthread_mutex_t b = PTHREAD_MUTEX_INITIALIZER;
    int j = pthread_mutex_destroy(&b);
    printf("%s\n", strerror(j));
}