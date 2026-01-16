#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <iomanip>

int main() {
    // --- Parent Process Logic ---

    // Get the current process ID
    pid_t currentPid = getpid();

    std::cout << "--------+-----------------\n";
    std::cout << "Process | Process ID       \n";
    std::cout << "--------+-----------------\n";

    // Fork 3 child processes
    for(int i = 0; i < 3; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            // --- Error Handling ---
            std::cerr << "Failed to start process (Fork failed)." << std::endl;
            return 1;
        }
        else if (pid == 0) {
            // --- Child Process Logic ---
            std::cout << std::setw(7) << (i+1) << " | " 
                      << std::setw(15) << getpid() << "\n";
            exit(0);
        }
    }

    // --- Waiting Logic (Parent) ---

    // Wait for all 3 child processes to finish
    for(int i = 0; i < 3; i++) {
        wait(NULL);
    }

    std::cout << "--------+-----------------\n";

    return 0;
}