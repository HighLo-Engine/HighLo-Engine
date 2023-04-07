#pragma once

namespace highlo
{
	struct RenderThreadData;

	class RenderThreadBase
	{
	public:

		RenderThreadBase(const HLString &name);

		template<typename Fn, typename... Args>
		void Dispatch(Fn &&func, Args &&...args)
		{
			m_Thread = std::thread(func, std::forward<Args>(args)...);
			SetName(m_Name);
		}

		void SetName(const HLString &name);
		void Join();

	private:

		HLString m_Name;
		std::thread m_Thread;
	};

	class RenderThreadSignal
	{
	public:

		RenderThreadSignal(const HLString &name, bool manualReset = false);

		void Wait();
		void Signal();
		void Reset();

	private:

		void *m_SignalHandle = nullptr;
	};

	enum class RenderThreadPolicy
	{
		None = 0,
		SingleThreaded,
		MultiThreaded
	};

	class RenderThread
	{
	public:

		enum class State
		{
			Idle = 0,
			Busy,
			Kick
		};

		RenderThread(RenderThreadPolicy policy);
		~RenderThread();

		void Run();
		bool IsRunning() const { return m_IsRunning; }
		void Terminate();

		void Wait(State waitState);
		void WaitAndSet(State waitState, State setState);
		void Set(State setState);
		
		void NextFrame();
		void BlockUntilRenderComplete();
		void Kick();
		void Pump();

	private:

		RenderThreadData *m_Data;
		RenderThreadPolicy m_Policy;

		RenderThreadBase m_Thread;
		bool m_IsRunning = false;
		std::atomic<uint32> m_AppThreadFrame = 0;
	};
}

