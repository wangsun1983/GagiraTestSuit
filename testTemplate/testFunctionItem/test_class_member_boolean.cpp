#include <stdio.h>
#include <unistd.h>

#include "Template.hpp"
#include "TemplateFunctionParser.hpp"
#include "Reflect.hpp"
#include "TestLog.hpp"

using namespace obotcha;
using namespace gagira;

DECLARE_CLASS(BoolData) {
public:
    bool value;
    bool value2;
    DECLARE_REFLECT_FIELD(BoolData,value,value2);
};

void test_class_member_boolean() {
    auto scopedValues = HashMap<String,TemplateScopedValue>::New();
    
    while(1) {
        BoolData d = BoolData::New();
        d->value = true;
        String str1 = String::New(".value");
        auto parser = TemplateFunctionParser::New(str1);
        auto item = parser->doParse();
        auto result = item->execute(scopedValues,d);
        if(result->getBoolValue() != true) {
            TEST_FAIL("testFunctionItem class_member_boolean case1 ,value is %d",
                        result->getIntValue());
        }
        
        if(result->getType() != st(TemplateScopedValue)::Type::Bool) {
            TEST_FAIL("testFunctionItem class_member_boolean case1_2 ,type is %d",
                        result->getType());
        }
        break;
    } 
    
    while(1) {
        BoolData d = BoolData::New();
        d->value = true;
        String str1 = String::New(".value && ((5+1) > 10)");
        auto parser = TemplateFunctionParser::New(str1);
        auto item = parser->doParse();
        auto result = item->execute(scopedValues,d);
        if(result->getBoolValue() != false) {
            TEST_FAIL("testFunctionItem class_member_boolean case2");
        }
        
        if(result->getType() != st(TemplateScopedValue)::Type::Bool) {
            TEST_FAIL("testFunctionItem class_member_boolean case2_2 ,type is %d",
                        result->getType());
        }
        break;
    } 
    
    TEST_OK("testFunctionItem class_member_boolean case100");
}
