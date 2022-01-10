#pragma once

#include "Engine/Application/ApplicationSettings.h"

namespace highlo::utils
{
	class CommandLineHelper
	{
	public:

		CommandLineHelper();
		CommandLineHelper(int32 argc, char *argv[]);
		virtual ~CommandLineHelper();

		bool ShouldShowHelpMenu() const { return m_HelpFlag; }
		bool ShouldShowVersion() const { return m_VersionFlag; }

		ApplicationSettings &GetAppSettings() { return m_Settings; }
		const ApplicationSettings &GetAppSettings() const { return m_Settings; }

		std::vector<HLString> &GetRawArguments() { return m_Arguments; }
		const std::vector<HLString> &GetRawArguments() const { return m_Arguments; }

		void Parse();

	private:

		void ResetStates();

		std::vector<HLString> m_Arguments;

		// Flags
		bool m_HelpFlag;
		bool m_VersionFlag;

		ApplicationSettings m_Settings;
	};
}

