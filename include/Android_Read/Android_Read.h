//
// Created by Administrator on 2023-08-17.
//

#ifndef BIGWHITETOOL_ANDROID_READ_H
#define BIGWHITETOOL_ANDROID_READ_H
#include "BigWhiteRead.h"
#include "biu.h"
#include "Android_draw/ItemData.h"
// 获取指针(64位游戏)
int readmode = 0;
int XY_pid;

uintptr_t XY_GetModuleBase(int pid){
    if (readmode==1)    return BigWhite_GetModuleBase(pid,"libUE4.so");
    if (readmode==2)    return getBase(pid,"libUE4.so");

}
uintptr_t XY_GetAddr(unsigned long addr){

    if (readmode==1)    return BigWhite_GetPtr64(addr);
    if (readmode==2)    return getaddr(addr);

}

bool XY_Read(uintptr_t address, void *buffer, size_t size){
    if (readmode==1)    return BigWhite_vm_readv(address, buffer,size);
    if (readmode==2)    return driverRead(BigWhite_pid,address, buffer,size);
}

float XY_GetFloat(uintptr_t addr){
    if (readmode==1)    return BigWhite_GetFloat(addr);
    if (readmode==2)    return getfloat(addr);
}

int XY_GetDword(uintptr_t addr){
    if (readmode==1)    return BigWhite_GetDword(addr);
    if (readmode==2)    return getint(addr);
}







#endif //BIGWHITETOOL_ANDROID_READ_H
