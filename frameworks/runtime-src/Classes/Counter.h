#ifndef __Boids_Counter_h_
#define __Boids_Counter_h_

#include <map>
#include <string>
#include <sstream>
#include "Utils.h"

class Counter
{
protected:
	std::map<std::string, int> data_i;
	std::map<std::string, float> data_f;

public:
	void operator () (const std::string& title)
	{
		data_i[title] ++;
	}

	void accumulate(const std::string& title, float v)
	{
		data_f[title] += v;
	}

	void print()
	{
		{
			std::ostringstream os;
			for (const auto& title_and_count : data_i)
			{
				os << title_and_count.first << ":" << title_and_count.second << ", ";
			}
			Utils::logAI("%s", os.str().c_str());
		}
		{
			std::ostringstream os;
			for (const auto& title_and_count : data_f)
			{
				os << title_and_count.first << ":" << title_and_count.second << ", ";
			}
			Utils::logAI("%s", os.str().c_str());
		}
	}
};

extern Counter counter;

#endif