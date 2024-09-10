#include"test.h"

// not division and not mod
void test_situation_1() {
	int temp1 = 1;
	int tem2 = 1 + temp1;
}

// not an integer or a float number
void test_situation_2() {
	Test temp1;
	Test temp2("hello");
	Test temp3 = temp1 / temp2;
	Test temp4 = temp1 % temp2;
}

//divide undefined value
void test_situation_3() {
	int temp1;
	int temp2 = 10 / temp1;
}

// divide zero
void test_situation_4() {
	int temp1 = 0;
	int temp2 = 10 / temp1;
}

// mod zero 
void test_situation_5() {
	int temp1 = 0;
	int temp2 = 10 % temp1;
}

//correct
void test_situation_6() {
	int temp1=10;
	int temp2=2;
	int temp3=temp1/temp2;
}

//unary operator
void test_situation_7(){
	bool temp1=false;
	bool temp2=!temp1;
}

//complex example
void test_situation_8(){
	int temp1=1;
	int temp2=2;
	int temp3=temp1/temp1+temp2;
	int temp4=temp1/temp3;
}

int main() {
	test_situation_1();
	test_situation_2();
	test_situation_3();
	test_situation_4();
	test_situation_5();
	test_situation_6();
	test_situation_7();
	test_situation_8();
	return 0;
}