#include "Template.hpp"
#include "TemplatePrintParser.hpp"
#include "TemplatePrintItem.hpp"
#include "FileInputStream.hpp"
#include "TestLog.hpp"
#include "Reflect.hpp"

using namespace obotcha;
using namespace gagira;

DECLARE_CLASS(Case3Data) {
public:
    int value;
    DECLARE_REFLECT_FIELD(Case3Data,value);
};

void test_case3() {    
    auto input = FileInputStream::New("./test_case/case3.txt");
    input->open();
    String content = input->readAll()->toString();
    Template t = Template::New(content);
    auto scopedValues = HashMap<String,TemplateScopedValue>::New();
    Case3Data data = Case3Data::New();
    data->value = 1000;
    
    auto result = t->execute(scopedValues,data);
    if(!result->sameAs("    <br ggcondition1>\n")) {
        TEST_FAIL("Template test_case3 issue1,result is [%s] \n",result->toChars());
    }
    
    if(data->value != 2) {
        TEST_FAIL("Template test_case3 issue2");
    }
    
    auto scoped_value = scopedValues->get(String::New("v"));
    if(scoped_value == nullptr) {
        TEST_FAIL("Template test_case3 issue3");
    } else {
        if(scoped_value->getIntValue() != 101) {
            TEST_FAIL("Template test_case3 issue4");
        }
    }
    
    TEST_OK("Template test_case3");
}
