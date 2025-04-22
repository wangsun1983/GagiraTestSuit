#include "Template.hpp"
#include "TemplatePrintParser.hpp"
#include "TemplatePrintItem.hpp"
#include "FileInputStream.hpp"
#include "TestLog.hpp"
#include "Reflect.hpp"

using namespace obotcha;
using namespace gagira;

DECLARE_CLASS(SetValue2_Data) {
public:
    int data;
    DECLARE_REFLECT_FIELD(SetValue2_Data,data)
};

void case_set_value_2() {    
    auto input = FileInputStream::New("./test_case/case_set_value_2.txt");
    input->open();
    String content = input->readAll()->toString();
    Template t = Template::New(content);
    auto scopedValues = HashMap<String,TemplateScopedValue>::New();
    auto obj = SetValue2_Data::New();
    obj->data = 1;
    auto result = t->execute(scopedValues,obj);
    auto scopedValue = scopedValues->get(String::New("v"));
    if(obj->data != 1234) {
        TEST_FAIL("Template Function case_set_value_2,data is %d",obj->data);
    }
    
    TEST_OK("Template Function case_set_value_2");
}
