
#include<iostream>
#include"enDecryption.h"
#include"const.h"
using namespace std;

/*
 * Simon���ܣ��ִ�СΪ32
 * plainText������
 * keys����Կ
 */
void encryptionSimon32 ( unsigned int * plainText, unsigned int * keys ) {
	/*
	 * ������ʱ����
	 */
	unsigned int tempCipherHigher = 0x0;
	unsigned int tempCipherLower  = 0x0;


	/*
	 * ����SIMON_ROUNDS�ּ���
	 */
	for ( int i = 0; i < SIMON_ROUNDS; i++ ) {
		/*
		 * ���ܺ�ĵ�32λ�����ĵĸ�32λ
		 */
		tempCipherLower  = plainText[0];
		tempCipherHigher = plainText[1] ^ keys[i] ^ 
						( ((plainText[0]<<1)|(plainText[0]>>(SIMON_WORD_SIZE-1))) & ((plainText[0]<<8)|(plainText[0]>>(SIMON_WORD_SIZE-8))) ) ^
						((plainText[0]<<2)|(plainText[0]>>(SIMON_WORD_SIZE-2)));
		/*
		 * ���½����ܵĽ�����Ƶ�plainText��
		 */
		plainText[0]     = tempCipherHigher;
		plainText[1]     = tempCipherLower;
	}
	
}

/*
 * Simon���ܣ��ִ�СΪ32
 * cipherText������
 * keys����Կ
 */
void decryptionSimon32 ( unsigned int * cipherText, unsigned int * keys ) {
	/*
	 * ������ʱ����
	 */
	unsigned int tempPlainHigher = 0x0;
	unsigned int tempPlainLower  = 0x0;     

	int i;
	/*
	 * ���������˳���෴�������һ�ֿ�ʼ
	 */
	for ( i = SIMON_ROUNDS-1; i >= 0; i-- ) {
		tempPlainHigher = cipherText[1];
		tempPlainLower  = cipherText[0] ^ keys[i] ^ 
						( ((cipherText[1]<<1)|(cipherText[1]>>(SIMON_WORD_SIZE-1))) & ((cipherText[1]<<8)|(cipherText[1]>>(SIMON_WORD_SIZE-8))) ) ^
						((cipherText[1]<<2)|(cipherText[1]>>(SIMON_WORD_SIZE-2)));
		cipherText[0]   = tempPlainHigher;
		cipherText[1]   = tempPlainLower;
		
	}

}

/*
 * Simon���ܣ��ִ�СΪ32
 * plainText������
 * keys����Կ
 */
void encryptionSimon64 ( unsigned long long * plainText, unsigned long long * keys ) {
	/*
	 * ������ʱ����
	 */
	unsigned long long tempCipherHigher = 0x0;
	unsigned long long tempCipherLower  = 0x0;


	/*
	 * ����SIMON_ROUNDS�ּ���
	 */
	for ( int i = 0; i < SIMON_ROUNDS; i++ ) {
		/*
		 * ���ܺ�ĵ�32λ�����ĵĸ�32λ
		 */
		tempCipherLower  = plainText[0];
		tempCipherHigher = plainText[1] ^ keys[i] ^ 
						( ((plainText[0]<<1)|(plainText[0]>>(SIMON_WORD_SIZE-1))) & ((plainText[0]<<8)|(plainText[0]>>(SIMON_WORD_SIZE-8))) ) ^
						((plainText[0]<<2)|(plainText[0]>>(SIMON_WORD_SIZE-2)));
		/*
		 * ���½����ܵĽ�����Ƶ�plainText��
		 */
		plainText[0]     = tempCipherHigher;
		plainText[1]     = tempCipherLower;
	}
}

/*
 * Simon���ܣ��ִ�СΪ64
 * cipherText������
 * keys����Կ
 */
void decryptionSimon64 ( unsigned long long * cipherText, unsigned long long * keys ) {
	/*
	 * ������ʱ����
	 */
	unsigned long long tempPlainHigher = 0x0;
	unsigned long long tempPlainLower  = 0x0;     

	int i;
	/*
	 * ���������˳���෴�������һ�ֿ�ʼ
	 */
	for ( i = SIMON_ROUNDS-1; i >= 0; i-- ) {
		tempPlainHigher = cipherText[1];
		tempPlainLower  = cipherText[0] ^ keys[i] ^ 
						( ((cipherText[1]<<1)|(cipherText[1]>>(SIMON_WORD_SIZE-1))) & ((cipherText[1]<<8)|(cipherText[1]>>(SIMON_WORD_SIZE-8))) ) ^
						((cipherText[1]<<2)|(cipherText[1]>>(SIMON_WORD_SIZE-2)));
		cipherText[0]   = tempPlainHigher;
		cipherText[1]   = tempPlainLower;
		
	}
}

/*
 * Speck���ܣ���32λ
 * plainText������
 * keys����Կ
 */
void encryptionSpeck32 ( unsigned int * plainText, unsigned int * keys ) {
	/*
	 * ����SPECK_ROUNDS�ּ���
	 */
	unsigned int tempCipherHigher = 0x0; // ������ʱ����
	unsigned int tempCipherLower  = 0x0;
	unsigned int additionModulo   = 0x0; // ��ʱ����ģ2^n�ӷ�������

	for ( int i = 0; i < SPECK_ROUNDS; i++ ) {
		additionModulo = ((plainText[0]>>SPECK_ROT_A)|(plainText[0]<<(SPECK_WORD_SIZE-SPECK_ROT_A))) + plainText[1];
		/*
		 * ǰSPECK_WORD_SIZEλ
		 */
		tempCipherHigher = additionModulo ^ keys[i];
		/*
		 * ��SPECK_WORD_SIZEλ
		 */
		tempCipherLower  = ( (plainText[1]<<SPECK_ROT_B)|(plainText[1]>>(SPECK_WORD_SIZE-SPECK_ROT_B)) ) ^ additionModulo ^ keys[i];
		/*
		 * ���½����ܵĽ�����Ƶ�plainText��
		 */
		plainText[0] = tempCipherHigher;
		plainText[1] = tempCipherLower;
		//cout<<"����"<<(i+1)<<":"<<plainText[0]<<","<<plainText[1]<<endl;
	}

}

