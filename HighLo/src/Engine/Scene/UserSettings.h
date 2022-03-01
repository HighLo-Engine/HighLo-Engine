// Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-15) initial release
//

#pragma once

namespace highlo
{
	struct RecentProject
	{
		HLString Name;
		HLString FilePath;
		time_t LastOpened;
	};

	struct UserSettings : public IsSharedReference
	{
		bool ShowWelcomeScreen = true;
		HLString StartupProject;
		HLString FilePath;
		std::map<time_t, RecentProject, std::greater<time_t>> RecentProjects;
	};
}

