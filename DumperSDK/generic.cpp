#include "generic.h"
#include "Mem.h"

#include<codecvt>


uint64_t GameBase = 0;
uint64_t AddrGObject = 0;
uint64_t AddrGNames = 0;
FNamePool* NamePoolData = NULL;
TUObjectArray* ObjObjects = NULL;

#define _WORD  uint16_t
#define WORD1(x)   (*((_WORD*)&(x)+1))

string FNamePool::GetName(uint32_t index)
{
    unsigned int Block = index >> 16;
    unsigned short int Offset = index & 65535;

    uintptr_t FNamePool = AddrGNames;

    auto NamePoolChunk = Read<uintptr_t>(FNamePool+0x40 + (Block * 0x8));

    uintptr_t FNameEntry = NamePoolChunk + (0x2 * Offset);

    auto FNameEntryHeader = Read<short int>(FNameEntry);
    uintptr_t StrPtr = FNameEntry + 0x2;

    int StrLength = FNameEntryHeader >> 6;
    if (StrLength > 0 && StrLength < 250) {
        string name(StrLength, '\0');
        vm_readv((void*)StrPtr, name.data(), StrLength * sizeof(char));
        name.shrink_to_fit();
        return name;
    } else {
        return "None";
    }
}

uint32_t TUObjectArray::GetNumChunks()
{
    printf("%lx\n",this);
    cout << " "<<endl;
	return Read<uint32_t>(this + Offsets.TUObjectArray.NumChunks);
}

uint32_t TUObjectArray::GetNumElements()
{
	return Read<uint32_t>(this + Offsets.TUObjectArray.NumElements);
}

UE_UObject* TUObjectArray::GetObjectFormId(size_t Id)
{
	size_t i = 0;
	for (; Id > 65536; i++)
	{
		Id -= 65536;
	}

	uint8_t* Chunks = Read<uint8_t*>(this) + i * 8;
	uint8_t* Item = Read<uint8_t*>(Chunks) + Offsets.FUObjectItem.Size * Id;

	return Read<UE_UObject*>(Item + Offsets.FUObjectItem.Object);
}

UE_UObject* TUObjectArray::FindObject(string FullName)
{
	for (size_t i = 0; i < GetNumElements(); i++)
	{
		UE_UObject* Object = GetObjectFormId(i);

		if (Object && Object->GetFullName() == FullName)
			return Object;
	}

	return NULL;
}

