#include "Template.hpp"
#include "TemplatePrintParser.hpp"
#include "TemplatePrintItem.hpp"
#include "FileInputStream.hpp"
#include "TestLog.hpp"
#include "Reflect.hpp"

using namespace obotcha;
using namespace gagira;

DECLARE_CLASS(SetValue5_Data) {
public:
    int data;
    DECLARE_REFLECT_FIELD(SetValue5_Data,data)
};

void case_set_value_5() {    
    auto input = FileInputStream::New("./test_case/case_set_value_5.txt");
    input->open();
    String content = input->readAll()->toString();
    Template t = Template::New(content);
    auto scopedValues = HashMap<String,TemplateScopedValue>::New();
    auto obj = SetValue5_Data::New();
    obj->data = 1;
    auto result = t->execute(scopedValues,obj);
    auto scopedValue = scopedValues->get(String::New("v"));
    if(obj->data != 400) {
        TEST_FAIL("Template Function case_set_value_5, case1 data is %d",obj->data);
    }
    
    auto result_v = scopedValues->get(String::New("v"));
    if(result_v->getIntValue() != 103) {
        TEST_FAIL("Template Function case_set_value_5, case2 v is %d",result_v->getIntValue());
    }
    
    TEST_OK("Template Function case_set_value_5");
}
