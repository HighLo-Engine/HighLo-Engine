// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "Service.h"

#include "Engine/Core/DataTypes/Sorting.h"

namespace highlo
{
	namespace utils
	{
		static bool CompareServices(Service *const &a, Service *const &b)
		{
			return a->Order < b->Order;
		}
	}


	Service::ServiceArray &Service::GetServices()
	{
		static ServiceArray services;
		return services;
	}

	void Service::Sort()
	{
		auto &services = GetServices();
		if (!services.empty())
			Sorting::QuickSort(&services[0], services.size(), &utils::CompareServices);
	}

	bool Service::Init()
	{
		return false;
	}

	Service::Service(const HLString &name, int32 order)
	{
		Name = name;
		Order = order;

		auto &services = GetServices();
		services.push_back(this);

		if (services[0]->m_IsInitialized)
			Sort();
	}

	void Service::OnUpdate(bool updateBackwards)
	{
		auto &services = GetServices();
		if (updateBackwards)
		{
			for (int32 i = (int32) services.size() - 1; i >= 0; --i)
			{
				const auto service = services[i];
				service->Update();
			}
		}
		else
		{
			for (int32 i = 0; i < (int32)services.size(); ++i)
			{
				const auto service = services[i];
				service->Update();
			}
		}
	}

	void Service::OnInit()
	{
		Sort();

		auto &services = GetServices();
		for (int32 i = 0; i < services.size(); ++i)
		{
			const auto service = services[i];

			HL_CORE_INFO("Initialize {0}...", *service->Name);
			service->m_IsInitialized = true;
			if (service->Init())
			{
				HL_CORE_ERROR("Failed to initialize {0}", *service->Name);
			}
		}

		HL_CORE_INFO("All Services are ready!");
	}

	void Service::OnExit()
	{
		auto &services = GetServices();
		for (int32 i = (int32)services.size() - 1; i >= 0; --i)
		{
			const auto service = services[i];
			if (service->m_IsInitialized)
			{
				service->m_IsInitialized = false;
				service->Exit();
			}
		}
	}
}

