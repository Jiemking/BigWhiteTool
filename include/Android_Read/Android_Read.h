//
// Created by Administrator on 2023-08-17.
//

#ifndef BIGWHITETOOL_ANDROID_READ_H
#define BIGWHITETOOL_ANDROID_READ_H
#include "BigWhiteRead.h"
#include "biu.h"

union FloatInt {
    int i;
    float f;
};

extern int readmode;
//获取模块地址
uint64_t GetLibBase(int pid);
// 获取指针(64位游戏)
uint64_t GetAddr(uint64_t addr);
//读地址
bool ReadAddr(uintptr_t address, void *buffer, size_t size);
//读四字节整形
int GetDowrd(uintptr_t addr);
//读浮点型
float GetFloat(uintptr_t addr);
//整形转浮点
float intToFloat(int intValue);

// 模板函数用于读取内存（重载）
template<typename T>
T TRead(kaddr address) {
    T data{};
    if (readmode==1){
        BigWhite_pvm(reinterpret_cast<void *>(address), reinterpret_cast<void *>(&data), sizeof(T), false);
    }else if (readmode==2){
        ReadAddr(address, reinterpret_cast<void *>(&data),sizeof(T));
    }
    return data;
}

#endif //BIGWHITETOOL_ANDROID_READ_H
