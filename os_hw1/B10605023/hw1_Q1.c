#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
// 計算 Fibonacci 序列
void printFibonacci(int n) {
    int a = 0, b = 1, c;
    if (n <= 0) return;

    for (int i = 0; i < n; i++) {
        if (i == 0)
            printf("%d", a);
        else if (i == 1)
            printf(",%d", b);
        else {
            c = a + b;
            printf(",%d", c);
            a = b;
            b = c;
        }
    }
    printf("\n");
}
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <non-negative integer>\n", argv[0]);
        exit(1);
    }
    int n = atoi(argv[1]);
    if (n < 0) {
        fprintf(stderr, "Error: Number must be non-negative.\n");
        exit(1);
    }
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(1);
    } else if (pid == 0) {
        // Child process
        printFibonacci(n);
        exit(0);
    } else {
        // Parent process
        wait(NULL);  // 等待子程序完成
    }
    return 0;
}
