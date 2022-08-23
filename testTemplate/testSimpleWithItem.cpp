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

DECLARE_CLASS(SimpleData8) {
public:
    int age;
    String name;
    DECLARE_REFLECT_FIELD(SimpleData8,age,name)
};

void testSimpleWithItem() {
    ArrayList<SimpleData8> list = createArrayList<SimpleData8>();
    SimpleData8 data1 = createSimpleData8();
    data1->age = 10;
    data1->name = createString("wang");

    SimpleData8 data2 = createSimpleData8();
    data2->age = 20;
    data2->name = createString("sun");

    String ret = nullptr;

    list->add(data1);
    list->add(data2);

    HtmlTemplate temp = nullptr;
    
    //temp  = createHtmlTemplate();
    //temp->import("{{range $index, .}} \r\n<tr> \r\n<td>{{$index}}</td> \r\n<td>{{.name}}</td>\r\n<td>{{.age}}</td>\r\n</tr>\r\n{{end}}");
    
    temp  = createHtmlTemplate();
    temp->import("{{with index . 1}} {{.name}} is a good man {{end}}");
    ret = temp->execute(list);

    
    printf("ret is %s \n",ret->toChars());
    
    

    
}

