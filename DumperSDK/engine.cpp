#include "engine.h"
#include "Mem.h"
#include "generic.h"
#include "file.h"



UE_UClass* UE_UObject::GetClass()
{
	return Read<UE_UClass*>(this + Offsets.UObject.Class);
}

UE_UObject* UE_UObject::GetOuter()
{
	return Read<UE_UClass*>(this + Offsets.UObject.Outer);
}

string UE_UObject::GetName()
{
	uint32_t NameId = Read<uint32_t>(this + Offsets.UObject.Name);
	return NamePoolData->GetName(NameId);
}

string UE_UObject::GetCppName()
{
	string Name = GetName();

	if (Name == "" || Name == "None")
		return string();

	for (UE_UStruct* c = this->Cast<UE_UStruct*>(); c; c = c->GetSuper())
	{
		if (c > (void*)GameBase)
			break;

		if (c == UE_AActor::StaticClass())
			return "A" + Name;

		if (c == UE_UObject::StaticClass())
			return "U" + Name;

	}

	return "F" + Name;
}

string UE_UObject::GetFullName()
{
	string temp = GetName();

	if (temp == "" || temp == "None")
		return string();

	for (auto* Outer = GetOuter(); Outer; Outer = Outer->GetOuter())
	{
		temp = Outer->GetName() + '.' + temp;
	}

	return GetClass()->GetName() + "  " + temp;
}

UE_UObject* UE_UObject::StaticClass()
{
	static UE_UObject* cmp = ObjObjects->FindObject("Class  /Script/CoreUObject.Object");
	return cmp;
}

UE_UObject* UE_AActor::StaticClass()
{
	static UE_UObject* cmp = ObjObjects->FindObject("Class  /Script/Engine.Actor");
	return cmp;
}

Tarray UE_UEnum::GetNames()
{
	return Read<Tarray>(this + Offsets.UEnum.Names);
}

UE_UObject* UE_UEnum::StaticClass()
{
	static UE_UObject* cmp = ObjObjects->FindObject("Class  /Script/CoreUObject.Enum");
	return cmp;
}

UE_UObject* UE_UClass::StaticClass()
{
	static UE_UObject* cmp = ObjObjects->FindObject("Class  /Script/CoreUObject.Class");
	return cmp;
}

uint64_t UE_UFunction::GetFunc()
{
	return Read<uint64_t>(this + Offsets.UFunction.Func);
}

UE_UObject* UE_UFunction::StaticClass()
{
	static UE_UObject* cmp = ObjObjects->FindObject("Class  /Script/CoreUObject.Function");
	return cmp;
}

UE_UObject* UE_UScriptStruct::StaticClass()
{
	static UE_UObject* cmp = ObjObjects->FindObject("Class  /Script/CoreUObject.ScriptStruct");
	return cmp;
}

int UE_UStruct::GetSize()
{
	return Read<int>(this + Offsets.UStruct.PropertiesSize);
}

UE_UStruct* UE_UStruct::GetSuper()
{
	return Read<UE_UStruct*>(this + Offsets.UStruct.SuperStruct);
}

UE_FField* UE_UStruct::GetChildProperties()
{
	return Read<UE_FField*>(this + Offsets.UStruct.ChildProperties);
}

uint32_t UE_UStruct::GetPropertiesSize()
{
	return Read<uint32_t>(this + Offsets.UStruct.PropertiesSize);
}

int UE_FField::GetSize()
{
	return Read<int>(this + Offsets.FProperty.ElementSize);
}

int UE_FField::GetOffset()
{
	return Read<int>(this + Offsets.FProperty.Offset);
}

string UE_FField::GetName()
{
	uint32_t NameId = Read<uint32_t>(this + Offsets.FField.Name);
	return NamePoolData->GetName(NameId);
}

