#ifndef _DES_H_
#define _DES_H_
 
#include <string>
 
#define BITS_PER_CHAR 8
#define KEY_SZIE 8
#define SUBKEY_NUM 16
#define SUBKEY_LENGHT 48
#define EXPAND_SIZE 48
#define PC_2_SIZE 48
#define PC_1_SIZE 56
#define BIT_STR_SIZE 64

class DESEnc
{
public:
	DESEnc(void);
	~DESEnc(void);

	std::string Encrypt(const std::string& plain, const std::string& key);
	std::string Decrypt(const std::string& cipher, const std::string& key);

private:
	//����16������Կ
	bool CreateSubKey(const std::string& key, char subKey[SUBKEY_NUM][SUBKEY_LENGHT]);
 
	//����8�ֽ����ݿ�
	bool EncryptBlock(std::string& block, char subKey[SUBKEY_NUM][SUBKEY_LENGHT]);
	//����8�ֽ����ݿ�
	bool DecryptBlock(std::string& block, char subKey[SUBKEY_NUM][SUBKEY_LENGHT]);	private:
	//----------------------------------ת������-----------------------------------------------
	bool PC1_Transform(const std::string& bitStr, std::string& PC1BitStr);
	
	bool PC2_Transform(const std::string& PC1BitStr, char subKey[SUBKEY_LENGHT]);
 
	bool IP_Transform(std::string& bitStr);
 
	bool Expand_Transform(const std::string& halfBitStr, std::string& eBitStr);
 
	bool SBox_Transform(const std::string& eBitStr, std::string& halfBitStr);
 
	bool Permute_Transform(std::string& halfBitStr);
 
	bool IP_1_Transform(std::string& bitStr);
 
private:
	//------------------------------------��������------------------------------------------------
	bool Char8ToBit64(const std::string& str, std::string& bitStr);
	bool Bit64ToChar8(const std::string& bitStr, std::string& str);
 
	bool XOR(std::string& strFirst, std::string& strSecond, size_t num); 
 
	bool LeftCycle(std::string& str, size_t beginSection, size_t endSection, size_t step);
 
private:
	//------------------------------------������Կ��Ҫ�ı�----------------------------------------
	//�û�ѡ���1
	//ѹ����λȥ��ÿ���ֽڵĵ�8λ��������żУ�죬�����ϵ�8λ�ɺ���
	static char PC1_Table[PC_1_SIZE];
	//��ѭ�������
	static char Move_Table[SUBKEY_NUM];
	//�û�ѡ���2
	//ѡ�����е�ĳЩλ������ٵ�48λ
	static char PC2_Table[PC_2_SIZE];
 
 
	//----------------------------------���ܻ������Ҫ�ı�--------------------------------------------
	//��ʼ�û���
	//���е���ֵ��ʾ����Ϊ���û������λ��
	static char IP_Table[BIT_STR_SIZE];
	//��չ��
	//ͨ���ظ�ĳЩλ��32λ���Ұ벿�ְ��ձ���չ��48λ
	static char Expand_Table[EXPAND_SIZE];
	//�����û���
	static char Permute_Table[BIT_STR_SIZE / 2];
	//���û���
	static char IP_1_Table[BIT_STR_SIZE];
	//�û���
	static char SBox_Table[KEY_SZIE][4][16];
};
 
#endif