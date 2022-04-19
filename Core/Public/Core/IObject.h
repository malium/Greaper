/***********************************************************************************
*   Copyright 2021 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_I_OBJECT_H
#define CORE_I_OBJECT_H 1

#include "Uuid.h"
#include "Interface.h"

namespace greaper
{
	class IObject
	{
		Uuid m_UUID;
		String m_Name;
		IInterface* m_Handler;

		friend class IObjectFactory;
		template<class TObject> friend class TObjectFactory;

	public:
		static constexpr Uuid InvalidUUID = Uuid{ };

		virtual ~IObject() = default;

		virtual void OnHandlerTransfer(IInterface* nHandler) { /* No-op */ }

		INLINE IInterface* GetHandler() noexcept { return m_Handler; }

		INLINE const String& GetName() const noexcept { return m_Name; }

		INLINE const Uuid& GetUUID() const noexcept { return m_UUID; }

		virtual const Uuid& GetObjectTypeUUID() const noexcept = 0;
	};
}

#endif /* CORE_I_OBJECT_H */