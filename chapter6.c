#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define BUFSIZE 100
char buf[BUFSIZE]; /* buffer for ungetch */
int bufp = 0; /* next free position in buf */
int g_line = 0;
int getch(void) /* get a (possibly pushed-back) character */
{
	int retch;
	retch = (bufp > 0) ? buf[--bufp] : getchar();
	if(retch == '\n')
		++g_line;

	return retch;
}
void ungetch(int c) /* push character back on input */
{
	if (bufp >= BUFSIZE)
		printf("ungetch: too many characters\n");
	else
		buf[bufp++] = c;
	if(c == '\n')
		--g_line;

	return;
}
/* exercise 6-1 */
/* a better version of `getword` */
struct key {
	char *word;
	int count;
} keytab[] = {
	"auto", 0,
	"break", 0,
	"case", 0,
	"char", 0,
	"const", 0,
	"continue", 0,
	"default", 0,
	/* ... */
	"unsigned", 0,
	"void", 0,
	"volatile", 0,
	"while", 0
};
int getword(char *, int);
int binsearch(char *, struct key *, int);
#define MAXWORD 500
#define NKEYS sizeof(keytab) / sizeof(keytab[0])

void e_6_1(void)
{
	int n;
	char word[MAXWORD];

	while(getword(word, MAXWORD) != EOF){
		//printf("word:%s\n", word);
		if(isalpha(word[0])){
			if((n = binsearch(word, keytab, NKEYS)) >= 0){
				++keytab[n].count;
			}
		}
	}

	for(n = 0; n < NKEYS; ++n){
		if(keytab[n].count > 0)
			printf("%4d %s\n", keytab[n].count, keytab[n].word);
	}

	return;
}
int getword(char *word, int lim)
{
	int c;
	int is_special_def = 0;
	int is_special_comm = 0;
	int is_special_comm_new = 0;
	int is_special_con_d = 0;
	int is_special_con_s = 0;
	int is_special_und = 0;
	int maybe_comm_end = 0;
	int prv_backslash = 0;
	char *w = word;
	static int pre_def = 0;
#define ISSPECIAL(c)   ('#' == (c) || '/' == (c) || '"' == (c) || '\'' == (c) || '_' == (c) || '*' == (c))

	while(isspace(c = getch()))
		;
	if(c != EOF){
		*w++ = c;
	}
	if(pre_def == 1){
		while((c = getch()) != '\n' && --lim > 0){
			//printf("c:%c,%d\n", c, c);
			*w++ = c;
		}
		*w = 0;
		pre_def = 0;
		return word[0];
	}

	if(!isalpha(c) && !ISSPECIAL(c) && !isdigit(c)){
		*w = 0;
		return c;
	}

	pre_def--;
	if('_' == c){
	    is_special_und = 1;
	}else if('#' == c){
	    is_special_def = 1;
	    pre_def = 2;
	}else if('\'' == c){
		is_special_con_s = 1;
	}else if('"' == c){
		is_special_con_d = 1;
	}else if('/' == c){
		if((*w = getch()) == '*'){
			is_special_comm = 1;
			++w;
			--lim;
		}else if(*w == '/'){
			is_special_comm_new = 1;
			++w;
			--lim;
		}else{
			ungetch(*w); /* to keep same proc of other chars */
		}
	}

	for(; --lim > 0; ++w){
		*w = getch();
		if(is_special_und || is_special_def){
			if(!isalnum(*w) || isspace(*w)){
				ungetch(*w);
				is_special_und = 0;
				is_special_def = 0;
				--w;
				//break;
			}
		}else if(is_special_con_s){
			if('\'' == *w && prv_backslash == 0){
				is_special_con_s = 0;
			}
		}else if(is_special_con_d){
			if('"' == *w){
				is_special_con_d = 0;
				//break;
			}
		}else if(is_special_comm){
			if(maybe_comm_end && '/' == *w){
				is_special_comm = 0;
				//break;
			}

			maybe_comm_end = 0;
			if('*' == *w){
				maybe_comm_end = 1;
			}
		}else if(is_special_comm_new){
			if(*w == '\n'){
				is_special_comm_new = 0;
				break;
			}
		}else{
			if('_' == *w){
				is_special_und = 1;
			}else if(!isalnum(*w)){
				ungetch(*w);
				break;
			}
		}

		prv_backslash = ('\\' == *w && prv_backslash == 0) ? 1 : 0;
	}
	*w = 0;

	return word[0];
}

