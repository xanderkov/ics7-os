#include "defines.h"
#include "array_operations.h"


void menu()
{
    printf("1) Добавить элемент в стек\n");
    printf("2) Удалить элемент\n");
    printf("3) Перевести в постфиксное\n");
    printf("4) Вывод стека\n");
    printf("5) Выйти из подпрогаммы\n");
}

int input_key(int *action)
{
    if (scanf("%d", action) != 1)
    {
        printf("Неверный ввод!\n");
        return ERR_READ;
    }
    return OK;
}

bool is_digit(char ch)
{
    return (ch >= '0' && ch <= '9');
}

int is_number(char *s)
{
    for (int i = 0; i < strlen(s); i++)
        if (!is_digit(s[i]))
            return ERR_READ;
    return OK;
}