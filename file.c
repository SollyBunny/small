 // Import
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>

#include "file.h"

// Get a files size
unsigned int f_size(const char * f) {
	struct stat sb;
	stat (f, &sb);
	return sb.st_size;
}

// Read file content
unsigned char * f_read(const char * f) {

	// get file size
	unsigned int s = f_size(f);

	// malloc content
	unsigned char * contents = malloc(s + 1);
	contents[s] = '\0';

	// read file
	FILE *file = fopen(f, "r");
	fread(contents, sizeof(unsigned char), s, file);

	// close & return
	fclose(file);
	return contents;

}

// Write file content
void f_write(const char * f, unsigned char * contents) {

	// write file
	FILE *file = fopen(f, "w");
	fwrite(contents, sizeof(char), sizeof(contents), file);

	// close & return
	fclose(file);

}

