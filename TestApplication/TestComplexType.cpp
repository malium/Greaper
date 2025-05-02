/***********************************************************************************************************************
 *                                   Copyright 2025 Marcos Sánchez Torrent (@malium)                                   *
 *                                               All Rights Reserved                                                   *
 **********************************************************************************************************************/

#include "../GreaperCore/Public/CorePrerequisites.hpp"
#include "../GreaperCore/Public/Reflection/ComplexType.hpp"
#include "../GreaperCore/Public/Reflection/ContainerType.hpp"
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
		inline ComplexClass(int32 i = 15, String s = "hi", float f = 0.5f, Vector<SimpleStruct> vec = { SimpleStruct{25,7.25f,"value"} })
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

	template<>
	struct TypeInfo<greaper::SimpleStruct>
	{
		static constexpr ReflectedTypeID_t ID = RTI_SimpleStruct;
		using Type = ComplexType<SimpleStruct>;
		static constexpr StringView Name = "SimpleStruct"sv;
	};
	template<>
	struct TypeInfo<greaper::ComplexClass>
	{
		static constexpr ReflectedTypeID_t ID = RTI_ComplexClass;
		using Type = ComplexType<ComplexClass>;
		static constexpr StringView Name = "ComplexClass"sv;
	};
	template<>
	const Vector<std::shared_ptr<IField>> ComplexType<SimpleStruct>::Fields = {
		std::make_shared<TField<int32>>("i32"sv, 
			(IField::GetValueFn)[](const void* obj) -> const void* { return &(((SimpleStruct*)obj)->i32); },
			(IField::SetValueFn)[](void* obj, const void* value) -> void { ((SimpleStruct*)obj)->i32 = *((const int32*)value); }
		),
		std::make_shared<TField<float>>("f32"sv, 
			(IField::GetValueFn)[](const void* obj) -> const void* { return &(((SimpleStruct*)obj)->f32); },
			(IField::SetValueFn)[](void* obj, const void* value) -> void { ((SimpleStruct*)obj)->f32 = *((const float*)value); }
		),
		std::make_shared<TField<String>>("Name"sv, 
			(IField::GetValueFn)[](const void* obj) -> const void* { return &(((SimpleStruct*)obj)->Name); },
			(IField::SetValueFn)[](void* obj, const void* value) -> void { ((SimpleStruct*)obj)->Name = *((const String*)value); }
		)
	};

	template<>
	const Vector<std::shared_ptr<IField>> ComplexType<ComplexClass>::Fields = {
		std::make_shared<TField<int32>>("Int"sv, 
			(IField::GetValueFn)[](const void* obj) -> const void* { return &(((ComplexClass*)obj)->m_Int); },
			(IField::SetValueFn)[](void* obj, const void* value) -> void { ((ComplexClass*)obj)->m_Int = *((const int32*)value); }
		),
		std::make_shared<TField<String>>("String"sv, 
			(IField::GetValueFn)[](const void* obj) -> const void* { return &(((ComplexClass*)obj)->m_String); },
			(IField::SetValueFn)[](void* obj, const void* value) -> void { ((ComplexClass*)obj)->m_String = *((const String*)value); }
		),
		std::make_shared<TField<float>>("Float"sv, 
			(IField::GetValueFn)[](const void* obj) -> const void* { return &(((ComplexClass*)obj)->m_Float); },
			(IField::SetValueFn)[](void* obj, const void* value) -> void { ((ComplexClass*)obj)->m_Float = *((const float*)value); }
		),
		std::make_shared<TField<Vector<SimpleStruct>>>("Vector"sv, 
			(IField::GetValueFn)[](const void* obj) -> const void* { return &(((ComplexClass*)obj)->m_Vector); },
			(IField::SetValueFn)[](void* obj, const void* value) -> void { ((ComplexClass*)obj)->m_Vector = *((const Vector<SimpleStruct>*)value); }
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
}