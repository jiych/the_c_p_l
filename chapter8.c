#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

void filecopy_2(int fd1, int fd2)
{
#define BUFSIZE 1000
	int n;
	char buf[BUFSIZE];

	while((n = read(fd1, buf, sizeof buf)) > 0){
		write(fd2, buf, n);
	}
}

int e_8_1(int argc, char *argv[])
{
	int fd;
	char *prog = argv[0]; /* program name for errors */

	if(argc == 1)
		filecopy_2(0, 1);
	else
		while(--argc > 0)
			if((fd = open(argv[argc], O_RDONLY, 0)) == -1){
				fprintf(stderr, "%s: can’t open %s\n", prog, argv[argc]);
				exit(1);
			}else{
				filecopy_2(fd, 1);
				close(fd);
			}

	exit(0);
}

/* filecopy: copy file ifp to file ofp */
void filecopy(FILE *ifp, FILE *ofp)
{
	int c;
	while ((c = getc(ifp)) != EOF)
		putc(c, ofp);
}
int cat_from_ch7(int argc, char *argv[])
{
	FILE *fp;
	void filecopy(FILE *, FILE *);
	char *prog = argv[0]; /* program name for errors */
	if (argc == 1 ) /* no args; copy standard input */
		filecopy(stdin, stdout);
	else
		while (--argc > 0)
			if ((fp = fopen(*++argv, "r")) == NULL) {
				fprintf(stderr, "%s: can’t open %s\n",
						prog, *argv);
				exit(1);
			} else {
				filecopy(fp, stdout);
				fclose(fp);
			}
	if (ferror(stdout)) {
		fprintf(stderr, "%s: error writing stdout\n", prog);
		exit(2);
	}
	exit(0);
}

int main(int argc, char *argv[])
{
	
	//cat_from_ch7(argc, argv);
	e_8_1(argc, argv);

	return 0;
}
