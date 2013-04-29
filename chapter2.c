#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <float.h>
#include <ctype.h>

/* exercise 2-3 */
/* Write a program to determine the ranges of char, short, int, and long variables, both signed and unsigned,
 * by printing appropriate values from standard headers and by direct computation. */
static void e_2_1(void)
{
	signed char sc;
	signed int si;
	signed short ss;
	signed long sl;

	unsigned char uc;
	unsigned int ui;
	unsigned short us;
	unsigned long ul;

	float f;
	double db;
	long double ld;

	printf("Get ranges:"
		"1.from standard headers;"
		"2.by direct computation.\n");
	printf("1.\n");
	printf("signed char:%d~%d\n", SCHAR_MIN, SCHAR_MAX);
	printf("signed short:%d~%d\n", SHRT_MIN, SHRT_MAX);
	printf("signed int:%d~%d\n", INT_MIN, INT_MAX);
	printf("signed long:%ld~%ld\n", LONG_MIN, LONG_MAX);

	printf("unsigned char:%u~%u\n", 0, UCHAR_MAX);
	printf("unsigned short:%u~%u\n", 0, USHRT_MAX);
	printf("unsigned int:%u~%u\n", 0, UINT_MAX);
	printf("unsigned long:%u~%lu\n", 0, ULONG_MAX);
	printf("========================================\n");

	printf("2.\n");
	// signed 
	sc = sizeof(sc);
	printf("signed char:%d~%d\n", (signed char)(0x1<<(8*sc-1)), 
			(signed char)((0x1<<(8*sc-1))-1));
	ss = sizeof(ss);
	printf("signed short:%d~%d\n", (signed short)(0x1<<(8*ss-1)), 
			(signed short)((0x1<<(8*ss-1))-1));
	si = sizeof(si);
	printf("signed int:%d~%d\n", (signed int)(0x1<<(8*si-1)), 
			(signed int)((0x1<<(8*si-1))-1));
	sl = sizeof(sl);
	printf("signed long:%ld~%ld\n", (signed long)(0x1<<(8*sl-1)), 
			(signed long)((0x1<<(8*sl-1))-1));

	// unsigned
	uc = sizeof(uc);
	printf("unsigned char:%u~%u\n", 0, (unsigned char)((0x1<<8*uc)-1));
	us = sizeof(us);
	printf("unsigned short:%u~%u\n", 0, (unsigned short)((0x1<<8*us)-1));
	ui = sizeof(ui);
	printf("unsigned int:%u~%u\n", 0, (unsigned int)((0x1<<8*si)-1));
	ul = sizeof(ul);
	//printf("unsigned long:%lu\n", 0UL);
	printf("unsigned long:%lu~%lu\n", 0UL, (unsigned long)((0x1<<8*ul)-1));

	printf("float:%e~%e\n", FLT_MIN, FLT_MAX);
	printf("double:%e~%e\n", DBL_MIN, DBL_MAX);
	printf("long double:%Le~%Le\n", LDBL_MIN, LDBL_MAX);

	// TODO determine ranges of various floats by direct computation

	return;
}

int htoi(const char *nptr)
{
	int i = 0;
	int n = 0;
	int c = 0;
	char s[10] = "";

	if(!nptr){
		printf("err:nptr is NULL\n");
		return n;
	}
	if(*nptr == '0'){
		if(*(nptr+1) != 'x' && *(nptr+1) != 'X'){
			printf("err:invalid hex value,%s\n", nptr);
			return n;
		}
		strncpy(s, nptr+2, sizeof(s)-1);
	}else{
		strncpy(s, nptr, sizeof(s)-1);
	}

	while(s[i]){
		c = s[i];
		if(isxdigit(c)){
		   if(isdigit(c)){
			   n = n*16 + c - '0';
		   }else if(isupper(c)){
			   n = n*16 + c - 'A' + 10;
		   }else{
			   n = n*16 + c - 'a' + 10;
		   }
		}else{
			printf("err:contains invalid char,%c\n", c);
			return n;
		}
		++i;
	}

	printf("right:%s ret %d\n", nptr, n);
	return n;
}

void e_2_3(void)
{
	int i,j,k;
	char s[20];

	/* just to test htoi */
	for(i=0;i<9;i++)
		for(j=1;j<31;++j)
			for(k=2;k<412;++k){
				sprintf(s,"%x%02x%X",i,j,k);
				htoi(s);
			}
}
/* exercise 2-3 */

/* exercise 2-4 */
/* Write an alternative version of squeeze(s1,s2) that deletes each character in s1 that matches any character in
 * the string s2. */
void squeeze(char s1[], char s2[])
{
	int i,j,k;
	int find = 0;

	printf("before squeeze:s1 is %s,", s1);

	for (i = j = 0; s1[i] != '\0'; i++){
		find = 0;
		for(k = 0; s2[k] != '\0'; k++){
			if (s1[i] == s2[k]){
				find = 1;
				break;
			}
		}

		if(!find){
			s1[j++] = s1[i];
		}
	}
	s1[j] = '\0';
	printf(" after squeeze s1 is %s.\n", s1);

	return;
}
/* squeeze: delete all c from s */

void e_2_4(void)
{
	char s[] = "squeeze";
	squeeze(s, "ze");
}
/* exercise 2-4 */

