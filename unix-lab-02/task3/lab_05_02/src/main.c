#include "defines.h"
#include "array.h"
#include "io.h"
#include "list.h"
#include "time.h"


int main()
{
    int rc = OK, action;
    interval t1, t2, t3, t4;
    t1.min = 1;
    t1.max = 5;
    t2.min = 0;
    t2.max = 3;
    t3.min = 0;
    t3.max = 4;
    t4.min = 0;
    t4.max = 1;
    setbuf(stdout, NULL); 
    //get_all_time();
    printf("ЛР №5. Вариант номер 2\n");
    printf("Система состоит из обслуживающего аппарата и двух очередей заявок двух типов\n");
    do
    {
        printf("   1) Задать параметры модели\n   2) Запустить модель\n   3) Выход\n");
        printf("Выбирете нужный вам пункт, введите соотвествующую цифру (от 1 до 2): ");
        if (!(rc = input_key(&action)))
        {
            switch (action)
            {
            case 1:
                start_edit_menu(&t1, &t2, &t3, &t4);
                break;
            case 2:
                printf("   1) Модель в виде массива\n   2) Модель в виде списка\n");
                printf("Выбирете нужный вам пункт, введите соотвествующую цифру (от 1 до 2): ");
                if (!(rc = input_key(&action)))
                {
                    switch (action)
                    {
                    case 1:
                        start_arr_time(t1, t2, t3, t4);
                        break;
                    case 2:
                        start_list_time(t1, t2, t3, t4);
                    default:
                        break;
                    }
                }
                break;
            case 3:
                printf("Выход");
                break;
            default :
                printf("Неверный ввод\n");
                break;
            }
        }
        else
            printf("Неверный ввод\n");
    }
    while (action != 3 && rc == OK);
    return rc;
}