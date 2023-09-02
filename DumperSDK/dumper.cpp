#include <sys/stat.h>
#include "dumper.h"
#include "generic.h"
#include "file.h"
#include "engine.h"


void Dumper::Init(uint64_t Base,uint64_t GName, uint64_t GObject)
{
	GameBase = Base;
	NamePoolData = (FNamePool*)GName;
	ObjObjects = (TUObjectArray*)GObject;

    AddrGNames = GName;
    AddrGObject = GObject;
/*
    cout << NamePoolData->GetName(325177) << endl;*/

	if(NamePoolData->GetName(0) != "None")
		throw "GetName有误\nGetName(0)不为None";

	if (ObjObjects->GetNumChunks() == 0)
		throw "GObject块数为0";

	setlocale(LC_ALL, "");
    mkdir("/storage/emulated/0/A_BigWhiteTool/SDK",2770);

}

void Dumper::Dump()
{
/*    UE_UObject* Object = ObjObjects->GetObjectFormId(0x135c);

    if (Object){
        cout << "Object\n" << endl;
        if (Object->IsA<UE_UEnum>())
        {
            cout << "IsA:UE_UEnum\n" << endl;
            Object->Cast<UE_UEnum*>()->Generate();
        }
    }*/

    FILE* ObjectsName = fopen("/storage/emulated/0/A_BigWhiteTool/SDK/Object.cpp", "w+");
    fprintf(ObjectsName, "Object dump by BigWhite   @t.me/BigWhiteChat\n\n\n\n");
    fprintf(ObjectsName, "Count : %d\n\n", ObjObjects->GetNumElements());
	for (size_t i = 0; i < ObjObjects->GetNumElements(); i++)
	{
		UE_UObject* Object = ObjObjects->GetObjectFormId(i);

        if (!Object)
            continue;
		if (Object->IsA<UE_UEnum>())
		{
			Object->Cast<UE_UEnum*>()->Generate();
		}
		else if (Object->IsA<UE_UClass>())
		{
			Object->Cast<UE_UClass*>()->Generate();
		}
		else if (Object->IsA<UE_UFunction>())
		{
			Object->Cast<UE_UFunction*>()->Generate();
		}
		else if (Object->IsA<UE_UScriptStruct>())
		{
			Object->Cast<UE_UScriptStruct*>()->Generate();
		}
		fprintf(ObjectsName, "{%.6p}\t[%p]\t%s\n",i, Object, Object->GetFullName().c_str());
    }

    cout<< "OK!\n" <<endl;
}
