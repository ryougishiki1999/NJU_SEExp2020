#include <new>
#include <string>
struct Str
{
    Str(int l) : len(l)
    {
    }
    int len;
    std::string s;
};
void case_trival(void)
{
    int *p = new int;
    delete p;
    delete p; // double free
}
void case_array(void)
{
    int *arr = new int[10];
    delete[] arr;
    delete[] arr; // double free
}
void case_class(void)
{
    Str *s = new Str(5);
    delete s;
    delete s; // double free
}
void case_new_func(void)
{
    void *p = operator new(4);
    operator delete(p);
    operator delete(p); // double free
}
void case_new_func_array(void)
{
    void *p = operator new[](16);
    operator delete[](p);
    operator delete[](p); // double free
}