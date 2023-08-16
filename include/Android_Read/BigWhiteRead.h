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
#define UNI_SUR_HIGH_START (UTF32)0xD800
#define UNI_SUR_HIGH_END (UTF32)0xDBFF
#define UNI_SUR_LOW_START (UTF32)0xDC00
#define UNI_SUR_LOW_END (UTF32)0xDFFF
// Some fundamental constants
#define UNI_REPLACEMENT_CHAR (UTF32)0x0000FFFD
#define UNI_MAX_BMP (UTF32)0x0000FFFF
#define UNI_MAX_UTF16 (UTF32)0x0010FFFF
#define UNI_MAX_UTF32 (UTF32)0x7FFFFFFF
#define UNI_MAX_LEGAL_UTF32 (UTF32)0x0010FFFF
int BigWhite_pid;
// syscall内存读写
#if defined(__arm__)
int process_vm_readv_syscall = 376;
int process_vm_writev_syscall = 377;
#elif defined(__aarch64__)
int process_vm_readv_syscall = 270;
int process_vm_writev_syscall = 271;
#elif defined(__i386__)
int process_vm_readv_syscall = 347;
int process_vm_writev_syscall = 348;
#else
int process_vm_readv_syscall = 310;
int process_vm_writev_syscall = 311;
#endif

ssize_t BigWhite_process_v(pid_t __pid, const struct iovec *__local_iov, unsigned long __local_iov_count,
                  const struct iovec *__remote_iov, unsigned long __remote_iov_count,
                  unsigned long __flags, bool iswrite)
{
    return syscall((iswrite ? process_vm_writev_syscall : process_vm_readv_syscall), __pid,
                   __local_iov, __local_iov_count, __remote_iov, __remote_iov_count, __flags);
}
int BigWhite_getProcessID(const char *packageName)
{
    int id = -1;
    DIR *dir;
    FILE *fp;
    char filename[64];
    char cmdline[64];
    struct dirent *entry;
    dir = opendir("/proc");
    while ((entry = readdir(dir)) != NULL)
    {
        id = atoi(entry->d_name);
        if (id != 0)
        {
            sprintf(filename, "/proc/%d/cmdline", id);
            fp = fopen(filename, "r");
            if (fp)
            {
                fgets(cmdline, sizeof(cmdline), fp);
                fclose(fp);
                if (strcmp(packageName, cmdline) == 0)
                {
                    return id;
                }
            }
        }
    }
    closedir(dir);
    return -1;
}

bool BigWhite_mem_addr_virtophy(unsigned long vaddr)
{
    int fh = 1;
    int pageSize = getpagesize();

    // 虚拟地址相对于0X0经过的页面数
    unsigned long v_pageIndex = vaddr / pageSize;
    // 在pagemap文件中的偏移量
    unsigned long pfn_item_offset = v_pageIndex * sizeof(uint64_t);

    // 页内偏移量
    unsigned long page_offset = vaddr % pageSize;
    uint64_t item = 0;
    char filename[32];
    snprintf(filename, sizeof(filename), "/proc/%d/pagemap", BigWhite_getProcessID);
    int fd = open(filename, O_RDONLY);
    if (fd < 0)
    {
        // printf("open failed\n");
        return false;
    }

    if (lseek(fd, pfn_item_offset, SEEK_SET) < 0)
    {
        printf("lseek failed\n");
        return false;
    }

    if (read(fd, &item, sizeof(uint64_t)) != sizeof(uint64_t))
    {
        printf("read failed\n");
        return false;
    }

    if (0 == (item & (((uint64_t) 1) << 63)))
    {
        fh = 0;
        printf("page is not present");
        return false;
    }
    close(fd);
    return true;
}

// 进程读写内存
bool BigWhite_pvm(void *address, void *buffer, size_t size, bool iswrite)
{
    struct iovec local[1];
    struct iovec remote[1];
    local[0].iov_base = buffer;
    local[0].iov_len = size;
    remote[0].iov_base = address;
    remote[0].iov_len = size;
    if (BigWhite_pid < 0)
    {
        return false;
    }
    ssize_t bytes = BigWhite_process_v(BigWhite_pid, local, 1, remote, 1, 0, iswrite);
    return bytes == size;
}

// 读取内存
bool BigWhite_vm_readv(unsigned long address, void *buffer, size_t size)
{
    return BigWhite_pvm(reinterpret_cast < void *>(address), buffer, size, false);
}
// 写入内存
bool BigWhite_vm_writev(unsigned long address, void *buffer, size_t size)
{
    return BigWhite_pvm(reinterpret_cast < void *>(address), buffer, size, true);
}
// 获取F类内存
float BigWhite_GetFloat(unsigned long addr)
{
    float var = 0;
    BigWhite_vm_readv(addr, &var, 4);
    return (var);
}

