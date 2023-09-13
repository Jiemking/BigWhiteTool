//
// Created by Administrator on 2023-08-16.
//
#ifndef BIGWHITETOOL_TOOL_H
#define BIGWHITETOOL_TOOL_H
#include <Enum.h>
#include <fstream>
#include<Android_Read/Android_Read.h>
#include "Android_draw/ItemData.h"
#include "dumper.h"
#include "engine.h"
#include "generic.h"
int tencent = 0;//这个全局变量过滤进程使用

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
    return {};
}


std::string GetName(uint64_t Address) {
    int FnameComparisonIndex = BigWhite_GetDword(Address + Offsets.UObject.Name);
    std::string GetName;
    if (isUE423){
        GetName = NamePoolData->GetName(FnameComparisonIndex); //新算法获取Name
    }else{
        GetName = GetName_Old(FnameComparisonIndex); //旧版本算法获取Name
    }
    return GetName;
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
    for (size_t i = 0; i < 0x500; i+=4) {
        UE_UClass* Tmp = XY_TRead<UE_UClass*>(Address + i);
        string KlassName = Tmp->GetClass()->GetName();
        string outerName = Tmp->GetName();
        string trans = ItemData::UamoGetString(KlassName);

        StructureList data;
        data.address = (Address + i);
        data.type = KlassName;
        data.name = outerName;
        data.trans = trans;
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
    dirent *ptr;
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
    isUE423 = true;
}

namespace UEinit{
    AddrOffsets GetGname(){
        AddrOffsets addrOffsets;
        addrOffsets.Addr=0;
        addrOffsets.Offsets=0;
        for (int i = 20000000;; i++) {
            uint64_t TMP;
            if (isUE423){
                TMP = addr.libbase + (0x8*i) + 0x40;
                if (TMP != 0){
                    uint64_t TMPGnames = BigWhite_GetPtr64(TMP);
                    char name[0x100];
                    BigWhite_vm_readv(TMPGnames+0x8, name, 0xc);
                    std::string aa = name;
                    if (aa.find("ByteProperty") != std::string::npos){
                        addrOffsets.Addr=addr.libbase+(0x8*i);
                        addrOffsets.Offsets=(0x8*i);
                        offsets.GNames=(0x8*i);//设置全局Gname偏移
                        addr.GNames = addr.libbase + offsets.GNames;//设置全局Gname地址
                        AddrGNames=addr.GNames;
                        break;
                    }
                }
            } else{
                TMP = addr.libbase + (0x8*i);
/*                printf("%lx",(0x8*i));
                cout << "\n"<<endl;*/
                if (TMP < 0x1000000000) continue;
                uint64_t TMP1 = BigWhite_GetPtr64(TMP);
                if (TMP1 < 0x1000000000) continue;
                uint64_t TMP2 = BigWhite_GetPtr64(TMP1);
                if (TMP2 < 0x1000000000) continue;
                uint64_t TMP3 = BigWhite_GetPtr64(TMP2);
                if (TMP3 < 0x1000000000) continue;
                uint64_t TMPGnames = TMP3;
                char name[0x100];
                BigWhite_vm_readv(TMPGnames+0x24, name, 0xc);
                std::string aa = name;
                if (aa.find("ByteProperty") != std::string::npos){
                    addrOffsets.Addr=addr.libbase+(0x8*i);
                    addrOffsets.Offsets=(0x8*i);
                    offsets.GNames=(0x8*i);//设置全局Gname偏移
                    addr.GNames = addr.libbase + offsets.GNames;//设置全局Gname地址
                    AddrGNames=addr.GNames;
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
        for (int i = 0;; i++) {
            uint64_t TMP;
            TMP = addr.libbase + offsets.GNames + (0x8*i);
            if (TMP < 0x1000000000) continue;
            UE_UObject* TMP1 = XY_TRead<UE_UObject*>(TMP);
            string TMP1Class = TMP1->GetClass()->GetName();
            if (TMP1Class=="World"){
                addrOffsets.Addr=TMP;
                addrOffsets.Offsets=offsets.GNames + (0x8*i);
                offsets.Uworld=offsets.GNames + (0x8*i);
                break;
            }
        }
        return addrOffsets;
    }

    AddrOffsets GetGWorld(){
        AddrOffsets addrOffsets;
        addrOffsets.Addr=0;
        addrOffsets.Offsets=0;
        for (int i = 0;; i++) {
            uint64_t TMP;
            TMP = addr.libbase + offsets.GNames + (0x8*i);
            if (TMP < 0x1000000000) continue;
            UE_UObject* TMP1 = XY_TRead<UE_UObject*>(TMP);
            string TMP1Class = TMP1->GetClass()->GetName();

            if (TMP1Class=="World"){
                addrOffsets.Addr=TMP;
                addrOffsets.Offsets=offsets.GNames + (0x8*i);
                offsets.Uworld=offsets.GNames + (0x8*i);
                break;
            }
        }
        return addrOffsets;
    }

    AddrOffsets GetGobject(){
        AddrOffsets addrOffsets;
        addrOffsets.Addr=0;
        addrOffsets.Offsets=0;
        for (int i = 0;; i++) {
            uint64_t TMP;
            TMP = addr.libbase + offsets.GNames + (0x8*i) +0x10;
            if (TMP < 0x1000000000) continue;
            uint64_t TMP1 = BigWhite_GetPtr64(TMP);
            if (TMP1 < 0x1000000000) continue;
            uint64_t TMP2 = BigWhite_GetPtr64(TMP1);
            if (TMP2 < 0x1000000000) continue;
            UE_UObject* TMP3 = XY_TRead<UE_UObject*>(TMP2);
            UE_UObject* TMP4 = XY_TRead<UE_UObject*>(TMP2+0x18);
            string TMP3Class = TMP3->GetClass()->GetName();
            string TMP3Outer = TMP3->GetName();
            string TMP4Class = TMP4->GetClass()->GetName();
            string TMP4Outer = TMP3->GetName();
            if (TMP3Class.find("Package") != std::string::npos && TMP3Outer.find("/Script/CoreUObject") != std::string::npos ){
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
        while (1){
            UE_UObject* TMPEngine = XY_TRead<UE_UObject*>(addr.libbase + offsets.GNames + (0x8*i));
            if (TMPEngine != 0){
                if (TMPEngine->GetClass()->GetName()== "GameEngine"){
                    addrOffsets.Addr= reinterpret_cast<uint64_t>(TMPEngine);
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

        UE_UObject* TMPS= XY_TRead<UE_UObject*>(BigWhite_GetPtr64(addr.libbase + 0xada8ea8)+0x20);
        cout << TMPS->GetClass()->GetName() <<endl;
        while (true){
            uint64_t TMPMatrix = BigWhite_GetPtr64(addr.libbase + offsets.GNames + (0x8*i));
            if (TMPMatrix != 0){
                UE_UObject* Tmp = XY_TRead<UE_UObject*>((TMPMatrix)+0x20);
                if (Tmp->GetClass()->GetName()== "Canvas"){
                    addrOffsets.Addr=TMPMatrix;
                    addrOffsets.Offsets=offsets.GNames + (0x8*i);
                    offsets.Matrix=offsets.GNames + (0x8*i);
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
