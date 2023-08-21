#include <string>
#include <unordered_map>

std::unordered_map<string, std::string> UamoidMap = {
        {"BP_Condmilk_C", "炼奶"},
};

std::string UamoGetString(string aid)
{
    if (UamoidMap.find(aid) != UamoidMap.end()) {
        return UamoidMap[aid];
    }
    return "未知";  // 或者返回一个默认名称
}
