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

bool isInvoke1 = false;
bool isInvoke2 = false;
bool isInvoke3 = false;
bool isInvoke4 = false;
bool isInvoke5 = false;

DECLARE_CLASS(TestRestFulListener2_1) IMPLEMENTS(RouterListener) {
public:
    HttpResponseEntity onInvoke() {
        String username = GetStringParam(user);
        if(username == nullptr || !username->equals("wang")) {
            TEST_FAIL("TestHttpRouter2,restFul case1");
        }
        String password = GetStringParam(password);
        if(password == nullptr || !password->equals("1234")) {
            TEST_FAIL("TestHttpRouter2,restFul case2");
        }
        Integer abc = GetIntParam(abc);
        if(abc == nullptr || abc->toValue() != 1) {
            TEST_FAIL("TestHttpRouter2,restFul case3");
        }
        Integer bbb = GetIntParam(bbb);
        if(bbb == nullptr || bbb->toValue() != 3) {
            TEST_FAIL("TestHttpRouter2,restFul case4");
        }
        isInvoke1 = true;
        return nullptr;
    }
};

DECLARE_CLASS(TestRestFulListener2_2) IMPLEMENTS(RouterListener) {
public:
    HttpResponseEntity onInvoke() {
        String username = GetStringParam(user);
        if(username == nullptr || !username->equals("wang2")) {
            TEST_FAIL("TestHttpRouter2,restFul case5");
        }
        String password = GetStringParam(password);
        if(password == nullptr || !password->equals("12342")) {
            TEST_FAIL("TestHttpRouter2,restFul case6");
        }
        Integer abc = GetIntParam(abc);
        if(abc == nullptr || abc->toValue() != 1) {
            TEST_FAIL("TestHttpRouter2,restFul case7");
        }
        Integer bbb = GetIntParam(bbb);
        if(bbb == nullptr || bbb->toValue() != 3) {
            TEST_FAIL("TestHttpRouter2,restFul case8");
        }

        Integer id = GetIntParam(id);
        if(id == nullptr || id->toValue() != 55) {
            TEST_FAIL("TestHttpRouter2,restFul case9");
        }

        isInvoke2 = true;
        return nullptr;
    }
};

DECLARE_CLASS(TestRestFulListener2_3) IMPLEMENTS(RouterListener) {
public:
    HttpResponseEntity onInvoke() {
        String username = GetStringParam(user2);
        if(username == nullptr || !username->equals("wang3")) {
            TEST_FAIL("TestHttpRouter2,restFul case10");
        }
        String password = GetStringParam(password2);
        if(password == nullptr || !password->equals("12343")) {
            TEST_FAIL("TestHttpRouter2,restFul case11");
        }
    
        Integer id = GetIntParam(id2);
        if(id == nullptr || id->toValue() != 33) {
            TEST_FAIL("TestHttpRouter2,restFul case12");
        }

        isInvoke3 = true;
        return nullptr;
    }
};

DECLARE_CLASS(TestRestFulListener2_4) IMPLEMENTS(RouterListener) {
public:
    HttpResponseEntity onInvoke() {
        String username = GetStringParam(user2);
        if(username != nullptr) {
            TEST_FAIL("TestHttpRouter2,restFul case13");
        }

        isInvoke4 = true;
        return nullptr;
    }
};

DECLARE_CLASS(TestRestFulListener2_5) IMPLEMENTS(RouterListener) {
public:
    HttpResponseEntity onInvoke() {
        Integer abc = GetIntParam(abc);
        if(abc == nullptr || abc->toValue() != 1) {
            TEST_FAIL("TestHttpRouter2,restFul case14");
        }

        Integer bbb = GetIntParam(bbb);
        if(bbb == nullptr || bbb->toValue() != 3) {
            TEST_FAIL("TestHttpRouter2,restFul case15");
        }

        isInvoke5 = true;
        return nullptr;
    }
};


void testRestfulUrl2() {
    HttpRouter router = createHttpRouter(
                            String::New("/login/{user}/{password}"),
                            createTestRestFulListener2_1());

    HttpRouter router2 = createHttpRouter(
                            String::New("/login/{user}/{password}/{id}"),
                            createTestRestFulListener2_2());

    HttpRouter router3 = createHttpRouter(
                            String::New("/login2/{user2}/{password2}/{id2}"),
                            createTestRestFulListener2_3());
    
    HttpRouter router4 = createHttpRouter(
                            String::New("/login"),
                            createTestRestFulListener2_4());

    HttpRouter router5 = createHttpRouter(
                            String::New("/login2"),
                            createTestRestFulListener2_5());

    HttpRouterMap map = createHttpRouterMap();
    map->addRouter(router);
    map->addRouter(router2);
    map->addRouter(router3);
    map->addRouter(router4);
    map->addRouter(router5);
    
    HashMap<String,String> params;
    HttpRouter r;
    FetchRet(r,params) = map->findRouter(String::New("/login/wang/1234?abc=1&bbb=3"));
    ServletRequestCache cache1 = createServletRequestCache(nullptr,createControllerParam(params));
    st(GlobalCacheManager)::getInstance()->add(cache1);
    r->invoke();
    if(!isInvoke1) {
        TEST_FAIL("TestHttpRouter2,restFul case100");
    }

    HashMap<String,String> params2;
    HttpRouter r2;
    FetchRet(r2,params2) = map->findRouter(String::New("/login/wang2/12342/55?abc=1&bbb=3"));
    ServletRequestCache cache2 = createServletRequestCache(nullptr,createControllerParam(params2));
    st(GlobalCacheManager)::getInstance()->add(cache2);
    r2->invoke();
    if(!isInvoke2) {
        TEST_FAIL("TestHttpRouter2,restFul case101");
    }

    HashMap<String,String> params3;
    HttpRouter r3;
    FetchRet(r3,params3) = map->findRouter(String::New("/login2/wang3/12343/33?abc=1&bbb=3"));
    ServletRequestCache cache3 = createServletRequestCache(nullptr,createControllerParam(params3));
    st(GlobalCacheManager)::getInstance()->add(cache3);
    r3->invoke();
    if(!isInvoke3) {
        TEST_FAIL("TestHttpRouter2,restFul case102");
    }

    HashMap<String,String> params4;
    HttpRouter r4;
    FetchRet(r4,params4) = map->findRouter(String::New("/login"));
    ServletRequestCache cache4 = createServletRequestCache(nullptr,createControllerParam(params4));
    st(GlobalCacheManager)::getInstance()->add(cache4);
    r4->invoke();
    if(!isInvoke4) {
        TEST_FAIL("TestHttpRouter2,restFul case103");
    }

    HashMap<String,String> params5;
    HttpRouter r5;
    FetchRet(r5,params5) = map->findRouter(String::New("/login2?abc=1&bbb=3"));
    ServletRequestCache cache5 = createServletRequestCache(nullptr,createControllerParam(params5));
    st(GlobalCacheManager)::getInstance()->add(cache5);
    r5->invoke();
    if(!isInvoke5) {
        TEST_FAIL("TestHttpRouter2,restFul case104");
    }

    TEST_OK("TestHttpRouter2,restFul case200");
} 