#include "defines.h"
#include "io.h"

typedef struct list_stack list_stack; 

struct list_stack
{
    int data;
    list_stack *next;
};

int empty_list(list_stack *s)
{
    if (!s)
        return ERR_LENGTH;
    return OK;
}


int full_list(list_stack *s)
{
    list_stack *p = s;
    int i = 0;
    for (; p->next; p = p->next)
        i++;
    if (i == MAXSIZE - 1)
        return ERR_LENGTH;
    return OK;
}


int push_list(list_stack **s, char x, int *n)
{
    list_stack *new = malloc(sizeof(list_stack));
    if (*n >= MAXSIZE)
        return ERR_LENGTH;
    *n += 1;
    if (!new)
        return ERR_READ;
    new->data = x;
    new->next = *s;
    *s = new;
    return OK;
}

int top_list(list_stack *s)
{
    list_stack *p = s;
    if (!p)
        return ERR_LENGTH;
    for (; p->next; p = p->next);
    return p->data;
}

int number_list(list_stack *s)
{
    list_stack *p = s;
    int i = 0;
    if (!p)
        return ERR_LENGTH;
    for (; p->next; p = p->next)
        i++;
    return i;
}
 
int pop_list(list_stack **s, int *n)
{   
    int x;
    if (*n < 1 || !s)
        return ERR_LENGTH;
    list_stack *new = (*s)->next;
    *n -= 1;
    x = (*s)->data;
    free(*s);
    *s = new;
    return x;
}


int precedence_list(char x)
{
    if(x == '(')
        return 0;
    if(x == '+' || x == '-')
        return 1;
    if(x == '*' || x== '/' || x=='%')
        return 2;
    return 3;
}

void infix_to_postfix_list(char infix[], char postfix[])
{
    list_stack *s = NULL;
    char x, token;
    int i, j, n = 0;
    j = 0;
    for (i = 0; infix[i] != '\0'; i++)
    {
        token = infix[i];
        if (isalnum(token))
            postfix[j++] = token;
        else
            if (token=='(')
                push_list(&s, '(', &n);
        else

            if (token==')')
                while ((x = pop_list(&s, &n)) != '(' && n > 0)
                      postfix[j++] = x;
            else
            {
                while (precedence_list(token) <= precedence_list(top_list(s)) && !empty_list(s) && n > 0)
                {
                    x = pop_list(&s, &n);
                    postfix[j++]=x;
                }
                push_list(&s, token, &n);
            }
    }
 
    while (n > 0 && !empty_list(s))
    {
        x = pop_list(&s, &n);
        if (x < 0)
            break;
        postfix[j++] = x;
    }
    postfix[j] = '\0';
}

void create_infix_form_list(list_stack *arr, char *infix)
{
    int i = 0;
    list_stack *p = arr;
    char temp[N];
    for (; p->next; p = p->next)
        temp[i++] = p->data;
    temp[i++] = p->data;
    temp[i] = '\0';
    for (int j = 0; j < i; j++) 
    {
        infix[i - j - 1] = temp[j];
    }
    infix[i] = '\0';
}

void print_stack(list_stack *list)
{
    list_stack *p = list;
    for (; p->next; p = p->next)
    {
        printf("%c\n", p->data);
        printf("%p\n", (void*)p);
    }
    printf("%c\n", p->data);
    printf("%p\n", (void*)p);
}

int add_delete_memory(list_stack *s, int n, void *(arr[N]), int *m)
{
    if (n < 1 || !s)
        return ERR_LENGTH;
    arr[*m] = s;
    *m += 1;
    return OK;
}

int delete_old_memory_from_array(list_stack *s, int n, void *(arr[N]), int *m)
{
    for (int i = 0; i < *m; i++)
    {
        if (arr[i] == s)
        {
            void *tmp = arr[*m];
            arr[*m] = arr[i];
            arr[i] = tmp;
            *m -= 1;
            return OK;
        }
    }
    return OK;
}

void start_list_menu()
{
    int rc = OK, action = 0, n = 0, m = 0;
    list_stack *list = NULL;
    void *arr[N];
    char infix[MAXSIZE], postfix[MAXSIZE], s[MAXSIZE];
    char c;
    do
    {
        printf("1) Добавить элемент в стек\n");
        printf("2) Удалить элемент\n");
        printf("3) Перевести в постфиксное\n");
        printf("4) Вывести текущий стек, вметсе с адрессами элементов\n");
        printf("5) Выйти из подпрогаммы\n");
        if ((rc = input_key(&action)) == OK)
        {
            switch (action)
            {
            case 1:
                printf("Введите цифру или арифмитическую операцию (+-*.)\n");
                if (scanf("%s", s) == 1 && strlen(s) == 1)
                {
                    c = s[0];
                    rc = push_list(&list, c, &n);
                    delete_old_memory_from_array(list, n, arr, &m);
                }
                else
                    rc = ERR_READ;
                if (rc)
                    printf("В стек ничего не добавлено\n");
                if (!rc)
                    printf("Успшено добавлен элемент в стек!\n");
                break;
            case 2:
                add_delete_memory(list, n, arr, &m);
                rc = pop_list(&list, &n);
                if (rc > 0)
                    printf("Элемент успешно удален\n");
                else
                    printf("Стек пуст\n");
                break;
            case 3:
                printf("Введите выражение до 100 элементов: ");
                scanf("%s", infix);
                if (strlen(infix) < MAXSIZE)
                {
                    infix_to_postfix_list(infix, postfix);
                    printf("%s\n", postfix);
                }
                else
                    printf("Превышен лимит ввода");
                break;
            case 4:
                if (n >= 1)
                {
                    print_stack(list);
                    printf("Адреса удаленых элементов\n");
                    for (int i = 0; i < m; i++)
                        printf("%p\n", arr[i]);
                }
                else
                    printf("Стек пуст\n");
            case 5:
                printf("Выход\n");
                break;
            default :
                printf("incorrect input\n");
                break;
            }
        }
        else
        {
            printf("Неверный ввод\n");
        }   
    }while (action != 5);
    
}