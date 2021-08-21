#include "HighLoPch.h"
#include "Core.h"

static highlo::HLAllocationMetrics AllocationMetrics;

void *operator new(size_t size)
{
	AllocationMetrics.TotalAllocated += (uint32) size;
	return malloc(size);
}

void operator delete(void *memory, size_t size)
{
	AllocationMetrics.TotalFreed += (uint32) size;
	free(memory);
}

