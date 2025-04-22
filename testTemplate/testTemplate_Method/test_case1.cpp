#include "Template.hpp"
#include "TemplatePrintParser.hpp"
#include "TemplatePrintItem.hpp"
#include "FileInputStream.hpp"
#include "TestLog.hpp"
#include "Reflect.hpp"
#include "ReflectMethod.hpp"
using namespace obotcha;
using namespace gagira;

DECLARE_CLASS(TestCase1) {
public:
    String value_large(String arg1) {
        return arg1->append(" is large");
    }
    
    String value_small(String arg1) {
        return arg1->append(" is smaller");
    }
    
    DECLARE_REFLECT_METHOD(TestCase1,value_large,value_small)
};

void test_case1() {    
    auto input = FileInputStream::New("./test_case/case1.txt");
    input->open();
    String content = input->readAll()->toString();
    Template t = Template::New(content);
    auto scopedValues = HashMap<String,TemplateScopedValue>::New();
    
    auto data = TestCase1::New();
    auto result = t->execute(scopedValues,data);
    if(!result->sameAs("2 is smaller")) {
        TEST_FAIL("testTemplate_Method test_case1 case1,result is %s",result->toChars());
    }
    
    TEST_OK("testTemplate_Method test_case1 case100");
}
