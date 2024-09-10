#pragma once
#include<iostream>
#include<string>

class Test {
private:
	std::string t;
public:
	Test();
	Test(std::string t);
	Test operator /(const Test& T2);
	Test operator %(const Test& T2);
};

Test::Test() {
	this->t = "test";
}

Test::Test(std::string t) {
	this->t = t;
}

Test Test::operator /(const Test& T2) {
	this->t = T2.t.substr(0, 1);
	return *this;
}

Test Test::operator%(const Test& T2) {
	this->t = T2.t;
	return *this;
}


