#include <stdio.h>
#include <unistd.h>

#include "Template.hpp"
#include "TemplateFunctionParser.hpp"
#include "Reflect.hpp"
#include "TestLog.hpp"

using namespace obotcha;
using namespace gagira;

DECLARE_CLASS(MixBoolData) {
public:
    bool value;
    DECLARE_REFLECT_FIELD(MixBoolData,value);
};

void test_mixture_value_bool() {
    
    // while(1) {
    //     auto scopedValues = HashMap<String,TemplateScopedValue>::New();
    //     auto scopedValue = TemplateScopedValue::New(Boolean::New(true));
    //     auto scopedValue2 = TemplateScopedValue::New(Boolean::New(true));
    //     scopedValues->put(String::New("tag1"),scopedValue);
    //     scopedValues->put(String::New("tag2"),scopedValue2);
        
    //     MixBoolData data = MixBoolData::New();
    //     data->value = false;
        
    //     String str1 = String::New("tag1 && tag2 || .value");
    //     auto parser = TemplateFunctionParser::New(str1);
    //     auto item = parser->doParse();
    //     auto result = item->execute(scopedValues,data);
    //     if(result->getBoolValue() != true) {
    //         TEST_FAIL("testFunctionItem mixture_value_bool case1");
    //     }
        
    //     if(result->getType() != st(TemplateScopedValue)::Type::Bool) {
    //         TEST_FAIL("testFunctionItem mixture_value_bool case1_2 ,type is %d",
    //                     result->getType());
    //     }
    //     break;
    // } 
    
    
    // while(1) {
    //     auto scopedValues = HashMap<String,TemplateScopedValue>::New();
    //     auto scopedValue = TemplateScopedValue::New(Boolean::New(true));
    //     auto scopedValue2 = TemplateScopedValue::New(Boolean::New(true));
    //     scopedValues->put(String::New("tag1"),scopedValue);
    //     scopedValues->put(String::New("tag2"),scopedValue2);
        
    //     MixBoolData data = MixBoolData::New();
    //     data->value = false;
        
    //     String str1 = String::New("tag1 && tag2 && .value");
    //     auto parser = TemplateFunctionParser::New(str1);
    //     auto item = parser->doParse();
    //     auto result = item->execute(scopedValues,data);
    //     if(result->getBoolValue() != false) {
    //         TEST_FAIL("testFunctionItem mixture_value_bool case2");
    //     }
        
    //     if(result->getType() != st(TemplateScopedValue)::Type::Bool) {
    //         TEST_FAIL("testFunctionItem mixture_value_bool case2_2 ,type is %d",
    //                     result->getType());
    //     }
    //     break;
    // } 
    
    
    while(1) {
        auto scopedValues = HashMap<String,TemplateScopedValue>::New();
        auto scopedValue = TemplateScopedValue::New(Boolean::New(false));
        auto scopedValue2 = TemplateScopedValue::New(Boolean::New(false));
        scopedValues->put(String::New("tag1"),scopedValue);
        scopedValues->put(String::New("tag2"),scopedValue2);
        
        MixBoolData data = MixBoolData::New();
        data->value = false;
        
        String str1 = String::New("!tag1 && !.value");
        auto parser = TemplateFunctionParser::New(str1);
        auto item = parser->doParse();
        auto result = item->execute(scopedValues,data);
        if(result->getBoolValue() != true) {
            TEST_FAIL("testFunctionItem mixture_value_bool case3");
        }
        
        if(result->getType() != st(TemplateScopedValue)::Type::Bool) {
            TEST_FAIL("testFunctionItem mixture_value_bool case3_2 ,type is %d",
                        result->getType());
        }
        break;
    } 
    TEST_OK("testFunctionItem mixture_value_bool case100");
}
