#include <stdio.h>
#include <stdlib.h>
//#include <string.h> 
#include <ctype.h>
#include <limits.h>

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

/* exercise 5-1 */
/* As written, getint treats a + or - not followed by a digit as a valid 
 * representation of zero. Fix it to push such a
 * character back on the input. */
/* getint: get next integer from input into *pn */
int getint(int *pn)
{
	int c, sign;

	while (isspace(c = getch())) /* skip white space */
		;
	if (!isdigit(c) && c != EOF && c != '+' && c != '-') {
		ungetch(c); /* it is not a number */
		return 0;
	}
	sign = (c == '-') ? -1 : 1;
	if (c == '+' || c == '-')
		c = getch();
	if(!isdigit(c)){
		ungetch(c);
		return c;
	}
	for (*pn = 0; isdigit(c); c = getch())
		*pn = 10 * *pn + (c - '0');
	*pn *= sign;
	if (c != EOF)
		ungetch(c);

	return c;
}

/*  test it:./a.out < test5 */
void e_5_1(void)
{
	int n[20]={0};
	int i;

	printf("e_5_1:");
	for(i=0;i<20;++i){
		getint(&n[i]);

		printf("n is %d\n", n[i]);
	}

	return;
}
/* exercise 5-1 */

/* exercise 5-2 */
/* Write getfloat, the floating-point analog of getint. */
int getfloat(float *pf)
{
	int c, sign;
	float power = 1.0;

	while (isspace(c = getch())) /* skip white space */
		;
	if (!isdigit(c) && c != EOF && c != '+' && c != '-') {
		ungetch(c); /* it is not a number */
		return 0;
	}
	sign = (c == '-') ? -1 : 1;
	if (c == '+' || c == '-')
		c = getch();
	if(!isdigit(c)){
		ungetch(c);
		return c;
	}
	for (*pf = 0.0; isdigit(c); c = getch())
		*pf = 10.0 * *pf + (c - '0');
	if(c == '.'){
		c = getch();
		for (; isdigit(c); c = getch()){
			*pf = 10.0 * *pf + (c - '0');
			power *= 10.0;
		}
	}
	*pf = sign * ((*pf)/power);
	if (c != EOF)
		ungetch(c);

	return c;
}

void e_5_2(void)
{
	int i;
	float f[20] = {0.0};

	printf("e_5_2:");
	for(i = 0;i < 20;++i){
		getfloat(&f[i]);
		printf("f is %f\n", f[i]);
	}

	return;
}
/* exercise 5-2 */

/* Write a pointer version of the function strcat that we showed in 
 * Chapter 2: strcat(s,t) copies the string t to the end of s. */
/* exercise 5-3 */
void  strcat(char *s, char *t)
{
	while(*s)		// wrong:while(*s++);
		s++;
	while(*s++ = *t++)
		;
}
void e_5_3(void)
{
	char s[10] = "1234";
	char t[] = "789";

	strcat(s, t);
	printf("after cat,s:%s\n", s);
}
/* exercise 5-3 */

/* exercise 5-4 */
/* Write the function strend(s,t), which returns 1 if the string t occurs at
 * the end of the string s, and zero
 * otherwise. */
int strend(char *s, char *t)
{
	int s_len = strlen(s);
	int t_len = strlen(t);

	if(s_len < t_len)
		return 0;
	s += (s_len - t_len);
	if(strcmp(s, t, t_len) == 0)
		return 1;

	return 0;
}
void e_5_4(void)
{
	char s[] = "abc098";
	char t[] = "098iik";
	int ret;

	ret = strend(s, t);
	printf("ret:%d\n", ret);

	return;
}
/* exercise 5-4 */

