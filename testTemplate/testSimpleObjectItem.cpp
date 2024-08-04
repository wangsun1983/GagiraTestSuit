#include <stdio.h>
#include <unistd.h>

#include "Controller.hpp"
#include "Server.hpp"
#include "Inet4Address.hpp"
#include "ServletRequest.hpp"
#include "HttpResourceManager.hpp"
#include "Reflect.hpp"
#include "Utils.hpp"
#include "HtmlTemplate.hpp"

using namespace obotcha;
using namespace gagira;

DECLARE_CLASS(SimpleData1) {
public:
    int age;
    String name;
    DECLARE_REFLECT_FIELD(SimpleData1,age,name)
};

void testSimpleObjectItem() {

    SimpleData1 data1 = createSimpleData1();
    data1->age = 10;
    data1->name = String::New("wang");
    String ret = nullptr;
    HtmlTemplate temp = nullptr;
    
    temp  = createHtmlTemplate();
    temp->import("{{.name}}");
    ret = temp->execute(data1);
    
    
    temp = createHtmlTemplate();
    temp->import("<p> {{.name}} </p> \r\n <br>{{.age}}</br>");
    ret = temp->execute(data1);
    
    
    
    temp = createHtmlTemplate();
    temp->import("<p> {{.}} </p> \r\n <br>{{.}}</br>");
    ret = temp->execute(createInteger(5));
    
    

    temp = createHtmlTemplate();
    temp->import("<p> {{.}} </p> \r\n <br>{{.}}</br>");
    ret = temp->execute(String::New("abc"));
    
    
    temp = createHtmlTemplate();
    temp->import("<p> {{.}} </p> \r\n <br>{{.}}</br>");
    ret = temp->execute(createBoolean(true));
    printf("ret is %s \n",ret->toChars());
    
    

    
}

