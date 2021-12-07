/***********************************************************************************
*   Copyright 2021 Marcos S�nchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_I_TIME_MANAGER_H
#define CORE_I_TIME_MANAGER_H 1

#include "Interface.h"

namespace greaper
{
	struct TimeManagerConfig
	{
		uint32 MaxUpdatesPerSecond = 200;
		float FixedDeltaSeconds = 0.02f;
	};

	class ITimeManager : public IInterface
	{
	public:
		static constexpr Uuid InterfaceUUID = Uuid{ 0x7646A5C7, 0x81254E1C, 0x80D21DEE, 0x3ADAAAA4 };
		static constexpr StringView InterfaceName = "TimeManager"sv;

		virtual ~ITimeManager()noexcept = default;

		virtual Timepoint_t GetStartTime()const = 0;

		virtual Timepoint_t GetLastUpdateTime()const = 0;

		virtual Timepoint_t GetLastFixedUpdateTime()const = 0;

		virtual uint64 GetFrameNum()const = 0;

		virtual float GetUpdateDelta()const = 0;

		virtual void SetMaxUpdateRate(uint32 updatesPerSecond) = 0;

		virtual uint32 GetMaxUpdateRate()const = 0;

		virtual void SetFixedDelta(float deltaSeconds) = 0;

		virtual float GetFixedDelta()const = 0;
	};
}

#endif /* CORE_I_TIME_MANAGER_H */