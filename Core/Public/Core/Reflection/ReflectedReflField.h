/***********************************************************************************
*   Copyright 2021 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_REFLECTION_REFLECTED_REFERENCE_FIELD_H
#define CORE_REFLECTION_REFLECTED_REFERENCE_FIELD_H 1

#include "../CorePrerequisites.h"
#include "ReflectedField.h"

namespace greaper
{
	struct IReflectedReferenceField : IReflectedField
	{
		virtual ~IReflectedReferenceField() = default;


	};
}

#endif /* CORE_REFLECTION_REFLECTED_REFERENCE_FIELD_H */