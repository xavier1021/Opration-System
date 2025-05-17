#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>

#define SHM_NAME "/collatz_shm"
#define MAX_SIZE 1024

void generateCollatz(int n, char *buffer) {
    char temp[32];
    while (n != 1) {
        sprintf(temp, "%d,", n);
        strcat(buffer, temp);
        if (n % 2 == 0)
            n /= 2;
        else
            n = 3 * n + 1;
    }
    sprintf(temp, "%d", n);  // 加入最後的 1
    strcat(buffer, temp);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <positive integer>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    if (n <= 0) {
        fprintf(stderr, "Error: Please provide a positive integer.\n");
        return 1;
    }

    // a. 建立 shared memory
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open failed");
        return 1;
    }

    // 設定 shared memory 大小
    if (ftruncate(shm_fd, MAX_SIZE) == -1) {
        perror("ftruncate failed");
        return 1;
    }

    // 映射 shared memory 到虛擬記憶體空間
    char *shm_ptr = mmap(NULL, MAX_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm_ptr == MAP_FAILED) {
        perror("mmap failed");
        return 1;
    }

    // b. 建立子程序
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        return 1;
    } else if (pid == 0) {
        // 子程序：產生 Collatz 數列寫入 shared memory
        shm_ptr[0] = '\0'; // 清空記憶體
        generateCollatz(n, shm_ptr);
        exit(0);
    } else {
        // 母程序：等待子程序完成
        wait(NULL);

        // c. 讀取 shared memory
        printf("Collatz sequence: %s\n", shm_ptr);

        // d. 移除 shared memory
        munmap(shm_ptr, MAX_SIZE);
        shm_unlink(SHM_NAME);
    }

    return 0;
}
