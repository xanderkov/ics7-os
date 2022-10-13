#include "defines.h"
#include "array.h"
#include "io.h"
#include "list.h"

void get_all_time()
{
    int64_t t = 0;
    struct timeval tv_start, tv_stop;
    list *tail = NULL, *head = NULL;
    gettimeofday(&tv_start, NULL);
    for (int i = 0; i < N_TIME; i++)
    {
        if (!head)
            head = create_list(head);
        else
        {
            tail = create_list(tail);
            head = add_end(head, tail);
        }
    }
    gettimeofday(&tv_stop, NULL);
    t = (tv_stop.tv_sec - tv_start.tv_sec) * 1000000LL + (tv_stop.tv_usec - tv_start.tv_usec);
    printf("Время доавления в список: %lf\n", t / (double)N_TIME);
    gettimeofday(&tv_start, NULL);
    for (int i = 0; i < N_TIME; i++)
    {
        head = pop(head);
    }
    gettimeofday(&tv_stop, NULL);
    t = (tv_stop.tv_sec - tv_start.tv_sec) * 1000000LL + (tv_stop.tv_usec - tv_start.tv_usec);
    printf("Время удаления из списка: %lf\n", t /(double) N_TIME);
    queue q[MAXSIZE], *pin = &q[0], *pout = &q[0];
    gettimeofday(&tv_start, NULL);
    for (int i = 0; i < N_TIME; i++)
    {
        arr_push(&pin, pout, q);
    }
    gettimeofday(&tv_stop, NULL);
    t = (tv_stop.tv_sec - tv_start.tv_sec) * 1000000LL + (tv_stop.tv_usec - tv_start.tv_usec);
    printf("Время доавления в массив: %lf\n", t /(double) N_TIME);
    gettimeofday(&tv_start, NULL);
    for (int i = 0; i < N_TIME; i++)
    {
        arr_pop(&pout, pin);
    }
    gettimeofday(&tv_stop, NULL);
    t = (tv_stop.tv_sec - tv_start.tv_sec) * 1000000LL + (tv_stop.tv_usec - tv_start.tv_usec);
    printf("Время удаления из массива: %lf\n", t / (double)N_TIME);

}