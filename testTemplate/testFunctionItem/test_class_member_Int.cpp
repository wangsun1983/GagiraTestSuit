#include <stdio.h>
#include <unistd.h>

#include "Template.hpp"
#include "TemplateFunctionParser.hpp"
#include "Reflect.hpp"
#include "TestLog.hpp"

using namespace obotcha;
using namespace gagira;

DECLARE_CLASS(Data) {
public:
    int value;
    int value2;
    DECLARE_REFLECT_FIELD(Data,value,value2);
};

void test_class_member_Int() {
    auto scopedValues = HashMap<String,TemplateScopedValue>::New();
    
    while(1) {
        Data d = Data::New();
        d->value = 100;
        String str1 = String::New(".value");
        auto parser = TemplateFunctionParser::New(str1);
        auto item = parser->doParse();
        auto result = item->execute(scopedValues,d);
        if(result->getIntValue() != 100) {
            TEST_FAIL("testFunctionItem class_member_int case1 ,value is %d",
                        result->getIntValue());
        }
        
        if(result->getType() != st(TemplateScopedValue)::Type::Integer) {
            TEST_FAIL("testFunctionItem class_member_int case1_2 ,type is %d",
                        result->getType());
        }
        break;
    } 
    
    while(1) {
        Data d = Data::New();
        d->value = 100;
        String str1 = String::New("2 * (5 + .value)");
        auto parser = TemplateFunctionParser::New(str1);
        auto item = parser->doParse();
        auto result = item->execute(scopedValues,d);
        if(result->getIntValue() != 210) {
            TEST_FAIL("testFunctionItem class_member_int case2 ,value is %d",
                        result->getIntValue());
        }
        
        if(result->getType() != st(TemplateScopedValue)::Type::Integer) {
            TEST_FAIL("testFunctionItem class_member_int case2_2 ,type is %d",
                        result->getType());
        }
        break;
    } 
    
    while(1) {
        Data d = Data::New();
        d->value = 3;
        d->value2 = 4;
        String str1 = String::New("(2 + .value2) * (5 + .value)");
        auto parser = TemplateFunctionParser::New(str1);
        auto item = parser->doParse();
        auto result = item->execute(scopedValues,d);
        if(result->getIntValue() != 48) {
            TEST_FAIL("testFunctionItem class_member_int case3 ,value is %d",
                        result->getIntValue());
        }
        
        if(result->getType() != st(TemplateScopedValue)::Type::Integer) {
            TEST_FAIL("testFunctionItem class_member_int case3_2 ,type is %d",
                        result->getType());
        }
        break;
    } 
    
    while(1) {
        Data d = Data::New();
        d->value = 3;
        d->value2 = 4;
        String str1 = String::New(".value2 * .value");
        auto parser = TemplateFunctionParser::New(str1);
        auto item = parser->doParse();
        auto result = item->execute(scopedValues,d);
                                      
        if(result->getIntValue() != 12) {
            TEST_FAIL("testFunctionItem class_member_int case4 ,value is %d",
                        result->getIntValue());
        }
        
        if(result->getType() != st(TemplateScopedValue)::Type::Integer) {
            TEST_FAIL("testFunctionItem class_member_int case4_2 ,type is %d",
                        result->getType());
        }
        break;
    } 
    
    TEST_OK("testFunctionItem class_member_int case100");
}
