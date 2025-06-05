#include <iostream>
#include <csignal>
#include <cstdlib>
#include <limits>

int main() {
    int pid, sig;

    // Prompt for PID and validate input
    std::cout << "Enter the PID of the receiver: ";
    while (!(std::cin >> pid) || pid <= 0) {
        std::cout << "Invalid PID. Please enter a positive integer: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    // Prompt for signal number and validate input
    std::cout << "Enter the signal number to send: ";
    while (!(std::cin >> sig) || sig <= 0) {
        std::cout << "Invalid signal number. Please enter a positive integer: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    // Send the signal and check for errors
    if (kill(pid, sig) == 0) {
        std::cout << "Sent signal " << sig << " to PID " << pid << std::endl;
    } else {
        std::cerr << "Failed to send signal. Check PID and permissions.\n";
    }

    return 0;
}