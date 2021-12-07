/***********************************************************************************
*   Copyright 2021 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_PROPERTY_VALIDATOR_H
#define CORE_PROPERTY_VALIDATOR_H 1

#include "../CorePrerequisites.h"

namespace greaper
{
	/// @brief Property Validator types included with GreaperCore
	namespace EPropertyValidatorType
	{
		enum Type
		{
			Invalid = -1,
			None,
			Bounded,
			Selected
		};
	}
	
	/** 
	 * @brief Generic Property Validator, you can extend this class in order
	 * to provide your own value validation methods.
	 * 
	 * @tparam T Base type of the property, never specialize it as a unique value,
	 * follow the examples like PropertyValidatorNone or PropertyValidatorBounded
	 * 
	 * @param ValidatorType You must provide a unique identifier when extending this
	 * class, so code outside TProperty can know which validator is being in use.
	*/
	template<class T>
	class TPropertyValidator
	{
	public:
		inline static constexpr int32 ValidatorType = EPropertyValidatorType::Invalid;
		virtual bool Validate(const T& value, T* validated)const noexcept = 0;
		virtual ~TPropertyValidator() noexcept = default;
	};

	template<class T>
	class PropertyValidatorNone : public TPropertyValidator<T>
	{
	public:
		inline static constexpr int32 ValidatorType = EPropertyValidatorType::None;
		bool Validate(const T& value, T* validated)const noexcept override
		{
			*validated = value;
			return true;
		}
	};
	template<class T>
	class PropertyValidatorBounded : public TPropertyValidator<T>
	{
		T m_Min;
		T m_Max;

	public:
		inline static constexpr int32 ValidatorType = EPropertyValidatorType::Bounded;
		PropertyValidatorBounded(T min, T max) noexcept
			:m_Min(std::move(min))
			, m_Max(std::move(max))
		{

		}
		bool Validate(const T& value, T* validated)const noexcept override
		{
			if (value >= m_Min)
			{
				if (value <= m_Max)
				{
					*validated = value;
				}
				else
				{
					*validated = m_Min;
				}
			}
			else
			{
				*validated = m_Max;
			}
			return true;
		}
	};

	template<class T>
	class PropertyValidatorSelected : TPropertyValidator<T>
	{
		Vector<T> m_ValidValues;

	public:
		inline static constexpr int32 ValidatorType = EPropertyValidatorType::Selected;
		PropertyValidatorSelected(Vector<T> validValues) noexcept
			:m_ValidValues(std::move(validValues))
		{

		}

		bool Validate(const T& value, T* validated)const noexcept override
		{
			*validated = value;
			for (const auto& valid : m_ValidValues)
			{
				if (valid == value)
				{
					return true;
				}
			}
			return false;
		}
	};
}

#endif /* CORE_PROPERTY_VALIDATOR_H */