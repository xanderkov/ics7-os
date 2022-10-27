#include "defines.h"
#include "io.h"


typedef struct stack
{
    int data[N];
    int top;
}stack;

int empty(stack *s)
{
    if (s->top==-1)
        return ERR_LENGTH;
    return OK;
}
 
int full(stack *s)
{
    if(s->top == MAXSIZE)
        return ERR_LENGTH;
    return OK;
}
 
int push(stack *s, int x)
{
    if (s->top >= MAXSIZE - 1)
        return ERR_LENGTH;
    s->top = s->top + 1;
    s->data[s->top] = x;
    return OK;
}
 
int pop(stack *s)
{
    int x;
    if (s->top == -1)
        return ERR_LENGTH;
    x = s->data[s->top];
    s->top=s->top-1;
    return x;
}
 
int top(stack *p)
{
    return (p->data[p->top]);
}

int precedence(char x)
{
    if(x == '(')
        return 0;
    if(x == '+' || x == '-')
        return 1;
    if(x == '*' || x== '/' || x=='%')
        return 2;
 
    return 3;
}
 
void init(stack *s)
{
    s->top=-1;
}

void infix_to_postfix(char infix[], char postfix[])
{
    stack s;
    char x, token;
    int i,j;
    init(&s);
    j = 0;
    for(i = 0; infix[i] != '\0'; i++)
    {
        token = infix[i];
        if (isalnum(token))
            postfix[j++] = token;
        else
            if (token == '(')
               push(&s, '(');
        else
            if (token == ')')
                while((x = pop(&s)) != '(' && x > 0)
                      postfix[j++] = x;
            else
            {
                while (precedence(token) <= precedence(top(&s)) && !empty(&s))
                {
                    x=pop(&s);
                    postfix[j++]=x;
                }
                push(&s, token);
            }
    }
 
    while(!empty(&s))
    {
        x = pop(&s);
        if (x < 0)
            break;
        postfix[j++] = x;
    }
 
    postfix[j]='\0';
}


void create_infix_form(stack arr, char *infix)
{
    int i = 0;
    for (i = 0; i <= arr.top; i++)
        infix[i] = arr.data[i];
    infix[i] = '\0';
}


void start_array_menu()
{
    int rc = OK, action = 0;
    stack arr;
    init(&arr);
    char infix[N], postfix[N], s[N];
    char c;
    do
    {
        menu();
        if ((rc = input_key(&action)) == OK)
        {
            switch (action)
            {
            case 1:
                printf("Введите цифру или арифмитическую операцию (+-*.)\n");
                if (scanf("%s", s) == 1 && strlen(s) == 1)
                {
                    c = s[0];
                    rc = push(&arr, c);
                }
                else
                    rc = ERR_READ;
                if (rc)
                    printf("В стек ничего не добавлено\n");
                if (!rc)
                    printf("Успшено добавлен элемент в стек!\n");
                rc = OK;
                break;
            case 2:
                rc = pop(&arr);
                if (rc > 0)
                    printf("Элемент успешно удален\n");
                else
                    printf("Стек пуст\n");
                rc = OK;    
                break;
            case 3:
                printf("Введите выражение до 100 элементов: ");
                scanf("%s", infix);
                if (strlen(infix) < MAXSIZE)
                {
                    infix_to_postfix(infix, postfix);
                    printf("%s\n", postfix);
                }
                else
                    printf("Превышен лимит ввода");
                break;
            case 4:
                for (int i = 0; i <= arr.top; i++)
                    printf("%c\n", arr.data[i]);
                break;
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
    }while (action != 5 && rc == OK);
    
}