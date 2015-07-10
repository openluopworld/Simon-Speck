#include<iostream>
#include<time.h>
#include"keys.h"
#include"enDecryption.h"
using namespace std;

int SIMON_BLOCK_SIZE;       // Simon���ĳ���
int SIMON_KEY_SIZE;         // Simon��Կ����
int SIMON_WORD_SIZE;        // Simon�ִ�С
int SIMON_KEY_WORDS;        // Simon��Կ�ֵĸ���
int SIMON_SEQUENCE_NUMBER;  // Simon��Ӧ�汾Z������
int SIMON_ROUNDS;           // Simon�ӽ�������

int SPECK_BLOCK_SIZE;       // Speck���С
int SPECK_KEY_SIZE;         // Speck��Կ��С
int SPECK_WORD_SIZE;        // Speck�ִ�С
int SPECK_KEY_WORDS;        // Speck��Կ������
int SPECK_ROT_A;            // Speck����λ����a
int SPECK_ROT_B;            // Speck����λ����b
int SPECK_ROUNDS;           // Speck�ӽ�������

void simon();
void speck();
int simonChoose ();
int speckChoose ();

/*
 * Simon��Speck�㷨
 * Version:2.0
 * Time:2015-1-10
 *
 * �ִ�СΪ32λʱ����unsigned int���棬����Ҫ���м��㣬���Ա����ǷǸ�����Ϊ64λʱ��unsigned long long����
 *
 */
int main () {
	//unsigned int temp1 = 0x00000000;
	//unsigned int temp2 = 0xffffffff;
	//cout<<temp1-temp2<<endl;
	//cout<<temp1<<endl;
	//cout<<temp2<<endl;

	//cout<<sizeof(unsigned long long)<<endl;

	//cout<<7/3<<endl;

	int flag = 0;
	while( true ) {
		cout<<"*******************************************************************************"<<endl;
		cout<<"                                  1-->SIMON Algorithm                          "<<endl;
		cout<<"                                  2-->SPECK Algorithm                          "<<endl;
		cout<<"                                  3-->EXIT                                     "<<endl;
		cout<<"*******************************************************************************"<<endl<<endl;
		cin>>flag;
		if ( flag == 1 ) {
			simon();
		} else if ( flag == 2 ) {
			speck();
		} else if ( flag == 3 ) {
			break;
		}
	}
	return 0;

}

/*
 * Simon�㷨
 * 1��64/96
 * 2��64/128
 * 3��128/128
 * 
 */
