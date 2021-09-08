#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

unsigned int u_intsize(size_t i) {

	if (i < 10) { return 1; }
	else if (i < 100) { return 2; }
	else if (i < 1000) { return 3; }
	else if (i < 10000) { return 4; }
	else if (i < 100000) { return 5; }
	else if (i < 1000000) { return 6; }
	else if (i < 10000000) { return 7; }
	else if (i < 100000000) { return 8; }
	else if (i < 1000000000) { return 9; }
	return 10;
	
}

unsigned char ** u_split(unsigned char * data, unsigned int ** size, unsigned int * linelen) {

	unsigned int lines = 1;
	unsigned int lineon = 0;
	
	unsigned int i = 0;
	unsigned int j;
	unsigned int b = 0;
	
	while (data[i] != '\0') {
		if (data[i] == '\n') { lines++; }
		i++;
	}
	*linelen = lines;
	
	unsigned char ** out  = malloc(lines * sizeof(void *));
	*size = malloc(lines * sizeof(unsigned int));
	i = 0;
	
	while (data[i] != '\0') {

		if (data[i] == '\n') {

		
			/*if ((data[i - 1] == '\n')) {
				out[lineon] = malloc(1);
				out[lineon][0] = '\0';
				lineon++;
				i++;
				b = i;
				continue;
			}*/
							
			i++;

			if (data[b] == '\n') {
				out[lineon] = malloc(1);
				out[lineon][0] = '\0';
				lineon++;
				b++;
				printf("%c %d\n", data[b], b);
			}				


			(*size)[lineon] = (i - b - 1);
			out[lineon] = malloc(i - b);
			//printf("%d %d\n", i - b, (*size)[i]);
			//printf("%d\n", (*size)[0]);
			
			unsigned char *p = out[lineon];
			for (j = 0; j < i - b - 1; j++)
				*p++ = data[b + j];
			*p++ = '\0';
			
			//out[lineon][i - b] = '\0';

			//printf("LINE %u: '%s'\n", lineon + 1, out[lineon]);
			lineon++;

			/*while (data[i] == '\n') {
				out[lineon] = malloc(1);
				out[lineon][0] = '\0';
				lineon++;
				i++;
				if (data[i] == '\0') {
					break;
				}
			}*/
			b = i;	
		}
		i++;
	}
	
	//out[lineon + 1] = malloc(1);
	//out[lineon + 1][0] = '\0';

	//out[512123] = 3; // crash for test
	
	return out;

}
