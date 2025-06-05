#include "hardware_monitor.h"
#include <iostream>
#include <sstream>
#include <vector>

HardwareMonitor::HardwareMonitor(SSHConnection& connection) : connection(connection) {}

std::string HardwareMonitor::executeAndGetOutput(const std::string& command) {
    return connection.executeCommand(command);
}

void HardwareMonitor::displayHardwareInfo() {
    // CPU Information
    std::cout << "System CPU Information:" << std::endl;
    std::string cpuInfo = executeAndGetOutput("lscpu");
    std::istringstream cpuStream(cpuInfo);
    std::string line;
    while (std::getline(cpuStream, line)) {
        if (line.find("Architecture") != std::string::npos ||
            line.find("CPU(s):") != std::string::npos ||
            line.find("Model name") != std::string::npos) {
            std::cout << line << std::endl;
        }
    }

    // Memory Information
    std::cout << "\nSystem Memory Information:" << std::endl;
    std::string memInfo = executeAndGetOutput("free -h | grep Mem");
    std::istringstream memStream(memInfo);
    std::getline(memStream, line);
    std::cout << line << std::endl;

    // SSH Connection Information
    std::cout << "\nSSH Connection Information (eth0):" << std::endl;
    std::string ipInfo = executeAndGetOutput("ip addr show eth0 | grep inet");
    std::istringstream ipStream(ipInfo);
    while (std::getline(ipStream, line)) {
        if (line.find("inet") != std::string::npos) {
            std::cout << line << std::endl;
        }
    }
}

double HardwareMonitor::getCPUUsage() {
    std::string topOutput = executeAndGetOutput("top -bn1 | grep 'Cpu(s)'");
    size_t usPos = topOutput.find("us,");
    if (usPos != std::string::npos) {
        size_t start = topOutput.rfind(' ', usPos - 2);
        std::string usageStr = topOutput.substr(start + 1, usPos - start - 2);
        try {
            return std::stod(usageStr);
        } catch (...) {
            return 0.0;
        }
    }
    return 0.0;
}

double HardwareMonitor::getMemoryUsage() {
    std::string memOutput = executeAndGetOutput("free -m | grep Mem");
    std::istringstream iss(memOutput);
    std::string token;
    std::vector<std::string> tokens;
    while (iss >> token) tokens.push_back(token);
    if (tokens.size() >= 7) {
        double total = std::stod(tokens[1]);
        double used = std::stod(tokens[2]);
        return (used / total) * 100.0;
    }
    return 0.0;
}

std::pair<int, double> HardwareMonitor::getTopCPUProcess() {
    std::string psOutput = executeAndGetOutput("ps -eo pid,pcpu --sort=-pcpu | head -n 2");
    std::istringstream iss(psOutput);
    std::string line;
    std::getline(iss, line); // Skip header
    std::getline(iss, line);
    std::istringstream lineStream(line);
    int pid;
    double pcpu;
    lineStream >> pid >> pcpu;
    return {pid, pcpu};
}

std::pair<int, double> HardwareMonitor::getTopMemoryProcess() {
    std::string psOutput = executeAndGetOutput("ps -eo pid,pmem --sort=-pmem | head -n 2");
    std::istringstream iss(psOutput);
    std::string line;
    std::getline(iss, line); // Skip header
    std::getline(iss, line);
    std::istringstream lineStream(line);
    int pid;
    double pmem;
    lineStream >> pid >> pmem;
    return {pid, pmem};
}