/* exercise 5-5 */
char *strncpy(char *s, char *t, int n)
{
	char *p = s;

	while(n-- && (*s++ = *t++)){
		;
	}
	*s = '\0';

	return p;
}
char *strncat(char *s, char *t, int n)
{
	char *p = s;

	while(*s){
		s++;
	}
	while(n-- && (*s++ = *t++))
		;
	*s = '\0';

	return p;
}
int strncmp(char *s, char *t, int n)
{
	while(n-- && (*s++ == *t++))
		;

	if(n < 0)
		return 0;
	else
		return *s - *t;
}
void e_5_5(void)
{
	char s[20] = "abcde";
	char t[]   = "123";
	char *p;
	int ret;

	p = strncpy(s, t, strlen(t));
	printf("strncpy:%s\n", p);
	p = strncat(s, t, 3);
	printf("strncat:%s\n", p);
	ret = strncmp(s, t, 4);
	printf("strncmp:%d\n", ret);

	return;
}
/* exercise 5-5 */

/* exercise 5-6 */
/* just take `getline` for example */
int getline(char *line, int maxlen)
{
	char *p = line;
	int c;

	while(maxlen-- && (c = getchar()) != EOF && c != '\n'){
		*line++ = c;
	}
	if(maxlen > 0)
		*line = '\0';

	return line - p;
}
void e_5_6(void)
{
	int ret;
	char line[50];

	ret = getline(line, 50);
	printf("ret:%d,line:%s\n", ret, line);

	return;
}
/* exercise 5-6 */

/* exercise 5-7 */
#define MAX_LINES  20
#define MAX_LENGTH 200
int readlines(char lineptr[MAX_LINES][MAX_LENGTH], int maxlines)
{
	int len, nlines;
	char line[MAX_LENGTH];

	nlines = 0;
	while((len = getline(line, MAX_LENGTH)) > 0){
		if(nlines >= maxlines){
			return -1;
		}else{
			line[len] = '\0';
			strncpy(lineptr[nlines++], line, len);
		}
	}

	return nlines;
}

void e_5_7(void)
{
	int lines;
	int i;
	char lineptr[MAX_LINES][MAX_LENGTH];

	lines = readlines(lineptr, MAX_LINES);
	printf("lines:%d\n", lines);
	for(i = 0; i < lines; ++i){
		printf("lineptr[%d] = %s\n", i, lineptr[i]);
	}

	return;
}
/* exercise 5-7 */

/* exercise 5-8 */
/* There is no error checking in day_of_year or month_day. Remedy this defect. */
static char daytab[2][13] = {
	{0, 31, 28, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30},
	{0, 31, 29, 31, 30, 31, 30, 31, 30, 31, 30, 31, 30},
};
/* day_of_year: set day of year from month & day */
int day_of_year(int year, int month, int day)
{
	if((year < 1) || (month < 1 || month > 12) || (day < 1)){
		printf("err:invalid para %d,%d,%d\n", year, month, day);
		return -1;
	}
	
	int leap = (year%4 == 0 && year%100 != 0) || (year%400 ==0);
	if(day > daytab[leap][month]){
		printf("err:invalid para %d\n", day);
	}

	while(--month > 0){
	    day += daytab[leap][month];
	}

	return day;
}

/* month_year: set month, day from day of year */
void month_year(int year, int yearday, int *pmonth, int *pday)
{
	int i;
	
	if(year < 1 || yearday > 366){
		printf("err:invalid para %d,%d\n", year, yearday);
		return;
	}
	if(pmonth == NULL || pday == NULL){
		printf("err:invalid para %p,%p\n", pmonth, pday);
		return;
	}

	int leap = (year%4 == 0 && year%100 != 0) || (year%400 ==0);

	for(i = 1; yearday > daytab[leap][i]; ++i){
		yearday -= daytab[leap][i];
	}
	*pmonth = i;
	*pday = yearday;

	return;
}

void e_5_8(void)
{
	int m, d;
	
	printf("day_of_year(1988, 3, 1) is %d\n", day_of_year(1988, 3, 1));
	month_year(1988, 60, &m, &d);
	printf("month_year(1988, 60, *, *)--->>month:%d,day:%d\n", m, d);

	return;
}
/* exercise 5-8 */

