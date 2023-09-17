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
    uint64_t address;
    int offset;
    string type;
    string name;
    string trans;

    uint64_t P;
    float F;
    int D;

};

//游戏偏移结构
struct Offsets
{
public:
    uintptr_t GNames = 0;
    uintptr_t Uworld = 0;
    uint64_t Gobject = 0;
    uintptr_t Matrix = 0;
    int Matrix1 = 0;
    int Matrix2 = 0;
    int Ulevel = 0x30;
    int Arrayaddr = 0x98;
    int ArrayaddrCount = 0xA0;

    int RootComponent  = 0x0;
    int XYZ_X = 0x0;

    int isBot = 0;

    //自身
    int GameInstance = 0x180;
    int LocalPlayer = 0x38;
    int PlayerController = 0x30;
    int AcknowledgedPawn = 0x0;


    int Mesh = 0x0;
    int Human = 0x0;
    int Bone = 0x0;
    int BoneArray[17];

    string packagename = "";

    void SetBoneArray(const int* values, int length) {
        for (int i = 0; i < length; i++) {
            BoneArray[i] = values[i];
        }
    }

};
Offsets offsets;


//游戏地址结构
struct Addr
{
public:
    uint64_t base;
    uint64_t libbase;
    uint64_t Address = 0;
    uint64_t GNames = 0;
    uint64_t Uworld = 0;
    uint64_t Gobject = 0;
    uint64_t Matrix = 0;
    uint64_t oneself = 0;
    uint64_t Ulevel = 0x30;
    uint64_t Arrayaddr = 0x98;


    uint64_t RootComponent  = 0x0;
    int XYZ_X = 0x0;

    int isBot = 0;

    //自身
    uint64_t GameInstance = 0x0;
    uint64_t LocalPlayer = 0x0;
    uint64_t PlayerController = 0x0;
    uint64_t AcknowledgedPawn = 0x0;


    int Mesh = 0x0;
    int Human = 0x0;
    int Bone = 0x0;
    int BoneArray[17];

    string packagename = "";

    void SetBoneArray(const int* values, int length) {
        for (int i = 0; i < length; i++) {
            BoneArray[i] = values[i];
        }
    }

};
Addr addr;




struct Vector3A
{
    float X;
    float Y;
    float Z;

    Vector3A()
    {
        this->X = 0;
        this->Y = 0;
        this->Z = 0;
    }

    Vector3A(float x, float y, float z)
    {
        this->X = x;
        this->Y = y;
        this->Z = z;
    }

};

#endif //BIGWHITETOOL_ENUM_H
