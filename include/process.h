#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  int Pid();                               // See src/process.cpp
  std::string User();                      // See src/process.cpp
  std::string Command();                   // See src/process.cpp
  float CpuUtilization();                  // See src/process.cpp
  std::string Ram();                       // See src/process.cpp
  long int UpTime();                       // See src/process.cpp
  bool operator<(Process const& a) const;
  bool operator>(Process const& a) const;  // See src/process.cpp
  void Pid(int);                               // See src/process.cpp
  void User(int);                      // See src/process.cpp
  void Command(int);                   // See src/process.cpp
  void CpuUtilization(int);                  // See src/process.cpp
  void Ram(int);                       // See src/process.cpp
  void UpTime(int);

  // Declare any necessary private members
 private:
    float cpuUtilization_{0.0};
    int pid_{0};
    std::string cmd_{""};
    std::string ram_{""};
    long int upTime_{0}; 
    std::string user_{""};
};

#endif