#include <sys/stat.h>
#include "dumper.h"
#include "generic.h"
#include "file.h"
#include "engine.h"

// 初始化Dumper
void Dumper::Init(uint64_t Base,uint64_t GName, uint64_t GObject)
{
    // 设置游戏基地址、名称池和对象数组
    GameBase = Base;
    NamePoolData = (FNamePool*)GName;
    ObjObjects = (TUObjectArray*)GObject;
    // 设置全局名称和对象地址
    AddrGNames = GName;
    AddrGObject = GObject;
/*
    cout << NamePoolData->GetName(325177) << endl;*/
    // 检查名称池是否正确
    if(NamePoolData->GetName(0) != "None")
        throw "GetName有误\nGetName(0)不为None";
    // 检查对象数组是否为空
    if (ObjObjects->GetNumChunks() == 0)
        throw "GObject块数为0";
    // 设置本地化
    setlocale(LC_ALL, "");
    // 创建目录
    mkdir("/storage/emulated/0/A_BigWhiteTool/SDK",2770);

}

void Dumper::Dump()
{
/*    UE_UObject* Object = ObjObjects->GetObjectFormId(0x135c);

    if (Object){
        cout << "Object\n" << endl;
        if (Object->IsA<UE_UEnum>())
        {
            cout << "IsA:UE_UEnum\n" << endl;
            Object->Cast<UE_UEnum*>()->Generate();
        }
    }*/

    // 打开文件用于写入
    FILE* ObjectsName = fopen("/storage/emulated/0/A_BigWhiteTool/SDK/Object.cpp", "w+");

    // 遍历所有对象
    for (size_t i = 0; i < ObjObjects->GetNumElements(); i++)
    {
        // 获取对象
        UE_UObject* Object = ObjObjects->GetObjectFormId(i);
        // 检查对象是否为空
        if (!Object)
            continue;
        // 生成枚举
        if (Object->IsA<UE_UEnum>())
        {
            Object->Cast<UE_UEnum*>()->Generate();
        }
        // 生成类
        else if (Object->IsA<UE_UClass>())
        {
            Object->Cast<UE_UClass*>()->Generate();
        }
        // 生成函数
        else if (Object->IsA<UE_UFunction>())
        {
            Object->Cast<UE_UFunction*>()->Generate();
        }
        // 生成结构
        else if (Object->IsA<UE_UScriptStruct>())
        {
            Object->Cast<UE_UScriptStruct*>()->Generate();
        }
        // 写入文件
        fprintf(ObjectsName, "{%.6p}\t[%p]\t%s\n",i, Object, Object->GetFullName().c_str());
    }
    // 输出完成信息
    cout<< "OK!\n" <<endl;
}
