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

DECLARE_CLASS(SimpleData2) {
public:
    int age;
    String name;
    bool isStudent;
    DECLARE_REFLECT_FIELD(SimpleData2,age,name,isStudent)
};

void testSimpleConditionItem() {
    HtmlTemplate temp = createHtmlTemplate();
    String ret = nullptr;

    SimpleData2 data1 = createSimpleData2();
    data1->age = 10;
    data1->name = String::New("wang");
    data1->isStudent = false;

/*
    temp->import("{{if gt .age 18}}\r\n<div>abc</div>\r\n{{else}}\r\n<div>efg</div>\r\n{{end}}");
    ret = temp->execute(data1);


    temp = createHtmlTemplate();
    temp->import("this is {{if gt .age 18}} abc {{else if gt .age 9}} def {{else}} gh {{end}}");
    ret = temp->execute(data1);



    temp = createHtmlTemplate();
    temp->import("this is {{if .isStudent}} i am a student,my name is {{.name}} {{else}} i am not a student {{end}}");
    ret = temp->execute(data1);

    temp = createHtmlTemplate();
    temp->import("{{if .isStudent}} abc {{end}},this is {{.name}}");

    temp = createHtmlTemplate();
    temp->import("this is {{ if .isStudent}} i am a student,my name is {{.name}} {{else}} i am not a student {{end}},{{.name}} is a good man");
    */
    temp = createHtmlTemplate();
    temp->import("this is {{ if .isStudent}} i am a student,my name is {{.name}} {{else}} i am not a student {{end}},{{.name}} is a good man");
    
    ret = temp->execute(data1);
    printf("ret is %s \n",ret->toChars());
}

