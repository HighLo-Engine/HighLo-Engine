// Copyright (c) 2021 Can Karka and Albert Slepak. All rights reserved.

#include "HighLoPch.h"
#include "Encryptor.h"

#include <openssl/conf.h>
#include <openssl/evp.h>

namespace highlo
{
	static EVP_CIPHER_CTX *s_CipherContext = nullptr;
	
	namespace utils
	{
		static const EVP_CIPHER *ConvertAlgorithmFromType(EncryptionAlgorithm algorithm)
		{
			switch (algorithm)
			{
				case EncryptionAlgorithm::AES_128_CBC:
					return EVP_aes_128_cbc();

				case EncryptionAlgorithm::AES_256_CBC:
					return EVP_aes_256_cbc();

				case EncryptionAlgorithm::AES_192_CBC:
					return EVP_aes_192_cbc();

				case EncryptionAlgorithm::AES_128_CCM:
					return EVP_aes_128_ccm();

				case EncryptionAlgorithm::AES_256_CCM:
					return EVP_aes_256_ccm();

				case EncryptionAlgorithm::AES_192_CCM:
					return EVP_aes_192_ccm();

				case EncryptionAlgorithm::AES_128_CFB:
					return EVP_aes_128_cfb128();

				case EncryptionAlgorithm::AES_256_CFB:
					return EVP_aes_256_cfb128();

				case EncryptionAlgorithm::AES_192_CFB:
					return EVP_aes_192_cfb128();



				case EncryptionAlgorithm::None:
				default:
					HL_ASSERT(false, "Unknown algorithm!");
					return nullptr;
			}
		}
	}

	Encryptor::Encryptor(const HLString &key, const HLString &iv, EncryptionAlgorithm algorithm)
		: m_Key(key), m_IV(iv), m_Algorithm(algorithm)
	{
		if (!s_CipherContext)
		{
			s_CipherContext = EVP_CIPHER_CTX_new();
			if (!s_CipherContext)
				HL_CORE_ERROR("Error: Cipher context could not be created!");
		}
	}

	Encryptor::~Encryptor()
	{
	}

	void Encryptor::Init()
	{
		// This Init function is used by the engine itself, without the user being able to manipulate parameters here

		// TODO: all the passphrases, salts, keys and ivs are going to be part of config files in the future, we don't want them to be accessible directly here
		// because the source is open and it shouldn't be visible to the public. 

		// Pass = secretPassPhrase
		// Salt = 2986C2DB93452761
		m_Key = "58E7151818B518BF2A490389C8EE1C150F0C50D9564A9A6688215F9AA9397F77";
		m_IV = "8ED75991622CE5C4EB201C26B31D60A1";
		m_Algorithm = EncryptionAlgorithm::AES_256_CBC;
		
		if (!s_CipherContext)
		{
			s_CipherContext = EVP_CIPHER_CTX_new();
			if (!s_CipherContext)
				std::cout << "Error: Cipher context could not be created!" << std::endl;
		}
	}

	void Encryptor::Shutdown()
	{
		if (s_CipherContext)
			EVP_CIPHER_CTX_free(s_CipherContext);
	}
	
	HLString Encryptor::Encrypt(const HLString &plainText)
	{
		int32 length = 0;
		uint32 ciphertextLength = 0;
		unsigned char resultCipher[4096];
		unsigned char *plainTextRaw = (unsigned char*)*plainText;

		if (!EVP_EncryptInit_ex(s_CipherContext, utils::ConvertAlgorithmFromType(m_Algorithm), NULL, (unsigned char*)*m_Key, (unsigned char*)*m_IV))
			std::cout << "Error: Could not initialize the Encryptor" << std::endl;

		if (!EVP_EncryptUpdate(s_CipherContext, resultCipher, &length, plainTextRaw, plainText.Length()))
			std::cout << "Error: Could not update the encryption" << std::endl;

		ciphertextLength = length;

		if (!EVP_EncryptFinal_ex(s_CipherContext, resultCipher + length, &length))
			std::cout << "Error: Could not encrypt" << std::endl;

		ciphertextLength += length;
		HLString result = HLString((const char*)resultCipher, ciphertextLength);
	//	delete[] &resultCipher;
		return result;
	}
	
	HLString Encryptor::Decrypt(const HLString &cipherText)
	{
		int32 length = 0;
		uint32 plaintextLength = 0;
		unsigned char resultPlainText[4096];
		unsigned char *cipherTextRaw = (unsigned char*)*cipherText;

		if (!EVP_DecryptInit_ex(s_CipherContext, utils::ConvertAlgorithmFromType(m_Algorithm), NULL, (unsigned char*)*m_Key, (unsigned char*)*m_IV))
			std::cout << "Error: Could not initialize the decryptor" << std::endl;

		if (!EVP_DecryptUpdate(s_CipherContext, resultPlainText, &length, cipherTextRaw, cipherText.Length()))
			std::cout << "Error: Could not update the decryption" << std::endl;

		plaintextLength = length;

		if (!EVP_DecryptFinal_ex(s_CipherContext, resultPlainText + length, &length))
			std::cout << "Error: Could not decrypt" << std::endl;

		plaintextLength += length;
		HLString result = HLString((const char*)resultPlainText, plaintextLength);
	//	delete[] &resultPlainText;
		return result;
	}
	
	HLString Encryptor::EncryptBase64(const HLString &plainText)
	{
		HLString cipher = Encrypt(plainText);
		return Base64::Encode((Byte*)*cipher, cipher.Length());
	}
	
	HLString Encryptor::DecryptBase64(const HLString &cipherText)
	{
		HLString cipher = Base64::DecodeToString(cipherText);
		return Decrypt(cipher);
	}
}

