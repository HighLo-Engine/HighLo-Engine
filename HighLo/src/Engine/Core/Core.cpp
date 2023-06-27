// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "Core.h"

static highlo::HLAllocationMetrics AllocationMetrics;

void *operator new(size_t size)
{
	AllocationMetrics.TotalAllocated += size;
	return malloc(size);
}

void operator delete(void *memory, size_t size)
{
	AllocationMetrics.TotalFreed += size;
	free(memory);
}

