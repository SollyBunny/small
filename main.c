// Import

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/ioctl.h>

#include "file.h"
#include "util.h"

// Forward func decleration
void actualresize();
void redraw();
void resize();
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

//void clear() {

	/*char *blank = malloc(sizey);
	memset(blank, '\n', sizey);
	fwrite(blank, 1, sizey, stdout);
	fwrite("\x1b[0m\x1b[H", 1, 7, stdout); // reset modes and clear screen
	free(blank);*/
	
	
//}

void actualresize() {
	ioctl(0, TIOCGWINSZ, &w);
	sizey = w.ws_row;
	sizex = w.ws_col;
}

void resize() {
	redraw();
	actualresize();
}

void redraw() {

	fputs("\x1b[H\x1b[2J", stdout);

	unsigned int i;
	
	for (i = scrolly; !((i == linelen) || (i == scrolly + sizey - 1)); ++i) {
		//printf("\x1b[48;5;237m%0*d\x1b[0m %d: %s\n", maxintsize, i + 1, sizes[i], lines[i]);
		if (lines[i]) {
			printf("\x1b[48;5;237m%0*d\x1b[0m %s\n", maxintsize, i + 1, lines[i]);
		} else {
			printf("\x1b[48;5;237m%0*d\x1b[0m\n", maxintsize, i + 1);
		}
	}
	if (lines[i]) { // last line must be printed without \n
		printf("\x1b[48;5;237m%0*d\x1b[0m %s", maxintsize, i + 1, lines[i]);
	} else {
		printf("\x1b[48;5;237m%0*d\x1b[0m", maxintsize, i + 1);
	}
}

void curset() {
	printf("\x1b[%d;%dH", cury + 1 - scrolly, curx + maxintsize + 2);
}

int main(void) {

	signal(SIGWINCH, resize);

	actualresize();

	int c;

	unsigned char * data = f_read("test.txt");
	unsigned char * temp;
	
	lines = u_split(data, &sizes, &linelen);
	maxintsize = u_intsize(linelen);

	
	
	redraw();
	curset();

	//system("/bin/stty -ignbrk -brkint -ignpar -parmrk -inpck -istrip -inlcr -icrnl -ixon -ixoff -icanon -opost -isig -iuclc -ixany -imaxbel -xcase min 1 time 0");
	system("/bin/stty -echo -ixon -icanon min 1 intr undef"); //TODO change to exec

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
						if (cury - scrolly == sizey) {
							scrolly++;
							redraw();
						}
						curset();
						break;
					case 65: //up
						if (cury == 0) // check if first line
							break;
						if (curx > sizes[cury - 1]) // check if new pointer pos is over max length of line
							curx = sizes[cury - 1];
						cury--;
						if ((cury + 1) - scrolly == 0) {
							scrolly--;
							redraw();
						}
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
					case 72: // home
						curx = 0;
						curset();
						break;
					case 52: // end
						curx = sizes[cury];
						curset();
						c = getchar();
						break;
					default:
						printf(" CONTROL %d ", c);
				}

				// control character
				
				continue;
			}
		}

		
		if (c == 127) { // backspace

			if (curx == 0)
				continue;
			
			curx--;
			sizes[cury]--;

			
			for (unsigned int i = curx; i > sizes[cury]; ++i) {
				lines[cury][i] = lines[cury][i + 1];
			}
			lines[cury] = realloc(lines[cury], sizes[cury] + 1);
			lines[cury][sizes[cury]] = '\0';
			//printf("'%s'", lines[cury]);
			temp = lines[cury]; // create temp pointer and move it to curx
			temp += curx;
			fputs("\x1b[1D", stdout);
			fwrite(temp, 1, sizes[cury] - curx, stdout); // write out the section after the insert
			putchar(' ');
			curset(); // move the cursor to correct position
			//putchar('\'');

		} else if (c == 10) { // Enter
			
			if (curx == sizes[cury]) {

				cury++;
				linelen++;
				lines = realloc(lines, (1 + linelen) * sizeof(void *));

				unsigned int i;
				for (i = linelen - 1; i != cury; i--) {
					sizes[i] = sizes[i - 1];
					//lines[i] = realloc(lines[i], sizes[i]);
					lines[i] = lines[i - 1];
					
				}
				lines[cury] = malloc(2);
				sizes[cury] = 1; 
				lines[cury][0] = '!';
				lines[cury][1] = '\0';
				redraw();
				printf("%d %d\n", cury, linelen);
				curset();
				

				continue;
			}

		
		} else if ((c > 96 && c < 123) || c == 32) {// a-z
		
			curx++; // move cursor right 1
			sizes[cury]++; // add 1 to line size
			//printf("%d", sizes[cury]);
			lines[cury] = realloc(lines[cury], sizes[cury] + 1); // realloc line too add a new chr
			if (curx == sizes[cury]) { // if cursor is at end just appent it to the end
				putchar(c);
				lines[cury][curx] = c;
			} else { // otherwise move all the chrs right 1 after cursor and add it there
				//lines[0] = realloc(lines[0], sizes[cury]);
				// offset everything by 1 after curx
				unsigned int i;
				for (i = sizes[cury] - 1; i != curx - 1; i--) {
					lines[cury][i] = lines[cury][i - 1];
				}
				lines[cury][i] = lines[cury][i - 1];
				
				lines[cury][curx - 1] = c; // set the character (insert)
				temp = lines[cury]; // create temp pointer and move it to curx
				temp += curx - 1;
				fwrite(temp, 1, sizes[cury] - curx + 1, stdout); // write out the section after the insert
				curset(); // move the cursor to correct position
				//putchar('\'');
			}
			
		} else {
			printf("\%d ", c);

		}

	}


	system ("/bin/stty sane");
	fputs("\x1b[H\x1b[2J", stdout);
	
	free(sizes);
	free(lines);
	free(data);
	free(temp);

}
