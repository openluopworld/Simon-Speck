
#include"keys.h"
#include"keyZ.h"
#include"const.h"
#include<iostream>
using namespace std;

/*
 * Simon��������Կ���ִ�СΪ32
 * inputKey����ʼ����Կ
 * keys�������õ���ÿ����Կ
 */
void setSimonKeys32 ( unsigned int * inputKey, unsigned int * keys ) {

	/*
	 * �㷨�еĳ���c����СΪ2^n - 4������n���ֵĳ��ȣ���SIMON_WORD_SIZE
	 * ת��Ϊ�����ƣ��������λΪ0������λȫΪ1
	 */
	unsigned int c = 0xfffffffc;

	int i;
	for ( i = 0; i < SIMON_KEY_WORDS; i++ )  {
		keys[i] = inputKey[i];
	}

	/*
	 * �������ֵ���Կ
	 * ������������������Zji�����Ϊ1������λ�����޸ģ����򲻱�
	 */
	if ( SIMON_KEY_WORDS == 3 ) {
		for ( i = 0; i < SIMON_ROUNDS-SIMON_KEY_WORDS; i++ ) {
			keys[i+SIMON_KEY_WORDS] = c ^ keys[i] ^
							((keys[i+2]>>3) | (keys[i+2]<<(SIMON_WORD_SIZE-3))) ^
							((keys[i+2]>>4) | (keys[i+2]<<(SIMON_WORD_SIZE-4)));
			// SIMON_WORD_SIZEΪ32ʱ������SIMON_KEY_WORDSΪ3����4�����ڶ���62
			if ( z[SIMON_SEQUENCE_NUMBER][i%62] == 1 ) {
				keys[i+SIMON_KEY_WORDS] ^=  0x1;
			}
		}
	} else if ( SIMON_KEY_WORDS == 4 ) {
		// int cycle = (SIMON_SEQUENCE_NUMBER == 0 || SIMON_SEQUENCE_NUMBER == 1)?31:62;
		unsigned int temp = 0x00000000;
		for ( i = 0; i < SIMON_ROUNDS-SIMON_KEY_WORDS; i++ ) {
			temp = ((keys[i+3]>>3) | (keys[i+3]<<(SIMON_WORD_SIZE-3))) ^ keys[i+1];
			keys[i+SIMON_KEY_WORDS] = c ^ keys[i] ^ temp ^ ((temp>>1) | (temp<<(SIMON_WORD_SIZE-1)));
			if ( z[SIMON_SEQUENCE_NUMBER][i%62] == 1 ) {
				keys[i+SIMON_KEY_WORDS] ^=  0x00000001;
			}
		}
	}

}

/*
 * Simon��������Կ���ִ�СΪ64
 * inputKey����ʼ����Կ
 * keys�������õ���ÿ����Կ
 */
void setSimonKeys64 ( unsigned long long * inputKey, unsigned long long * keys ) {

	/*
	 * �㷨�еĳ���c����СΪ2^n - 4������n���ֵĳ��ȣ���SIMON_WORD_SIZE
	 * ת��Ϊ�����ƣ��������λΪ0������λȫΪ1
	 */
	unsigned long long c = 0xfffffffffffffffc;

	int i;
	for ( i = 0; i < SIMON_KEY_WORDS; i++ )  {
		keys[i] = inputKey[i];
	}

	/*
	 * �������ֵ���Կ
	 * ������������������Zji�����Ϊ1������λ�����޸ģ����򲻱�
	 */
	if ( SIMON_KEY_WORDS == 2 ) {
		for ( i = 0; i < SIMON_ROUNDS-SIMON_KEY_WORDS; i++ ) {
			/*
			 * û��ѭ����λ������ͨ���ֱ�����������λ�ٻ�õ�
			 */
			keys[i+SIMON_KEY_WORDS] = c ^ keys[i] ^ 
									((keys[i+1]>>3) | (keys[i+1]<<(SIMON_WORD_SIZE-3))) ^ 
									((keys[i+1]>>4) | (keys[i+1]<<(SIMON_WORD_SIZE-4)));
			/*
			 * �����z[j][i]����������㣬��SIMON_KEY_WORDSȷ��ʱ��jҲ��ȷ����
			 */
			if ( z[SIMON_SEQUENCE_NUMBER][i%62] == 1 ) {
				keys[i+SIMON_KEY_WORDS] ^= 0x1 ;
			}
		}
	} else if ( SIMON_KEY_WORDS == 3 ) {
		for ( i = 0; i < SIMON_ROUNDS-SIMON_KEY_WORDS; i++ ) {
			keys[i+SIMON_KEY_WORDS] = c ^ keys[i] ^
							((keys[i+2]>>3) | (keys[i+2]<<(SIMON_WORD_SIZE-3))) ^
							((keys[i+2]>>4) | (keys[i+2]<<(SIMON_WORD_SIZE-4)));
			// SIMON_WORD_SIZEΪ32ʱ������SIMON_KEY_WORDSΪ3����4�����ڶ���62
			if ( z[SIMON_SEQUENCE_NUMBER][i%62] == 1 ) {
				keys[i+SIMON_KEY_WORDS] ^=  0x1;
			}
		}
	} else if ( SIMON_KEY_WORDS == 4 ) {
		// int cycle = (SIMON_SEQUENCE_NUMBER == 0 || SIMON_SEQUENCE_NUMBER == 1)?31:62;
		unsigned long long temp = 0x0123456789abcdef;
		for ( i = 0; i < SIMON_ROUNDS-SIMON_KEY_WORDS; i++ ) {
			temp = ((keys[i+3]>>3) | (keys[i+3]<<(SIMON_WORD_SIZE-3))) ^ keys[i+1];
			keys[i+SIMON_KEY_WORDS] = c ^ keys[i] ^ temp ^ ((temp>>1) | (temp<<(SIMON_WORD_SIZE-1)));
			if ( z[SIMON_SEQUENCE_NUMBER][i%62] == 1 ) {
				keys[i+SIMON_KEY_WORDS] ^=  0x1;
			}
		}
	}

}

