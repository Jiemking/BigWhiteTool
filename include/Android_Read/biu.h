//
// Created by Administrator on 2023/8/12.
//

#ifndef JNITEST_CALCUL_H
#define JNITEST_CALCUL_H

#include <unistd.h>
#include <sys/uio.h>
#include <stdio.h>
#include <string>


/*
 * 获取模块地址
 */
uintptr_t getBase(int pid,const char *basename);
/*
 * 检查驱动是否正常
 */
bool checkDriver();
/*
 * 调用驱动读取数据,自由自由读取
 */
int driverRead(int pid,uintptr_t addr,void *buff,size_t size);
/*
 * 获取包名的pid
 */
int getpid(const char *PackageName);//需要root权限

int setpid(int pid);

uintptr_t getaddr(uintptr_t addr);
int getint(uintptr_t addr);
float getfloat(uintptr_t addr);
float getchar(uintptr_t addr);

/*
除了getpid需要root权限外，其他函数都不需要root权限
即你的app无需root权限(但至少要保证编译的elf文件在你的app资源文件夹内)
也可以调用Biu二进制进行内存管理操作,或者app使用jniso都可以直接使用
 */
#endif //JNITEST_CALCUL_H