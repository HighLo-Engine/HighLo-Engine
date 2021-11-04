// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "Task.h"

namespace highlo
{
	void Task::Start()
	{
		if (m_State != TaskState::Created)
			return;

		OnStart();

		m_State = TaskState::Queued;
		Enqueue();
	}
	
	void Task::Cancel()
	{
		if (Atomic::AtomicRead((int64 volatile*)&m_CancelFlag) != 0)
		{
			OnCancel();

			if (m_NextTask)
				m_NextTask->Cancel();
		}
	}
	
	Task *Task::ContinueWith(Task *task)
	{
		HL_ASSERT(task != nullptr && task != this);

		if (m_NextTask)
			return m_NextTask->ContinueWith(task);

		m_NextTask = task;
		return task;
	}
	
	Task *Task::StartNew(Task *task)
	{
		HL_ASSERT(task);
		task->Start();
		return task;
	}
	
	void Task::Execute()
	{
		if (IsCanceled())
			return;

		HL_ASSERT(IsQueued());
		m_State = TaskState::Running;

		bool failed = Run();
		if (HasBeenCanceled())
		{
			m_State = TaskState::Canceled;
		}
		else if (failed)
		{
			OnFail();
		}
		else
		{
			OnFinish();
		}
	}
	
	bool Task::Wait(double timeInMilliseconds) const
	{
		return false;
	}
	
	void Task::OnStart()
	{
	}
	
	void Task::OnEnd()
	{
		HL_ASSERT(!IsRunning());

		// TODO: Delete object
	}
	
	void Task::OnFinish()
	{
		HL_ASSERT(IsRunning());
		HL_ASSERT(!HasBeenCanceled());

		m_State = TaskState::Finished;

		if (m_NextTask)
			m_NextTask->Start();

		OnEnd();
	}
	
	void Task::OnFail()
	{
		m_State = TaskState::Failed;

		if (m_NextTask)
			m_NextTask->OnFail();

		OnEnd();
	}
	
	void Task::OnCancel()
	{
		// TODO
	}
}

