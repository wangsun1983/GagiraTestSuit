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

DECLARE_CLASS(SimpleData10) {
public:
    int age;
    String name;
    String school;
    DECLARE_REFLECT_FIELD(SimpleData10,age,name,school)
};

DECLARE_CLASS(MyFunction) {
public:
    String transalte(ArrayList<String> list) {
        String start = createString("i get params is ");
        auto iterator = list->getIterator();
        while(iterator->hasValue()) {
            auto v = iterator->getValue();
            start = start->append(" ",v);
            iterator->next();
        }

        return start;
    }
};

void testSimpleFunctionItem() {

    SimpleData10 data10 = createSimpleData10();
    data10->age = 10;
    data10->name = createString("wang");
    data10->school = createString("haha middle school");
    String ret = nullptr;
    HtmlTemplate temp = nullptr;
    
    temp  = createHtmlTemplate();
    MyFunction instance = createMyFunction();
    InjectTemplateFunction(temp,"trans",instance,transalte);


    temp->import("{{trans .name .school abcd}}");
    ret = temp->execute(data10);
    printf("ret is %s \n",ret->toChars());

}

