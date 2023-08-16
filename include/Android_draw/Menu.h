//
// Created by Administrator on 2023-08-16.
//
#include <Enum.h>
#include <Tool.h>

#ifndef BIGWHITETOOL_MENU_H
#define BIGWHITETOOL_MENU_H

// 显示节点信息
static void ShowPlaceholderObject(StructureList data, int uid) {
    // 使用对象的地址作为标识符，确保唯一性。
    ImGui::PushID(data.address);

    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::AlignTextToFramePadding();

    bool node_open = ImGui::TreeNode("Tree", "%x——Class:%s————————Name:%s————————%lx : P->%lx F->%f D->%d", data.offset, data.type.c_str(), data.name.c_str(),data.address, data.P,data.F,data.D);

    ImGui::TableSetColumnIndex(0);
    if (node_open && GetClass(data.P)!=NULL) {
        static float placeholder_members[8] = {0.0f, 0.0f, 1.0f, 3.1416f, 100.0f, 999.0f};
        std::vector<StructureList> aaa = foreachAddress(data.P);
        int i = 0;
        for (const auto &item: aaa) {
            ImGui::PushID(i); // Use field index as identifier.
            ShowPlaceholderObject(item, i);
            ImGui::PopID();
            i++;
        }
        ImGui::TreePop();
    }

    ImGui::PopID();
}
// 假设这个变量用于存储编辑框的文本
static char inputText[256] = "";
// 虚拟键盘的字符
static const char* keyboardChars = "abcdefghijklmnopqrstuvwxyz1234567890";

bool showVirtualKeyboard = false;

