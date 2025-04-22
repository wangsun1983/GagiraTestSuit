#include "Template.hpp"
#include "TemplatePrintParser.hpp"
#include "TemplatePrintItem.hpp"
#include "FileInputStream.hpp"
#include "TestLog.hpp"
#include "Reflect.hpp"
#include "ForEveryOne.hpp"

using namespace obotcha;
using namespace gagira;

DECLARE_CLASS(LoopTestCase5_Data) {
public:
    int count;
    int value;
    DECLARE_REFLECT_FIELD(LoopTestCase5_Data,count,value)
};

void test_case5() {    
    auto input = FileInputStream::New("./test_case/case5.txt");
    input->open();
    String content = input->readAll()->toString();
    
    LoopTestCase5_Data data = LoopTestCase5_Data::New();
    data->count = 10;
    
    Template t = Template::New(content);
    auto scopedValues = HashMap<String,TemplateScopedValue>::New();
    auto result = t->execute(scopedValues,data);
    if(data->value != 18) {
        TEST_FAIL("testTemplate_Loop test_case5 case1,data->value is %d",data->value);
    }
    
    TEST_OK("testTemplate_Loop test_case5 case100");
}
