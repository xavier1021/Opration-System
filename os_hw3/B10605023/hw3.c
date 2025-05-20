#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<time.h>

int counter = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* random_points(void* num){
    long long* nptr = (long long*)(num);
    long long n = *nptr;

    srand(time(0));

    for(long long i = 0; i < n; i++){
        double x = ((double)rand() / RAND_MAX) * 2 - 1;
        double y = ((double)rand() / RAND_MAX) * 2 - 1;
        if((x*x + y*y) <= 1){
            pthread_mutex_lock(&mutex);
            counter++;
            pthread_mutex_unlock(&mutex);
        }
    }

    pthread_exit(0);
}

int main(){
    //fix the number of threads and points per thread
    long long threadsNum = 5;
    long long pointsPerThread = 1000;

    pthread_t* tids = (pthread_t*)malloc(threadsNum * sizeof(pthread_t));
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    for(int i = 0; i < threadsNum; i++){
        pthread_create(&tids[i], &attr, random_points, &pointsPerThread);
    }

    for(int i = 0; i < threadsNum; i++){
        pthread_join(tids[i], NULL);
    }

    free(tids);
    double pi = (double)counter / (threadsNum * pointsPerThread) * 4;
    printf("Estimated value of pi: %lf\n", pi);
}
