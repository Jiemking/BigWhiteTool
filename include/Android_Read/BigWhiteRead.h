#ifndef NATIVESURFACE_MEMREAD_H
#define NATIVESURFACE_MEMREAD_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/syscall.h>
#include <sys/mman.h>
#include <sys/uio.h>
#include <malloc.h>
#include <math.h>
#include <thread>
#include <iostream>
#include <sys/stat.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include<stdint.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <iostream>
#include <locale>
#include <string>
#include <codecvt>
#include <dlfcn.h>
// 定义圆周率
#define PI 3.141592653589793238
typedef unsigned int ADDRESS;
typedef char PACKAGENAME;
typedef unsigned short UTF16;
typedef char UTF8;
typedef uintptr_t kaddr;

extern int BigWhite_pid;

// syscall内存读写
extern int BigWhite_process_vm_readv_syscall;
extern int BigWhite_process_vm_writev_syscall;

ssize_t BigWhite_process_v(pid_t __pid, const struct iovec *__local_iov, unsigned long __local_iov_count,
                  const struct iovec *__remote_iov, unsigned long __remote_iov_count,
                  unsigned long __flags, bool iswrite);

int BigWhite_getProcessID(const char *packageName);

bool BigWhite_mem_addr_virtophy(unsigned long vaddr);

// 进程读写内存
bool BigWhite_pvm(void *address, void *buffer, size_t size, bool iswrite);

// 读取内存
bool BigWhite_vm_readv(unsigned long address, void *buffer, size_t size);
// 写入内存
bool BigWhite_vm_writev(unsigned long address, void *buffer, size_t size);
// 获取F类内存
float BigWhite_GetFloat(unsigned long addr);

// 获取D类内存
int BigWhite_GetDword(unsigned long addr);

// 获取指针(32位游戏)
unsigned int BigWhite_GetPtr32(unsigned int addr);
// 获取指针(64位游戏)
unsigned long BigWhite_GetPtr64(unsigned long addr);

// 获取指针(64位游戏) 无过缺页
unsigned long BigWhite_getPtr641(unsigned long addr);
// 写入D类内存
void BigWhite_WriteDword(unsigned long addr, int data);

void BigWhite_WriteFloat(unsigned long addr, float data);

// 获取进程
int BigWhite_GetPID(const char *packageName);

// 获取模块基址
unsigned long BigWhite_GetModuleBase(int pid, const char *module_name);

//获取基址
unsigned long BigWhite_GetProcessBaseAddress(int pid);


// 读取字符信息
void BigWhite_GetUTF8(UTF8 * buf, unsigned long namepy);


template<typename T>
T XY_TRead(void* address) {
    T data{};
    BigWhite_pvm(reinterpret_cast<void *>(address), reinterpret_cast<void *>(&data), sizeof(T), false);
    return data;
}

// 模板函数用于读取内存（重载）
template<typename T>
T XY_TRead(kaddr address) {
    T data{};
    BigWhite_pvm(reinterpret_cast<void *>(address), reinterpret_cast<void *>(&data), sizeof(T), false);
    return data;
}

#endif