/*
 * Speck��������Կ
 * inputKey����ʼ����Կ
 * keys�������õ���ÿ����Կ
 */
void setSpeckKeys32 ( unsigned int * inputKey, unsigned int * keys ) {
	int i;
	/*
	 * inputKey = (L(m-2), ...,L(0),keys(0));��L��������keys��m-2(��SPECK_KEY_WORDS-2)
	 * keys��������SPECK_ROUNDS����L������ΪSPECK_ROUNDS+SPECK_KEY_WORDS-2
	 */
	unsigned int * L = new unsigned int[SPECK_ROUNDS+SPECK_KEY_WORDS-2];

	/*
	 * �ҵ�keys[0]����ʼ�±�
	 */
	keys[0] = inputKey[SPECK_KEY_WORDS-1];

	/*
	 * ����L��ǰSPECK_KEY_WORDS-1��
	 */
	for ( i = 0; i <= SPECK_KEY_WORDS-2; i++ ) {
		L[SPECK_KEY_WORDS-2-i] = inputKey[i];
	}

	/*
	 * ������Կ
	 */
	for ( i = 0; i < SPECK_ROUNDS-1; i++ ) {
		L[i+SPECK_KEY_WORDS-1] = ( keys[i] + ((L[i]>>SPECK_ROT_A)|(L[i]<<(SPECK_WORD_SIZE-SPECK_ROT_A))) ) ^ ((unsigned int)i);
		keys[i+1] = ( (keys[i]<<SPECK_ROT_B)|(keys[i]>>(SPECK_WORD_SIZE-SPECK_ROT_B)) ) ^ L[i+SPECK_KEY_WORDS-1];
	}

	/*
	 * �ͷ��ڴ�
	 */
	delete L;

}

/*
 * Speck��������Կ:64λ
 * inputKey����ʼ����Կ
 * keys�������õ���ÿ����Կ
 */
void setSpeckKeys64 ( unsigned long long * inputKey, unsigned long long * keys ) {
	int i;
	/*
	 * inputKey = (L(m-2), ...,L(0),keys(0));��L��������keys��m-2(��SPECK_KEY_WORDS-2)
	 * keys��������SPECK_ROUNDS����L������ΪSPECK_ROUNDS+SPECK_KEY_WORDS-2
	 */
	unsigned long long * L = new unsigned long long[SPECK_ROUNDS+SPECK_KEY_WORDS-2];

	/*
	 * �ҵ�keys[0]����ʼ�±�
	 */
	keys[0] = inputKey[SPECK_KEY_WORDS-1];

	/*
	 * ����L��ǰSPECK_KEY_WORDS-1��
	 */
	for ( i = 0; i <= SPECK_KEY_WORDS-2; i++ ) {
		L[SPECK_KEY_WORDS-2-i] = inputKey[i];
	}

	/*
	 * ������Կ
	 */
	for ( i = 0; i < SPECK_ROUNDS-1; i++ ) {
		L[i+SPECK_KEY_WORDS-1] = ( keys[i] + ((L[i]>>SPECK_ROT_A)|(L[i]<<(SPECK_WORD_SIZE-SPECK_ROT_A))) ) ^ ((unsigned int)i);
		keys[i+1] = ( (keys[i]<<SPECK_ROT_B)|(keys[i]>>(SPECK_WORD_SIZE-SPECK_ROT_B)) ) ^ L[i+SPECK_KEY_WORDS-1];
	}

	/*
	 * �ͷ��ڴ�
	 */
	delete L;
}