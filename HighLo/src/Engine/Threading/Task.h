// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-10-27) initial release
//

#pragma once

#include "Engine/Core/Atomic.h"

namespace highlo
{
	enum class TaskState
	{
		Created = 0,
		Failed,
		Canceled,
		Queued,
		Running,
		Finished
	};

	class Task
	{
	public:

		HL_NON_COPYABLE(Task);

		HLAPI HL_FORCE_INLINE TaskState GetState() const
		{
			return static_cast<TaskState>(Atomic::AtomicRead((int64 volatile*)&m_State));
		}

		HLAPI HL_FORCE_INLINE Task *GetNextTask() const
		{
			return m_NextTask;
		}

		HLAPI HL_FORCE_INLINE bool IsCanceled() const
		{
			return m_State == TaskState::Canceled;
		}

		HLAPI HL_FORCE_INLINE bool IsFailed() const
		{
			return m_State == TaskState::Failed;
		}

		HLAPI HL_FORCE_INLINE bool IsQueued() const
		{
			return m_State == TaskState::Queued;
		}

		HLAPI HL_FORCE_INLINE bool IsRunning() const
		{
			return m_State == TaskState::Running;
		}

		HLAPI HL_FORCE_INLINE bool IsFinished() const
		{
			return m_State == TaskState::Finished;
		}

		HLAPI HL_FORCE_INLINE bool HasEnded() const
		{
			return m_State == TaskState::Failed || m_State == TaskState::Canceled || m_State == TaskState::Finished;
		}

		HLAPI HL_FORCE_INLINE bool HasBeenCanceled() const
		{
			return Atomic::AtomicRead((volatile int64*)&m_CancelFlag) != 0;
		}

		HLAPI void Start();
		HLAPI void Cancel();

		template<typename T = Task>
		HLAPI static bool WaitAll(std::vector<T*> &tasks, double timeoutInMilliseconds = -1.0)
		{
			for (int32 i = 0; i < tasks.size(); ++i)
			{
				if (tasks[i]->Wait())
					return true;
			}

			return false;
		}

		template<typename T = Task>
		HLAPI static void CancelAll(std::vector<T*> &tasks)
		{
			// Cancel all registered tasks and clear the array
			for (int32 i = 0; i < tasks.size(); ++i)
				tasks[i]->Cancel();

			tasks.clear();
		}

		HLAPI Task *ContinueWith(Task *task);
		HLAPI Task *StartNew(Task *task);

	protected:

		HLAPI void Execute();
		HLAPI bool Wait(double timeInMilliseconds = -1.0) const;

		HLAPI virtual bool Run() = 0;
		HLAPI virtual void Enqueue() = 0;
		HLAPI virtual void OnStart();
		HLAPI virtual void OnEnd();
		HLAPI virtual void OnFinish();
		HLAPI virtual void OnFail();
		HLAPI virtual void OnCancel();

		volatile int64 m_CancelFlag = 0;
		volatile TaskState m_State = TaskState::Created;
		Task *m_NextTask = nullptr;
	};
}

