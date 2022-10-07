/***********************************************************************************
*   Copyright 2021 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_REFLECTION_REFLECTED_FIELD_H
#define CORE_REFLECTION_REFLECTED_FIELD_H 1

#include "../CorePrerequisites.h"
#include "../Enumeration.h"

ENUMERATION(ReflectedFieldType, Plain, DataChunk, Reference, ReferencePtr);

namespace greaper
{
	struct IReflectedType;

	struct ReflectedFieldInfo
	{
		ReflectedFieldType_t ReflectFieldType;
		ReflectedTypeID_t TypeID;
		bool IsArray;
		ReflectedFieldID_t FieldID;
		ReflectedSize_t StaticSize;
		ReflectedSize_t DynamicSize;

		constexpr ReflectedFieldInfo() noexcept;
	};

	struct IReflectedField
	{
		String Name;
		ReflectedFieldInfo Info;

		virtual ~IReflectedField() = default;

		virtual ReflectedSize_t GetArraySize(IReflectedType* baseType, void* object) = 0;

		virtual void SetArraySize(IReflectedType* baseType, void* object, ReflectedSize_t size) = 0;

		virtual void InitFieldInfo();

		void VerifyIsArray(bool isArray) noexcept;		

	protected:
		void Init(String name, const ReflectedFieldInfo& info) noexcept;
	};
}

constexpr greaper::ReflectedFieldInfo::ReflectedFieldInfo() noexcept
	:ReflectFieldType(ReflectedFieldType_t::Plain)
	, TypeID(0)
	, IsArray(false)
	, FieldID(0)
	, StaticSize(0)
	, DynamicSize(0)
{

}

void greaper::IReflectedField::InitFieldInfo()
{
	/* NO-OP */
}

void greaper::IReflectedField::VerifyIsArray(bool isArray) noexcept
{
#if !GREAPER_FRELEASE
	constexpr const char* gArrayText = "an array type";
	constexpr const char* gSingleText = "a single type";
	VerifyEqual(Info.IsArray, isArray,
		"Invalid field, wanted %s, received %s",
		isArray ? gArrayText : gSingleText,
		Info.IsArray ? gArrayText : gSingleText);
#endif
}

void greaper::IReflectedField::Init(greaper::String name, const greaper::ReflectedFieldInfo& info) noexcept
{
	Name = std::move(name);
	Info = info;
}

#endif /* CORE_REFLECTION_REFLECTED_FIELD_H */