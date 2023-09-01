#ifndef MEMORY_H
#define MEMORY_H

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

extern int pidA;/*
uint64_t AddrGNames,AddrGObject;*/

bool pvm(void *address, void *buffer, size_t size, bool iswrite);

bool vm_readv(void *address, void *buffer, size_t size);


pid_t find_pid(const char *process_name);

kaddr get_module_base(const char *module_name);

template<typename T>
T Read(void* address) {
    T data{};
    vm_readv(reinterpret_cast<void *>(address), reinterpret_cast<void *>(&data), sizeof(T));
    return data;
}

template<typename T>
T Read(kaddr address) {
    T data{};
    vm_readv(reinterpret_cast<void *>(address), reinterpret_cast<void *>(&data), sizeof(T));
    return data;
}
#endif //MEMORY_H
