#include <cstdio>
extern int* f();
void case_implicit()
{
	int a = *f();
}
void case_reference_implicit()
{
	int &a = *f();
}
int main()
{
	case_implicit();
	case_reference_implicit();
	return 0;
}