#include "defines.h"

int input_student(student **st, char *filename);
void free_list(student **stud);
void print_error(int rc);
void print_student(student *st);