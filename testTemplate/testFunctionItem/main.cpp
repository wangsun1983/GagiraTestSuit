#include <stdio.h>
#include <unistd.h>

#include "Template.hpp"

using namespace obotcha;
using namespace gagira;

extern void test_basic_Int();
extern void test_class_member_Int();
extern void test_scoped_value_Int();
extern void test_mixture_value_Int();
extern void test_basic_String();
extern void test_class_member_String();
extern void test_scoped_value_String();
extern void test_mixture_value_String();
extern void test_basic_boolean();
extern void test_class_member_boolean();
extern void test_mixture_value_bool();

int main() {
    test_mixture_value_bool();
#if 0    
    test_basic_boolean();
    test_basic_Int();
    test_basic_String();
    test_class_member_Int();
    test_class_member_boolean();
    test_scoped_value_Int();
    test_mixture_value_Int();
    
    test_class_member_String();
    test_scoped_value_String();
    test_mixture_value_String();
#endif    
    return 0;
}
