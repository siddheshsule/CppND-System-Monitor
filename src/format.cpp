#include <string>
#include <chrono>
#include <unistd.h>
#include <iomanip>
#include <sstream>

#include "format.h"

using std::string;
using namespace std::chrono;

string Format::ElapsedTime(long seconds) { 
    std::ostringstream resultStream;    
    // compute h, m, s
    const int hours = seconds / 3600;
    const int minutes = (seconds % 3600) / 60;
    const int secs = seconds % 60;

    // Fromatting the resultStream in HH:MM:SS format
    resultStream << std::setw(2) << std::setfill('0') << hours << ":"
                    << std::setw(2) << std::setfill('0') << minutes << ":"
                    << std::setw(2) << std::setfill('0') << secs;

    return resultStream.str();
}