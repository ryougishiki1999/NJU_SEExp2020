struct RegularFiledTest {

  struct StructTest {
    int st_x;
    double st_y;
  };

  union UnionTest {
    char ut_z;
  };

  int primitve_test;
  double array_test[10]; // CSA regards array type as initialization no matter
                         // what the situation is...

  StructTest struct_test;
  UnionTest union_test; // CSA regards union type as initialization no matter
                        // what the situation is...

  RegularFiledTest() {}
};

void regular_field_test() { RegularFiledTest rft; }

int main() { regular_field_test(); }