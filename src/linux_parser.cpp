#ifndef LINUX_PARSER_H
#define LINUX_PARSER_H

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unistd.h> 
#include <iostream>
#include <dirent.h>

#include "linux_parser.h"
#include "format.h"

using std::string;
using std::vector;

// An example of how to read data from the filesystem
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

// An example of how to read data from the filesystem
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


// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  
  long memTotal = 0;
  long memFree = 0;
  long memBuffers = 0;
  long memCached = 0;
  string token;
  
  std::ifstream file (kProcDirectory + kMeminfoFilename);
  
  while(file >> token) {
  	string temp;
    if (token == "MemTotal:"){
   		file >> temp;
      	memTotal = std::stol(temp);
    } else if (token == "MemFree:"){
   		file >> temp;
      	memFree = std::stol(temp);
    } else if (token == "Buffers:"){
   		file >> temp;
      	memBuffers = std::stol(temp);
    } else if (token == "Cached:"){
   		file >> temp;
      	memCached = std::stol(temp);
    }
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }
  long memUse = memFree + memBuffers + memCached;
  float utilization = (memTotal - memUse) / static_cast<float> (memTotal);
  return utilization;
}

// Read and return the system uptime
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

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  long totalJiffies = 0;
  auto AllJiffies = LinuxParser::CpuUtilization();
  for(auto Jiffy: AllJiffies) {
    totalJiffies += std::stol(Jiffy);
  }
  return totalJiffies;  
}

// Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) { 
  std::string line;
  std::ifstream fileStream(kProcDirectory + std::to_string(pid) + kStatFilename);

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

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  vector<string> cpuUtilization = LinuxParser::CpuUtilization();
  long activeJiffies = LinuxParser::Jiffies() - LinuxParser::IdleJiffies();
  return activeJiffies;
 }

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  vector<string> cpuUtilization = LinuxParser::CpuUtilization("cpu");
  long idletime = std::stol(cpuUtilization[3]);
  long iowaittime = std::stol(cpuUtilization[4]);
  long idleJiffies = idletime + iowaittime;
  return idleJiffies;
 }


// Read and return CPU utilization as per ptovided key
vector<string> LinuxParser::CpuUtilization() { 
  std::ifstream file("/proc/stat");
    std::vector<string> cpuStats;
    std::string line;

    if (file.is_open()) {
        std::getline(file, line); // Read the first line

        std::istringstream linestream(line);
        std::string cpuLabel;
        linestream >> cpuLabel; // Read the 'cpu' label        
        string value;
        while (linestream >> value) {
            cpuStats.push_back(value);
        }
      
        file.close();
    }

    return cpuStats;
}

// Read and return CPU utilization as per ptovided key
vector<string> LinuxParser::CpuUtilization(std::string cpuLabel_ ) { 
  std::ifstream file("/proc/stat");
    std::vector<string> cpuStats;
    std::string line;

    if (file.is_open()) {
        while (std::getline(file, line)) { // Read the first line
          std::istringstream linestream(line);
          std::string cpuLabel;
          linestream >> cpuLabel; // Read the 'cpu' label
          if (cpuLabel == cpuLabel_) {
            string value;
            while (linestream >> value) {
                cpuStats.push_back(value);
            }
            break;
          }
        }
        file.close();
    }
    return cpuStats;
}


// Read and return the total number of processes
int LinuxParser::TotalProcesses() {  
  int totalProcesses = 0;
  std::string path = kProcDirectory + kStatFilename;
  totalProcesses = std::stoi(LinuxParser::parseWithKey("processes", path));
  return totalProcesses; 
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  int runningProcesses = 0;
  std::string path = kProcDirectory + kStatFilename;
  runningProcesses = std::stoi(LinuxParser::parseWithKey("procs_running", path));
  return runningProcesses; 
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  string command;  
  std::ifstream fileStream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (fileStream.is_open()) {
    std::getline(fileStream, command);    
  }
  return command; 
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) {  
  std::string path = kProcDirectory + std::to_string(pid) + kStatusFilename;
  string result = LinuxParser::parseWithKey("VmSize:", path);
  return result;     
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  std::string path = kProcDirectory + std::to_string(pid) + kStatusFilename;
  string result = LinuxParser::parseWithKey("Uid:", path);
  return result;
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) { 
  string line, probableUser, probableUid, token, user;
  bool continueSearch = true;
  std::string uid = LinuxParser::Uid(pid);
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (continueSearch && filestream.peek() != EOF) {
        std::getline(filestream, line);
        std::replace(line.begin(), line.end(), ':', ' ');
        std::istringstream lineStream(line);
        lineStream  >> probableUser >> token >> probableUid;
        if (uid == probableUid) {
          user = probableUser;
          continueSearch = false;
        }
      }
      filestream.close();
    }  
  return user;
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 
  long ticks = 0;
  string line;
  string skip;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line); 
    for(int i = 1; i < 22; ++i) {
      linestream >> skip;
    }
    linestream >> ticks;
  }
  return ticks / sysconf(_SC_CLK_TCK);
}


// A function to parse files according to the specified key
std::string LinuxParser::parseWithKey(std::string key, std::string path) {
  std::ifstream filestream(path);
    std::string line, fileKey, value;    
    if (!filestream.is_open())
        return "n/a";
    while (std::getline(filestream, line)) {
        std::istringstream linestream(line);
        linestream >> fileKey;        
        if (fileKey == key) {
            linestream >> value;
            return value;
        }
    }    
    return "n/a";
}


#endif // LINUX_PARSER_H