
#include"enDecryption.h"
#include"const.h"

/*
 * Simon����
 * plainText������
 * keys����Կ
 */
void encryptionSimon ( int * plainText, int ** keys ) {
	/*
	 * ������ʱ����
	 */
	int * tempCipher = new int[SIMON_BLOCK_SIZE];

	/*
	 * ����SIMON_ROUNDS�ּ���
	 */
	int i, j;
	for ( i = 0; i < SIMON_ROUNDS; i++ ) {
		for ( j = 0; j < SIMON_WORD_SIZE; j++ ) {
			tempCipher[j+SIMON_WORD_SIZE] = plainText[j];
		}
		for ( j = 0; j < SIMON_WORD_SIZE; j++ ) {
			/*
			 * ����1λ�����λ��Ӧ����ֵ��1����(x+1)%32
			 * ������������������ٶ�2ȡ����Ϊ������������������
			 */
			tempCipher[j] = ((plainText[(j+1)%SIMON_WORD_SIZE] + plainText[(j+8)%SIMON_WORD_SIZE])/2 +
							plainText[j+SIMON_WORD_SIZE] +
							plainText[(j+2)%SIMON_WORD_SIZE] +
							keys[i][j]) % 2;
		}

		/*
		 * ���½����ܵĽ�����Ƶ�plainText��
		 */
		for ( j = 0; j < SIMON_BLOCK_SIZE; j++ ) {
			plainText[j] = tempCipher[j];
		}
	}
	
	/*
	 * �ͷ��ڴ�
	 */
	delete tempCipher;
}

/*
 * Simon����
 * cipherText������
 * keys����Կ
 */
void decryptionSimon ( int * cipherText, int ** keys ) {
	/*
	 * ������ʱ���ܵ�����
	 */
	int * tempPlain = new int[SIMON_BLOCK_SIZE];       

	int i, j;
	/*
	 * ���������˳���෴�������һ�ֿ�ʼ
	 */
	for ( i = SIMON_ROUNDS-1; i >= 0; i-- ) {
		for ( j = 0; j < SIMON_WORD_SIZE; j++ ) {
			tempPlain[j] = cipherText[j+SIMON_WORD_SIZE];
		}
		for ( j = SIMON_WORD_SIZE; j < SIMON_BLOCK_SIZE; j++ ) {
			tempPlain[j] = ((cipherText[(j+1)%SIMON_WORD_SIZE+SIMON_WORD_SIZE] + cipherText[(j+8)%SIMON_WORD_SIZE+SIMON_WORD_SIZE])/2 +
							cipherText[(j+2)%SIMON_WORD_SIZE+SIMON_WORD_SIZE] +
							cipherText[j-SIMON_WORD_SIZE] +
							keys[i][j-SIMON_WORD_SIZE]) % 2; 
		}
		for ( j = 0; j < SIMON_BLOCK_SIZE; j++ ) {
			cipherText[j] = tempPlain[j];
		}
	}
	delete tempPlain;
}

/*
 * Speck����
 * plainText������
 * keys����Կ
 */
void encryptionSpeck ( int * plainText, int ** keys ) {
	/*
	 * ����SPECK_ROUNDS�ּ���
	 */
	int i, j;
	int * tempCipher = new int[SPECK_BLOCK_SIZE];        // ������ʱ����
	int * additionModulo = new int[SPECK_WORD_SIZE];     // ��ʱ����ģ2^n�ӷ�������
	int jinwei;

	for ( i = 0; i < SPECK_ROUNDS; i++ ) {
		jinwei = 0;
		for ( j = SPECK_WORD_SIZE-1; j >= 0; j--) {
			additionModulo[j] = ( plainText[(j-SPECK_ROT_A+SPECK_WORD_SIZE)%SPECK_WORD_SIZE] + plainText[j+SPECK_WORD_SIZE] + jinwei ) % 2;
			jinwei = ( plainText[(j-SPECK_ROT_A+SPECK_WORD_SIZE)%SPECK_WORD_SIZE] + plainText[j+SPECK_WORD_SIZE] + jinwei ) / 2;
		}

		/*
		 * ǰSPECK_WORD_SIZEλ
		 */
		for ( j = 0; j < SPECK_WORD_SIZE; j++ ) {
			tempCipher[j] = ( additionModulo[j] + keys[i][j]) % 2;
		}

		/*
		 * ��SPECK_WORD_SIZEλ
		 */
		for ( j = SPECK_WORD_SIZE; j < SPECK_BLOCK_SIZE; j++ ) {
			tempCipher[j] = ( additionModulo[j-SPECK_WORD_SIZE] + 
							keys[i][j-SPECK_WORD_SIZE] + 
							plainText[(j+SPECK_ROT_B)%SPECK_WORD_SIZE+SPECK_WORD_SIZE]) % 2;
		}

		/*
		 * ���½����ܵĽ�����Ƶ�plainText��
		 */
		for ( j = 0; j < SPECK_BLOCK_SIZE; j++ ) {
			plainText[j] = tempCipher[j];
		}
	}
	delete additionModulo;
	delete tempCipher;
}

/*
 * Speck����
 * cipherText������
 * keys����Կ
 */
void decryptionSpeck ( int * cipherText, int ** keys ) {
	/*
	 * ������ʱ����
	 */
	int * tempCipher = new int[SPECK_BLOCK_SIZE];        

	/*
	 * ����SPECK_ROUNDS�ּ���
	 */
	int i, j;
	int * first  = new int[SPECK_WORD_SIZE];
	int * second = new int[SPECK_WORD_SIZE];
	int jiewei;
	for ( i = SPECK_ROUNDS-1; i >= 0; i-- ) {
		for ( j = 0; j < SPECK_WORD_SIZE; j++ ) {
			first[j] = (cipherText[j] + keys[i][j] ) % 2;
		}
		for ( j = 0; j < SPECK_WORD_SIZE; j++ ) {
			second[j] = (cipherText[(j-SPECK_ROT_B+SPECK_WORD_SIZE)%SPECK_WORD_SIZE] +
						cipherText[(j-SPECK_ROT_B+SPECK_WORD_SIZE)%SPECK_WORD_SIZE+SPECK_WORD_SIZE] ) % 2;
		}

		/*
		 * �����ǰSPECK_WORD_SIZEλ
		 * 
		 * ����������ӷ���һ���������������ڽ���ѭ������SPECK_ROT_Aλ����������λ�ټ�
		 */
		jiewei = 0;
		for ( j = SPECK_WORD_SIZE-1; j >= 0; j-- ) {
			// ��λ���
			tempCipher[j] = first[j]-jiewei-second[j];
			if ( tempCipher[j] < 0 ) {
				tempCipher[j] += 2;
				jiewei = 1;
			} else {
				jiewei = 0;
			}
		}

		/*
		 * ���½����ܵĽ�����Ƶ�plainText��
		 */
		for ( j = 0; j < SPECK_WORD_SIZE; j++ ) {
			cipherText[j] = tempCipher[(j+SPECK_ROT_A)%SPECK_WORD_SIZE];
		}
		for ( j = SPECK_WORD_SIZE; j < SPECK_BLOCK_SIZE; j++ ) {
			cipherText[j] = second[j-SPECK_WORD_SIZE];
		}
	}
	
	/*
	 * �ͷ��ڴ�
	 */
	delete second;
	delete first;
	delete tempCipher;
}