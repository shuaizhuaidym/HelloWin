#pragma once

#define KEYLENGTH  0x00380000
#define ENCRYPT_ALGORITHM CALG_DES 
#define ENCRYPT_BLOCK_SIZE 64 

class DESDecoder
{
public:
	DESDecoder(void);
	~DESDecoder(void);

	bool MyDecryptFile(LPTSTR szSource, LPTSTR szDestination, LPTSTR szPassword);

	void MyHandleError(LPTSTR psz, int nErrorNumber);

};

