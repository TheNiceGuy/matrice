#ifndef INCLUDE_CONFIG_H
#define INCLUDE_CONFIG_H

#define TRUE  1
#define FALSE 0

#define INPUT_BUFFER_SIZE 50

#ifndef __LINUX__
    #define clear(); system("cls");
#else
    #define clear(); system("clear");
#endif

#endif