/* exercise 5-9 */
/* Rewrite the routines day_of_year and month_day with pointers instead of indexing. */
int day_of_year_p(int year, int month, int day)
{
	char (*pdaytab)[13] = daytab;

	if((year < 1) || (month < 1 || month > 12) || (day < 1)){
		printf("err:invalid para %d,%d,%d\n", year, month, day);
		return -1;
	}
	
	int leap = (year%4 == 0 && year%100 != 0) || (year%400 ==0);
	if(day > *(pdaytab[leap]+month)){
		printf("err:invalid para %d\n", day);
	}

	while(--month > 0){
	    day += *(pdaytab[leap]+month);
	}

	return day;
}

void month_year_p(int year, int yearday, int *pmonth, int *pday)
{
	int i;
	char (*pdaytab)[13] = daytab;
	
	if(year < 1 || yearday > 366){
		printf("err:invalid para %d,%d\n", year, yearday);
		return;
	}
	if(pmonth == NULL || pday == NULL){
		printf("err:invalid para %p,%p\n", pmonth, pday);
		return;
	}

	int leap = (year%4 == 0 && year%100 != 0) || (year%400 ==0);

	for(i = 1; yearday > *(pdaytab[leap]+i); ++i){
		yearday -= *(pdaytab[leap]+i);
	}
	*pmonth = i;
	*pday = yearday;

	return;
}

void e_5_9(void)
{
	int m, d;
	
	printf("day_of_year(1988, 3, 1) is %d\n", day_of_year_p(1988, 3, 1));
	month_year_p(1988, 60, &m, &d);
	printf("month_year(1988, 60, *, *)--->>month:%d,day:%d\n", m, d);

	return;
}
/* exercise 5-9 */

int find_test(int argc, char *argv[])
{
	char line[MAX_LENGTH];
	int c, except = 0, found = 0, number = 0;
	long lineno = 0;
	int i = 0;

	while(--argc > 0 && (*++argv)[0] == '-'){
		while(c = *++argv[0]){
			switch(c){
				case 'x':
					except = 1;
					break;
				case 'n':
					number = 1;
					break;
				default:
					printf("find:illegal option %c\n", c);
					argc = 0;
					found = -1;
					break;
			}
		}
	}
	if(argc != 1){
		printf("Usage:find -x -n pattern\n");
	}else{
		while(getline(line, MAX_LENGTH) > 0){
			lineno++;
			if((strstr(line, *argv) != NULL) != except){
				if(number)
					printf("%ld:", lineno);
				printf("%s\n", line);
				found++;
			}
		}
	}
	return found;
}

static int number_stack[20];
static int number_stack_top = 0;
void push(int n)
{
	if(n > 20){
		printf("%d is larger than 20.\n", n);
		return;
	}
	number_stack[number_stack_top++] = n;

	return;
}

int pop(void)
{
	if(number_stack_top <= 0){
		//printf("number_stack is NULL.\n");
		return INT_MIN;
	}

	return number_stack[--number_stack_top];
}
void expr(int argc, char *argv[])
{
	//printf("argc %d\n", argc);
	while(--argc > 0){
		++argv;
		//printf("%s\n", *argv);
		if(isdigit((*argv)[0])){
		//	printf("push %d\n", atoi(*argv));
			push(atoi(*argv));
		}else{
			int op = (*argv)[0];
			int a1 = pop();
			int a2 = pop();
			switch(op){
				case '+':
					push(a1 + a2);
					break;
				case '-':
					push(a1 - a2);
					break;
				case '*':
					push(a1 * a2);
					break;
				case '/':
					push(a1 / a2);
					break;
				default:
					printf("can't support %c.\n", op);
					break;
			}
		}
		//printf("%s\n", *++argv);
	}
	printf("reslut:%d\n", pop());

	return;
}

/*
 * Write the program expr, which evaluates a reverse Polish expression from the command line, where each
 * operator or operand is a separate argument.
 * */
void e_5_10(int argc, char *argv[])
{
	expr(argc, argv);
}