string UE_FField::GetType()
{
	uint32_t NameId = Read<uint32_t>(Read<uint32_t*>(this + Offsets.FField.Class));
	string Name = NamePoolData->GetName(NameId);

	if (Name == "NameProperty")			{ return "FName"; };
	if (Name == "StrProperty")			{ return "FString"; };
	if (Name == "TextProperty")			{ return "FText"; }
	if (Name == "Int8Property")			{ return "int8_t"; };
	if (Name == "FloatProperty")		{ return "float"; };
	if (Name == "DoubleProperty")		{ return "double"; };
	if (Name == "Int16Property")		{ return "int16_t"; };
	if (Name == "IntProperty")			{ return "int32_t";};
	if (Name == "Int64Property")		{ return "int64_t"; };
	if (Name == "UInt16Property")		{ return "uint16_t"; };
	if (Name == "UInt32Property")		{ return "uint32_t"; };
	if (Name == "UInt64Property")		{ return "uint64_t"; };
	if (Name == "DelegateProperty")		{ return "FDelegate"; };
	if (Name == "SoftClassProperty")	{ return "TSoftClassPtr<UObject>"; };
	if (Name == "MulticastDelegateProperty")		{ return "FMulticastDelegate"; };
	if (Name == "MulticastSparseDelegateProperty")	{ return "FMulticastSparseDelegate"; };
	if (Name == "MulticastInlineDelegateProperty")	{ return "FMulticastInlineDelegate"; };

	if (Name == "MapProperty")			{ return Cast<UE_FMapProperty*>()->GetTypeStr(); };
	if (Name == "SetProperty")			{ return Cast<UE_FSetProperty*>()->GetTypeStr(); };
	if (Name == "EnumProperty")			{ return Cast<UE_FEnumProperty*>()->GetTypeStr(); }
	if (Name == "BoolProperty")			{ return Cast<UE_FBoolProperty*>()->GetTypeStr(); };
	if (Name == "ByteProperty")			{ return Cast<UE_FByteProperty*>()->GetTypeStr(); };
	if (Name == "ClassProperty")		{ return Cast<UE_FClassProperty*>()->GetTypeStr(); };
	if (Name == "StructProperty")		{ return Cast<UE_FStructProperty*>()->GetTypeStr(); };
	if (Name == "InterfaceProperty")	{ return Cast<UE_FInterfaceProperty*>()->GetTypeStr(); };
	if (Name == "ObjectProperty")	    { return Cast<UE_FObjectPropertyBase*>()->GetTypeStr(); };
	if (Name == "ArrayProperty")		{ return Cast<UE_FArrayProperty*>()->GetTypeStr(); };
	if (Name == "WeakObjectProperty")	{ return "TWeakObjectPtr<" + Cast<UE_FStructProperty*>()->GetTypeStr() + ">"; };
	if (Name == "SoftObjectProperty")	{ return "TSoftObjectPtr<" + Cast<UE_FObjectPropertyBase*>()->GetUEObject()->GetCppName() + ">"; };

	return GetUEObject()->GetCppName() + "*";
}

UE_FField* UE_FField::GetNext()
{
	return Read<UE_FField*>(this + Offsets.FField.Next);
}

UE_UObject* UE_FField::GetUEObject()
{
	return Read<UE_UObject*>(this + Offsets.FProperty.Size);
}

string UE_FMapProperty::GetTypeStr()
{
	UE_FField* KeyProp = Read<UE_FField*>(this + Offsets.FProperty.Size);
	UE_FField* ValueProp = Read<UE_FField*>(this + Offsets.FProperty.Size + 0x8);

	return "TMap<" + KeyProp->GetType() + "," + ValueProp->GetType() + ">";
}

string UE_FStructProperty::GetTypeStr()
{
	return GetUEObject()->GetCppName();
}

string UE_FByteProperty::GetTypeStr()
{
	UE_UObject* obj = GetUEObject();
	if (obj) return "enum class " + obj->GetName();
	return "char";
}

string UE_FBoolProperty::GetTypeStr()
{
	if (Read<uint8_t>(this + Offsets.FProperty.Size + 3) == 0xFF)
		return "bool";

	return "char";
}

string UE_FObjectPropertyBase::GetTypeStr()
{
	return GetUEObject()->GetCppName() + "*";
}

string UE_FClassProperty::GetTypeStr()
{
	UE_UObject* obj = Read<UE_UObject*>(this + Offsets.FProperty.Size + 8);
	return obj->GetCppName() + "*";
}

string UE_FSetProperty::GetTypeStr()
{
	return "TSet<" + GetUEObject()->Cast<UE_FField*>()->GetType() + ">";
}

string UE_FArrayProperty::GetTypeStr()
{
	return "TArray<" + GetUEObject()->Cast<UE_FField*>()->GetType() + ">";
}

string UE_FEnumProperty::GetTypeStr()
{
	return "enum class " + Read<UE_UObject*>(this + Offsets.FProperty.Size + 8)->GetName();
}

string UE_FInterfaceProperty::GetTypeStr()
{
	return "TScriptInterface<I" + GetUEObject()->GetName() + ">";
}

