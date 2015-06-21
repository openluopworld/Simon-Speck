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
 *
 * Simon��z0��z1��peoriod�����ڣ�Ϊ31����ǰ31������֮�󣬴ӵ�32�����ص���62�����أ����ظ�������ͬ��ǰ31�����أ�һֱ������ȥ��
 * ͬ����z2,z3,z4�����ھ�Ϊ62����62������֮�����ظ�������ͬ��62�����أ�һֱ������ȥ��
 *
 * 
 */

int main () {
	
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

	int i, j;
	int * plainText       =  new int[SIMON_BLOCK_SIZE];            // ���ı���λ�ĸ���
	for ( i = 0; i < SIMON_BLOCK_SIZE; i++ ) {
		plainText[i] = 0;
	}
	int * inputKeys   = new int[SIMON_KEY_SIZE];                   // ��Կ����λ�ĸ���
	for ( i = 0; i < SIMON_KEY_SIZE; i++ ) {
		inputKeys[i] = 0;
	}
	int ** keys     = new int* [SIMON_ROUNDS];                     // ��Կ����SIMON_ROUNDS�֣�ÿ��SIMON_WORD_SIZEλ
	for ( i = 0; i < SIMON_ROUNDS; i++ ) {
		keys[i] = new int[SIMON_WORD_SIZE];
	}
	for ( i = 0; i < SIMON_ROUNDS; i++ ) {
		for ( j = 0; j < SIMON_WORD_SIZE; j++ ) {
			keys[i][j] = 0;
		}
	}
	int * plainTextHex  = new int[SIMON_BLOCK_SIZE/8];             // Ϊ�˼����룬ÿ���������ֽڱ�ʾ��[0,255]
	for ( i = 0; i < SIMON_BLOCK_SIZE/8; i++ ) {
		plainTextHex[i] = 0;
	}
	int * inputKeysHex = new int [SIMON_KEY_SIZE/8];               // ���ֽڱ�ʾ����Կ�����ڱ����������Կ
	for ( i = 0; i < SIMON_KEY_SIZE/8; i++ ) {
		inputKeysHex[i] = 0;
	}
	int * cipherTextHex = new int [SIMON_BLOCK_SIZE/8];            // ���ֽڱ�ʾ������
	for ( i = 0; i < SIMON_BLOCK_SIZE/8; i++ ) {
		cipherTextHex[i] = 0;
	}

	/*
	 * �õ����ĺͳ�ʼ��Կ
	 */
	cout<<"Please input the plainText of "<<SIMON_BLOCK_SIZE/8<<" bytes:"<<endl;
	for ( i = 0; i < SIMON_BLOCK_SIZE/8; i++ ) {
		cin>>plainTextHex[i];
	}
	cout<<"Please input the key of "<<SIMON_KEY_SIZE/8<<" bytes:"<<endl;
	for ( i = 0; i < SIMON_KEY_SIZE/8; i++ ) {
		cin>>inputKeysHex[i];
	}

	/*
	 * �����ĺ���Կ�Զ�������ʽ��ʾ
	 */
	int index;
	for ( i = 0; i < SIMON_BLOCK_SIZE/8; i++ ) {
		index = 7;
		while ( plainTextHex[i] != 0 ) {
			plainText[i*8+index] = plainTextHex[i]%2;
			plainTextHex[i] /= 2;
			index--;
		}
	}
	for ( i = 0; i < SIMON_KEY_SIZE/8; i++ ) {
		index = 7;
		while ( inputKeysHex[i] != 0 ) {
			inputKeys[i*8+index] = inputKeysHex[i]%2;
			inputKeysHex[i] /= 2;
			index--;
		}
	}

	time_t start, end;
	int recordCount = 0;
	for ( recordCount = 1; recordCount <= 32; recordCount*=2 ) {
		start = clock();
		for ( j = 0; j < 10000*recordCount; j++ ) {
			/*
			 * ����ÿ�ֵ���Կ
			 */
			setSimonKeys ( inputKeys, keys );
		}
		end = clock();
		/*
		 * = (10000*recordCount*SIMON_KEY_SIZE)/(end-start) ����ÿ����
		 * = (1000*10000*recordCount*SIMON_KEY_SIZE)/(end-start) ����ÿ��
		 * = (1000*10000*recordCount*SIMON_KEY_SIZE)/��1024*1024*(end-start)) �ױ���ÿ��
		 * = (10.0*recordCount*SIMON_KEY_SIZE)/��.1024*1.024*(end-start)) �ױ���ÿ��
		 */
		printf("Keys extend: %d, speed: %0.2f Mbps.\n", 10000*recordCount, 10.0*recordCount*SIMON_KEY_SIZE/(1.024*1.024*(end-start )));
	}

	
	/*
	 * �������ĸ�����ÿ�μ���ǰ���»ָ���������ġ���μ�����ƽ�����Լ�����Ϊ����ʱ��̫�̴����ļ������
	 */
	int * tempPlainText = new int[SIMON_BLOCK_SIZE];
	for ( i = 0; i < SIMON_BLOCK_SIZE; i++ ) {
		tempPlainText[i] = plainText[i];
	}

	/*
	 * ��μ�¼�������ٶ��Ƿ��ȶ�
	 */
	for ( recordCount = 1; recordCount <= 32; recordCount*=2 ) {
		/*
		 * ��ʼʱ��
		 */
		start = clock();
		/*
		 * ������ȡƽ��ֵ
		 */
		for ( j = 0; j < 10000*recordCount; j++ ) {
			for ( i = 0; i < SIMON_BLOCK_SIZE; i++ ) {
				plainText[i] = tempPlainText[i];
			}
			/*
			 * ����
			 */
			encryptionSimon ( plainText, keys );
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
		printf("Encryption counts: %d, speed: %0.2f Mbps.\n", 10000*recordCount, 10.0*recordCount*SIMON_BLOCK_SIZE/(1.024*1.024*(end-start )));
	}

	/*
	 * ��ÿ8������λת��Ϊ�ֽ�����Ӧ����ֵ�������
	 */
	for ( i = 0; i < SIMON_BLOCK_SIZE/8; i++ ) {
		for ( j = 0; j < 8; j++ ) {
			cipherTextHex[i] = cipherTextHex[i]*2 + plainText[i*8+j];
		}
	}
	cout<<"After encryption:"<<endl;
	for ( i = 0; i < SIMON_BLOCK_SIZE/8; i++ ) {
		cout<<cipherTextHex[i]<<"  ";
	}
	cout<<endl;

	/*
	 * ���ܣ���������ܺ�Ľ��
	 */
	decryptionSimon ( plainText, keys );
	for ( i = 0; i < SIMON_BLOCK_SIZE/8; i++ ) {
		// ��ս����֮ǰ������Ǽ��ܵĽ��
		cipherTextHex[i] = 0;      
		for ( j = 0; j < 8; j++ ) {
			cipherTextHex[i] = cipherTextHex[i]*2 + plainText[i*8+j];
		}
	}
	cout<<"After decryption:"<<endl;
	for ( i = 0; i < SIMON_BLOCK_SIZE/8; i++ ) {
		cout<<cipherTextHex[i]<<"  ";
	}
	cout<<endl;

	/*
	 * �ͷ��ڴ�
	 */
	delete tempPlainText;
	delete cipherTextHex;
	delete inputKeysHex;
	delete plainTextHex;
	// ע�������ͷŵ�˳��
	for ( int i = 0; i < SIMON_ROUNDS; i++ ) {
		delete keys[i];
	}
	delete keys;
	delete inputKeys;
	delete plainText;

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

	int i, j;
	int * plainText      =  new int[SPECK_BLOCK_SIZE];            // ����
	for ( i = 0; i < SPECK_BLOCK_SIZE; i++ ) {
		plainText[i] = 0;
	}
	int * inputKeys   = new int[SPECK_KEY_SIZE];                   // ��Կ
	for ( i = 0; i < SPECK_KEY_SIZE; i++ ) {
		inputKeys[i] = 0;
	}
	int ** keys     = new int * [SPECK_ROUNDS];                     // ��Կ����SIMON_ROUNDS�֣�ÿ��SIMON_WORD_SIZEλ
	for ( i = 0; i < SPECK_ROUNDS; i++ ) {
		keys[i] = new int[SPECK_WORD_SIZE];
	}
	for ( i = 0; i < SPECK_ROUNDS; i++ ) {
		for ( j = 0; j < SPECK_WORD_SIZE; j++ ) {
			keys[i][j] = 0;
		}
	}
	int * plainTextHex  = new int[SPECK_BLOCK_SIZE/8];             // ���ֽڱ�ʾ�����ģ����ڱ������������
	for ( i = 0; i < SPECK_BLOCK_SIZE/8; i++ ) {
		plainTextHex[i] = 0;
	}
	int * inputKeysHex = new int [SPECK_KEY_SIZE/8];               // ���ֽڱ�ʾ����Կ�����ڱ����������Կ
	for ( i = 0; i < SPECK_KEY_SIZE/8; i++ ) {
		inputKeysHex[i] = 0;
	}
	int * cipherTextHex = new int [SPECK_BLOCK_SIZE/8];            // ���ֽڱ�ʾ������
	for ( i = 0; i < SPECK_BLOCK_SIZE/8; i++ ) {
		cipherTextHex[i] = 0;
	}

	/*
	 * �õ����ĺͳ�ʼ��Կ
	 */
	cout<<"Please input the plainText of "<<SPECK_BLOCK_SIZE/8<<" bytes:"<<endl;
	for ( i = 0; i < SPECK_BLOCK_SIZE/8; i++ ) {
		cin>>plainTextHex[i];
	}
	cout<<"Please input the key of "<<SPECK_KEY_SIZE/8<<" bytes:"<<endl;
	for ( i = 0; i < SPECK_KEY_SIZE/8; i++ ) {
		cin>>inputKeysHex[i];
	}

	/*
	 * �����ĺ���Կ�Զ�������ʽ��ʾ
	 */
	int index;
	for ( i = 0; i < SPECK_BLOCK_SIZE/8; i++ ) {
		index = 7;
		while ( plainTextHex[i] != 0 ) {
			plainText[i*8+index] = plainTextHex[i]%2;
			plainTextHex[i] /= 2;
			index--;
		}
	}
	for ( i = 0; i < SPECK_KEY_SIZE/8; i++ ) {
		index = 7;
		while ( inputKeysHex[i] != 0 ) {
			inputKeys[i*8+index] = inputKeysHex[i]%2;
			inputKeysHex[i] /= 2;
			index--;
		}
	}

	time_t start, end;
	int recordCount = 0;
	for ( recordCount = 1; recordCount <= 32; recordCount*=2 ) {
		start = clock();
		for ( j = 0; j < 10000*recordCount; j++ ) {
			/*
			 * ����ÿ�ֵ���Կ
			 */
			setSpeckKeys( inputKeys, keys );;
		}
		end = clock();
		printf("Keys extend: %d, speed: %0.2f Mbps.\n", 10000*recordCount, 10.0*recordCount*SPECK_KEY_SIZE/(1.024*1.024*(end-start )));
	}
		

	/*
	 * ���渱�� 
	 */
	int * tempPlainText = new int[SPECK_BLOCK_SIZE];
	for ( i = 0; i < SPECK_BLOCK_SIZE; i++ ) {
		tempPlainText[i] = plainText[i];
	}

	/*
	 * ��μ�¼�������ٶ��Ƿ��ȶ�
	 */
	for ( recordCount = 1; recordCount <= 32; recordCount*=2 ) {
		start = clock();
		/* 
		 * ������ȡƽ��ֵ
		 */
		for ( j = 0; j < 10000*recordCount; j++ ) {
			for ( i = 0; i < SPECK_BLOCK_SIZE; i++ ) {
				plainText[i] = tempPlainText[i];
			}
			encryptionSpeck ( plainText, keys );
		}
		end = clock();
		/*
		 * = (10000*recordCount*SPECK_BLOCK_SIZE)/(end-start) ����ÿ����
		 * = (1000*10000*recordCount*SPECK_BLOCK_SIZE)/(end-start) ����ÿ��
		 * = (1000*10000*recordCount*SPECK_BLOCK_SIZE)/��1024*1024*(end-start)) �ױ���ÿ��
		 * = (10.0*recordCount*SPECK_BLOCK_SIZE)/��1.024*1.024*(end-start)) �ױ���ÿ��
		 */
		printf("Encryption counts: %d, speed: %0.2f Mbps.\n", 10000*recordCount, 10.0*recordCount*SPECK_BLOCK_SIZE/(1.024*1.024*(end-start )));
	}

	for ( i = 0; i < SPECK_BLOCK_SIZE/8; i++ ) {
		cipherTextHex[i] = 0;
		for ( j = 0; j < 8; j++ ) {
			cipherTextHex[i] = cipherTextHex[i]*2 + plainText[i*8+j];
		}
	}
	cout<<"After encryption:"<<endl;
	for ( i = 0; i < SPECK_BLOCK_SIZE/8; i++ ) {
		cout<<cipherTextHex[i]<<"  ";
	}
	cout<<endl;

	/*
	 * ���ܣ���������ܺ�Ľ��
	 */
	decryptionSpeck ( plainText, keys );
	for ( i = 0; i < SPECK_BLOCK_SIZE/8; i++ ) {
		cipherTextHex[i] = 0;      // ��ս����֮ǰ������Ǽ��ܵĽ��
		for ( j = 0; j < 8; j++ ) {
			cipherTextHex[i] = cipherTextHex[i]*2 + plainText[i*8+j];
		}
	}
	cout<<"After decryption:"<<endl;
	for ( i = 0; i < SPECK_BLOCK_SIZE/8; i++ ) {
		cout<<cipherTextHex[i]<<"  ";
	}
	cout<<endl;

	/*
	 * �ͷ��ڴ�
	 */
	delete tempPlainText;
	delete cipherTextHex;
	delete inputKeysHex;
	delete plainTextHex;
	// ע�������ͷŵ�˳��
	for ( int i = 0; i < SPECK_ROUNDS; i++ ) {
		delete keys[i];
	}
	delete keys;
	delete inputKeys;
	delete plainText;

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