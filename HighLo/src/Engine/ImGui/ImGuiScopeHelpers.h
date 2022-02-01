// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-19) initial release
//

#pragma once

#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"

#include "Engine/Core/Core.h"

namespace highlo::UI
{
	class ScopedStyle
	{
	public:

		HLAPI ScopedStyle(const ScopedStyle&) = delete;
		HLAPI ScopedStyle &operator=(const ScopedStyle&) = delete;

		template<typename T>
		HLAPI ScopedStyle(ImGuiStyleVar styleVar, T style)
		{
			ImGui::PushStyleVar(styleVar, style);
		}

		HLAPI ~ScopedStyle()
		{
			ImGui::PopStyleVar();
		}
	};

	class ScopedColor
	{
	public:

		HLAPI ScopedColor(const ScopedColor&) = delete;
		HLAPI ScopedColor &operator=(const ScopedColor&) = delete;

		template<typename T>
		HLAPI ScopedColor(ImGuiCol colorID, T color)
		{
			ImGui::PushStyleColor(colorID, color);
		}

		HLAPI ~ScopedColor()
		{
			ImGui::PopStyleColor();
		}
	};

	class ScopedFont
	{
	public:

		HLAPI ScopedFont(const ScopedFont&) = delete;
		HLAPI ScopedFont &operator=(const ScopedFont&) = delete;

		HLAPI ScopedFont(ImFont *font)
		{
			ImGui::PushFont(font);
		}

		HLAPI ~ScopedFont()
		{
			ImGui::PopFont();
		}
	};

	class ScopedID
	{
	public:

		HLAPI ScopedID(const ScopedID&) = delete;
		HLAPI ScopedID &operator=(const ScopedID&) = delete;

		template<typename T>
		HLAPI ScopedID(T id)
		{
			ImGui::PushID(id);
		}

		HLAPI ~ScopedID()
		{
			ImGui::PopID();
		}
	};

	class ScopedColorStack
	{
	public:

		HLAPI ScopedColorStack(const ScopedColorStack&) = delete;
		HLAPI ScopedColorStack &operator=(const ScopedColorStack&) = delete;

		template<typename ColorType, typename... OtherColors>
		HLAPI ScopedColorStack(ImGuiCol colorID, ColorType color, OtherColors&& ...otherColorPairs)
			: m_Count((sizeof... (otherColorPairs) / 2) + 1)
		{
			static_assert((sizeof... (otherColorPairs) & 1u) == 0);
			PushColor(colorID, color, std::forward<OtherColors>(otherColorPairs)...);
		}

		HLAPI ~ScopedColorStack()
		{
			ImGui::PopStyleColor(m_Count);
		}

	private:

		template<typename ColorType, typename... OtherColors>
		void PushColor(ImGuiCol id, ColorType color, OtherColors&& ...otherColorPairs)
		{
			if constexpr (sizeof... (otherColorPairs) == 0)
			{
				ImGui::PushStyleColor(id, color);
			}
			else
			{
				ImGui::PushStyleColor(id, color);
				PushColor(std::forward<OtherColors>(otherColorPairs)...);
			}
		}

		uint32 m_Count;
	};

	class ScopedStyleStack
	{
	public:

		HLAPI ScopedStyleStack(const ScopedStyleStack&) = delete;
		HLAPI ScopedStyleStack &operator=(const ScopedStyleStack&) = delete;

		template<typename StyleType, typename... OtherStyles>
		HLAPI ScopedStyleStack(ImGuiStyleVar id, StyleType style, OtherStyles&& ...otherStylePairs)
			: m_Count((sizeof... (otherStylePairs) / 2) + 1)
		{
			static_assert((sizeof... (otherStylePairs) & 1u) == 0);
			PushStyle(id, style, otherStylePairs);
		}

		HLAPI ~ScopedStyleStack()
		{
			ImGui::PopStyleVar(m_Count);
		}

	private:

		template<typename StyleType, typename... OtherStyles>
		void PushStyle(ImGuiStyleVar id, StyleType style, OtherStyles&& ...otherStylePairs)
		{
			if constexpr (sizeof... (otherStylePairs) == 0)
			{
				ImGui::PushStyleVar(id, style);
			}
			else
			{
				ImGui::PushStyleVar(id, style);
				PushStyle(std::forward<OtherStyles>(otherStylePairs)...);
			}
		}

		uint32 m_Count;
	};

	class ScopedItemFlags
	{
	public:
		
		HL_NON_COPYABLE(ScopedItemFlags);

		HLAPI ScopedItemFlags(const ImGuiItemFlags flags, const bool enabled = true)
		{
			ImGui::PushItemFlag(flags, enabled);
		}

		HLAPI ~ScopedItemFlags()
		{
			ImGui::PopItemFlag();
		}
	};
}

