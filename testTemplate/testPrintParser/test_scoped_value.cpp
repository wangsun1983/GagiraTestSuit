#include "Template.hpp"
#include "TemplatePrintParser.hpp"
#include "TemplatePrintItem.hpp"
#include "Reflect.hpp"

using namespace obotcha;
using namespace gagira;

void test_scoped_value() {    
    // while(1) {
    //     String case1_str = "\"a is #1,b is #2\",tag1,tag2";
    //     auto scopedValues = HashMap<String,TemplateScopedValue>::New();
    //     auto scopedValue = TemplateScopedValue::New(Integer::New(2));
    //     auto scopedValue2 = TemplateScopedValue::New(Integer::New(3));
    //     scopedValues->put(String::New("tag1"),scopedValue);
    //     scopedValues->put(String::New("tag2"),scopedValue2);
        
    //     auto parser = TemplatePrintParser::New(case1_str);
    //     auto item = parser->doParse();
        
    //     String result = item->execute(scopedValues,
    //                                   nullptr,
    //                                   0);
    //     printf("[testPrintParser normal test] result is %s \n",result->toChars());
    //     break;
    // }
    
    while(1) {
        String case1_str = " (\"a is #1,b is #2\",tag1,tag2)";
        auto scopedValues = HashMap<String,TemplateScopedValue>::New();
        auto scopedValue = TemplateScopedValue::New(String::New("this is value1"));
        auto scopedValue2 = TemplateScopedValue::New(String::New("this is value2"));
        scopedValues->put(String::New("tag1"),scopedValue);
        scopedValues->put(String::New("tag2"),scopedValue2);
        
        auto parser = TemplatePrintParser::New(case1_str);
        auto item = parser->doParse();
        
        String result = item->execute(scopedValues,
                                      nullptr,
                                      0);
        printf("[testPrintParser normal test] result is %s \n",result->toChars());
        break;
    }
}
