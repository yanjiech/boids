#include "Trace.h"

Trace __global_t;

void Trace::beginTrace_static()
{
	__global_t.beginTrace();
}

void Trace::trace_static(std::string title)
{
	__global_t.trace(title);
}

void Trace::beginAccumulate_static()
{
	__global_t.beginAccumulate();
}

void Trace::accumulate_static(std::string title)
{
	__global_t.accumulate(title);
}

void Trace::printStatistics_static(bool clear)
{
	__global_t.printStatistics(clear);
}