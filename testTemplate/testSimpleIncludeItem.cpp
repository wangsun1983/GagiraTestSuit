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

DECLARE_CLASS(SimpleData4) {
public:
    int age;
    String name;
    bool isStudent;
    DECLARE_REFLECT_FIELD(SimpleData4,age,name,isStudent)
};

void testSimpleIncludeItem() {
    
    SimpleData4 data1 = createSimpleData4();
    data1->age = 10;
    data1->name = String::New("wang");
    data1->isStudent = true;
    String ret = nullptr;
    HtmlTemplate temp = nullptr;
    
    temp  = createHtmlTemplate();
    temp->import("{{.name}},{{template includeTest.html}}");
    ret = temp->execute(data1);
    
    printf("ret is %s \n",ret->toChars());

    
}

