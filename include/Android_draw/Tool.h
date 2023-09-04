//
// Created by Administrator on 2023-08-16.
//
#include <Enum.h>
#include <fstream>
#include<Android_Read/Android_Read.h>
#include "../../DumperSDK/dumper.h"
#include "../../DumperSDK/base.h"

#ifndef BIGWHITETOOL_TOOL_H
#define BIGWHITETOOL_TOOL_H
int tencent = 0;//这个全局变量过滤进程使用
uint64_t GetClass(uint64_t Address) {
    uint64_t UobjectClass = XY_GetAddr((Address + 0x10));
    if (UobjectClass != NULL) {
        return UobjectClass;
    }
    return 0;
}

FName ReadProcessFname(uint64_t address) {
    FName buffer = {0};
    XY_Read(address, &buffer, sizeof(FName));
    return buffer;
}

FName GetFName(uint64_t Address) {
    FName Name = ReadProcessFname((Address + 0x18));
    if (Name.ComparisonIndex)
        return Name;
    return FName{};

}

std::string GetName_Old(int i) //旧版本算法
{
    uintptr_t G_Names = XY_GetAddr(addr.GNames);
    int Id = (int)(i / (int)0x4000);
    int Idtemp = (int)(i % (int)0x4000);
    auto NamePtr = XY_GetAddr((G_Names + Id * 8));
    auto Name = XY_GetAddr((NamePtr + 8 * Idtemp));
    char name[0x100] = { 0 };
    if (XY_Read((Name + 0xC), name, 0x100)) //0xC需要手动推算，默认是0x10
    {
        return name;
    }
    return {};
}

std::string GetName_New(uint32_t index) //新版本算法
{
    unsigned int Block = index >> 16;
    unsigned short int Offset = index & 65535;

    uintptr_t FNamePool = addr.GNames;

    uintptr_t NamePoolChunk = XY_GetAddr(FNamePool+0x40 + (Block * 0x8));

    uintptr_t FNameEntry = NamePoolChunk + (0x2 * Offset);

    short int FNameEntryHeader = XY_GetDword(FNameEntry);
    uintptr_t StrPtr = FNameEntry + 0x2;

    int StrLength = FNameEntryHeader >> 6;
    if (StrLength > 0 && StrLength < 250) {
        string name(StrLength, '\0');
        XY_Read(StrPtr, name.data(), StrLength * sizeof(char));
        name.shrink_to_fit();
        return name;
    } else {
        return "None";
    }
}

std::string GetName(uint64_t Address) {
    int FnameComparisonIndex = GetFName(Address).ComparisonIndex; //这里获取的是基类的Gname编号
    std::string GetName;
    if (addr.isUE423){
        GetName = GetName_New(FnameComparisonIndex); //新算法获取Name
    }else{
        GetName = GetName_Old(FnameComparisonIndex); //旧版本算法获取Name
    }

    return GetName;

}

std::string GetClassName(uint64_t Address) {
    uint64_t UobjectClass = GetClass(Address); //获取基类地址
    std::string name = GetName(UobjectClass);
    return name;
}

uint64_t GetOuter(uint64_t Address) {
    uint64_t outer = XY_GetAddr(Address + 0x20);
    if (outer != NULL)
        return outer;
    return uint64_t();
};

std::string GetOuterName(uint64_t Address) {
    auto klass = GetClass(Address);
    if (klass != NULL) {
        std::string temp;
        temp = GetName(Address);  //自己的类名
        return temp;
    }

    return {"(null)"};
}


std::vector<ProcessInfo> GetTencentProcesses() {
    std::vector<ProcessInfo> processes;

    for (const auto& entry : std::filesystem::directory_iterator("/proc")) {
        if (entry.is_directory()) {
            std::string pid = entry.path().filename().string();
            std::string cmdlineFilePath = entry.path() / "cmdline";

            std::ifstream cmdlineFile(cmdlineFilePath, std::ios::binary);
            if (cmdlineFile.is_open()) {
                std::string processName;
                char ch;
                while (cmdlineFile.get(ch) && ch != '\0') {
                    processName += ch;
                }

                if (!processName.empty()) {
                    if ((tencent==0 && processName.find("com.") != std::string::npos)) {
                        ProcessInfo processInfo;
                        processInfo.pid = pid;
                        processInfo.name = processName;
                        processInfo.isSelected = false;
                        processes.push_back(processInfo);
                    }
                    else if ((tencent==1 && (processName.find("tencent.") != std::string::npos || processName.find("uamo") != std::string::npos))) {
                        ProcessInfo processInfo;
                        processInfo.pid = pid;
                        processInfo.name = processName;
                        processInfo.isSelected = false;
                        processes.push_back(processInfo);
                    }
                }

                cmdlineFile.close();
            }
        }
    }

    return processes;
}
static vector<StructureList> foreachAddress(uint64_t Address) {
    std::vector<StructureList> structureList; // 使用std::vector存储输出内容
    for (size_t i = 0; i < 0xF00; i+=4) {

        uint64_t Tmp = XY_GetAddr(Address + i);
        string KlassName = GetClassName(Tmp);
        string outerName = GetOuterName(Tmp);
        string trans = ItemData::UamoGetString(KlassName);
        //string KlassName = "KlassName";
        //string outerName = "outerName";

        StructureList data;
        data.address = (Address + i);
        data.type = KlassName;
        data.name = outerName;
        data.trans = trans;
        data.offset=i;


        data.P = XY_GetAddr(data.address);
        data.F= XY_GetFloat(data.address);
        data.D= XY_GetDword(data.address);
//        if (outerName.find("None") && KlassName.find("null") && getPtr64(data.address)==0)

        structureList.push_back(data);

    }
    return structureList;
}
int GetEventCount() {
    DIR *dir = opendir("/dev/input/");
    dirent *ptr = nullptr;
    int count = 0;
    while ((ptr = readdir(dir)) != nullptr) {
        if (strstr(ptr->d_name, "event"))
            count++;
    }
    return count ? count : -1;
}