int binsearch(char *word, struct key tab[], int n)
{
	int cond;
	int low, high, mid;

	low = 0;
	high = n - 1;
	while(low <= high){
		mid = (low + high)/2;
		if((cond = strcmp(word, tab[mid].word)) < 0){
			high = mid - 1;
		}else if(cond > 0){
			low = mid + 1;
		}else{
			return mid;
		}
	}

	return -1;
}

struct tnode{
	char *word;
	int count;
	struct tnode *left;
	struct tnode *right;
};
struct tnode *addtree(struct tnode *, char *);
void treeprint(struct tnode *);

#define DEFAULT_STR_NUM 3
int g_str_num = DEFAULT_STR_NUM;
/* prints in alphabetical order each group of variable names */
void e_6_2(int argc, char *argv[])
{
	struct tnode *root;
	char word[MAXWORD];

	if(argc >= 2){
		g_str_num = atoi(argv[1]);
	}
	//printf("argc:%d,str_num:%d\n", argc, g_str_num);
	root = NULL;
	while(getword(word, MAXWORD) != EOF){
		if(isalpha(word[0])){
			root = addtree(root, word);
		}
	}

	treeprint(root);

	return;
}

struct tnode *talloc(void)
{
	return malloc(sizeof(struct tnode));
}
struct tnode *addtree(struct tnode *p, char *w)
{
	int cond;

	if(p == NULL){
		p = talloc();
		if(strlen(w) < g_str_num){
			p->word = strdup(w);
		}else{
			w[g_str_num] = 0;
			p->word = strdup(w);
		}
		p->count = 1;
		p->left = p->right = NULL;
	}else if((cond = strncmp(w, p->word, g_str_num)) == 0){
		p->count++;
	}else if(cond < 0){
		p->left = addtree(p->left, w);
	}else{
		p->right = addtree(p->right, w);
	}

	return p;
}

void treeprint(struct tnode *p)
{
	if(p != NULL){
		treeprint(p->left);
		printf("%4d %s\n", p->count, p->word);
		treeprint(p->right);
	}

	return;
}

/*
 * Write a cross-referencer that prints a list of all words in a document, 
 * and for each word, a list of the line
 * numbers on which it occurs. Remove noise words like ‘‘the,’’ ‘‘and,’’ 
 * and so on.
 * */
struct s_cr *addtree_cr(struct s_cr *p, char *word);
void treeprint_cr(struct s_cr *p);
int noise_words(char *word);
void e_6_3(void)
{
	struct s_cr *root;
	char word[MAXWORD];

	root = NULL;
	while(getword(word, MAXWORD) != EOF){
		if(isalpha(word[0]) && !noise_words(word))
			root = addtree_cr(root, word);
	}

	treeprint_cr(root);
	//destroytree_cr();

	return;
}

/* 
 * noise word ret 1,else 0
 * */
int noise_words(char *word)
{
	char *noise_wds[] = {"and", "the", "i", "|", "you"};
	int i;
	i = sizeof(noise_wds) / sizeof(noise_wds[0]);
	while(--i >= 0){
		if(!strncmp(word, noise_wds[i], strlen(noise_wds[i])))
			return 1;
	}

	return 0;
}

#define MAXLINES 1000
typedef struct s;
typedef struct t;
struct s_cr{
	char *ps;
	struct t_cr *q;
	struct s_cr *left;
	struct s_cr *right;
};
struct t_cr{
	int lines[MAXLINES];
	int line_no;
	struct s_cr *p;
};

struct s_cr *salloc_cr()
{
	return malloc(sizeof(struct s_cr));
}

struct t_cr *talloc_cr()
{
	return malloc(sizeof(struct t_cr));
}

struct s_cr *addtree_cr(struct s_cr *p, char *word)
{
	int cond;

