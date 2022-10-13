#include "defines.h"
#include "array_operations.h"
#include "io.h"
#include "list_operations.h"

void compare_time()
{
    
    int64_t time;
    struct timeval tv_start, tv_stop;
    char str[N], post[N];
    for (int i = 0; i < 50; i++)
    {
        str[i * 2] = '1';
        str[i * 2 + 1] = '+';
    }
    time = 0;
    for (int i = 0; i < N_TIME; i++)
    {
        gettimeofday(&tv_start, NULL);
        infix_to_postfix(str, post);
        gettimeofday(&tv_stop, NULL);
        time += (tv_stop.tv_sec - tv_start.tv_sec) * 1000000LL + (tv_stop.tv_usec - tv_start.tv_usec);
    }
    printf("100 элементов)Время использование массивом = %lf mcs\n", (float)time / N_TIME);
    time = 0;
    for (int i = 0; i < N_TIME; i++)
    {
        gettimeofday(&tv_start, NULL);
        infix_to_postfix_list(str, post);
        gettimeofday(&tv_stop, NULL);
        time += (tv_stop.tv_sec - tv_start.tv_sec) * 1000000LL + (tv_stop.tv_usec - tv_start.tv_usec);
    }
    printf("100 элементов)Время использование спском = %lf mcs\n", (float)time / N_TIME);
}