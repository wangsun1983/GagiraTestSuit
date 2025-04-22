#include "Template.hpp"
#include "TemplatePrintParser.hpp"
#include "TemplatePrintItem.hpp"
#include "FileInputStream.hpp"
#include "TestLog.hpp"
#include "Reflect.hpp"
#include "ForEveryOne.hpp"

using namespace obotcha;
using namespace gagira;

DECLARE_CLASS(LoopTestCase6_Data) {
public:
    int count;
    int value;
    DECLARE_REFLECT_FIELD(LoopTestCase6_Data,count,value)
};

void test_case6() {    
    auto input = FileInputStream::New("./test_case/case6.txt");
    input->open();
    String content = input->readAll()->toString();
    
    LoopTestCase6_Data data = LoopTestCase6_Data::New();
    data->count = 3;
    data->value = 0;
    
    Template t = Template::New(content);
    auto scopedValues = HashMap<String,TemplateScopedValue>::New();
    auto result = t->execute(scopedValues,data);
    if(data->value != 4) {
        TEST_FAIL("testTemplate_Loop test_case6 case1,data->value is %d",data->value);
    }
    
    TEST_OK("testTemplate_Loop test_case6 case100");
}
