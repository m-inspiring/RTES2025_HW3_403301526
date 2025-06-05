#include "ssh_connection.h"
#include "hardware_monitor.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <iomanip>
#include <ctime>

int main(int argc, char* argv[]) {
    if (argc != 5) {
        std::cerr << "Usage: " << argv[0] << " <user@host> <password> <cpu_threshold> <mem_threshold>" << std::endl;
        return 1;
    }

    std::string host = argv[1];
    std::string password = argv[2];
    double cpuThreshold = std::stod(argv[3]);
    double memThreshold = std::stod(argv[4]);
    std::string user = host.substr(0, host.find('@'));

    std::cout << "Connecting to remote host..." << std::endl;
    SSHConnection connection(host, user, password);
    if (!connection.connect()) {
        std::cerr << "Failed to connect to remote host." << std::endl;
        return 1;
    }
    std::cout << "Connection established." << std::endl;

    HardwareMonitor monitor(connection);
    monitor.displayHardwareInfo();

    while (true) {
        auto now = std::chrono::system_clock::now();
        auto now_c = std::chrono::system_clock::to_time_t(now);
        std::cout << "\n[" << std::put_time(std::localtime(&now_c), "%H:%M:%S %d-%m-%Y") << "]" << std::endl;

        double cpuUsage = monitor.getCPUUsage();
        double memUsage = monitor.getMemoryUsage();
        std::cout << "CPU Usage: " << std::fixed << std::setprecision(1) << cpuUsage << "%" << std::endl;
        std::cout << "Memory Usage: " << std::fixed << std::setprecision(1) << memUsage << "%" << std::endl;

        if (cpuUsage > cpuThreshold) {
            std::cout << "Warning: CPU usage exceeds threshold!" << std::endl;
            auto [pid, usage] = monitor.getTopCPUProcess();
            std::cout << "Killing process with PID: " << pid << " due to high CPU usage (" << usage << "%)." << std::endl;
            connection.executeCommand("kill " + std::to_string(pid));
        }

        if (memUsage > memThreshold) {
            std::cout << "Warning: Memory usage exceeds threshold!" << std::endl;
            auto [pid, usage] = monitor.getTopMemoryProcess();
            std::cout << "Killing process with PID: " << pid << " due to high memory usage (" << usage << "%)." << std::endl;
            connection.executeCommand("kill " + std::to_string(pid));
        }

        std::this_thread::sleep_for(std::chrono::seconds(30));
    }

    return 0;
}
