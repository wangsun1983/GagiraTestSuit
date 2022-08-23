#include <stdio.h>
#include <unistd.h>

#include "Controller.hpp"
#include "Server.hpp"
#include "Inet4Address.hpp"
#include "ServletRequest.hpp"
#include "HttpResourceManager.hpp"
#include "Reflect.hpp"
#include "Utils.hpp"
#include "MqCenter.hpp"
#include "MqCenterBuilder.hpp"

using namespace obotcha;
using namespace gagira;



int main() {
    MqCenterBuilder builder = createMqCenterBuilder();
    builder->setUrl("tcp://127.0.0.1:1250");
    MqCenter center = builder->build();

    center->waitForExit();
    return 0;
}

