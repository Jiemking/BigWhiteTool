#include "generic.h"
#include "Android_Read/Android_Read.h"

#include<codecvt>


uint64_t GameBase = 0;
uint64_t AddrGObject = 0;
uint64_t AddrGNames = 0;
bool isUE423 = true;
FNamePool* NamePoolData = NULL;
TUObjectArray* ObjObjects = NULL;

#define _WORD  uint16_t
#define WORD1(x)   (*((_WORD*)&(x)+1))

string FNamePool::GetName(uint32_t index)
{
    unsigned int Block = index >> 16;
    unsigned short int Offset = index & 65535;

    uintptr_t FNamePool = AddrGNames;

    auto NamePoolChunk = XY_TRead<uintptr_t>(FNamePool+0x40 + (Block * 0x8));

    uintptr_t FNameEntry = NamePoolChunk + (0x2 * Offset);

    auto FNameEntryHeader = XY_TRead<short int>(FNameEntry);
    uintptr_t StrPtr = FNameEntry + 0x2;

    int StrLength = FNameEntryHeader >> 6;
    if (StrLength > 0 && StrLength < 250) {
        string name(StrLength, '\0');
        ReadAddr(StrPtr, name.data(), StrLength * sizeof(char));
        name.shrink_to_fit();
        return name;
    } else {
        return "None";
    }
}
string FNamePool::GetName_Old(uint32_t index)
{
    uintptr_t G_Names = GetAddr(AddrGNames);
    int Id = (int)(index / (int)0x4000);
    int Idtemp = (int)(index % (int)0x4000);
    auto NamePtr = GetAddr((G_Names + Id * 8));
    auto Name = GetAddr((NamePtr + 8 * Idtemp));
    char name[0x100] = { 0 };
    if (ReadAddr((Name + 0xC), name, 0x100)) //0xC需要手动推算，默认是0x10
    {
        return name;
    }
    return {};
}

uint32_t TUObjectArray::GetNumChunks()
{
    return XY_TRead<uint32_t>(this + Offsets.TUObjectArray.NumChunks);
}

uint32_t TUObjectArray::GetNumElements()
{
    return XY_TRead<uint32_t>(this + Offsets.TUObjectArray.NumElements);
}

UE_UObject* TUObjectArray::GetObjectFormId(size_t Id)
{
    size_t i = 0;
    for (; Id > 65536; i++)
    {
        Id -= 65536;
    }

    uint8_t* Chunks = XY_TRead<uint8_t*>(this) + i * 8;
    uint8_t* Item = XY_TRead<uint8_t*>(Chunks) + Offsets.FUObjectItem.Size * Id;

    return XY_TRead<UE_UObject*>(Item + Offsets.FUObjectItem.Object);
}

UE_UObject* TUObjectArray::FindObject(string FullName)
{
    for (size_t i = 0; i < GetNumElements(); i++)
    {
        UE_UObject* Object = GetObjectFormId(i);
/*        cout << Object->GetFullName() << endl;*/
        if (Object && Object->GetFullName() == FullName)
            return Object;
    }

    return NULL;
}

