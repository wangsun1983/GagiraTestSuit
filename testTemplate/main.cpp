#include <stdio.h>
#include <unistd.h>

#include "Controller.hpp"
#include "Server.hpp"
#include "Inet4Address.hpp"
#include "ServletRequest.hpp"
#include "HttpResourceManager.hpp"
#include "Reflect.hpp"
#include "Utils.hpp"

using namespace obotcha;
using namespace gagira;

extern void testSimpleObjectItem();
extern void testSimpleConditionItem();
extern void testSimpleRangeItem();
extern void testSimpleIncludeItem();
extern void testSimpleDefineItem();
extern void testSimpleLenItem();
extern void testSimpleWithItem();
extern void testSimpleFunctionItem();

int main() {
    testSimpleFunctionItem();
    //testSimpleWithItem();
    //testSimpleDefineItem();
    //testSimpleIncludeItem();
    //testSimpleRangeItem();
    //testSimpleConditionItem();
    //testSimpleObjectItem();
    //testSimpleLenItem();
    return 0;
}

