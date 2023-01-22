// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-10-21) initial release
//

#pragma once

#include "Engine/Core/Core.h"

namespace highlo
{
	/// <summary>
	/// Defines the function scope for a Runnable function inside Thread operations.
	/// </summary>
	class IRunnable
	{
	public:

		HLAPI virtual ~IRunnable();

		HLAPI virtual bool Init();
		HLAPI virtual int32 Run() = 0;
		HLAPI virtual void Stop();
		HLAPI virtual void Exit();

		HLAPI virtual void AfterExit(bool wasKilled);
		HLAPI virtual void OnThreadStart();
		HLAPI virtual void OnThreadEnd(int32 exitCode);
	};

	using RunnableCallback = std::function<int32()>;

	/// <summary>
	/// Represents a function that can be run inside a Thread operation.
	/// </summary>
	class Runnable : public IRunnable
	{
	public:

		HLAPI Runnable(const RunnableCallback &callback, bool autoDelete = true);
		
		HLAPI int32 Run() override;
		HLAPI void AfterExit(bool wasKilled) override;

		HLAPI void SetCallback(const RunnableCallback &callback);
		HLAPI void ShouldAutoDeleteAfterJob(bool autoDelete = true);

	private:

		bool m_AutoDelete;
		RunnableCallback m_CallbackFn;
	};
}

