#include "Template.hpp"
#include "TemplatePrintParser.hpp"
#include "TemplatePrintItem.hpp"
#include "FileInputStream.hpp"
#include "TestLog.hpp"
#include "Reflect.hpp"

using namespace obotcha;
using namespace gagira;

DECLARE_CLASS(TestCase4_Data) {
public:
    int value;
    DECLARE_REFLECT_FIELD(TestCase4_Data,value)
};

void test_case4() {    
    auto input = FileInputStream::New("./test_case/case4.txt");
    input->open();
    String content = input->readAll()->toString();
    Template t = Template::New(content);
    auto scopedValues = HashMap<String,TemplateScopedValue>::New();
    
    auto data = TestCase4_Data::New();
    data->value = 0;
    
    auto result = t->execute(scopedValues,data);
    if(data->value != 103 ) {
        TEST_FAIL("testTemplate_If test_case4 case1,data->value is %d",data->value);
    }
    
    TEST_OK("testTemplate_If test_case4 case100");
}
