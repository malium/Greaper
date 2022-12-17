/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#include "../Reflection/BaseType.h"
#include "../Reflection/ContainerType.h"

namespace greaper::refl
{
	template<class T>
	struct GetCategoryType
	{
		using Type =
			std::conditional_t<std::is_base_of_v<BaseType<T>, PlainType<T>>, PlainType<T>,
			std::conditional_t<std::is_base_of_v<BaseType<T>, ContainerType<T>>, ContainerType<T>,
			std::conditional_t<std::is_base_of_v<BaseType<T>, ComplexType<T>>, ComplexType<T>,
			std::conditional_t<std::is_base_of_v<BaseType<T>, DataType<T>>, DataType<T>,
			void>>>>;
	};
}