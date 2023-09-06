//
// Created by Administrator on 2023-08-16.
//

#ifndef BIGWHITETOOL_GETDATA_H
#define BIGWHITETOOL_GETDATA_H
string selectedPID,ProcessName;
#include "Android_Read/Android_Read.h"
#include "Enum.h"
#endif //BIGWHITETOOL_GETDATA_H

void GameInit(){
    addr.libbase = XY_GetModuleBase(std::stoi(selectedPID));
/*    printf("%lx",addr.libbase);
    cout << "" <<endl;*/
    if (readmode==2){
        bool ret=checkDriver();
        cout << (ret?"biu加载成功":"biu加载失败") << endl;
        if (!ret){
            exec_native_surface("killall BigWhiteTool");
        }
        setpid(std::stoi(selectedPID));
    }else if (readmode==3){

        cout << "这个驱动不好调用没写" << endl;
        exec_native_surface("killall BigWhiteTool");
    }
    if (ProcessName=="com.tencent.mf.uam"){//暗区
        cout << "AQ"<< endl;
        offsets.GNames=0xB1D5640;
        offsets.Ulevel=0x30;
        offsets.Matrix=0xb3e8a30;
        offsets.Uworld=0xb39e3c0;
        offsets.GameInstance=0x38;
        offsets.LocalPlayer=0x88;
        offsets.PlayerController=0x30;
        offsets.AcknowledgedPawn=0x330;
        offsets.RootComponent=0x158;
        offsets.XYZ_X=0x15C;
        offsets.isBot=0x330;
    } else if (ProcessName.find("ShuiSha") != std::string::npos){//枪战特训
        cout << "QZTX"<< endl;
        offsets.GNames=0xac1d440;
        offsets.Gobject=0xac61718;
        offsets.Uworld=0xadd8850;
        offsets.Matrix=0xada8ea8;
        offsets.Matrix1=0x20;
        offsets.Matrix2=0x280;
        offsets.Ulevel=0x30;
        offsets.Arrayaddr=0x98;
        offsets.ArrayaddrCount=0xa0;
        offsets.RootComponent = 0x130;
        offsets.XYZ_X=0x1D0;
    }else if (ProcessName.find(".mf.uamo") != std::string::npos){//暗区国际
        cout << "AQGJ"<< endl;
         offsets.GNames=0xDDE1980;
         offsets.Uworld=0xDFF05C8;
         offsets.Gobject=0xde00a20;
         offsets.Matrix=0xDFA6EF8;
         offsets.Matrix1=0x20;
         offsets.Matrix2=0x280;
         offsets.Ulevel=0x30;
         offsets.Arrayaddr=0x98;
         offsets.ArrayaddrCount=0xa0;

        offsets.GameInstance=0x180;
        offsets.LocalPlayer=0x38;
        offsets.PlayerController=0x30;
        offsets.AcknowledgedPawn=0x320;

        offsets.RootComponent = 0x148;
        offsets.XYZ_X=0x120;
    }else if (ProcessName.find(".tmgp.gnyx") != std::string::npos){//高能英雄
        cout << "GNYX"<< endl;
        offsets.GNames=0xcd29fc0;
        offsets.Uworld=0xCEA28E0;
        offsets.Matrix=0xce733e8;
        offsets.Matrix1=0x20;
        offsets.Matrix2=0x270;
        offsets.Ulevel=0x30;
        offsets.Arrayaddr=0x98;
        offsets.ArrayaddrCount=0xa0;
        offsets.GameInstance=0x3b0;
        offsets.LocalPlayer=0x38;
        offsets.PlayerController=0x30;
        offsets.AcknowledgedPawn=0x2D0;
        offsets.RootComponent = 0x1a8;
        offsets.XYZ_X=0x200;


    }else if (ProcessName=="com.tencent.mf.uamty"){//暗区体验
        cout << "AQTY"<< endl;
        offsets.GNames=0xee03740;
        offsets.Uworld=0xF01E7C0;
        offsets.Matrix=0xefd4718;
        offsets.Matrix1=0x20;
        offsets.Matrix2=0x2a0;
        offsets.Ulevel=0x30;
        offsets.Arrayaddr=0x98;
        offsets.ArrayaddrCount=0xa0;

        offsets.GameInstance=0x180;
        offsets.LocalPlayer=0x38;
        offsets.PlayerController=0x30;
        offsets.AcknowledgedPawn=0x330;
        offsets.RootComponent = 0x158;
        offsets.XYZ_X=0x138;
    }else if (ProcessName=="com.tencent.tmgp.pubgmhd"){//和平
        cout << "HEPING"<< endl;
        addr.isUE423=false;
/*        offsets.GNames=0xB1D5640;
        offsets.Matrix=0xB2FF440;
        offsets.Uworld=0xb3e8a30;*/
    }
}