/* exercise 2-5 */
/* Write the function any(s1,s2), which returns the first location in a string s1 where any character from the
 * string s2 occurs, or -1 if s1 contains no characters from s2. */ 
int any(char s1[], char s2[])
{
	int i, j; 

	for(i = 0; s1[i] != '\0'; ++i){
		for(j = 0; s2[j] != '\0'; ++j){
			if(s1[i] == s2[j]){
				break;
			}
		}
		if(s2[j] != '\0')
			break;
	}

	if(s1[i] == '\0'){
		return -1;
	}

	return i;
}

void e_2_5(void)
{
	char s1[] = "squeeze";
	char s2[] = "eu";

	int ret = any(s1, s2);
	if(ret != -1){
		printf("%s contain %c(%d) of %s\n", s1, s1[ret], ret, s2);
	}else{
		printf("%s does not contain char of %s\n", s1, s2);
	}
}
/* exercise 2-5 */

/* exercise 2-6,2-7,2-8 */
/* get n 1s rightmost */
#define GETnONESRIGHTMOST(n) ((1<<(n))-1) /* (~(~0<<(n))) */
/* get n 1s leftmost */
#define GETnONESLEFTMOST(n) (~((unsigned)~0>>(n)))

/* get n 1s from position p rightmost to one,others are zero */
#define GETnONESFROMpRIGHTMOST(p, n) (GETnONESRIGHTMOST(n)<<((p)+1-(n)))
/* get n 1s from position p leftmost to one,others are zero */
#define GETnONESFROMpLEFTMOST(p, n) (GETnONESRIGHTMOST<<(p))

/* get n bits from position p rightmost,others are zero */
#define GETnBITSFROMpRIGHTMOST(x, p, n) (((x)>>((p)+1-(n)))&GETnONESRIGHTMOST(n))
/* get n bits from position p leftmost,others are zero */
#define GETnBITSFROMpLEFTMOST(x, p, n) (((x)>>(p))&GETnONESRIGHTMOST(n))

/* set n bits from position p rightmost zero,and leaving other bits unchanged */
#define SETnBITSFROMpRIGHTMOSTZERO(x, p, n) ((x)&(~(((1<<(n))-1)<<((p)-(n)+1))))
/* set n bits from position p leftmost zero,and leaving other bits unchanged */
#define SETnBITSFROMpLEFTMOSTZERO(x, p, n) ((x)&(~(((1<<(n))-1)<<(p))))
/* set n bits from position p rightmost one,and leaving other bits unchanged */
#define SETnBITSFROMpRIGHTMOSTONE(x, p, n) ((x)&(~(((1<<(n))-1)<<((p)-(n)+1))))
/* set n bits from position p leftmost one,and leaving other bits unchanged */
#define SETnBITSFROMpLEFTMOSTONE(x, p, n) ((x)&(~(((1<<(n))-1)<<(p))))

/* set n bits of x that begin at position p to the rightmost n
 * bits of y, leaving the other bits unchanged.
 */
unsigned setbits(unsigned x, int p, int n, int y)
{
	return SETnBITSFROMpLEFTMOSTZERO(x,p,n)|((GETnBITSFROMpRIGHTMOST(y,p,n))<<p);
}

/* Write a function invert(x,p,n) that returns x with the n bits that begin at 
 * position p inverted (i.e., 1 changed into 0 and vice versa), leaving the 
 * others unchanged.
 */
unsigned invert(unsigned x, int p, int n)
{
	return x ^ (GETnONESRIGHTMOST(n)<<(p));
}

/* Write a function rightrot(x,n) that returns the value of the integer x rotated to the right by n positions. */
unsigned rightrot(unsigned x, int n)
{
	int left_shift = CHAR_BIT * sizeof(x) - n;
	return (x<<left_shift) | (x>>n);
}

void e_2_6(void)
{
	unsigned x = 0xce5, y = 0xf2e4;
	printf("x:%08x,y:%08x,ret:%08x\n", x,y,setbits(x,5,6,y));
}

void e_2_7(void)
{
	unsigned x = 0xce5, y = 0xf2e4;
	printf("x:%08x,y:%08x,ret:%08x\n", y,(GETnONESRIGHTMOST(6)),invert(y,5,6));
}

void e_2_8(void)
{
	unsigned x = 0xc75, y = 0xf2e4;
	printf("x:%08x,ret:%08x\n", x,rightrot(x,6));
}
/* exercise 2-6,2-7,2-8 */

/* a faster version of bitcount */
int bitcount(unsigned x)
{
	int b = 0;

	while(x){
		++b;
		x &= (x-1);
	}

	return b;

}
void e_2_9()
{
	printf("OX%08x has %d bits\n", 0x2319, bitcount(0x2319));
}

/* Rewrite the function lower, which converts upper case letters to lower case, with a conditional expression
 * instead of if-else.
 */
int lower(int c)
{
	return (c >= 'A' && c <= 'Z') ? (c+0x20):(c);
}

void e_2_10()
{
	printf("c %c(%c)\n", 'F', lower('F'));
}

int main(void)
{
	//e_2_1();	
	//e_2_3();
	//e_2_4();
	//e_2_5();
	//e_2_6();
	//e_2_7();
	//e_2_8();
	//e_2_9();
	e_2_10();

	return 0;
}
