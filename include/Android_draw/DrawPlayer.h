//
// Created by Administrator on 2023-08-16.
//

#ifndef BIGWHITETOOL_DRAWPLAYER_H

#include "Android_Read/Android_Read.h"
#include "Android_Read/GetData.h"
#include <sstream>
#define BIGWHITETOOL_DRAWPLAYER_H

#endif //BIGWHITETOOL_DRAWPLAYER_H



void DrawPlayer(ImDrawList *Draw) {
    addr.Uworld = XY_GetAddr(addr.libbase + offsets.Uworld);
    addr.Ulevel = XY_GetAddr(addr.Uworld + offsets.Ulevel);
    addr.Arrayaddr = XY_GetAddr(addr.Ulevel + offsets.Arrayaddr);
/*    addr.Matrix =  XY_GetAddr(XY_GetAddr(XY_GetAddr(addr.libbase + offsets.Matrix) + offsets.Matrix1) + offsets.Matrix2);//暗区还需要跳进去一层
    uint64_t oneself = XY_GetAddr(XY_GetAddr(XY_GetAddr(XY_GetAddr(addr.Uworld + 0x180)+0x38))+0x30)+320;//暗区国际*/
/*    addr.Matrix =  XY_GetAddr(XY_GetAddr(addr.libbase + offsets.Matrix) + offsets.Matrix1) + offsets.Matrix2;//枪战特训
    uint64_t oneself = XY_GetAddr(XY_GetAddr(XY_GetAddr(XY_GetAddr(addr.Uworld + 0x180)+0x38))+0x30)+250;*/
/*    addr.Matrix =  XY_GetAddr(XY_GetAddr(XY_GetAddr(addr.libbase + offsets.Matrix) + offsets.Matrix1) + offsets.Matrix2);//暗区体验
    uint64_t oneself = XY_GetAddr(XY_GetAddr(XY_GetAddr(XY_GetAddr(addr.Uworld + 0x180)+0x38))+0x30)+330;//暗区体验  */
    addr.Matrix =  XY_GetAddr(XY_GetAddr(addr.libbase + offsets.Matrix) + offsets.Matrix1) + offsets.Matrix2;//高能英雄
    addr.AcknowledgedPawn = XY_GetAddr(XY_GetAddr(XY_GetAddr(XY_GetAddr(addr.Uworld + offsets.GameInstance)+offsets.LocalPlayer))+offsets.PlayerController)+offsets.AcknowledgedPawn;//暗区体验

    //7c0e74ff38
/*    printf("矩阵：%lx",addr.libbase + offsets.Matrix);
    cout << "" <<endl;*/

    float top, right, left, bottom, x1, top1;
    int py = displayInfo.height / 2;
    int px = displayInfo.width / 2;

    int ArrayaddrCount = XY_GetDword( addr.Ulevel+ offsets.ArrayaddrCount);//数量
    float matrix[16];
    memset(matrix, 0, 16);
    XY_Read(addr.Matrix, matrix, 16 * 4);

    //cout <<ArrayaddrCount<<endl;
    for (int i = 0; i < ArrayaddrCount; i++) {

        uint64_t Objaddr = XY_GetAddr(addr.Arrayaddr + 8 * i);  // 遍历数量次数
        if (Objaddr == 0x0000000000 || Objaddr == 0 || Objaddr == 0x000 )   continue;
        // 自身坐标
        Vector3A Z;
        XY_Read(XY_GetAddr(addr.AcknowledgedPawn + offsets.RootComponent) + offsets.XYZ_X, &Z, sizeof(Z)); // 自己坐标


        // 敌人和物资坐标
        Vector3A D;
        XY_Read(XY_GetAddr(Objaddr + offsets.RootComponent) + offsets.XYZ_X, &D, sizeof(D)); // 对象坐标

        //吸人
/*        BigWhite_WriteFloat(XY_GetAddr(Objaddr + offsets.RootComponent) + offsets.XYZ_X,Z.X);
        BigWhite_WriteFloat(XY_GetAddr(Objaddr + offsets.RootComponent) + offsets.XYZ_X+0x4,Z.Y);
        BigWhite_WriteFloat(XY_GetAddr(Objaddr + offsets.RootComponent) + offsets.XYZ_X+0x8,Z.Z+3000.0f);*/
//        暗区国际基本上都是138 148 158
        if (D.X == 0 || D.Y == 0 || D.Z == 0)   continue;

        float camera,r_x,r_y,r_w;
        camera = matrix[3] * D.X + matrix[7] * D.Y + matrix[11] * D.Z + matrix[15];
        r_x = px + (matrix[0] * D.X + matrix[4] * D.Y + matrix[8] * D.Z + matrix[12]) / camera * px;
        r_y = py - (matrix[1] * D.X + matrix[5] * D.Y + matrix[9] * (D.Z - 5) + matrix[13]) / camera * py;
        r_w = py - (matrix[1] * D.X + matrix[5] * D.Y + matrix[9] * (D.Z + 205) + matrix[13]) / camera * py;

        float x = r_x - (r_y - r_w) / 4;
        float y = r_y;
        float w = (r_y - r_w) / 2;
        float xlx = r_x - (r_y - r_w) / 85;
        //float Distance = sqrt(pow(D.X - Z.X, 2) + pow(D.Y - Z.Y, 2) + pow(D.Z - Z.Z, 2)) * 0.01;
        float middle = x + w / 2;
        float bottom = y + w;
        float top = y - w;
        float ah =r_y - r_w;
        if (ah<=0)  continue;//过滤背部敌人
        left = (x + w / 2) - w / 2.6f;
        right = x + w / 1.12f;


        std::string ClassName = GetName(Objaddr);//类名
/*        if (ClassName.find("BP_DeathBox")!= std::string::npos){
            Draw->AddText(NULL, 24, {r_x , r_y-60}, ImColor(255,255,255,255) , ClassName.c_str());
        }*/

       //类名绘制
        Draw->AddText(NULL, 24, {r_x , r_y-60}, ImColor(255,255,255,255) , ClassName.c_str());/*
        //类名翻译
        Draw->AddText(NULL, 24, {r_x , r_y}, ImColor(255,255,255,255) , ItemData::UamoGetString(ClassName).c_str());
        //类名地址
        std::stringstream Objaddrstr;
        Objaddrstr << std::hex << Objaddr;  // 将长整型以十六进制格式写入 stringstream
        std::string ObjaddString = Objaddrstr.str();  // 获取十六进制字符串
        Draw->AddText(NULL,20 , {r_x , r_y-20}, ImColor(255,255,255,255) , ObjaddString.c_str());*/

/*        if (XY_GetFloat(Objaddr + 0x42C) !=40.0f)
        {
            continue;
        }*/
        //Draw->AddRect({(x + w / 2) - w / 2.0f, y-w},{x+w/1.12f, y + w},ImColor(255,0,0,255),{0.0},0,{1});
/*        std::stringstream oneselfstr;
        oneselfstr << std::hex << oneself;  // 将长整型以十六进制格式写入 stringstream
        std::string oneselfString = oneselfstr.str();  // 获取十六进制字符串*/


        //Draw->AddText(NULL,20 , {middle-100, top}, ImColor(255,255,255,255) , hexString.c_str());
        //Draw->AddText(NULL,20 , {(x + w / 2) - w / 2.0f, y + w}, ImColor(255,255,255,255) , ObjaddString.c_str());
        //Draw->AddText(NULL,40 , {100, 100}, ImColor(255,255,255,255) , oneselfString.c_str());
    }

}

