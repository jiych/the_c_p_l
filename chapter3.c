#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

/* exercise 3_1 */
/* Write a version with only one test inside the loop and measure the difference in run-time. */
/* binsearch: find x in v[0] <= v[1] <= ... <= v[n-1] */
int binsearch(int x, int v[], int n)
{
	int low, high, mid;

	low = 0;
	high = n - 1;
	mid = (low+high)/2;
	printf("n is %d\n", n);
	while(low <= high && x != v[mid]){
		mid = (high+low)/2;
		printf("mid is %d\n", mid);
		if(x < v[mid]){
			high = mid - 1;
		}else{
			low = mid + 1;
		}
		printf("low is %d,high is %d\n", low,high);
	}

	return (x==v[mid])?mid:-1;
}

void e_3_1(void)
{
	int v[] = {2,3,4,7,8,23,54,65,76};
	int ret = binsearch(7, v, sizeof(v)/sizeof(int));

	printf("%d,ret is %d\n", sizeof(v),ret);
}
/* exercise 3-1 */

/* exercise 3-2 */
/* Write a function escape(s,t) that converts characters like newline and tab into visible escape sequences like
 * \n and \t as it copies the string t to s. Use a switch. Write a function for the other direction as well, converting escape
 * sequences into the real characters. */
void escape(char s[], char t)
{
	char *p;

	switch(t){
		case '\n':
			p = "\\n";
			break;
		case '\t':
			p = "\\t";
			break;
		default:
			printf("don't recognize %d\n", t);
			return;
	}

	strncpy(s, p, 3);
	return;
}
void epacse(char s[], char *t)
{
	if(strncmp(s, "\\t", 3) == 0){
		*t = '\t';
	}else if(strncmp(s, "\\n", 3) == 0){
		*t = '\n';
	}

	return;
}

void e_3_2(void)
{
	char s[4];
	char t;

	escape(s, '\n');
	printf("escape:'%s'\n", s);
	epacse(s, &t);
	printf("epacse:'%d'\n", t);

	return;
}
/* exercise 3-2 */

/* exercise 3-3 */
/* Write a function expand(s1,s2) that expands shorthand notations like a-z in the string s1 into the equivalent complete 
 * list abc...xyz in s2. Allow for letters of either case and digits, and be prepared to handle cases like a-b-c and a-z0-9 
 * and -a-z. Arrange that a leading or trailing - is taken literally. */
void expand(char s1[], char s2[])
{
	char *ps1 = s1;
	char *ps2 = s2;
	char c = 0;
	int previous_num = 0;
	int dash_num = 0;
	char tmp_char[11];
	int tmp_num;
	enum en_prev_type{
		PREV_TYPE_DASH,
		PREV_TYPE_DIGIT,
		PREV_TYPE_ALPHA,
		PREV_TYPE_BUTT
	}previous_char_type = PREV_TYPE_BUTT;
#define WRITE_DIGIT2BUF(buf, digit) 	do{\
						sprintf(tmp_char, "%d", digit);\
						strncpy(buf, tmp_char, strlen(tmp_char)+1);\
						buf += strlen(tmp_char);\
					}while(0)
#define WRITE_CHAR2BUF(buf, c)		do{\
						*(buf)++ = c;\
					}while(0)