void UE_UEnum::Generate()
{
    static FILE* EnumName = fopen("/storage/emulated/0/A_BigWhiteTool/SDK/Enum.cpp", "w+");
    string Type = "uint8_t";
	string FullName = "// " + GetFullName() + "\n";
	string ClassName = "enum class " + GetName() + " : ";

	string Body;
	uint32_t Max = 0;
	Tarray Names = GetNames();
	for (size_t i = 0; i < Names.Count; i++)
	{
		uint32_t Value = Read<uint32_t>(Names.Data + i * Offsets.UEnum.Size + Offsets.UEnum.Value);
		uint32_t NameId = Read<uint32_t>(Names.Data + i * Offsets.UEnum.Size + Offsets.UEnum.Name);
		string Name = "    " + NamePoolData->GetName(NameId);

		size_t Pos = Name.rfind(':');
		if (Pos != std::string::npos)
			Name = "    " + Name.substr(Pos + 1);

		if (Value > Max)
			Max = Value;

		if (Name.size() < 50)
		{
			size_t size = 50 - Name.size();
			for (size_t i = 0; i < size; i++)
			{
				Name += " ";
			}
		}

		Name += " = " + to_string(Value);
		if (i != Names.Count - 1)
			Name += ",\n";
		else
			Name += "\n";

		Body += Name;
	}

	if (Max > 256)
		Type = "uint32_t";

    fprintf(EnumName,"%s",(FullName + ClassName + Type + "\n{\n" + Body + "\n};\n\n\n").c_str());
}

void UE_UClass::Generate()
{
    static FILE* UClass = fopen("/storage/emulated/0/A_BigWhiteTool/SDK/Class.cpp", "w+");
    char buf[64]{ 0 };
	sprintf(buf, "Class Size::0x%.4X\n", GetPropertiesSize());

	string FullName = "// " + GetFullName() + "\n";

	string ClassSize = "// ";
	ClassSize += buf;
	string ClassName = "class " + GetCppName();

	int Pos = 0;
	UE_UStruct* SuperStruct = GetSuper();
	if (SuperStruct)
	{
		Pos = SuperStruct->GetPropertiesSize();
		ClassName += " : public " + SuperStruct->GetCppName();
	}
	ClassName += "\n{\n";

	string Body;
	for (UE_FField* temp = GetChildProperties(); temp; temp = temp->GetNext())
	{
		string Name = "    " + temp->GetType();
		if (Name.size() < 50)
		{
			size_t size = 50 - Name.size();
			for (size_t i = 0; i < size; i++)
			{
				Name += " ";
			}
		}

		Name += temp->GetName() + ";";
		if (Name.size() < 112)
		{
			size_t size = 112 - Name.size();
			for (size_t i = 0; i < size; i++)
			{
				Name += " ";
			}
		}
		int Size = temp->GetSize();
		int Offset = temp->GetOffset();

		char buf[128]{ 0 };
		sprintf(buf, "// 0x%.4X(0x%.4X)\n", Offset, Size);
		Name += buf;

		if (Pos < Offset)
		{
			int diff = Offset - Pos;

			string sbuf = "    char";
			size_t size = 50 - sbuf.size();
			for (size_t i = 0; i < size; i++)
			{
				sbuf += " ";
			}
			
			char cbuf[128]{ 0 };
			sprintf(cbuf,"pa_%.4X[0x%.4X]", Pos,diff);
			sbuf += cbuf;

			size = 112 - sbuf.size();
			for (size_t i = 0; i < size; i++)
			{
				sbuf += " ";
			}

            memset(cbuf, 0, sizeof(cbuf));
			sprintf(cbuf, "// 0x%.4X(0x%.4X)\n", Pos,diff);
			sbuf += cbuf;

			Body += sbuf;
		}

		Pos = Size + Offset;
		Body += Name;
	}

	if ((int)GetPropertiesSize() > Pos)
	{
		int diff = (int)GetPropertiesSize() - Pos;

		string sbuf = "    char";
		size_t size = 50 - sbuf.size();
		for (size_t i = 0; i < size; i++)
		{
			sbuf += " ";
		}

		char cbuf[128]{ 0 };
		sprintf(cbuf, "pa_%.4X[0x%.4X]", Pos, diff);
		sbuf += cbuf;

		size = 112 - sbuf.size();
		for (size_t i = 0; i < size; i++)
		{
			sbuf += " ";
		}


        memset(cbuf, 0, sizeof(cbuf));
		sprintf(cbuf, "// 0x%.4X(0x%.4X)\n", Pos, diff);
		sbuf += cbuf;

		Body += sbuf;
	}
	fprintf(UClass,"%s",(FullName + ClassSize + ClassName + Body + "};\n\n\n").c_str());
}

