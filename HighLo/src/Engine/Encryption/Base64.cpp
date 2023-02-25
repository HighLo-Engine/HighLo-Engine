// Copyright (c) 2021-2023 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "Base64.h"

namespace highlo
{
    static const HLString base64_chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";

	bool Base64::IsBase64(Byte c)
	{
		return (isalnum(c) || (c == '+') || (c == '/'));
	}
	
	HLString Base64::Encode(const Byte *buffer, uint32 bufferLength)
	{
        HLString result;
        int32 i = 0;
        int32 j = 0;
        Byte char_array_3[3];
        Byte char_array_4[4];

        while (bufferLength--)
        {
            char_array_3[i++] = *(buffer++);
            if (i == 3)
            {
                char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
                char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
                char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
                char_array_4[3] = char_array_3[2] & 0x3f;

                for (i = 0; (i < 4); i++)
                    result += base64_chars[char_array_4[i]];
                i = 0;
            }
        }

        if (i)
        {
            for (j = i; j < 3; j++)
                char_array_3[j] = '\0';

            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for (j = 0; (j < i + 1); j++)
                result += base64_chars[char_array_4[j]];

            while ((i++ < 3))
                result += '=';
        }

        return result;
    }
	
	std::vector<Byte> Base64::Decode(const HLString &encodedString)
	{
        int32 in_len = (int32)encodedString.Length();
        int32 i = 0;
        int32 j = 0;
        int32 in_ = 0;
        Byte char_array_4[4], char_array_3[3];
        std::vector<Byte> result;

        while (in_len-- && (encodedString[in_] != '=') && IsBase64(encodedString[in_]))
        {
            char_array_4[i++] = encodedString[in_]; in_++;
            if (i == 4)
            {
                for (i = 0; i < 4; i++)
                    char_array_4[i] = base64_chars.IndexOf(char_array_4[i]);

                char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
                char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
                char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

                for (i = 0; (i < 3); i++)
                    result.push_back(char_array_3[i]);
                i = 0;
            }
        }

        if (i)
        {
            for (j = i; j < 4; j++)
                char_array_4[j] = 0;

            for (j = 0; j < 4; j++)
                char_array_4[j] = base64_chars.IndexOf(char_array_4[j]);

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (j = 0; (j < i - 1); j++)
                result.push_back(char_array_3[j]);
        }

        return result;
    }
	
	HLString Base64::DecodeToString(const HLString &encodedString)
	{
        int32 in_len = (int32)encodedString.Length();
        int32 i = 0;
        int32 j = 0;
        int32 in_ = 0;
        unsigned char char_array_4[4], char_array_3[3];
        HLString result;

        while (in_len-- && (encodedString[in_] != '=') && IsBase64(encodedString[in_]))
        {
            char_array_4[i++] = encodedString[in_]; in_++;
            if (i == 4)
            {
                for (i = 0; i < 4; i++)
                    char_array_4[i] = base64_chars.IndexOf(char_array_4[i]);

                char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
                char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
                char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

                for (i = 0; (i < 3); i++)
                    result += char_array_3[i];
                i = 0;
            }
        }

        if (i)
        {
            for (j = i; j < 4; j++)
                char_array_4[j] = 0;

            for (j = 0; j < 4; j++)
                char_array_4[j] = base64_chars.IndexOf(char_array_4[j]);

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (j = 0; (j < i - 1); j++)
                result += char_array_3[j];
        }

        return result;
    }
}

