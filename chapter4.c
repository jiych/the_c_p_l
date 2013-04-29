#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

/* exercise 4-1 */
/* Write the function strindex(s,t) which returns the position of the rightmost occurrence of t in s, or -1 if
there is none. */
int strindex(char s[], char t[])
{
	int i,j,k;

	for(i = strlen(s) - 1; i >= 0; --i){
		for(j = 0, k = i; t[j] != '\0' && s[k] != '\0' && s[k] == t[j]; k++, j++)
			;
		if(t[j] == '\0')
			return i;
	}

	return -1;
}
void e_4_1()
{
	int ind;

	ind = strindex("i would what y would like ok", "oul");

	printf("ind is %d\n", ind);
}
/* exercise 4-1 */

/* exercise 4-2 */
/* Extend atof to handle scientific notation of the form
 * 123.45e-6 
 * where a floating-point number may be followed by e or E and 
 * an optionally signed exponent. */
double atof(char s[])
{
	double val, power;
	int i, sign;
	int exp_val, exp_sign;

	for (i = 0; isspace(s[i]); i++) /* skip white space */
		;
	sign = (s[i] == '-') ? -1 : 1;
	if (s[i] == '+' || s[i] == '-')
		i++;
	for (val = 0.0; isdigit(s[i]); i++)
		val = 10.0 * val + (s[i] - '0');
	if (s[i] == '.')
		i++;
	for (power = 1.0; isdigit(s[i]); i++) {
		val = 10.0 * val + (s[i] - '0');
		power *= 10;
	}
	if (s[i] == 'e' || s[i] == 'E')
		i++;
	exp_sign = (s[i] == '-') ? -1 : 1;
	if (s[i] == '+' || s[i] == '-')
		i++;
	for (exp_val = 0; isdigit(s[i]); i++)
		exp_val = 10 * exp_val + (s[i] - '0');

	return sign * (val / power) * exp(exp_sign * exp_val);
}
void e_4_2(void)
{
	double ret = atof("123.45e-6");

	printf("ret:%g\n", ret);
}
/* exercise 4-2 */

/* exercise 4-3 */
/* pls IGNORE the function name, which includes exercise 4-3,4-4,4-5,4-6,4-7 */
#define MAXOP 100 /* max size of operand or operator */
#define NUMBER '0' /* signal that a number was found */
#define FUNC '1' /* signal that function was found */
#define LETTER '2' /* signal that a letter was found */

int getop(char []);
void push(double);
double pop(void);
void print_stack(void);
void duplicate_stack(void);
void clear_stack(void);
void swap_stack(void);
void oper(char s[]);
void deal_letter(char s[]);
double g_top = 0;
int g_assign = 0;
int g_user = 0;
int g_assign_ind = -1;
static struct letter_val    /* use static global variable, so dont need to initialize it */
{
	int valid_flag;
	double val;
}g_letter_val[26];

/* reverse Polish calculator */
void e_4_3(void)
{
	int type;
	double op2;
	double op1;
	char s[MAXOP];
	while ((type = getop(s)) != EOF) {
		g_user = 0;
		switch (type) {
			case NUMBER:
				if(g_assign){
					if(g_assign_ind < 0){
						printf("error:g_assign_ind %d\n", g_assign_ind);
						return;
					}
					g_letter_val[g_assign_ind].val = atof(s);
					g_letter_val[g_assign_ind].valid_flag = 1;
					g_assign = 0;
					g_assign_ind = -1;
				}else
					push(atof(s));
				break;
			case '+':
				push(pop() + pop());
				break;
			case '*':
				push(pop() * pop());
				break;
			case '-':
				op2 = pop();
				push(pop() - op2);
				break;
			case '/':
				op2 = pop();
				if (op2 != 0.0)
					push(pop() / op2);
				else
					printf("error: zero divisor\n");
				break;
			case '%':
				op2 = pop();
				op1 = pop();
				if (op2 > 0.0 && op1 > 0.0){
					printf("op1:%d,op2:%d\n", (int)op1,(int)op2);
					push((int)op1 % (int)op2);
				}
				else
					printf("error: negative modulus\n");
				break;
			case FUNC:
				oper(s);
				printf("func:%s\n", s);
				break;
			case LETTER:
				deal_letter(s);
				printf("letter:%s\n", s);
				break;
			case '\n':
				printf("============\n");
				printf("\t%.8g\n", pop());
				break;
			default:
				printf("error: unknown command %s\n", s);
				break;
		}
	}
	return;
}