void RenderVirtualKeyboard() {
    // 创建虚拟键盘的ImGui窗口
    ImGui::Begin("虚拟键盘");

    // 使用循环创建虚拟键盘按钮
    for (int i = 0; i < strlen(keyboardChars); ++i) {
        if (i % 10 != 0) {
            ImGui::SameLine();
        }
        std::string a(1, keyboardChars[i]);
        if (ImGui::Button(a.c_str(), ImVec2(80, 80))) {
            // 点击按钮时将字符添加到编辑框文本
            char c[2] = { keyboardChars[i], '\0' };
            strcat(inputText, c);
        }
    }
    // 添加 "Back" 按钮
    if (ImGui::Button("Back", ImVec2(80, 80))) {
        size_t inputLength = strlen(inputText);
        if (inputLength > 0) {
            inputText[inputLength - 1] = '\0'; // 删除最后一个字符
        }
    }
    if (ImGui::Button("关闭键盘", ImVec2(100, 80))) {
        showVirtualKeyboard = false;
    }

    ImGui::End();
}
void ShowUE4DumperWindow();
namespace Menu{
    void ShowUE4DumperWindow(){
        ImGui::Begin("UE4Dump工具");
        // 渲染编辑框

        ImGui::InputText("输入框", inputText, IM_ARRAYSIZE(inputText));
        // 如果点击了编辑框，则显示虚拟键盘并获取焦点
        if (ImGui::IsItemClicked()) {
            showVirtualKeyboard = true;
            ImGui::SetKeyboardFocusHere(-1); // 将焦点设置给虚拟键盘
        }

        if (ImGui::Button("确定")){
            // 尝试将输入框内容转换为长整型（十六进制）
            long inputValue = strtoll(inputText, nullptr, 16);  // 第三个参数是进制，这里使用十六进制
            // 检查转换是否成功
            if (inputValue != 0 || inputText[0] == '0' || (inputText[0] == '0' && inputText[1] == 'x')) {
                // 转换成功，显示长整型值
                ImGui::Text("转换为长整型：%ld", inputValue);
            } else {
                // 转换失败，显示错误信息
                ImGui::Text("无效的输入");
            }
            cout << inputValue<<endl;
            addr.Address=inputValue;
        }
        if (ImGui::Button("Uworld")){
            addr.Address= (addr.Uworld);
        }
        if (ImGui::Button("Debug")){
            addr.Address= (addr.libbase+0x5BDEF8);
        }



        // 渲染虚拟键盘
        if (showVirtualKeyboard) {
            RenderVirtualKeyboard();
        }
        ImGui::SetNextWindowSize(ImVec2(1500, 1000), ImGuiCond_FirstUseEver);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
        if (ImGui::BeginTable("split", 1, ImGuiTableFlags_BordersOuter | ImGuiTableFlags_Resizable)) {
           // std::vector<StructureList> aaa = foreachAddress(addr.Address);

            std::vector<StructureList> aaa = foreachAddress(addr.Address);
            int i = 100000;
            for (const auto &data : aaa) {
                ImGui::PushID(i);
                ShowPlaceholderObject(data, i);
                ImGui::PopID();
                //ImGui::Separator();
            }
            ImGui::EndTable();
        }
        ImGui::PopStyleVar();
        ImGui::End();
    }
    void ShowFindDataWindow(){
        ImGui::Begin("初始化数据");
        // 设置窗口的目标大小
        ImVec2 windowSize(500, 700); // 宽度为 400，高度为 300
        ImGui::SetWindowSize(windowSize);

        static bool isShow = false;
        static char GnameBuffer[0x128];
        static char GObjectBuffer[0x128];
        static char GWorldBuffer[0x128];
        static char GEngineBuffer[0x128];
        static char DebugCanvasBuffer[0x128];


        if (BigWhite_pid>0){
            isShow= true;
            if (ImGui::Button("一键获取",ImVec2(ImGui::GetWindowWidth(),75))){
                sprintf(GnameBuffer,"Gname->%lx   Offset->%lx",UEinit::GetGname().Addr,UEinit::GetGname().Offsets);
                sprintf(GWorldBuffer,"Uworld->%lx  Offset->%lx",UEinit::GetGworld().Addr,UEinit::GetGworld().Offsets);
                sprintf(GEngineBuffer,"GEngine->%lx  Offset->%lx",UEinit::GetEngine().Addr,UEinit::GetEngine().Offsets);

            }
            if (ImGui::Button("获取Gname",ImVec2(ImGui::GetWindowWidth(),75))){
                sprintf(GnameBuffer,"Gname->%lx   Offset->%lx",UEinit::GetGname().Addr,UEinit::GetGname().Offsets);
            }
            if (ImGui::Button("获取GetGobject",ImVec2(ImGui::GetWindowWidth(),75))){
//                sprintf(GObjectBuffer,"GetGobject->%lx   Offset->%lx",UEinit::GetGobject().Addr,UEinit::GetGobject().Offsets);
            }
            if (ImGui::Button("获取Uworld",ImVec2(ImGui::GetWindowWidth(),75))){
                sprintf(GWorldBuffer,"Uworld->%lx  Offset->%lx",UEinit::GetGworld().Addr,UEinit::GetGworld().Offsets);
            }
            if (ImGui::Button("获取GEngine",ImVec2(ImGui::GetWindowWidth(),75))){
                sprintf(GEngineBuffer,"GEngine->%lx  Offset->%lx",UEinit::GetEngine().Addr,UEinit::GetEngine().Offsets);
            }
            if (isShow && ImGui::Button("保存",ImVec2(ImGui::GetWindowWidth(),75))){
                ImGui::Text("保存路径在：内部存储/A_BigWhiteTool");
                mkdir("/storage/emulated/0/A_BigWhiteTool",2770);
                FILE* outFile = fopen("/storage/emulated/0/A_BigWhiteTool/Data.txt", "w+");

                if (outFile) {
                    fprintf(outFile, "%s\n", GnameBuffer);
                    fprintf(outFile, "%s\n", GWorldBuffer);
                    fclose(outFile);
                    printf("Output written to output.txt\n");
                } else {
                    printf("Error opening output file\n");
                }
            }
            if (isShow){
                ImGui::Text("%s",GnameBuffer,ImVec2(ImGui::GetWindowWidth(),75));
                ImGui::Text("%s",GWorldBuffer,ImVec2(ImGui::GetWindowWidth(),75));
                ImGui::Text("%s",GEngineBuffer,ImVec2(ImGui::GetWindowWidth(),75));
            }
        } else{
            ImGui::Text("请先选择进程！",ImVec2(ImGui::GetWindowWidth(),75));
        }



        ImGui::End();
    }
}
#endif //BIGWHITETOOL_MENU_H
