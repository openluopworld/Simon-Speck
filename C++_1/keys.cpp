
#include"keys.h"
#include"keyZ.h"
#include"const.h"
#include<iostream>
using namespace std;

/*
 * Simon��������Կ
 * inputKey����ʼ����Կ
 * keys�������õ���ÿ����Կ
 */
void setSimonKeys ( int * inputKey, int ** keys ) {

	/*
	 * �㷨�еĳ���c����СΪ2^n - 4������n���ֵĳ��ȣ���SIMON_WORD_SIZE
	 * ת��Ϊ�����ƣ��������λΪ0������λȫΪ1
	 */
	int * c = new int[SIMON_WORD_SIZE];
	for ( int i = 0; i < SIMON_WORD_SIZE-2; i++ ) {
		c[i] = 1;
	}
	c[SIMON_WORD_SIZE-2] = 0;
	c[SIMON_WORD_SIZE-1] = 0;

	/*
	 * ���ݳ�ʼ����õ�ǰSIMON_KEY_WORDS����Կ
	 * inputKey�ĳ�����SIMON_KEY_SIZE(=SIMON_WORD_SIZE*SIMON_KEY_WORDS)
	 * ��������һ��Ϊk0,k1,k(m-1)
	 */
	int i, j;
	for ( i = SIMON_KEY_WORDS-1; i >= 0; i-- ) {
		for ( j = SIMON_WORD_SIZE-1; j >= 0; j-- ) {
			keys[SIMON_KEY_WORDS-1-i][j] = inputKey[i*SIMON_WORD_SIZE+j];
		}
	}

	/*
	 * �������ֵ���Կ
	 * ������������������Zji�����Ϊ1������λ�����޸ģ����򲻱�
	 */
	if ( SIMON_KEY_WORDS == 2 ) {
		for ( i = 0; i < SIMON_ROUNDS-SIMON_KEY_WORDS; i++ ) {
			for ( j = 0; j < SIMON_WORD_SIZE; j++ ) {
				/*
				 * ����3λ�����λ��Ӧ����ֵ��3���������Ϊ����ȡģ�õ����Ǹ����������ȼ���SIMON_WORD_SIZE����ȡģ��
				 * ��(x-3+SIMON_WORD_SIZE)%SIMON_WORD_SIZE��ͬ������1λ��4λ����Ӧ����
				 */
				keys[i+SIMON_KEY_WORDS][j] = (c[j] + 
								keys[i][j] +
								keys[i+1][(j-3+SIMON_WORD_SIZE)%SIMON_WORD_SIZE] +
								keys[i+1][(j-4+SIMON_WORD_SIZE)%SIMON_WORD_SIZE] 
								) % 2;
			}
			/*
			 * �����z[j][i]����������㣬��SIMON_KEY_WORDSȷ��ʱ��jҲ��ȷ����
			 */
			if ( z[SIMON_SEQUENCE_NUMBER][i%62] == 1 ) {
				keys[i+SIMON_KEY_WORDS][SIMON_WORD_SIZE-1] =  ( keys[i+SIMON_KEY_WORDS][SIMON_WORD_SIZE-1] + 1 ) % 2;
			}
		}
	} else if ( SIMON_KEY_WORDS == 3 ) {
		int cycle = (SIMON_SEQUENCE_NUMBER == 0 || SIMON_SEQUENCE_NUMBER == 1)?31:62;
		for ( i = 0; i < SIMON_ROUNDS-SIMON_KEY_WORDS; i++ ) {
			for ( j = 0; j < SIMON_WORD_SIZE; j++ ) {
				keys[i+SIMON_KEY_WORDS][j] = (c[j] + 
								keys[i][j] +
								keys[i+2][(j-3+SIMON_WORD_SIZE)%SIMON_WORD_SIZE] +
								keys[i+2][(j-4+SIMON_WORD_SIZE)%SIMON_WORD_SIZE] 
								) % 2;
			}
			if ( z[SIMON_SEQUENCE_NUMBER][i%cycle] == 1 ) {
				keys[i+SIMON_KEY_WORDS][SIMON_WORD_SIZE-1] =  ( keys[i+SIMON_KEY_WORDS][SIMON_WORD_SIZE-1] + 1 ) % 2;
			}
		}
	} else if ( SIMON_KEY_WORDS == 4 ) {
		int cycle = (SIMON_SEQUENCE_NUMBER == 0 || SIMON_SEQUENCE_NUMBER == 1)?31:62;
		for ( i = 0; i < SIMON_ROUNDS-SIMON_KEY_WORDS; i++ ) {
			for ( j = 0; j < SIMON_WORD_SIZE; j++ ) {
				keys[i+SIMON_KEY_WORDS][j] = (c[j] + 
								keys[i][j] +
								(keys[i+3][(j-3+SIMON_WORD_SIZE)%SIMON_WORD_SIZE]+keys[i+1][j])%2 +
								(keys[i+3][(j-4+SIMON_WORD_SIZE)%SIMON_WORD_SIZE]+keys[i+1][(j-1+SIMON_WORD_SIZE)%SIMON_WORD_SIZE])%2 
								) % 2;
			}
			if ( z[SIMON_SEQUENCE_NUMBER][i%cycle] == 1 ) {
				keys[i+SIMON_KEY_WORDS][SIMON_WORD_SIZE-1] =  ( keys[i+SIMON_KEY_WORDS][SIMON_WORD_SIZE-1] + 1 ) % 2;
			}
		}
	}
	/*
	 * �ͷ�c
	 */
	delete c;
}

