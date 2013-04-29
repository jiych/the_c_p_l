#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

/*
 * Write a program that converts upper case to lower or lower case to 
 * upper, depending on the name it is invoked with, as found in argv[0].
 *
 * */
void e_7_1(int argc, char *argv[])
{
	int c;

	if(strcmp(argv[0], "./ltou") == 0){
		while((c = getchar()) != EOF){
			putchar(toupper(c));
		}
	}else if(strcmp(argv[0], "./utol") == 0){
		while((c = getchar()) != EOF){
			putchar(tolower(c));
		}
	}else{
		printf("err para!\n");
	}

	return;
}

void minprintf(char *fmt, ...)
{
	va_list ap;
	int ival = 0;
	char *p, *sval;
	double dval;
	int left_assign = 0;
	int assign_n = 0;
	char strval[200];
	int i = 0;
	
	va_start(ap, fmt);
	for(p = fmt; *p; p++){
		if(left_assign != 1 && *p != '%'){
			putchar(*p);
			continue;
		}else if(*p == '%'){
			p++;
		}
		while(isdigit(*p)){
			assign_n = 10*assign_n + *p - '0';
			p++;
		}
		//printf("assign_n:%d\n", assign_n);

		switch(*p){
			case '-':
				left_assign = 1;
				break;
			case 'd':
				dval = va_arg(ap, double);
				printf(left_assign ? left_assign = 0,"%-*f" : "%*f", assign_n, dval);
				break;
			case 'i':
				ival = va_arg(ap, int);
				printf(left_assign?left_assign=0,"%-*d":"%*d", assign_n, ival);
				break;
			case 's':
				for(sval = va_arg(ap, char *); *sval; sval++){
					strval[i++] = *sval;
				}
				printf(left_assign?left_assign=0,"%-*s":"%*s", assign_n, strval);
				strval[0] = '\0';
				i = 0;
				break;
			case 'c':
				ival = va_arg(ap, int);
				printf("%c", ival);
				break;
			case 'x':
				ival = va_arg(ap, int);
				printf(left_assign?left_assign=0,"%-x":"%x", ival);
				break;
			default:
				putchar(*p);
				break;
		}
		assign_n = 0;
	}
	va_end(ap);

	return;
}

/*
 * Revise minprintf to handle more of the other facilities of printf.
 *
 * */
void e_7_3(void)
{
	minprintf("arg1:%10d,arg2:%4i,arg3:%-3s,arg4:%%,arg5:%5x", 3.1,4,"hi", 90);
}

void minscanf(char *fmt, ...)
{
	va_list ap;
	char *p;
	float fval;
	int ival;
	int xval;
	float *fp;
	int *ip;
	int *xp = NULL;
	int assign_n = 0;
	char scan_fmt[20] = "";
	int pre_arg = 0;

	va_start(ap, fmt);
	for(p = fmt; *p; p++){
		if(*p == '%'){
			p++;
			pre_arg = 1;
		}else if(pre_arg != 1){
			continue;
		}
		pre_arg = 0;
		while(isdigit(*p)){
			assign_n = 10*assign_n + *p - '0';
			p++;
		}
		switch(*p){
			case 'f':
				if(assign_n != 0)
					sprintf(scan_fmt, "%%%df", assign_n);
				else
					strcpy(scan_fmt, "%f");
				//printf("fmt:%s\n", scan_fmt);
				fp = va_arg(ap, float*);
				scanf(scan_fmt, &fval);
				*fp = fval;
				break;
			case 'd':
				if(assign_n != 0)
					sprintf(scan_fmt, "%%%dd", assign_n);
				else
					strcpy(scan_fmt, "%d");
				//printf("fmt:%s\n", scan_fmt);
				ip = va_arg(ap, int*);
				scanf(scan_fmt, &ival);
				*ip = ival;
				break;
			case 'x':
				if(assign_n != 0)
					sprintf(scan_fmt, "%%%dx", assign_n);
				else
					strcpy(scan_fmt, "%x");
				//printf("fmt:%s\n", scan_fmt);
				xp = va_arg(ap, int*);
				scanf(scan_fmt, &xval);
				*xp = xval;
				break;
			case '*':		    /* %* skipped */
				break;
			default:
				break;
		}
		assign_n = 0;
	}
	va_end(ap);

	return;
}

/*
 * Write a private version of scanf analogous to minprintf from the previous 
 * section.
 * */
void e_7_4(void)
{
	float fval = 0.0;
	int ival = 0;
	int xval = 0;
	minscanf("%4d,%3f,%*d, %6x", &ival, &fval, &xval);
	printf("ival:%d,fval:%f,xval:%x\n", ival, fval, xval);
}

/*
 * Rewrite the postfix calculator of Chapter 4 to use scanf and/or sscanf to do 
 * the input and number conversion. */