static int DEFAULT_TAB_SPACE = 4;

void show_usage(void)
{
	printf("\r\nusage:./a.out -t num1 num2 ... < test_data\n");

	return;
}
void entab(int argc, char *argv[])
{
	char line[MAX_LENGTH];
	char *p = line;
	int tab_n = 0;
	int tab_space[10];
	int tab_space_n = 0;
	
	while(--argc){
		if((*++argv)[0] == '-'){
			if('t' != *++argv[0]){
				printf("err %c\n", *++argv[0]);
				show_usage();
				return;
			}
		}else if(isdigit((*argv)[0])){
			printf("arg:%d\n", atoi(*argv));
			if(tab_space_n > 9){
				printf("too many args\n");
				break;
			}
			/* [0] is not used */
			tab_space[++tab_space_n] = atoi(*argv);
		}else{
			show_usage();
			return;
		}
	}

	while(fgets(p, MAX_LINES, stdin)){
	    while(*p){
		    if(*p == '\t'){
			    ++tab_n;
			    if(tab_n <= tab_space_n){
				    printf("%*c", tab_space[tab_n]+1, '|');
			    }else{
				    printf("%5c", '|');
			    }
		    }else{
			    putchar(*p);
		    }
		    ++p;
	    }
	}

	return;
}

void detab(int argc, char *argv[])
{
	char line[MAX_LENGTH];
	char *p = line;
	int space_n = 0;
	int space_tab[10];
	int space_tab_n = 0;
	int argv_n = 0;
	
	while(--argc){
		if((*++argv)[0] == '-'){
			if('t' != *++argv[0]){
				printf("err %c\n", *++argv[0]);
				show_usage();
				return;
			}
		}else if(isdigit((*argv)[0])){
			printf("arg:%d\n", atoi(*argv));
			if(space_tab_n > 9){
				printf("too many args\n");
				break;
			}
			space_tab[space_tab_n++] = atoi(*argv);
		}else{
			show_usage();
			return;
		}
	}

	while(fgets(p, MAX_LINES, stdin)){
	    while(*p){
		    if(*p == ' '){
			    ++space_n;
			    if((argv_n < space_tab_n && space_n == space_tab[argv_n]) || 
					    (argv_n >= space_tab_n && space_n == 4)){
				    putchar('|');
				    space_n = 0;
				    argv_n++;
			    }
		    }else{
			    if(space_n > 0){
				    if((argv_n < space_tab_n && space_n < space_tab[argv_n]) ||
						    (argv_n >= space_tab_n && space_n < 4)){
					    printf("%*c", space_n - 1, ' ');
					    space_n = 0;
					    argv_n++;
				    }
			    }
			    putchar(*p);
		    }
		    ++p;
	    }
	}

	return;
}

/*
 * Modify the program entab and detab (written as exercises in Chapter 1) to accept a list of tab stops as
 * arguments. Use the default tab settings if there are no arguments.
 * */
void e_5_11(int argc, char *argv[])
{
	//entab(argc, argv);
	detab(argc, argv);

	return;
}

/*
 * Extend entab and detab to accept the shorthand
 * entab -m +n
 * to mean tab stops every n columns, starting at column m.
 * */
void e_5_12(int argc, char *argv[])
{
	/*
	 * just like e_5_11,just get m&n from argv,and deal between m~m+n
	 *
	 * */
}

/*
 * Write the program tail, which prints the last n lines of its input.
 *
 * */
