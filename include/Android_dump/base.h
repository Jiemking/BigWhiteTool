#ifndef BASE_H
#define BASE_H

#include <iostream>
#include <string>

using namespace std;

struct _Offsets {
    struct {
        uint16_t Objects = 0;
        uint16_t NumChunks = 0x1C;
        uint16_t NumElements = 0x14;
    }TUObjectArray;
    struct {
        uint16_t Size = 0x18;
        uint16_t Object = 0x0;
    } FUObjectItem;
    struct {
        uint16_t Class = 0x10; // Offset to UClass pointer (UClass* ClassPrivate)
        uint16_t Name = 0x18;  // Offset to FName structure
        uint16_t Outer = 0x20; // (UObject* OuterPrivate)
    } UObject;
    struct {
        uint16_t SuperStruct = 0x40;
        uint16_t ChildProperties = 0x50;//0x28是ID
        uint16_t PropertiesSize = 0x58;
    } UStruct;
    struct {
        uint16_t Name = 0x0;
        uint16_t Value = 0x8;
        uint16_t Size = 0x10;
        uint16_t Names = 0x40;
    } UEnum;
    struct {
        uint16_t Func = 0xD8;
    } UFunction;
    struct {
        uint16_t Class = 0x8;
        uint16_t Next = 0x20;
        uint16_t Name = 0x28;
    } FField;
    struct {
        uint16_t ElementSize = 0x38;
        uint16_t Offset = 0x4C;
        uint16_t Size = 0x80;
    } FProperty;
};

static _Offsets Offsets;
#endif // !BASE_H
