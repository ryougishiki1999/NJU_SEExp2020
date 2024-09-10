#include <cstdio>
class s1{
public:
	int i,j;
};
extern s1 *a;
class s2{
public:
	int* i,j;
};
void case_param(int* p)
{
	int temp=*p;
}
void case_unary_operator()
{
	int a = 2;
	int *b = NULL;
	int c = *b;
}
void case_array()
{
	int* a = new int[10];
	a=NULL;
	int c=a[1];
}
void case_member1()
{
	s1 *a=NULL;
	a->i = 0;
}
void case_member2()
{
	s2 a;
	a.i=NULL;
	int c=*a.i;
}
void case_reference1()
{
	int *a = NULL;
	int &b = *a;
}
void case_reference2()
{
	s1& b=*a;
	if(a!=NULL)
		return;
	b.i=0;
}
int main()
{
	case_param(NULL);
	case_unary_operator();
	case_array();
	case_member1();
	case_member2();
	case_reference1();
	case_reference2();
	return 0;
}