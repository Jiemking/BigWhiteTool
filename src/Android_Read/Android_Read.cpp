//
// Created by Administrator on 2023-09-16.
//
#include "Android_Read/Android_Read.h"
int readmode;

uint64_t GetLibBase(int pid){
    if (readmode==1){
        return BigWhite_GetModuleBase(pid,"libUE4.so");
    }
    else if (readmode==2){
        return getBase(pid,"libUE4.so");
    }
    return 0;
}

uint64_t GetAddr(uint64_t addr){
    if(readmode==1){
        return BigWhite_GetPtr64(addr);
    } else if (readmode==2){
        return getaddr(addr);
    }
    return 0;
}

bool ReadAddr(uintptr_t addr, void *buffer, size_t size){
    if (readmode==1){
        return BigWhite_vm_readv(addr,buffer,size);
    }else if (readmode==2){
        return driverRead(BigWhite_pid,addr, buffer,size);
    }
    return false;
}

int GetDowrd(uintptr_t addr){
    if (readmode==1){
        return BigWhite_GetDword(addr);
    }else if (readmode==2){
        return getint(addr);
    }
    return 0;
}

float GetFloat(uintptr_t addr){
    if (readmode==1){
        return BigWhite_GetFloat(addr);
    }else if (readmode==2){
        return getfloat(addr);
    }
    return 0;
}
