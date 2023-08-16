//
// Created by Ssage on 2022/3/18.
//
#include "main.h"


/**
 * imgui测试
 */

int main(int argc, char *argv[]) {
    // 初始化imgui
    if (!initDraw(true)) {
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
                if (ImGui::BeginMenu("进程")) {
                    std::vector<ProcessInfo> processes = GetTencentProcesses();
                    for (const ProcessInfo& process : processes) {
                        std::string jc = "PID: " + process.pid + " | Name: " + process.name;
                        if (ImGui::MenuItem(jc.c_str())) {
                            selectedPID = process.pid;
                            BigWhite_pid = std::stoi(selectedPID);//这里给BigWhite_pid赋值是为了BigWhiteRead里面需要用
                            ProcessName=process.name;//将进程名保存为全局变量
                            ResetOffsets();//重新选择进程时 重置偏移结构体变量
                            GameInit(process.name);//这里是初始化游戏偏移
                            addr.GNames = addr.libbase + offsets.GNames;
/*                            printf("libbase:%lx  GNames:%lx",addr.libbase,offsets.GNames);
                            printf("addr.GNames:%lx ",addr.GNames);*/
                            cshzt = true;
                        }
                    }
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("窗口"))
                {
                    ImGui::MenuItem("ImguiDemo", NULL, &ShowDemoWindow);
                    ImGui::Separator();
                    ImGui::MenuItem("初始化数据", NULL, &ShowFindData);
                    ImGui::MenuItem("UE4Dumper", NULL, &ShowUE4Dumper);
                    if (ImGui::MenuItem("Cut", "CTRL+X")) {}
                    if (ImGui::MenuItem("Copy", "CTRL+C")) {}
                    if (ImGui::MenuItem("Paste", "CTRL+V")) {}
                    ImGui::EndMenu();
                }
                ImGui::EndMainMenuBar();
            }
        }



        if (cshzt){
            if (ProcessName.find("tencent.uam")){}
                addr.Uworld = BigWhite_GetPtr64(addr.libbase + offsets.Uworld);
/*                printf("%lx",addr.GNames);
                cout << "" << endl;*/

        }
        if (ShowDemoWindow) ImGui::ShowDemoWindow();//ImguiDemo菜单
        if (ShowUE4Dumper) Menu::ShowUE4DumperWindow();//ImguiDemo菜单
        if (ShowFindData) Menu::ShowFindDataWindow();//ImguiDemo菜单




        drawEnd();
        std::this_thread::sleep_for(1ms);
    }
    shutdown();
    touchEnd();
    return 0;
}

