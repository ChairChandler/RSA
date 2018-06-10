#include <stdio.h>
#include <string.h>
#include "BigDec.h"

BigDec BigDec_div(BigDec number1, BigDec number2) {
	
	BigDec numberhlp,number3,x,q,pop,buff[2];
	size_t nb1_len,nb2_len,nb3_len;
	char ans,negnmb1,negnmb2;
	register int i,j,z,m;
	
	if(number1[0]=='-')
	{
		negnmb1=1;
		number1=number1+1;		
	}
	else
		negnmb1=0;
		
	if(number2[0]=='-')
	{
		negnmb2=1;
		number2=number2+1;
	}
	else
		negnmb2=0;


	ans=BigDec_max(number1,number2);
	if(ans==0)
	{
		numberhlp=(BigDec)malloc(2*sizeof(char));
		numberhlp[0]='0';
		numberhlp[1]='\0';
		return numberhlp;
	}
	else if(ans==1&&!negnmb1^negnmb2)
	{
		numberhlp=(BigDec)malloc(2*sizeof(char));
		numberhlp[0]='1';
		numberhlp[1]='\0';
		return numberhlp;
	}
	else if(ans==1)
	{
		numberhlp=(BigDec)malloc(3*sizeof(char));
		numberhlp[0]='-';
		numberhlp[1]='1';
		numberhlp[2]='\0';
		return numberhlp;
	}
	
	nb1_len=strlen(number1);
	nb2_len=strlen(number2);
	nb3_len=nb1_len;
	
	numberhlp=(BigDec)malloc((nb2_len+1)*sizeof(char));
	number3=(BigDec)malloc((nb3_len+1)*sizeof(char));
	
	memset(number3,'0',nb3_len);
	number3[nb3_len]='\0';
	
	for(i=0,j=0;i<nb1_len;i++)
	{	
		numberhlp[j]=number1[i];
		numberhlp[j+1]='\0';	
		
		if(j+1<nb2_len)
		{	
		number3[i]='0';
		j++;
		continue;
		}
			
		if(BigDec_max(numberhlp,number2)==0)
		{
		number3[i]='0';
		j++;
		continue;
		}
		
		for(z=1;z<=9;z++)
		{
			numberhlp=BigDec_delZeroes(numberhlp,strlen(numberhlp));
			sprintf(buff,"%d",z);
			q=BigDec_mul(number2,buff);
			if(BigDec_max(numberhlp,q)==1)
			{
				number3[i]=z+'0';
				j=0;
				break;
			}
			else if(z==9&&BigDec_max(numberhlp,q)==2)
			{
				number3[i]=z+'0';
				q=BigDec_sub(numberhlp,q);
				memcpy(numberhlp,q,strlen(q));
				j=strlen(q);
			}
			else if(z>1&&BigDec_max(numberhlp,pop)==2&&BigDec_max(numberhlp,q)==0)
			{
				number3[i]=(z-1)+'0';
				q=BigDec_sub(numberhlp,pop);
				memcpy(numberhlp,q,strlen(q));
				j=strlen(q);
				break;	
			}
			pop=q;
		}
		
	}
	
	number3=BigDec_delZeroes(number3,nb3_len);
	
	if(negnmb1^negnmb2)
	{
		number1=(BigDec)malloc((strlen(number3)+2)*sizeof(char));
		memcpy(number1+1,number3,strlen(number3)+1);
		number1[0]='-';
		number3=number1;
	}
	
	return number3;
	
}

