#include "Template.hpp"
#include "TemplatePrintParser.hpp"
#include "TemplatePrintItem.hpp"
#include "FileInputStream.hpp"
#include "TestLog.hpp"
#include "Reflect.hpp"

using namespace obotcha;
using namespace gagira;

DECLARE_CLASS(TestCase1_Data) {
public:
    int value;
    DECLARE_REFLECT_FIELD(TestCase1_Data,value)
};

void test_case1() {    
    auto input = FileInputStream::New("./test_case/case1.txt");
    input->open();
    String content = input->readAll()->toString();
    Template t = Template::New(content);
    auto scopedValues = HashMap<String,TemplateScopedValue>::New();
    
    auto data = TestCase1_Data::New();
    data->value = 0;
    
    auto result = t->execute(scopedValues,data);
    if(data->value != 101 ) {
        TEST_FAIL("testTemplate_If test_case1 case1,data->value is %d",data->value);
    }
    
    TEST_OK("testTemplate_If test_case1 case100");
}
