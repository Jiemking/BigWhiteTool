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

    char formattedData[1000]; // 足够大的缓冲区来存储格式化后的数据

    if (data.P<0xffffffffff && data.P>0x7000000000){

        sprintf(formattedData, "(%x)—%lx—P->%lx-Class:%s————————Name:%s",
                data.offset,  data.address, data.P, data.type.c_str(), data.name.c_str());

        bool node_open = ImGui::TreeNode("Tree", "%s",formattedData);
        ImGui::TableSetColumnIndex(0);
        if (node_open) {
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

    } else if (data.F>0.001f && data.D>0){
        sprintf(formattedData, "(%x)—%lx—Class:%s————————Name:%s————————F->%f D->%d",
                data.offset,  data.address,data.type.c_str(), data.name.c_str(), data.F, data.D);
        ImGui::Text("      %s", formattedData);
    } else if (data.F>0.001f){
        sprintf(formattedData, "(%x)—%lx—Class:%s————————Name:%s————————F->%f",
                data.offset,  data.address,data.type.c_str(), data.name.c_str(), data.F);
        ImGui::Text("      %s", formattedData);
    } else if (data.D>0){
        sprintf(formattedData, "(%x)—%lx—Class:%s————————Name:%s————————D->%d",
                data.offset,  data.address,data.type.c_str(), data.name.c_str(),data.D);
        ImGui::Text("      %s", formattedData);
    } else{
        sprintf(formattedData, "(%x)—%lx—Class:%s————————Name:%s————————F->%f D->%d",
                data.offset,  data.address,data.type.c_str(), data.name.c_str(), data.F, data.D);
        ImGui::Text("      %s", formattedData);
    }



    ImGui::PopID();
}


bool showVirtualKeyboard = false;
const int MaxEditBoxes = 10; // 假设最多有10个编辑框
std::string inputText[MaxEditBoxes];
int activeEditBoxIndex = -1; // 跟踪活动编辑框的索引
int OffsetLevel= 1;

