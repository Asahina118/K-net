#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "tests/extern/doctest.h"

int factorial(int number)
{
    return number <= 1 ? number : factorial(number - 1) * number;
}

TEST_CASE("testing the factorial function") {
    CHECK(factorial(1) == 1);
    CHECK(factorial(5) == 120);
}


