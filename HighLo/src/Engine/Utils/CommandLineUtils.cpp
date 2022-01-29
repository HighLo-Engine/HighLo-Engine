#include "HighLoPch.h"
#include "CommandLineUtils.h"

namespace highlo::utils
{
	CommandLineHelper::CommandLineHelper()
	{
		ResetStates();
	}

	CommandLineHelper::CommandLineHelper(int32 argc, char *argv[])
	{
		for (int32 i = 0; i < argc; ++i)
		{
			m_Arguments.push_back(argv[i]);
		}

		Parse();
	}

	CommandLineHelper::~CommandLineHelper()
	{
	}

	void CommandLineHelper::Parse()
	{
		ResetStates();

		// index 0 is the program executable path, we don't need that
		for (uint32 i = 1; i < m_Arguments.size(); ++i)
		{
			HLString &cmd = m_Arguments[i];

			if (cmd == "--help")
			{
				m_HelpFlag = true;
			}
			else if (cmd == "--version" || cmd == "-v")
			{
				m_VersionFlag = true;
			}
			else if (cmd == "--headless")
			{
				m_Settings.Headless = true;
			}
			else if (cmd == "--project" || cmd == "-p")
			{
				m_Settings.ProjectPath = m_Arguments[i + 1];
			}
			else if (cmd == "--shader-registry")
			{
				m_Settings.ShaderRegistryPath = m_Arguments[i + 1];
			}
			else if (cmd == "--asset-registry")
			{
				m_Settings.AssetsRegistryPath = m_Arguments[i + 1];
			}
			else if (cmd == "--window-title")
			{
				m_Settings.WindowTitle = m_Arguments[i + 1];
			}
			else if (cmd == "--window-width")
			{
				m_Settings.WindowWidth = m_Arguments[i + 1].ToUInt32();
			}
			else if (cmd == "--window-height")
			{
				m_Settings.WindowHeight = m_Arguments[i + 1].ToUInt32();
			}
			else if (cmd == "--window-fullscreen")
			{
				m_Settings.Fullscreen = m_Arguments[i + 1].ToBool();
			}
			else if (cmd == "--window-maximized")
			{
				m_Settings.Maximized = m_Arguments[i + 1].ToBool();
			}
			else if (cmd == "--window-vsync")
			{
				m_Settings.VSync = m_Arguments[i + 1].ToBool();
			}
			else if (cmd == "--window-window-api")
			{
				m_Settings.WindowAPI = m_Arguments[i + 1];
			}
			else if (cmd == "--window-rendering-api")
			{
				m_Settings.RenderingAPI = m_Arguments[i + 1];
			}
			else if (cmd == "--nlog")
			{
				m_Settings.NoLog = true;
			}
			else
			{
				std::cout << "Error: Could not parse argument " << m_Arguments[i] << "! Please refer to the help menu with --help to see all valid arguments." << std::endl;
			}
		}
	}

	void CommandLineHelper::ResetStates()
	{
		// Set default states
		m_HelpFlag = false;
		m_VersionFlag = false;
	}
}

