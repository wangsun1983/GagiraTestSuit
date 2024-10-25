#include <stdio.h>
#include <unistd.h>

#include "Template.hpp"
#include "TemplateFunctionParser.hpp"
#include "Reflect.hpp"
#include "TestLog.hpp"

using namespace obotcha;
using namespace gagira;

void test_scoped_value_Int() {
    while(1) {
        auto scopedValues = HashMap<String,TemplateScopedValue>::New();
        auto scopedValue = TemplateScopedValue::New(Integer::New(2));
        auto scopedValue2 = TemplateScopedValue::New(Integer::New(3));
        scopedValues->put(String::New("tag1"),scopedValue);
        scopedValues->put(String::New("tag2"),scopedValue2);
        
        String str1 = String::New("tag1 + tag2");
        auto parser = TemplateFunctionParser::New(str1);
        auto item = parser->doParse();
        auto result = item->execute(scopedValues,nullptr);
        if(result->getIntValue() != 5) {
            TEST_FAIL("testFunctionItem scoped_value_int case1 ,value is %d",
                        result->getIntValue());
        }
        
        if(result->getType() != st(TemplateScopedValue)::Type::Integer) {
            TEST_FAIL("testFunctionItem scoped_value_int case16_2 ,type is %d",
                        result->getType());
        }
        break;
    }
    
    while(1) {
        auto scopedValues = HashMap<String,TemplateScopedValue>::New();
        auto scopedValue = TemplateScopedValue::New(Integer::New(2));
        auto scopedValue2 = TemplateScopedValue::New(Integer::New(3));
        scopedValues->put(String::New("tag1"),scopedValue);
        scopedValues->put(String::New("tag2"),scopedValue2);
        
        String str1 = String::New("tag1 + (tag2 * 2)");
        auto parser = TemplateFunctionParser::New(str1);
        auto item = parser->doParse();
        auto result = item->execute(scopedValues,nullptr);
        if(result->getIntValue() != 8) {
            TEST_FAIL("testFunctionItem scoped_value_int case2 ,value is %d",
                        result->getIntValue());
        }
        
        if(result->getType() != st(TemplateScopedValue)::Type::Integer) {
            TEST_FAIL("testFunctionItem scoped_value_int case2_2 ,type is %d",
                        result->getType());
        }
        break;
    }
    
    TEST_OK("testFunctionItem scoped_value_int case100");
}
