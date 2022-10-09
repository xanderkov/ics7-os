#include "defines.h"
#include "io.h"


int main(int argc, char **argv)
{
	setbuf(stdout, NULL);
	int rc = OK;
	student *st = NULL;
	if (argc == 3 && !strcmp(argv[1], "-f"))
	{
		rc = input_student(&st, argv[2]);
		if (!rc)
			print_student(st);
	}
	/*
	else if (argc == 5 && !strcmp(argv[1], "-f") && !strcmp(argv[3], "-n"))
	{
		rc = input_student();
		if (!rc)
		{
			rc = separete_students();
			if (!rc)
				print_student();
		}
	*/
	else
		rc = ERR_KEY;
	if (rc)
		print_error(rc);
	return rc;
}