#ifndef RSA_H
#define RSA_H
#define MAX_NUM 4
#define EQUALS 0
#define MAX_DIGITS 2048
#define MAX_SIZE 200
#define MIN_SIZE 3
#include "BigDec.h"

int RSA_secure_ON;

typedef struct RSA_Keys_ {
	
	struct {
		
		BigDec e,n;
		
	} Public;
	
	struct {
		
		BigDec d,n;
		
	} Private;
	
} RSA_Keys;


RSA_Keys RSA_generateKeys(size_t length);
BigDec* RSA_encrypt(char *number, BigDec e, BigDec n);
char* RSA_decrypt(BigDec *number, BigDec d, BigDec n, size_t length);

static void RSA_publicKey(BigDec *eAdress, BigDec *nAdress, BigDec *xAdress, size_t length);
static void RSA_privateKey(BigDec *dAdress, BigDec x, BigDec e);
static BigDec RSA_modInv(BigDec a, BigDec b);
static BigDec RSA_checkPrime(BigDec number);
static BigDec RSA_numGenerate(size_t length);
static BigDec RSA_relPrimeNum(BigDec number);
#endif