BigDec BigDec_add(BigDec number1, BigDec number2) {
	
	BigDec ptrhlp,number3,fixnumber3;
	char eq,negnmb;
	int i,j;
	negnmb=0;
	
		if(number1[0]=='-'&&number2[0]=='-')
		{
			number1=number1+1;
			number2=number2+1;
			negnmb=1;
		}
		else if(number1[0]=='-')
		{
			number1=number1+1;
			return BigDec_sub(number2, number1);
		}
		else if(number2[0]=='-')
		{
			number2=number2+1;
			return BigDec_sub(number1, number2);
		}
	
	size_t nb1_len,nb2_len;
	nb1_len=strlen(number1);
	nb2_len=strlen(number2);
	
	if(nb1_len<nb2_len)
	{		
		ptrhlp=number1;
		number1=number2;
		number2=ptrhlp;
		
		nb1_len^=nb2_len;
		nb2_len^=nb1_len;
		nb1_len^=nb2_len;
	}
	
	number3=(BigDec)malloc((nb1_len+2)*sizeof(char));
	number3[nb1_len+1]='\0';
	eq=0;

	for(i=nb1_len-1,j=nb2_len-1;i>=0;i--)
	{
		if(j>=0)
		{
		eq+=(number1[i]-'0')+(number2[j]-'0');
		j--;
		}
		else
		eq+=number1[i]-'0';
	
		number3[i+1]=eq%10+'0';
		eq/=10;
	}
	
	number3[0]=eq+'0';
	
	number3=BigDec_delZeroes(number3,nb1_len+2);
	
	if(negnmb)
	{
		number1=(BigDec)malloc((strlen(number3)+2)*sizeof(char));
		memcpy(number1+1,number3,strlen(number3)+1);
		number1[0]='-';
		number3=number1;
	}
	return number3;
}

BigDec BigDec_sub(BigDec number1, BigDec number2) {

	BigDec ptrhlp,number3;
	size_t nb1_len,nb2_len,nb3_len;
	register int i,j,z;
	char negnmb,eq,rest,negnmb1,negnmb2;
	
	negnmb=0;
	
	if(number1[0]=='-')
	{
		negnmb1=1;
		number1=number1+1;
	}
	else
		negnmb1=0;
	
	if(number2[0]=='-')
	{
		negnmb2=1;
		number2=number2+1;
	}
	else
		negnmb2=0;
		
	if(negnmb1==0&&negnmb2==1)
		return BigDec_add(number1,number2);
	else if(negnmb1==1&&negnmb2==1)
	{
		ptrhlp=number1;
		number1=number2;
		number2=ptrhlp;
	}
	else if(negnmb1==1&&negnmb2==0)
	{
		ptrhlp=BigDec_add(number1,number2);
		number1=(BigDec)malloc((strlen(ptrhlp)+2)*sizeof(char));
		memcpy(number1+1,ptrhlp,strlen(ptrhlp)+1);
		number1[0]='-';
		return number1;		
	}

	nb1_len=strlen(number1);
	nb2_len=strlen(number2);
	
	if(nb1_len<nb2_len)
	{
		ptrhlp=number1;
		number1=number2;
		number2=ptrhlp;
		
		nb1_len^=nb2_len;
		nb2_len^=nb1_len;
		nb1_len^=nb2_len;
		negnmb=1;
	}
	else if(nb1_len==nb2_len)
	{
		i=0;
		while(number1[i]!='\0')
		{
			if(number1[i]>number2[i])
			break;
			
			if(number1[i]<number2[i])
			{
			ptrhlp=number1;
			number1=number2;
			number2=ptrhlp;
			
			nb1_len^=nb2_len;
			nb2_len^=nb1_len;
			nb1_len^=nb2_len;
			negnmb=1;
			break;
			}
			
			i++;
		}
	}


		number3=(BigDec)malloc((nb1_len+1)*sizeof(char));
		number3[nb1_len]='\0';
		z=nb1_len-1;

	rest=0;
	for(i=nb1_len-1,j=nb2_len-1;i>=0;i--,z--)
	{	
		if(j>=0)
		{
		eq=(number1[i]-'0')-(number2[j]-'0')+rest;
		j--;
		}
		else
		eq=number1[i]-'0'+rest;
		
		rest=0;
		
		if(eq<0)
		{
		eq+=10;
		rest--;
		}
		
		number3[z]=eq+'0';
	}
	
	nb3_len=strlen(number3);
	number3=BigDec_delZeroes(number3,nb3_len);
	
	if(negnmb)
	{
		number1=(BigDec)malloc((strlen(number3)+2)*sizeof(char));
		memcpy(number1+1,number3,strlen(number3)+1);
		number1[0]='-';
		number3=number1;
	}
	
	return number3;
}

BigDec BigDec_tradPow(BigDec base, unsigned long long exp) {
	
	BigDec sum,hlptr;
	int i,j;
	i=2;
	
	hlptr=(BigDec)malloc((strlen(base)+1)*sizeof(char));
	memcpy(hlptr,base,strlen(base)+1);
	
	while(i<=exp)
	{
		sum=BigDec_mul(hlptr,hlptr);
		free(hlptr);
		hlptr=sum;
		i*=2;
	}
	
	if(i>exp)
	i/=2;
	
	for(j=i;j<exp;j++)
		sum=BigDec_mul(sum,base);
	
	return sum;
	
}

