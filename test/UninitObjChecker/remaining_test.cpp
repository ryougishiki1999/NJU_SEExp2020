struct CorrectInit {
  /* data */
  struct StructTest {
    /* data */
    int st_x;
    StructTest(int st_x) : st_x(st_x) {}
  };

  int x;
  double y;
  int *iptr;
  StructTest st;
  StructTest *st_ptr;

  CorrectInit(int x, int y, int *iptr, StructTest st, StructTest *st_ptr)
      : x(x), y(y), iptr(iptr), st(st), st_ptr(st_ptr) {}
};

struct RemainingPath {
  /* data */
  int *iptr_unknown = new int;
  double *array_ptr;
  RemainingPath(double array[]) : array_ptr(array) {}
  RemainingPath() {}
};

struct RemainPath2 {
  /* data */
  void *pvoid = this;
  RemainPath2() {}
};

struct Base {
  int x1;
  /* data */
  Base() {}
};

struct Base2 : public Base {
  /* data */
  int x2;
  Base2() {}
};

void test_correct_init() {
  int x = 1;
  double y = 2.0;
  int val = 2020;
  CorrectInit::StructTest st(x);
  CorrectInit ci(x, y, &val, st, &st);
}

void test_remaining_path() {
  double array[10];
  RemainPath2 *rp2;
  RemainingPath rp(array);
}

int main() {
  test_correct_init();
  test_remaining_path();
}
