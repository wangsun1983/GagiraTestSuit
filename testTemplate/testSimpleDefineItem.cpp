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

DECLARE_CLASS(SimpleData5) {
public:
    int age;
    String name;
    bool isStudent;
    DECLARE_REFLECT_FIELD(SimpleData5,age,name,isStudent)
};

void testSimpleDefineItem() {
    ArrayList<SimpleData5> list = createArrayList<SimpleData5>();
    SimpleData5 data1 = createSimpleData5();
    data1->age = 10;
    data1->name = String::New("wang");
    data1->isStudent = true;

    HtmlTemplate temp  = createHtmlTemplate();

    temp->importFile("./definetest.html");
    String ret = temp->execute("case1",data1);

    
    printf("ret is %s \n",ret->toChars());
    
    

    
}

