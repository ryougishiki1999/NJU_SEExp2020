struct CyclicLocFieldTest {
  /* data */
  int *cyclic;

  CyclicLocFieldTest() : cyclic((int *)&cyclic) {}
};

void test_cyclic_reference() { CyclicLocFieldTest clft; }

int main() { test_cyclic_reference(); }
