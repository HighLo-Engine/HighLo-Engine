// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

//
// version history:
//     - 1.1 (2021-10-17) Refactored Encryption class to not have a extra unsigned char* function
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Engine/Core/Core.h"
#include "Base64.h"

namespace highlo
{
	enum class EncryptionAlgorithm
	{
		None = 0,

		AES_128_CBC,
		AES_256_CBC,
		AES_192_CBC,

		AES_128_CCM,
		AES_256_CCM,
		AES_192_CCM,

		AES_128_CFB,
		AES_256_CFB,
		AES_192_CFB,


	};

	class Encryptor : public IsSharedReference
	{
	public:

		HLAPI Encryptor() = default;
		HLAPI Encryptor(const HLString &key, const HLString &iv, EncryptionAlgorithm algorithm = EncryptionAlgorithm::AES_256_CBC);
		HLAPI ~Encryptor();

		HLAPI void Init();
		HLAPI void Shutdown();

		HLAPI HLString Encrypt(const HLString &plainText);
		HLAPI HLString Decrypt(const HLString &cipherText);

		HLAPI HLString EncryptBase64(const HLString &plainText);
		HLAPI HLString DecryptBase64(const HLString &cipherText);

	private:

		HLString m_Key;
		HLString m_IV;
		EncryptionAlgorithm m_Algorithm;
	};
}

