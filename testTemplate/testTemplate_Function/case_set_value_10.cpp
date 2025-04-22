#include "Template.hpp"
#include "TemplatePrintParser.hpp"
#include "TemplatePrintItem.hpp"
#include "FileInputStream.hpp"
#include "TestLog.hpp"
#include "Reflect.hpp"

using namespace obotcha;
using namespace gagira;

void case_set_value_10() {    
    auto input = FileInputStream::New("./test_case/case_set_value_10.txt");
    input->open();
    String content = input->readAll()->toString();
    Template t = Template::New(content);
    auto scopedValues = HashMap<String,TemplateScopedValue>::New();
    auto result = t->execute(scopedValues,nullptr);
    auto result_r1 = scopedValues->get(String::New("r1"));
    printf("map addr is %lx \n",scopedValues.get_pointer());
    if(result_r1->getBoolValue() != true) {
        TEST_FAIL("Template Function case_set_value_10 case1");
    }
    
    auto result_r2 = scopedValues->get(String::New("r2"));
    if(result_r2->getBoolValue() != false) {
        TEST_FAIL("Template Function case_set_value_10 case2");
    }
    
    auto result_r3 = scopedValues->get(String::New("r3"));
    if(result_r3->getBoolValue() != true) {
        TEST_FAIL("Template Function case_set_value_10 case3");
    }
    
    auto result_r4 = scopedValues->get(String::New("r4"));
    if(result_r4->getBoolValue() != false) {
        TEST_FAIL("Template Function case_set_value_10 case4");
    }
    
    auto result_r5 = scopedValues->get(String::New("r5"));
    if(result_r5->getBoolValue() != true) {
        TEST_FAIL("Template Function case_set_value_10 case5");
    }
    
    auto result_r6 = scopedValues->get(String::New("r6"));
    if(result_r6->getBoolValue() != false) {
        TEST_FAIL("Template Function case_set_value_10 case6");
    }
    
    auto result_r7 = scopedValues->get(String::New("r7"));
    if(result_r7->getBoolValue() != true) {
        TEST_FAIL("Template Function case_set_value_10 case7");
    }
    
    auto result_r8 = scopedValues->get(String::New("r8"));
    if(result_r8->getBoolValue() != false) {
        TEST_FAIL("Template Function case_set_value_10 case8");
    }
    
    TEST_OK("Template Function case_set_value_10");
}
