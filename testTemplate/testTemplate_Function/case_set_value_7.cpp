#include "Template.hpp"
#include "TemplatePrintParser.hpp"
#include "TemplatePrintItem.hpp"
#include "FileInputStream.hpp"
#include "TestLog.hpp"
#include "Reflect.hpp"

using namespace obotcha;
using namespace gagira;

DECLARE_CLASS(SetValue7_Data) {
public:
    int data;
    DECLARE_REFLECT_FIELD(SetValue7_Data,data)
};

void case_set_value_7() {    
    auto input = FileInputStream::New("./test_case/case_set_value_7.txt");
    input->open();
    String content = input->readAll()->toString();
    Template t = Template::New(content);
    auto scopedValues = HashMap<String,TemplateScopedValue>::New();
    auto obj = SetValue7_Data::New();
    obj->data = 1;
    auto result = t->execute(scopedValues,obj);
    auto result_a = scopedValues->get(String::New("a"));
    if(result_a->getIntValue() != 99) {
        TEST_FAIL("Template Function case_set_value_7, case2 v is %d",result_a->getIntValue());
    }
    
    auto result_b = scopedValues->get(String::New("b"));
    if(result_b->getIntValue() != 201) {
        TEST_FAIL("Template Function case_set_value_7, case3 v is %d",result_b->getIntValue());
    }
    
    auto result_c = scopedValues->get(String::New("c"));
    if(result_c->getIntValue() != -100) {
        TEST_FAIL("Template Function case_set_value_7, case4 v is %d",result_c->getIntValue());
    }

    auto result_d = scopedValues->get(String::New("d"));
    if(result_d->getIntValue() != -100) {
        TEST_FAIL("Template Function case_set_value_7, case5 v is %d",result_d->getIntValue());
    }
    
    auto result_f = scopedValues->get(String::New("f"));
    if(result_f->getIntValue() != 6) {
        TEST_FAIL("Template Function case_set_value_7, case6 v is %d",result_f->getIntValue());
    }
    
    auto result_g = scopedValues->get(String::New("g"));
    if(result_g->getIntValue() != 4) {
        TEST_FAIL("Template Function case_set_value_7, case7 v is %d",result_g->getIntValue());
    }
    
    auto result_h = scopedValues->get(String::New("h"));
    if(result_h->getIntValue() != -98) {
        TEST_FAIL("Template Function case_set_value_7, case8 v is %d",result_h->getIntValue());
    }
    TEST_OK("Template Function case_set_value_7");
}