#define WRITE_DASH2BUF(buf, num)	while(num > 0){\
						*(buf)++ = '-';\
						num--;\
					}


	while(*ps1){
		c = *ps1;
		printf("c is %c,s2:%s\n", c, s2);
		if('-' == c){
			dash_num++;
		}else if(isspace(c)){
			*ps2++ = c;
		}else if(isalpha(c)){
			if(dash_num != 0){
				if(PREV_TYPE_ALPHA == previous_char_type){
					if(previous_num >= c){
						WRITE_CHAR2BUF(ps2, previous_num);
						WRITE_DASH2BUF(ps2, dash_num);
						WRITE_CHAR2BUF(ps2, c);
					}else{
						while(previous_num <= c){
							WRITE_CHAR2BUF(ps2, previous_num);
							previous_num++;
						}
					}
					previous_char_type = PREV_TYPE_BUTT;
					previous_num = 0;
					dash_num = 0;
				}else if(PREV_TYPE_BUTT != previous_char_type){
					WRITE_DIGIT2BUF(ps2, previous_num);
					WRITE_DASH2BUF(ps2, dash_num);

					previous_char_type = PREV_TYPE_ALPHA;
					previous_num = c;
				}else{
					WRITE_DASH2BUF(ps2, dash_num);
					previous_char_type = PREV_TYPE_ALPHA;
					previous_num = c;
				}
			}else{
				if(PREV_TYPE_ALPHA == previous_char_type){
					*ps2++ = previous_num;
				}
				previous_char_type = PREV_TYPE_ALPHA;
				previous_num = c;
			}
		}else if(isdigit(c)){
			tmp_num = *ps1 - '0';
			ps1++;
			for(;*ps1 && isdigit(*ps1);ps1++){
				tmp_num = tmp_num * 10 + *ps1 - '0';
			}
//			printf("previous_num:%d,%d\n", previous_char_type, tmp_num);

			if(dash_num != 0){
				if(PREV_TYPE_DIGIT == previous_char_type){
//					printf("p:%d,t:%d\n", previous_num, tmp_num);
					if(previous_num >= tmp_num){
						WRITE_DIGIT2BUF(ps2, previous_num);
						WRITE_DASH2BUF(ps2, dash_num);
						WRITE_DIGIT2BUF(ps2, tmp_num);
					}else{
						while(previous_num <= tmp_num){
							WRITE_DIGIT2BUF(ps2, previous_num);
							previous_num++;
						}
					}
					previous_char_type = PREV_TYPE_BUTT;
					previous_num = 0;
					dash_num = 0;
				}else if(PREV_TYPE_BUTT != previous_char_type){
					WRITE_CHAR2BUF(ps2, previous_num);
					WRITE_DASH2BUF(ps2, dash_num);
					previous_char_type = PREV_TYPE_DIGIT;
					previous_num = tmp_num;
				}else{
					WRITE_DASH2BUF(ps2, dash_num);
					previous_char_type = PREV_TYPE_DIGIT;
					previous_num = tmp_num;
				}
			}else{
				previous_char_type = PREV_TYPE_DIGIT;
				previous_num = tmp_num;
			}
			continue;
		}else{
			if(PREV_TYPE_DIGIT == previous_char_type){
				WRITE_DIGIT2BUF(ps2, previous_num);
			}else if(PREV_TYPE_ALPHA == previous_char_type){
				WRITE_CHAR2BUF(ps2, previous_num);
			}
			WRITE_DASH2BUF(ps2, dash_num);
			WRITE_CHAR2BUF(ps2, c);
			previous_char_type = PREV_TYPE_BUTT;
			previous_num = 0;
		}
		ps1++;
	}
	printf("dash_num:%d, previous_char_type:%d, previous_num:%d\n", dash_num, previous_char_type, previous_num);
	if(dash_num != 0 || PREV_TYPE_BUTT != previous_char_type){
		if(PREV_TYPE_DIGIT == previous_char_type){
			WRITE_DIGIT2BUF(ps2, previous_num);
		}else if(PREV_TYPE_ALPHA == previous_char_type){
			WRITE_CHAR2BUF(ps2, c);
		}
		WRITE_DASH2BUF(ps2, dash_num);
	}
	*ps2 = '\0';

	return;
}

void e_3_3(void)
{
	char s1[] = "--sab-j-i==-1-9--a-z0-9---a-z-";
	char s2[130];
	expand(s1, s2);

	printf("s1:%s, s2:%s\n", s1,s2);
}
/* exercise 3-3 */

/* exercise 3-4 */
/* In a two’s complement number representation, our version of itoa does not handle the largest negative
 * number, that is, the value of n equal to -(2wordsize-1). Explain why not.Modify it to print that value correctly, regardless of the
 * machine on which it runs. */
void reverse(char s[])
{
	int c;
	int l, r;

	l = 0;
	r = strlen(s) - 1;

	while(l < r){
		c = s[l];
		s[l] = s[r];
		s[r] = c;
		++l;
		--r;
	}

	return;
}

void itoa(int n, char s[])
{
	int i, sign;
	int add_one = 0;
	int len;

	if((sign = n) < 0){
		if(n != INT_MIN){
			n = -n;
		}else{
			n = INT_MAX; //INT_MIN = (-INT_MAX - 1)
			add_one = 1;
		}
	}

	i = 0;
	do{
		s[i++] = (n % 10) + '0';
	}while((n /= 10) > 0);

	if(add_one){
		for(len = 0; len < i; len++){
			s[len] += add_one;
			if(s[len] > '9'){
				s[len] = '0';
				add_one = 1;
			}else{
				add_one = 0;
			}
		}
		if(add_one){
			s[i++] = '1';
		}
	}

	if(sign < 0){
		s[i++] = '-';
	}
	s[i] = '\0';
	printf("b:%s\n", s);
	reverse(s);
	printf("a:%s,%d,%d\n", s, INT_MIN,INT_MAX);

	return;
}

