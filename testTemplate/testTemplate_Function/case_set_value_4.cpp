#include "Template.hpp"
#include "TemplatePrintParser.hpp"
#include "TemplatePrintItem.hpp"
#include "FileInputStream.hpp"
#include "TestLog.hpp"
#include "Reflect.hpp"

using namespace obotcha;
using namespace gagira;

DECLARE_CLASS(SetValue4_Data) {
public:
    int data;
    DECLARE_REFLECT_FIELD(SetValue4_Data,data)
};

void case_set_value_4() {    
    auto input = FileInputStream::New("./test_case/case_set_value_4.txt");
    input->open();
    String content = input->readAll()->toString();
    Template t = Template::New(content);
    auto scopedValues = HashMap<String,TemplateScopedValue>::New();
    auto obj = SetValue4_Data::New();
    obj->data = 1;
    auto result = t->execute(scopedValues,obj);
    auto scopedValue = scopedValues->get(String::New("v"));
    if(obj->data != 301) {
        TEST_FAIL("Template Function case_set_value_4, case1 data is %d",obj->data);
    }
    
    auto result_v = scopedValues->get(String::New("v"));
    if(result_v->getIntValue() != 101) {
        TEST_FAIL("Template Function case_set_value_4, case2 v is %d",result_v->getIntValue());
    }
    
    TEST_OK("Template Function case_set_value_4");
}