	if(p == NULL){
	    p = salloc_cr();
	    p->ps = strdup(word);
	    p->q = talloc_cr();
	    p->q->p = p;
	    p->q->line_no = 1;
	    p->q->lines[0] = g_line + 1;
	}else if((cond = strcmp(p->ps, word)) == 0){
		if(p->q->line_no >= MAXLINES){
			printf("too many lines:%d\n", p->q->line_no);
			return p;
		}
		p->q->lines[p->q->line_no++] = g_line + 1;
	}else if(cond < 0){
		p->right = addtree_cr(p->right, word);
	}else{
		p->left = addtree_cr(p->left, word);
	}

	return p;
}

void treeprint_cr(struct s_cr *p)
{
	int i;

	if(p != NULL){
		treeprint_cr(p->left);
		printf("word:(%s)\n", p->ps);
		if(p->q != NULL){
			for(i = 0; i < p->q->line_no; ++i){
				printf("\tline:%4d\n", p->q->lines[i]);
			}
		}else{
			printf("something err\n");
		}
		treeprint_cr(p->right);
	}
}

/*
 * Write a program that prints the distinct words in its input sorted into 
 * decreasing order of frequency of occurrence. 
 * Precede each word by its count.
 * */
void e_6_4(void)
{
	printf("i don't know\n");
}

struct nlist{
	struct nlist *next;
	char *name;
	char *defn;
};
#define HASHSIZE 101
static struct nlist *hashtab[HASHSIZE];
/* hash: form hash value for string s */
unsigned hash(char *s)
{
	unsigned hashval;
	for(hashval = 0; *s != '\0'; s++)
		hashval = *s + 31 * hashval;

	return hashval % HASHSIZE;
}
/* lookup: look for s in hashtab */
struct nlist *lookup(char *s)
{
	struct nlist *np;

	for(np = hashtab[hash(s)]; np != NULL; np = np->next)
		if(np->name != NULL && strcmp(s, np->name) == 0)
			return np;

	return NULL;
}
/* install: put (name, defn) in hashtab */
struct nlist *install(char *name, char *defn)
{
	struct nlist *np;
	unsigned hashval;

	if((np = lookup(name)) == NULL){
		np = (struct nlist *)malloc(sizeof(*np));
		if(np == NULL || (np->name = strdup(name)) == NULL)
			return NULL;
		hashval = hash(name);
		np->next = hashtab[hashval];
		hashtab[hashval] = np;
	}else{
		free((void*)np->defn);
		np->defn = NULL;
	}

	if((np->defn = strdup(defn)) == NULL)
		return NULL;

	return np;
}
void show()
{
	struct nlist *np;
	struct nlist *npn;
	int i = 0;

	for(np = hashtab[i]; i < HASHSIZE; i++){
		np = hashtab[i];
		for(npn = np; npn != NULL; npn = np->next){
			printf("name:%s,def:%s\n", npn->name ? npn->name : "nil", npn->defn ? npn->defn : "nil");
		}
	}
}
int undef(char *name)
{
	struct nlist *np;

	if((np = lookup(name)) != NULL){
		if(np->defn != NULL){
			free(np->defn);
			np->defn = NULL;
		}
		return 1;
	}

	return 0;
}
/*
 * Write a function undef that will remove a name and definition from the 
 * table maintained by lookup and install.
 * */
void e_6_5(void)
{
	install("name", "jyc");
	install("sex", "boy");
	show();
	undef("name");
	undef("name");
	undef("aksdka");
	show();
}

/*
 * Implement a simple version of the #define processor (i.e., no arguments) suitable for use with C programs,
 * based on the routines of this section.
 * */
void e_6_6(void)
{
	char word[MAXWORD];
	char macro_cnt = 0;
	char name[MAXWORD];
	char defn[MAXWORD];

	while(getword(word, MAXWORD) != EOF){
		if(macro_cnt == 2){
			strcpy(name, word);
			macro_cnt = 1;
		}else if(macro_cnt == 1){
			strcpy(defn, word);
			macro_cnt = 0;
			printf("install:%s|%s\n", name, defn);
			install(name, defn);
		}
		if(strcmp(word, "#define") == 0)
			macro_cnt = 2;
	}
	show();
}
int main(int argc, char *argv[])
{
	//e_6_1();
	//e_6_2(argc, argv);
	//e_6_3();
	//e_6_5();
	e_6_6();

	return 0;
}
