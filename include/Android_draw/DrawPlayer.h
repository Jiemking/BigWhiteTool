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
    int py = displayInfo.height / 2;
    int px = displayInfo.width / 2;

    int ArrayaddrCount = BigWhite_GetDword( addr.Ulevel+ offsets.ArrayaddrCount);//数量
    float matrix[16];
    memset(matrix, 0, 16);
    BigWhite_vm_readv(addr.Matrix, matrix, 16 * 4);

    for (int i = 0; i < ArrayaddrCount; i++) {

        uint64_t Objaddr = BigWhite_GetPtr64(addr.Arrayaddr + 8 * i);  // 遍历数量次数
        if (Objaddr == 0)   continue;
        // 自身坐标
        Vector3A Z;
        BigWhite_vm_readv(BigWhite_GetPtr64(addr.AcknowledgedPawn + offsets.RootComponent) + offsets.XYZ_X, &Z, sizeof(Z)); // 自己坐标

        // 敌人和物资坐标
        Vector3A D;
        BigWhite_vm_readv(BigWhite_GetPtr64(Objaddr + offsets.RootComponent) + offsets.XYZ_X, &D, sizeof(D)); // 对象坐标
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
        std::string ClassName = GetName(Objaddr);//类名
        Draw->AddText(NULL, 24, {r_x , r_y-60}, ImColor(255,255,255,255) , ClassName.c_str());//类名绘制

        Draw->AddText(NULL, 24, {r_x , r_y+60}, ImColor(255,255,255,255) , to_string(BigWhite_GetDword(BigWhite_GetPtr64(Objaddr+0x2C0)+0x2a4)).c_str());

        char PlayerNameTmp[100] = "";
        BigWhite_GetUTF8(PlayerNameTmp,BigWhite_GetPtr64(BigWhite_GetPtr64(Objaddr + 0x2C0) + 0x320));//获取该数组人物名称
        Draw->AddText(NULL, 24, {r_x , r_y+0}, ImColor(255,255,255,255) , PlayerNameTmp);


        //类名绘制



        /*
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
    }

}

