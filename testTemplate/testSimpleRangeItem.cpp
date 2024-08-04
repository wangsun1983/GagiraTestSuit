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

DECLARE_CLASS(SimpleData3) {
public:
    int age;
    String name;
    DECLARE_REFLECT_FIELD(SimpleData3,age,name)
};

void testSimpleRangeItem() {
    ArrayList<SimpleData3> list = createArrayList<SimpleData3>();
    SimpleData3 data1 = createSimpleData3();
    data1->age = 10;
    data1->name = String::New("wang");

    SimpleData3 data2 = createSimpleData3();
    data2->age = 20;
    data2->name = String::New("sun");

    String ret = nullptr;

    list->add(data1);
    list->add(data2);

    HtmlTemplate temp = nullptr;
    
    //temp  = createHtmlTemplate();
    //temp->import("{{range $index, .}} \r\n<tr> \r\n<td>{{$index}}</td> \r\n<td>{{.name}}</td>\r\n<td>{{.age}}</td>\r\n</tr>\r\n{{end}}");
    
    temp  = createHtmlTemplate();
    temp->import("{{range $bcc, .}} \r\n<tr> \r\n<td>{{$bcc}}</td> \r\n<td>{{.name}}</td>\r\n<td>{{.age}}</td>\r\n</tr>\r\n{{else}} no value{{end}}");
    ret = temp->execute(list);

    
    printf("ret is %s \n",ret->toChars());
    
    

    
}

