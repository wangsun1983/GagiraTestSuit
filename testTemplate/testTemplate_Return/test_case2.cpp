#include "Template.hpp"
#include "TemplatePrintParser.hpp"
#include "TemplatePrintItem.hpp"
#include "FileInputStream.hpp"
#include "TestLog.hpp"
#include "Reflect.hpp"
#include "ReflectMethod.hpp"
using namespace obotcha;
using namespace gagira;

void test_case2() {    
    auto input = FileInputStream::New("./test_case/case2.txt");
    input->open();
    String content = input->readAll()->toString();
    Template t = Template::New(content);
    auto scopedValues = HashMap<String,TemplateScopedValue>::New();
    
    auto result = t->execute(scopedValues,nullptr);
    if(!result->sameAs("")) {
        TEST_FAIL("testTemplate_Return test_case2 case1,result is %s",result->toChars());
    }
    
    TEST_OK("testTemplate_Return test_case2case100");
}
