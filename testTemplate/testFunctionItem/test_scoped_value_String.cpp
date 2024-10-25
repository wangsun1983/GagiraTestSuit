#include <stdio.h>
#include <unistd.h>

#include "Template.hpp"
#include "TemplateFunctionParser.hpp"
#include "Reflect.hpp"
#include "TestLog.hpp"

using namespace obotcha;
using namespace gagira;

void test_scoped_value_String() {
    
    while(1) {
        auto scopedValues = HashMap<String,TemplateScopedValue>::New();
        auto scopedValue = TemplateScopedValue::New(String::New("hello"));
        auto scopedValue2 = TemplateScopedValue::New(String::New("world"));
        scopedValues->put(String::New("tag1"),scopedValue);
        scopedValues->put(String::New("tag2"),scopedValue2);
        
        String str1 = String::New("tag1 + tag2");
        auto parser = TemplateFunctionParser::New(str1);
        auto item = parser->doParse();
        auto result = item->execute(scopedValues,nullptr);
        
        if(result->getStringValue()->sameAs("helloworld")) {
            TEST_FAIL("testFunctionItem scoped_value_string case2 ,value is %s",
                        result->getStringValue()->toChars());
        }
        
        if(result->getType() != st(TemplateScopedValue)::Type::String) {
            TEST_FAIL("testFunctionItem scoped_value_string case2_2 ,type is %d",
                        result->getType());
        }
        break;
    } 
    TEST_OK("testFunctionItem scoped_value_string case100");
}