#define MAXVAL 100 /* maximum depth of val stack */
int sp = 0; /* next free stack position */
double val[MAXVAL]; /* value stack */
/* push: push f onto value stack */
void push(double f)
{
	if (sp < MAXVAL)
		val[sp++] = f;
	else
		printf("error: stack full, can't push %g\n", f);
}
/* pop: pop and return top value from stack */
double pop(void)
{
	if (sp > 0){
		g_top = val[--sp];
	}else{
		printf("error: stack empty\n");
		g_top = 0.0;
	}

	return g_top;
}

void oper(char s[])
{
	double op1;

	printf("s:%s\n",s);
	if(!strncmp(s, "sin", 3)){
		push(sin(pop()));
	}else if(!strncmp(s, "cos", 3)){
		push(cos(pop()));
	}else if(!strncmp(s, "pow", 3)){
		op1 = pop();
		push(pow(pop(), op1));
	}else if(!strncmp(s, "print", 5)){
		print_stack();
		g_user = 1;
	}else if(!strncmp(s, "clear", 5)){
		clear_stack();
		g_user = 1;
	}else if(!strncmp(s, "swap", 4)){
		swap_stack();
		g_user = 1;
	}else if(!strncmp(s, "dup", 3)){
		duplicate_stack();
		g_user = 1;
	}else{
		printf("error:can't support '%s'\n", s);
	}

	return;
}

void deal_letter(char s[])
{
	if(!s || (!isalpha(s[0]) && s[0] != '_')){
		printf("error:%s is not alpha\n", s);
		return;
	}

	if(s[0] == '_'){
		printf("%f\n", g_top);
	}else if(g_assign){
		g_assign_ind = tolower(s[0] - 'a');
	}else{
		if(g_letter_val[tolower(s[0]) - 'a'].valid_flag)
			push(g_letter_val[tolower(s[0]) - 'a'].val);
		else
			printf("error:%c's value not valid\n", s[0]);
	}

	return;
}

/* print_stack: print top value from stack */
int get_top(double *dtop)
{
	if(dtop == NULL){
		printf("error: dtop is NULL\n");
		return -1;
	}
	if(sp > 0){
		*dtop = val[sp-1];
		return 0;
	}else{
		return -1;
	}

}
void print_stack(void)
{
	double dtop;

	if(get_top(&dtop) == 0){
		printf("value of stack's top:%g\n", dtop);
	}else{
		printf("error: stack empty\n");
	}

	return;
}
/* duplicate_stack: duplicate top value from stack */
void duplicate_stack(void)
{
	double dtop;

	if(get_top(&dtop) == 0){
		push(dtop);
	}else{
		printf("error: stack empty\n");
	}

	return;
}

/* swap_stack: swap top 2 elements of stack */
void swap_stack(void)
{
	double op1;
	double op2;

	op1 = pop();
	op2 = pop();
	push(op1);
	push(op2);

	return;
}

/* clear_stack: clear stack */
void clear_stack(void)
{
	sp = 0;

	return;
}

int getch(void);
void ungetch(int);
/* getop: get next character or numeric operand */
int getop(char s[])
{
	int i, c;
	while ((s[0] = c = getch()) == ' ' || c == '\t')
		;
	s[1] = '\0';
	i = 0;
	if(isalpha(c) || c == '_'){
		while(isalpha(s[++i] = c = getch()))
			;
		s[i] = '\0';
		if(c != '=' && c != EOF)
			ungetch(c);
		if(1 == i){
			if(c == '=')
				g_assign = 1;
			return LETTER;
		}else
			return FUNC;

	}
	if (!isdigit(c) && c != '.' && c != '-')
		return c; /* not a number */
	if(c == '-'){
		c = getch();
		if(!isdigit(c)){
		    ungetch(c);
		    return (int)'-';
		}
	}
	if (isdigit(c)) /* collect integer part */
		while (isdigit(s[++i] = c = getch()))
			;
	if (c == '.') /* collect fraction part */
		while (isdigit(s[++i] = c = getch()))
			;
	s[i] = '\0';
	if (c != EOF)
		ungetch(c);

	return NUMBER;
}