void e_5_13(int argc, char *argv[])
{
	int lines = 0;
	char **pp_lines = NULL;
	char line[MAX_LENGTH];
	int line_no = 0;
	int first_ind = 0;
	int tmp_ind = 0;
#define DEFAULT_PUT_LINES 10

	if(argc == 1){
		lines = DEFAULT_PUT_LINES;
	}else if(argc == 2){
		++argv;
		lines = atoi(++argv[0]);
	}else{
		printf("Usage:tail -n\n");
		return;
	}
	if(0 >= lines){
		printf("invalid arg:%d\n", lines);
		return;
	}

	pp_lines = malloc(sizeof(char*) * lines);
	if(pp_lines == NULL){
		printf("malloc err!");
		return;
	}
	memset(pp_lines, 0, sizeof(char*) * lines);

	while(fgets(line, sizeof(line), stdin)){
		pp_lines[++line_no % lines] = strdup(line);
		if(line_no >= lines)
			first_ind = (first_ind + 1) % lines;
	}

	tmp_ind = first_ind;
	while(1){
		if(pp_lines[tmp_ind] != NULL){
			printf(pp_lines[tmp_ind]);
		}
		tmp_ind = (tmp_ind + 1) % lines;
		if(tmp_ind == first_ind)
			break;
	}

	return;
}

/*--------------------begin----------------------*/
/* 
 * exercise 5-14 to exercise 5-17
 *
 * */
#define MAXLINES 5000
char *lineptr[MAXLINES];
int readlines2(char *lineptr[], int nlines);
void writelines(char *lineptr[], int nlines);
void qsort2(void *v[], int left, int right, int (*comp)(void *, void *));
int numcmp(void *, void *);
int strcmp2(void *, void *);
static int g_reverse_flag = 0;
static int g_ignore_case = 0;
static int g_directory_sort = 0;
static int g_field_sort = 0;
void sort(int argc, char *argv[])
{
	int nlines = 0;
	int numberic = 0;
	char *pargv;
	int i;

	if(argc >= 2){
		for(i = 1; i < argc; ++i){
			pargv = argv[i];
			if(strcmp(pargv, "-n") == 0)
				numberic = 1;
			else if(strcmp(pargv, "-d") == 0)
				g_directory_sort = 1;
			else if(strcmp(pargv, "-r") == 0)
				g_reverse_flag = 1;
			else if(strcmp(pargv, "-f") == 0)
				g_ignore_case = 1;
			else if(strncmp(pargv, "-F", 2) == 0){
				g_field_sort = atof(pargv+2);
			}
			else if(strncmp(pargv, "-df", 2) == 0){
				g_directory_sort = 1;
				g_ignore_case = 1;
			}
			else
				;
		}
	}
printf("numberic:%d, g_directory_sort:%d, g_reverse_flag:%d, g_ignore_case:%d, g_field_sort:%d\n", numberic, g_directory_sort,g_reverse_flag,g_ignore_case,g_field_sort);

	if((nlines = readlines2(lineptr, MAXLINES)) > 0){
		qsort2(lineptr, 0, nlines - 1, (int (*)(void *, void *))(numberic ? numcmp :  strcmp2));
		writelines(lineptr, nlines);
	}else{
		printf("input too big to sort!\n");
	}

	return;
}

int readlines2(char *lineptr[], int maxlines)
{
	int len, nlines = 0;
	char line[MAX_LENGTH];

	while(fgets(line, MAX_LENGTH, stdin) > 0){
		lineptr[nlines++] = strdup(line);
		if(nlines >= maxlines){
			printf("input too big to deal:%d\n", nlines);
			break;
		}
	}

	return nlines;
}

void writelines(char *lineptr[], int nlines)
{
	int tmp_lines = 0;
	if(nlines <= 0)
		return;

	while(tmp_lines < nlines){
		printf(lineptr[tmp_lines]);
		free(lineptr[tmp_lines]);
		lineptr[tmp_lines] = NULL;
		++tmp_lines;
	}

	return;
}

void swap(void *v[], int, int);
void qsort2(void *v[], int left, int right, int (*comp)(void *, void *))
{
	int i, last;

	if(left >= right)
		return;

	swap(v, left, (left+right)/2);
	last = left;
	for(i = left + 1; i <= right; ++i){
		if((*comp)(v[i], v[left]) < 0)
			swap(v, ++last, i);
	}
	swap(v, left, last);
	qsort2(v, left, last - 1, comp);
	qsort2(v, last + 1, right, comp);

	return;
}
void swap(void *v[], int i, int j)
{
	void *tmp;

	tmp = v[i];
	v[i] = v[j];
	v[j] = tmp;

	return;
}

