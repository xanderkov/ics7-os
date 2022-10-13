#include "defines.h"

void input_interval(interval *t)
{
    printf("Введите наименьшее время заявки (>= 0 ): ");
    while (scanf("%lf", &t->min) != 1 || t->min < 0)
    {
        printf("Неверный ввод\n");
        printf("Введите наименьшее время заявки (> 0 ): ");
    }
    printf("Введите наибольшое время заявки (> %lf ): ", t->min);
    while (scanf("%lf", &t->max) != 1 || t->max < t->min || fabs(t->max - t->min) < EPS)
    {
        printf("Неверный ввод\n");
        printf("Введите наибольшое время заявки (> %lf ): ", t->min);
    }
}


void start_edit_menu(interval *t1, interval *t2, interval *t3, interval *t4)
{
    printf("Интервал первой очереди\n");
    input_interval(t1);
    printf("Интервал второй очереди\n");
    input_interval(t2);
    printf("Интервал аппарата для первой очереди\n");
    input_interval(t3);
    printf("Интервал аппарата для второй очереди\n");
    input_interval(t4);
}

int input_key(int *key)
{
    if (scanf("%d", key) == 1)
        return OK;
    return ERR_READ;
}


void print_fault_1(interval t1, double time, int count)
{
    double one_time = time / ((t1.max + t1.min) / 2);

    printf("Погрешность: %lf\n", 100 * fabs(count - one_time) / one_time);
}


void print_fault_2(double time, double work, double t_wait)
{
    double check_time = work + t_wait;
    if (check_time > time)
        printf("Погрешность 2: %lf\n", 100 * fabs(time - check_time) / check_time);
    else
        printf("Погрешность 2: %lf\n", 100 * fabs(time - check_time) / time);
}

void print_fault_3(double time, interval t1, interval t2, interval t3, interval t4)
{
    double average = ((t1.max + t1.min) / 2.0 + (t3.max + t3.min) / 2.0 + (t2.max + t2.min) / 2.0 + (t4.max + t4.min) / 2.0) / 2.0 * N;
    printf("Погрешность 3: %lf\n", 100 * fabs(time - average) / average);
}