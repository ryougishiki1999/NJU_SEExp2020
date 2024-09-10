struct UserProvide {
  /* data */
  int *iptr;
};

class UserDef {
  int udx;

public:
  UserDef(int udx) : udx(udx) {}
};

class UserDef2 : public UserDef {

public:
  UserDef2(int udx) : UserDef(udx) {}
};

struct NeedCastLocFieldTest {

  void *pvoid;
  void *pvoid2;
  int loc_as_integer;
  UserDef *udf_ptr;

  NeedCastLocFieldTest(void *pvoid, int loc_as_integer, void *pvoid2,
                       UserDef *udf)
      : pvoid(pvoid), loc_as_integer(loc_as_integer), pvoid2(pvoid2),
        udf_ptr(udf) {}
};

void test_need_cast_loc_field() {
  unsigned val;
  int integer;
  //int loc_as_integer = reinterpret_cast<int>(&integer);
  //int loc_as_integer = (int)&integer;
  unsigned long loc_as_integer = (unsigned long )&integer;
  UserProvide up;
  
  int udx;
  UserDef2 udf2(udx);
  NeedCastLocFieldTest nclf(&val, loc_as_integer, &up, &udf2);
}

int main() { test_need_cast_loc_field(); }
