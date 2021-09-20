#include <stdio.h>
#include <stdlib.h>
#include <err.h>

void byte_a_byte(FILE * file ){
	int c,ret;
	/* Read file byte by byte */
		while ((c = getc(file)) != EOF) {
			/* Print byte to stdout */
			ret=putc((unsigned char) c, stdout);

			if (ret==EOF){
				fclose(file);
				err(3,"putc() failed!!");
			}
		}
	
}

void number_bytes(FILE * file, int n_bytes){
	char c[n_bytes];


	while ( fread(c,n_bytes,sizeof(char),file) != 0){
		
		fwrite( c,n_bytes,sizeof(char),stdout);
	}

	
}
int main(int argc, char* argv[]) {
	FILE* file=NULL;
	

	if (argc!=2) {
		fprintf(stderr,"Usage: %s <file_name>\n",argv[0]);
		printf("Number of args: %d \n", argc);
		exit(1);
	}

	/* Open file */
	if ((file = fopen(argv[1], "r")) == NULL)
		err(2,"The input file %s could not be opened",argv[1]);	
	

	//byte_a_byte(file);
	number_bytes(file,8);

	fclose(file);

	return 0;
}
