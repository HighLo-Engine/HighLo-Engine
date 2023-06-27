// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.2 (2021-10-21) Added Unique reference class (equals std::unique_ptr)
//     - 1.1 (2021-09-22) Added Singleton class
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#ifdef HL_PLATFORM_LINUX
    #define STDC_LIB_EXT1
    #define STDC_WANT_LIB_EXT1 1
#endif

/* Basic Stuff */
#include <atomic>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <limits>
#include <memory>
#include <functional>
#include <algorithm>
#include <ctime>
#include <chrono>
#include <vector>
#include <array>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

/* Streams */
#include <fstream>
#include <sstream>

#include "Engine/Core/Core.h"
#include "Engine/Core/Singleton.h"
#include "Engine/Core/SharedReference.h"
#include "Engine/Core/UniqueReference.h"
#include "Engine/Core/WeakReference.h"
#include "Engine/Core/DataTypes/String.h"
#include "Engine/Core/Exceptions/Exceptions.h"

