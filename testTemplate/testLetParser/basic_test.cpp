#include "Template.hpp"
#include "TemplatePrintParser.hpp"
#include "TemplatePrintItem.hpp"
#include "TemplateLetParser.hpp"
#include "TemplateLetItem.hpp"

using namespace obotcha;
using namespace gagira;

void basic_test() {    
    while(1) {
        String case1_str = String::New("int v  = 100"); //let command
        String case1_str2 = String::New("(\"v is #1\",v)"); //print command
        
        auto scopedValues = HashMap<String,TemplateScopedValue>::New();
        auto parser = TemplateLetParser::New(case1_str);
        auto item = parser->doParse();
        
        String result = item->execute(scopedValues,
                                      nullptr,
                                      0);
        
        printf("test trace1 \n");                              
        auto parser2 = TemplatePrintParser::New(case1_str2);
        printf("test trace2 \n");   
        item = parser2->doParse();
        printf("test trace3 \n");   
        result = item->execute(scopedValues,
                                      nullptr,
                                      0);
        printf("test trace4 \n");                                 
        //printf("[testLetParser normal test] result is %s \n",result->toChars());
        break;
    }
    
}