void ResetOffsets(){
    offsets.GNames = 0;
    offsets.Uworld = 0;
    offsets.Matrix = 0;
    offsets.Ulevel = 0x0;
    offsets.Arrayaddr = 0x0;
    offsets.ArrayaddrCount = 0x0;

    offsets.RootComponent = 0x0;
    offsets.XYZ_X = 0x0;

    offsets.isBot = 0;

    offsets.GameInstance = 0x0;
    offsets.LocalPlayer = 0x0;
    offsets.PlayerController = 0x0;
    offsets.AcknowledgedPawn = 0x0;

    offsets.Mesh = 0x0;
    offsets.Human = 0x0;
    offsets.Bone = 0x0;
    for (int i = 0; i < 17; i++) {
        offsets.BoneArray[i] = 0;
    }
    addr.isUE423 = true;
}

namespace UEinit{
    AddrOffsets GetGname(){
        AddrOffsets addrOffsets;
        addrOffsets.Addr=0;
        addrOffsets.Offsets=0;
        for (int i = 20000000;; i++) {

            uint64_t TMP;
            if (addr.isUE423){
                TMP = addr.libbase + (0x8*i) + 0x40;
                if (TMP != NULL){
                    uint64_t TMPGnames = XY_GetAddr(TMP);
                    char name[0x100];
                    XY_Read(TMPGnames+0x8, name, 0xc);
                    std::string aa = name;
                    if (aa.find("ByteProperty") != std::string::npos){
                        addrOffsets.Addr=addr.libbase+(0x8*i);
                        addrOffsets.Offsets=(0x8*i);
                        offsets.GNames=(0x8*i);//设置全局Gname偏移
                        addr.GNames = addr.libbase + offsets.GNames;//设置全局Gname地址
                        break;
                    }
                }
            } else{
                TMP = addr.libbase + (0x8*i);
/*                printf("%lx",(0x8*i));
                cout << "\n"<<endl;*/
                if (TMP < 0x1000000000) continue;
                uint64_t TMP1 = XY_GetAddr(TMP);
                if (TMP1 < 0x1000000000) continue;
                uint64_t TMP2 = XY_GetAddr(TMP1);
                if (TMP2 < 0x1000000000) continue;
                uint64_t TMP3 = XY_GetAddr(TMP2);
                if (TMP3 < 0x1000000000) continue;
                uint64_t TMPGnames = TMP3;
                char name[0x100];
                XY_Read(TMPGnames+0x24, name, 0xc);
                std::string aa = name;
                if (aa.find("ByteProperty") != std::string::npos){
                    addrOffsets.Addr=addr.libbase+(0x8*i);
                    addrOffsets.Offsets=(0x8*i);
                    offsets.GNames=(0x8*i);//设置全局Gname偏移
                    addr.GNames = addr.libbase + offsets.GNames;//设置全局Gname地址
                    break;
                }
            }
        }

        return addrOffsets;
    }
    AddrOffsets GetUworld(){
        AddrOffsets addrOffsets;
        addrOffsets.Addr=0;
        addrOffsets.Offsets=0;
        int i=0;
        while (true){
            uint64_t TMPUworld = XY_GetAddr(addr.libbase + offsets.GNames + (0x8*i));
            if (TMPUworld != NULL){
                if (GetClassName(TMPUworld)== "World"){
                    //(GetName(TMPUworld)=="TutorialRange_Main") 这个可以不需要
                    //if (GetClassName(XY_GetAddr(TMPUworld+offsets.Ulevel))=="Level"){}
                    addrOffsets.Addr=TMPUworld;
                    addrOffsets.Offsets=offsets.GNames + (0x8*i);
                    offsets.Uworld=offsets.GNames + (0x8*i);
                    cout << offsets.Uworld<<endl;

                }
            }
            i++;
        }
        return addrOffsets;
    }

