#include "HighLoPch.h"
#include "Engine/Threading/RenderThread.h"

#ifdef HL_PLATFORM_WINDOWS

namespace highlo
{
	RenderThreadBase::RenderThreadBase(const HLString &name)
		: m_Name(name)
	{
	}

	void RenderThreadBase::SetName(const HLString &name)
	{
		HANDLE threadHandle = m_Thread.native_handle();
		std::string str = *name;
		std::wstring wName(str.begin(), str.end());

		SetThreadDescription(threadHandle, wName.c_str());
		SetThreadAffinityMask(threadHandle, 8);
	}

	void RenderThreadBase::Join()
	{
		m_Thread.join();
	}

	RenderThreadSignal::RenderThreadSignal(const HLString &name, bool manualReset)
	{
		std::string str = *name;
		std::wstring wName(str.begin(), str.end());
		m_SignalHandle = CreateEvent(NULL, (BOOL)manualReset, FALSE, wName.c_str());
	}

	void RenderThreadSignal::Wait()
	{
		WaitForSingleObject(m_SignalHandle, INFINITE);
	}

	void RenderThreadSignal::Signal()
	{
		SetEvent(m_SignalHandle);
	}

	void RenderThreadSignal::Reset()
	{
		ResetEvent(m_SignalHandle);
	}
}

#endif // HL_PLATFORM_WINDOWS