#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#undef NULL

#define OPEN_MAX 20
#define NULL 0
#define EOF (-1) 
#define BUFSIZE (1024) 
enum _flags{
	_READ  = 01,
	_WRITE = 02,
	_UNBUF = 04,
	_EOF   = 010,
	_ERROR = 020,
};
typedef struct _iobuf{
	int cnt;
	char *ptr;
	char *base;
#ifdef WITH_FIELDS
	struct{
		int read :1;
		int write:1;
		int unbuf:1;
		int eof  :1;
		int err  :1;
	}flag;
#else
	int flag;
#endif
	int fd;
}FILE;
#ifdef WITH_FIELDS
FILE _iob[OPEN_MAX] = 
{
	{0, NULL, NULL, {.read=1,.write=0,.unbuf=0,.eof=0,.err=0}, 0},
	{0, NULL, NULL, {.read=0,.write=1,.unbuf=0,.eof=0,.err=0}, 1},
	{0, NULL, NULL, {.read=0,.write=1,.unbuf=1,.eof=0,.err=0}, 2},
};
#else
FILE _iob[OPEN_MAX] = 
{
	{0, NULL, NULL, _READ, 0},
	{0, NULL, NULL, _WRITE, 1},
	{0, NULL, NULL, _WRITE | _UNBUF, 2},
};
#endif
#define stdin  &(_iob[0])
#define stdout &(_iob[1])
#define stderr &(_iob[2])


#ifdef WITH_FIELDS
#define feof(p) (((p)->flag.eof) != 0)
#define ferror(p) (((p)->flag.err) != 0)
#else
#define feof(p) (((p)->flag & _EOF) != 0)
#define ferror(p) (((p)->flag & _ERROR) != 0)
#endif

#define fileno(p) ((p)->fd)

#define getc(p) ((--(p)->cnt >= 0) ? (unsigned char)*(p)->ptr++ : _fillbuf(p))
#define putc(x, p) ((--(p)->cnt >= 0) ? *(p)->ptr++ = (x) : _flushbuf((x), p))
#define PERMS 0666

FILE *fopen(char *name, char *mode)
{
	int fd;
	FILE *fp;

	if(*mode != 'r' && *mode != 'w' && *mode != 'a')
		return NULL;

	for(fp = _iob; fp < _iob + OPEN_MAX; fp++)
#ifdef WITH_FIELDS
		if((fp->flag.read | fp->flag.write) == 0)
#else
		if((fp->flag & (_READ | _WRITE)) == 0)
#endif
			break;

	if(fp >= _iob + OPEN_MAX)
		return NULL;

	if(*mode == 'w')
		fd = creat(name, PERMS);
	else if(*mode == 'a'){
		if((fd = open(name, O_WRONLY, 0)) == -1)
			fd = creat(name, PERMS);
		lseek(fd, 0L, 2);
	}else
		fd = open(name, O_RDONLY, 0);

	if(fd == -1)
		return NULL;
	fp->fd = fd;
	fp->cnt = 0;
	fp->base = NULL;
#ifdef WITH_FIELDS
	if(*mode == 'r')
	       	fp->flag.read =  1;
	else
	       	fp->flag.write = 1;
#else
	fp->flag = (*mode == 'r') ? _READ : _WRITE;
#endif

	return fp;
}

int _fillbuf(FILE *fp)
{
	int bufsize;

#ifdef WITH_FIELDS
	if(fp->flag.read != 1 || fp->flag.eof != 0 || fp->flag.err != 0)
#else
	if((fp->flag & (_READ | _EOF | _ERROR)) != _READ)
#endif
		return EOF;
#ifdef WITH_FIELDS
	bufsize = (fp->flag.unbuf) ? 1 : BUFSIZE;
#else
	bufsize = (fp->flag & _UNBUF) ? 1 : BUFSIZE;
#endif
	if(fp->base == NULL)
		if((fp->base = (char*)malloc(bufsize)) == NULL)
			return EOF;
	fp->ptr = fp->base;
	fp->cnt = read(fp->fd, fp->ptr, bufsize);
	if(--fp->cnt < 0){
		if(fp->cnt == -1)
#ifdef WITH_FIELDS
			fp->flag.eof = 1;
#else
			fp->flag |= _EOF;
#endif
		else
#ifdef WITH_FIELDS
			fp->flag.err = 1;
#else
			fp->flag |= _ERROR;
#endif
		fp->cnt = 0;
		return EOF;
	}

	return (unsigned char) *fp->ptr++;
}

int _flushbuf(char x, FILE *fp)
{
	int bufsize;

#ifdef WITH_FIELDS
	if(fp->flag.write != 1 || fp->flag.eof != 0 || fp->flag.err != 0)
#else
	if((fp->flag & (_WRITE | _EOF | _ERROR)) != _READ)
#endif
		return EOF;
#ifdef WITH_FIELDS
	bufsize = (fp->flag.unbuf) ? 1 : BUFSIZE;
#else
	bufsize = (fp->flag & _UNBUF) ? 1 : BUFSIZE;
#endif
	if(fp->base == NULL){
		if((fp->base = (char*)malloc(bufsize)) == NULL)
			return EOF;
	}else{
		if(write(fp->fd, fp->ptr, bufsize) != bufsize){
#ifdef WITH_FIELDS
			fp->flag.err = 1;
#else
			fp->flag |= _ERROR;
#endif
		}
	}

	fp->ptr = fp->base;
	fp->cnt = bufsize - 1;

	return (unsigned char) (*fp->ptr++ = x);
}

int fflush(FILE *fp)
{
#ifdef WITH_FIELDS
	if(fp->flag.write != 1 || fp->flag.eof != 0 || fp->flag.err != 0)
#else
	if((fp->flag & (_WRITE | _EOF | _ERROR)) != _READ)
#endif
		return EOF;

	if(fp->base == NULL)
		return 0;
	if(write(fp->fd, fp->base, fp->cnt) != fp->cnt){
#ifdef WITH_FIELDS
		fp->flag.err = 1;
#else
		fp->flag |= _ERROR;
#endif
		return EOF;
	}
	fp->ptr = fp->base;
	fp->cnt = 0;

	return 0;
}

int fclose(FILE *fp)
{
#ifdef WITH_FIELDS
	if(fp->flag.write == 1){
#else
	if(fp->flag & _WRITE){
#endif
		if(fflush(fp) != 0)
			return EOF;
	}

	free(fp->base);
#ifdef WITH_FIELDS
	fp->flag.read = 0;
	fp->flag.write = 0;
#else
	fp->flag = 0;
#endif
	close(fp->fd);

	return 0;
}

void *calloc(unsigned n, unsigned size)
{
	void *p;

	p = malloc(n*size);
	if(p != NULL)
		memset(p, 0, n*size);

	return p;
}

int main(void)
{
	int i=0;
	FILE *fp;
	char name[20];

	for(;i<50;i++){
		sprintf(name, "tmp/test%d", i%20);
		fp = fopen(name, "r");
		if(fp == NULL)
			printf("open %s err!\n", name);
		else{
		//	printf("open ok!\n");
			fclose(fp);
		}
	}

	return 0;
}
