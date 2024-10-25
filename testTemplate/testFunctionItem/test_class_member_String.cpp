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

DECLARE_CLASS(Data2) {
public:
    String value;
    String value2;
    DECLARE_REFLECT_FIELD(Data2,value,value2);
};

void test_class_member_String() {
    auto scopedValues = HashMap<String,TemplateScopedValue>::New();
    
    while(1) {
        Data d = Data::New();
        d->value = 300;
        d->value2 = 411;
        String str1 = String::New(".value2 + .value");
        auto parser = TemplateFunctionParser::New(str1);
        auto item = parser->doParse();
        auto result = item->execute(scopedValues,d);
        if(result->getIntValue() != 711) {
            TEST_FAIL("testFunctionItem class_member_string case1 ,value is %d",
                        result->getIntValue());
        }
        
        if(result->getType() != st(TemplateScopedValue)::Type::Integer) {
            TEST_FAIL("testFunctionItem class_member_string case1_2 ,type is %d",
                        result->getType());
        }
        break;
    } 
    
    while(1) {
        Data2 d = Data2::New();
        d->value = String::New("hello");
        d->value2 = String::New("world");
        
        String str1 = String::New(".value + .value2");
        auto parser = TemplateFunctionParser::New(str1);
        auto item = parser->doParse();
        auto result = item->execute(scopedValues,d);
        if(result->getStringValue()->sameAs("helloworld")) {
            TEST_FAIL("testFunctionItem class_member_string case2 ,value is %s",
                        result->getStringValue()->toChars());
        }
        
        if(result->getType() != st(TemplateScopedValue)::Type::String) {
            TEST_FAIL("testFunctionItem class_member_string case2_2 ,type is %d",
                        result->getType());
        }
        break;
    } 
    
    TEST_OK("testFunctionItem class_member_string case100");
}
