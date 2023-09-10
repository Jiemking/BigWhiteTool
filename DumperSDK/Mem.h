
#ifndef MEMORY_H
#define MEMORY_H

// 包含头文件
#include <dirent.h>
#include <cstdio>
#include <cstdlib>
#include <cstddef>
#include <cmath>
#include <ctime>
#include <algorithm>
#include <string>
#include <list>
#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <unistd.h>
#include <sys/uio.h>
#include <sys/syscall.h>

using namespace std;

typedef uintptr_t kaddr;

// 全局变量
extern int debug;
extern int pidA;

// 函数原型
bool pvm(void *address, void *buffer, size_t size, bool iswrite);
bool vm_readv(void *address, void *buffer, size_t size);
pid_t find_pid(const char *process_name);
kaddr get_module_base(const char *module_name);

// 模板函数用于读取内存
template<typename T>
T Read(void* address) {
    T data{};
    vm_readv(reinterpret_cast<void *>(address), reinterpret_cast<void *>(&data), sizeof(T));
    return data;
}

// 模板函数用于读取内存（重载）
template<typename T>
T Read(kaddr address) {
    T data{};
    vm_readv(reinterpret_cast<void *>(address), reinterpret_cast<void *>(&data), sizeof(T));
    return data;
}
#endif //MEMORY_H
