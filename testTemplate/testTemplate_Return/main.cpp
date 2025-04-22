#include <stdio.h>
#include <unistd.h>

#include "Template.hpp"

using namespace obotcha;
using namespace gagira;

extern void test_case1();
extern void test_case2();

int main() {
    //test_case1();
    test_case2();
    return 0;
}
