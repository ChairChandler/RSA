#include <windows.h>
#include <pthread.h>
#include <stdio.h>
#define DIV 1048576
#include "BigDec_secure.h"
static char secure=0;

static void* BigDec_secureThread(void *arg);

static void* BigDec_secureThread(void *arg) {
	
	MEMORYSTATUSEX statex;
	
	do
	{
	GlobalMemoryStatusEx(&statex);
	statex.dwLength=sizeof(statex);
	GlobalMemoryStatusEx(&statex);
	}while(statex.ullAvailPhys/DIV/1000>1&&secure==1);
	
	if(secure==1)
	{
	fprintf(stderr,"\n[BigDec_secureThread] Memory overflow protection.");
	exit(EXIT_FAILURE);
	}
}

int BigDec_secure(void) {
	
	if(secure==0)
	{
		secure=1;
		pthread_t secureThread;
		pthread_create(&secureThread,NULL,BigDec_secureThread,NULL);
		return 1;
	}
	else
	{
		secure=0;
		return 0;
	}
	
}
