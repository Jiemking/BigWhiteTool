#include "engine.h"
#include "Android_Read/Android_Read.h"
#include "generic.h"



// 获取类对象
UE_UClass* UE_UObject::GetClass()
{
	return XY_TRead<UE_UClass*>(this + Offsets.UObject.Class);
}
// 获取外部对象
UE_UObject* UE_UObject::GetOuter()
{
	return XY_TRead<UE_UClass*>(this + Offsets.UObject.Outer);
}
// 获取对象名称
string UE_UObject::GetName()
{
	uint32_t NameId = XY_TRead<uint32_t>(this + Offsets.UObject.Name);
    if (isUE423){
        return NamePoolData->GetName(NameId);
    }else{
        return NamePoolData->GetName_Old(NameId);
    }

}
// 获取C++名称
string UE_UObject::GetCppName()
{
	string Name = GetName();

	if (Name == "" || Name == "None")
		return string();

	for (UE_UStruct* c = this->Cast<UE_UStruct*>(); c; c = c->GetSuper())
	{
        /*这个地方容易出问题*/
        if (c > (void*)GameBase)
            break;

		if (c == UE_AActor::StaticClass())
			return "A" + Name;

		if (c == UE_UObject::StaticClass())
			return "U" + Name;

	}

	return "F" + Name;
}
// 获取完整名称
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
// 获取静态类对象
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
// 获取枚举名称
Tarray UE_UEnum::GetNames()
{
	return XY_TRead<Tarray>(this + Offsets.UEnum.Names);
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
// 获取函数
uint64_t UE_UFunction::GetFunc()
{
	return XY_TRead<uint64_t>(this + Offsets.UFunction.Func);
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
// 获取结构大小
int UE_UStruct::GetSize()
{
	return XY_TRead<int>(this + Offsets.UStruct.PropertiesSize);
}
// 获取父结构
UE_UStruct* UE_UStruct::GetSuper()
{
	return XY_TRead<UE_UStruct*>(this + Offsets.UStruct.SuperStruct);
}
// 获取子属性
UE_FField* UE_UStruct::GetChildProperties()
{
	return XY_TRead<UE_FField*>(this + Offsets.UStruct.ChildProperties);
}
// 获取属性大小
uint32_t UE_UStruct::GetPropertiesSize()
{
	return XY_TRead<uint32_t>(this + Offsets.UStruct.PropertiesSize);
}
// 获取字段大小
int UE_FField::GetSize()
{
	return XY_TRead<int>(this + Offsets.FProperty.ElementSize);
}
// 获取字段偏移量
int UE_FField::GetOffset()
{
	return XY_TRead<int>(this + Offsets.FProperty.Offset);
}
// 获取字段名称
string UE_FField::GetName()
{
	uint32_t NameId = XY_TRead<uint32_t>(this + Offsets.FField.Name);
	return NamePoolData->GetName(NameId);
}
// 获取字段类型
string UE_FField::GetType()
{
	uint32_t NameId = XY_TRead<uint32_t>(XY_TRead<uint32_t*>(this + Offsets.FField.Class));
	string Name = NamePoolData->GetName(NameId);

	if (Name == "NameProperty")			{ return "FName"; }
	if (Name == "StrProperty")			{ return "FString"; }
	if (Name == "TextProperty")			{ return "FText"; }
	if (Name == "Int8Property")			{ return "int8_t"; }
	if (Name == "FloatProperty")		{ return "float"; }
	if (Name == "DoubleProperty")		{ return "double"; }
	if (Name == "Int16Property")		{ return "int16_t"; }
	if (Name == "IntProperty")			{ return "int32_t";}
	if (Name == "Int64Property")		{ return "int64_t"; }
	if (Name == "UInt16Property")		{ return "uint16_t"; }
	if (Name == "UInt32Property")		{ return "uint32_t"; }
	if (Name == "UInt64Property")		{ return "uint64_t"; }
	if (Name == "DelegateProperty")		{ return "FDelegate"; }
	if (Name == "SoftClassProperty")	{ return "TSoftClassPtr<UObject>"; }
	if (Name == "MulticastDelegateProperty")		{ return "FMulticastDelegate"; }
	if (Name == "MulticastSparseDelegateProperty")	{ return "FMulticastSparseDelegate"; }
	if (Name == "MulticastInlineDelegateProperty")	{ return "FMulticastInlineDelegate"; }

	if (Name == "MapProperty")			{ return Cast<UE_FMapProperty*>()->GetTypeStr(); }
	if (Name == "SetProperty")			{ return Cast<UE_FSetProperty*>()->GetTypeStr(); }
	if (Name == "EnumProperty")			{ return Cast<UE_FEnumProperty*>()->GetTypeStr(); }
	if (Name == "BoolProperty")			{ return Cast<UE_FBoolProperty*>()->GetTypeStr(); }
	if (Name == "ByteProperty")			{ return Cast<UE_FByteProperty*>()->GetTypeStr(); }
	if (Name == "ClassProperty")		{ return Cast<UE_FClassProperty*>()->GetTypeStr(); }
	if (Name == "StructProperty")		{ return Cast<UE_FStructProperty*>()->GetTypeStr(); }
	if (Name == "InterfaceProperty")	{ return Cast<UE_FInterfaceProperty*>()->GetTypeStr(); }
	if (Name == "ObjectProperty")	    { return Cast<UE_FObjectPropertyBase*>()->GetTypeStr(); }
	if (Name == "ArrayProperty")		{ return Cast<UE_FArrayProperty*>()->GetTypeStr(); }
	if (Name == "WeakObjectProperty")	{ return "TWeakObjectPtr<" + Cast<UE_FStructProperty*>()->GetTypeStr() + ">"; }
	if (Name == "SoftObjectProperty")	{ return "TSoftObjectPtr<" + Cast<UE_FObjectPropertyBase*>()->GetUEObject()->GetCppName() + ">"; }

	return GetUEObject()->GetCppName() + "*";
}
// 获取下一个字段
UE_FField* UE_FField::GetNext()
{
	return XY_TRead<UE_FField*>(this + Offsets.FField.Next);
}
// 获取UE对象
UE_UObject* UE_FField::GetUEObject()
{
	return XY_TRead<UE_UObject*>(this + Offsets.FProperty.Size);
}
// 获取Map类型字符串
string UE_FMapProperty::GetTypeStr()
{
	UE_FField* KeyProp = XY_TRead<UE_FField*>(this + Offsets.FProperty.Size);
	UE_FField* ValueProp = XY_TRead<UE_FField*>(this + Offsets.FProperty.Size + 0x8);

	return "TMap<" + KeyProp->GetType() + "," + ValueProp->GetType() + ">";
}
// 获取结构类型字符串
string UE_FStructProperty::GetTypeStr()
{
	return GetUEObject()->GetCppName();
}
// 获取字节类型字符串
string UE_FByteProperty::GetTypeStr()
{
	UE_UObject* obj = GetUEObject();
	if (obj) return "enum class " + obj->GetName();
	return "char";
}
// 获取布尔类型字符串
string UE_FBoolProperty::GetTypeStr()
{
	if (XY_TRead<uint8_t>(this + Offsets.FProperty.Size + 3) == 0xFF)
		return "bool";

	return "char";
}
// 获取对象基础类型字符串
string UE_FObjectPropertyBase::GetTypeStr()
{
	return GetUEObject()->GetCppName() + "*";
}
// 获取类属性类型字符串
string UE_FClassProperty::GetTypeStr()
{
	UE_UObject* obj = XY_TRead<UE_UObject*>(this + Offsets.FProperty.Size + 8);
	return obj->GetCppName() + "*";
}
// 获取集合类型字符串
string UE_FSetProperty::GetTypeStr()
{
	return "TSet<" + GetUEObject()->Cast<UE_FField*>()->GetType() + ">";
}
// 获取数组类型字符串
string UE_FArrayProperty::GetTypeStr()
{
	return "TArray<" + GetUEObject()->Cast<UE_FField*>()->GetType() + ">";
}
// 获取枚举属性类型字符串
string UE_FEnumProperty::GetTypeStr()
{
	return "enum class " + XY_TRead<UE_UObject*>(this + Offsets.FProperty.Size + 8)->GetName();
}
// 获取接口属性类型字符串
string UE_FInterfaceProperty::GetTypeStr()
{
	return "TScriptInterface<I" + GetUEObject()->GetName() + ">";
}
// 生成枚举类型的C++代码并写入文件
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
		uint32_t Value = XY_TRead<uint32_t>(Names.Data + i * Offsets.UEnum.Size + Offsets.UEnum.Value);
		uint32_t NameId = XY_TRead<uint32_t>(Names.Data + i * Offsets.UEnum.Size + Offsets.UEnum.Name);
		string Name = "    " + NamePoolData->GetName(NameId);

		size_t Pos = Name.rfind(':');
		if (Pos != std::string::npos)
			Name = "    " + Name.substr(Pos + 1);

		if (Value > Max)
			Max = Value;

		if (Name.size() < 50)
		{
			size_t size = 50 - Name.size();
			for (size_t j = 0; j < size; j++)
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
// 生成类的C++代码并写入文件
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

		char dest[128]{0 };
		sprintf(dest, "// 0x%.4X(0x%.4X)\n", Offset, Size);
		Name += dest;

		if (Pos < Offset)
		{
			int diff = Offset - Pos;

			string sbuf = "    char";
			size_t size = 50 - sbuf.size();
			for (size_t i = 0; i < size; i++)
			{
				sbuf += " ";
			}
			
			char dst[128]{0 };
			sprintf(dst, "pa_%.4X[0x%.4X]", Pos, diff);
			sbuf += dst;

			size = 112 - sbuf.size();
			for (size_t i = 0; i < size; i++)
			{
				sbuf += " ";
			}

            memset(dst, 0, sizeof(dst));
			sprintf(dst, "// 0x%.4X(0x%.4X)\n", Pos, diff);
			sbuf += dst;

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
// 生成函数的C++代码并写入文件
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
// 生成结构体的C++代码并写入文件
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

		char dest[128]{0};
		sprintf(dest, "// 0x%.4X(0x%.4X)\n", temp->GetOffset(), temp->GetSize());
		Name += dest;

		if (Pos < Offset)
		{
			int diff = Offset - Pos;

			string sbuf = "    char";
			size_t size = 50 - sbuf.size();
			for (size_t i = 0; i < size; i++)
			{
				sbuf += " ";
			}

			char dst[128]{0 };
			sprintf(dst, "pa_%.4X[0x%.4X]", Pos, diff);
			sbuf += dst;

			size = 112 - sbuf.size();
			for (size_t i = 0; i < size; i++)
			{
				sbuf += " ";
			}

            memset(dst, 0, sizeof(dst));
			sprintf(dst, "// 0x%.4X(0x%.4X)\n", Pos, diff);
			sbuf += dst;

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

	fprintf(Struct,"%s",(FullName + ClassSize + ClassName + Body + "};\n\n\n").c_str());
}

