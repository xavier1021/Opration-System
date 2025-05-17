#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// 產生 Collatz 數列
void printCollatz(int n) {
    printf("Collatz sequence: ");
    while (n != 1) {
        printf("%d,", n);
        if (n % 2 == 0)
            n = n / 2;
        else
            n = 3 * n + 1;
    }
    printf("%d\n", n); // 印出最後的 1
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <positive integer>\n", argv[0]);
        exit(1);
    }

    int n = atoi(argv[1]);

    if (n <= 0) {
        fprintf(stderr, "Error: Please enter a positive integer.\n");
        exit(1);
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(1);
    } else if (pid == 0) {
        // 子程序：輸出 Collatz 數列
        printCollatz(n);
        exit(0);
    } else {
        // 母程序：等待子程序結束
        wait(NULL);
    }

    return 0;
}
