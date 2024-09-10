struct Base {
  int base_x;
  Base() {}
};

struct Derived1 : public Base {
  /* data */
  int derived1_x;
  Derived1() {}
};

struct Derived2 : public Derived1 {
  /* data */
  int derived2_x;
  Derived2() {}
};

void test_baseclass_field() {
  Base base;
  Derived1 derived1;
  Derived2 derived2;
}

int main() {
    test_baseclass_field();
}
