#include "defines.h"


void free_marks(marks **cur)
{
	marks *next;
	for (; *cur; *cur = next)
    {
        next = (*cur)->next;
        free(*cur);
    }
}

void free_list(student **stud)
{
    student *next;
    for (; *stud; *stud = next)
    {
        next = (*stud)->next;
		marks *cur_mark = (*stud)->mark;
		free_marks(&cur_mark);
        free(*stud);
    }
}

void copy_str(char *str1, char *str2)
{
	int i = 0;
	for (i = 0; str2[i] != '\0'; i++)
		str1[i] = str2[i];
	str1[i] = '\0';
}


student *create(char *name, int age)
{
    student *node = malloc(sizeof(student));
    if (node)
    {
		copy_str(node->surname, name);
		node->age = age;
		node->mark = NULL;
        node->next = NULL;
    }
    return node;
}

student *append(student *head, student *node)
{
    if (!head)
        return node;
    student *cur = head;
    for (; cur->next; cur = cur->next);
    cur->next = node;
    return head;
}

marks *create_mark(int x)
{
    marks *node = malloc(sizeof(marks));
    if (node)
    {
		
		node->mark = x;
        node->next = NULL;
    }
    return node;
}

marks *append_mark(marks *head, marks *node)
{
    if (!head)
        return node;
    marks *cur = head;
    for (; cur->next; cur = cur->next);
    cur->next = node;
    return head;
}


int input_student(student **st, char *filename)
{
    char sur[N] = { 0 };
	int age = 0, mark = 0, rc = OK, n = 0;
	FILE *f = NULL;
	f = fopen(filename, "r");
    if (!f)
        rc = FILE_NOT_FOUNT;
    if (!rc)
    {
        while (fscanf(f, "%s%d", sur, &age) == 2) 
		{
			student *node = NULL;
			node = create(sur, age);
			*st = append(*st, node);
            while (fscanf(f, "%d", &mark) == 1) 
            {
                marks *cur = NULL;
				student *next = *st;
				if (n > 0)
					for (; next->next; next = next->next);
				cur = create_mark(mark);
				next->mark = append_mark(next->mark, cur);
				
            }
			n++;
        }
        if (rc)
            free_list(st);
		fclose(f);
    }
	
    return rc;
}

void print_marks(marks *m)
{
	if (!m)
		return;
    marks *p = m;
	
    for (; p->next; p = p->next)
		printf("%d ", p->mark);
    printf("%d", p->mark);
	printf("\n");
}


void print_student(student *st)
{
    student *p = st;
    for (; p->next; p = p->next)
	{
        printf("%s ", p->surname);
		printf("%d ", p->age);
		print_marks(p->mark);
	}
    printf("%s ", p->surname);
	printf("%d ", p->age);
	print_marks(p->mark);
    printf("\n");
}

void print_error(int rc)
{
	switch (rc) 
	{
		case FILE_NOT_FOUNT:
			printf("File not found\n");
			break;
		case FILE_NOT_VALID:
			printf("File not valid\n");
			break;
		case ERR_KEY:
			printf("Key not valid");
			break;
	}
}



