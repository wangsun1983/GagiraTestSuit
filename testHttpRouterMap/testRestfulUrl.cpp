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
#include "HttpRouterMap2.hpp"

using namespace obotcha;
using namespace gagira;

DECLARE_CLASS(TestRestFulListener) IMPLEMENTS(RouterListener) {
public:
    HttpResponseEntity onInvoke() {
        printf("woqu,hit!!!!");
        return nullptr;
    }
};

void testRestfulUrl() {
    HttpRouter router = createHttpRouter(
                            createString("/login/{user}/{password}"),
                            createTestRestFulListener());
                            
    HttpRouterMap2 map = createHttpRouterMap2();
    map->addRouter(router);
    HashMap<String,String> params = createHashMap<String,String>();
    
    auto r = map->findRouter(createString("/login/wang/1234?abc=1&bbb=3"),params);
    if(r != nullptr) {
        printf("i found router \n");
    } else {
        printf("i cannot found router \n");
    }

    auto iterator = params->getIterator();
    while(iterator->hasValue()) {
        printf("key is %s,value is %s \n",iterator->getKey()->toChars(),iterator->getValue()->toChars());
        iterator->next();
    }
}

