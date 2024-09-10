#include <cstdlib>
void case_trival(void)
{
    int *p = (int *)malloc(sizeof(int));
    free(p);
    free(p); // double free
}