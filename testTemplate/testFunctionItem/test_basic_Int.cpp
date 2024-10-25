#include <stdio.h>
#include <unistd.h>

#include "Template.hpp"
#include "TemplateFunctionParser.hpp"
#include "Log.hpp"
#include "TestLog.hpp"

using namespace obotcha;
using namespace gagira;

void test_basic_Int() {
    auto scopedValues = HashMap<String,TemplateScopedValue>::New();
    
    while(1) {
        String str1 = String::New("1+2+3");
        auto parser = TemplateFunctionParser::New(str1);
        auto item = parser->doParse();
        auto result = item->execute(scopedValues,nullptr);
        if(result->getIntValue() != 6) {
            TEST_FAIL("testFunctionItem basic_int case1 ,value is %d",
                        result->getIntValue());
        }
        
        if(result->getType() != st(TemplateScopedValue)::Type::Integer) {
            TEST_FAIL("testFunctionItem basic_int case1_2 ,type is %d",
                        result->getType());
        }
        break;
    }                              
    
    while(1) {
        String str1 = String::New("1*(2+3)");
        auto parser = TemplateFunctionParser::New(str1);
        auto item = parser->doParse();
        auto result = item->execute(scopedValues,nullptr);
        if(result->getIntValue() != 5) {
            TEST_FAIL("testFunctionItem basic_int case2 ,value is %d",
                        result->getIntValue());
        }
        
        if(result->getType() != st(TemplateScopedValue)::Type::Integer) {
            TEST_FAIL("testFunctionItem basic_int case2_2 ,type is %d",
                        result->getType());
        }
        break;
    } 
    
    while(1) {
        String str1 = String::New("(1+2)*(2+3)");
        auto parser = TemplateFunctionParser::New(str1);
        auto item = parser->doParse();
        auto result = item->execute(scopedValues,nullptr);
        if(result->getIntValue() != 15) {
            TEST_FAIL("testFunctionItem basic_int case3 ,value is %d",
                        result->getIntValue());
        }
        
        if(result->getType() != st(TemplateScopedValue)::Type::Integer) {
            TEST_FAIL("testFunctionItem basic_int case3_2 ,type is %d",
                        result->getType());
        }
        break;
    }
    
    while(1) {
        String str1 = String::New("2 + (1+2)*(2+3)");
        auto parser = TemplateFunctionParser::New(str1);
        auto item = parser->doParse();
        auto result = item->execute(scopedValues,nullptr);
        if(result->getIntValue() != 17) {
            TEST_FAIL("testFunctionItem basic_int case4 ,value is %d",
                        result->getIntValue());
        }
        
        if(result->getType() != st(TemplateScopedValue)::Type::Integer) {
            TEST_FAIL("testFunctionItem basic_int case4_2 ,type is %d",
                        result->getType());
        }
        break;
    } 
    
    while(1) {
        String str1 = String::New("2 * (1+2)*(2+3)");
        auto parser = TemplateFunctionParser::New(str1);
        auto item = parser->doParse();
        auto result = item->execute(scopedValues,nullptr);
        if(result->getIntValue() != 30) {
            TEST_FAIL("testFunctionItem basic_int case5 ,value is %d",
                        result->getIntValue());
        }
        
        if(result->getType() != st(TemplateScopedValue)::Type::Integer) {
            TEST_FAIL("testFunctionItem basic_int case5_2 ,type is %d",
                        result->getType());
        }
        break;
    }
    
    while(1) {
        String str1 = String::New("2 * (2*(2+3))*(2+3)");
        auto parser = TemplateFunctionParser::New(str1);
        auto item = parser->doParse();
        auto result = item->execute(scopedValues,nullptr);
        if(result->getIntValue() != 100) {
            TEST_FAIL("testFunctionItem basic_int case6 ,value is %d",
                        result->getIntValue());
        }
        
        if(result->getType() != st(TemplateScopedValue)::Type::Integer) {
            TEST_FAIL("testFunctionItem basic_int case6_2 ,type is %d",
                        result->getType());
        }
        break;
    } 
    
    while(1) {
        String str1 = String::New("2*(2+3) - 5");
        auto parser = TemplateFunctionParser::New(str1);
        auto item = parser->doParse();
        auto result = item->execute(scopedValues,nullptr);
        if(result->getIntValue() != 5) {
            TEST_FAIL("testFunctionItem basic_int case7 ,value is %d",
                        result->getIntValue());
        }
        
        if(result->getType() != st(TemplateScopedValue)::Type::Integer) {
            TEST_FAIL("testFunctionItem basic_int case7_2 ,type is %d",
                        result->getType());
        }
        break;
    }

    while(1) {
        String str1 = String::New("2*8 - 5");
        auto parser = TemplateFunctionParser::New(str1);
        auto item = parser->doParse();
        auto result = item->execute(scopedValues,nullptr);
        if(result->getIntValue() != 11) {
            TEST_FAIL("testFunctionItem basic_int case8 ,value is %d",
                        result->getIntValue());
        }
        
        if(result->getType() != st(TemplateScopedValue)::Type::Integer) {
            TEST_FAIL("testFunctionItem basic_int case8_2 ,type is %d",
                        result->getType());
        }
        break;
    } 
    
    while(1) {
        String str1 = String::New("(8 - 5) / 3");
        auto parser = TemplateFunctionParser::New(str1);
        auto item = parser->doParse();
        auto result = item->execute(scopedValues,nullptr);
        if(result->getIntValue() != 1) {
            TEST_FAIL("testFunctionItem basic_int case9 ,value is %d",
                        result->getIntValue());
        }
        
        if(result->getType() != st(TemplateScopedValue)::Type::Integer) {
            TEST_FAIL("testFunctionItem basic_int case9_2 ,type is %d",
                        result->getType());
        }
        break;
    } 
    
    while(1) {
        String str1 = String::New("1 << 4");
        auto parser = TemplateFunctionParser::New(str1);
        auto item = parser->doParse();
        auto result = item->execute(scopedValues,nullptr);
        if(result->getIntValue() != 16) {
            TEST_FAIL("testFunctionItem basic_int case10 ,value is %d",
                        result->getIntValue());
        }
        
        if(result->getType() != st(TemplateScopedValue)::Type::Integer) {
            TEST_FAIL("testFunctionItem basic_int case10_2 ,type is %d",
                        result->getType());
        }
        break;
    } 
    
    while(1) {
        String str1 = String::New("127 >> 3");
        auto parser = TemplateFunctionParser::New(str1);
        auto item = parser->doParse();
        auto result = item->execute(scopedValues,nullptr);
        if(result->getIntValue() != 15) {
            TEST_FAIL("testFunctionItem basic_int case11 ,value is %d",
                        result->getIntValue());
        }
        
        if(result->getType() != st(TemplateScopedValue)::Type::Integer) {
            TEST_FAIL("testFunctionItem basic_int case11_2 ,type is %d",
                        result->getType());
        }
        break;
    } 
    
    while(1) {
        String str1 = String::New("1 | 4");
        auto parser = TemplateFunctionParser::New(str1);
        auto item = parser->doParse();
        auto result = item->execute(scopedValues,nullptr);
        if(result->getIntValue() != 5) {
            TEST_FAIL("testFunctionItem basic_int case12 ,value is %d",
                        result->getIntValue());
        }
        
        if(result->getType() != st(TemplateScopedValue)::Type::Integer) {
            TEST_FAIL("testFunctionItem basic_int case12_2 ,type is %d",
                        result->getType());
        }
        break;
    } 
    
    while(1) {
        String str1 = String::New("5 & 2");
        auto parser = TemplateFunctionParser::New(str1);
        auto item = parser->doParse();
        auto result = item->execute(scopedValues,nullptr);
        if(result->getIntValue() != 0) {
            TEST_FAIL("testFunctionItem basic_int case13 ,value is %d",
                        result->getIntValue());
        }
        
        if(result->getType() != st(TemplateScopedValue)::Type::Integer) {
            TEST_FAIL("testFunctionItem basic_int case13_2 ,type is %d",
                        result->getType());
        }
        break;
    }
    
    while(1) {
        String str1 = String::New("6 & 4");
        auto parser = TemplateFunctionParser::New(str1);
        auto item = parser->doParse();
        auto result = item->execute(scopedValues,nullptr);
        if(result->getIntValue() != 4) {
            TEST_FAIL("testFunctionItem basic_int case14 ,value is %d",
                        result->getIntValue());
        }
        
        if(result->getType() != st(TemplateScopedValue)::Type::Integer) {
            TEST_FAIL("testFunctionItem basic_int case14_2 ,type is %d",
                        result->getType());
        }
        break;
    }
    
    while(1) {
        String str1 = String::New("5--");
        auto parser = TemplateFunctionParser::New(str1);
        auto item = parser->doParse();
        auto result = item->execute(scopedValues,nullptr);
        if(result->getIntValue() != 4) {
            TEST_FAIL("testFunctionItem basic_int case15 ,value is %d",
                        result->getIntValue());
        }
        
        if(result->getType() != st(TemplateScopedValue)::Type::Integer) {
            TEST_FAIL("testFunctionItem basic_int case15_2 ,type is %d",
                        result->getType());
        }
        break;
    }
    
    while(1) {
        String str1 = String::New("5++");
        auto parser = TemplateFunctionParser::New(str1);
        auto item = parser->doParse();
        auto result = item->execute(scopedValues,nullptr);
        if(result->getIntValue() != 6) {
            TEST_FAIL("testFunctionItem basic_int case16 ,value is %d",
                        result->getIntValue());
        }
        
        if(result->getType() != st(TemplateScopedValue)::Type::Integer) {
            TEST_FAIL("testFunctionItem basic_int case16_2 ,type is %d",
                        result->getType());
        }
        break;
    }
    
    TEST_OK("testFunctionItem basic_int case100");
}