BigDec BigDec_powRes(BigDec base, unsigned long long exp) {
	
	if(exp==0)
	{
		BigDec a;
		a=(BigDec)malloc(2*sizeof(char));
		a[0]='1';
		a[1]='\0';
		return a;
	}
	else if(exp%2==0)
	{
		BigDec a,b;
		size_t length;
		a=BigDec_powRes(base,exp/2);	
		b=BigDec_mul(a,a);
		free(a);
		return b;
	}
	else
	{
		BigDec a,b;
		size_t length;
		a=BigDec_powRes(base,exp-1);	
		b=BigDec_mul(a,base);
		free(a);
		return b;
	}
		
}

BigDec BigDec_pow(BigDec base, unsigned long long exp) {
		
	BigDec numberhlp;
	
	if(base[0]=='-')
	{
		int baseLeng;
		baseLeng=strlen(base);
		numberhlp=(BigDec)malloc(baseLeng*sizeof(char));
		memcpy(numberhlp,base+1,baseLeng);
		base=numberhlp;
		numberhlp=BigDec_powRes(base,exp);
		free(base);
		baseLeng=strlen(numberhlp);
		
		if(exp%2==1)
		baseLeng++;
		
		base=(BigDec)malloc(baseLeng*sizeof(char));
		
		if(exp%2==1)
		{
			memcpy(base+1,numberhlp,baseLeng);
			base[0]='-';
		}
		else
			memcpy(base,numberhlp,baseLeng);
		
		return base;
	}
	else
		return BigDec_powRes(base,exp);
}

BigDec BigDec_mul(BigDec number1, BigDec number2) {
	
	BigDec number3,ptrhlp;
	size_t nb1_len,nb2_len,nb3_len,lastNr1,lastNr2,lastNr3;
	int i,j,eq;
	char negnmb;
	
	nb1_len=strlen(number1);
	nb2_len=strlen(number2);
	nb3_len=nb1_len+nb2_len;
	
	negnmb=0;
	if(number1[0]=='-'&&number2[0]=='-')
	{
		number1=number1+1;
		nb1_len--;
			
		number2=number2+1;	
		nb2_len--;
	}
	else if(number1[0]=='-')
	{
		number1=number1+1;
		nb1_len--;
		negnmb=1;
	}
	else if(number2[0]=='-')
	{
		number2=number2+1;
		nb2_len--;
		negnmb=1;
	}
	
	if(strlen(number1)<strlen(number2))//number1>=number2
	{
		ptrhlp=number1;
		number1=number2;
		number2=ptrhlp;
		
		nb1_len^=nb2_len;
		nb2_len^=nb1_len;
		nb1_len^=nb2_len;
	}
	
	number3=(BigDec)malloc((nb3_len+1+negnmb)*sizeof(char));
	number3[nb3_len+negnmb]='\0';
	
	for(i=0;i<nb3_len;i++)
	number3[i]='0';
	
	lastNr1=nb1_len-1;
	lastNr2=nb2_len-1;
	lastNr3=nb3_len-1;

	for(i=lastNr2;i>=0;i--)
	{
		eq=0;
		for(j=lastNr1;j>=0;j--)
		{
			eq=eq+(number2[i]-'0')*(number1[j]-'0');
			number3[lastNr3-(lastNr2-i)-(lastNr1-j)]+=eq%10;
			
			eq/=10;
			
			if(number3[lastNr3-(lastNr2-i)-(lastNr1-j)]-'0'>9)
			{
				eq+=(number3[lastNr3-(lastNr2-i)-(lastNr1-j)]-'0')/10;
				number3[lastNr3-(lastNr2-i)-(lastNr1-j)]=(number3[lastNr3-(lastNr2-i)-(lastNr1-j)]-'0')%10+'0';
			}
		}	
		
		if(eq)
		number3[lastNr3-(lastNr2-i)-(lastNr1-j)]+=eq;
	}
	
	number3=BigDec_delZeroes(number3,nb3_len);

	if(negnmb)
	{
		if(number3[0]=='0')
		number3[0]='-';
		else
		{
			number1=(BigDec)malloc((nb3_len+2)*sizeof(char));
			memcpy(number1+1,number3,nb3_len+1);
			free(number3);
			number1[0]='-';
			number3=number1;
		}
	}

	return number3;	
}