/*
 * Speck��������Կ
 * inputKey����ʼ����Կ
 * keys�������õ���ÿ����Կ
 */
void setSpeckKeys ( int * inputKey, int ** keys ) {
	int i, j;
	/*
	 * inputKey = (L(m-2), ...,L(0),keys(0));��L��������keys��m-2(��SPECK_KEY_WORDS-2)
	 * keys��������SPECK_ROUNDS����L������ΪSPECK_ROUNDS+SPECK_KEY_WORDS-2
	 */
	int ** L = new int * [SPECK_ROUNDS+SPECK_KEY_WORDS-2];
	for ( i = 0; i < SPECK_ROUNDS+SPECK_KEY_WORDS-2; i++ ) {
		L[i] = new int[SPECK_WORD_SIZE];
	}

	/*
	 * �ҵ�keys[0]����ʼ�±�
	 */
	int beginIndex = (SPECK_KEY_WORDS-1)*SPECK_WORD_SIZE;
	for ( i = beginIndex; i < SPECK_KEY_SIZE; i++ ) {
		keys[0][i-beginIndex] = inputKey[i];
	}

	/*
	 * ����L��ǰSPECK_KEY_WORDS-1��
	 */
	for ( i = SPECK_KEY_WORDS-2; i >= 0; i-- ) {
		for ( j = SPECK_WORD_SIZE-1; j >= 0; j-- ) {
			L[SPECK_KEY_WORDS-2-i][j] = inputKey[i*SPECK_WORD_SIZE+j];
		}
	}

	/*
	 * ������Կ
	 */
	int index;
	int temp;
	int jinwei;
	for ( i = 0; i < SPECK_ROUNDS-1; i++ ) {
		/*
		 * ����L[i+SPECK_KEY_WORDS-1]
		 * �Ƚ���ģ2^n�ļӷ����㣬����i�����
		 * ģ2^n�ļӷ��������һ�����λ�ļӷ��������λ������������λ����ʼ�ӣ�������ģ2^n�����һ����λ���ÿ�������
		 */
		jinwei = 0;
		for ( j = SPECK_WORD_SIZE-1; j >= 0; j-- ) {
			L[i+SPECK_KEY_WORDS-1][j] = ( keys[i][j] + L[i][(j-SPECK_ROT_A+SPECK_WORD_SIZE)%SPECK_WORD_SIZE] + jinwei ) % 2;
			jinwei = ( keys[i][j] + L[i][(j-SPECK_ROT_A+SPECK_WORD_SIZE)%SPECK_WORD_SIZE] + jinwei) / 2;
		}
		temp = i;
		index = SPECK_WORD_SIZE-1;
		while ( temp != 0 ) {
			if ( temp % 2 == 1 ) {
				L[i+SPECK_KEY_WORDS-1][index] = ( L[i+SPECK_KEY_WORDS-1][index] + 1 ) % 2;
			}
			temp /= 2;
			index--;
		}

		/*
		 * ����keys[i+1]
		 * 
		 * for ( j = 0; j < SPECK_WORD_SIZE-1; j++ ) {
		 * ���˺ô󾢲��ҳ�������󣬲�֪��Ϊʲô��д��-1����ʽ
		 * ����ÿ����Կ�����һ��������λ����ȷ�����¼��ܽ������ȷ�����ǽ��ܺ����ܻ�ԭ
		 */
		for ( j = 0; j < SPECK_WORD_SIZE; j++ ) {
			keys[i+1][j] = ( keys[i][(j+SPECK_ROT_B)%SPECK_WORD_SIZE] + L[i+SPECK_KEY_WORDS-1][j] ) % 2;
		}
	}

	//cout<<endl<<"L:"<<endl;
	//for ( i = 0; i < SPECK_ROUNDS+SPECK_KEY_WORDS-2; i++ ) {
	//	cout<<i<<":";
	//	for ( j = 0; j < SPECK_WORD_SIZE; j++ ) {
	//		cout<<L[i][j]<<" ";
	//	}
	//	cout<<endl;
	//}
	//cout<<endl;

	/*
	 * �ͷ��ڴ�
	 */
	for ( i = 0; i < SPECK_ROUNDS+SPECK_KEY_WORDS-2; i++ ) {
		delete L[i];
	}
	delete L;

}