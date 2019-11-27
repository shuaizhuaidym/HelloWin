#pragma once
class DESEncoder
{
public:
	DESEncoder(void);
	~DESEncoder(void);

	bool MYEncryptFile(LPTSTR szSource, LPTSTR szDestination, LPTSTR szPassword);
	void MyHandleError(LPTSTR psz, int nErrorNumber);
};

