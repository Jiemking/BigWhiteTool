//
// Created by Administrator on 2023-08-17.
//

#ifndef BIGWHITETOOL_ANDROID_READ_H
#define BIGWHITETOOL_ANDROID_READ_H
#include "BigWhiteRead.h"
#include "biu.h"
// 获取指针(64位游戏)
int readmode = 1;

unsigned long XY_GetAddr(unsigned long addr){
    if (readmode==0)    return BigWhite_GetPtr64(addr);
    if (readmode==1)    return getaddr(addr);

}

bool XY_Read(unsigned long address, void *buffer, size_t size){
    if (readmode==0)    return BigWhite_vm_readv(address, buffer,size);
    if (readmode==1)    return driverRead(BigWhite_pid,address, buffer,size);
}

float XY_GetFloat(unsigned long addr){
    if (readmode==0)    return BigWhite_GetFloat(addr);
    if (readmode==1)    return getfloat(addr);
}

float XY_GetDword(unsigned long addr){
    if (readmode==0)    return BigWhite_GetDword(addr);
    if (readmode==1)    return getint(addr);
}




#endif //BIGWHITETOOL_ANDROID_READ_H
