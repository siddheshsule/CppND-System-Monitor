#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

class System {
 public:
  Processor& Cpu();                   //  See src/system.cpp
  Processor& Cpu0();                  //  See src/system.cpp
  Processor& Cpu1();                  //  See src/system.cpp
  Processor& Cpu2();                  //  See src/system.cpp
  Processor& Cpu3();                  //  See src/system.cpp
  Processor& Cpu4();                  //  See src/system.cpp
  Processor& Cpu5();                  //  See src/system.cpp
  Processor& Cpu6();                  //  See src/system.cpp
  Processor& Cpu7();                  //  See src/system.cpp
  std::vector<Process>& Processes();  //  See src/system.cpp
  float MemoryUtilization();          //  See src/system.cpp
  long UpTime();                      //  See src/system.cpp
  int TotalProcesses();               //  See src/system.cpp
  int RunningProcesses();             //  See src/system.cpp
  std::string Kernel();               //  See src/system.cpp
  std::string OperatingSystem();      //  See src/system.cpp

  //  Define any necessary private members
 private:
  Processor cpu_ = {};
  Processor cpu0_ = {};
  Processor cpu1_ = {};
  Processor cpu2_ = {};
  Processor cpu3_ = {};
  Processor cpu4_ = {};
  Processor cpu5_ = {};
  Processor cpu6_ = {};
  Processor cpu7_ = {};
  std::vector<Process> processes_ = {};
};

#endif