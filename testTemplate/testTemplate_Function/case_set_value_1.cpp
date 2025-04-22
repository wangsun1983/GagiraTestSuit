#include "Template.hpp"
#include "TemplatePrintParser.hpp"
#include "TemplatePrintItem.hpp"
#include "FileInputStream.hpp"
#include "TestLog.hpp"
#include "Reflect.hpp"

using namespace obotcha;
using namespace gagira;

void case_set_value_1() {    
    auto input = FileInputStream::New("./test_case/case_set_value_1.txt");
    input->open();
    String content = input->readAll()->toString();
    Template t = Template::New(content);
    auto scopedValues = HashMap<String,TemplateScopedValue>::New();
    
    auto result = t->execute(scopedValues,nullptr);
    auto scopedValue = scopedValues->get(String::New("v"));
    if(scopedValue->getIntValue() != 223) {
        TEST_FAIL("Template Function case_set_value_1 case1");
    }
    
    TEST_OK("Template Function case_set_value_1");
}
