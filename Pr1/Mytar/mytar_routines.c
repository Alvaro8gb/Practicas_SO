#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mytar.h"

extern char *use;

/** Copy nBytes bytes from the origin file to the destination file.
 *
 * origin: pointer to the FILE descriptor associated with the origin file
 * destination:  pointer to the FILE descriptor associated with the destination file
 * nBytes: number of bytes to copy
 *
 * Returns the number of bytes actually copied or -1 if an error occured.
 */
int copynFile(FILE * origin, FILE * destination, int nBytes){
	int nb_read = 0;
	int c ;

	if( origin == NULL || destination == NULL || nBytes < 0 ) 
		return -1;

	while ( (( c = getc(origin)) != EOF ) && nb_read < nBytes ) {

		if (putc((unsigned char) c, destination)==EOF){
			fclose(origin);
			fclose(destination);
			return -1;
		}

		nb_read++;
	}

	fseek(origin,-1,SEEK_CUR);

	return nb_read;
}
// int_MAX

/** Loads a string from a file.
 *
 * file: pointer to the FILE descriptor 
 * 
 * The loadstr() function must allocate memory from the heap to store 
 * the contents of the string read from the FILE. 
 * Once the string has been properly built in memory, the function returns
 * the starting address of the string (pointer returned by malloc()) 
 * 
 * Returns: !=NULL if success, NULL if error
 */

char* loadstr(FILE * file){

	int  c, size = 0; 
	char * str = NULL;

	if (file == NULL)  
		return NULL;

	do{
		c = getc(file) ;

		if (c == EOF)
			return NULL;
		else 
			size++;

	} while( c != (int) '\0' );
	
	str = malloc(size);

	fseek(file,-size, SEEK_CUR);

	fread(str,sizeof(char),size,file);

	return str;
	
}

/** Read tarball header and store it in memory.
 *
 * tarFile: pointer to the tarball's FILE descriptor 
 * nFiles: output parameter. Used to return the number 
 * of files stored in the tarball archive (first 4 bytes of the header).
 *
 * On success it returns the starting memory address of an array that stores 
 * the (name,size) pairs read from the tar file. Upon failure, the function returns NULL.
 */
stHeaderEntry* readHeader(FILE * tarFile, int *nFiles){

	stHeaderEntry * array_headers = NULL;
	int nr_files = 0,res;

	res = fread(&nr_files,sizeof(unsigned int),1,tarFile);

	if ( res == 0)
		return NULL;

	array_headers = malloc(sizeof(stHeaderEntry) * nr_files );

	for ( int i = 0 ; i < nr_files ; i++){

	 	array_headers[i].name = loadstr(tarFile);
		res = fread(&array_headers[i].size ,sizeof(unsigned int) ,1,tarFile);

		if(res == 0) 
			return NULL;

	}

	(*nFiles ) = nr_files;

	return array_headers;
}

/** Creates a tarball archive 
 *
 * nfiles: number of files to be stored in the tarball
 * filenames: array with the path names of the files to be included in the tarball
 * tarname: name of the tarball archive
 * 
 * On success, it returns EXIT_SUCCESS; upon error it returns EXIT_FAILURE. 
 * (macros defined in stdlib.h).
 *
 * HINTS: First reserve room in the file to store the tarball header.
 * Move the file's position indicator to the data section (skip the header)
 * and dump the contents of the source files (one by one) in the tarball archive. 
 * At the same time, build the representation of the tarball header in memory.
 * Finally, rewind the file's position indicator, write the number of files as well as 
 * the (file name,file size) pairs in the tar archive.
 *
 * Important reminder: to calculate the room needed for the header, a simple sizeof 
 * of stHeaderEntry will not work. Bear in mind that, on disk, file names found in (name,size) 
 * pairs occupy strlen(name)+1 bytes.
 *
 */
int createTar(int nFiles, char *fileNames[], char tarName[]){
	// Complete the function
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

/** Extract files stored in a tarball archive
 *
 * tarName: tarball's pathname
 *
 * On success, it returns EXIT_SUCCESS; upon error it returns EXIT_FAILURE. 
 * (macros defined in stdlib.h).
 *
 * HINTS: First load the tarball's header into memory.
 * After reading the header, the file position indicator will be located at the 
 * tarball's data section. By using information from the 
 * header --number of files and (file name, file size) pairs--, extract files 
 * stored in the data section of the tarball.
 *
 */
int extractTar(char tarName[])
{
	FILE * tar_file = fopen(tarName,"r");
	int nFiles = 0;
	stHeaderEntry * array_headers = readHeader(tar_file,&nFiles);

	for(int i = 0 ; i < nFiles ; i++){ 

		FILE * f = fopen(array_headers[i].name,"w");
		copynFile(tar_file,f,array_headers[i].size);
		fclose(f);

	}

	fclose(tar_file);				

	return EXIT_SUCCESS;
}
