struct seven_words
{
    /* data */
    int c[7];
    int b;
};

static seven_words subtype[] = {{0, 1, 2, 3, 4, 5, 6}, {1, 2, 3, 4, 5, 6, 7}};

void test()
{
    subtype[2].b = 3;
}
