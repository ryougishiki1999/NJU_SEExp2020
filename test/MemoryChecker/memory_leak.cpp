#include <cstdlib>
void case_trival(void)
{
    int *p = new int;
    int a = 1 + 2;
    *p = a + 3; // memory leak 'p'
}

void case_trival_malloc(void)
{
    int *p = (int *)malloc(sizeof(int));
    *p = 1; // memory leak 'p'
}

void case_arr(void)
{
    int *p = (int *)malloc(10 * sizeof(int));
    p[4] = 1; // memory leak 'p'
}

void case_arr_cpp(void)
{
    int *p = new int[10];
    p[4] = 1; // memory leak 'p'
}

void case_alias(void)
{
    int *p = (int *)malloc(sizeof(int));
    int *q = p; // memory leak 'p','q'
}

void case_alias_noleak(void)
{
    int *p = (int *)malloc(sizeof(int));
    int *q = p;
    free(q);
}

void case_cond_noleak(void)
{
    int *p = (int *)malloc(sizeof(int));
    if (p == NULL)
    {
        return;
    }
    else
    {
        free(p);
        return;
    }
}

void case_cond_noleak2(void)
{
    int *p = new int;
    if (p == nullptr)
    {
        return;
    }
    else
    {
        delete p;
        return;
    }
}

void case_cond_noleak3(void)
{
    int *p = new int;
    if (!p)
    {
        return;
    }
    else
    {
        delete p;
        return;
    }
}

int *case_return_noleak_helper(void)
{
    int *p = new int;
    return p;
}
void case_return_noleak(void)
{
    int *p = case_return_noleak_helper();
    delete p;
}