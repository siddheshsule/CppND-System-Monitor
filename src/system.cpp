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

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { return processes_; }

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
