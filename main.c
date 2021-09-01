// Import

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>

#include "file.h"

// Forward func decleration
void actualresize();
void redraw();
int main(void);

// Var decleration
struct winsize w;
unsigned int sizex, sizey, size;

void actualresize() {
	ioctl(0, TIOCGWINSZ, &w);
	sizex = w.ws_row;
	sizey = w.ws_col;
}

int main(void) {

	actualresize();

	int c;

	unsigned char * data = f_read("test.txt");
	unsigned char ** split = f_split(data);
	printf("\n WOO: %s", split[0]);
	//for (size_t i = 0; split[i] != NULL; ++i) {
		//printf((char *)split[i]);
		/*for (size_t j = 0; split[i][j] != '\0'; ++j) {
			return 0;
			//putchar(split[i][j]);
		}*/
	//}

	system("/bin/stty -ignbrk -brkint -ignpar -parmrk -inpck -istrip -inlcr -icrnl -ixon -ixoff -icanon -opost -isig -iuclc -ixany -imaxbel -xcase min 1 time 0");
	
	while((c=getchar()) != 3) {

	}


	system ("/bin/stty cooked");


	// Clear screen
	char *blank = malloc(sizey + 1);
	memset(blank, '\n', sizey);
	blank[sizey] = '\0';
	printf(blank);
	puts("\x1b[0m\x1b[H"); // reset modes and clear screen
	
}
