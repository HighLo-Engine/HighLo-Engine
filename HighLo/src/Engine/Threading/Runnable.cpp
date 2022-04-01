// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "Runnable.h"

namespace highlo
{
	IRunnable::~IRunnable()
	{
	}

	bool IRunnable::Init()
	{
		return true;
	}
	
	int32 IRunnable::Run()
	{
		return 1;
	}
	
	void IRunnable::Stop()
	{
	}
	
	void IRunnable::Exit()
	{
	}
	
	void IRunnable::AfterExit(bool wasKilled)
	{
	}
	
	void IRunnable::OnThreadStart()
	{
	}
	
	void IRunnable::OnThreadEnd(int32 exitCode)
	{
	}

	// =================================================================================================
	//                                         Runnable
	// =================================================================================================

	Runnable::Runnable(const RunnableCallback &callback, bool autoDelete)
		: m_AutoDelete(autoDelete), m_CallbackFn(callback)
	{
	}

	int32 Runnable::Run()
	{
		int32 result = -1;
		result = m_CallbackFn();
		return result;
	}

	void Runnable::AfterExit(bool wasKilled)
	{
		if (m_AutoDelete)
			delete this;
	}
	
	void Runnable::SetCallback(const RunnableCallback &callback)
	{
		m_CallbackFn = callback;
	}

	void Runnable::ShouldAutoDeleteAfterJob(bool autoDelete)
	{
		m_AutoDelete = autoDelete;
	}
}

