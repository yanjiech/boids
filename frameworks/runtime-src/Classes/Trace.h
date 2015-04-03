//
//  Trace.h
//  Lalune
//
//  Created by WeiYuermin on 8/20/14.
//
//

#ifndef __TRACE_H__
#define __TRACE_H__

#include "cocos2d.h"
#include <string>
#include <cstdint>
#if ( CC_TARGET_PLATFORM != CC_PLATFORM_WIN32 )
#include <sys/time.h>
#else
#include "CCStdC-win32.h"
#endif
#include <map>

//#define __TRACE__
//#define __PRINT_SLOW_ACCUMULATE__

class Trace
{
private:
	uint64_t _time, _accumulate_time;

	std::map<std::string, long> accumulated_time;

	uint64_t gettime()
	{
		struct timeval _time;
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
		cocos2d::gettimeofday(&_time, nullptr);
#else
        gettimeofday(&_time, nullptr);
#endif
		return (uint64_t)_time.tv_sec * 1000000 + _time.tv_usec;
	}

public:
	Trace()
	{
#ifdef __TRACE__
		beginTrace(); 
		beginAccumulate();
#endif
	}

	void beginTrace()
	{
#ifdef __TRACE__
		_time = gettime();;
#endif
	}

	void trace(std::string title)
	{
#ifdef __TRACE__
		uint64_t time_new = gettime();
		long t = time_new - _time; //ºŸ…Ë≤ªª·≥¨π˝long
		_time = time_new;
		printf("[%s]: %ldus", title.c_str(), t);
#endif
	}

	void beginAccumulate()
	{
#ifdef __TRACE__
		_accumulate_time = gettime();
#endif
	}

	void accumulate(std::string title)
	{
#ifdef __TRACE__
		uint64_t time_new = gettime();
		long t = time_new - _accumulate_time; //ºŸ…Ë≤ªª·≥¨π˝long
		_accumulate_time = time_new;

		accumulated_time[title] += t;

#ifdef __PRINT_SLOW_ACCUMULATE__
		if (t >= 100000)
		{
			printf("<%s>: %ldms\n", title.c_str(), (t + 500) / 1000);
		}
#endif
#endif
	}

	void printStatistics(bool clear = false)
	{
#ifdef __TRACE__
    printf( "print statistics start......\n" );
		for (auto kvp : accumulated_time)
		{
			printf("{%s}: %ldms\n", kvp.first.c_str(), (kvp.second + 500) / 1000);
		}
		if (clear)
		{
			accumulated_time.clear();
		}
    printf( "print statistics end......\n" );
#endif
	}

	static void beginTrace_static();
	static void trace_static(std::string title);

	static void beginAccumulate_static();
	static void accumulate_static(std::string title);
	static void printStatistics_static(bool clear = false);
};


#ifdef __TRACE__

#define BEGIN_TRACE Trace::beginTrace_static()
#define TRACE(V) Trace::trace_static(V)

#define BEGIN_ACCUMULATE Trace::beginAccumulate_static()
#define ACCUMULATE(V) Trace::accumulate_static(V)
#define PRINTSTAT Trace::printStatistics_static()

#else

#define BEGIN_TRACE
#define TRACE(V)

#define BEGIN_ACCUMULATE
#define ACCUMULATE(V)
#define PRINTSTAT

#endif

#endif