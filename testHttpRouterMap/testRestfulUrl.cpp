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
#include "HttpRouterMap.hpp"
#include "TestLog.hpp"
#include "GlobalCacheManager.hpp"
#include "ControllerParam.hpp"
#include "Process.hpp"

using namespace obotcha;
using namespace gagira;

bool isInvoke = false;

DECLARE_CLASS(TestRestFulListener) IMPLEMENTS(RouterListener) {
public:
    HttpResponseEntity onInvoke() {
        String username = GetStringParam(user);
        if(username == nullptr || !username->equals("wang")) {
            TEST_FAIL("TestHttpRouter,restFul case1");
        }
        String password = GetStringParam(password);
        if(password == nullptr || !password->equals("1234")) {
            TEST_FAIL("TestHttpRouter,restFul case2");
        }
        Integer abc = GetIntParam(abc);
        if(abc == nullptr || abc->toValue() != 1) {
            TEST_FAIL("TestHttpRouter,restFul case3");
        }
        Integer bbb = GetIntParam(bbb);
        if(bbb == nullptr || bbb->toValue() != 3) {
            TEST_FAIL("TestHttpRouter,restFul case4");
        }
        isInvoke = true;
        return nullptr;
    }
};

void testRestfulUrl() {
    HttpRouter router = createHttpRouter(
                            createString("/login/{user}/{password}"),
                            createTestRestFulListener());
                            
    HttpRouterMap map = createHttpRouterMap();
    map->addRouter(router);
    
    HashMap<String,String> params;
    HttpRouter r;
    
    FetchRet(r,params) = map->findRouter(createString("/login/wang/1234?abc=1&bbb=3"));
    ServletRequestCache cache = createServletRequestCache(nullptr,createControllerParam(params));
    st(GlobalCacheManager)::getInstance()->add(cache);
    r->invoke();
    if(!isInvoke) {
        TEST_FAIL("TestHttpRouter,restFul case100");
    } else {
        TEST_OK("TestHttpRouter,restFul case100");
    }
}