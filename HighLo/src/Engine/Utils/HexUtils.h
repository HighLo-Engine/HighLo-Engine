// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-15) initial release
//

#pragma once

#include "Engine/Core/Log.h"

namespace highlo
{
	namespace utils
	{
		static void Hexdump(const char *headline, const void *addr, int32 size, const int32 width)
		{
            int32  offset;
            char *p, c[256], strBuf[64];
            bool more = false;

            p = (char*)addr;
            if (!size) 
                size = (int32)strlen((char*)addr);

            // Disable type conversion warning
        #pragma warning(push)
        #pragma warning(disable : 4311)
        #pragma warning(disable : 4302)
            sprintf_s(strBuf, "Hexdump([%s], $%08lx, %d, %d)", headline, (int32)p, size, width);
        #pragma warning(pop)
            HL_CORE_TRACE("{0}", strBuf);

            for (offset = 0; offset < size; ++offset)
            {
                if (0 == offset % width)
                {
                    sprintf_s(strBuf, "%06lx: ", offset);
                    HL_CORE_TRACE("{0}", strBuf);
                }

                sprintf_s(strBuf, "%02x ", p[offset] & 0xFF); HL_CORE_TRACE("{0}", strBuf);
                c[offset % width] = isprint(p[offset]) ? p[offset] : '.';

                if ((width / 2 - 1) == offset % width) 
                    HL_CORE_TRACE(" ");

                if ((width - 1) == offset % width)
                {
                    c[width] = 0;  sprintf_s(strBuf, " : %s", c);
                    HL_CORE_TRACE("{0}", strBuf);
                }
            }

            // Output final bytes
            while (0 != offset % width)
            {
                HL_CORE_TRACE("-- ");

                if ((width / 2 - 1) == (offset % width)) 
                    HL_CORE_TRACE(" ");

                c[offset % width] = ' ';
                ++offset;
                more = true;
            }

            c[width] = 0;
            if (more)
                HL_CORE_TRACE(" : {0}", c);
		}
	}
}

