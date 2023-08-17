//
// Created by Administrator on 2023-08-16.
//

#ifndef BIGWHITETOOL_GETDATA_H
#define BIGWHITETOOL_GETDATA_H
string selectedPID,ProcessName;
#include "Android_Read/Android_Read.h"
#include "Enum.h"
#endif //BIGWHITETOOL_GETDATA_H

void GameInit(string name){
    addr.libbase = BigWhite_GetModuleBase(std::stoi(selectedPID),"libUE4.so");
    cout << name << endl;
    if (readmode==1){
        bool ret=checkDriver();
        puts(ret?"biu is true":"biu is false");
        setpid(std::stoi(selectedPID));

    }
    if (name.find("tencent.mf.uam") != std::string::npos){//暗区偏移
        cout << "AQ"<< endl;
        offsets.GNames=0xB1D5640;
        offsets.Ulevel=0x30;
        offsets.Matrix=0xB2FF440;
        offsets.Uworld=0xb3e8a30;
        offsets.GameInstance=0x38;
        offsets.ULocalPlayer=0x88;
        offsets.PlayerController=0x30;
        offsets.AcknowledgedPawn=0x330;
        offsets.RootComponent=0x158;
        offsets.XYZ_X=0x15C;
        offsets.isBot=0x330;
    } else if (name.find("ShuiSha") != std::string::npos){//枪战特训偏移
        cout << "QZTX"<< endl;
        offsets.GNames=0xac1d440;
        offsets.Uworld=0xadd8850;
        offsets.Matrix=0xada8ea8;
        offsets.Ulevel=0x30;
    }else if (name.find(".mf.uamo") != std::string::npos){//暗区
        cout << "AQGJ"<< endl;
         offsets.GNames=0xdde1980;
         offsets.Uworld=0xDFF05C8;
         offsets.Matrix=0xada8ea8;
         offsets.Ulevel=0x30;
    }
}