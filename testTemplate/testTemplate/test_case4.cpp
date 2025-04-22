#include "Template.hpp"
#include "TemplatePrintParser.hpp"
#include "TemplatePrintItem.hpp"
#include "FileInputStream.hpp"
#include "TestLog.hpp"
#include "Reflect.hpp"

using namespace obotcha;
using namespace gagira;

DECLARE_CLASS(Case4Data) {
public:
    int value1;
    int value2;
    DECLARE_REFLECT_FIELD(Case4Data,value1,value2);
};

void test_case4() {    
    auto input = FileInputStream::New("./test_case/case4.txt");
    input->open();
    String content = input->readAll()->toString();
    Template t = Template::New(content);
    auto scopedValues = HashMap<String,TemplateScopedValue>::New();
    Case4Data data = Case4Data::New();
    data->value1 = 1;
    data->value2 = 1;
    
    auto result = t->execute(scopedValues,data);
    if(!result->sameAs("    <br ggcondition1>\n")) {
        TEST_FAIL("Template test_case4 issue1,result is %s \n",result->toChars());
    }
    
    if(data->value1 != 23 || data->value2 != 101) {
        TEST_FAIL("Template test_case4 issue2,value1 is %d,value2 is %d",data->value1,data->value2);
    }
    
    auto scoped_value = scopedValues->get(String::New("v"));
    if(scoped_value == nullptr) {
        TEST_FAIL("Template test_case4 issue3");
    } else {
        if(scoped_value->getIntValue() != 101) {
            TEST_FAIL("Template test_case4 issue4");
        }
    }
    
    TEST_OK("Template test_case4");
}
