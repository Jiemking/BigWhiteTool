#ifndef DUMPER_H
#define DUMPER_H

#include "base.h"
#include "Mem.h"
class Dumper
{
public:
	static Dumper& GetInstance() {
		static Dumper Instance;
		return Instance;
	}

	void Init(uint64_t Base, uint64_t GName, uint64_t GObject);

	void Dump();

private:
	Dumper() {};
	Dumper(Dumper&) {};
};

#endif // !DUMPER_H