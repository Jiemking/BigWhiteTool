#ifndef GENERIC_H
#define GENERIC_H

#include "base.h"
#include "engine.h"



struct FNameEntryHeader
{
public:
	uint16_t bIsWide : 1;
	static constexpr uint32_t ProbeHashBits = 5;
	uint16_t LowercaseProbeHash : ProbeHashBits;
	uint16_t Len : 10;
};


struct FNameEntry
{
public:
	FNameEntryHeader Header;
};

class FNamePool 
{
public:
	string GetName(uint32_t NameId);
};

class TUObjectArray 
{
public:
	uint32_t GetNumChunks();
	
	uint32_t GetNumElements();

	UE_UObject* GetObjectFormId(size_t Id);

	UE_UObject* FindObject(string FullName);
};

extern uint64_t GameBase;
extern uint64_t AddrGNames;
extern uint64_t AddrGObject;
extern FNamePool* NamePoolData;
extern TUObjectArray* ObjObjects;

#endif // !GENERIC_H
