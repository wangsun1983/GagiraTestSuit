#ifndef __OBOTCHA_TEST_LOG_HPP__
#define __OBOTCHA_TEST_LOG_HPP__
#include <string.h>

#define TEST_OK(...) \
    {\
        char buff[1024];\
        memset(buff,0,1024);\
        sprintf(buff,__VA_ARGS__);\
        printf("%-100s :\033[32m%s \033[37m\n",buff,"[OK]");\
    }

#define TEST_FAIL(...) \
    {\
        char buff[1024];\
        memset(buff,0,1024);\
        sprintf(buff,__VA_ARGS__);\
        printf("%-100s :\033[31m%s \033[37m\n",buff,"[FAIL]");\
    }

#define TEST_TODO(...) \
    {\
        char buff[1024];\
        memset(buff,0,1024);\
        sprintf(buff,__VA_ARGS__);\
        printf("%-100s :\033[31m%s \033[37m\n",buff,"[TODO]");\
    }
#endif