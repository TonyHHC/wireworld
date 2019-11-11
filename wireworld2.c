/* 
	How to make : 
		gcc wireworld2.c neillncurses.c -lncurses -o wireworld2
		
	How to execute :
		wireworld2 w5.txt
*/

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <ncurses.h>

#define KEY_ESC 27
 
char world_40x40[2][2000];

void NCURS_Delay(double ms){

    const clock_t start = clock();
    clock_t current;
    do{
        current = clock();
    }while((double)(current-start)/CLOCKS_PER_SEC < (ms/1000.0));

}
 
void next_world(const char *in, char *out, int w, int h)
{
	int i;
 
	for (i = 0; i < w*h; i++)
	{
		switch (in[i]) 
		{
			case ' ':
				out[i] = ' '; break;
			case 't': 
				out[i] = 'c'; break;
			case 'H': 
				out[i] = 't'; break;
			case 'c': 
				{
					int hc = (in[i-w-1] == 'H') + (in[i-w] == 'H') + (in[i-w+1] == 'H') + (in[i-1]   == 'H') + (in[i+1]   == 'H') + (in[i+w-1] == 'H') + (in[i+w] == 'H') + (in[i+w+1] == 'H');
					out[i] = (hc == 1 || hc == 2) ? 'H' : 'c';
					break;
				}
			default:
				out[i] = in[i];
		}
	}
	out[i] = in[i];
}
 
int main(int argc, char *argv[])
{
	int f;
	int ch;
	int generation;
	int bStop = 0;
	
	int i, row, col;
	
	char c;
  
	int fd = open(argv[1], O_RDONLY);
	
	assert(fd !=-1);
  
	if (fd ==-1)
	{
		printf("Open File Error!!\n");
		return -1;
	}
	else 
	{
		read(fd, world_40x40[0], 1700);
		close(fd);
		
		initscr();
		
		if (has_colors() == FALSE)
		{
			endwin();
			printf("Your terminal does not support color\n");
			return -1;
		}
		
		start_color();
		
		init_pair(0, COLOR_BLACK, COLOR_BLACK);
		init_pair(1, COLOR_BLACK, COLOR_RED);
		init_pair(2, COLOR_BLACK, COLOR_BLUE);
		init_pair(3, COLOR_BLACK, COLOR_GREEN);
		
		raw();
		noecho();
		timeout(0);
		
		f = 0;
		while(bStop == 0) 
		{
			if((ch = getch()) != ERR)
			{
				switch(ch)
				{
					/* Any mouse press */
					case KEY_MOUSE:
					/* ESC key pressed - why not defined in ncurses.h ? */
					case KEY_ESC:
					case 'q':
					case 'Q':
						bStop = 1;
						break;
				}
			}
				
			clear();
			curs_set(0);
			
			for(i=0;i<1700;i++)
			{
				c = world_40x40[f][i];
				
				row = i % 41;
				col = i / 41;
				
				move(col, row);
				
				switch(c)
				{
					case 't':
						attron(COLOR_PAIR(1));
						addch(c);
						break;
					case 'H':
						attron(COLOR_PAIR(2));
						addch(c);
						break;
					case 'c':
						attron(COLOR_PAIR(3));
						addch(c);
						break;
				}
			}
			
			refresh();
			next_world(world_40x40[f], world_40x40[1-f], 41, 40);
			f = 1-f;
			NCURS_Delay(500);
		}
		
		endwin();
	}
 
	return 0;
}