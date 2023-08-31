//
// Created by Ssage on 2022/3/18.
//
#include "main.h"
#include "Login.h"


/**
 * imgui测试
 */

int main(int argc, char *argv[]) {
    std::cout << "免责声明：" << std::endl << std::endl;
    std::cout << "此软件是一款用于逆向分析和测试游戏功能的工具，旨在帮助开发者理解游戏的内部工作原理以及进行相关测试。然而，请注意以下几点：" << std::endl << std::endl;

    std::cout << "1. 本软件的使用仅限于合法的研究和测试目的。任何将此软件用于非法活动、侵犯版权或其他违法行为的用户将承担相应的法律责任。" << std::endl;
    std::cout << "2. 本软件可能会影响游戏的正常运行，甚至可能导致游戏崩溃、数据损坏或其他不可预测的问题。使用本软件可能会对游戏的稳定性和功能性产生不利影响。" << std::endl;
    std::cout << "3. 本软件可能会涉及游戏厂商的知识产权和利益，因此使用本软件可能会引起游戏厂商的不满甚至法律纠纷。" << std::endl;
    std::cout << "4. 用户对使用本软件所产生的一切后果和风险负全部责任。软件开发者及其关联方不承担任何直接或间接的责任，包括但不限于因软件使用造成的损失、损害、法律纠纷或其他不良后果。" << std::endl;
    std::cout << "5. 本软件的发布和使用可能受到不同国家和地区的法律法规的限制。用户应当遵守当地的法律法规，确保在合法范围内使用本软件。" << std::endl << std::endl;

    std::cout << "请在使用本软件前详细阅读并理解上述免责声明。一旦您开始使用本软件，即代表您同意遵守上述声明，并愿意承担因此产生的一切风险和责任。" << std::endl;
    std::cout << "对于任何因使用本软件所引起的问题，我们将不负有任何法律或经济责任。" << std::endl << std::endl;

    std::cout << "（https://t.me/BigWhiteChat）" << std::endl;
    if (!Login()){
        cout << "登录失败"<<endl;
        return 0;
    }
    while (true){
        screen_config();
        if (displayInfo.orientation==1||displayInfo.orientation==3){
            break;
        }
        cout<<"等待横屏!"<<endl;
    }
    if (!initDraw()) {
        return -1;
    }
    Init_touch_config();

    printf("Pid is %d\n", getpid());
    bool flag = true;
    bool cshzt = false;
    bool ShowMenuWindows = true;

//获取输入信息
    int EventCount = GetEventCount();
    if (EventCount < 0) {
        printf("未找到输入设备\n");
        return -1;
    }
    int *fdArray = (int *)malloc(EventCount * sizeof(int));

    for (int i = 0; i < EventCount; i++) {
        char temp[128];
        sprintf(temp, "/dev/input/event%d", i);
        fdArray[i] = open(temp, O_RDWR | O_NONBLOCK);
    }
    input_event ev;

    while (flag) {
        // imgui画图开始前调用
        drawBegin();

        //根据输入信息判断菜单的显示
        for (int i = 0; i < EventCount; i++) {
            memset(&ev, 0, sizeof(ev));
            read(fdArray[i], &ev, sizeof(ev));
            if (ev.type == EV_KEY  && ev.value == 1) {
                if (ev.code==115){
                    ShowMenuWindows = true;
                }else if(ev.code==114){
                    ShowMenuWindows = false;
                }
            }
        }
        // 通过设置窗口大小和位置来影响主菜单栏的大小和位置

        if (ShowMenuWindows){

            if (ImGui::BeginMainMenuBar())
            {
                // 计算菜单宽度
                ImVec2 menuSize = ImVec2(100, 50); // 菜单的宽度和高度
                float menuBarWidth = ImGui::GetWindowWidth();
                float menuX = (menuBarWidth - menuSize.x) * 0.5f; // 计算居中位置
                ImGui::SetCursorPosX(menuX);

                if (readmode==0){
                    if (ImGui::BeginMenu("驱动选择"))
                    {
                        if (ImGui::MenuItem("普通模式")) {
                            readmode = 1;
                        }
                        if (ImGui::MenuItem("Biu驱动")) {
                            readmode = 2;
                        }
                        if (ImGui::MenuItem("XXX驱动")) {
                            readmode = 3;
                        }
                        ImGui::EndMenu();
                    }
                }

                if (readmode>0){
                    if (ImGui::BeginMenu("进程")) {
                        if (ImGui::MenuItem("ALL")){
                            tencent=0;
                        }
                        if (ImGui::MenuItem("过滤tencent")){
                            tencent=1;
                        }
                        std::vector<ProcessInfo> processes = GetTencentProcesses();
                        for (const ProcessInfo& process : processes) {
                            std::string jc = "PID: " + process.pid + " | Name: " + process.name;
                            if (ImGui::MenuItem(jc.c_str())) {
                                selectedPID = process.pid;
                                BigWhite_pid = std::stoi(selectedPID);//这里给BigWhite_pid赋值 是为了BigWhiteRead里面需要用
                                XY_pid = BigWhite_pid;
                                ProcessName=process.name;//将进程名保存为全局变量
                                ResetOffsets();//重新选择进程时 重置偏移结构体变量
                                GameInit();//这里是初始化游戏偏移
                                addr.GNames = addr.libbase + offsets.GNames;
                                cshzt = true;
                            }
                        }
                        ImGui::EndMenu();
                    }
                }
                if (cshzt){
                    DrawPlayer(ImGui::GetForegroundDrawList());
                    if (ImGui::BeginMenu("窗口"))
                    {
                        ImGui::MenuItem("ImguiDemo", NULL, &ShowDemoWindow);
                        ImGui::Separator();
                        ImGui::MenuItem("初始化数据", NULL, &ShowFindData);
                        ImGui::MenuItem("UE4Dumper", NULL, &ShowUE4Dumper);
                        ImGui::MenuItem("DebugDumper", NULL, &ShowDebugDumper);
                        ImGui::MenuItem("DeBUGGG", NULL, &ShowDebugMatrix);
                        if (ImGui::MenuItem("Debug", "CTRL+X")) {}
                        ImGui::EndMenu();
                    }
                }
                if (ImGui::BeginMenu("退出"))    exec_native_surface("killall BigWhiteTool");
                ImGui::EndMainMenuBar();
            }
            if (ShowDemoWindow) ImGui::ShowDemoWindow();
            if (ShowUE4Dumper) Menu::ShowUE4DumperWindow();
            if (ShowFindData) Menu::ShowFindDataWindow();
            if (ShowDebugMatrix) Menu::ShowDebugMatrixWindow();
            if (ShowDebugDumper) Menu::ShowDebugDumperWindow();
        }


        drawEnd();
        std::this_thread::sleep_for(1ms);
    }
    shutdown();
    touchEnd();
    return 0;
}

