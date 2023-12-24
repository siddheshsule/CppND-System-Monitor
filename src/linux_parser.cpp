#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <pwd.h>

#include "linux_parser.h"
#include "format.h"

#define LINE_NUMBER_OF_TOTAL_PROCESSES 13
#define LINE_NUMBER_OF_RUNNING_PROCESSES 14


using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream fileStream(kProcDirectory + kVersionFilename);
  if (fileStream.is_open()) {
    std::getline(fileStream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string line;
  string key;
  string value;
  float memTotal, memFree, memAvailable, buffers;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {        
        if (key == "MemTotal:") {
          memTotal = std::stof(value);          
        } else if (key == "MemFree:") {
          memFree = std::stof(value);
        } else if (key == "MemAvailable:") {
          memAvailable = std::stof(value);
        } else {
          buffers = std::stof(value);
        }
      }
    }
  }
  return (memTotal- memFree) - buffers;
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() {   
  long upTimeInSeconds, idleTimeInSeconds;
  string line;  
  std::ifstream fileStream(kProcDirectory + kUptimeFilename);
  if (fileStream.is_open()) {
    std::getline(fileStream, line);    
    std::istringstream linestream(line);
    linestream >> upTimeInSeconds >> idleTimeInSeconds;
  }
  return upTimeInSeconds; 
}

// DONE: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  long totalJiffies = 0;
  string line;  
  std::ifstream fileStream(kProcDirectory + kStatFilename);
  if (fileStream.is_open()) {
    while (std::getline(fileStream, line)) {
      if (line.compare(0,4,"cpu ") == 0) {
        std::istringstream lineStream(line);
        std::string cpuLabel;
        long jiffies;
        while (lineStream >> jiffies) {
          totalJiffies += jiffies;
        }
        break;
      }
    }
    fileStream.close();    
  }
  return totalJiffies;  
}

// DONE: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) { 

  std::string line;
  std::ifstream fileStream(kProcDirectory + std::to_string(pid) + kStatFilename);

  long utime = 0;
  long stime = 0;
  long cutime = 0;
  long cstime = 0;

  if (fileStream.is_open()) {
    std::getline(fileStream, line);
    std::istringstream lineStream(line);
    std::vector<std::string> tokens;

    while (lineStream >> line) {
      tokens.push_back(line);
    }

    long utime = std::stol(tokens[13]);
    long stime = std::stol(tokens[14]);
    long cutime = std::stol(tokens[15]);
    long cstime = std::stol(tokens[16]);    

    fileStream.close();

    long activeJiffies = utime + stime + cutime + cstime;
    return activeJiffies;  
  }    
  return 0; 
}

// DONE: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  vector<string> cpuUtilization = LinuxParser::CpuUtilization();

  long utime = std::stol(cpuUtilization[0]);
  long nicetime = std::stol(cpuUtilization[1]);
  long systemtime = std::stol(cpuUtilization[2]);
  long idletime = std::stol(cpuUtilization[3]);

  long activeJiffies = utime + nicetime + systemtime + idletime;
  return activeJiffies;
 }

// DONE: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  vector<string> cpuUtilization = LinuxParser::CpuUtilization();
  long idleJiffies = std::stol(cpuUtilization[3]);
  return idleJiffies;
 }

// DONE: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  vector<string> cpuUtilization;
  string next;
  string line, cpuName;
  std::ifstream fileStream(kProcDirectory + kStatFilename);
  if (fileStream.is_open()) {
    std::getline(fileStream, line);    
    std::istringstream linestream(line);    
    while (linestream) {
      linestream >> next;
      cpuUtilization.push_back(next);
    }
  }  
  cpuUtilization.erase(std::begin(cpuUtilization));
  return cpuUtilization; 
}


// LineParser function ... To avoid code duplication! :-)
int LinuxParser::LineParser(int lineNumberStart, int lineNumberEnd, string pathDirectory, string pathFilename) {
  string processes;
  int processCount;
  string line;
  int count = 0;
  std::ifstream fileStream(pathDirectory + pathFilename);
  if (fileStream.is_open()) {
    while (count < lineNumberEnd - lineNumberStart) 
    {
      std::getline(fileStream, line);
      count++;
    }    
    std::istringstream linestream(line);
    linestream >> processes >> processCount;
  }
  return processCount;
}


// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() { return LinuxParser::LineParser(0, LINE_NUMBER_OF_TOTAL_PROCESSES, kProcDirectory, kStatFilename); }

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() { return LinuxParser::LineParser(0, LINE_NUMBER_OF_RUNNING_PROCESSES, kProcDirectory, kStatFilename); }

// DONE: Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  string command;  
  std::ifstream fileStream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (fileStream.is_open()) {
    std::getline(fileStream, command);    
  }
  return command; 
}

// DONE: Read and return the memory used by a process
string LinuxParser::Ram(int pid) {  
  string line, key, value, ram;
  int count = 0;
  std::ifstream fileStream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (fileStream.is_open()) {
    while (std::getline(fileStream, line)) {      
      std::istringstream linestream(line);
      while (linestream >> key >> value) {        
        if (key == "VmSize:") {
          ram = value;
          break;          
        }      
      }
    }
    fileStream.close();
  }
  return ram;  
}

// DONE: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  string line, key, value, uid;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {        
        if (key == "Uid:") {
          uid = value;          
        } 
      }
    }
  }
  return uid;
}

// DONE: Read and return the user associated with a process
string LinuxParser::User(int pid) { 
  string line, key, value, uid, userName;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      if (line.substr(0,5) == "Uid: ") {
        std::istringstream stream(line.substr(5));
        int uid;
        stream >> uid;
        struct passwd *pw = getpwuid(uid);
        if (pw != nullptr) {
          userName = pw->pw_name;
        }
        break;
      }
    }
    filestream.close();
  }
  return userName;
}

// TODO: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 
  std::vector<std::string> tokens = LinuxParser::parseWithPid(pid , kProcDirectory, kStatFilename);
  return std::stol(tokens[21]) / sysconf(_SC_CLK_TCK);
}


vector<std::string> LinuxParser::parseWithPid(int pid, std::string directory, std::string fileLocation) {
  vector<std::string> result;
  string line, next;  

  std::ifstream fileStream(directory + std::to_string(pid) + fileLocation);
  if (fileStream.is_open()) {
    std::getline(fileStream, line);    
    std::istringstream linestream(line);
    while(true) {
      linestream >> next;
      result.push_back(next);
    }
  }
  return result; 


}
