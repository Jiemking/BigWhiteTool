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
    addr.libbase = XY_GetModuleBase(std::stoi(selectedPID));
    if (readmode==2){
        bool ret=checkDriver();
        cout << (ret?"biu加载成功":"biu加载失败") << endl;
        if (!ret){
            exec_native_surface("killall BigWhiteTool");
        }
        setpid(std::stoi(selectedPID));
    }
    if (name.find("tencent.mf.uam") != std::string::npos){//暗区偏移
        cout << "AQ"<< endl;
/*        Gname->710bcf8640   Offset->b1d5640
        Uworld->685a66f960  Offset->b39e3c0
        Matrix->6b5a93a320  Offset->b3e8a30*/
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
    } else if (name.find("ShuiSha") != std::string::npos){//枪战特训
        cout << "QZTX"<< endl;
        offsets.GNames=0xac1d440;
        offsets.Uworld=0xadd8850;
        offsets.Matrix=0xada8ea8;
        offsets.Matrix1=0x20;
        offsets.Matrix2=0x280;
        offsets.Ulevel=0x30;
        offsets.Arrayaddr=0x98;
        offsets.ArrayaddrCount=0xa0;
    }else if (name.find(".mf.uamo") != std::string::npos){//暗区国际
        cout << "AQGJ"<< endl;
         offsets.GNames=0xdde1980;
         offsets.Uworld=0xDFF05C8;
         offsets.Matrix=0xDFA6EF8;
         offsets.Matrix1=0x20;
         offsets.Matrix2=0x280;
         offsets.Ulevel=0x30;
         offsets.Arrayaddr=0x98;
         offsets.ArrayaddrCount=0xa0;
    }else if (name.find(".tmgp.gnyx") != std::string::npos){//暗区国际
        cout << "GNYX"<< endl;
        offsets.GNames=0xc4f74c0;
        offsets.Uworld=0xc63fea8;
        offsets.Matrix=0xc66f290;

    }
}