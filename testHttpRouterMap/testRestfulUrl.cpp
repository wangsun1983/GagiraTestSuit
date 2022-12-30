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

bool isHit = false;
bool isHit1 = false;

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
        isHit = true;
        return nullptr;
    }
};

DECLARE_CLASS(TestRestFulListener1) IMPLEMENTS(RouterListener) {
public:
    HttpResponseEntity onInvoke() {
        String username = GetStringParam(user);
        if(username == nullptr || !username->equals("wang2")) {
            TEST_FAIL("TestHttpRouter,restFul case5");
        }
        String password = GetStringParam(password);
        if(password == nullptr || !password->equals("12342")) {
            TEST_FAIL("TestHttpRouter,restFul case6");
        }
        
        isHit1 = true;
        return nullptr;
    }
};


void testRestfulUrl() {
    HttpRouter router = createHttpRouter(
                            createString("/login/{user}/{password}"),
                            createTestRestFulListener());

    HttpRouter router1 = createHttpRouter(
                            createString("/login2/{user}/{password}"),
                            createTestRestFulListener1());
                            
    HttpRouterMap map = createHttpRouterMap();
    map->addRouter(router);
    map->addRouter(router1);
    
    HashMap<String,String> params;
    HttpRouter r;
    FetchRet(r,params) = map->findRouter(createString("/login/wang/1234?abc=1&bbb=3"));
    ServletRequestCache cache = createServletRequestCache(nullptr,createControllerParam(params));
    st(GlobalCacheManager)::getInstance()->add(cache);
    r->invoke();
    if(!isHit) {
        TEST_FAIL("TestHttpRouter,restFul case100");
    }

    HashMap<String,String> params1;
    HttpRouter r1;
    FetchRet(r1,params1) = map->findRouter(createString("/login2/wang2/12342"));
    ServletRequestCache cache1 = createServletRequestCache(nullptr,createControllerParam(params1));
    st(GlobalCacheManager)::getInstance()->add(cache1);
    r1->invoke();
    if(!isHit1) {
        TEST_FAIL("TestHttpRouter,restFul case101");
    }

    TEST_OK("TestHttpRouter,restFul case200");
}