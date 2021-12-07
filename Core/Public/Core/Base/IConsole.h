/***********************************************************************************
*   Copyright 2021 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_I_CONSOLE_H
#define CORE_I_CONSOLE_H 1

#include "../Memory.h"
#include "../Concurrency.h"
#include "../Enumeration.h"
#include "../Event.h"

namespace greaper
{
    ENUMERATION(ConsoleType, VIRTUAL, EXTERNAL);

    class IConsole
    {
    public:
        using ConsoleEvt_t = Event<const String&>;

        virtual ConsoleType_t GetConsoleType()const = 0;

        virtual void WriteToConsole(const String& msg) = 0;
        
        virtual TAsyncOp<String> ReadFromConsole() = 0;

        virtual ConsoleEvt_t*const GetConsoleEvent() = 0;

        virtual void SetCursorPosition(std::pair<int16, int16> position) = 0;

        virtual std::pair<int16, int16> GetCursorPosition() = 0;
    };
}

#endif /* CORE_I_CONSOLE_H */