#ifndef ENGINE_H
#define ENGINE_H

#include "base.h"

class Tarray
{
public:
    uint8_t* Data;      // 数据指针
    uint32_t Count;     // 元素数量
    uint32_t Max;       // 最大容量
};

class UE_UClass;
class UE_FField;

class UE_UObject
{
public:
    UE_UClass* GetClass();          // 获取对象的类
    UE_UObject* GetOuter();        // 获取对象的外层对象
    string GetName();              // 获取对象名称
    string GetCppName();           // 获取对象的C++名称
    string GetFullName();          // 获取对象的完整名称

    static UE_UObject* StaticClass(); // 获取静态类对象

    template <typename Base> Base Cast() const { return Base(this); } // 类型转换模板函数

    template <typename T> bool IsA(); // 判断对象是否是某种类型
};

class UE_AActor : public UE_UObject {
public:
    static UE_UObject* StaticClass(); // 获取静态类对象
};

class UE_UField : public UE_UObject {
public:
};

class UE_UStruct : public UE_UField {
public:
    UE_UStruct* GetSuper();           // 获取结构体的父结构体
    UE_FField* GetChildProperties();  // 获取结构体的子属性列表
    uint32_t GetPropertiesSize();     // 获取结构体属性的大小
    int GetSize();                    // 获取结构体的大小
};

class UE_UFunction : public UE_UStruct {
public:
    uint64_t GetFunc();               // 获取函数指针
    string GetFunctionFlags();       // 获取函数标志

    void Generate();                  // 生成函数

    static UE_UObject* StaticClass(); // 获取静态类对象
};

class UE_UScriptStruct : public UE_UStruct {
public:
    void Generate();                  // 生成脚本结构体

    static UE_UObject* StaticClass(); // 获取静态类对象
};

class UE_UClass : public UE_UStruct {
public:
    void Generate();                  // 生成类

    static UE_UObject* StaticClass(); // 获取静态类对象
};

class UE_UEnum : public UE_UField {
public:
    void Generate();                  // 生成枚举

    Tarray GetNames();                // 获取枚举名称列表

    static UE_UObject* StaticClass(); // 获取静态类对象
};

class UE_FFieldClass
{
public:
    string GetName();                // 获取字段类的名称
};

class UE_FField
{
public:
    UE_FField* GetNext();            // 获取下一个字段
    string GetName();                // 获取字段名称
    string GetType();                // 获取字段类型
    int GetSize();                   // 获取字段大小
    int GetOffset();                 // 获取字段偏移量
    UE_UObject* GetUEObject();       // 获取字段的UE对象

    template <typename Base> Base Cast() const { return Base(this); } // 类型转换模板函数
};

class UE_FProperty : public UE_FField {
public:
};

class UE_FStructProperty : public UE_FProperty {
public:
    string GetTypeStr();             // 获取结构体属性的类型字符串
};

class UE_FMapProperty : public UE_FProperty {
public:
    string GetTypeStr();             // 获取映射属性的类型字符串
};

class UE_FObjectPropertyBase : public UE_FProperty {
public:
    string GetTypeStr();             // 获取对象属性的类型字符串
};

class UE_FArrayProperty : public UE_FProperty {
public:
    string GetTypeStr();             // 获取数组属性的类型字符串
};

class UE_FByteProperty : public UE_FProperty {
public:
    string GetTypeStr();             // 获取字节属性的类型字符串
};

class UE_FBoolProperty : public UE_FProperty {
public:
    string GetTypeStr();             // 获取布尔属性的类型字符串
};

class UE_FEnumProperty : public UE_FProperty {
public:
    string GetTypeStr();             // 获取枚举属性的类型字符串
};

class UE_FClassProperty : public UE_FObjectPropertyBase {
public:
    string GetTypeStr();             // 获取类属性的类型字符串
};

class UE_FSetProperty : public UE_FProperty {
public:
    string GetTypeStr();             // 获取集合属性的类型字符串
};

class UE_FInterfaceProperty : public UE_FProperty {
public:
    string GetTypeStr();             // 获取接口属性的类型字符串
};

class UE_FFieldPathProperty : public UE_FProperty {
public:
    string GetTypeStr();             // 获取字段路径属性的类型字符串
};

// 模板函数 IsA，用于判断对象是否是特定类型T的实例
template <typename T> bool UE_UObject::IsA()
{
    UE_UObject* cmp = T::StaticClass();
    // 如果静态类对象不存在，返回 false
    if (!cmp)
        return false;
    // 从当前对象的类开始，逐级获取父类，直到找到类型T的类或者到达最顶层类
    for (UE_UClass* super = GetClass(); super; super = super->GetSuper()->Cast<UE_UClass*>())
    {
        // 如果找到类型T的类，返回 true
        if (super == cmp)
            return true;
    }

    // 如果未找到类型T的类，返回 false
    return false;
}


#endif // !ENGINE_H
