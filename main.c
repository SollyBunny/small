// Import

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>

#include "file.h"
#include "util.h"

// Forward func decleration
void actualresize();
void redraw();
void curset();
int main(void);

// Var decleration
struct winsize w;
unsigned int sizex, sizey, size, scrollx, scrolly;
unsigned int curx=0, cury=0;
unsigned char ** lines;
unsigned int * sizes;
unsigned int linelen;
unsigned int maxintsize;

void clear() {

	char *blank = malloc(sizey);
	memset(blank, '\n', sizey);
	fwrite(blank, 1, sizey, stdout);
	fwrite("\x1b[0m\x1b[H", 1, 7, stdout); // reset modes and clear screen
	free(blank);
	
}

void actualresize() {
	ioctl(0, TIOCGWINSZ, &w);
	sizey = w.ws_row;
	sizex = w.ws_col;
}

void redraw() {

	clear();

	unsigned int i;

	for (i = scrolly; (i < linelen) && (i < scrolly + sizey); ++i) {

		//printf("\x1b[48;5;237m%0*d\x1b[0m %d: %s\n", maxintsize, i + 1, sizes[i], lines[i]);
		if (lines[i]) {
			printf("\x1b[48;5;237m%0*d\x1b[0m %s\n", maxintsize, i + 1, lines[i]);
		} else {
			printf("\x1b[48;5;237m%0*d\x1b[0m\n", maxintsize, i + 1);
		}
	}
}

void curset() {
	printf("\x1b[%d;%dH", cury + 1, curx + maxintsize + 2);
}

int main(void) {

	actualresize();

	int c;

	unsigned char * data = f_read("test.txt");
	unsigned char * temp;
	
	lines = u_split(data, &sizes, &linelen);
	maxintsize = u_intsize(linelen);
	
	redraw();
	curset();

	//system("/bin/stty -ignbrk -brkint -ignpar -parmrk -inpck -istrip -inlcr -icrnl -ixon -ixoff -icanon -opost -isig -iuclc -ixany -imaxbel -xcase min 1 time 0");
	system("/bin/stty -ignbrk -igncr -inlcr -icanon -echo -ctlecho -parmrk");

	while((c=getchar()) != 3) {
		if (c == 27) {
			if ((c=getchar()) == 91) {
				c=getchar();

				switch (c) {
					case 66: //down
						if (cury == linelen - 1) // check if last line
							break;
						if (curx > sizes[cury + 1]) // check if new pointer pos is over max length of line
							curx = sizes[cury + 1];
						cury++;
						curset();
						break;
					case 65: //up
						if (cury == 0) // check if first line
							break;
						if (curx > sizes[cury - 1]) // check if new pointer pos is over max length of line
							curx = sizes[cury - 1];
						cury--;
						curset();
						break;
					case 68: //left
						if (curx == 0)
							break;
						curx--;
						curset();
						break;
					case 67:
						if (curx == sizes[cury])
							break;
						curx++;
						curset();
						break;
					default:
						printf(" CONTROL %d ", c);
				}

				// control character
				
				continue;
			}
		}

		
		if (c == 96) { // ` used as redraw
			clear();
		} else if (c > 96 && c < 123) {// a-z
			curx++;
			sizes[cury]++;
			lines[cury] = realloc(lines[cury], sizes[cury]);
			if (curx == sizes[cury]) {
				putchar(c);
				lines[cury][curx] = c;
			} else {
				//lines[0] = realloc(lines[0], sizes[cury]);
				unsigned int i = sizes[cury];
				do {
					--i;
					lines[cury][i + 1] = lines[cury][i];
				} while (i >= curx);
				lines[cury][curx - 1] = c;
				temp = lines[cury];
				temp += curx - 1;
				fwrite(temp, 1, sizes[cury] - curx + 1, stdout);
				curset();
				//putchar('\'');
			}
		} else {
			printf("\%d ", c);

		}

	}


	system ("/bin/stty cooked");


	clear();

	free(sizes);
	free(lines);
	free(data);

}
