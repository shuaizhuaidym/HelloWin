#pragma once
class DESDecoder
{
public:
	DESDecoder(void);
	~DESDecoder(void);

	bool MyDecryptFile(LPTSTR szSource, LPTSTR szDestination, LPTSTR szPassword);

	void MyHandleError(LPTSTR psz, int nErrorNumber);

};

