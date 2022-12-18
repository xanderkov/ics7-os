#include "defines.h"

list *add_end(list *head, list *language);
list *create_list(list *head);
list *pop(list *head);
int list_size(list *head);
void free_all(list *head);
int start_list_time(interval t1, interval t2, interval t3, interval t4);
double get_min(double t1, double t2, double t3);
double get_min_of_two(double a, double b);
double get_random_time(const double min, const double max);
