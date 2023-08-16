//
// Created by Administrator on 2023-08-16.
//
#include <Enum.h>
#include <fstream>
#include "Android_Read/Android_Read.h"

#ifndef BIGWHITETOOL_TOOL_H
#define BIGWHITETOOL_TOOL_H

uint64_t GetClass(uint64_t Address) {
    uint64_t UobjectClass = BigWhite_GetPtr64((Address + 0x10));
    if (UobjectClass != NULL) {
        return UobjectClass;
    }
    return NULL;
}

FName ReadProcessFname(uint64_t address) {
    FName buffer = {0};
    BigWhite_vm_readv(address, &buffer, sizeof(FName));
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
    uintptr_t G_Names = BigWhite_GetPtr64(addr.GNames);
    int Id = (int)(i / (int)0x4000);
    int Idtemp = (int)(i % (int)0x4000);
    auto NamePtr = BigWhite_GetPtr64((G_Names + Id * 8));
    auto Name = BigWhite_GetPtr64((NamePtr + 8 * Idtemp));
    char name[0x100] = { 0 };
    if (BigWhite_vm_readv((Name + 0xC), name, 0x100)) //0xC需要手动推算，默认是0x10
    {
        return name;
    }
    return std::string();
}

std::string GetName_New(uint32_t index) //新版本算法
{
    unsigned int Block = index >> 16;
    unsigned short int Offset = index & 65535;

    uintptr_t FNamePool = addr.GNames;

    uintptr_t NamePoolChunk = BigWhite_GetPtr64(FNamePool+0x40 + (Block * 0x8));

    uintptr_t FNameEntry = NamePoolChunk + (0x2 * Offset);

    short int FNameEntryHeader = BigWhite_GetDword(FNameEntry);
    uintptr_t StrPtr = FNameEntry + 0x2;

    int StrLength = FNameEntryHeader >> 6;
    if (StrLength > 0 && StrLength < 250) {
        string name(StrLength, '\0');
        BigWhite_vm_readv(StrPtr, name.data(), StrLength * sizeof(char));
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
    uint64_t outer = BigWhite_GetPtr64(Address + 0x20);
    if (outer != NULL)
        return outer;
    return uint64_t();
};

std::string GetOuterName(uint64_t Address) {
    auto klass = GetClass(Address);
    if (klass != NULL) {
        std::string temp;
        for (auto outer = GetOuter(Address); outer != 0; outer = GetOuter(outer)) {
            temp = GetName(outer) + "." + temp;
        }

        temp += GetName(Address);  //自己的类名

        return temp;
    }

    return std::string("(null)");
}


std::vector<ProcessInfo> GetTencentProcesses() {
    std::vector<ProcessInfo> processes;

    // Open the /proc directory
    DIR* procDir = opendir("/proc");
    if (!procDir) {
        return processes;
    }

    // Read the entries in the /proc directory
    struct dirent* entry;
    while ((entry = readdir(procDir)) != NULL) {
        // Check if the entry is a directory and its name is a number (process ID)
        if (entry->d_type == DT_DIR && isdigit(entry->d_name[0])) {
            std::string pid = entry->d_name;
            std::string statFilePath = "/proc/" + pid + "/status";

            // Read the process name from the status file
            std::ifstream statFile(statFilePath);
            if (statFile.is_open()) {
                std::string line;
                while (std::getline(statFile, line)) {
                    if (line.find("Name:") == 0) {
                        std::string processName = line.substr(6);

                        if (processName.find("tencent") != std::string::npos || processName.find("m.") != std::string::npos|| processName.find("uam") != std::string::npos) {
                            ProcessInfo processInfo;
                            processInfo.pid = pid;
                            processInfo.name = processName;
                            processInfo.isSelected = false; // Initialize selection state
                            processes.push_back(processInfo);
                        }
                        break;
                    }
                }
                statFile.close();
            }
        }
    }
    closedir(procDir);

    return processes;
}
static vector<StructureList> foreachAddress(uint64_t Address) {
    std::vector<StructureList> structureList; // 使用std::vector存储输出内容
    for (size_t i = 0; i < 0x200; i+=4) {
        long int Tmp = BigWhite_GetPtr64(Address + i);
        string KlassName = GetClassName(Tmp);
        string outerName = GetOuterName(Tmp);

        StructureList data;
        data.address = (Address + i);
        data.type = KlassName.c_str();
        data.name = outerName.c_str();
        data.offset=i;


        data.P = BigWhite_GetPtr64(data.address);
        data.F= BigWhite_GetFloat(data.address);
        data.D= BigWhite_GetDword(data.address);
//        if (outerName.find("None") && KlassName.find("null") && getPtr64(data.address)==0)

        structureList.push_back(data);

    }
    return structureList;
}
int GetEventCount() {
    DIR *dir = opendir("/dev/input/");
    dirent *ptr = NULL;
    int count = 0;
    while ((ptr = readdir(dir)) != NULL) {
        if (strstr(ptr->d_name, "event"))
            count++;
    }
    return count ? count : -1;
}

void ResetOffsets(){
    offsets.libbase = 0;
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
    offsets.ULocalPlayer = 0x0;
    offsets.PlayerController = 0x0;
    offsets.AcknowledgedPawn = 0x0;

    offsets.Mesh = 0x0;
    offsets.Human = 0x0;
    offsets.Bone = 0x0;
    for (int i = 0; i < 17; i++) {
        offsets.BoneArray[i] = 0;
    }
    offsets.isUE423 = true;
}

namespace UEinit{
    AddrOffsets GetGname(){
        AddrOffsets addrOffsets;
        addrOffsets.Addr=0;
        addrOffsets.Offsets=0;
        int i=20000000;
        while (1){
            long int TMP = addr.libbase + (0x8*i) + 0x40;
            if (TMP != NULL){
                long int TMPGnames = BigWhite_GetPtr64(TMP);
                char name[0x100];
                BigWhite_vm_readv(TMPGnames+0x8, name, 0xc);
                std::string aa = name;
                if (aa=="ByteProperty"){
                    addrOffsets.Addr=addr.libbase+(0x8*i);
                    addrOffsets.Offsets=(0x8*i);
                    offsets.GNames=(0x8*i);//设置全局Gname偏移
                    addr.GNames = addr.libbase + offsets.GNames;//设置全局Gname地址
                    break;
                }
            }
            i++;
        }
        return addrOffsets;
    }
    AddrOffsets GetGworld(){
        AddrOffsets addrOffsets;
        addrOffsets.Addr=0;
        addrOffsets.Offsets=0;
        int i=0;

        while (1){
            long int TMPUworld = BigWhite_GetPtr64(addr.libbase + offsets.GNames + (0x8*i));
            if (TMPUworld != NULL){
                //printf("%lx",TMPUworld);
                if (GetClassName(TMPUworld)== "World"){
                    //(GetName(TMPUworld)=="TutorialRange_Main") 这个可以不需要
                    //if (GetClassName(BigWhite_GetPtr64(TMPUworld+offsets.Ulevel))=="Level"){}
                        addrOffsets.Addr=TMPUworld;
                        addrOffsets.Offsets=offsets.GNames + (0x8*i);
                        offsets.Uworld=offsets.GNames + (0x8*i);
                        break;

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
        int i=0;

        cout << GetClassName(BigWhite_GetPtr64(addr.libbase + 0xAC61718)) <<endl;
/*        while (1){
            long int TMPEngine = BigWhite_GetPtr64(addr.libbase + offsets.GNames + (0x8*i));
            if (TMPEngine != NULL){
                //printf("%lx",TMPUworld);
                if (GetClassName(TMPEngine)== "GameEngine"){
//                    if (GetClassName(BigWhite_GetPtr64(TMPUworld+offsets.Ulevel))=="Level"){}
                    addrOffsets.Addr=TMPEngine;
                    addrOffsets.Offsets=offsets.GNames + (0x8*i);
                    break;

                }
            }
            i++;
        }*/
        return addrOffsets;
    }
    AddrOffsets GetEngine(){
        AddrOffsets addrOffsets;
        addrOffsets.Addr=0;
        addrOffsets.Offsets=0;
        int i=0;

        //cout << GetClassName(BigWhite_GetPtr64(addr.libbase + 0xADD4868)) <<endl;
        while (1){
            long int TMPEngine = BigWhite_GetPtr64(addr.libbase + offsets.GNames + (0x8*i));
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
}


#endif //BIGWHITETOOL_TOOL_H