#define BUFSIZE 100
char buf[BUFSIZE]; /* buffer for ungetch */
int bufp = 0; /* next free position in buf */
int getch(void) /* get a (possibly pushed-back) character */
{
	return (bufp > 0) ? buf[--bufp] : getchar();
}
void ungetch(int c) /* push character back on input */
{
	if (bufp >= BUFSIZE)
		printf("ungetch: too many characters\n");
	else
		buf[bufp++] = c;

	return;
}

/* ungets don not need to know `buf` and `bufp`, it can just use ungetch */
void ungets(char s[])
{
	if(s == NULL)
		return;

	int i = 0;
	int s_len = strlen(s);
	while(i < s_len){
		ungetch(s[i]);
		++i;
	}

	return;
}
/* exercise 4-3 */

/* exercise 4-8 */
/* Suppose that there will never be more than one character of pushback. Modify getch and ungetch accordingly. */
// just modify BUFSIZE from 100 to 1
// #define BUFSIZE 1
/* exercise 4-8 */

/* exercise 4-9 */
/* i dont know the exact meaning of this problem:) */
/* exercise 4-9 */

/* exercise 4-10 */
/* just need to modify the main loop and getop function, draft code like this */
#if 0
...
while(s = getline()){
	while(getop(s)){
		....
	}
}
...
int getop(char s[])
{
	for(i=0;i<strlen(s);++i){
		if(s[i] == '_')
			...
	}
}
#endif
/* exercise 4-10 */

/* exercise 4-11 */
/* just modify the origin `getop` */
int getop_use_static_val(char s[])
{
	int i, c;
	static int push_back_c = EOF;

	c = s[0] = (push_back_c == EOF) ? getch() : push_back_c;
	while(c == ' ' || c == '\t'){
		c = s[0] = getch();
	}
	s[1] = '\0';
	if(!isdigit(c)){
		push_back_c = EOF;
		return c;
	}
	i = 0;
	while(isdigit(s[++i] = c = getch()))
		;
	if(c == '.')
		while(isdigit(s[++i] = c = getch()))
			;
	s[i] = '\0';
	if(c != EOF)
		push_back_c = c;

	return NUMBER;
}

void e_4_11(void)
{
	int type;
	char s[100];

	while ((type = getop_use_static_val(s)) != EOF) {
		switch (type) {
			case '%':
				printf("module:%s\n", s);
				break;
			case '/':
				printf("divid:%s\n", s);
				break;
			case NUMBER:
				printf("number:%s\n", s);
				break;
			case '\n':
				break;
			default:
				printf("can't recognize %s...\n", s);
				break;
		}
	}

	return;
}
/* exercise 4-11 */

/* exercise 4-12 */
/* Adapt the ideas of printd to write a recursive version of itoa; that is, convert an integer into a string by
 * calling a recursive routine. */
char* itoa(int n, char s[])
{
	char *p = s;
	
	if(n/10){
		p = itoa(n/10, p);
	}else if(n < 0){
		*p++ = '-';
	}
	*p++ = abs(n%10) + '0';
	*p = '\0';

	return p;
}
void e_4_12(void)
{
	char s[100];

	itoa(-1234, s);
	printf("s:(%s)\n", s);

	return;
}
/* exercise 4-12 */

/* exercise 4-13 */
/* Write a recursive version of the function reverse(s), 
 * which reverses the string s in place. */
// did't have any idea
void reverse(char s[])
{
}

void e_4_13(void)
{
	char s[] = "e_4_13";

	reverse(s);
	printf("s:%s\n", s);

	return;
}
/* exercise 4-13 */

/* exercise 4-14 */
/* Define a macro swap(t,x,y) that interchanges two arguments of type t. 
 * (Block structure will help.) */
void e_4_14()
{
#define swap(t, x, y) do{ \
			t temp;\
			temp = *(x);\
			*(x)  = *(y);\
			*(y)  = temp;\
			}while(0)

	int x = 4;
	int y = 3;
	swap(int, &x, &y);
	printf("after swap:x=%d, y=%d\n", x, y);
}
/* exercise 4-14 */

int main(void)
{
	//e_4_1();
	//e_4_2();
	//e_4_3();
	//e_4_11();
	//e_4_12();
	//e_4_13();
	e_4_14();

	return 0;
}
