#include "Template.hpp"
#include "TemplatePrintParser.hpp"
#include "TemplatePrintItem.hpp"
#include "TemplateLetParser.hpp"
#include "TemplateLetItem.hpp"
#include "TestLog.hpp"

using namespace obotcha;
using namespace gagira;

void basic_test() {    
    // while(1) {
    //     String case1_str = String::New("int v  = 100"); //let command
    //     String case1_str2 = String::New("(\"v is #1\",v)"); //print command
        
    //     auto scopedValues = HashMap<String,TemplateScopedValue>::New();
    //     auto parser = TemplateLetParser::New(case1_str);
    //     auto item = parser->doParse();
        
    //     String result = item->execute(scopedValues,
    //                                   nullptr,
    //                                   0);
        
    //     printf("test trace1 \n");                              
    //     auto parser2 = TemplatePrintParser::New(case1_str2);
    //     printf("test trace2 \n");   
    //     item = parser2->doParse();
    //     printf("test trace3 \n");   
    //     result = item->execute(scopedValues,
    //                                   nullptr,
    //                                   0);
    //     printf("test trace4 \n");                                 
    //     //printf("[testLetParser normal test] result is %s \n",result->toChars());
    //     break;
    // }
    
    while(1) {
        String case1_str = String::New("bool v  = true"); //let command
        
        auto scopedValues = HashMap<String,TemplateScopedValue>::New();
        auto parser = TemplateLetParser::New(case1_str);
        auto item = parser->doParse();
        
        auto result = item->execute(scopedValues,
                                      nullptr);
                                      
        case1_str = String::New("v");
        result = item->execute(scopedValues,
                                      nullptr);

        if(result->getBoolValue() != true) {
            TEST_FAIL("testLetParser basic_test case1");
        }
        
        if(result->getType() != st(TemplateScopedValue)::Bool) {
            TEST_FAIL("testLetParser basic_test case2,type is %d",result->getType());
        }
        break;
    }
    
    TEST_OK("testLetParser basic_test case100");
    
}
