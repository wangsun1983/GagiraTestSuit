#include "Template.hpp"
#include "TemplatePrintParser.hpp"
#include "TemplatePrintItem.hpp"
#include "FileInputStream.hpp"
#include "TestLog.hpp"
#include "Reflect.hpp"

using namespace obotcha;
using namespace gagira;

DECLARE_CLASS(SetValue11_Data) {
public:
    int data;
    DECLARE_REFLECT_FIELD(SetValue11_Data,data)
};

void case_set_value_11() {    
    auto input = FileInputStream::New("./test_case/case_set_value_11.txt");
    input->open();
    String content = input->readAll()->toString();
    Template t = Template::New(content);
    auto scopedValues = HashMap<String,TemplateScopedValue>::New();
    auto obj = SetValue11_Data::New();
    obj->data = 12;
    auto result = t->execute(scopedValues,obj);
    auto result_v1 = scopedValues->get(String::New("v1"));
    if(result_v1->getIntValue() != 12) {
        TEST_FAIL("Template Function case_set_value_11,case1 data is %d",result_v1->getIntValue());
    }
    
    auto result_v2 = scopedValues->get(String::New("v2"));
    if(result_v2->getIntValue() != 124) {
        TEST_FAIL("Template Function case_set_value_11,case2 data is %d",result_v2->getIntValue());
    }
    
    TEST_OK("Template Function case_set_value_11");
}
