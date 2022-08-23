#ifndef __OBOTCHA_TEST_NET_PORT_HPP__
#define __OBOTCHA_TEST_NET_PORT_HPP__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "File.hpp"
#include "LineReader.hpp"
#include "FileOutputStream.hpp"

#define DEFAULT_PORT 2001
#define DEFAULT_PATH "/tmp/obotcha_test_suit_port.txt"

using namespace obotcha;


int getEnvPort() {
    File file = createFile(DEFAULT_PATH);
    if(file->exists()) {
        LineReader r = createLineReader(file);
        String value = r->readLine();
        return value->toBasicInt();
    }

    return DEFAULT_PORT;
}

void setEnvPort(int p) {
    if(p > 60000) {
        p = DEFAULT_PORT;
    }
    File file = createFile(DEFAULT_PATH);
    if(!file->exists()) {
        file->createNewFile();
    }
    FileOutputStream stream = createFileOutputStream(file);
    stream->open();
    stream->write(createString(p)->toByteArray());
    stream->flush();
    stream->close();
}

void resetEnvPort() {
    setEnvPort(DEFAULT_PORT);
}
#endif