int numcmp(void *p1, void *p2)
{
	double v1, v2;
	int ret;
	char *pc1;
	char *pc2;
	int len1,len2;
	char temp1[20] = {0};
	char temp2[20] = {0};
	int i = 0;

	if(p1 == NULL || p2 == NULL){
		printf("invalid argv!\n");
		return 0;
	}

	pc1 = p1;
	pc2 = p2;
	if(g_field_sort){
		len1 = strlen(p1);
		len2 = strlen(p2);

		if(len1 < g_field_sort || len2 < g_field_sort)
			return 0;

		pc1 += g_field_sort;
		pc2 += g_field_sort;

		while(*pc1){
			temp1[i++] = *pc1;
			if(isspace(*pc1)){
				temp1[i] = 0;
				break;
			}
			++pc1;
		}

		i = 0;
		while(*pc2){
			temp2[i++] = *pc2;
			if(isspace(*pc2)){
				temp2[i] = 0;
				break;
			}
			++pc2;
		}

		v1 = atof(temp1);
		v2 = atof(temp2);
	}else{
		v1 = atof(pc1);
		v2 = atof(pc2);
	}
	//printf("v1:%f,v2:%f", v1,v2);

	if(v1 < v2)
		ret = -1;
	else if(v1 > v2)
		ret = 1;
	else
		ret = 0;

	return g_reverse_flag ? -ret : ret;
}

int strcmp2(void *p1, void *p2)
{
	int ret = 0;
	char *pc1 = p1;
	char *pc2 = p2;
	int c1,c2;
	int len1,len2;

	if(p1 == NULL || p2 == NULL){
		printf("invalid argv!\n");
		return 0;
	}

	//printf("p1:%s,p2:%s\n",p1,p2);
	len1 = strlen(p1);
	len2 = strlen(p2);

	if(len1 < g_field_sort || len2 < g_field_sort)
		return 0;

	pc1 += g_field_sort;
	pc2 += g_field_sort;

	if(g_directory_sort == 0){
		while(*pc1 && *pc2){
			c1 = (int)*pc1;
			c2 = (int)*pc2;

			if(g_field_sort && (isspace(c1) || isspace(c2)))
				break;
			if(g_ignore_case){
				c1 = tolower(c1);
				c2 = tolower(c2);
			}
			if(c1 == c2)
				ret = 0;
			else if(c1 < c2)
				ret = -1;
			else
				ret = 1;

			if(ret != 0)
				break;
			++pc1;
			++pc2;
		}
	}else{
		while(*pc1 && *pc2){
			c1 = (int)*pc1;
			c2 = (int)*pc2;

			if(g_field_sort && (isspace(c1) || isspace(c2)))
				break;
			if(g_ignore_case){
				c1 = tolower(c1);
				c2 = tolower(c2);
			}
			if(isalpha(c1) || isspace(c1)){
				if(isalpha(c2) || isspace(c2)){
					if(c1 == c2)
						ret = 0;
					else if(c1 < c2)
						ret = -1;
					else
						ret = 1;
				}else{
					ret = 1;
				}
			}else{
				if(isalpha(c2) || isspace(c2))
					ret = -1;
				else
					ret = 0;
			}

			if(ret != 0)
				break;
			++pc1;
			++pc2;
		}
	}

	return g_reverse_flag ? -ret : ret;
}
/*--------------------end----------------------*/

int main(int argc, char *argv[])
{
	//e_5_1();
	//e_5_2();
	//e_5_3();
	//e_5_4();
	//e_5_5();
	//e_5_6();
	//e_5_7();
	//e_5_8();
	//e_5_9();
	//find_test(argc, argv);
	//e_5_10(argc, argv);
	//e_5_11(argc, argv);
	//e_5_13(argc, argv);
	sort(argc, argv);

	return 0;
}
