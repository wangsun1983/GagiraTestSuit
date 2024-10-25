#include "Template.hpp"
#include "TemplatePrintParser.hpp"
#include "TemplatePrintItem.hpp"
#include "Reflect.hpp"

using namespace obotcha;
using namespace gagira;

DECLARE_CLASS(Data) {
public:
    int value1;
    int value2;
    DECLARE_REFLECT_FIELD(Data,value1,value2);
};

void test_class_member() {    
    while(1) {
        Data d = Data::New();
        d->value1 = 123;
        d->value2 = 456;
        
        String case1_str = "\"a is #1,b is #2\",.value1,.value2";
        auto scopedValues = HashMap<String,TemplateScopedValue>::New();
        auto parser = TemplatePrintParser::New(case1_str);
        auto item = parser->doParse();
        
        String result = item->execute(scopedValues,
                                      d,
                                      0);
        printf("[testPrintParser normal test] result is %s \n",result->toChars());
        break;
    }
}
