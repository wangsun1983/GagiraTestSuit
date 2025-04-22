#include <stdio.h>
#include <unistd.h>

#include "Template.hpp"
#include "TemplateFunctionParser.hpp"
#include "Log.hpp"
#include "TestLog.hpp"

using namespace obotcha;
using namespace gagira;

void test_basic_boolean() {
    auto scopedValues = HashMap<String,TemplateScopedValue>::New();
    
    while(1) {
        String str1 = String::New("1 == 1");
        auto parser = TemplateFunctionParser::New(str1);
        auto item = parser->doParse();
        auto result = item->execute(scopedValues,nullptr);
        if(result->getBoolValue() != true) {
            TEST_FAIL("testFunctionItem test_basic_boolean case1");
        }
        
        if(result->getType() != st(TemplateScopedValue)::Type::Bool) {
            TEST_FAIL("testFunctionItem basic_boolean case1_2 ,type is %d",
                        result->getType());
        }
        break;
    }
                            
    while(1) {
        String str1 = String::New("5 > (7+8)");
        auto parser = TemplateFunctionParser::New(str1);
        auto item = parser->doParse();
        auto result = item->execute(scopedValues,nullptr);
        if(result->getBoolValue() != false) {
            TEST_FAIL("testFunctionItem basic_boolean case2");
        }
        
        if(result->getType() != st(TemplateScopedValue)::Type::Bool) {
            TEST_FAIL("testFunctionItem basic_boolean case2_2 ,type is %d",
                        result->getType());
        }
        break;
    }    
    
    while(1) {
        String str1 = String::New("(5 + 1) != (7+8)");
        auto parser = TemplateFunctionParser::New(str1);
        auto item = parser->doParse();
        auto result = item->execute(scopedValues,nullptr);
        if(result->getBoolValue() != true) {
            TEST_FAIL("testFunctionItem basic_boolean case3");
        }
        
        if(result->getType() != st(TemplateScopedValue)::Type::Bool) {
            TEST_FAIL("testFunctionItem basic_boolean case3_2 ,type is %d",
                        result->getType());
        }
        break;
    } 
    
    TEST_OK("testFunctionItem basic_boolean case100");
}
