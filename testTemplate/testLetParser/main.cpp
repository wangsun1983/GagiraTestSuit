#include <stdio.h>
#include <unistd.h>

#include "Template.hpp"

using namespace obotcha;
using namespace gagira;

extern void basic_test();
extern void test_class_member();
extern void test_scoped_value();

int main() {
    basic_test();
    //test_class_member();
    //test_scoped_value();
    return 0;
}
