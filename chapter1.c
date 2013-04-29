#include <stdio.h>
#include <string.h>

/* exercise 1-9 */
/* copy input to output,replacing one or more blanks by a single blank */
int e_1_9(void)
{
	int c;
	int blankc = 0;

	printf("exercise 1-9:\n");
	while((c = getchar()) != EOF){
		if(' ' == c){
			++blankc;
		}
		else{
			blankc = 0;
		}
		if(0 == blankc || 1 == blankc){
		    putchar(c);
		}
	}

	return 0;
}
/* exercise 1-9 */

/* exercise 1-10 */
/* copy input to output,makes tabs and backspaces visible */
static void replace_char(int *c)
{
	if('\t' == *c)
		*c = 't';
	else if(' ' == *c)
		*c = 'b';
	else if('\\' == *c)
		*c = '\\';

	return;
}

int e_1_10(void)
{
	int c;

	printf("exercise 1-10:\n");
	while((c = getchar()) != EOF){
		if('\t' == c || ' ' == c || '\\' == c){
			putchar('\\');
			replace_char(&c);
		}
		putchar(c);
	}

	return 0;
}
/* exercise 1-10 */

/* exercise 1-13 */
/* print a histogram of the lengths of words in input */
#define MAX_WORDS 20
#define MAX_LENGTH 20
#define STATE_IN  0
#define STATE_OUT 1
int e_1_13(void)
{
	int c;
	int i, j;
	int state = STATE_OUT;
	int wcnt = 0;
	int wlength[MAX_WORDS];
	char words[MAX_WORDS][MAX_LENGTH];

	for(i = 0; i < MAX_WORDS; ++i)
		wlength[i] = 0;

	printf("exercise 1-13:\n");
	while((c = getchar()) != EOF){
		if(' ' == c || '\t' == c || '\n' == c){
			state = STATE_OUT;
		}
		else if(state == STATE_OUT){
			state = STATE_IN;
			words[wcnt][wlength[wcnt]] = c;
			++wlength[wcnt];
			words[wcnt][wlength[wcnt]] = '\0';
			++wcnt;
		}
		else{
			words[wcnt - 1][wlength[wcnt - 1]] = c;
			++wlength[wcnt - 1];
			words[wcnt - 1][wlength[wcnt - 1]] = '\0';
		}
	}

	for(i = 0; i < wcnt; ++i){
		for(j = 0; j < wlength[i]; ++j){
			printf("|\r\n");
		}
		printf("%s\n", words[i]);
	}

	return 0;
}
/* exercise 1-13 */

/* exercise 1-16 & 1-17 */
#define MAXLINE 1000
int getline(char line[], int maxline);
void copy(char to[], char from[]);
void e_1_16(void)
{
	int len, max = 0;
	int c;
	char line[MAXLINE], longest[MAXLINE];

	while((len = getline(line, MAXLINE)) > 0){
		if(len > max){
			copy(longest, line);
			max = len;
			if(max == MAXLINE - 1){
				while((c = getchar()) != EOF && c != '\n')
					++max;
			}
		}
		if(len > 80)
			printf("%s", line);
	}
	if(max > 0)
		printf("%d:%s", max, longest);

	return;
}

int getline(char line[], int maxline)
{
	int c, i;

	for(i = 0; i < maxline - 1 && (c = getchar()) != EOF && c != '\n'; ++i){
		line[i] = c;
	}
	if(c == '\n'){
		line[i] = c;
		++i;
	}

	line[i] = '\0';

	return i;
}

void copy(char to[], char from[])
{
	while((*to++ = *from++) != '\0')
		;
}
/* exercise 1-16 & 1-17 */

/* exercise 1-18 */
void e_1_18(void)
{
	int line_l;
	int flag = 0;
	int c;
	char line[MAXLINE];

	while((line_l = getline(line, MAXLINE)) > 0){
		flag = 0;
		while(line_l >= 0){
			if((c = line[line_l]) != '\0' && c != '\n'){
				if(c == '\t' || c == ' '){
					line[line_l] = '\0';
					flag = 1;
				}else{
					break;
				}
			}
			--line_l;     
		}
		if(flag)
			printf("%s", line);
	}

	return;
}
/* exercise 1-18 */

