#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mytar.h"

extern char *use;

int copynFile(FILE * origin, FILE * destination, int nBytes){
	int nb_read = 0;
	int c , ret ;

	if( origin == NULL || destination == NULL || nBytes < 0 ) 
		return -1;

	while (  nb_read < nBytes && ((c = getc(origin)) != EOF) ) {

		ret = putc((unsigned char) c, destination);

		if (ret==EOF){
			fclose(origin);
			fclose(destination);
			return -1;
		}

		nb_read++;
	}

	return nb_read;
}


char* loadstr(FILE * file){

	int  c, size = 0; 
	char * str = NULL;

	if (file == NULL)  
		return NULL;

	do{
		c = getc(file) ;

		size++;

		if (c == EOF)
			return NULL;

	} while( c != (int) '\0' );
	
	str = malloc(size);

	fseek(file,-size, SEEK_CUR);

	fread(str,size,1,file);

	return str;
	
}
stHeaderEntry* readHeader(FILE * tarFile, int *nFiles){

	stHeaderEntry * array_headers = NULL;
	int nr_files = 0,res;

	res = fread(&nr_files,sizeof(unsigned int),1,tarFile);

	if ( res == 0)
		return NULL;

	array_headers = malloc(sizeof(stHeaderEntry) * nr_files );

	for ( int i = 0 ; i < nr_files ; i++){

	 	array_headers[i].name = loadstr(tarFile);
		res = fread(&array_headers[i].size ,sizeof(int) ,1,tarFile);

		if(res == 0) 
			return NULL;

	}

	(*nFiles ) = nr_files;

	return array_headers;
}

int createTar(int nFiles, char *fileNames[], char tarName[]){

	FILE * tar_file = fopen(tarName, "w");

	if (tar_file == NULL) 
		return EXIT_FAILURE;

	unsigned int offData = sizeof (int),str_len = 0 ;

	stHeaderEntry * array_headers = malloc(sizeof(stHeaderEntry) * nFiles );

	for ( int i = 0; i < nFiles ; i++){

		str_len = strlen(fileNames[i]) + 1;

		char * name = malloc(str_len);

		strcpy(name,fileNames[i]);

		array_headers[i].name = name ;

		offData+= sizeof(array_headers->size) + str_len;

	}

	fseek(tar_file, offData ,SEEK_SET);

	for(int i = 0; i < nFiles; i++){

		FILE * f = (FILE *)fopen(fileNames[i],"r");

		if (f == NULL)
			return EXIT_FAILURE;

		array_headers[i].size = copynFile(f,tar_file,INT_MAX);

		fclose(f);
	}

	fseek(tar_file, 0, SEEK_SET);

	fwrite(&nFiles, sizeof(int) , 1, tar_file); 


	for (int i = 0 ; i< nFiles ; i++){

		fwrite(array_headers[i].name , strlen(array_headers[i].name) + 1, 1, tar_file);
		fwrite(&array_headers[i].size, sizeof(int),1,tar_file);
	}

	for (int i = 0 ; i< nFiles ; i++)
		free(array_headers[i].name);
		
	free(array_headers);
	
	fclose(tar_file);

	return EXIT_SUCCESS;
}

int extractTar(char tarName[]){
	
	FILE * tar_file = fopen(tarName,"r");
	int nFiles = 0;
	stHeaderEntry * array_headers = readHeader(tar_file,&nFiles);

	for(int i = 0 ; i < nFiles ; i++){ 

		FILE * f = fopen(array_headers[i].name,"w");
		copynFile(tar_file,f,array_headers[i].size);
		fclose(f);

	}

	free(array_headers);
	fclose(tar_file);				

	return EXIT_SUCCESS;
}
