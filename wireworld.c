#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <assert.h>
#include <ncurses.h>
 
char world_40x40[2][2000];
 
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
	int generation;
  
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
		
		
		
		
		
		for (f = 0, generation = 0; generation < 1000 ; f = 1 - f, generation = generation+1) {
			printf("========================================\n");
			printf("Generation : %i\n", generation+1);
			puts(world_40x40[f]);
			next_world(world_40x40[f], world_40x40[1-f], 41, 40);
		}
	}
 
	return 0;
}