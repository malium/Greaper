/***********************************************************************************
*   Copyright 2021 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_REFLECTION_REFLECTED_PLAIN_FIELD_H
#define CORE_REFLECTION_REFLECTED_PLAIN_FIELD_H 1

#include "../CorePrerequisites.h"
#include "ReflectedField.h"

namespace greaper
{
	struct IReflectedPlainField : IReflectedField
	{
		virtual ~IReflectedPlainField() = default;

		virtual ReflectedTypeID_t GetTypeID();

		virtual ReflectedSize_t GetDynamicSize(IReflectedType* baseType, void* object);

		virtual ReflectedSize_t GetContainerDynamicSize(IReflectedType* baseType, void* object, int64 containerIdx);

		virtual void ToStream(IReflectedType* baseType, void* object, IStream& stream) = 0;
		
		virtual void ContainerElemToStream(IReflectedType* baseType, void* object, int64 containerIdx, IStream& stream) = 0;

		virtual void FromStream(IReflectedType* baseType, void* object, IStream& stream) = 0;

		virtual void ContainerElemFromStream(IReflectedType* baseType, void* object, int64 containerIdx, IStream& stream) = 0;
	};

	template<class InterfaceType, class DataType, class ObjectType>
	struct ReflectedPlainField : IReflectedPlainField
	{
		using SingleGetter_t = DataType & (InterfaceType::*)(ObjectType*);
		using SingleSetter_t = void (InterfaceType::*)(ObjectType*, DataType&);
		using ContainerGetter_t = DataType & (InterfaceType::*)(ObjectType*, int64);
		using ContainerSetter_t = void (InterfaceType::*)(ObjectType*, int64, DataType&);
		using ContainerGetSize_t = int64(InterfaceType::*)(ObjectType*);
		using ContainerSetSize_t = void (InterfaceType::*)(ObjectType*, int64);

		void InitSingle(String name, ReflectedFieldID_t fieldID, SingleGetter_t getter, SingleSetter_t setter);

		void InitContainer(String name, ReflectedFieldID_t fieldID, ContainerGetter_t getter, ContainerSetter_t setter,
			ContainerGetSize_t getSize, ContainerSetSize_t setSize);

		ReflectedTypeID_t GetTypeID()override;

		ReflectedSize_t GetDynamicSize(IReflectedType* baseType, void* object)override;

		ReflectedSize_t GetContainerDynamicSize(IReflectedType* baseType, void* object, int64 containerIdx)override;

		ReflectedSize_t GetArraySize(IReflectedType* baseType, void* object)override;

		void SetArraySize(IReflectedType* baseType, void* object, ReflectedSize_t size)override;

		void ToStream(IReflectedType* baseType, void* object, IStream& stream)override;

		void ContainerElemToStream(IReflectedType* baseType, void* object, int64 containerIdx, IStream& stream)override;

		void FromStream(IReflectedType* baseType, void* object, IStream& stream)override;

		void ContainerElemFromStream(IReflectedType* baseType, void* object, int64 containerIdx, IStream& stream)override;

	protected:
		union
		{
			struct
			{
				SingleGetter_t Getter;
				SingleSetter_t Setter;
			};
			struct
			{
				ContainerGetter_t  ContainerGetter;
				ContainerSetter_t  ContainerSetter;
				ContainerGetSize_t ContainerGetSize;
				ContainerSetSize_t ContainerSetSize;
			};
		};
	};
}

greaper::ReflectedTypeID_t greaper::IReflectedPlainField::GetTypeID()
{
	return ReflectedTypeID_t(-1);
}

inline greaper::ReflectedSize_t greaper::IReflectedPlainField::GetDynamicSize(IReflectedType* baseType, void* object)
{
	UNUSED(baseType); UNUSED(object);
	return ReflectedSize_t(-1);
}

inline greaper::ReflectedSize_t greaper::IReflectedPlainField::GetContainerDynamicSize(IReflectedType* baseType, void* object, int64 containerIdx)
{
	UNUSED(baseType); UNUSED(object); UNUSED(containerIdx);
	return ReflectedSize_t(-1);
}


template<class InterfaceType, class DataType, class ObjectType>
inline void greaper::ReflectedPlainField<InterfaceType, DataType, ObjectType>::InitSingle(String name, ReflectedFieldID_t fieldID,
	SingleGetter_t getter, SingleSetter_t setter)
{
	static_asset(ReflectedPlainType<DataType>::ID > 0, "Type has no ReflectedTypeID");

	const ReflectedSize_t dynamicSize = ReflectedPlainType<DataType>::GetSize(DataType());
	const bool hasDynamicSize = ReflectedPlainType<DataType>::HasDynamicSize != 0;
	const ReflectedTypeID_t typeID = ReflectedPlainType<DataType>::ID;
		
	VerifyNot(!hasDynamicSize && (sizeof(DataType) > 255),
		"Trying to create a plain RTTI field with size larger than 255. In order to use larger sizes "
		"for plain types please specialize RTTIPlainType, set hasDynamicSize to true.")
	
	Getter = getter;
	Setter = setter;

	Init(std::move(name), ReflectedFieldInfo
		{
			ReflectedFieldType_t::Plain,
			typeID,
			fieldID,
			(ReflectedSize_t)sizeof(DataType),
			dynamicSize
		});
}

template<class InterfaceType, class DataType, class ObjectType>
inline void greaper::ReflectedPlainField<InterfaceType, DataType, ObjectType>::InitContainer(String name, ReflectedFieldID_t fieldID,
	ContainerGetter_t getter, ContainerSetter_t setter, ContainerGetSize_t getSize, ContainerSetSize_t setSize)
{
	static_asset(ReflectedPlainType<DataType>::ID > 0, "Type has no ReflectedTypeID");

	const ReflectedSize_t dynamicSize = ReflectedPlainType<DataType>::GetSize(DataType());
	const bool hasDynamicSize = ReflectedPlainType<DataType>::HasDynamicSize != 0;
	const ReflectedTypeID_t typeID = ReflectedPlainType<DataType>::ID;

	VerifyNot(!hasDynamicSize && (sizeof(DataType) > 255),
		"Trying to create a plain RTTI field with size larger than 255. In order to use larger sizes "
		"for plain types please specialize RTTIPlainType, set hasDynamicSize to true.");

	ContainerGetter = getter;
	ContainerSetter = setter;
	ContainerGetSize = getSize;
	ContainerSetSize = setSize;

	Init(std::move(name), ReflectedFieldInfo
		{
			ReflectedFieldType_t::Plain,
			typeID,
			fieldID,
			(ReflectedSize_t)sizeof(DataType),
			dynamicSize
		});
}

template<class InterfaceType, class DataType, class ObjectType>
inline greaper::ReflectedTypeID_t greaper::ReflectedPlainField<InterfaceType, DataType, ObjectType>::GetTypeID()
{
	return ReflectedPlainType<DataType>::ID;
}

template<class InterfaceType, class DataType, class ObjectType>
inline greaper::ReflectedSize_t greaper::ReflectedPlainField<InterfaceType, DataType, ObjectType>::GetDynamicSize(IReflectedType* baseType, void* object)
{
	VerifyIsArray(false);

	InterfaceType* interface = static_cast<InterfaceType*>(baseType);
	ObjectType* obj = static_cast<ObjectType*>(object);

	DataType& value = (interface->*Getter)(obj);

	return ReflectedPlainType<DataType>::GetSize(value);
}

template<class InterfaceType, class DataType, class ObjectType>
inline greaper::ReflectedSize_t
greaper::ReflectedPlainField<InterfaceType, DataType, ObjectType>::GetContainerDynamicSize(IReflectedType* baseType, void* object, int64 containerIdx)
{
	VerifyIsArray(true);
	
	InterfaceType* interface = static_cast<InterfaceType*>(baseType);
	ObjectType* obj = static_cast<ObjectType*>(object);

	DataType& value = (interface->*ContainerGetter)(obj, containerIdx);

	return ReflectedPlainType<DataType>::GetSize(value);
}

template<class InterfaceType, class DataType, class ObjectType>
inline greaper::ReflectedSize_t
greaper::ReflectedPlainField<InterfaceType, DataType, ObjectType>::GetArraySize(IReflectedType* baseType, void* object)
{
	VerifyIsArray(true);

	InterfaceType* interface = static_cast<InterfaceType*>(baseType);
	ObjectType* obj = static_cast<ObjectType*>(object);

	return (interface->*ContainerGetSize)(obj);
}

template<class InterfaceType, class DataType, class ObjectType>
inline void greaper::ReflectedPlainField<InterfaceType, DataType, ObjectType>::SetArraySize(IReflectedType* baseType, void* object, ReflectedSize_t size)
{
	VerifyIsArray(true);

	InterfaceType* interface = static_cast<InterfaceType*>(baseType);
	ObjectType* obj = static_cast<ObjectType*>(object);

	(interface->*ContainerSetSize)(obj, size);
}

template<class InterfaceType, class DataType, class ObjectType>
inline void greaper::ReflectedPlainField<InterfaceType, DataType, ObjectType>::ToStream(IReflectedType* baseType, void* object, IStream& stream)
{
	VerifyIsArray(false);

	InterfaceType* interface = static_cast<InterfaceType*>(baseType);
	ObjectType* obj = static_cast<ObjectType*>(object);

	DataType& value = (interface->*Getter)(obj);

	ReflectedPlainType<DataType>::ToStream(value, stream);
}

template<class InterfaceType, class DataType, class ObjectType>
inline void greaper::ReflectedPlainField<InterfaceType, DataType, ObjectType>::ContainerElemToStream(
	IReflectedType* baseType, void* object, int64 containerIdx, IStream& stream)
{
	VerifyIsArray(true);

	InterfaceType* interface = static_cast<InterfaceType*>(baseType);
	ObjectType* obj = static_cast<ObjectType*>(object);

	DataType& value = (interface->*ContainerGetter)(obj, containerIdx);

	ReflectedPlainType<DataType>::ToStream(value, stream);
}

template<class InterfaceType, class DataType, class ObjectType>
inline void greaper::ReflectedPlainField<InterfaceType, DataType, ObjectType>::FromStream(IReflectedType* baseType, void* object, IStream& stream)
{
	VerifyIsArray(false);

	InterfaceType* interface = static_cast<InterfaceType*>(baseType);
	ObjectType* obj = static_cast<ObjectType*>(object);

	DataType value;
	ReflectedPlainType<DataType>::FromStream(value, stream);

	(interface->*Setter)(obj, value);
}

template<class InterfaceType, class DataType, class ObjectType>
inline void greaper::ReflectedPlainField<InterfaceType, DataType, ObjectType>::ContainerElemFromStream(
	IReflectedType* baseType, void* object, int64 containerIdx, IStream& stream)
{
	VerifyIsArray(true);

	InterfaceType* interface = static_cast<InterfaceType*>(baseType);
	ObjectType* obj = static_cast<ObjectType*>(object);

	DataType value;
	ReflectedPlainType<DataType>::FromStream(value, stream);

	(interface->*ContainerSetter)(obj, containerIdx, value);
}

#endif /* CORE_REFLECTION_REFLECTED_PLAIN_FIELD_H */