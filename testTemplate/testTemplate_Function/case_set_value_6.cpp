#include "Template.hpp"
#include "TemplatePrintParser.hpp"
#include "TemplatePrintItem.hpp"
#include "FileInputStream.hpp"
#include "TestLog.hpp"
#include "Reflect.hpp"

using namespace obotcha;
using namespace gagira;

DECLARE_CLASS(SetValue6_Data) {
public:
    int data;
    DECLARE_REFLECT_FIELD(SetValue6_Data,data)
};

void case_set_value_6() {    
    auto input = FileInputStream::New("./test_case/case_set_value_6.txt");
    input->open();
    String content = input->readAll()->toString();
    Template t = Template::New(content);
    auto scopedValues = HashMap<String,TemplateScopedValue>::New();
    auto obj = SetValue6_Data::New();
    obj->data = 1;
    auto result = t->execute(scopedValues,obj);
    auto result_a = scopedValues->get(String::New("a"));
    if(result_a->getIntValue() != 4) {
        TEST_FAIL("Template Function case_set_value_6, case2 v is %d",result_a->getIntValue());
    }
    
    auto result_b = scopedValues->get(String::New("b"));
    if(result_b->getIntValue() != 1) {
        TEST_FAIL("Template Function case_set_value_6, case3 v is %d",result_b->getIntValue());
    }
    
    auto result_c = scopedValues->get(String::New("c"));
    if(result_c->getIntValue() != 2) {
        TEST_FAIL("Template Function case_set_value_6, case4 v is %d",result_c->getIntValue());
    }
    
    auto result_d = scopedValues->get(String::New("d"));
    if(result_d->getIntValue() != 1) {
        TEST_FAIL("Template Function case_set_value_6, case5 v is %d",result_d->getIntValue());
    }
    
    auto result_e = scopedValues->get(String::New("e"));
    if(result_e->getIntValue() != -3) {
        TEST_FAIL("Template Function case_set_value_6, case6 v is %d",result_e->getIntValue());
    }
    
    auto result_f = scopedValues->get(String::New("f"));
    if(result_f->getIntValue() != (6>>2)) {
        TEST_FAIL("Template Function case_set_value_6, case7 v is %d",result_f->getIntValue());
    }
    
    auto result_g = scopedValues->get(String::New("g"));
    if(result_g->getIntValue() != (3<<2)) {
        TEST_FAIL("Template Function case_set_value_6, case7 v is %d",result_f->getIntValue());
    }
    
    TEST_OK("Template Function case_set_value_6");
}
