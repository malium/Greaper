/***********************************************************************************************************************
 *                                   Copyright 2025 Marcos Sánchez Torrent (@malium)                                   *
 *                                               All Rights Reserved                                                   *
 **********************************************************************************************************************/

#include "../GreaperCore/Public/CorePrerequisites.hpp"
#include "../GreaperCore/Public/Reflection/ComplexType.hpp"
#include "../GreaperCore/Public/Reflection/ContainerType.hpp"
#include "../GreaperCore/Public/MemoryStream.hpp"
#include <ostream>
#include <iostream>

namespace greaper
{
	struct SimpleStruct
	{
		int32 i32;
		float f32;
		String Name;
	};

	class ComplexClass
	{
	public:
		inline ComplexClass(int32 i = 15, String s = "hi", float f = 0.5f,
			Vector<SimpleStruct> vec = { SimpleStruct{25,7.25f,"value"} })
			:m_Int(i)
			,m_String(std::move(s))
			,m_Float(f),
			m_Vector(std::move(vec))
		{

		}

		~ComplexClass() = default;
		
		inline int32 get_int_value()const { return m_Int; }
		inline void set_int_value(int32 i) { m_Int = i; }

		inline const String& get_string_value()const { return m_String; }
		inline void set_string_value(const String& s) { m_String = s; }

		inline float get_float_value()const { return m_Float; }
		inline void set_float_value(float f) { m_Float = f; }

		inline const Vector<SimpleStruct>& get_vector_value()const { return m_Vector; }
		inline Vector<SimpleStruct>& get_vector_value() { return m_Vector; }

	private:
		int32 m_Int;
		String m_String;
		float m_Float;
		Vector<SimpleStruct> m_Vector;

		friend refl::ComplexType<ComplexClass>;
	};
}

namespace greaper::refl
{
	enum TestReflectedID : ReflectedTypeID_t
	{
		RTI_SimpleStruct = 1000,
		RTI_ComplexClass,
	};
}

CREATE_TYPEINFO_CNAME(greaper::SimpleStruct, greaper::refl::RTI_SimpleStruct, ComplexType, "SimpleStruct");
CREATE_TYPEINFO_CNAME(greaper::ComplexClass, greaper::refl::RTI_ComplexClass, ComplexType, "ComplexClass");

namespace greaper
{
	template<>
	const Vector<std::shared_ptr<refl::IField>> refl::ComplexType<SimpleStruct>::Fields = {
		std::make_shared<refl::TField<int32>>("i32"sv, 
			(refl::IField::GetValueFn)[](const void* obj) -> const void* { return &(((SimpleStruct*)obj)->i32); },
			(refl::IField::SetValueFn)[](void* obj, const void* value) -> 
				void { ((SimpleStruct*)obj)->i32 = *((const int32*)value); }
		),
		std::make_shared<refl::TField<float>>("f32"sv, 
			(refl::IField::GetValueFn)[](const void* obj) -> const void* { return &(((SimpleStruct*)obj)->f32); },
			(refl::IField::SetValueFn)[](void* obj, const void* value) -> 
				void { ((SimpleStruct*)obj)->f32 = *((const float*)value); }
		),
		std::make_shared<refl::TField<String>>("Name"sv, 
			(refl::IField::GetValueFn)[](const void* obj) -> const void* { return &(((SimpleStruct*)obj)->Name); },
			(refl::IField::SetValueFn)[](void* obj, const void* value) -> 
				void { ((SimpleStruct*)obj)->Name = *((const String*)value); }
		)
	};

	template<>
	const Vector<std::shared_ptr<refl::IField>> refl::ComplexType<ComplexClass>::Fields = {
		std::make_shared<TField<int32>>("Int"sv, 
			(refl::IField::GetValueFn)[](const void* obj) -> const void* { return &(((ComplexClass*)obj)->m_Int); },
			(refl::IField::SetValueFn)[](void* obj, const void* value) -> 
				void { ((ComplexClass*)obj)->m_Int = *((const int32*)value); }
		),
		std::make_shared<refl::TField<String>>("String"sv, 
			(refl::IField::GetValueFn)[](const void* obj) -> const void* { return &(((ComplexClass*)obj)->m_String); },
			(refl::IField::SetValueFn)[](void* obj, const void* value) -> 
				void { ((ComplexClass*)obj)->m_String = *((const String*)value); }
		),
		std::make_shared<refl::TField<float>>("Float"sv, 
			(refl::IField::GetValueFn)[](const void* obj) -> const void* { return &(((ComplexClass*)obj)->m_Float); },
			(refl::IField::SetValueFn)[](void* obj, const void* value) -> 
				void { ((ComplexClass*)obj)->m_Float = *((const float*)value); }
		),
		std::make_shared<refl::TField<Vector<SimpleStruct>>>("Vector"sv, 
			(refl::IField::GetValueFn)[](const void* obj) -> const void* { return &(((ComplexClass*)obj)->m_Vector); },
			(refl::IField::SetValueFn)[](void* obj, const void* value) -> 
				void { ((ComplexClass*)obj)->m_Vector = *((const Vector<SimpleStruct>*)value); }
		),
	};
}

constexpr auto CC_Name = "TestComplexClass"sv;

void ComplexTypeTest()
{
	using CC_RTI = greaper::refl::TypeInfo_t<greaper::ComplexClass>::Type;

	greaper::ComplexClass cc{};
	auto json_ret = CC_RTI::CreateJSON(cc, CC_Name);
	if (!json_ret.has_value())
	{
		std::cout << "ERROR" << json_ret.error() << std::endl;
		return;
	}
	auto json = json_ret.value();

	std::cout << cJSON_Print(json.get()) << std::endl;

	greaper::ComplexClass dd{0, "", 0.f, {}};

	auto json_ret2 = CC_RTI::FromJSON(dd, json.get(), CC_Name);
	if (!json_ret2.has_value())
	{
		std::cout << "ERROR" << json_ret2.error() << std::endl;
		return;
	}

	auto str_ret = CC_RTI::ToString(dd);
	if (!str_ret.has_value())
	{
		std::cout << "ERROR" << str_ret.error() << std::endl;
		return;
	}
	std::cout << str_ret.value() << std::endl;

	greaper::ComplexClass ee{0, "", 0.f, {}};
	greaper::MemoryStream ms {};

	auto stream_ret = CC_RTI::ToStream(cc, ms);
	if (!stream_ret.has_value())
	{
		std::cout << "ERROR" << stream_ret.error() << std::endl;
		return;
	}
	std::cout << "Written " << stream_ret.value() << "bytes" << std::endl;

	ms.Seek(0);
	auto stream_ret2 = CC_RTI::FromStream(ee, ms);
	if (!stream_ret2.has_value())
	{
		std::cout << "ERROR" << stream_ret2.error() << std::endl;
		return;
	}
	std::cout << "Read " << stream_ret2.value() << "bytes" << std::endl;

	auto str_ret2 = CC_RTI::ToString(ee);
	if (!str_ret2.has_value())
	{
		std::cout << "ERROR" << str_ret2.error() << std::endl;
		return;
	}
	std::cout << str_ret2.value() << std::endl;
}