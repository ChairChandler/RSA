#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include "BigDec.h"
#include "RSA.h"

static void RSA_publicKey(BigDec *eAdress, BigDec *nAdress, BigDec *xAdress, size_t length);
static void RSA_privateKey(BigDec *dAdress, BigDec x, BigDec e);
static BigDec RSA_modInv(BigDec a, BigDec b);
static BigDec RSA_checkPrime(BigDec number);
static BigDec RSA_numGenerate(size_t length);
static BigDec RSA_relPrimeNum(BigDec number);


static BigDec RSA_modInv(BigDec a, BigDec b) {

	uint16_t nm1_len,nm2_len;
	BigDec helpPtr,u,w,x,z,q;
	
	nm1_len=strlen(a);
	nm2_len=strlen(b);
	
	w=(BigDec)malloc((nm1_len+1)*sizeof(char));
	z=(BigDec)malloc((nm2_len+1)*sizeof(char));
	
	memcpy(w,a,nm1_len+1);
	memcpy(z,b,nm2_len+1);
	
	u=(BigDec)malloc(2*sizeof(char));
	x=(BigDec)malloc(2*sizeof(char));
	u[0]='1';
	u[1]='\0';
	x[0]='0';
	x[1]='\0';
	q=NULL;
	while(strcmp(w,"0")!=EQUALS)
	{
		if(BigDec_max(w,z)==0)
		{
			helpPtr=x;
			x=u;
			u=helpPtr;
			
			helpPtr=w;
			w=z;
			z=helpPtr;
		}
		
		if(q!=NULL)
		free(q);
		q=BigDec_div(w,z);
		
		helpPtr=u;
		u=BigDec_sub(u,BigDec_mul(q,x));
		free(helpPtr);
		
		helpPtr=w;
		w=BigDec_sub(w,BigDec_mul(q,z));
		free(helpPtr);
	}
	
	if(strcmp(z,"1")!=0)
	{
		helpPtr=(BigDec)malloc(sizeof(char));
		helpPtr[0]='\0';
		free(q);
		free(u);
		free(w);
		free(x);
		free(z);
		return helpPtr;
	}
	
	if(x[0]=='-')
	x=BigDec_add(x,b);
	
	free(q);
	free(u);
	free(w);
	free(z);
	
	return x;
}

static BigDec RSA_checkPrime(BigDec number) {
	
	FILE *pPipe;
	char buff[MAX_DIGITS+1],*arg;
	
	arg=(char*)malloc((10+strlen(number))*sizeof(char));
	memset(arg,'\0',10+strlen(number));
	strncat(arg,"Prime.py ",9);
	strncat(arg,number,strlen(number));
	
	pPipe=popen(arg,"r");
	fgets(buff,MAX_DIGITS,pPipe);
	fgets(buff,MAX_DIGITS,pPipe);
	pclose(pPipe);
	
	int i=1;
	while(buff[i]=='-'||(buff[i]>='0'&&buff[i]<='9'))
	i++;
	
	free(number);
	number=(BigDec)malloc((i) * sizeof(char));
	memcpy(number,buff+1,i-1);
	number[i-1]='\0';
	
	free(arg);
	return number;

}

static BigDec RSA_numGenerate(size_t length) {
	
	BigDec number;
	uint16_t i;
	
	number=(BigDec)malloc((length+1)*sizeof(char));
	number[0]=rand()%9+'1';
	
	for(i=1;i<length-1;i++)
	{
		number[i]=rand()%10+'0';
	}
	
	number[i]=(rand()%5)*2+1+'0';
	number[i+1]='\0';
	
	return number;

}

static BigDec RSA_relPrimeNum(BigDec number) {

	BigDec nmb,gcd,aux;
	gcd=NULL;

	nmb=(BigDec)malloc(2*sizeof(char));
	nmb[0]='3';
	nmb[1]='\0';

	while(strcmp(number,nmb)!=EQUALS)
	{	
		if(gcd!=NULL)
		free(gcd);
		
		gcd=BigDec_fgcd(number,nmb);
		
		if(strcmp(gcd,"1")==EQUALS)
			break;
			
		aux=nmb;
		nmb=BigDec_add(nmb,"1");
		free(aux);
	}
	
	if(gcd!=NULL)
	free(gcd);
	
	return nmb;
}

BigDec* RSA_encrypt(char *message, BigDec e, BigDec n) {
	
	BigDec_secure();//ON
	
	BigDec aux,*encMessage,nmb[MAX_NUM];
	uint16_t i,length;
	
	length=strlen(message);
	encMessage=(BigDec*)malloc(length*sizeof(BigDec));
	for(i=0;i<length;i++)
	{
		sprintf(nmb,"%d",message[i]);
		aux=BigDec_modularPower(nmb,e,n);
		encMessage[i]=aux;
	}
	
	BigDec_secure();//OFF
	
	return encMessage;
}

char* RSA_decrypt(BigDec *numbers, BigDec d, BigDec n, size_t length) {
	
	BigDec_secure();//ON
	
	char *decMessage;
	BigDec aux;
	uint16_t i;
	
	decMessage=(char*)malloc((length+1)*sizeof(char));
	
	for(i=0;i<length;i++)
	{
		aux=BigDec_modularPower(numbers[i],d,n);
		decMessage[i]=atoi(aux);
		free(aux);
	}
	decMessage[i]='\0';
	
	BigDec_secure();//OFF
	
	return decMessage;
	
}

static void RSA_publicKey(BigDec *eAdress, BigDec *nAdress, BigDec *xAdress, size_t length) {
	
	BigDec p,q;
	srand(time(NULL));
	
	p=RSA_numGenerate(length);
	q=RSA_numGenerate(length);
	p=RSA_checkPrime(p);
	q=RSA_checkPrime(q);

	(*xAdress)=BigDec_mul(BigDec_sub(p,"1"),BigDec_sub(q,"1"));
	(*nAdress)=BigDec_mul(p,q);
	(*eAdress)=RSA_relPrimeNum(*xAdress);
	
	free(p);
	free(q);
	
}

static void RSA_privateKey(BigDec *dAdress, BigDec x, BigDec e) {
	
	(*dAdress)=RSA_modInv(e,x);
	
}

RSA_Keys RSA_generateKeys(size_t length) {
	
	if(length<MIN_SIZE)
	{
		fprintf(stderr,"[RSA_generateKeys] Keys length have to be over than %d.",MIN_SIZE-1);
		exit(EXIT_FAILURE);
	}
	else if(length>MAX_SIZE)
	{
		fprintf(stderr,"[RSA_generateKeys] Keys length have to be less than %d.",MAX_SIZE+1);
		exit(EXIT_FAILURE);
	}
	
	BigDec e,n,d,x;
	RSA_Keys info;
	
	BigDec_secure();//ON
	
	do
	{
	RSA_publicKey(&e,&n,&x,length);
	RSA_privateKey(&d,x,e);
	}while(e[0]=='-'||n[0]=='-'||x[0]=='-'||d[0]=='-');
	
	info.Public.e=e;
	info.Public.n=n;
	info.Private.d=d;
	info.Private.n=n;
	
	BigDec_secure();//OFF
	
	return info;
}
