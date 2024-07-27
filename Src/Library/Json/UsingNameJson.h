/**
* @file UsingNameJson.h
*/
#ifndef USINGNAMEJSON_H_INCLUDED
#define USINGNAMEJSON_H_INCLUDED

#include <string>
#include <vector>
#include <map>

// ------------------------
// �O���錾
// ------------------------

namespace nlohmann 
{
    inline namespace json_abi_v3_11_3 
    {
        template<typename T = void, typename SFINAE = void>
        struct adl_serializer;

        template<template<typename U, typename V, typename... Args> class ObjectType =
            std::map,
            template<typename U, typename... Args> class ArrayType = std::vector,
            class StringType = std::string, class BooleanType = bool,
            class NumberIntegerType = std::int64_t,
            class NumberUnsignedType = std::uint64_t,
            class NumberFloatType = double,
            template<typename U> class AllocatorType = std::allocator,
            template<typename T, typename SFINAE = void> class JSONSerializer =
            adl_serializer,
            class BinaryType = std::vector<std::uint8_t>, // cppcheck-suppress syntaxError
            class CustomBaseClass = void>
        class basic_json;

        template<class Key, class T, class IgnoredLess, class Allocator>
        struct ordered_map;

        using ordered_json = basic_json<nlohmann::ordered_map>;
    }
}

// ------------------------------
// �^�̕ʖ����`
// ------------------------------

using Json = nlohmann::ordered_json;

#endif // !USINGNAMEJSON_H_INCLUDED
