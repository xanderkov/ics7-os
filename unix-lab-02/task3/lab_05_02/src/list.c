#include "defines.h"
#include "io.h"

list *add_end(list *head, list *language)
{
    list *cur = head;

    if (!head)
        return language;

    for (; cur->next; cur = cur->next);

    cur->next = language;

    return head;
}


list *create_list(list *head)
{
    list *tmp = malloc(sizeof(list));
    if (tmp)
    {
        tmp->next = NULL;
        head = tmp;
    }
    return head;
}


list *pop(list *head)
{
    if (!head)
        return NULL;
    list *tmp;
    tmp = head;
    head = head->next;
    free(tmp);
    tmp = NULL;
    return head;
}


int list_size(list *head)
{
    int count = 0;
    while (head)
    {
        head = head->next;
        count++;
    }
    return count;
}


void free_all(list *head)
{
    list *next;

    for (; head; head = next)
    {
        next = head->next;
        free(head);
    }
}


double get_random_time(const double min, const double max)
{
    return (max - min) * (double)(rand() / (double)RAND_MAX) + min + EPS;
}


void get_adr(int flag, void **adr, int *adr_num, list *head)
{
    if (flag == 1)
    {
        if (*adr_num == N - 1)
            *adr_num = 0;
        adr[*adr_num] = head;
        *adr_num += 1;
    }
}

void get_time_work_exit_element(double *t_work, double t_out, int flag, void **adr, int *adr_num, list **head, int *n_quit)
{
    *n_quit += 1;
    *t_work += t_out;
    get_adr(flag, adr, adr_num, *head);
    *head = pop(*head);
}


void add_element_to_queue(list **head, int *count)
{
    *count += 1;
    list *tail = NULL;
    if (!*head)
        *head = create_list(*head);
    else
    {
        tail = create_list(tail);
        *head = add_end(*head, tail);
    }
}

double get_min_of_two(double a, double b)
{
    if (a < b)
        return a;
    else
        return b;
}


double get_min(double t1, double t2, double t3)
{
    if (t1 > 0 && t2 > 0 && t3 > 0)
    {
        if (t1 < t2 && t1 < t3)
            return t1;
        if (t2 < t1 && t2 < t3)
            return t2;
        else
            return t3;
    }
    if (t1 > 0 && t2 > 0)
        return get_min_of_two(t1, t2);
    if (t1 > 0 && t3 > 0)
        return get_min_of_two(t1, t3);
    if (t2 > 0 && t3 > 0)
        return get_min_of_two(t2, t3);
    if (t1 > 0)
        return t1;
    if (t2 > 0)
        return t2;
    if (t3 > 0)
        return t3;
    return OK;
}


void print_adresses(void **adresses, int count)
{
    printf("memore adresses:\n");
    printf("-----------------------\n");
    for (int i = 0; i < count; i++)
    {
        printf("| adress %p |\n",  adresses[i]);
    }
    printf("------------------------");
    printf("\n");
}

int start_list_time(interval t1, interval t2, interval t3, interval t4)
{
    void *adr[MAXSIZE];
    int adr_num = 0;
    double t_in_1 = 0, t_out = 0, t_wait = 0, time = 0, t_in_2 = 0, t_min = 0, t_work = 0;
    list *head = NULL, *head_2 = NULL;
    int count = 0, n_quit = 0, flag = 0, n_quit_2 = 0, count_2 = 0;
    double av_len = 0;
    printf("Выводить список свободных областей (введите 1, чтобы был вывод): ");
    scanf("%d", &flag);
    int i = 0;
    int64_t t = 0;
    struct timeval tv_start, tv_stop;
    gettimeofday(&tv_start, NULL);
    while (n_quit < 1000)
    {
        if (t_in_1 <= EPS)
        {
            t_in_1 = get_random_time(t1.min, t1.max);
            add_element_to_queue(&head, &count);
        } 
        if (t_in_2 <= EPS)
        {
            t_in_2 = get_random_time(t2.min, t2.max);
            add_element_to_queue(&head_2, &count_2);
        }
        if (t_out <= EPS)
        {
            if (!head)
                if (!head_2)
                    t_wait += t_min;
                else
                {
                    t_out = get_random_time(t4.min, t4.max);
                    get_time_work_exit_element(&t_work, t_out, flag, adr, &adr_num, &head_2, &n_quit_2);
                }
            else
            {
                t_out = get_random_time(t3.min, t3.max);
                get_time_work_exit_element(&t_work, t_out, flag, adr, &adr_num, &head, &n_quit);
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
            len = list_size(head);
            if (n_quit > 0)
                av_len += len / ((double)n_quit / 100);
            printf("Средняя длина первой очереди: %lf\n", av_len);
            printf("Количество элементов первой очереди: %d\n", len);
            len = list_size(head_2);
            if (n_quit_2 > 0)
                av_len += len / ((double)n_quit_2 / 100);
            printf("Средняя длина второй очереди: %lf\n", av_len);
            printf("Количество элементов второй очереди: %d\n", len);
            printf("Количество прошедших элементов через первую очередь: %d\n", count);
            printf("Количество заявок вышедших из первой очереди: %d\n", n_quit);
            if (n_quit > 0)
                printf("Среднее время пребывание в первой очереди: %lf\n", time / (double)n_quit);
            if (n_quit_2 > 0)
                printf("Среднее время пребывание во второй очереди: %lf\n", time / (double)n_quit_2);
            printf("Время ожидания: %lf\n", t_wait);
            printf("Время работы аппарата: %lf\n", t_work);
        }
    }
    gettimeofday(&tv_stop, NULL);
    printf("-----------------------------------------------------------------\n");
    t = (tv_stop.tv_sec - tv_start.tv_sec) * 1000000LL + (tv_stop.tv_usec - tv_start.tv_usec);
    printf("Время использование стеком = %lf mcs\n", (float)t);
    printf("Количество вошедших заявок во вторую очередь: %d\n", count_2);
    printf("Количество вышедших заявок из второй очереди: %d\n", n_quit_2);
    print_fault_1(t1, time, count);
    //print_fault_2(time, t_work, t_wait);
    //print_fault_3(time, t1, t2, t3, t4);

    if (flag == 1)
        print_adresses(adr, adr_num);
    free_all(head);
    free_all(head_2);
    return OK;
}