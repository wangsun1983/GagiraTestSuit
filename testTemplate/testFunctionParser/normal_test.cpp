#include "Template.hpp"
#include "TemplateFunctionParser.hpp"

using namespace obotcha;
using namespace gagira;

int normal_test() {
    // String arg1 = String::New("a+b*c");
    // auto parser = TemplateFunctionParser::New(arg1);
    // auto item = parser->doParse();
    
    // auto arg1 = String::New("a*(b+c)");
    // auto parser = TemplateFunctionParser::New(arg1);
    // auto item = parser->doParse();
    
    // auto arg1 = String::New("(a+d)*(b+c)");
    // auto parser = TemplateFunctionParser::New(arg1);
    // auto item = parser->doParse();
    
    // auto arg1 = String::New("a*(b+c)+d");
    // auto parser = TemplateFunctionParser::New(arg1);
    // auto item = parser->doParse();
    
    // auto arg1 = String::New("a*(b>>c)+d");
    // auto parser = TemplateFunctionParser::New(arg1);
    // auto item = parser->doParse();
    
    // auto arg1 = String::New("a*(-b + 1)+d");
    // auto parser = TemplateFunctionParser::New(arg1);
    // auto item = parser->doParse();
    
    auto arg1 = String::New("(-b(a,b,c)+a)*c-d");
    auto parser = TemplateFunctionParser::New(arg1);
    auto item = parser->doParse();
    return 0;
}
