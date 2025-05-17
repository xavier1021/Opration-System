#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 10

int input_array[SIZE] = {7, 12, 19, 3, 18, 4, 2, 6, 15, 8};
int sorted_array[SIZE];

typedef struct {
    int start;
    int end;
} ThreadData;

void *sort_sublist(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    int start = data->start;
    int end = data->end;

    // Bubble sort (or any sorting algorithm)
    for (int i = start; i < end - 1; ++i) {
        for (int j = start; j < end - (i - start) - 1; ++j) {
            if (input_array[j] > input_array[j + 1]) {
                int temp = input_array[j];
                input_array[j] = input_array[j + 1];
                input_array[j + 1] = temp;
            }
        }
    }
    pthread_exit(NULL);
}

void *merge_sublist(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    int mid = data->start;
    int end = data->end;

    int i = 0, j = mid, k = 0;
    while (i < mid && j < end) {
        if (input_array[i] < input_array[j])
            sorted_array[k++] = input_array[i++];
        else
            sorted_array[k++] = input_array[j++];
    }
    while (i < mid) sorted_array[k++] = input_array[i++];
    while (j < end) sorted_array[k++] = input_array[j++];

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[3];
    ThreadData data[3];

    int mid = SIZE / 2;

    // First half sorting
    data[0].start = 0;
    data[0].end = mid;
    pthread_create(&threads[0], NULL, sort_sublist, (void *)&data[0]);

    // Second half sorting
    data[1].start = mid;
    data[1].end = SIZE;
    pthread_create(&threads[1], NULL, sort_sublist, (void *)&data[1]);

    // Wait for sorting threads
    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);

    // Merge the two sorted sublists
    data[2].start = mid;
    data[2].end = SIZE;
    pthread_create(&threads[2], NULL, merge_sublist, (void *)&data[2]);
    pthread_join(threads[2], NULL);

    // Output the final sorted array
    printf("Sorted array:\n");
    for (int i = 0; i < SIZE; ++i) {
        if (i != SIZE - 1)
            printf("%d, ", sorted_array[i]);
        else
            printf("%d\n", sorted_array[i]);
    }

    return 0;
}
