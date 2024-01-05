#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;


// Getters

// Return this process's ID
int Process::Pid() { return pid_;}

// Return this process's CPU utilization
float Process::CpuUtilization() { return cpuUtilization_; }

// Return the command that generated this process
string Process::Command() { return cmd_; }

// Return this process's memory utilization
string Process::Ram() { return ram_; }

// Return the user (name) that generated this process
string Process::User() { return user_; }

// Return the age of this process (in seconds)
long int Process::UpTime() { return upTime_; }

// Overload the "less than" comparison operator for Process objects
bool Process::operator < (Process const& a) const { 
    return this->cpuUtilization_ < a.cpuUtilization_;
}

bool Process::operator > (Process const& a) const { 
    return this->cpuUtilization_ > a.cpuUtilization_;
}

// Setters

void Process::Pid(int pid) {pid_ = pid; }
void Process::UpTime(int pid) {upTime_ = LinuxParser::UpTime(pid);}
void Process::User(int pid){user_ = LinuxParser::User(pid);}                      // See src/process.cpp
void Process::Command(int pid){cmd_ = LinuxParser::Command(pid);}                   // See src/process.cpp
void Process::CpuUtilization(int pid){
    float process = static_cast<float>(LinuxParser::ActiveJiffies(pid));
    float processor = static_cast<float>(LinuxParser::ActiveJiffies());
    cpuUtilization_ = process / processor;
}                  // See src/process.cpp

void Process::Ram(int pid) {
    int kb = std::atoi(LinuxParser::Ram(pid).c_str());
    int mb = kb / 1000;
    ram_ = std::to_string(mb);
}