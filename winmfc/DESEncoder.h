#pragma once
//#define KEYLENGTH  0x00800000
#define KEYLENGTH  0x00380000

//CALG_DES	0x00006601	DES encryption algorithm.

//Key length: 56 bits.Default mode: Cipher block chaining.
//Block size: 64 bits.
//No salt allowed.
#define ENCRYPT_ALGORITHM CALG_DES 
//#define ENCRYPT_BLOCK_SIZE 8
#define ENCRYPT_BLOCK_SIZE 64
class DESEncoder
{
public:
	DESEncoder(void);
	~DESEncoder(void);
	
	bool MYEncryptFile(LPTSTR szSource, LPTSTR szDestination, LPTSTR szPassword);
	void MyHandleError(LPTSTR psz, int nErrorNumber);
};

