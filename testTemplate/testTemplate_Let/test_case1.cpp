#include "Template.hpp"
#include "TemplatePrintParser.hpp"
#include "TemplatePrintItem.hpp"
#include "FileInputStream.hpp"
#include "TestLog.hpp"
#include "Reflect.hpp"

using namespace obotcha;
using namespace gagira;

void test_case1() {    
    auto input = FileInputStream::New("./test_case/case1.txt");
    input->open();
    String content = input->readAll()->toString();
    Template t = Template::New(content);
    auto scopedValues = HashMap<String,TemplateScopedValue>::New();
    
    auto result = t->execute(scopedValues,nullptr);
    auto scopedValue = scopedValues->get(String::New("v"));
    if(scopedValue->getIntValue() != 123) {
        TEST_FAIL("testTemplate_Let test_case1 case1");
    }
    
    auto scopedValue2 = scopedValues->get(String::New("v2"));
    if(scopedValue2->getIntValue() != 222) {
        TEST_FAIL("testTemplate_Let test_case1 case2");
    }
    
    auto scopedValue3 = scopedValues->get(String::New("v3"));
    if(scopedValue3->getIntValue() != 234) {
        TEST_FAIL("testTemplate_Let test_case1 case3");
    }
    
    TEST_OK("testTemplate_Let test_case1 case100");
}
