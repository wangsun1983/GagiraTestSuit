#include "Template.hpp"
#include "TemplatePrintParser.hpp"
#include "TemplatePrintItem.hpp"
#include "FileInputStream.hpp"
#include "TestLog.hpp"
#include "Reflect.hpp"
#include "ForEveryOne.hpp"

using namespace obotcha;
using namespace gagira;

void test_case3() {    
    auto input = FileInputStream::New("./test_case/case3.txt");
    input->open();
    String content = input->readAll()->toString();
    Template t = Template::New(content);
    auto scopedValues = HashMap<String,TemplateScopedValue>::New();
    auto result = t->execute(scopedValues,nullptr);
    auto v_result = scopedValues->get(String::New("value"));
    if(v_result->getIntValue() != 18) {
        TEST_FAIL("testTemplate_Loop test_case3 case1");
    }
    
    TEST_OK("testTemplate_Loop test_case3 case100");
}
