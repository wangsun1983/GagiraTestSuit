#include "Template.hpp"
#include "TemplatePrintParser.hpp"
#include "TemplatePrintItem.hpp"
#include "FileInputStream.hpp"
#include "TestLog.hpp"
#include "Reflect.hpp"
#include "ReflectMethod.hpp"
using namespace obotcha;
using namespace gagira;

DECLARE_CLASS(TestCase4) {
public:
    String sum(String start,String end) {
        int _start = start->toBasicInt();
        int _end = end->toBasicInt();
        int sum = 0;
        for(int i = _start;i < _end;i++) {
            sum += i;
        }
        
        return String::New(sum);
    }
    
    DECLARE_REFLECT_METHOD(TestCase4,sum)
};

void test_case4() {    
    auto input = FileInputStream::New("./test_case/case4.txt");
    input->open();
    String content = input->readAll()->toString();
    Template t = Template::New(content);
    auto scopedValues = HashMap<String,TemplateScopedValue>::New();
    
    auto data = TestCase4::New();
    auto result = t->execute(scopedValues,data);
    if(!result->sameAs("100")) {
        TEST_FAIL("testTemplate_Method test_case4 case1,result is %s",result->toChars());
    }
    
    TEST_OK("testTemplate_Method test_case4 case100");
}
