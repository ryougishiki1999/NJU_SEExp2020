#include <cstdlib>
struct S
{
    int f()
    {
        return varf;
    }
    int varf;
};
int func(int *p)
{
    return *p;
}
void case_trival(void)
{
    int *p = new int;
    delete p;
    func(p); // use-after-free
    *p = 4;
}
int f(int *q){
    return 1;
}
void case_arg(void)
{
    int *p = new int;
    delete p;
    f(p);
}
void case_class(void)
{
    S *s = new S;
    delete s;
    s->f(); // use-after-free
}
int *case_return(void)
{
    int *p = new int;
    delete p;
    return p; // use-after-free
}
void case_loop_for(void)
{
    bool ok;
    int *p = (int *)malloc(sizeof(int) * 10);
    for (int i = 0; i < 10; ++i)
    {
        p[i] = i;
    }
    free(p);
    p[1] = 2;
}
void case_loop_while(void)
{
    bool ok;
    int *p = (int *)malloc(sizeof(int) * 10);
    int i = 10;
    while (i--)
    {
        p[i] = i;
    }
    free(p);
    p[1] = 2;
}