/* exercise 1-19 */
void reverse(char s[])
{
	int l,r;
	int c;

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

void e_1_19()
{
	int line_l;
	char line[MAXLINE];

	while((line_l = getline(line, MAXLINE)) > 0){
		reverse(line);
		printf("%s", line);
	}
}
/* exercise 1-19 */

/* exercise 1-20 */
/* Write a program detab that replaces tabs in the input with the 
 * proper number of blanks to space to the next tab stop. */
#define TABSTOP 4
int rep_num(int cur)
{
	int tmp = (cur%TABSTOP)?((cur/TABSTOP) + 1):(cur/TABSTOP);
	return (tmp * TABSTOP + 1 - (cur));
}

void detab(char line[])
{
	char *p = line;
	int i = 0;
	int tab_num = 0;
	//\t;;\t;
	while(*p){
		if(*p == '\\' && (*(p+1) && *(p+1) == 't')){
			printf("(%d)(%d)", (p-line+1-tab_num),
				    rep_num((int)(p-line+1-tab_num)));
			for(i=0;i<rep_num((int)(p-line+1-tab_num));++i){
				putchar(' ');
			}
			++p;
			++tab_num;
		}else{
			putchar(*p);
		}
		++p;
	}
}

void e_1_20(void)
{
	int line_l;
	char line[MAXLINE];

	while((line_l = getline(line, MAXLINE)) > 0){
		detab(line);
	}
}
/* exercise 1-20 */

/* exercise 1-21 */
/* Write a program entab that replaces strings of blanks by the minimum 
 * number of tabs and blanks to achieve the same spacing. */
void entab(char line[])
{
	char *p = line;
	int is_placeholder = 0;
	int blanks = 0;
	int i;

	while(*p){
		if(*p == ' '){
			++blanks;
			if((int)(p-line+1)%TABSTOP == 0){
				is_placeholder = 1;
			}
		}else{
			for(i=0;i<blanks;++i){
				putchar(' ');
			}
			blanks = 0;
			putchar(*p);
		}

		if(is_placeholder){
		    i = blanks/4 + 1;
		    /* Todo, either a tab or a single blank suffice,give a preference */
		    while(i){
			    printf("%c%c",'\\','t');
			    --i;
		    }
		    is_placeholder = 0;
		    blanks = 0;
		}
		++p;
	}
}

void e_1_21(void)
{
	int line_l;
	char line[MAXLINE];

	while((line_l = getline(line, MAXLINE)) > 0){
		entab(line);
	}
}
/* exercise 1-21 */

/* exercise 1-22 */
/* Write a program to ‘‘fold’’ long input lines into two or more shorter 
 * lines after the last non-blank character that occurs before the 
 * n-th column of input. */
#define NCOLUMN 300
#define LINE_LENGTH 80
void e_1_22(void)
{
	int c;
	int n = 0;
	int total = 0;

	while((c = getchar()) != '\n'){
		++total;
		if(total > NCOLUMN){
			putchar(c);
			continue;
		}

		++n;
		if((LINE_LENGTH - n < 5) && (LINE_LENGTH - n > -5)){
			if(c == ' ' || c == '\t'){
				putchar('\n');
				n = 0;
			}else{
				putchar(c);
			}
		}else if(n - LINE_LENGTH >= 5){
			putchar('\n');
			if(c != ' '){
				printf("%c%c",'\\','n');
				putchar(c);
			}
			n = 0;
		}else{
			putchar(c);
		}
	}
}
/* exercise 1-22 */

/* exercise 1-23 */
/* Write a program to remove all comments from a C program. */
void e_1_23(void)
{
	char *s = "jj/*"/*ppas'"kkk"oo'*/; /*this line is used to test*/
	int c;
	char prevc;
	int open_comm = 0;
	int close_comm = 0;
	int quotes = 0;
	int pure_ch = 0;

	prevc = c = getchar();
	while(c != EOF){
		if((pure_ch == 1 && prevc == '\'') || prevc == '\\'){
			goto LABEL;
		}
		if(c == '/'){
			if((c = getchar()) == '*' && quotes != 1){
				open_comm = 1;
			}else{
				if(open_comm!=1&&close_comm!=1){
					putchar('/');
				}
			}
		}else if(c == '"' && open_comm != 1 && close_comm != 1){
			++quotes;
		}else if(c == '*'){
			if((c = getchar()) == '/' && quotes != 1){
				close_comm = 1;
			}else{
				if(open_comm!=1&&close_comm!=1){
					putchar('*');
				}
			}
		}else if(c == '\'' && open_comm != 1 && close_comm != 1){
			++pure_ch;
		}

LABEL:
		if(open_comm != 1 && close_comm != 1){
			putchar(c);
		}
		if(close_comm == 1){
			open_comm = 0;
			close_comm = 0;
		}
		if(quotes == 2){
			quotes = 0;
		}
		if(pure_ch == 2){
			pure_ch = 0;
		}

		prevc = c;
		c = getchar();
	}

}
/* exercise 1-23 */

/* exercise 1-24 */
/* Write a program to check a C program for rudimentary syntax errors 
 * like unmatched parentheses, brackets and braces. */
#define MAX_NEST 20
void e_1_24(void)
{
	int c;
	char prevc;
	int open_comm = 0;
	int close_comm = 0;
	int quotes = 0;
	int pure_ch = 0;
	char ch[MAX_NEST];
	int ind = 0;
	int ln = 1;
	int line[MAX_NEST];
	int char_pos = 0;
	int char_p[MAX_NEST];
	int attion = 0;	/* just to deal with some trick case:if((*p = '\'') || ...); */

	prevc = c = getchar();
	while(c != EOF){
		++char_pos;
		if(c == '\n')
			++ln;

		if((attion == 0) && ((pure_ch == 1 && prevc == '\'') || prevc == '\\')){
			goto LABEL;
		}

		if(c == '/'){
			if((c = getchar()) == '*' && quotes != 1){
				open_comm = 1;
			}
			++char_pos;
			if(c == '\n')
				++ln;
		}else if(c == '*'){
			if((c = getchar()) == '/' && quotes != 1){
				close_comm = 1;
			}
			++char_pos;
			if(c == '\n')
				++ln;
		}

		if(open_comm == 1 || close_comm == 1){
			goto LABEL;
		}

	       	if(c == '{' || c == '[' || c == '('){
			if(quotes == 0 && ind < MAX_NEST - 1){
				line[ind] = ln;
				char_p[ind] = char_pos;
				ch[ind++] = c;
			}else if(ind > MAX_NEST - 1){
				printf("1111111111\n");
			}
		}else if(c == '"'){
			++quotes;
		}else if((c == '}' || c == ']' || c == ')') && quotes != 1){
			if(ind && ((c == ')' && ch[ind-1] == '(') ||
					(ch[ind-1] + 0x2 == c))){
				//printf("%d,%d %c<--->%d,%d%c matchs\n", 
						//line[ind-1], char_p[ind-1], ch[ind-1],ln,char_pos,c);
				--ind;
			}else{
				printf("error1:%d,%d %c mismatchs %d\n",ln,char_pos,c,ind);
			}
		}else if(c == '\''){
			++pure_ch;
		}else{
			;
		}

LABEL:
		if(quotes == 2){
			quotes = 0;
		}
		if(pure_ch == 2){
			pure_ch = 0;
		}
		if(close_comm == 1){
			open_comm = 0;
			close_comm = 0;
		}

		attion = 0;
		if(prevc == '\\' && (c == '\'' || c == '\\')){
			attion = 1;
		}
		prevc = c;
		c = getchar();
	}

	while(ind){
		--ind;
		printf("error:%d,%d %c mismatchs\n", line[ind],char_p[ind],ch[ind]);
	}
	printf("ln:%d\n", ln);

}
/* exercise 1-24 */

int main(void)
{
	//e_1_9();
	//e_1_10();
	//e_1_13();
	//e_1_16();
	//e_1_18();
	//e_1_19();
	//e_1_20();
	//e_1_21();
	//e_1_22();
	//e_1_23();
	e_1_24();

	return 0;
}
