#ifndef BIGDEC_H
#define BIGDEC_H
#include <string.h>
#include <stdlib.h>

typedef char* BigDec;
BigDec BigDec_add(BigDec number1, BigDec number2);
BigDec BigDec_sub(BigDec number1, BigDec number2);
BigDec BigDec_mul(BigDec number1, BigDec number2);
BigDec BigDec_div(BigDec number1, BigDec number2);
BigDec BigDec_pow(BigDec base, unsigned long long exp);
BigDec BigDec_tradPow(BigDec base, unsigned long long exp);
BigDec BigDec_powRes(BigDec base, unsigned long long exp);
BigDec BigDec_mod(BigDec number1, BigDec number2);
inline int BigDec_max(BigDec number1, BigDec number2);
static inline BigDec BigDec_delZeroes(BigDec number1, size_t length);
BigDec BigDec_gcd(BigDec number1, BigDec number2);
BigDec BigDec_fgcd(BigDec number1, BigDec number2);
BigDec BigDec_genrfn(BigDec number);
static BigDec BigDec_modularPowerAction(BigDec number, BigDec exp, BigDec mod);
BigDec BigDec_modularPowerAction(BigDec number, BigDec exp, BigDec mod);


inline int BigDec_max(BigDec number1, BigDec number2) { //0-number1<number2 1-number1==number2 2-number1>number2
	
	int i;
	size_t nb1_len,nb2_len;
	
	nb1_len=strlen(number1);
	nb2_len=strlen(number2);
	
	if(nb1_len>nb2_len)
		return 2;
	else if(nb1_len<nb2_len)
		return 0;
		
	for(i=0;i<nb1_len;i++)
	{
		if(number1[i]>number2[i])
			return 2;
		
		if(number1[i]<number2[i])
			return 0;
	}
	
	return 1;
}

static inline BigDec BigDec_delZeroes(BigDec number, size_t length) {
	
	BigDec hlptr;
	int i;
	
	if(number[0]=='0')
	{	
		i=0;
		while(number[i]=='0'&&i!=length-1) i++;
		hlptr=(BigDec)malloc((length+1-i)*sizeof(char));
		memcpy(hlptr,number+i,length+1-i); 
		free(number);
		number=hlptr;
	}
	
	if(strcmp(number,"")==0)
	{
		free(number);
		number=(C_BigDec)malloc(2*sizeof(char));
		number[0]='0';
		number[1]='\0';
	}
	
	return number;
}
#endif
