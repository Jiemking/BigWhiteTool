#ifndef DUMPER_H
#define DUMPER_H
// 包含基础头文件和内存操作头文件
#include "base.h"
#include "Mem.h"
// Dumper类定义
class Dumper
{
public:
    // 获取Dumper实例
	static Dumper& GetInstance() {
		static Dumper Instance;
		return Instance;
	}
    // 初始化函数
	void Init(uint64_t Base, uint64_t GName, uint64_t GObject);
    // 转储函数
	void Dump();

private:
    // 私有构造函数
	Dumper() {};
    // 禁用复制构造函数
	Dumper(Dumper&) {};
};

#endif // !DUMPER_H