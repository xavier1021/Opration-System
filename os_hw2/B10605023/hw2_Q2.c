#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int *fib_sequence;
int num_terms;

void *generate_fibonacci(void *arg) {
    if (num_terms > 0) fib_sequence[0] = 0;
    if (num_terms > 1) fib_sequence[1] = 1;

    for (int i = 2; i < num_terms; ++i) {
        fib_sequence[i] = fib_sequence[i - 1] + fib_sequence[i - 2];
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <number of Fibonacci terms>\n", argv[0]);
        return 1;
    }

    num_terms = atoi(argv[1]);
    if (num_terms <= 0) {
        printf("Please enter a positive integer.\n");
        return 1;
    }

    fib_sequence = malloc(num_terms * sizeof(int));
    if (!fib_sequence) {
        perror("Failed to allocate memory");
        return 1;
    }

    pthread_t thread;

    // Create the thread to generate Fibonacci numbers
    pthread_create(&thread, NULL, generate_fibonacci, NULL);

    // Wait for the child thread to finish
    pthread_join(thread, NULL);

    // Output the generated sequence
    printf("Fibonacci sequence:\n");
    for (int i = 0; i < num_terms; ++i) {
        if (i != num_terms - 1)
            printf("%d, ", fib_sequence[i]);
        else
            printf("%d\n", fib_sequence[i]);
    }


    free(fib_sequence);
    return 0;
}