void simon () {
	int flag = simonChoose();
	if ( flag == 1) {
		SIMON_BLOCK_SIZE      = 64;
		SIMON_KEY_SIZE        = 96;
		SIMON_WORD_SIZE       = SIMON_BLOCK_SIZE/2;
		SIMON_KEY_WORDS       = SIMON_KEY_SIZE/SIMON_WORD_SIZE;
		SIMON_SEQUENCE_NUMBER = 2;
		SIMON_ROUNDS          = 42;
	} else if ( flag == 2) {
		SIMON_BLOCK_SIZE      = 64;
		SIMON_KEY_SIZE        = 128;
		SIMON_WORD_SIZE       = SIMON_BLOCK_SIZE/2;
		SIMON_KEY_WORDS       = SIMON_KEY_SIZE/SIMON_WORD_SIZE;
		SIMON_SEQUENCE_NUMBER = 3;
		SIMON_ROUNDS          = 44;
	} else if ( flag == 3) {
		SIMON_BLOCK_SIZE      = 128;
		SIMON_KEY_SIZE        = 128;
		SIMON_WORD_SIZE       = SIMON_BLOCK_SIZE/2;
		SIMON_KEY_WORDS       = SIMON_KEY_SIZE/SIMON_WORD_SIZE;
		SIMON_SEQUENCE_NUMBER = 2;
		SIMON_ROUNDS          = 68;
	}

	/*
	 * SIMON_WORD_SIZEΪ32λʱ����unsigned int��ʾ����ʱ����һ��64λ���ֱ�������unsigned int��ʾ��32λ�͵�32λ
	 *
	 * SIMON_WORD_SIZEΪ64λʱ����unsigned long long��ʾ
	 */
	if ( SIMON_WORD_SIZE == 32 ) {
		int i, j;
		unsigned int * plainText       = new unsigned int[2]; // ����
		plainText[0] = plainText[1]    = 0x0;
		unsigned int * inputKeys       = new unsigned int[SIMON_KEY_WORDS]; // ��ʼ��Կ��һ����SIMON_KEY_WORDS��
		for ( i = 0; i < SIMON_KEY_WORDS; i++ ) {
			inputKeys[i]               = 0x0;
		}
		unsigned int * keys            = new unsigned int[SIMON_ROUNDS]; // ���յ���Կ��һ��SIMON_ROUNDS��

		unsigned int * plainTextByte   = new unsigned int[SIMON_BLOCK_SIZE/8]; // Ϊ�˼����룬ÿ���������ֽڱ�ʾ��[0,255]
		for ( i = 0; i < SIMON_BLOCK_SIZE/8; i++ ) {
			plainTextByte[i]           = 0x0;
		}
		unsigned int * inputKeysByte   = new unsigned int[SIMON_KEY_SIZE/8];  // ���ֽڱ�ʾ����Կ�����ڱ����������Կ
		for ( i = 0; i < SIMON_KEY_SIZE/8; i++ ) {
			inputKeysByte[i]           = 0x0;
		}
		unsigned int * cipherTextByte  = new unsigned int[SIMON_BLOCK_SIZE/8];// ���ֽڱ�ʾ������
		for ( i = 0; i < SIMON_BLOCK_SIZE/8; i++ ) {
			cipherTextByte[i]          = 0x0;
		}

		/*
		 * �õ��������ĺͳ�ʼ��Կ
		 */
		cout<<"Please input the plainText of "<<SIMON_BLOCK_SIZE/8<<" bytes:"<<endl;
		for ( i = 0; i < SIMON_BLOCK_SIZE/8; i++ ) {
			cin>>plainTextByte[i];
		}
		cout<<"Please input the key of "<<SIMON_KEY_SIZE/8<<" bytes:"<<endl;
		for ( i = 0; i < SIMON_KEY_SIZE/8; i++ ) {
			cin>>inputKeysByte[i];
		}

		/*
		 * ��ʱSIMON_BLOCK_SIZEΪ64��һ��8���ֽڣ����ĸ��ֽ����plainText[0]�����ĸ��ֽ����plainText[1]
		 */
		for ( i = 0; i < 4; i++ ) {
			plainText[0]  = plainText[0] * 256 + plainTextByte[i];
		}
		for ( i = 4; i < SIMON_BLOCK_SIZE/8; i++ ) { 
			plainText[1]  = plainText[1] * 256 + plainTextByte[i];
		}
		//cout<<endl<<endl<<plainText[1]/(256*256*256)<<","<<plainText[1]/(256*256)%256<<","<<plainText[1]%(256*256)/256<<","<<plainText[1]%256<<endl;
		/*
		 * ��Կ�Ǵ�������ʼ���㣬��index��С�ı�ʾ����k����
		 * ����SIMON_KEY_WORDS=3����inputKeysByte[0...3]=k2;inputKeysByte[4...7]=k1;inputKeysByte[8...11]=k0
		 */
		for ( i = 0; i < SIMON_KEY_SIZE/8; i++ ) {
			inputKeys[SIMON_KEY_WORDS-1-i/4] = inputKeys[SIMON_KEY_WORDS-1-i/4] * 256 + inputKeysByte[i];
		}

		time_t start, end;
		int recordCount = 0;
		for ( recordCount = 4; recordCount <= 1024; recordCount*=2 ) {
			start = clock();
			for ( j = 0; j < 10000*recordCount; j++ ) {
				/*
				 * ����ÿ�ֵ���Կ
				 */
				setSimonKeys32 ( inputKeys, keys );
			}
			end = clock();
			/*
			 * = (10000*recordCount*SIMON_KEY_SIZE)/(end-start) ����ÿ����
			 * = (1000*10000*recordCount*SIMON_KEY_SIZE)/(end-start) ����ÿ��
			 * = (1000*10000*recordCount*SIMON_KEY_SIZE)/��1024*1024*(end-start)) �ױ���ÿ��
			 * = (10.0*recordCount*SIMON_KEY_SIZE)/��.1024*1.024*(end-start)) �ױ���ÿ��
			 */
			printf("Keys extend: %d, speed: %0.3f Mbps.\n", 10000*recordCount, 10.0*recordCount*SIMON_KEY_SIZE/(1.024*1.024*(end-start )));
		}

		for ( int i = 0; i < SIMON_ROUNDS; i++ ) {
			cout<<"��Կ��"<<keys[i]%256<<","<<(keys[i]/256%256)<<","<<(keys[i]/(256*256)%256)<<","<<keys[i]/(256*256*256)<<endl;
		}

		/*
		 * �������ĸ�����ÿ�μ���ǰ���»ָ���������ġ���μ�����ƽ�����Լ�����Ϊ����ʱ��̫�̴����ļ������
		 */
		unsigned int tempPlainHigher   = plainText[0];
		unsigned int tempPlainLower    = plainText[1];
		//cout<<endl<<endl<<plainText[1]/(256*256*256)<<","<<plainText[1]/(256*256)%256<<","<<plainText[1]%(256*256)/256<<","<<plainText[1]%256<<endl;

		/*
		 * ��μ�¼�������ٶ��Ƿ��ȶ�
		 */
		for ( recordCount = 4; recordCount <= 1024; recordCount*=2 ) {
			/*
			 * ��ʼʱ��
			 */
			start = clock();
			/*
			 * ������ȡƽ��ֵ
			 */
			for ( j = 0; j < 10000*recordCount; j++ ) {
				plainText[0]  = tempPlainHigher;
				plainText[1]  = tempPlainLower;
				/*
				 * ����
				 */
				encryptionSimon32 ( plainText, keys );
			}
			/*
			 * ����ʱ��
			 */
			end = clock();
			/*
			 * = (10000*recordCount*SIMON_BLOCK_SIZE)/(end-start) ����ÿ����
			 * = (1000*10000*recordCount*SIMON_BLOCK_SIZE)/(end-start) ����ÿ��
			 * = (1000*10000*recordCount*SIMON_BLOCK_SIZE)/��1024*1024*(end-start)) �ױ���ÿ��
			 * = (10.0*recordCount*SIMON_BLOCK_SIZE)/��.1024*1.024*(end-start)) �ױ���ÿ��
			 */
			printf("Encryption counts: %d, speed: %0.3f Mbps.\n", 10000*recordCount, 10.0*recordCount*SIMON_BLOCK_SIZE/(1.024*1.024*(end-start )));
		}

		/*
		 * ���ܺ���������ֽ���ʽ���
		 */
		tempPlainHigher   = plainText[0];
		tempPlainLower    = plainText[1];
		for ( i = 3; i >= 0; i-- ) {
			cipherTextByte[i] = tempPlainHigher % 256;
			tempPlainHigher   /= 256;
		}

		/*
		 * ����for ( i = SIMON_BLOCK_SIZE/8; i >= 4; i-- ) {
		 * 
		 * ������еĽ������5λ���������ң��Ǵ���ģ�����Ϊ0�����������ͷ��ڴ�ʱ����ִ���
		 * ������Ϊ����Խ�磬�����������������һλ
		 */
		for ( i = SIMON_BLOCK_SIZE/8-1; i >= 4; i-- ) {
			cipherTextByte[i] = tempPlainLower % 256;
			tempPlainLower    /= 256;
		}
		cout<<"After encryption:"<<endl;
		for ( i = 0; i < SIMON_BLOCK_SIZE/8; i++ ) {
			cout<<cipherTextByte[i]<<"  ";
		}
		cout<<endl;

		/*
		 * ���ܣ���������ܺ�Ľ��
		 */
		decryptionSimon32 ( plainText, keys );
		for ( i = 3; i >= 0; i-- ) {
			cipherTextByte[i] = plainText[0] % 256;
			plainText[0]      /= 256;
		}
		for ( i = SIMON_BLOCK_SIZE/8-1; i >= 4; i-- ) {
			cipherTextByte[i] = plainText[1] % 256;
			plainText[1]      /= 256;
		}
		cout<<"After decryption:"<<endl;
		for ( i = 0; i < SIMON_BLOCK_SIZE/8; i++ ) {
			cout<<cipherTextByte[i]<<"  ";
		}
		cout<<endl;

		/*
		 * �ͷ��ڴ�
		 */
		delete cipherTextByte;
		delete inputKeysByte;
		delete plainTextByte;
		delete keys;
		delete inputKeys;
		delete plainText;

	} else if ( SIMON_WORD_SIZE == 64 ){

		/*unsigned long long temp = 0x8000000000000000;
		cout<<temp<<endl;*/
		int i, j;
		unsigned long long * plainText       = new unsigned long long[2];
		plainText[0] = plainText[1]    = 0x0;
		unsigned long long * inputKeys       = new unsigned long long[SIMON_KEY_WORDS]; // ��ʼ��Կ��һ����SIMON_KEY_WORDS��
		for ( i = 0; i < SIMON_KEY_WORDS; i++ ) {
			inputKeys[i]               = 0x0;
		}
		unsigned long long * keys            = new unsigned long long[SIMON_ROUNDS]; // ���յ���Կ��һ��SIMON_WORD_SIZE��
		unsigned long long * plainTextByte   = new unsigned long long[SIMON_BLOCK_SIZE/8]; // Ϊ�˼����룬ÿ���������ֽڱ�ʾ��[0,255]
		for ( i = 0; i < SIMON_BLOCK_SIZE/8; i++ ) {
			plainTextByte[i]           = 0x0;
		}
		unsigned long long * inputKeysByte   = new unsigned long long[SIMON_KEY_SIZE/8]; // ���ֽڱ�ʾ����Կ�����ڱ����������Կ
		for ( i = 0; i < SIMON_KEY_SIZE/8; i++ ) {
			inputKeysByte[i]           = 0x0;
		}
		unsigned long long * cipherTextByte  = new unsigned long long[SIMON_BLOCK_SIZE/8]; // ���ֽڱ�ʾ������
		for ( i = 0; i < SIMON_BLOCK_SIZE/8; i++ ) {
			cipherTextByte[i]          = 0x0;
		}

		/*
		 * �õ��������ĺͳ�ʼ��Կ
		 */
		cout<<"Please input the plainText of "<<SIMON_BLOCK_SIZE/8<<" bytes:"<<endl;
		for ( i = 0; i < SIMON_BLOCK_SIZE/8; i++ ) {
			cin>>plainTextByte[i];
		}
		cout<<"Please input the key of "<<SIMON_KEY_SIZE/8<<" bytes:"<<endl;
		for ( i = 0; i < SIMON_KEY_SIZE/8; i++ ) {
			cin>>inputKeysByte[i];
		}

		/*
		 * ��ʱSIMON_BLOCK_SIZEΪ128��һ��16���ֽ�
		 */
		for ( i = 0; i < 8; i++ ) {
			plainText[0]  = plainText[0] * 256 + plainTextByte[i];
		}
		for ( i = 8; i < SIMON_BLOCK_SIZE/8; i++ ) { 
			plainText[1]  = plainText[1] * 256 + plainTextByte[i];
		}
		/*
		 * ��Կ�Ǵ�������ʼ���㣬��index��С�ı�ʾ����k����
		 * ����SIMON_KEY_WORDS=3����inputKeysByte[0...3]=k2;inputKeysByte[4...7]=k1;inputKeysByte[8...11]=k0
		 */
		for ( i = 0; i < SIMON_KEY_SIZE/8; i++ ) {
			inputKeys[SIMON_KEY_WORDS-1-i/8] = inputKeys[SIMON_KEY_WORDS-1-i/8] * 256 + inputKeysByte[i];
		}

		time_t start, end;
		int recordCount = 0;
		for ( recordCount = 4; recordCount <= 1024; recordCount*=2 ) {
			start = clock();
			for ( j = 0; j < 10000*recordCount; j++ ) {
				/*
				 * ����ÿ�ֵ���Կ
				 */
				setSimonKeys64 ( inputKeys, keys );
			}
			end = clock();
			/*
			 * = (10000*recordCount*SIMON_KEY_SIZE)/(end-start) ����ÿ����
			 * = (1000*10000*recordCount*SIMON_KEY_SIZE)/(end-start) ����ÿ��
			 * = (1000*10000*recordCount*SIMON_KEY_SIZE)/��1024*1024*(end-start)) �ױ���ÿ��
			 * = (10.0*recordCount*SIMON_KEY_SIZE)/��1.1024*1.024*(end-start)) �ױ���ÿ��
			 */
			printf("Keys extend: %d, speed: %0.3f Mbps.\n", 10000*recordCount, 10.0*recordCount*SIMON_KEY_SIZE/(1.024*1.024*(end-start )));
		}

		/*
		 * �������ĸ�����ÿ�μ���ǰ���»ָ���������ġ���μ�����ƽ�����Լ�����Ϊ����ʱ��̫�̴����ļ������
		 */
		unsigned long long tempPlainHigher   = plainText[0];
		unsigned long long tempPlainLower    = plainText[1];
		
		/*
		 * ��μ�¼�������ٶ��Ƿ��ȶ�
		 */
		for ( recordCount = 4; recordCount <= 1024; recordCount*=2 ) {
			/*
			 * ��ʼʱ��
			 */
			start = clock();
			/*
			 * ������ȡƽ��ֵ
			 */
			for ( j = 0; j < 10000*recordCount; j++ ) {
				plainText[0]  = tempPlainHigher;
				plainText[1]  = tempPlainLower;
				/*
				 * ����
				 */
				encryptionSimon64 ( plainText, keys );
			}
			/*
			 * ����ʱ��
			 */
			end = clock();
			/*
			 * = (10000*recordCount*SIMON_BLOCK_SIZE)/(end-start) ����ÿ����
			 * = (1000*10000*recordCount*SIMON_BLOCK_SIZE)/(end-start) ����ÿ��
			 * = (1000*10000*recordCount*SIMON_BLOCK_SIZE)/��1024*1024*(end-start)) �ױ���ÿ��
			 * = (10.0*recordCount*SIMON_BLOCK_SIZE)/��1.1024*1.024*(end-start)) �ױ���ÿ��
			 */
			printf("Encryption counts: %d, speed: %0.3f Mbps.\n", 10000*recordCount, 10.0*recordCount*SIMON_BLOCK_SIZE/(1.024*1.024*(end-start )));
		}

		printf("\n\nKeys\n");
		for ( i = 0; i < SIMON_ROUNDS; i++ ) {
			printf("%x,%x,%x,%x\n", keys[i]%256, keys[i]/256%256, keys[i]/256/256%256, keys[i]/256/256/256);
		}
		printf("\n\nEnd\n");


		/*
		 * ��ÿ8������λת��Ϊ�ֽ�����Ӧ����ֵ�������
		 */
		tempPlainHigher   = plainText[0];
		tempPlainLower    = plainText[1];
		for ( i = 7; i >= 0; i-- ) {
			cipherTextByte[i] = tempPlainHigher % 256;
			tempPlainHigher   /= 256;
		}
		for ( i = SIMON_BLOCK_SIZE/8-1; i >= 8; i-- ) {
			cipherTextByte[i] = tempPlainLower % 256;
			tempPlainLower    /= 256;
		}
		cout<<"After encryption:"<<endl;
		for ( i = 0; i < SIMON_BLOCK_SIZE/8; i++ ) {
			cout<<cipherTextByte[i]<<"  ";
		}
		cout<<endl;

		/*
		 * ���ܣ���������ܺ�Ľ��
		 */
		decryptionSimon64 ( plainText, keys );
		for ( i = 7; i >= 0; i-- ) {
			cipherTextByte[i] = plainText[0] % 256;
			plainText[0]      /= 256;
		}
		for ( i = SIMON_BLOCK_SIZE/8-1; i >= 8; i-- ) {
			cipherTextByte[i] = plainText[1] % 256;
			plainText[1]      /= 256;
		}
		cout<<"After decryption:"<<endl;
		for ( i = 0; i < SIMON_BLOCK_SIZE/8; i++ ) {
			cout<<cipherTextByte[i]<<"  ";
		}
		cout<<endl;

		/*
		 * �ͷ��ڴ�
		 */
		delete cipherTextByte;
		delete inputKeysByte;
		delete plainTextByte;
		delete keys;
		delete inputKeys;
		delete plainText;

	}

}