/*
 * Speck���ܣ���32λ
 * cipherText������
 * keys����Կ
 */
void decryptionSpeck32 ( unsigned int * cipherText, unsigned int * keys ) {
	/*
	 * ������ʱ����
	 */
	unsigned int tempPlainHigher = 0x0;
	unsigned int tempPlainLower  = 0x0;       

	/*
	 * ����SPECK_ROUNDS�ֽ���
	 */
	unsigned int temp  = 0x0;
	for ( int i = SPECK_ROUNDS-1; i >= 0; i-- ) {
		//cout<<"����"<<(i+1)<<":"<<cipherText[0]<<","<<cipherText[1]<<endl;

		/*
		 * tempPlainLower = (cipherText[0]>>SPECK_ROT_B)|(cipherText[0]<<(SPECK_WORD_SIZE-SPECK_ROT_B)) ^
		 *		(cipherText[1]>>SPECK_ROT_B)|(cipherText[1]<<(SPECK_WORD_SIZE-SPECK_ROT_B));
		 * ��������������ȼ����ڻ�������ִ������½������
		 */
		tempPlainLower = ( (cipherText[0]>>SPECK_ROT_B)|(cipherText[0]<<(SPECK_WORD_SIZE-SPECK_ROT_B)) ) ^
				( (cipherText[1]>>SPECK_ROT_B)|(cipherText[1]<<(SPECK_WORD_SIZE-SPECK_ROT_B)) );
		/*
		 * �����ǰSPECK_WORD_SIZEλ
		 * ���Է��֣����޷��ŵ�����£�С�������������Ϊ�����൱�ڽ���һλ��ֻ��û�б�ʾ����
		 * 
		 * unsigned int temp1 = 0x40000000;
		 * unsigned int temp2 = 0x80000000;
		 * cout<<temp1-temp2<<endl;
		 * cout<<temp1<<endl;
		 * cout<<temp2<<endl;
		 */
		temp = (cipherText[0] ^ keys[i]) - tempPlainLower;
		tempPlainHigher = (temp<<SPECK_ROT_A)|(temp>>(SPECK_WORD_SIZE-SPECK_ROT_A));

		/*
		 * ���½����ܵĽ�����Ƶ�plainText��
		 */
		cipherText[0] = tempPlainHigher;
		cipherText[1] = tempPlainLower;
	}

}

/*
 * Speck����
 * plainText������
 * keys����Կ
 */
void encryptionSpeck64 ( unsigned long long * plainText, unsigned long long  * keys ) {
	/*
	 * ����SPECK_ROUNDS�ּ���
	 */
	unsigned long long tempCipherHigher = 0x0; // ������ʱ����
	unsigned long long tempCipherLower  = 0x0;
	unsigned long long additionModulo   = 0x0; // ��ʱ����ģ2^n�ӷ�������

	for ( int i = 0; i < SPECK_ROUNDS; i++ ) {
		additionModulo = ((plainText[0]>>SPECK_ROT_A)|(plainText[0]<<(SPECK_WORD_SIZE-SPECK_ROT_A))) + plainText[1];
		/*
		 * ǰSPECK_WORD_SIZEλ
		 */
		tempCipherHigher = additionModulo ^ keys[i];
		/*
		 * ��SPECK_WORD_SIZEλ
		 */
		tempCipherLower  = ( (plainText[1]<<SPECK_ROT_B)|(plainText[1]>>(SPECK_WORD_SIZE-SPECK_ROT_B)) ) ^ additionModulo ^ keys[i];
		/*
		 * ���½����ܵĽ�����Ƶ�plainText��
		 */
		plainText[0] = tempCipherHigher;
		plainText[1] = tempCipherLower;
		//cout<<"����"<<(i+1)<<":"<<plainText[0]<<","<<plainText[1]<<endl;
	}
}

/*
 * Speck����
 * cipherText������
 * keys����Կ
 */
void decryptionSpeck64 ( unsigned long long * cipherText, unsigned long long * keys ) {
	/*
	 * ������ʱ����
	 */
	unsigned long long tempPlainHigher = 0x0;
	unsigned long long tempPlainLower  = 0x0;       

	/*
	 * ����SPECK_ROUNDS�ֽ���
	 */
	unsigned long long temp  = 0x0;
	for ( int i = SPECK_ROUNDS-1; i >= 0; i-- ) {
		tempPlainLower = ( (cipherText[0]>>SPECK_ROT_B)|(cipherText[0]<<(SPECK_WORD_SIZE-SPECK_ROT_B)) ) ^
				( (cipherText[1]>>SPECK_ROT_B)|(cipherText[1]<<(SPECK_WORD_SIZE-SPECK_ROT_B)) );
		temp = (cipherText[0] ^ keys[i]) - tempPlainLower;
		tempPlainHigher = (temp<<SPECK_ROT_A)|(temp>>(SPECK_WORD_SIZE-SPECK_ROT_A));

		/*
		 * ���½����ܵĽ�����Ƶ�plainText��
		 */
		cipherText[0] = tempPlainHigher;
		cipherText[1] = tempPlainLower;
	}
}