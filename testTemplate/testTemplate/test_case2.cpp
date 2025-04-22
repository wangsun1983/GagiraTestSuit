#include "Template.hpp"
#include "TemplatePrintParser.hpp"
#include "TemplatePrintItem.hpp"
#include "FileInputStream.hpp"
#include "TestLog.hpp"

using namespace obotcha;
using namespace gagira;

void test_case2() {    
    auto input = FileInputStream::New("./test_case/case2.txt");
    input->open();
    String content = input->readAll()->toString();
    Template t = Template::New(content);
    auto scopedValues = HashMap<String,TemplateScopedValue>::New();
    auto result = t->execute(scopedValues,nullptr);
    if(!result->sameAs("    <br ggcondition1>\n")) {
        TEST_FAIL("Template test_case2,result is [%s]",result->toChars());
        auto values = result->toChars();
    }
    
    TEST_OK("Template test_case2");
}