// 获取D类内存
int BigWhite_GetDword(unsigned long addr)
{
    int var = 0;
    BigWhite_vm_readv(addr, &var, 4);
    return (var);
}

// 获取指针(32位游戏)
unsigned int BigWhite_GetPtr32(unsigned int addr)
{
    if (BigWhite_mem_addr_virtophy(addr) || addr == 0x0000000000 || addr == 0 || addr == 0x000){
        return 0;
    }
    unsigned int var = 0;
    BigWhite_vm_readv(addr, &var, 4);
    return (var);
}
// 获取指针(64位游戏)
unsigned long BigWhite_GetPtr64(unsigned long addr)
{
    if (BigWhite_mem_addr_virtophy(addr) || addr == 0x0000000000 || addr == 0 || addr == 0x000){
        return 0;
    }
    unsigned long var = 0;
    BigWhite_vm_readv(addr, &var, 8);
    return (var);
}

// 获取指针(64位游戏) 无过缺页
unsigned long BigWhite_getPtr641(unsigned long addr)
{
    unsigned long var = 0;
    BigWhite_vm_readv(addr, &var, 8);
    return (var);
}
// 写入D类内存
void BigWhite_WriteDword(unsigned long addr, int data)
{
    BigWhite_vm_writev(addr, &data, 4);
}

void BigWhite_WriteFloat(unsigned long addr, float data)
{
    BigWhite_vm_writev(addr, &data, 4);
}

// 获取进程
int BigWhite_GetPID(const char *packageName)
{
    int id = -1;
    DIR *dir;
    FILE *fp;
    char filename[64];
    char cmdline[64];
    struct dirent *entry;
    dir = opendir("/proc");
    while ((entry = readdir(dir)) != NULL)
    {
        id = atoi(entry->d_name);
        if (id != 0)
        {
            sprintf(filename, "/proc/%d/cmdline", id);
            fp = fopen(filename, "r");
            if (fp)
            {
                fgets(cmdline, sizeof(cmdline), fp);
                fclose(fp);
                if (strcmp(packageName, cmdline) == 0)
                {
                    return id;
                }
            }
        }
    }
    closedir(dir);
    return -1;
}

// 获取基址
unsigned long BigWhite_GetModuleBase(int pid, const char *module_name)
{
    FILE *fp;
    unsigned long addr = 0;
    char *pch;
    char filename[64];
    char line[1024];
    snprintf(filename, sizeof(filename), "/proc/%d/maps", pid);
    fp = fopen(filename, "r");
    if (fp != NULL)
    {
        while (fgets(line, sizeof(line), fp))
        {
            if (strstr(line, module_name))
            {
                pch = strtok(line, "-");
                addr = strtoul(pch, NULL, 16);
                if (addr == 0x8000)
                    addr = 0;
                break;
            }
        }
        fclose(fp);
    }
    return addr;
}


// 读取字符信息
void BigWhite_GetUTF8(UTF8 * buf, unsigned long namepy)
{
    UTF16 buf16[16] = { 0 };
    BigWhite_vm_readv(namepy, buf16, 28);
    UTF16 *pTempUTF16 = buf16;
    UTF8 *pTempUTF8 = buf;
    UTF8 *pUTF8End = pTempUTF8 + 32;
    while (pTempUTF16 < pTempUTF16 + 28)
    {
        if (*pTempUTF16 <= 0x007F && pTempUTF8 + 1 < pUTF8End)
        {
            *pTempUTF8++ = (UTF8) * pTempUTF16;
        }
        else if (*pTempUTF16 >= 0x0080 && *pTempUTF16 <= 0x07FF && pTempUTF8 + 2 < pUTF8End)
        {
            *pTempUTF8++ = (*pTempUTF16 >> 6) | 0xC0;
            *pTempUTF8++ = (*pTempUTF16 & 0x3F) | 0x80;
        }
        else if (*pTempUTF16 >= 0x0800 && *pTempUTF16 <= 0xFFFF && pTempUTF8 + 3 < pUTF8End)
        {
            *pTempUTF8++ = (*pTempUTF16 >> 12) | 0xE0;
            *pTempUTF8++ = ((*pTempUTF16 >> 6) & 0x3F) | 0x80;
            *pTempUTF8++ = (*pTempUTF16 & 0x3F) | 0x80;
        }
        else
        {
            break;
        }
        pTempUTF16++;
    }
}

#endif
