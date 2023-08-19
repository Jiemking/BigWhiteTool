//
// Created by Administrator on 2023-08-17.
//

#ifndef BIGWHITETOOL_ANDROID_READ_H
#define BIGWHITETOOL_ANDROID_READ_H
#include "BigWhiteRead.h"
#include "biu.h"
// 获取指针(64位游戏)
int readmode = 1;

uintptr_t XY_GetModuleBase(int pid){
    if (readmode==0)    return BigWhite_GetModuleBase(pid,"libUE4.so");
    if (readmode==1)    return getBase(pid,"libUE4.so");

}
uintptr_t XY_GetAddr(unsigned long addr){

    if (readmode==0)    return BigWhite_GetPtr64(addr);
    if (readmode==1)    return getaddr(addr);

}

bool XY_Read(uintptr_t address, void *buffer, size_t size){
    if (readmode==0)    return BigWhite_vm_readv(address, buffer,size);
    if (readmode==1)    return driverRead(BigWhite_pid,address, buffer,size);
}

float XY_GetFloat(uintptr_t addr){
    if (readmode==0)    return BigWhite_GetFloat(addr);
    if (readmode==1)    return getfloat(addr);
}

int XY_GetDword(uintptr_t addr){
    if (readmode==0)    return BigWhite_GetDword(addr);
    if (readmode==1)    return getint(addr);
}







#endif //BIGWHITETOOL_ANDROID_READ_H
