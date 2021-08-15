#include "HighLoPch.h"
#include "Encryptor.h"

#include <openssl/conf.h>
#include <openssl/evp.h>

namespace highlo
{
	static EVP_CIPHER_CTX *CipherContext = nullptr;
	
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
		: Key(key), IV(iv), Algorithm(algorithm)
	{
		if (!CipherContext)
		{
			CipherContext = EVP_CIPHER_CTX_new();
			if (!CipherContext)
				HL_CORE_ERROR("Error: Cipher context could not be created!");
		}
	}

	Encryptor::~Encryptor()
	{
		if (CipherContext)
			EVP_CIPHER_CTX_free(CipherContext);
	}

	uint32 Encryptor::Encrypt(unsigned char *plainText, uint32 plainTextLength, unsigned char *cipherText)
	{
		int32 len = 0;
		uint32 ciphertext_len = 0;

		if (!EVP_EncryptInit_ex(CipherContext, utils::ConvertAlgorithmFromType(Algorithm), NULL, (unsigned char*)*Key, (unsigned char*)*IV))
			HL_CORE_ERROR("Error: Could not initialize the Encryptor");

		if (!EVP_EncryptUpdate(CipherContext, cipherText, &len, plainText, plainTextLength))
			HL_CORE_ERROR("Error: Could not update the encryption");

		ciphertext_len = len;

		if (!EVP_EncryptFinal_ex(CipherContext, cipherText + len, &len))
			HL_CORE_ERROR("Error: Could not encrypt");

		ciphertext_len += len;
		cipherText[ciphertext_len] = '\0';
		return ciphertext_len;
	}

	uint32 Encryptor::Decrypt(unsigned char *cipherText, uint32 cipherTextLength, unsigned char *plainText)
	{
		int32 len = 0;
		uint32 plaintext_len = 0;

		if (!EVP_DecryptInit_ex(CipherContext, utils::ConvertAlgorithmFromType(Algorithm), NULL, (unsigned char *)*Key, (unsigned char *)*IV))
			HL_CORE_ERROR("Error: Could not initialize the decryptor");

		if (!EVP_DecryptUpdate(CipherContext, plainText, &len, cipherText, cipherTextLength))
			HL_CORE_ERROR("Error: Could not update the decryption");

		plaintext_len = len;

		if (!EVP_DecryptFinal_ex(CipherContext, plainText + len, &len))
			HL_CORE_ERROR("Error: Could not decrypt");

		plaintext_len += len;
		plainText[plaintext_len] = '\0';
		return plaintext_len;
	}
	
	HLString Encryptor::Encrypt(const HLString &plainText)
	{
		unsigned char cipherRaw[256];
		unsigned char *plainTextRaw = (unsigned char*)*plainText;
		uint32 plainTextLength = (uint32)plainText.Length();
		uint32 cipherLength = 0;

		cipherLength = Encrypt(plainTextRaw, plainTextLength, cipherRaw);
		return HLString((const char*)cipherRaw, cipherLength);
	}
	
	HLString Encryptor::Decrypt(const HLString &cipherText)
	{
		unsigned char plainTextRaw[256];
		unsigned char *cipherRaw = (unsigned char*)*cipherText;
		uint32 cipherLength = (uint32)cipherText.Length();
		uint32 plainTextLength = 0;

		plainTextLength = Decrypt(cipherRaw, cipherLength, plainTextRaw);
		return HLString((const char*) plainTextRaw, plainTextLength);
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

