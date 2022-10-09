#include "defines.h"

void swap_arr(int *a, int *b)
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
}


void sort_array(int *arr, int n)
{
	for (int i = 0; i < n; i++)
		for (int j = i; j < n - 1; j++)
		{
			if (arr[j] > arr[j + 1])
				swap_arr(arr + j, arr + j + 1);
		}
}
void sort_matrix(int **mtr, int n)
{
	int *arr = calloc(n * n, sizeof(int));
	int k = 0, i = 0, j = 0;
	for (i = 0; i < n - 1; i++)
		for (j = 0; j < n - 1 - i; j++)
			arr[k++] = mtr[i][j];
	for (i = n - 1; i >= 0; i--)
		for (j = n - i - 1; j < n - i; j++)
			arr[k++] = mtr[i][j];
	for (i = 1; i < n; i++)
		for (j = n - i; j < n; j++)
			arr[k++] = mtr[i][j];
	sort_array(arr, n * n);
	k = 0;
	for (i = 0; i < n - 1; i++)
		for (j = 0; j < n - 1 - i; j++)
			mtr[i][j] = arr[k++];
	for (i = n - 1; i >= 0; i--)
		for (j = n - i - 1; j < n - i; j++)
			mtr[i][j] = arr[k++];
	for (i = 1; i < n; i++)
		for (j = n - i; j < n; j++)
			mtr[i][j] = arr[k++];
	free(arr);
}