BigDec BigDec_mod(BigDec number1, BigDec number2) {
	
	char ans;
	BigDec rem,res,pom;
	size_t nmb1_len;
	ans=BigDec_max(number1,number2);
	
	if(ans==0)
	{
		nmb1_len=strlen(number1);
		rem=(BigDec)malloc((nmb1_len+1)*sizeof(char));
		memcpy(rem,number1,nmb1_len+1);
		return rem;
	}
	else if(ans==1)
	{
		rem=(BigDec)malloc(2*sizeof(char));
		rem[0]='0';
		rem[1]='\0';
		return rem;	
	}
	
	res=BigDec_div(number1,number2);
	pom=BigDec_mul(res,number2);
	rem=BigDec_sub(number1,pom);
	
	free(res);
	free(pom);
	
	return rem;
	
}

BigDec BigDec_gcd(BigDec number1, BigDec number2) {
	
	BigDec number3,helpPtr;
	char ans;
	
	if(number1[0]=='-')
	number1=number1+1;
	
	if(number2[0]=='-')
	number2=number2+1;

	ans=BigDec_max(number1,number2);
	
	while(ans!=1)//n1!=n2
	{
		if(ans==2)//n1>n2
		{
		helpPtr=number1;
		number1=BigDec_sub(number1,number2);
		free(helpPtr);
		}
		else if(ans==0)//n1<n2
		{
		helpPtr=number2;
		number2=BigDec_sub(number2,number1);
		free(helpPtr);
		}
		ans=BigDec_max(number1,number2);
	}
	
	return number1;
}

BigDec BigDec_fgcd(BigDec number1, BigDec number2) {
	
	BigDec a,b,c;

	a=(BigDec)malloc((strlen(number1)+1) * sizeof(char));
	b=(BigDec)malloc((strlen(number2)+1) * sizeof(char));
	c=NULL;
	
	memcpy(a,number1,strlen(number1)+1);
	memcpy(b,number2,strlen(number2)+1);
	
	while(strcmp(b,"0"))
	{
		c=BigDec_mod(a,b);
		free(a);
		a=b;
		b=c;
	}
	return a;
} 

BigDec BigDec_genrfn(BigDec number) {

	BigDec nmb,gcd,helpPtr;
	gcd=NULL;

	nmb=(BigDec)malloc(2*sizeof(char));
	nmb[0]='3';
	nmb[1]='\0';

	while(strcmp(number,nmb)!=0)
	{	
		if(gcd!=NULL)
		free(gcd);
		
		gcd=BigDec_fgcd(number,nmb);
		
		if(strcmp(gcd,"1")==0)
			break;
			
		helpPtr=nmb;
		nmb=BigDec_add(nmb,"1");
		free(helpPtr);
	}	
	free(gcd);
	return nmb;
}

BigDec BigDec_modularPower(BigDec number, BigDec exp, BigDec mod) {
	
	return	BigDec_modularPowerAction(number, exp, mod);

}

BigDec BigDec_modularPowerAction(BigDec number, BigDec exp, BigDec mod) {
	
	if(strcmp(exp,"1")==0)
	{
		return number;
	}
	else if(strcmp(BigDec_mod(exp,"2"),"0")==0)
	{
		BigDec aux,tmp;
		
		tmp=BigDec_div(exp,"2");
		aux=BigDec_modularPowerAction(number, tmp, mod);
	
		free(tmp);
	
		tmp=BigDec_mul(aux,aux);
		
		if(strcmp(aux,number))
		free(aux);
		
		aux=tmp;
		tmp=BigDec_mod(tmp,mod);
		free(aux);
		
		return tmp;
	}
	else
	{
		BigDec aux,tmp;
		
		tmp=BigDec_div(exp,"2");
		aux=BigDec_modularPowerAction(number, tmp, mod);
		
		free(tmp);
		
		tmp=aux;
		aux=BigDec_mul(aux,aux);
		if(strcmp(tmp,number))
		free(tmp);
	
		tmp=aux;
		aux=BigDec_mul(aux,number);
		free(tmp);

		tmp=aux;
		aux=BigDec_mod(aux,mod);
		free(tmp);

		return aux;
	}
	
}
