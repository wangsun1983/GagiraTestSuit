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

void testSimpleLenItem() {
    
    ArrayList<String> list = createArrayList<String>();
    list->add(createString("a"));
    list->add(createString("b"));
    list->add(createString("c"));
    HtmlTemplate temp = nullptr;
    String ret = nullptr;

    temp  = createHtmlTemplate();
    temp->import("{{len .}}");
    ret = temp->execute(list);

    printf("ret is %s \n",ret->toChars());    
}

