#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>

class Processor {
	public:
  		float Utilization();  // See src/processor.cpp
	private:
  		std::pair<long, long> m_jiffies;
};

#endif