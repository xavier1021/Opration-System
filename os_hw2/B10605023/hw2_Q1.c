#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define NUM_THREADS 4
#define TOTAL_POINTS 10000000

int points_in_circle = 0;
pthread_mutex_t lock;

void* monte_carlo_pi(void* arg) {
    int points_per_thread = TOTAL_POINTS / NUM_THREADS;
    int local_count = 0;
    unsigned int seed = time(NULL) ^ pthread_self();

    for (int i = 0; i < points_per_thread; ++i) {
        double x = (double)rand_r(&seed) / RAND_MAX * 2.0 - 1.0;
        double y = (double)rand_r(&seed) / RAND_MAX * 2.0 - 1.0;
        if (x * x + y * y <= 1.0)
            local_count++;
    }

    pthread_mutex_lock(&lock);
    points_in_circle += local_count;
    pthread_mutex_unlock(&lock);
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    pthread_mutex_init(&lock, NULL);

    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_create(&threads[i], NULL, monte_carlo_pi, NULL);
    }

    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

    double pi_estimate = 4.0 * points_in_circle / TOTAL_POINTS;
    printf("Estimated π = %f\n", pi_estimate);

    pthread_mutex_destroy(&lock);
    return 0;
}
