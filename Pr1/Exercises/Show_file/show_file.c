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

	
	char * buffer = malloc (n_bytes * sizeof(char) ) ;


	while ( fread(buffer,n_bytes,sizeof(char),file) != 0)
		fwrite( buffer ,n_bytes,sizeof(char),stdout);
	

	free(buffer);

	
}
int main(int argc, char* argv[]) {
	FILE* file=NULL;
	

	if (argc!=2 && argc !=3 ) {
		fprintf(stderr,"Usage: %s <file_name>\n",argv[0]);
		printf("Number of args: %d \n", argc);
		exit(1);
	}

	/* Open file */
	if ((file = fopen(argv[1], "r")) == NULL)
		err(2,"The input file %s could not be opened",argv[1]);	
	

	if (argc == 2) 
		byte_a_byte(file);
	
	
	
	if (argc == 3){	
		printf("NUmber of bytes: %d\n " , *argv[2] );
		char c =  *argv[2];
		int n_bytes = c -'0';
		number_bytes(file,n_bytes);
	}

	fclose(file);

	return 0;
}
