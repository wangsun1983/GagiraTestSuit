#ifndef __OBOTCHA_TEST_SAMPLE_FILE_HPP__
#define __OBOTCHA_TEST_SAMPLE_FILE_HPP__
#include <string.h>
#include "File.hpp"
#include "System.hpp"
#include "StringBuffer.hpp"
#include "FileOutputStream.hpp"

using namespace obotcha;

void createSampleFile(File file,long size) {
    if(file->exists()) {
        return;
    }
    file->createNewFile();
    
    FileOutputStream stream = createFileOutputStream(file);
    stream->open();
    while(size > 0) {
        String f = createString(st(System)::currentTimeMillis());
        if(f->size() > size) {
            f = f->subString(0,size);
        }

        stream->write(f->toByteArray());
        size -= f->size();
    }
}

#endif