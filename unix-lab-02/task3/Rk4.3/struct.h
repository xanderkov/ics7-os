#ifndef _MY_STRUCT_

#define _MY_STRUCT_

typedef struct student student;
typedef struct marks marks;

struct marks
{
	int mark;
	marks *next;
};

struct student
{
    char surname[256];
	int age;
	marks *mark;
    student *next;
};

#endif
