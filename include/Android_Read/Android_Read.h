//
// Created by Administrator on 2023-08-17.
//

#ifndef BIGWHITETOOL_ANDROID_READ_H
#define BIGWHITETOOL_ANDROID_READ_H
#include "BigWhiteRead.h"
// 获取指针(64位游戏)
unsigned long XY_GetAddr(unsigned long addr){
    return BigWhite_GetPtr64(addr);
}

bool XY_Read(unsigned long address, void *buffer, size_t size){
    return BigWhite_vm_readv(address, buffer,size);
}

float XY_GetFloat(unsigned long addr){
    return BigWhite_GetFloat(addr);
}

float XY_GetDword(unsigned long addr){
    return BigWhite_GetDword(addr);
}




#endif //BIGWHITETOOL_ANDROID_READ_H
