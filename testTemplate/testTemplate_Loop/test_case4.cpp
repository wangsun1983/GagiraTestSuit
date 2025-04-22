#include "Template.hpp"
#include "TemplatePrintParser.hpp"
#include "TemplatePrintItem.hpp"
#include "FileInputStream.hpp"
#include "TestLog.hpp"
#include "Reflect.hpp"
#include "ForEveryOne.hpp"

using namespace obotcha;
using namespace gagira;

DECLARE_CLASS(LoopTestCase4_Data) {
public:
    int count;
    DECLARE_REFLECT_FIELD(LoopTestCase4_Data,count)
};

void test_case4() {    
    auto input = FileInputStream::New("./test_case/case4.txt");
    input->open();
    String content = input->readAll()->toString();
    
    LoopTestCase4_Data data = LoopTestCase4_Data::New();
    data->count = 10;
    
    Template t = Template::New(content);
    auto scopedValues = HashMap<String,TemplateScopedValue>::New();
    auto result = t->execute(scopedValues,data);
    auto v_result = scopedValues->get(String::New("value"));
    if(v_result->getIntValue() != 18) {
        TEST_FAIL("testTemplate_Loop test_case4 case1");
    }
    
    TEST_OK("testTemplate_Loop test_case4 case100");
}
