#include "defines.h"
#include "list.h"
#include "io.h"

int arr_push(queue **pin, queue *pout, queue *begin)
{
    if (*pin == NULL || begin == NULL)
        return ERR_NULL;
    *pin += 1;
    if (*pin == pout)
        return ERR_LENGTH;
    if (*pin == begin + N)
        *pin = begin;
    return OK;
}

int arr_pop(queue **pout, queue *begin)
{
    if (*pout == NULL || begin == NULL)
        return ERR_LENGTH;
    *pout += 1;
    if (*pout == begin + N)
        *pout = begin;
    return OK;
}

void get_time_work_exit_element_arr(double *t_work, double t_out, queue *q, queue *pin, queue **pout, int *n_quit)
{
    *n_quit += 1;
    *t_work += t_out;
    arr_pop(pout, pin);
}


void add_element_to_queue_arr(queue *q, queue **pin, queue *pout, int *count)
{
    *count += 1;
    arr_push(pin, pout, q);
}

int start_arr_time(interval t1, interval t2, interval t3, interval t4)
{
    double t_in_1 = 0, t_out = 0, t_wait = 0, time = 0, t_in_2 = 0, t_min = 0, t_work = 0;
    queue q_1[MAXSIZE], q_2[MAXSIZE], *pin_1 = &q_1[0], *pout_1 = &q_1[0], *pin_2 = &q_2[0], *pout_2 = &q_2[0];
    int count = 0, n_quit = 0, n_quit_2 = 0, count_2 = 0;
    double av_len = 0;
    int i = 0;
    int64_t t = 0;
    struct timeval tv_start, tv_stop;
    gettimeofday(&tv_start, NULL);
    while (n_quit < 1000)
    {
        if (t_in_1 <= 0)
        {
            t_in_1 = get_random_time(t1.min, t1.max);
            add_element_to_queue_arr(q_1, &pin_1, pout_1, &count);
        } 
        if (t_in_2 <= 0)
        {
            t_in_2 = get_random_time(t2.min, t2.max);
            add_element_to_queue_arr(q_1, &pin_2, pout_2, &count_2);
        }
        if (t_out <= 0)
        {
            if (pin_1 == pout_1)
                if (pin_2 == pout_2)
                    t_wait += t_min;
                else
                {
                    t_out = get_random_time(t4.min, t4.max);
                    get_time_work_exit_element_arr(&t_work, t_out, q_1, pin_2, &pout_2, &n_quit_2);
                }
            else
            {
                t_out = get_random_time(t3.min, t3.max);
                get_time_work_exit_element_arr(&t_work, t_out, q_1, pin_1, &pout_1, &n_quit);
            } 
        }
        t_min = get_min(t_in_1, t_in_2, t_out);
        t_in_1 -= t_min;
        t_in_2 -= t_min;
        t_out -= t_min;
        time += t_min;
        if (n_quit % 100 == 0 && i != n_quit)
        {
            i = n_quit;
            int len;
            printf("-----------------------------------------------------------------\n");
            printf("Время : %lf\n", time);
            len = abs(pin_1 - pout_1);
            if (n_quit > 0)
            {
                av_len += len / (double)(n_quit / 100);
                printf("Средняя длина первой очереди: %lf\n", av_len);
            }
            printf("Количество элементов первой очереди: %d\n", len);
            len = abs(pin_2 - pout_2);
            if (n_quit_2 > 0)
            {
                av_len += len / (double)(n_quit_2 / 100);
                printf("Средняя длина второй очереди: %lf\n", av_len);
            }
            printf("Количество элементов второй очереди: %d\n", len);
            printf("Время ожидания: %lf\n", t_wait);
            printf("Количество прошедших элементов через первую очередь: %d\n", count);
            printf("Количество заявок вышедших из первой очереди: %d\n", n_quit);
            if (n_quit > 0)
                printf("Среднее время пребывание в первой очереди: %lf\n", time / n_quit);
            if (n_quit_2 > 0)
                printf("Среднее время пребывание во второй очереди: %lf\n", time / n_quit_2);
        }
    }
    gettimeofday(&tv_stop, NULL);
    printf("-----------------------------------------------------------------\n");
    t = (tv_stop.tv_sec - tv_start.tv_sec) * 1000000LL + (tv_stop.tv_usec - tv_start.tv_usec);
    printf("Время использование массивом = %lf mcs\n", (float)t);
    printf("Количество вошедших заявок во вторую очередь: %d\n", count_2);
    printf("Количество вышедших заявок из второй очереди: %d\n", n_quit_2);

    print_fault_1(t1, time, count);
    //print_fault_2(time, t_work, t_wait);
    //print_fault_3(time, t1, t2, t3, t4);
    return OK;
}