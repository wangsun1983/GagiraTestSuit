#include <stdio.h>
#include <unistd.h>

#include "Template.hpp"
#include "TemplateFunctionParser.hpp"
#include "Reflect.hpp"
#include "TestLog.hpp"

using namespace obotcha;
using namespace gagira;

void test_basic_String() {
    auto scopedValues = HashMap<String,TemplateScopedValue>::New();
    
    while(1) {
        String str1 = String::New(" \"abc\" + \"ffgg\" + \"qq\"");
        auto parser = TemplateFunctionParser::New(str1);
        auto item = parser->doParse();
        auto result = item->execute(scopedValues,nullptr);
        if(result->getStringValue()->sameAs("abcffggqq")) {
            TEST_FAIL("testFunctionItem basic_string case1 ,value is %s",
                        result->getStringValue()->toChars());
        }
        
        if(result->getType() != st(TemplateScopedValue)::Type::String) {
            TEST_FAIL("testFunctionItem basic_int case1_2 ,type is %d",
                        result->getType());
        }
        break;
    } 
    
    while(1) {
        String str1 = String::New(" \"abc\" + \"   \" + \"ffgg\" + \"qq\"");
        auto parser = TemplateFunctionParser::New(str1);
        auto item = parser->doParse();
        auto result = item->execute(scopedValues,nullptr);
        if(result->getStringValue()->sameAs("abc   ffggqq")) {
            TEST_FAIL("testFunctionItem basic_string case2 ,value is %s",
                        result->getStringValue()->toChars());
        }
        
        if(result->getType() != st(TemplateScopedValue)::Type::String) {
            TEST_FAIL("testFunctionItem basic_int case22 ,type is %d",
                        result->getType());
        }
        break;
    }
    
    TEST_OK("testFunctionItem basic_string case100")
}
