#include "io.h"
#include "sort_matrix.h"
#include "defines.h"



int main(int argc, char **argv)
{
	int n, rc = OK;
	int **mtr = NULL;
	setbuf(stdin, NULL);
	if (argc != 2) {
		printf("ERROR READ");
		return ERR_READ;
		}
	rc = read_matrix(argv[1], &mtr, &n);
	if (!rc)
	{
		sort_matrix(mtr, n);
		print_matrix(mtr, n);
		free_matrix(&mtr);
	}
	return rc;
}
