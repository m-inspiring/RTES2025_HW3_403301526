#ifndef HARDWARE_MONITOR_H
#define HARDWARE_MONITOR_H

#include "ssh_connection.h"
#include <string>

class HardwareMonitor {
public:
    HardwareMonitor(SSHConnection& connection);
    void displayHardwareInfo();
    double getCPUUsage();
    double getMemoryUsage();
    std::pair<int, double> getTopCPUProcess();
    std::pair<int, double> getTopMemoryProcess();

private:
    SSHConnection& connection;
    std::string executeAndGetOutput(const std::string& command);
};

#endif // HARDWARE_MONITOR_H
