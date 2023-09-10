#ifndef GENERIC_H
#define GENERIC_H

#include "base.h"
#include "engine.h"

// FNameEntryHeader 结构定义
struct FNameEntryHeader
{
public:
    // 宽字符标志
    uint16_t bIsWide : 1;
    // 哈希位数
    static constexpr uint32_t ProbeHashBits = 5;
    // 小写哈希
    uint16_t LowercaseProbeHash : ProbeHashBits;
    // 长度
    uint16_t Len : 10;
};

// FNameEntry 结构定义
struct FNameEntry
{
public:
    // 头部信息
    FNameEntryHeader Header;
};

// FNamePool 类定义
class FNamePool
{
public:
    // 获取名字的函数
    string GetName(uint32_t NameId);
};

// TUObjectArray 类定义
class TUObjectArray
{
public:
    // 获取块数量的函数
    uint32_t GetNumChunks();

    // 获取元素数量的函数
    uint32_t GetNumElements();

    // 根据ID获取对象的函数
    UE_UObject* GetObjectFormId(size_t Id);

    // 根据全名查找对象的函数
    UE_UObject* FindObject(string FullName);
};

// 全局变量声明
extern uint64_t GameBase;
extern uint64_t AddrGNames;
extern uint64_t AddrGObject;
extern FNamePool* NamePoolData;
extern TUObjectArray* ObjObjects;

#endif // !GENERIC_H
