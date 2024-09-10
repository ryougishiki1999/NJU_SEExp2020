struct LocFieldTest {
  struct SepratorRightArrow {
    int sra_test_x;
  };

  struct SepratorDot {
    /* data */
    int sd_test_x;
  };

  union UnionTest {
    double ut_x;
  };

  int *iptr;
  char *ch_pointee;
  double *array_ptr;

  SepratorDot &sd;
  SepratorRightArrow *sra_ptr;

  UnionTest *ut;

  LocFieldTest(char *ch, SepratorRightArrow *sra, SepratorDot &sd,
               UnionTest *ut, double array[])
      : ch_pointee(ch), sra_ptr(sra), sd(sd), ut(ut), array_ptr(array) {}
};

void test_loc_field() {
  LocFieldTest::SepratorRightArrow sra;
  LocFieldTest::SepratorDot sd;
  char ch;
  LocFieldTest::UnionTest ut;
  double array[10];

  LocFieldTest lf(&ch, &sra, sd, &ut, array);
}

int main() { test_loc_field(); }