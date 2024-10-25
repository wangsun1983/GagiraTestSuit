#include <stdio.h>
#include <unistd.h>

#include "Template.hpp"
#include "TemplateFunctionParser.hpp"
#include "Reflect.hpp"
#include "TestLog.hpp"

using namespace obotcha;
using namespace gagira;

DECLARE_CLASS(MixStringData) {
public:
    String value;
    DECLARE_REFLECT_FIELD(MixStringData,value);
};

void test_mixture_value_String() {
    
    while(1) {
        auto scopedValues = HashMap<String,TemplateScopedValue>::New();
        auto scopedValue = TemplateScopedValue::New(String::New("hello"));
        auto scopedValue2 = TemplateScopedValue::New(String::New("world"));
        scopedValues->put(String::New("tag1"),scopedValue);
        scopedValues->put(String::New("tag2"),scopedValue2);
        
        MixStringData data = MixStringData::New();
        data->value = String::New("wang");
        
        String str1 = String::New("tag1 + tag2 + haha + .value)");
        auto parser = TemplateFunctionParser::New(str1);
        auto item = parser->doParse();
        auto result = item->execute(scopedValues,data);
        if(result->getStringValue()->sameAs("helloworldhahawang")) {
            TEST_FAIL("testFunctionItem mixture_value_string case1 ,value is %s",
                        result->getStringValue()->toChars());
        }
        
        if(result->getType() != st(TemplateScopedValue)::Type::String) {
            TEST_FAIL("testFunctionItem mixture_value_string case1_2 ,type is %d",
                        result->getType());
        }
        break;
    } 
    TEST_OK("testFunctionItem mixture_value_string case100");
}
