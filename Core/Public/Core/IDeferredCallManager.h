/***********************************************************************************
*   Copyright 2021 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_I_DEFERRED_CALL_MANAGER_H
#define CORE_I_DEFERRED_CALL_MANAGER_H 1

#include "Interface.h"
#include "Enumeration.h"
#include <functional>

namespace greaper
{
    ENUMERATION(DeferredUpdate, PreUpdate, Update, PostUpdate, Fixed);

    class IDeferredCallManager : public TInterface<IDeferredCallManager>
    {
    public:
        static constexpr Uuid InterfaceUUID = Uuid{ 0x07C76D1A, 0xF08C4F0B, 0x8870B912, 0xE75E481E };
        static constexpr StringView InterfaceName = StringView{ "DeferredCallManager" };

        virtual void DelayCall(std::function<void()> call, uint32 updatesToWait = 0, DeferredUpdate_t updateWhen = DeferredUpdate_t::Update) = 0;

        virtual void DelayCallTime(std::function<void()> call, Duration_t timeToWait = Duration_t{0}, DeferredUpdate_t updateWhen = DeferredUpdate_t::Update) = 0;
    };
}

#endif /* CORE_I_DEFERRED_CALL_MANAGER_H */