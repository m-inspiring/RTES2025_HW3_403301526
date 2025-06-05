#include <iostream>
#include <csignal>
#include <cstring>
#include <fstream>
#include <unistd.h>

// Open log file globally in append mode
std::ofstream log_file("signal_log.txt", std::ios::app);

// Signal handler function
void signal_handler(int sig) {
    std::string signal_name = ::strsignal(sig); // Get signal name
    std::string message = "Received signal: " + std::to_string(sig) + " (" + signal_name + ")\n";
    std::cout << message; // Print to console
    log_file << message;  // Log to file
    log_file.flush();     // Ensure log is written immediately
}

int main() {
    // Display the PID of the receiver
    std::cout << "Receiver PID: " << getpid() << std::endl;

    // Register signal handler for all catchable signals
    for (int i = 1; i < NSIG; i++) {
        if (i != SIGKILL && i != SIGSTOP) { // Skip uncatchable signals
            signal(i, signal_handler);
        }
    }

    // Keep the program running indefinitely
    while (true) {
        pause(); // Wait for signals
    }

    // Unreachable, but included for completeness
    log_file.close();
    return 0;
}