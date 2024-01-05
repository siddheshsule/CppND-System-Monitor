#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;
/*You need to complete the mentioned TODOs in order to satisfy the rubric criteria "The student will be able to extract and display basic data about the system."

You need to properly format the uptime. Refer to the comments mentioned in format. cpp for formatting the uptime.*/

// Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// Return the system's CPU
Processor& System::Cpu0() { return cpu0_; }

// Return the system's CPU
Processor& System::Cpu1() { return cpu1_; }

// Return the system's CPU
Processor& System::Cpu2() { return cpu2_; }

// Return the system's CPU
Processor& System::Cpu3() { return cpu3_; }

// Return the system's CPU
Processor& System::Cpu4() { return cpu4_; }

// Return the system's CPU
Processor& System::Cpu5() { return cpu5_; }

// Return the system's CPU
Processor& System::Cpu6() { return cpu6_; }

// Return the system's CPU
Processor& System::Cpu7() { return cpu7_; }
// Return a container composed of the system's processes
vector<Process>& System::Processes() { 
    auto pids = LinuxParser::Pids();
    processes_.clear();
    for (auto pid: pids) {
        Process process;
        process.Pid(pid);
        process.User(pid);
        process.Command(pid);
        process.CpuUtilization(pid);
        process.Ram(pid);
        process.UpTime(pid);
        processes_.push_back(process);
    }

    std::sort(processes_.begin(), processes_.end(), std::greater<Process>());
    return processes_;
}

// Done
std::string System::Kernel() { return LinuxParser::Kernel(); }

// Done
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// Done
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// Done
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// Done
int System::TotalProcesses() { return LinuxParser::TotalProcesses() ; }

// Done
long int System::UpTime() { return LinuxParser::UpTime(); }