void push(double f);
double pop(void);
/* this program is from cls-wiki.net */
void e_7_5(void)
{
	char *c;
	char s[100], buf[100];
	double a = 0, op2;
	char e = '\0';

	while (scanf("%s%c", s, &e) == 2) { /* get no-space string and space behind it */
		if (sscanf(s, "%lf", &a) == 1) /* is it a number */
			push(a);
		else if (sscanf(s, "%s", buf)) {
			for (c = buf ; *c; c++) {
				switch (*c) {
					case '+':
						push(pop() + pop());
						break;
					case '-':
						op2 = pop();
						push(pop() - op2);
						break;
					case '*':
						push(pop() * pop());
						break;
					case '/':
						op2 = pop();
						if (op2 != 0.0)
							push(pop() / op2);
						else
							printf("error: zero divisor\n");
						break;
					default:
						printf("Unknown command\n");
						break;
				}
			} /* for */
			if (e == '\n') /* print result */
				printf("\t%.8g\n", pop());
		}
	}
	return;
}
#define MAXVAL 100 /* maximum depth of val stack */
int sp = 0; /* next free stack position */
double val[MAXVAL]; /* value stack */
double g_top = 0;
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

/*
 * Write a program to compare two files, printing the first line where 
 * they differ.
 * */
#define MAXLEN 200
void e_7_6(int argc, char *argv[])
{
	FILE *fp1, *fp2;
	char line1[MAXLEN], line2[MAXLEN];
	int same_files = 0;
	line1[0] = '\0';
	line2[0] = '\0';

	if(argc != 3){
		fprintf(stderr, "usage:./a.out file1 file2\n");
		return;
	}

	fp1 = fopen(argv[1], "r");
	fp2 = fopen(argv[2], "r");
	if(fp1 == NULL || fp2 == NULL){
		fprintf(stderr, "err, when open files\n");
		return;
	}

	while(fgets(line1, MAXLEN, fp1) && fgets(line2, MAXLEN, fp2)){
			if(strcmp(line1, line2) != 0){
				break;
			}
	}
	if(feof(fp1)){
	      if(fgets(line2, MAXLEN, fp2) == NULL){
			printf("'%s' and '%s' are same\n", argv[1], argv[2]);
			same_files = 1;
	      }
	}
	if(same_files == 0){
		printf("the first diff between '%s' and '%s':\n", argv[1], argv[2]);
		if(strlen(line1) > 1 && strlen(line2) > 1){
			line1[strlen(line1)-1] = '\0';
			line2[strlen(line2)-1] = '\0';
		}
		printf("%s<----->%s\n", line1, line2);
	}

	fclose(fp1);
	fclose(fp2);

	return;
}

/*
 * Modify the pattern finding program of Chapter 5 to take its input from 
 * a set of named files or, if no files are named as arguments, from the 
 * standard input.
 * */
int e_7_7(int argc, char *argv[])
{
	char line[MAXLEN];
	int c, except = 0, found = 0, number = 0;
	long lineno = 0;
	int i = 0;
	char pattern[50];
	char *files[50];
	FILE *fp[50];
	int ifiles = 0;

	while(--argc > 0 && (*++argv)[0] == '-'){
		while((c = *++argv[0])){
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
	if(argc < 1){
		printf("Usage:find -x -n pattern [files]\n");
	}else{
		strcpy(pattern, *argv++);
		while(--argc){
			printf("files:%s\n", *argv);
			files[ifiles++] = strdup(*argv);
			if(ifiles >= 50){
				fprintf(stderr, "warn,too many files\n");
				break;
			}
			argv++;
		}
		for(i = 0; i < ifiles; ++i){
			fp[i] = fopen(files[i], "rb"); 
			if(fp[i] == NULL)
				fprintf(stderr, "warn,can't open '%s'\n", files[i]);
		}
		for(i = 0; i < ifiles; ++i){
			while(fp[i] && fgets(line, MAXLEN, fp[i]) > 0){
				lineno++;
				if((strstr(line, pattern) != NULL) != except){
					if(number)
						printf("%ld:", lineno);
					printf("%s\n", line);
					found++;
				}
			}
		}
	}

	for(i = 0; i < ifiles; ++i){
		if(fp[i] != NULL)
			fclose(fp[i]);
	}
	return found;
}

/*
 * Write a program to print a set of files, starting each new one on a 
 * new page, with a title and a running page count for each file.
 * */
void e_7_8(int argc, char *argv[])
{
	FILE *fp;
	int pagenum = 0;
	int c;

	while(--argc){
		fp = fopen(argv[argc], "rb");
		if(fp == NULL){
			fprintf(stderr, "warn,can't open '%s'\n", argv[argc]);
			continue;
		}
		printf("--begin-----filename:%s,pagenum:%d----\n", argv[argc], pagenum);
		while((c = fgetc(fp)) != EOF){
			fputc(c, stdout);
		}
		printf("--end-----filename:%s,pagenum:%d----\n", argv[argc], pagenum);

		fclose(fp);
		pagenum++;
	}
}

int main(int argc, char *argv[])
{
	//e_7_1(argc, argv);
	//e_7_3();
	//e_7_4();
	//e_7_5();
	//e_7_6(argc, argv);
	//e_7_7(argc, argv);
	e_7_8(argc, argv);

	return 0;
}