void RenderVirtualKeyboard() {
    ImGui::Begin("虚拟键盘");

    static const char keyboardChars[] = "1234567890QWERTYUIOPASDFGHJKLZXCVBNM";

    for (int i = 0; i < strlen(keyboardChars); ++i) {
        if (i % 10 != 0) {
            ImGui::SameLine();
        }

        std::string a(1, keyboardChars[i]);
        if (ImGui::Button(a.c_str(), ImVec2(80, 80))) {
            if (activeEditBoxIndex >= 0 && activeEditBoxIndex < MaxEditBoxes) {
                char c[2] = { keyboardChars[i], '\0' };
                inputText[activeEditBoxIndex] += c;
            }
        }
    }

    if (ImGui::Button("Back", ImVec2(80, 80))) {
        if (activeEditBoxIndex >= 0 && activeEditBoxIndex < MaxEditBoxes && !inputText[activeEditBoxIndex].empty()) {
            inputText[activeEditBoxIndex].pop_back();
        }
    }

    if (ImGui::Button("关闭键盘", ImVec2(100, 80))) {
        showVirtualKeyboard = false;
    }

    ImGui::End();
}
namespace Menu{
    void ShowUE4DumperWindow(){
        ImGui::Begin("UE4Dump工具");
        // 渲染编辑框
        for (int i = 0; i < OffsetLevel; ++i) {
            ImGui::InputText((i == 0) ? "地址" : (std::to_string(i) + "层偏移").c_str(), inputText[i].data(), inputText[i].size() + 1); // 使用 size() + 1
            // 如果点击了编辑框，则显示虚拟键盘并获取焦点
            if (ImGui::IsItemClicked()) {
                activeEditBoxIndex = i; // 记录活动编辑框的索引
                showVirtualKeyboard = true;
                ImGui::SetKeyboardFocusHere(-1);
            }

            if (i==0){
                ImGui::SameLine();
                if (ImGui::Button("+")){
                    OffsetLevel++;
                }
                ImGui::SameLine();
                if (ImGui::Button("-")){
                    if (OffsetLevel>1)   OffsetLevel--;
                }
            }
        }



        if (ImGui::Button("确定")){
            uint64_t Address = 0;
            for (int i = 0; i < sizeof(inputText) / sizeof(inputText[0]); ++i) {
                if (inputText[i]!=""){
                    uint64_t inputValue;
                    if (inputText[i].find("BASE")!= std::string::npos){
                        inputValue = addr.libbase;
                    }else {
                        inputValue = std::stoi(inputText[i], nullptr, 16);// 第三个参数是进制，这里使用十六进制
                    }
                    if (i==0){
                        Address += inputValue;
                    }else{
                        Address=XY_GetAddr(Address+inputValue);
                    }
                }
                cout << i << endl;
            }
            addr.Address = Address;

            cout << ""<<endl;
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

    void ShowDebugMatrixWindow(){
        static int corner = 0;
        ImGuiIO& io = ImGui::GetIO();
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
        if (corner != -1)
        {
            const float PAD = 10.0f;
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
            ImVec2 work_size = viewport->WorkSize;
            ImVec2 window_pos, window_pos_pivot;
            window_pos.x = (corner & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
            window_pos.y = (corner & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
            window_pos_pivot.x = (corner & 1) ? 1.0f : 0.0f;
            window_pos_pivot.y = (corner & 2) ? 1.0f : 0.0f;
            ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
            window_flags |= ImGuiWindowFlags_NoMove;
        }
        ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
        if (ImGui::Begin("矩阵工具"))
        {
            float matrix[16];
            memset(matrix, 0, 16);
            XY_Read(addr.Matrix, matrix, 16 * 4);
            string result;
            for (int i = 0; i < 16; i++) {
                //std::cout << matrix[i] << " ";
                result += std::to_string(matrix[i]) + " ";
            }
            ImGui::Text("%s", result.c_str());
            ImGui::Separator();
            if (ImGui::IsMousePosValid())
                ImGui::Text("触摸位置: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
            else
                ImGui::Text("触摸位置: <无效>");
        }
        ImGui::End();
    }



    void ShowDebugDumperWindow(){
        ImGui::Begin("DebugDump工具");
        // 渲染编辑框
        for (int i = 0; i < OffsetLevel; ++i) {
            ImGui::InputText((i == 0) ? "地址" : (std::to_string(i) + "层偏移").c_str(), inputText[i].data(), inputText[i].size() + 1); // 使用 size() + 1
            // 如果点击了编辑框，则显示虚拟键盘并获取焦点
            if (ImGui::IsItemClicked()) {
                activeEditBoxIndex = i; // 记录活动编辑框的索引
                showVirtualKeyboard = true;
                ImGui::SetKeyboardFocusHere(-1);
            }

            if (i==0){
                ImGui::SameLine();
                if (ImGui::Button("+")){
                    OffsetLevel++;
                }
                ImGui::SameLine();
                if (ImGui::Button("-")){
                    if (OffsetLevel>1)   OffsetLevel--;
                }
            }
        }



        if (ImGui::Button("确定")){
            uint64_t Address = 0;
            for (int i = 0; i < sizeof(inputText) / sizeof(inputText[0]); ++i) {
                if (inputText[i]!=""){
                    uint64_t inputValue;
                    if (inputText[i].find("BASE")!= std::string::npos){
                        inputValue = addr.libbase;
                    }else {
                        inputValue = std::stoi(inputText[i], nullptr, 16);// 第三个参数是进制，这里使用十六进制
                    }
                    if (i==0){
                        Address += inputValue;
                    }else{
                        Address=XY_GetAddr(Address+inputValue);
                    }
                }
            }

            for (size_t i = 0; i < 0x300; i+=4) {
                long int Tmp = XY_GetAddr(Address + i);
                string KlassName = GetClassName(Tmp);
                string outerName = GetOuterName(Tmp);
                printf("[%x](%lx) %s  %s\n",i,XY_GetAddr((Address + i)),KlassName.c_str(),outerName.c_str());
            }
        }
        // 渲染虚拟键盘
        if (showVirtualKeyboard) {
            RenderVirtualKeyboard();
        }


        ImGui::End();
    }

}
#endif //BIGWHITETOOL_MENU_H
