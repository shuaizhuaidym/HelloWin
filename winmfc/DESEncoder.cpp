#include "StdAfx.h"
#include "DESEncoder.h"

#include <tchar.h>
#include <stdio.h>
#include <windows.h>
#include <wincrypt.h>
#include <conio.h>

// Link with the Advapi32.lib file.
#pragma comment (lib, "advapi32")

#define KEYLENGTH  0x00800000
#define ENCRYPT_ALGORITHM CALG_DES 
#define ENCRYPT_BLOCK_SIZE 8 

/**
 * Key length: 56 bits. Default mode: Cipher block chaining.
 * Block size: 64 bits.
 * No salt allowed.
 */

DESEncoder::DESEncoder(void){}

DESEncoder::~DESEncoder(void){}

//-------------------------------------------------------------------
// Code for the function EncryptFile called by main.
//-------------------------------------------------------------------
// @parameter pszSource, the name of the input, a plaintext file.
// @parameter pszDestination, the name of the output, an encrypted file to be created.
// @pszPassword either NULL if a password is not to be used or the string that is the password.
bool DESEncoder::MYEncryptFile(LPTSTR pszSourceFile, LPTSTR pszDestinationFile, LPTSTR pszPassword)
{ 
    //---------------------------------------------------------------
    // Declare and initialize local variables.
    bool fReturn = false;
    HANDLE hSourceFile = INVALID_HANDLE_VALUE;
    HANDLE hDestinationFile = INVALID_HANDLE_VALUE; 

    HCRYPTPROV hCryptProv = NULL; 
    HCRYPTKEY hKey = NULL; 
    HCRYPTKEY hXchgKey = NULL; 
    HCRYPTHASH hHash = NULL; 

    PBYTE pbKeyBlob = NULL; 
    DWORD dwKeyBlobLen; 

    PBYTE pbBuffer = NULL; 
    DWORD dwBlockLen; 
    DWORD dwBufferLen; 
    DWORD dwCount; 
     
    //---------------------------------------------------------------
    // Open the source file. 
    hSourceFile = CreateFile(pszSourceFile, FILE_READ_DATA, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if(INVALID_HANDLE_VALUE != hSourceFile) 
	{
        _tprintf(TEXT("The source plaintext file, %s, is open. \n"), pszSourceFile);
    } 
	else 
	{ 
        MyHandleError(TEXT("Error opening source plaintext file!\n"), GetLastError());
        goto Exit_EncryptFile;
    } 

    //---------------------------------------------------------------
    // Open the destination file. 
    hDestinationFile = CreateFile(pszDestinationFile, FILE_WRITE_DATA, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if(INVALID_HANDLE_VALUE != hDestinationFile) 
	{
         _tprintf(TEXT("The destination file, %s, is open. \n"), pszDestinationFile);
    }
	else
	{
        MyHandleError(TEXT("Error opening destination file!\n"), GetLastError()); 
        goto Exit_EncryptFile;
    }

    //---------------------------------------------------------------
    // Get the handle to the default provider. 
    if(CryptAcquireContext(&hCryptProv, NULL, MS_ENHANCED_PROV, PROV_RSA_FULL, 0)) 
	{
        _tprintf(TEXT("A cryptographic provider has been acquired. \n"));
    } 
	else 
	{
        MyHandleError(TEXT("Error during CryptAcquireContext!\n"), GetLastError());
        goto Exit_EncryptFile;
    }

    //---------------------------------------------------------------
    // Create the session key.
    if(!pszPassword || !pszPassword[0]) 
	{ 
        //-----------------------------------------------------------
        // No password was passed.
        // Encrypt the file with a random session key, and write the key to a file. 
        //-----------------------------------------------------------
        // Create a random session key. 
        if(CryptGenKey(hCryptProv, ENCRYPT_ALGORITHM, KEYLENGTH | CRYPT_EXPORTABLE, &hKey))
        {
            _tprintf(TEXT("A session key has been created. \n"));
        } 
        else
        {
            MyHandleError(TEXT("Error during CryptGenKey. \n"), 
                GetLastError()); 
            goto Exit_EncryptFile;
        }
        //-----------------------------------------------------------
        // Get the handle to the exchange public key. 
        if(CryptGetUserKey(hCryptProv, AT_KEYEXCHANGE, &hXchgKey))
        {
            _tprintf(TEXT("The user public key has been retrieved. \n"));
        }
        else
        { 
            if(NTE_NO_KEY == GetLastError())
            {
                // No exchange key exists. Try to create one.
                if(!CryptGenKey(hCryptProv, AT_KEYEXCHANGE, CRYPT_EXPORTABLE, &hXchgKey))
                {
                    MyHandleError(TEXT("Could not create a user public key.\n"), GetLastError()); 
                    goto Exit_EncryptFile;
                }
            }
            else
            {
                MyHandleError(
                    TEXT("User public key is not available and may ")
                        TEXT("not exist.\n"), 
                    GetLastError()); 
                goto Exit_EncryptFile;
            }
        }

        //-----------------------------------------------------------
        // Determine size of the key BLOB, and allocate memory. 
        if(CryptExportKey(hKey, hXchgKey, SIMPLEBLOB, 0, NULL, &dwKeyBlobLen))
        {
            _tprintf(TEXT("The key BLOB is %d bytes long. \n"), dwKeyBlobLen);
        }
        else
        {  
            MyHandleError(TEXT("Error computing BLOB length! \n"), GetLastError());
            goto Exit_EncryptFile;
        }
        if(pbKeyBlob = (BYTE *)malloc(dwKeyBlobLen))
        { 
            _tprintf(
                TEXT("Memory is allocated for the key BLOB. \n"));
        }
        else
        { 
            MyHandleError(TEXT("Out of memory. \n"), E_OUTOFMEMORY); 
            goto Exit_EncryptFile;
        }
        //-----------------------------------------------------------
        // Encrypt and export the session key into a simple key BLOB. 
        if(CryptExportKey(hKey, hXchgKey, SIMPLEBLOB, 0, pbKeyBlob, &dwKeyBlobLen))
        {
            _tprintf(TEXT("The key has been exported. \n"));
        } 
        else
        {
            MyHandleError(TEXT("Error during CryptExportKey!\n"), GetLastError());
            goto Exit_EncryptFile;
        } 
         
        //-----------------------------------------------------------
        // Release the key exchange key handle. 
        if(hXchgKey)
        {
            if(!(CryptDestroyKey(hXchgKey)))
            {
                MyHandleError(TEXT("Error during CryptDestroyKey.\n"), GetLastError()); 
                goto Exit_EncryptFile;
            }
            hXchgKey = 0;
        }
        //-----------------------------------------------------------
        // Write the size of the key BLOB to the destination file. 
        if(!WriteFile(hDestinationFile, &dwKeyBlobLen, sizeof(DWORD), &dwCount, NULL))
        { 
            MyHandleError(TEXT("Error writing header.\n"), GetLastError());
            goto Exit_EncryptFile;
        }
        else
        {
            _tprintf(TEXT("A file header has been written. \n"));
        }

        //-----------------------------------------------------------
        // Write the key BLOB to the destination file. 
        if(!WriteFile(hDestinationFile, pbKeyBlob, dwKeyBlobLen, &dwCount, NULL))
        { 
            MyHandleError(TEXT("Error writing header.\n"), GetLastError());
            goto Exit_EncryptFile;
        }
        else
        {
            _tprintf(TEXT("The key BLOB has been written to the "), TEXT("file. \n"));
        }
        // Free memory.
        free(pbKeyBlob);
    } 
    else 
    { 

        //-----------------------------------------------------------
        // The file will be encrypted with a session key derived 
        // from a password.
        // The session key will be recreated when the file is 
        // decrypted only if the password used to create the key is 
        // available. 

        //-----------------------------------------------------------
        // Create a hash object. 
        if(CryptCreateHash(hCryptProv, CALG_MD5, 0, 0, &hHash))
        {
            _tprintf(TEXT("A hash object has been created. \n"));
        }
        else
        { 
            MyHandleError(TEXT("Error during CryptCreateHash!\n"), GetLastError());
            goto Exit_EncryptFile;
        }  
        //-----------------------------------------------------------
        // Hash the password. 
        if(CryptHashData(hHash, (BYTE *)pszPassword, lstrlen(pszPassword), 0)) 
		{
            _tprintf(TEXT("The password has been added to the hash. \n"));
        } 
		else 
		{
            MyHandleError(TEXT("Error during CryptHashData. \n"), GetLastError()); 
            goto Exit_EncryptFile;
        }
        //-----------------------------------------------------------
        // Derive a session key from the hash object. 
        if(CryptDeriveKey(hCryptProv, ENCRYPT_ALGORITHM, hHash, KEYLENGTH, &hKey)) 
		{
            _tprintf(TEXT("An encryption key is derived from the "), TEXT("password hash. \n")); 
        } 
		else 
		{
            MyHandleError(TEXT("Error during CryptDeriveKey!\n"), GetLastError()); 
            goto Exit_EncryptFile;
        }
    }
    //---------------------------------------------------------------
    // The session key is now ready. If it is not a key derived from 
    // a  password, the session key encrypted with the private key 
    // has been written to the destination file.
     
    //---------------------------------------------------------------
    // Determine the number of bytes to encrypt at a time. 
    // This must be a multiple of ENCRYPT_BLOCK_SIZE.
    // ENCRYPT_BLOCK_SIZE is set by a #define statement.
    dwBlockLen = 1000 - 1000 % ENCRYPT_BLOCK_SIZE; 

    //---------------------------------------------------------------
    // Determine the block size. If a block cipher is used, 
    // it must have room for an extra block. 
    if(ENCRYPT_BLOCK_SIZE > 1) 
	{
        dwBufferLen = dwBlockLen + ENCRYPT_BLOCK_SIZE; 
    } 
	else 
	{
        dwBufferLen = dwBlockLen; 
    }
        
    //---------------------------------------------------------------
    // Allocate memory. 
    if(pbBuffer = (BYTE *)malloc(dwBufferLen)) 
	{
        _tprintf(TEXT("Memory has been allocated for the buffer. \n"));
    } 
	else 
	{ 
        MyHandleError(TEXT("Out of memory. \n"), E_OUTOFMEMORY); 
        goto Exit_EncryptFile;
    }

    //---------------------------------------------------------------
    // In a do loop, encrypt the source file, 
    // and write to the source file. 
    bool fEOF = FALSE;
    do{ 
        //-----------------------------------------------------------
        // Read up to dwBlockLen bytes from the source file. 
        if(!ReadFile(hSourceFile, pbBuffer, dwBlockLen, &dwCount, NULL))
		{
            MyHandleError(TEXT("Error reading plaintext!\n"), GetLastError());
            goto Exit_EncryptFile;
        }
        if(dwCount < dwBlockLen)
		{
            fEOF = TRUE;
        }

        //-----------------------------------------------------------
        // Encrypt data. 
        if(!CryptEncrypt(hKey, NULL, fEOF, 0, pbBuffer, &dwCount, dwBufferLen))
		{ 
            MyHandleError(TEXT("Error during CryptEncrypt. \n"),GetLastError()); 
            goto Exit_EncryptFile;
        } 

        //-----------------------------------------------------------
        // Write the encrypted data to the destination file. 
        if(!WriteFile(hDestinationFile, pbBuffer, dwCount, &dwCount, NULL))
		{ 
            MyHandleError(TEXT("Error writing ciphertext.\n"), GetLastError());
            goto Exit_EncryptFile;
        }
        //-----------------------------------------------------------
        // End the do loop when the last block of the source file 
        // has been read, encrypted, and written to the destination 
        // file.
    } while(!fEOF);

    fReturn = true;

Exit_EncryptFile:
    //---------------------------------------------------------------
    // Close files.
    if(hSourceFile){
        CloseHandle(hSourceFile);
    }

    if(hDestinationFile){
        CloseHandle(hDestinationFile);
    }

    //---------------------------------------------------------------
    // Free memory. 
    if(pbBuffer){
        free(pbBuffer); 
    }
     
    //-----------------------------------------------------------
    // Release the hash object. 
    if(hHash){
        if(!(CryptDestroyHash(hHash))){
            MyHandleError(TEXT("Error during CryptDestroyHash.\n"), GetLastError()); 
        }

        hHash = NULL;
    }

    //---------------------------------------------------------------
    // Release the session key. 
    if(hKey){
        if(!(CryptDestroyKey(hKey))){
            MyHandleError(
                TEXT("Error during CryptDestroyKey!\n"), 
                GetLastError());
        }
    }

    //---------------------------------------------------------------
    // Release the provider handle. 
    if(hCryptProv){
        if(!(CryptReleaseContext(hCryptProv, 0))){
            MyHandleError(TEXT("Error during CryptReleaseContext!\n"), GetLastError());
        }
    }
    
    return fReturn; 
} // End Encryptfile.


//-------------------------------------------------------------------
//  This example uses the function MyHandleError, a simple error
//  handling function, to print an error message to the  
//  standard error (stderr) file and exit the program. 
//  For most applications, replace this function with one 
//  that does more extensive error reporting.

void DESEncoder::MyHandleError(LPTSTR psz, int nErrorNumber){
    _ftprintf(stderr, TEXT("An error occurred in the program. \n"));
    _ftprintf(stderr, TEXT("%s\n"), psz);
    _ftprintf(stderr, TEXT("Error number %x.\n"), nErrorNumber);
}