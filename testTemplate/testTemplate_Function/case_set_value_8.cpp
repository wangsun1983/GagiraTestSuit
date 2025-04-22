#include "Template.hpp"
#include "TemplatePrintParser.hpp"
#include "TemplatePrintItem.hpp"
#include "FileInputStream.hpp"
#include "TestLog.hpp"
#include "Reflect.hpp"

using namespace obotcha;
using namespace gagira;

DECLARE_CLASS(SetValue8_Data) {
public:
    int data;
    DECLARE_REFLECT_FIELD(SetValue8_Data,data)
};

void case_set_value_8() {    
    auto input = FileInputStream::New("./test_case/case_set_value_8.txt");
    input->open();
    String content = input->readAll()->toString();
    Template t = Template::New(content);
    auto scopedValues = HashMap<String,TemplateScopedValue>::New();
    auto obj = SetValue8_Data::New();
    obj->data = 1;
    auto result = t->execute(scopedValues,obj);
    auto result_a = scopedValues->get(String::New("a"));
    if(result_a->getIntValue() != ((100 + 2)*3-8)) {
        TEST_FAIL("Template Function case_set_value_8, case1 v is %d",result_a->getIntValue());
    }
    
    auto result_b = scopedValues->get(String::New("b"));
    if(result_b->getIntValue() != ((100*2)+3*(8-2))) {
        TEST_FAIL("Template Function case_set_value_8, case2 v is %d",result_b->getIntValue());
    }
    
    auto value_a = result_a->getIntValue();
    auto value_b = result_b->getIntValue();
    auto result_c = scopedValues->get(String::New("c"));
    if(result_c->getIntValue() != (value_a*(value_b+2) + value_b*value_a-3)) {
        TEST_FAIL("Template Function case_set_value_8, case3 v is %d",result_c->getIntValue());
    }
    
    auto value_c = result_c->getIntValue();
    auto result_d = scopedValues->get(String::New("d"));
    if(result_d->getIntValue() != (value_c+2 + value_b*value_a-3)) {
        TEST_FAIL("Template Function case_set_value_8, case4 v is %d",result_d->getIntValue());
    }
    
    auto value_d = result_d->getIntValue();
    auto result_f = scopedValues->get(String::New("f"));
    if(result_f->getIntValue() != ((value_d+2 + value_b)*value_a-3)) {
        TEST_FAIL("Template Function case_set_value_8, case5 v is %d",result_f->getIntValue());
    }

    TEST_OK("Template Function case_set_value_8");
}
