#include "Template.hpp"
#include "TemplatePrintParser.hpp"
#include "TemplatePrintItem.hpp"
#include "FileInputStream.hpp"
#include "TestLog.hpp"
#include "Reflect.hpp"

using namespace obotcha;
using namespace gagira;

DECLARE_CLASS(Case5Data) {
public:
    int value1;
    DECLARE_REFLECT_FIELD(Case5Data,value1);
};

void test_case5() {    
    auto input = FileInputStream::New("./test_case/case5.txt");
    input->open();
    String content = input->readAll()->toString();
    Template t = Template::New(content);
    auto scopedValues = HashMap<String,TemplateScopedValue>::New();
    Case5Data data = Case5Data::New();
    data->value1 = 0;
    
    auto result = t->execute(scopedValues,data);
    if(!result->sameAs("    <br fffcondition2>\n")) {
        TEST_FAIL("Template test_case5 issue1,result is %s \n",result->toChars());
    }
    
    if(data->value1 != 2) {
        TEST_FAIL("Template test_case5 issue2,value1 is %d",data->value1);
    }
    
    auto scoped_value = scopedValues->get(String::New("v"));
    if(scoped_value == nullptr) {
        TEST_FAIL("Template test_case5 issue3");
    } else {
        if(scoped_value->getIntValue() != 3) {
            TEST_FAIL("Template test_case5 issue4");
        }
    }
    
    TEST_OK("Template test_case5");
}
