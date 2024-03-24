#include <stdio.h>
#include <stdlib.h>

void rotate(char matrix[10][10])
{
	int y = 0;
	char r90;
	char r180;
	char r270;

/*
4/1 matrix:
		 0123456789
		0*********
		1 ******* 
		2  *****  
		3   ***   
		4    *    
		5
		6
		7
		8
		9
*/
	while(y <= 10/2)
	{
		for (int x = y; x < 9 - y; ++x)
		{
			r90 = matrix[x][9-y];
			r180 = matrix[9-y][9-x];
			r270 = matrix[9-x][y];

			matrix[x][9-y] = matrix[y][x];
			matrix[9-y][9-x] = r90;
			matrix[9-x][y] = r180;
			matrix[y][x] = r270;
		}
		y++;
	}
}
