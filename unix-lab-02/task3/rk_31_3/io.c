#include "defines.h"


void free_matrix(int ***mtr)
{
	free(*mtr);
}

int **allocate_matrix(int n)
{
	int **data;
	data = malloc(n * sizeof(int*) + n * n * sizeof(int));
	if (!data)
		return NULL;
	for (int i = 0; i < n; i++)
		data[i] = (int*)((char*) data + n * sizeof(int*) + i * n * sizeof(int));
	return data;
}

int read_file(FILE *f, int **mtr, int n)
{
	int rc = OK, tmp;
	for (int i = 0; i < n && rc == OK; i++)
		for (int j = 0; j < n && rc == OK; j++)
		{
			if (fscanf(f, "%d", &tmp) == 1)
			{
				mtr[i][j] = tmp;
			}
			else
				rc = ERR_READ;
		}
	if (fscanf(f, "%d", &tmp) != EOF)
		rc = ERR_READ;
	return rc;
}


int read_matrix(char *filename, int ***mtr, int *n)
{
	FILE *f = NULL;
	int rc = OK;
	f = fopen(filename, "r");
	if (f == NULL)
		return ERR_READ;
	if (fscanf(f, "%d", n) == 1 && *n > 1 && *n < 100)
		rc = OK;
	else
		rc = ERR_READ;
	if (!rc)
	{
		*mtr = allocate_matrix(*n);
		if (mtr == NULL)
			rc = ERR_READ;
	}
	if (!rc)
		rc = read_file(f, *mtr, *n);
	if (rc)
		free_matrix(mtr);
	fclose(f);
	return rc;
}

int print_matrix(int **mtr, int n)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
			printf("%d ", mtr[i][j]);
		printf("\n");
	}
	return OK;
}