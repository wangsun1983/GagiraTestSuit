#include <stdio.h>
#include <unistd.h>

#include "Template.hpp"
#include "TemplateFunctionParser.hpp"
#include "Reflect.hpp"
#include "TestLog.hpp"

using namespace obotcha;
using namespace gagira;

DECLARE_CLASS(MixData) {
public:
    int value;
    DECLARE_REFLECT_FIELD(MixData,value);
};

void test_mixture_value_Int() {
    
    while(1) {
        auto scopedValues = HashMap<String,TemplateScopedValue>::New();
        auto scopedValue = TemplateScopedValue::New(Integer::New(2));
        auto scopedValue2 = TemplateScopedValue::New(Integer::New(3));
        scopedValues->put(String::New("tag1"),scopedValue);
        scopedValues->put(String::New("tag2"),scopedValue2);
        
        MixData data = MixData::New();
        data->value = 7;
        
        String str1 = String::New("tag1 + (tag2 * .value)");
        auto parser = TemplateFunctionParser::New(str1);
        auto item = parser->doParse();
        auto result = item->execute(scopedValues,data);
        if(result->getIntValue() != 23) {
            TEST_FAIL("testFunctionItem mixture_value_int case1 ,value is %d",
                        result->getIntValue());
        }
        
        if(result->getType() != st(TemplateScopedValue)::Type::Integer) {
            TEST_FAIL("testFunctionItem mixture_value_int case1_2 ,type is %d",
                        result->getType());
        }
        break;
    } 
    
    TEST_OK("testFunctionItem mixture_value_int case100");
}
