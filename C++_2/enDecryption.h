
#include<iostream>

/*
 * Simon���ܣ��ִ�СΪ32
 * plainText������
 * keys����Կ
 */
void encryptionSimon32 ( unsigned int * plainText, unsigned int * keys );

/*
 * Simon���ܣ��ִ�СΪ32
 * cipherText������
 * keys����Կ
 */
void decryptionSimon32 ( unsigned int * cipherText, unsigned int * keys );

/*
 * Simon���ܣ��ִ�СΪ32
 * plainText������
 * keys����Կ
 */
void encryptionSimon64 ( unsigned long long * plainText, unsigned long long * keys );

/*
 * Simon���ܣ��ִ�СΪ64
 * cipherText������
 * keys����Կ
 */
void decryptionSimon64 ( unsigned long long * cipherText, unsigned long long * keys );


/*
 * Speck���ܣ���32λ
 * plainText������
 * keys����Կ
 */
void encryptionSpeck32 ( unsigned int * plainText, unsigned int * keys );

/*
 * Speck���ܣ���32λ
 * cipherText������
 * keys����Կ
 */
void decryptionSpeck32 ( unsigned int * cipherText, unsigned int * keys );

/*
 * Speck����
 * plainText������
 * keys����Կ
 */
void encryptionSpeck64 ( unsigned long long * plainText, unsigned long long * keys );

/*
 * Speck����
 * cipherText������
 * keys����Կ
 */
void decryptionSpeck64 ( unsigned long long * cipherText, unsigned long long * keys );
