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

char *mystrcpy (char *dest, const char *src)
{
    char *p = dest;

    if (!src || !dest) {            /* ensure src or dest is not NULL */
        fputs ("error: src or dest parameters NULL in mystrcpy().\n", stderr);
        exit (EXIT_FAILURE);
    }

    do                              /* loop */
        *p++ = *src;                /* copy each char in src to dest */
    while (*src++);                 /* (including the nul-termianting char) */

    return dest;                    /* return pointer to dest */
}

// Split
unsigned char ** f_split(unsigned char * data) {

	unsigned int lines = 0;
	unsigned int lineon = 0;
	
	size_t i = 0;
	size_t j;
	size_t b = 0;

	

	while (data[i] != '\0') {
		if (data[i] == '\n') { lines++; }
		printf("lining literly %c\n", data[i]);
		i++;
	}
	unsigned char ** out = malloc(lines + 0);
	i = 0;
	
	while (data[i] != '\0') {

		if (data[i] == '\n') {

			i++;

			out[lineon] = malloc(i - b);
			unsigned char *p = out[lineon];
			
			for (j = 0; j < i - b - 1; j++)
				*p++ = data[b + j];
			
			printf("LINE %uz: '%s'\n", lineon + 1, out[lineon]);
			lineon++;

			/*if (data[i] == '\n') {
				out[lineon] = malloc(1);
				lineon++;
			}*/
		
			b = i;	
		}
		i++;
	}

	return out;

}

/*
if (data[i] == '\n') {
			i++;
			if (data[i] == '\n') {
				out[lineon] = malloc(1);
				out[lineon][0] = '\0';
			} else {
				printf("buf this much %zu\n", i - b);
				out[lineon] = malloc(i - b);
				out[lineon][i - b + 1] = '\0';
				for (j = 0; j < i - b - 1; j++) {
					printf("buffering literly %zu %zu %zu %c\n", j, i, b, data[j]);
					out[lineon][j] = data[b + j];
				}
				printf("LINE %zu: '%s'\n", lineon, out[lineon]);
			}
			lineon++;
			b = i;
			printf("LINE 0 AGAIN: '%s'", out[0]);
		}
		*/
