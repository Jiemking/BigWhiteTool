#ifndef ENGINE_H
#define ENGINE_H

#include "base.h"


class Tarray
{
public:
    uint8_t* Data;
	uint32_t Count;
	uint32_t Max;
};

class UE_UClass;
class UE_FField;

class UE_UObject 
{

public:
	UE_UClass* GetClass();
	UE_UObject* GetOuter();
	string GetName();
	string GetCppName();
	string GetFullName();

	static UE_UObject* StaticClass();

	template <typename Base> Base Cast() const { return Base(this); }

	template <typename T> bool IsA();
};

class UE_AActor : public UE_UObject {
public:
	static UE_UObject* StaticClass();
};


class UE_UField : public UE_UObject {
public:
};

class UE_UStruct : public UE_UField {
public:
	UE_UStruct* GetSuper();

	UE_FField* GetChildProperties();

	uint32_t GetPropertiesSize();

	int GetSize();

};

class UE_UFunction : public UE_UStruct {
public:

	uint64_t GetFunc();
	string GetFunctionFlags();

	void Generate();

	static UE_UObject* StaticClass();
};

class UE_UScriptStruct : public UE_UStruct {
public:

	void Generate();

	static UE_UObject* StaticClass();
};

class UE_UClass : public UE_UStruct {
public:

	void Generate();

	static UE_UObject* StaticClass();
};

class UE_UEnum : public UE_UField {
public:

	void Generate();

	Tarray GetNames();

	static UE_UObject* StaticClass();
};

class UE_FFieldClass 
{
public:
	string GetName();
};

class UE_FField 
{
public:
	UE_FField* GetNext();

	string GetName();

	string GetType();

	int GetSize();

	int GetOffset();

	UE_UObject* GetUEObject();

	template <typename Base> Base Cast() const { return Base(this); }
};

class UE_FProperty : public UE_FField {
public:
};

class UE_FStructProperty : public UE_FProperty {
public:

	string GetTypeStr();
};

class UE_FMapProperty : public UE_FProperty {
public:
	string GetTypeStr();
};

class UE_FObjectPropertyBase : public UE_FProperty {
public:
	string GetTypeStr();
};

class UE_FArrayProperty : public UE_FProperty {
public:
	string GetTypeStr();
};

class UE_FByteProperty : public UE_FProperty {
public:
	string GetTypeStr();
};

class UE_FBoolProperty : public UE_FProperty {
public:
	string GetTypeStr();
};

class UE_FEnumProperty : public UE_FProperty {
public:
	string GetTypeStr();
};

class UE_FClassProperty : public UE_FObjectPropertyBase {
public:
	string GetTypeStr();
};

class UE_FSetProperty : public UE_FProperty {
public:
	string GetTypeStr();
};

class UE_FInterfaceProperty : public UE_FProperty {
public:
	string GetTypeStr();
};

class UE_FFieldPathProperty : public UE_FProperty {
public:
	string GetTypeStr();
};


template <typename T> bool UE_UObject::IsA()
{
	UE_UObject* cmp = T::StaticClass();

	if (!cmp)
		return false;

	for (UE_UClass* super = GetClass(); super; super = super->GetSuper()->Cast<UE_UClass*>())
	{
		if (super == cmp)
			return true;
	}

	return false;
}

#endif // !ENGINE_H
