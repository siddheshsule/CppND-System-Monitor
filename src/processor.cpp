#include "processor.h"
#include "linux_parser.h"
#include <vector>
#include <string>
#include <iostream>


// DONE: Return the aggregate CPU utilization
float Processor::Utilization() { 
    std::vector<std::string> cpuUtilizationVector = LinuxParser::CpuUtilization();
    float Idle, NonIdle, Total;
    Idle = std::stof(cpuUtilizationVector.at(3)) + std::stof(cpuUtilizationVector.at(4));
    NonIdle = std::stof(cpuUtilizationVector.at(0)) // to grab value of user
            + std::stof(cpuUtilizationVector.at(1)) // to grab value of nice
            + std::stof(cpuUtilizationVector.at(2)) // to grab value of system
            + std::stof(cpuUtilizationVector.at(5)) // to grab value of irq
            + std::stof(cpuUtilizationVector.at(6)) // to grab value of softirq
            + std::stof(cpuUtilizationVector.at(7)); // to grab value of steal

    Total = Idle + NonIdle;
    return (Total - Idle)/Total;
 }