void UE_UFunction::Generate()
{
    static FILE* Function = fopen("/storage/emulated/0/A_BigWhiteTool/SDK/Function.cpp", "w+");
    uint64_t Func = GetFunc();
	if (Func == 0)
		return;

	char buf[64]{ 0 };
	sprintf(buf, "// Offset::0x%X;\n", (uint32_t)(Func - GameBase));
	string Offset = buf;

	string Ret = "void ";
	string FullName = "// " + GetFullName() + "\n";
	string FuncName = GetOuter()->GetCppName() + "::" + GetName();
	if (FullName.rfind("__Delegate") != string::npos)
		return;

	string Body;
	for (UE_FField* temp = GetChildProperties(); temp; temp = temp->GetNext())
	{
		string Name = temp->GetName();
		string Type = temp->GetType();

		if (Name == "ReturnValue")
		{
			Ret = Type + " ";
			break;
		}

		Body += Type + " " + Name;
		if (temp->GetNext() && temp->GetNext()->GetName() != "ReturnValue")
			Body += ",";
	}
    fprintf(Function,"%s",(FullName + Offset + Ret + FuncName + "(" + Body + ");\n\n\n").c_str());
}

void UE_UScriptStruct::Generate()
{
    static FILE* Struct = fopen("/storage/emulated/0/A_BigWhiteTool/SDK/Struct.cpp", "w+");
    char buf[64]{ 0 };
	sprintf(buf, "Struct Size::0x%.4X\n", GetPropertiesSize());

	string FullName = "// " + GetFullName() + "\n";
	string ClassSize = "// ";
	ClassSize += buf;
	string ClassName = "struct " + GetCppName();

	int Pos = 0;
	UE_UStruct* SuperStruct = GetSuper();
	if (SuperStruct)
	{
		Pos = SuperStruct->GetPropertiesSize();
		ClassName += " : public " + SuperStruct->GetCppName();
	}
	ClassName += "\n{\n";


	string Body;
	for (UE_FField* temp = GetChildProperties(); temp; temp = temp->GetNext())
	{
		string Name = "    " + temp->GetType();
		if (Name.size() < 50)
		{
			size_t size = 50 - Name.size();
			for (size_t i = 0; i < size; i++)
			{
				Name += " ";
			}
		}

		Name += temp->GetName() + ";";
		if (Name.size() < 112)
		{
			size_t size = 112 - Name.size();
			for (size_t i = 0; i < size; i++)
			{
				Name += " ";
			}
		}

		int Size = temp->GetSize();
		int Offset = temp->GetOffset();

		char buf[128]{0};
		sprintf(buf,"// 0x%.4X(0x%.4X)\n", temp->GetOffset(),temp->GetSize());
		Name += buf;

		if (Pos < Offset)
		{
			int diff = Offset - Pos;

			string sbuf = "    char";
			size_t size = 50 - sbuf.size();
			for (size_t i = 0; i < size; i++)
			{
				sbuf += " ";
			}

			char cbuf[128]{ 0 };
			sprintf(cbuf, "pa_%.4X[0x%.4X]", Pos, diff);
			sbuf += cbuf;

			size = 112 - sbuf.size();
			for (size_t i = 0; i < size; i++)
			{
				sbuf += " ";
			}

            // ʹ��memset������cbuf����
            memset(cbuf, 0, sizeof(cbuf));
			sprintf(cbuf, "// 0x%.4X(0x%.4X)\n", Pos, diff);
			sbuf += cbuf;

			Body += sbuf;
		}

		Pos = Size + Offset;

		Body += Name;
	}

	if ((int)GetPropertiesSize() > Pos)
	{
		int diff = (int)GetPropertiesSize() - Pos;

		string sbuf = "    char";
		size_t size = 50 - sbuf.size();
		for (size_t i = 0; i < size; i++)
		{
			sbuf += " ";
		}

		char cbuf[128]{ 0 };
		sprintf(cbuf, "pa_%.4X[0x%.4X]", Pos, diff);
		sbuf += cbuf;

		size = 112 - sbuf.size();
		for (size_t i = 0; i < size; i++)
		{
			sbuf += " ";
		}

        // ʹ��memset������cbuf����
        memset(cbuf, 0, sizeof(cbuf));
		sprintf(cbuf, "// 0x%.4X(0x%.4X)\n", Pos, diff);
		sbuf += cbuf;

		Body += sbuf;
	}

	fprintf(Struct,"%s",(FullName + ClassSize + ClassName + Body + "};\n\n\n").c_str());
}