    AddrOffsets GetGobject(){
        AddrOffsets addrOffsets;
        addrOffsets.Addr=0;
        addrOffsets.Offsets=0;
        for (int i = 0;; i++) {
            uint64_t TMP;
            TMP = addr.libbase + offsets.GNames + (0x8*i)+0x10;
            if (TMP < 0x1000000000) continue;
            uint64_t TMP1 = XY_GetAddr(TMP);
            if (TMP1 < 0x1000000000) continue;
            uint64_t TMP2 = XY_GetAddr(TMP1);
            if (TMP2 < 0x1000000000) continue;
            uint64_t TMP3 = XY_GetAddr(TMP2);
            if (TMP3 < 0x1000000000) continue;
            uint64_t TMP4 = XY_GetAddr(TMP2+0x18);
            if (TMP4 < 0x1000000000) continue;
            string TMP3Class = GetClassName(TMP3);
            string TMP3Outer = GetOuterName(TMP3);
            string TMP4Class = GetClassName(TMP4);
            string TMP4Outer = GetOuterName(TMP4);
            if (TMP3Class.find("Package") != std::string::npos && TMP3Outer.find("UObject") != std::string::npos ){
                if (TMP4Class.find("Class") != std::string::npos && TMP4Outer.find("Object") != std::string::npos ){
                    addrOffsets.Addr=TMP-0x10;
                    addrOffsets.Offsets=offsets.GNames + (0x8*i);
                    break;
                }
            }
        }
        return addrOffsets;
    }
    AddrOffsets GetEngine(){
        AddrOffsets addrOffsets;
        addrOffsets.Addr=0;
        addrOffsets.Offsets=0;
        int i=0;

        //cout << GetClassName(XY_GetAddr(addr.libbase + 0xADD4868)) <<endl;
        while (1){
            uint64_t TMPEngine = XY_GetAddr(addr.libbase + offsets.GNames + (0x8*i));
            if (TMPEngine != NULL){
                if (GetClassName(TMPEngine)== "GameEngine"){
                    addrOffsets.Addr=TMPEngine;
                    addrOffsets.Offsets=offsets.GNames + (0x8*i);
                    break;
                }
            }
            i++;
        }
        return addrOffsets;
    }
    AddrOffsets GetMatrix(){
        AddrOffsets addrOffsets;
        addrOffsets.Addr=0;
        addrOffsets.Offsets=0;
        int i=0;

        //cout << GetClassName(XY_GetAddr(XY_GetAddr(addr.libbase + 0xDFA6EF8)+0x20)) <<endl;
        while (1){
            uint64_t TMPMatrix = XY_GetAddr(addr.libbase + offsets.GNames + (0x8*i));
            if (TMPMatrix != NULL){
                if (GetClassName(XY_GetAddr(TMPMatrix+0x20))== "Canvas"){
                    addrOffsets.Addr=TMPMatrix;
                    addrOffsets.Offsets=offsets.GNames + (0x8*i);
                    offsets.Matrix=offsets.GNames + (0x8*i);
                    uint64_t Tmp270 =   XY_GetAddr(XY_GetAddr(XY_GetAddr(addr.libbase + offsets.Matrix) + 0x20) + 0x270);
                    uint64_t Tmp280 =   XY_GetAddr(XY_GetAddr(XY_GetAddr(addr.libbase + offsets.Matrix) + 0x20) + 0x280);

                    float matrix270[16];
                    memset(matrix270, 0, 16);
                    XY_Read(Tmp270, matrix270, 16 * 4);

                    float matrix280[16];
                    memset(matrix280, 0, 16);
                    XY_Read(Tmp280, matrix280, 16 * 4);

                    if (matrix270[14]==3){
                        cout << "20->270" << endl;
                        offsets.Matrix1=0x20;
                        offsets.Matrix2=0x270;
                    } else if (matrix280[14]==3){
                        cout << "20->280" << endl;
                        offsets.Matrix1=0x20;
                        offsets.Matrix2=0x280;
                    }
                    break;
                }
            }
            i++;
        }
        return addrOffsets;
    }
}


namespace DumpSDK{
    bool DumpUObject(){
        Dumper& Dump = Dumper::GetInstance();
        pidA = BigWhite_pid;
        uint64_t Base = addr.libbase;
        try
        {
            Dump.Init(Base, Base + offsets.GNames, Base + offsets.Gobject + 0x10);
        }
        catch (const char* error)
        {
            printf("%s\n", error);
            return -1;
        }
        Dump.Dump();
        return true;
    }
}
#endif //BIGWHITETOOL_TOOL_H