/*
 * ѡ��Simon�㷨
 * 1��64/96�汾
 * 2��64/128�汾
 * 3��128/128�汾
 */
int simonChoose () {
	int flag = 0;
	cout<<"***********************************   Simon   *********************************"<<endl;
	cout<<"                                  1-->64/96                                    "<<endl;
	cout<<"                                  2-->64/128                                   "<<endl;
	cout<<"                                  3-->128/128                                  "<<endl;
	cout<<"*******************************************************************************"<<endl<<endl;
	cin>>flag;
	return flag;
}

/*
 * Speck�㷨
 * 1��64/96�汾
 * 2��64/128�汾
 * 3��128/128�汾
 */
void speck () {
	
	/*unsigned int temp1 = 0xc0000000;
	unsigned int temp2 = 0x40000001;
	cout<<(temp1+temp2)<<endl;*/

	int flag = speckChoose();
	if ( flag == 1 ) {
		SPECK_BLOCK_SIZE = 64;
		SPECK_KEY_SIZE   = 96;
		SPECK_WORD_SIZE  = SPECK_BLOCK_SIZE/2;
		SPECK_KEY_WORDS  = SPECK_KEY_SIZE/SPECK_WORD_SIZE;
		SPECK_ROT_A      = 8;
		SPECK_ROT_B      = 3;
		SPECK_ROUNDS     = 26;
	} else if ( flag == 2 ) {
		SPECK_BLOCK_SIZE = 64;
		SPECK_KEY_SIZE   = 128;
		SPECK_WORD_SIZE  = SPECK_BLOCK_SIZE/2;
		SPECK_KEY_WORDS  = SPECK_KEY_SIZE/SPECK_WORD_SIZE;
		SPECK_ROT_A      = 8;
		SPECK_ROT_B      = 3;
		SPECK_ROUNDS     = 27;
	} else if ( flag == 3 ) {
		SPECK_BLOCK_SIZE = 128;
		SPECK_KEY_SIZE   = 128;
		SPECK_WORD_SIZE  = SPECK_BLOCK_SIZE/2;
		SPECK_KEY_WORDS  = SPECK_KEY_SIZE/SPECK_WORD_SIZE;
		SPECK_ROT_A      = 8;
		SPECK_ROT_B      = 3;
		SPECK_ROUNDS     = 32;
	}

	if ( SPECK_WORD_SIZE == 32 ) {
		int i, j;
		unsigned int * plainText      = new unsigned int[2]; // ����
		plainText[0] = plainText[1]   = 0x0;
		unsigned int * inputKeys      = new unsigned int[SPECK_KEY_WORDS]; // ��Կ
		for ( i = 0; i < SPECK_KEY_WORDS; i++ ) {
			inputKeys[i] = 0;
		}
		unsigned int * keys           = new unsigned int[SPECK_ROUNDS]; // ��Կ����SIMON_ROUNDS�֣�ÿ��SIMON_WORD_SIZEλ
		for ( i = 0; i < SPECK_ROUNDS; i++ ) {
			/*
			 * �����¼��keys = 0x0;
			 * ִ�г��ִ���д���ַ0x00000000ʱ������ͻ
			 */
			keys[i] = 0x0;
		}
		unsigned int * plainTextByte  = new unsigned int[SPECK_BLOCK_SIZE/8]; // ���ֽڱ�ʾ�����ģ����ڱ������������
		for ( i = 0; i < SPECK_BLOCK_SIZE/8; i++ ) {
			plainTextByte[i] = 0;
		}
		unsigned int * inputKeysByte  = new unsigned int [SPECK_KEY_SIZE/8]; // ���ֽڱ�ʾ����Կ�����ڱ����������Կ
		for ( i = 0; i < SPECK_KEY_SIZE/8; i++ ) {
			inputKeysByte[i] = 0;
		}
		unsigned int * cipherTextByte  = new unsigned int [SPECK_BLOCK_SIZE/8];  // ���ֽڱ�ʾ������
		for ( i = 0; i < SPECK_BLOCK_SIZE/8; i++ ) {
			cipherTextByte[i] = 0;
		}

		/*
		 * �õ����ĺͳ�ʼ��Կ
		 */
		cout<<"Please input the plainText of "<<SPECK_BLOCK_SIZE/8<<" bytes:"<<endl;
		for ( i = 0; i < SPECK_BLOCK_SIZE/8; i++ ) {
			cin>>plainTextByte[i];
		}
		cout<<"Please input the key of "<<SPECK_KEY_SIZE/8<<" bytes:"<<endl;
		for ( i = 0; i < SPECK_KEY_SIZE/8; i++ ) {
			cin>>inputKeysByte[i];
		}

		/*
		 * �����ĺ���Կ��unsigned int��ʾ
		 */
		for ( i = 0; i < 4; i++ ) {
			plainText[0] = plainText[0] * 256 + plainTextByte[i];
		}
		for ( i = 4; i < SPECK_BLOCK_SIZE/8; i++ ) {
			plainText[1] = plainText[1] * 256 + plainTextByte[i];
		}
		for ( i = 0; i < SPECK_KEY_SIZE/8; i++ ) {
			inputKeys[i/4] = inputKeys[i/4] * 256 + inputKeysByte[i];
		}

		time_t start, end;
		int recordCount = 0;
		for ( recordCount = 4; recordCount <= 1024; recordCount*=2 ) {
			start = clock();
			for ( j = 0; j < 10000*recordCount; j++ ) {
				/*
				 * ����ÿ�ֵ���Կ
				 */
				setSpeckKeys32 ( inputKeys, keys );
			}
			end = clock();
			printf("Keys extend: %d, speed: %0.3f Mbps.\n", 10000*recordCount, 10.0*recordCount*SPECK_KEY_SIZE/(1.024*1.024*(end-start )));
		}
		

		printf("\n\nKeys\n");
		for ( i = 0; i < SPECK_ROUNDS; i++ ) {
			printf("%x,%x,%x,%x\n", keys[i]%256, keys[i]/256%256, keys[i]/256/256%256, keys[i]/256/256/256);
		}
		printf("\n\nEnd\n");
			
		/*
		 * ���渱�� 
		 */
		unsigned int tempPlainHigher   = plainText[0];
		unsigned int tempPlainLower    = plainText[1];
		/*
		 * ��μ�¼�������ٶ��Ƿ��ȶ�
		 */
		for ( recordCount = 4; recordCount <= 1024; recordCount*=2 ) {
			start = clock();
			/* 
			 * ������ȡƽ��ֵ
			 */
			for ( j = 0; j < 10000*recordCount; j++ ) {
				plainText[0] = tempPlainHigher;
				plainText[1] = tempPlainLower;
				encryptionSpeck32 ( plainText, keys );
			}
			end = clock();
			printf("Encryption counts: %d, speed: %0.3f Mbps.\n", 10000*recordCount, 10.0*recordCount*SPECK_BLOCK_SIZE/(1.024*1.024*(end-start )));
		}

		tempPlainHigher   = plainText[0];
		tempPlainLower    = plainText[1];
		for ( i = 3; i >= 0; i-- ) {
			cipherTextByte[i] = tempPlainHigher % 256;
			tempPlainHigher   /= 256;
		}
		for ( i = SPECK_BLOCK_SIZE/8-1; i >= 4; i-- ) {
			cipherTextByte[i] = tempPlainLower % 256;
			tempPlainLower   /= 256;
		}
		cout<<"After encryption:"<<endl;
		for ( i = 0; i < SPECK_BLOCK_SIZE/8; i++ ) {
			cout<<cipherTextByte[i]<<"  ";
		}
		cout<<endl;

		/*
		 * ���ܣ���������ܺ�Ľ��
		 */
		decryptionSpeck32 ( plainText, keys );
		for ( i = 3; i >= 0; i-- ) {
			cipherTextByte[i] = plainText[0] % 256;
			plainText[0]   /= 256;
		}
		for ( i = SPECK_BLOCK_SIZE/8-1; i >= 4; i-- ) {
			cipherTextByte[i] = plainText[1] % 256;
			plainText[1]   /= 256;
		}
		cout<<"After decryption:"<<endl;
		for ( i = 0; i < SPECK_BLOCK_SIZE/8; i++ ) {
			cout<<cipherTextByte[i]<<"  ";
		}
		cout<<endl;

		/*
		 * �ͷ��ڴ�
		 */
		delete cipherTextByte;
		delete inputKeysByte;
		delete plainTextByte;
		delete keys;
		delete inputKeys;
		delete plainText;
	} else if ( SPECK_WORD_SIZE == 64 ) {

		int i, j;
		unsigned long long * plainText      = new unsigned long long[2]; // ����
		plainText[0] = plainText[1]   = 0x0;
		unsigned long long * inputKeys      = new unsigned long long[SPECK_KEY_WORDS]; // ��Կ
		for ( i = 0; i < SPECK_KEY_WORDS; i++ ) {
			inputKeys[i] = 0;
		}
		unsigned long long * keys           = new unsigned long long[SPECK_ROUNDS]; // ��Կ����SIMON_ROUNDS�֣�ÿ��SIMON_WORD_SIZEλ
		for ( i = 0; i < SPECK_ROUNDS; i++ ) {
			keys[i] = 0x0;
		}
		unsigned long long * plainTextByte  = new unsigned long long[SPECK_BLOCK_SIZE/8]; // ���ֽڱ�ʾ�����ģ����ڱ������������
		for ( i = 0; i < SPECK_BLOCK_SIZE/8; i++ ) {
			plainTextByte[i] = 0;
		}
		unsigned long long * inputKeysByte  = new unsigned long long [SPECK_KEY_SIZE/8]; // ���ֽڱ�ʾ����Կ�����ڱ����������Կ
		for ( i = 0; i < SPECK_KEY_SIZE/8; i++ ) {
			inputKeysByte[i] = 0;
		}
		unsigned long long * cipherTextByte  = new unsigned long long [SPECK_BLOCK_SIZE/8];  // ���ֽڱ�ʾ������
		for ( i = 0; i < SPECK_BLOCK_SIZE/8; i++ ) {
			cipherTextByte[i] = 0;
		}

		/*
		 * �õ����ĺͳ�ʼ��Կ
		 */
		cout<<"Please input the plainText of "<<SPECK_BLOCK_SIZE/8<<" bytes:"<<endl;
		for ( i = 0; i < SPECK_BLOCK_SIZE/8; i++ ) {
			cin>>plainTextByte[i];
		}
		cout<<"Please input the key of "<<SPECK_KEY_SIZE/8<<" bytes:"<<endl;
		for ( i = 0; i < SPECK_KEY_SIZE/8; i++ ) {
			cin>>inputKeysByte[i];
		}

		/*
		 * �����ĺ���Կ��unsigned int��ʾ
		 */
		for ( i = 0; i < 8; i++ ) {
			plainText[0] = plainText[0] * 256 + plainTextByte[i];
		}
		for ( i = 8; i < SPECK_BLOCK_SIZE/8; i++ ) {
			plainText[1] = plainText[1] * 256 + plainTextByte[i];
		}
		for ( i = 0; i < SPECK_KEY_SIZE/8; i++ ) {
			inputKeys[i/8] = inputKeys[i/8] * 256 + inputKeysByte[i];
		}

		time_t start, end;
		int recordCount = 0;
		for ( recordCount = 4; recordCount <= 1024; recordCount*=2 ) {
			start = clock();
			for ( j = 0; j < 10000*recordCount; j++ ) {
				/*
				 * ����ÿ�ֵ���Կ
				 */
				setSpeckKeys64 ( inputKeys, keys );
			}
			end = clock();
			printf("Keys extend: %d, speed: %0.3f Mbps.\n", 10000*recordCount, 10.0*recordCount*SPECK_KEY_SIZE/(1.024*1.024*(end-start )));
		}
		
		for ( i = 0; i < SPECK_ROUNDS; i++ ) {
			cout<<keys[i]%256<<","<<keys[i]/256%256<<","<<keys[i]/256/256%256<<","<<keys[i]/256/256/256<<endl;
		}

		/*
		 * ���渱�� 
		 */
		unsigned long long tempPlainHigher   = plainText[0];
		unsigned long long tempPlainLower    = plainText[1];
		/*
		 * ��μ�¼�������ٶ��Ƿ��ȶ�
		 */
		for ( recordCount = 4; recordCount <= 1024; recordCount*=2 ) {
			start = clock();
			/* 
			 * ������ȡƽ��ֵ
			 */
			for ( j = 0; j < 10000*recordCount; j++ ) {
				plainText[0] = tempPlainHigher;
				plainText[1] = tempPlainLower;
				encryptionSpeck64 ( plainText, keys );
			}
			end = clock();
			printf("Encryption counts: %d, speed: %0.3f Mbps.\n", 10000*recordCount, 10.0*recordCount*SPECK_BLOCK_SIZE/(1.024*1.024*(end-start )));
		}

		tempPlainHigher   = plainText[0];
		tempPlainLower    = plainText[1];
		for ( i = 7; i >= 0; i-- ) {
			cipherTextByte[i] = tempPlainHigher % 256;
			tempPlainHigher   /= 256;
		}
		for ( i = SPECK_BLOCK_SIZE/8-1; i >= 8; i-- ) {
			cipherTextByte[i] = tempPlainLower % 256;
			tempPlainLower   /= 256;
		}
		cout<<"After encryption:"<<endl;
		for ( i = 0; i < SPECK_BLOCK_SIZE/8; i++ ) {
			cout<<cipherTextByte[i]<<"  ";
		}
		cout<<endl;

		/*
		 * ���ܣ���������ܺ�Ľ��
		 */
		decryptionSpeck64 ( plainText, keys );
		for ( i = 7; i >= 0; i-- ) {
			cipherTextByte[i] = plainText[0] % 256;
			plainText[0]   /= 256;
		}
		for ( i = SPECK_BLOCK_SIZE/8-1; i >= 8; i-- ) {
			cipherTextByte[i] = plainText[1] % 256;
			plainText[1]   /= 256;
		}
		cout<<"After decryption:"<<endl;
		for ( i = 0; i < SPECK_BLOCK_SIZE/8; i++ ) {
			cout<<cipherTextByte[i]<<"  ";
		}
		cout<<endl;

		/*
		 * �ͷ��ڴ�
		 */
		delete cipherTextByte;
		delete inputKeysByte;
		delete plainTextByte;
		delete keys;
		delete inputKeys;
		delete plainText;

	}

}

/*
 * ѡ��Speck�㷨
 * 1��64/96�汾
 * 2��64/128�汾
 * 3��128/128�汾
 */
int speckChoose () {
	int flag = 0;
	cout<<"***********************************   Speck   *********************************"<<endl;
	cout<<"                                  1-->64/96                                    "<<endl;
	cout<<"                                  2-->64/128                                   "<<endl;
	cout<<"                                  3-->128/128                                  "<<endl;
	cout<<"*******************************************************************************"<<endl<<endl;
	cin>>flag;
	return flag;
}