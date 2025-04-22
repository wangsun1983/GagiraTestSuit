#include "Template.hpp"
#include "TemplatePrintParser.hpp"
#include "TemplatePrintItem.hpp"
#include "FileInputStream.hpp"
#include "TestLog.hpp"
#include "Reflect.hpp"

using namespace obotcha;
using namespace gagira;

void case_set_value_9() {    
    auto input = FileInputStream::New("./test_case/case_set_value_9.txt");
    input->open();
    String content = input->readAll()->toString();
    Template t = Template::New(content);
    auto scopedValues = HashMap<String,TemplateScopedValue>::New();
    auto result = t->execute(scopedValues,nullptr);
    auto result_a = scopedValues->get(String::New("a"));
    if(result_a->getBoolValue() != true) {
        TEST_FAIL("Template Function case_set_value_9");
    }
    
    auto result_b = scopedValues->get(String::New("b"));
    if(result_b->getBoolValue() != false) {
        TEST_FAIL("Template Function case_set_value_9, case2");
    }
    
    auto result_c = scopedValues->get(String::New("c"));
    if(result_c->getBoolValue() != true) {
        TEST_FAIL("Template Function case_set_value_9, case3");
    }
    
    auto result_d = scopedValues->get(String::New("d"));
    if(result_d->getBoolValue() != false) {
        TEST_FAIL("Template Function case_set_value_9, case4");
    }
    
    auto result_e = scopedValues->get(String::New("e"));
    if(result_d->getBoolValue() != false) {
        TEST_FAIL("Template Function case_set_value_9, case5");
    }
    
    auto result_f = scopedValues->get(String::New("f"));
    if(result_f->getBoolValue() != true) {
        TEST_FAIL("Template Function case_set_value_9, case5");
    }
    
    auto result_g = scopedValues->get(String::New("g"));
    if(result_g->getBoolValue() != true) {
        TEST_FAIL("Template Function case_set_value_9, case6");
    }
    
    auto result_h = scopedValues->get(String::New("h"));
    if(result_h->getBoolValue() != true) {
        TEST_FAIL("Template Function case_set_value_9, case7");
    }
    
    auto result_i = scopedValues->get(String::New("i"));
    if(result_i->getBoolValue() != false) {
        TEST_FAIL("Template Function case_set_value_9, case8");
    }
    
    auto result_j = scopedValues->get(String::New("j"));
    if(result_j->getBoolValue() != false) {
        TEST_FAIL("Template Function case_set_value_9, case9");
    }
    
    TEST_OK("Template Function case_set_value_9");
}