void e_3_4(void)
{
	int s[20];
	itoa(INT_MIN, (char*)s);
	itoa(INT_MAX, (char*)s);
	itoa(0, (char*)s);
	itoa(-1, (char*)s);
	itoa(1, (char*)s);
	itoa(1000, (char*)s);
	itoa(-100000, (char*)s);
}
/* exercise 3-4 */

/* exercise 3-5 */
/* Write the function itob(n,s,b) that converts the integer n into a base b character representation in the string s.
 * In particular, itob(n,s,16) formats s as a hexadecimal integer in s. */
/* The format of output is like `bc`, cause i do not know exactly what this question want */
enum en_base{
	BASE_2 = 2,
	BASE_8 = 8,
	BASE_10 = 10,
	BASE_16 = 16,
	BASE_BUTT
};
void itob(int n, char s[], int b)
{
	int i, sign;
	int add_one = 0;
	int len;
	int tmp;
	char base_ch[16] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
	
	if(b != BASE_2 && b != BASE_8 && b != BASE_10 && b != BASE_16){
		printf("sorry, can't support this base(%d)\n", b);
		return;
	}

	if((sign = n) < 0){
		if(n != INT_MIN){
			n = -n;
		}else{
			n = INT_MAX; //INT_MIN = (-INT_MAX - 1)
			add_one = 1;
		}
	}

	i = 0;
	do{
		s[i] = (n % b) + '0';
		tmp = s[i] - '0';
		s[i] = base_ch[tmp];
		i++;
	}while((n /= b) > 0);

	if(add_one){
		for(len = 0; len < i; len++){
			s[len] += add_one;
			tmp = s[len] - '0';
			s[len] = base_ch[tmp];
			if(tmp > (b - 1)){
				s[len] = '0';
				add_one = 1;
			}else{
				add_one = 0;
			}
		}
		if(add_one){
			s[i++] = '1';
		}
	}

	if(sign < 0){
		s[i++] = '-';
	}
	s[i] = '\0';
	printf("b:%s\n", s);
	reverse(s);
	printf("a:%s,%d,%d\n", s, INT_MIN,INT_MAX);

	return;
}

void e_3_5(void)
{
	int s[120];

	itob(INT_MIN, (char*)s, 8);
	itob(INT_MAX, (char*)s, 8);
	itob(1838, (char*)s, 8);
	itob(170, (char*)s, 8);
	itob(0, (char*)s, 8);
	itob(-1, (char*)s, 8);
	itob(15, (char*)s, 8);
	itob(-198838, (char*)s, 8);
}
/* exercise 3-5 */

/* exercise 3-6 */
/* Write a version of itoa that accepts three arguments instead of two. The third argument is a minimum field
 * width; the converted number must be padded with blanks on the left if necessary to make it wide enough. */
void itoa_rev(int a, char s[], int min_width)
{
	int len;
	int i;

	itoa(a, s);

	printf("s1:%s\n", s);
	len = strlen(s);
	if(len >= min_width)
		return;

	for(i = len - 1; i >= 0; i--){
		s[min_width - (len - i)] = s[i];
	}
	while(i < (min_width - len)){
		s[i++] = ' ';
	}
	s[min_width] = '\0';

	printf("s2:%s\n", s);

	return;
}
void e_3_6(void)
{
	int s[120];

	itoa_rev(INT_MIN, (char*)s, 16);
	itoa_rev(INT_MAX, (char*)s, 16);
	itoa_rev(11638, (char*)s, 16);
	itoa_rev(170, (char*)s, 16);
	itoa_rev(0, (char*)s, 16);
	itoa_rev(-1, (char*)s, 16);
	itoa_rev(15, (char*)s, 16);
	itoa_rev(-1916838, (char*)s, 16);
}
/* exercise 3-6 */

int main(void)
{
	//e_3_1();
	//e_3_2();
	//e_3_3();
	//e_3_4();
	//e_3_5();
	e_3_6();

	return 0;
}
