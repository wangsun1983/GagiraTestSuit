#include "Template.hpp"
#include "TemplatePrintParser.hpp"
#include "TemplatePrintItem.hpp"
#include "FileInputStream.hpp"
#include "TestLog.hpp"

using namespace obotcha;
using namespace gagira;

void test_case1() {    
    auto input = FileInputStream::New("./test_case/case1.txt");
    input->open();
    String content = input->readAll()->toString();
    Template t = Template::New(content);
    auto scopedValues = HashMap<String,TemplateScopedValue>::New();
    auto result = t->execute(scopedValues,nullptr);
    if(!result->sameAs("    fffcondition2\n")) {
        TEST_FAIL("Template test_case1,result is [%s]",result->toChars());
    }
    
    TEST_OK("Template test_case1");
}
