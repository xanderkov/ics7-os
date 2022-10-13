#include "defines.h"
#include "array_operations.h"
#include "io.h"
#include "list_operations.h"
#include "time.h"


int main()
{
    int rc = OK, action;
    setbuf(stdout, NULL); 
    printf("ЛР №4. Вариант номер 9\n");
    printf("Создать программу работы со стеком, выполняющую операции добавление\n");
    printf(", удаления элементов и вывод текущего состояния стека. Реализовать стек:\n");
    printf("а) массивом; б) списком. \n");
    printf("Все стандартные операции со стеком должны быть оформлены подпрограммами.\n");
    printf("При реализации стека списком в вывод текущего \n");
    printf("состояния стека добавить просмотр адресов элементов стека и\n");
    printf("создать свой список или массив свободных областей \n");
    printf("(адресов освобождаемых элементов) с выводом его на экран.\n");
    if (!rc)
    {
        do
        {
            printf(" 1) Реализация программы массивом\n 2) Реализация программы списком\n 3) Cравнить скорость работы массива и списка\n 4) Выйти \n");
            printf("Выбирете нужный вам пункт, напишите соотвествующую цифру (от 1 до 4)\n");
            printf("\n\n----------------------------------------------------------------\n");
            if ((rc = input_key(&action)) == OK)
            {
                switch (action)
                {
                case 1:
                    start_array_menu();
                    break;
                case 2:
                    start_list_menu();
                    break;
                case 3:
                    compare_time();
                    break;
                case 4:
                    printf("Выход");
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
        }
        while (action != 4);
    }
    return rc;
}