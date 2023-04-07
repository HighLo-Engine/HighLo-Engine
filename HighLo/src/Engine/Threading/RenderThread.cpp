#include "HighLoPch.h"
#include "RenderThread.h"

#include "Engine/Renderer/Renderer.h"

namespace highlo
{
	struct RenderThreadData
	{
		CRITICAL_SECTION CriticalSection;
		CONDITION_VARIABLE ConditionVariable;
		RenderThread::State State = RenderThread::State::Idle;
	};

	RenderThread::RenderThread(RenderThreadPolicy policy)
		: m_Thread("RenderThread"), m_Policy(policy)
	{
		m_Data = new RenderThreadData();

		if (m_Policy == RenderThreadPolicy::MultiThreaded)
		{
			InitializeCriticalSection(&m_Data->CriticalSection);
			InitializeConditionVariable(&m_Data->ConditionVariable);
		}
	}

	RenderThread::~RenderThread()
	{
		if (m_Policy == RenderThreadPolicy::MultiThreaded)
		{
			DeleteCriticalSection(&m_Data->CriticalSection);
		}

		delete m_Data;
		m_Data = nullptr;
	}

	void RenderThread::Run()
	{
		m_IsRunning = true;
		if (m_Policy == RenderThreadPolicy::MultiThreaded)
			m_Thread.Dispatch(Renderer::RenderThreadFunc, this);
	}

	void RenderThread::Terminate()
	{
		m_IsRunning = false;
		Pump();

		if (m_Policy == RenderThreadPolicy::MultiThreaded)
			m_Thread.Join();
	}

	void RenderThread::Wait(State waitState)
	{
		if (m_Policy == RenderThreadPolicy::SingleThreaded)
			return;

		EnterCriticalSection(&m_Data->CriticalSection);
		while (m_Data->State != waitState)
		{
			SleepConditionVariableCS(&m_Data->ConditionVariable, &m_Data->CriticalSection, INFINITE);
		}
		LeaveCriticalSection(&m_Data->CriticalSection);
	}

	void RenderThread::WaitAndSet(State waitState, State setState)
	{
		if (m_Policy == RenderThreadPolicy::SingleThreaded)
			return;

		EnterCriticalSection(&m_Data->CriticalSection);
		while (m_Data->State != waitState)
		{
			SleepConditionVariableCS(&m_Data->ConditionVariable, &m_Data->CriticalSection, INFINITE);
		}
		m_Data->State = setState;
		WakeAllConditionVariable(&m_Data->ConditionVariable);
		LeaveCriticalSection(&m_Data->CriticalSection);
	}

	void RenderThread::Set(State setState)
	{
		if (m_Policy == RenderThreadPolicy::SingleThreaded)
			return;

		EnterCriticalSection(&m_Data->CriticalSection);
		m_Data->State = setState;
		WakeAllConditionVariable(&m_Data->ConditionVariable);
		LeaveCriticalSection(&m_Data->CriticalSection);
	}

	void RenderThread::NextFrame()
	{
		m_AppThreadFrame++;
		Renderer::SwapQueues();
	}

	void RenderThread::BlockUntilRenderComplete()
	{
		if (m_Policy == RenderThreadPolicy::SingleThreaded)
			return;

		Wait(State::Idle);
	}

	void RenderThread::Kick()
	{
		if (m_Policy == RenderThreadPolicy::MultiThreaded)
		{
			Set(State::Kick);
		}
		else
		{
			Renderer::WaitAndRender(this);
		}
	}

	void RenderThread::Pump()
	{
		NextFrame();
		Kick();
		BlockUntilRenderComplete();
	}

}

