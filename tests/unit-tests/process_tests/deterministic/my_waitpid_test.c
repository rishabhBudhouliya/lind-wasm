#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

int main() {
    printf("\n=== TEST: waitpid on non-child pid ===\n");

    // Step 1: Fork a child
    pid_t child_pid = fork();

    if (child_pid == 0) {
        // Child process - just exit immediately
        printf("[CHILD] I am child, exiting with code 42\n");
        _exit(42);
    }

    // Parent process continues here
    printf("[PARENT] Created child with pid=%d\n", child_pid);

    // Step 2: Create a fake PID (non-existent child)
    // Note: In lind-wasm, MAX_CAGEID is 1024, so we must use a value < 1024
    pid_t fake_pid = child_pid + 100;  // Use small offset instead of 10000
    printf("[PARENT] Will try to wait for fake pid=%d (not my child)\n", fake_pid);

    // Step 3: Try to waitpid on the fake PID
    int status = 0;
    pid_t result = waitpid(fake_pid, &status, WNOHANG);

    printf("[PARENT] waitpid(%d) returned=%d, errno=%d (%s)\n",
           fake_pid, result, errno, strerror(errno));

    // Step 4: Now wait for our actual child
    printf("[PARENT] Now waiting for real child pid=%d\n", child_pid);
    result = waitpid(child_pid, &status, 0);
    printf("[PARENT] waitpid(%d) returned=%d, status=%d\n",
           child_pid, result, status);

    printf("=== TEST COMPLETE ===\n");
    return 0;
}
