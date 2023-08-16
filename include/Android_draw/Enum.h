//
// Created by Administrator on 2023-08-16.
//
#include "string"
#ifndef BIGWHITETOOL_ENUM_H
#define BIGWHITETOOL_ENUM_H

struct FName
{
    int ComparisonIndex;
    int Number;
};
struct AddrOffsets
{
    uint64_t Addr;
    int Offsets;
};
//进程选择器结构
struct ProcessInfo {
    std::string pid;
    std::string name;
    bool isSelected; // New variable to store selection state
};

struct StructureList
{
    long int address;
    int offset;
    string type;
    string name;

    long int P;
    float F;
    int D;

};
//游戏偏移结构
struct Offsets
{
public:
    uintptr_t libbase;

    uintptr_t GNames = 0;
    uintptr_t Uworld = 0;
    uintptr_t Matrix = 0;
    int Ulevel = 0x30;
    int Arrayaddr = 0x98;
    int ArrayaddrCount = 0xA0;

    int RootComponent  = 0x0;
    int XYZ_X = 0x0;

    int isBot = 0;

    //自身
    int GameInstance = 0x0;
    int ULocalPlayer = 0x0;
    int PlayerController = 0x0;
    int AcknowledgedPawn = 0x0;


    int Mesh = 0x0;
    int Human = 0x0;
    int Bone = 0x0;
    int BoneArray[17];

    string packagename = "";

    bool isUE423 = true;//true新版本算法，false旧版本算法
    void SetBoneArray(const int* values, int length) {
        for (int i = 0; i < length; i++) {
            BoneArray[i] = values[i];
        }
    }

};
Offsets offsets;
//游戏偏移结构
struct Addr
{
public:
    uintptr_t libbase;
    long int Address = 0;
    long int GNames = 0;
    long int Uworld = 0;
    long int Matrix = 0;
    int Ulevel = 0x30;
    int Arrayaddr = 0x98;
    int ArrayaddrCount = 0xA0;

    int RootComponent  = 0x0;
    int XYZ_X = 0x0;

    int isBot = 0;

    //自身
    int GameInstance = 0x0;
    int ULocalPlayer = 0x0;
    int PlayerController = 0x0;
    int AcknowledgedPawn = 0x0;


    int Mesh = 0x0;
    int Human = 0x0;
    int Bone = 0x0;
    int BoneArray[17];

    string packagename = "";

    bool isUE423 = true;//true新版本算法，false旧版本算法
    void SetBoneArray(const int* values, int length) {
        for (int i = 0; i < length; i++) {
            BoneArray[i] = values[i];
        }
    }

};
Addr addr;


#endif //BIGWHITETOOL_ENUM_H
