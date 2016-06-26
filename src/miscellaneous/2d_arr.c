#include "misc.h"

#define MAX_ROWS 5
#define MAX_COLS 5

/*
 * This file has implementation for 2d array using pointers.
 */
void two_d_arr()
{

	int **arr = NULL;
	int i, j;

	/*
	 * Allocate memory for base pointer.
	 */
	arr = (int **) malloc(sizeof (int*) * MAX_ROWS);

	for (i = 0; i < MAX_COLS; i++)
	{
		arr[i] = (int *) malloc(sizeof (int) * MAX_COLS);
	}

	printf("2-Dimensional array using malloc\n");
	for (i = 0; i < MAX_ROWS; i++)
	{
		for (j = 0; j < MAX_COLS; j++)
		{
			arr[i][j] = i * j;
			printf ("%d ", arr[i][j]);
		}
		printf("\n");
	}

	for (i = 0; i < MAX_COLS; i++)
	{
		free(arr[i]);
	}

	free(arr);

}

