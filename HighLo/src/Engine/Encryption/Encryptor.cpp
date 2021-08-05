#include "HighLoPch.h"
#include "Encryptor.h"

#include <openssl/conf.h>
#include <openssl/evp.h>

// Temp
#define KEY ""
#define IV ""

namespace highlo
{
	uint32 Encryptor::Encrypt(unsigned char *plainText, uint32 plainTextLength, unsigned char *cipherText)
	{
		EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
		int32 len;
		uint32 ciphertext_len;

		if (!ctx)
		{

		}

		if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, (unsigned char*)KEY, (unsigned char*)IV))
		{

		}

		if (1 != EVP_EncryptUpdate(ctx, cipherText, &len, plainText, plainTextLength))
		{

		}

		ciphertext_len = len;

		if (1 != EVP_EncryptFinal_ex(ctx, cipherText + len, &len))
		{

		}

		ciphertext_len += len;

		EVP_CIPHER_CTX_free(ctx);
		cipherText[ciphertext_len] = '\0';
		return ciphertext_len;
	}

	uint32 Encryptor::Decrypt(unsigned char *cipherText, uint32 cipherTextLength, unsigned char *plainText)
	{
		EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
		int32 len;
		uint32 plaintext_len;

		if (!ctx)
		{

		}

		if (!EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, (unsigned char *) KEY, (unsigned char *) IV))
		{

		}

		if (!EVP_DecryptUpdate(ctx, plainText, &len, cipherText, cipherTextLength))
		{

		}

		plaintext_len = len;

		if (!EVP_DecryptFinal_ex(ctx, plainText + len, &len))
		{

		}

		plaintext_len += len;

		EVP_CIPHER_CTX_free(ctx);
		plainText[plaintext_len] = '\0';
		return plaintext_len;
	}
	
	HLString Encryptor::Encrypt(const HLString &plainText)
	{
		HLString cipher;
		uint32 cipherLength = 0;
		unsigned char cipherRaw[256];
		unsigned char *plainTextRaw = (unsigned char*)*plainText;
		uint32 plainTextLength = (uint32)plainText.Length();

		cipherLength = Encrypt(plainTextRaw, plainTextLength, cipherRaw);
		cipher = HLString((const char*)cipherRaw, cipherLength);

		return cipher;
	}
	
	HLString Encryptor::Decrypt(const HLString &cipherText)
	{
		HLString plainText;
		uint32 plainTextLength = 0;
		unsigned char plainTextRaw[256];
		unsigned char *cipherRaw = (unsigned char*)*cipherText;
		uint32 cipherLength = (uint32)cipherText.Length();

		plainTextLength = Decrypt(cipherRaw, cipherLength, plainTextRaw);
		plainText = HLString((const char*) plainTextRaw, plainTextLength);

		return plainText;
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

