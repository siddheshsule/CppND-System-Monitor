#include "processor.h"
#include "linux_parser.h"
#include <vector>
#include <string>
#include <iostream>
#include <chrono>
#include <thread>


// Return the aggregate CPU utilization
float Processor::Utilization() {
	long activeJiffies = LinuxParser::ActiveJiffies();
    long jiffies = LinuxParser::Jiffies();
  
  long activeDiff = activeJiffies - m_jiffies.first;
  long totalDiff = jiffies - m_jiffies.second;
  float usage = static_cast<float> (activeDiff) / totalDiff;
  m_jiffies.first = activeJiffies;
  m_jiffies.second = jiffies;
  return usage;

}